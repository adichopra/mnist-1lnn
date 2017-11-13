/**
 * @file mnist-stats.h
 * @brief Utitlies for displaying processing details in the terminal
 * @author Matt Lind
 * @date July 2015
 */

#include <stdio.h>

/**
 * @brief Outputs image recognition progress and error count
 * @param imgCount Number of images already processed
 * @param errCount Number of images that were NOT correctly identified
 * @param y Row of terminal screen
 * @param x Column of terminal screen
 */

void displayProgress(int imgCount, int errCount, int y, int x);
