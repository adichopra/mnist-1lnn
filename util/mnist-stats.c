/**
 * @file mnist-stats.c
 * @brief Utitlies for displaying processing details in the terminal
 * @author Matt Lind
 * @date July 2015
 */

#include <string.h>

#include "mnist-utils.h"
#include "mnist-stats.h"

/**
 * @details Outputs reading progress while processing MNIST testing images
 */

void displayLoadingProgressTesting(int imgCount, int y, int x){
    
    float progress = (float)(imgCount+1)/(float)(MNIST_MAX_TESTING_IMAGES)*100;
        
    printf("2: TESTING:  Reading image No. %5d of %5d images [%d%%]\n                                  ",(imgCount+1),MNIST_MAX_TESTING_IMAGES,(int)progress);
    
}

/**
 * @details Outputs image recognition progress and error count
 */

void displayProgress(int imgCount, int errCount, int y, int x){    
    double successRate = 1 - ((double)errCount/(double)(imgCount+1));
    printf("Result: Correct=%5d  Incorrect=%5d  Success-Rate=%5.2f%% \n",imgCount+1-errCount, errCount, successRate*100);
}
