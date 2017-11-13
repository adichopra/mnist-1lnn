/**
 * @file 1lnn.c
 * @brief Machine learning functionality for a 1-layer neural network
 * @author Matt Lind
 * @date July 2015
 */

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "mnist-utils.h"
#include "1lnn.h"
#include "vec_mul.h"

/**
 * @details The output prediction is derived by simply sorting all output values
 * and using the index (=0-9 number) of the highest value as the prediction.
 */

int getLayerPrediction(Layer *l) {
    float maxOut = 0;
    int maxInd = 0;
    for (int i = 0; i < NUMBER_OF_OUTPUT_CELLS; i += 1) {
        if (l->cell[i].output > maxOut) {
            maxOut = l->cell[i].output;
            maxInd = i;
        }
    }
    return maxInd;
}

/**
 * @details Creates an input vector of length NUMBER_OF_INPUT_CELLS
 * of a given MNIST image, setting input vector cells to [0,1]
 * based on the pixels of the image.
 * Scalar pixel intensity [=grey-scale] is ignored, only 0 or 1 [=black-white].
 */

void setCellInput(Cell *c, MNIST_Image *img) {
    for (int i = 0; i < NUMBER_OF_INPUT_CELLS; i += 1) {
        c->input[i] = img->pixel[i] ? 1 : 0;
    }
}

float dotProduct(float *a, float *b, int length) {
    float result[length];
    vec_mul_asm(length, result, a, b);
    float output = 0;
    for (int i = 0; i < length; i += 1) {
        output += result[i];
    }
    return output;
}

/**
 * @details Calculates a cell's output by suming all input-weight-products
 * and normalizes to [0-1].
 */

float calcCellOutput(Cell *c) {
    c->output = dotProduct(c->input, c->weight, NUMBER_OF_INPUT_CELLS);
    c->output /= NUMBER_OF_INPUT_CELLS;             // normalize output (0-1)
    return c->output;
}

/**
 * @details Returns the difference between a target value and the cell's ouput
 */

float getCellError(Cell *c, int target) {
    float err = target - c->output;
    return err;
}


/**
 * @details Performs the testing of the trained network
 * Same as training a cell, but without updating weights (learning)
 */

void testCell(Cell *c, MNIST_Image *img) {
    setCellInput(c, img);
    calcCellOutput(c);
    
}

tMax testCellPipelined(Cell *c, MNIST_Image *img, int index, tMax max) {
    setCellInput(c, img);
    float output = calcCellOutput(c);
    if (output > max.val) {
        max = (tMax) {.val = output, .idx = index};
    }
    return max;
}

void printWeights(Layer *l){
    for (int i = 0; i < NUMBER_OF_OUTPUT_CELLS; i += 1) {
        printf("CELL #%d:\n[", i);
        for (int j = 0; j < NUMBER_OF_INPUT_CELLS; j+= 1) {
            printf("%f, ", l->cell[i].weight[j]);
        }
        printf("]\n");
    }
}
