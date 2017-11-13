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
 * @details Outputs image recognition progress and error count
 */

void displayProgress(int imgCount, int errCount){    
    float successRate = 1 - ((float)errCount/(float)(imgCount+1));
    printf("Result: Correct=%5d  Incorrect=%5d  Success-Rate=%5.2f%% \n",imgCount+1-errCount, errCount, successRate*100);
}
