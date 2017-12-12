#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "mnist.h"
#include "data.h"
#include "mmio.h"
#include "encoding.h"
#include "nic.h"

uint64_t out_packets[MNIST_MAX_TESTING_IMAGES][PACKET_WORDS];
int total_req = 0, total_comp = 0;
char inflight[MNIST_MAX_TESTING_IMAGES];

uint64_t in_packets[MNIST_MAX_TESTING_IMAGES][PACKET_WORDS];
int total_req2 = 0, total_comp2 = 0;
char inflight2[MNIST_MAX_TESTING_IMAGES];

MNIST_Image getImage(int i){
    return imgs[i];
}

static void fill_packet(uint64_t *packet, uint64_t srcmac, uint64_t dstmac, int id) {
    packet[0] = dstmac << 16;
    packet[1] = srcmac | (0x1008L << 48);
    packet[2] = id;

    MNIST_Image img = getImage(id);
    tMax max = (tMax) {.val = 0, .idx = 0};

    Message msg = (Message) {.img = img, .max = max};

    memcpy(packet + 24, &msg, sizeof msg);
}

static void process_loop(void) {
    uint16_t counts, send_req, send_comp;
    static int req_id = 0, comp_id = 0;

    counts = nic_counts();
    send_req  = (counts >> NIC_COUNT_SEND_REQ)  & 0xf;
    send_comp = (counts >> NIC_COUNT_SEND_COMP) & 0xf;

    for (int i = 0; i < send_comp; i++) {
        nic_complete_send();
        // printf("nic_complete_send [i=%d, comp_id=%d, send_req=%d, send_comp=%d]\n", i, comp_id, send_req, send_comp);
        inflight[comp_id] = 0;
        comp_id = (comp_id + 1) % MNIST_MAX_TESTING_IMAGES;
        total_comp++;
    }

    for (int i = 0; i < send_req; i++) {
        if (inflight[req_id])
            break;
        nic_post_send((uint64_t) out_packets[req_id], PACKET_WORDS * 8);
        // printf("nic_post_send [i=%d, req_id=%d, send_req=%d, send_comp=%d]\n", i, req_id, send_req, send_comp);
        inflight[req_id] = 1;
        req_id = (req_id + 1) % MNIST_MAX_TESTING_IMAGES;
        total_req++;
    }
}

static inline void process_loop2(void) {
    uint16_t counts, recv_req, recv_comp;
    static int req_id = 0, comp_id = 0;
    int len;

    counts = nic_counts();
    recv_req  = (counts >> NIC_COUNT_RECV_REQ)  & 0xf;
    recv_comp = (counts >> NIC_COUNT_RECV_COMP) & 0xf;

    for (int i = 0; i < recv_comp; i++) {
        len = nic_complete_recv();
        // printf("nic_complete_recv [i=%d, comp_id=%d, recv_req=%d, recv_comp=%d]\n", i, comp_id, recv_req, recv_comp);
        if (len != PACKET_WORDS * sizeof(uint64_t)) {
            printf("Incorrectly sized packet\n");
            abort();
        }

        // printf("Result = %d\n", ((Message*) (in_packets[comp_id] + 24))->max.idx);

        inflight2[comp_id] = 0;
        comp_id = (comp_id + 1) % MNIST_MAX_TESTING_IMAGES;
        total_comp2++;
    }

    for (int i = 0; i < recv_req; i++) {
        if (inflight2[req_id])
            break;
        nic_post_recv((uint64_t) in_packets[req_id]);
        // printf("nic_post_recv [i=%d, req_id=%d, recv_req=%d, recv_comp=%d]\n", i, req_id, recv_req, recv_comp);

        inflight2[req_id] = 1;
        req_id = (req_id + 1) % MNIST_MAX_TESTING_IMAGES;
        total_req2++;
    }
}

int main(int argc, const char * argv[]) {
    uint64_t srcmac = nic_macaddr();
    uint64_t dstmac = NEXT_MACADDR;
    uint64_t cycle;
    int counts, comp;

    printf("BEGIN FIRSTNODE\n");

    memset(inflight, 0, MNIST_MAX_TESTING_IMAGES);
    for (int i = 0; i < MNIST_MAX_TESTING_IMAGES; i++) {
        fill_packet(out_packets[i], srcmac, dstmac, i);
    }

    asm volatile ("fence");

    do {
        cycle = rdcycle();
    } while (cycle < START_CYCLE);

    // printf("Start MNIST\n");


    do {
        // if (cycle % 1000000 == 0) {
        //     printf("%d sent, process_loop again\n", total_comp);
        // }
        if (total_comp < MNIST_MAX_TESTING_IMAGES) {
            printf("%d sent, process_loop again\n", total_comp);
            process_loop();
        }
        printf("%d recvd, process_loop2 again\n", total_comp2);
        process_loop2();
        cycle = rdcycle();
    } while (total_comp2 < MNIST_MAX_TESTING_IMAGES);

    printf("cycles: %lu, completed: %d\n",
            cycle, total_comp);

    while (total_comp < total_req || total_comp2 < total_req2) {
        // printf("finish_comp\n");
        counts = nic_counts();
        comp = (counts >> NIC_COUNT_SEND_COMP) & 0xf;

        for (int i = 0; i < comp; i++) {
            // printf("finish_comp #%d\n", i);
            nic_complete_send();
            total_comp++;
        }
    }

    printf("2: cycles: %lu, completed: %d\n",
            cycle, total_comp);


    return 0;
}