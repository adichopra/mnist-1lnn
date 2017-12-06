#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "mnist.h"
#include "data.h"
#include "nic.h"

uint64_t out_packets[MNIST_MAX_TESTING_IMAGES][PACKET_WORDS];
uint64_t in_packet[3];
char completed[MNIST_MAX_TESTING_IMAGES];
int total_comp = 0;
int total_req = 0;

// uint64_t nic_macaddr() {
//     return 0x200006d1200;
// }

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

    // tMax max2 = ((Message*) (packet + 24))->max;
    // printf("%d\n", max2.idx);
}

static inline void post_send(uint64_t addr, uint64_t len) {
    uint64_t request = ((len & 0x7fff) << 48) | (addr & 0xffffffffffffL);
    reg_write64(SIMPLENIC_SEND_REQ, request);
}

static void process_loop(void) {
    uint16_t counts, send_req, send_comp;
    static int req_id = 0;
    static int comp_id = 0;

    counts = reg_read16(SIMPLENIC_COUNTS);
    send_req  = counts & 0xf;
    send_comp = (counts >> 8) & 0xf;

    for (int i = 0; i < send_req && total_req < MNIST_MAX_TESTING_IMAGES; i++) {
        printf("sending %d\n", i);
        post_send((uint64_t) out_packets[req_id], PACKET_WORDS * 8);
        req_id++;
        total_req++;
    }

    for (int i = 0; i < send_comp && total_comp < MNIST_MAX_TESTING_IMAGES; i++) {
        reg_read16(SIMPLENIC_SEND_COMP);
        completed[comp_id] = 1;
        comp_id++;
        total_comp++;
    }
}

void sendImage(MNIST_Image img) {
    // TODO(adichopra): send from NIC
    printf("Sending data to %lx\n", NEXT_MACADDR);
    return;
}

void getResult() {
    // TODO(adichopra): send from NIC
    printf("Recving data from %lx\n", PREV_MACADDR);
    return;
}

int main(int argc, const char * argv[]) {
    printf("BEGIN FIRSTNODE\n");
    uint64_t srcmac = nic_macaddr();
    uint64_t dstmac = NEXT_MACADDR;

    memset(in_packet, 0, sizeof(uint64_t) * 3);
    memset(completed, 0, MNIST_MAX_TESTING_IMAGES);
    for (int i = 0; i < MNIST_MAX_TESTING_IMAGES; i++) {
        fill_packet(out_packets[i], srcmac, dstmac, i);
    }

    asm volatile ("fence");

    printf("Start MNIST\n");

    reg_write64(SIMPLENIC_RECV_REQ, (uint64_t) in_packet);

    while (total_comp < MNIST_MAX_TESTING_IMAGES)
        printf("process_loop again\n");
        process_loop();

    nic_recv(in_packet);

    printf("End MNIST\n");

    for (int i = 0; i < MNIST_MAX_TESTING_IMAGES; i++) {
        if (!completed[i])
            printf("Packet %d was not sent\n", i);
    }

    nic_recv(in_packet);


    // for (int imgCount = 0; imgCount < MNIST_MAX_TESTING_IMAGES; imgCount += 1){
    //     MNIST_Image img = getImage(imgCount);
    //     sendImage(img);
    // }
    getResult();
    return 0;
}