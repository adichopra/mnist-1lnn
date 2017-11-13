/**
 * @file 1lnn.h
 * @brief Machine learning functionality for a 1-layer neural network
 * @author Matt Lind
 * @date July 2015
 */

#include <stdio.h>

#define NUMBER_OF_INPUT_CELLS 784   ///< use 28*28 input cells (= number of pixels per MNIST image)
#define NUMBER_OF_OUTPUT_CELLS 10   ///< use 10 output cells to model 10 digits (0-9)
#define LEARNING_RATE 0.05          ///< Incremental increase for changing connection weights

typedef struct Cell Cell;
typedef struct Layer Layer;
typedef struct Vector Vector;
typedef struct tMax tMax;

/**
 * @brief Core unit of the neural network (neuron and synapses)
 */

struct Cell {
    float input [NUMBER_OF_INPUT_CELLS];
    float weight[NUMBER_OF_INPUT_CELLS];
    float output;
    float bias;
};

/**
 * @brief The single (output) layer of this network (a layer is number cells)
 */

struct Layer {
    Cell cell[NUMBER_OF_OUTPUT_CELLS];
};

/**
 * @brief Data structure containing defined number of integer values (the output vector contains values for 0-9)
 */

struct Vector {
    int val[NUMBER_OF_OUTPUT_CELLS];
};

struct tMax{
    float val;
    int idx;
};

/**
 * @brief Returns the index of the cell with the hightest output
 * @param l A pointer to a NN layer
 */

int getLayerPrediction(Layer *l);

/**
 * @brief Sets a cell's input according to the pixels of a given MNIST image
 * @param c A pointer to a cell
 * @param img A pointer to an MNIST image
 */

void setCellInput(Cell *c, MNIST_Image *img);

/**
 * @brief Calculates a cell's output by suming all input-weight-products
 * @param c A cell of a NN layer
 */

float calcCellOutput(Cell *c);

/**
 * @brief Returns the difference between a target value and the cell's ouput
 * @param c The cell whose output is to be compared
 * @param target The desired value (= correct answer in supervised learning)
 */

float getCellError(Cell *c, int target);

/**
 * @brief Performs the testing of the trained network
 * @param c Pointer to the cell that is to be trained
 * @param img Pointer to the image that is to be processed
 * @param target Desired output value
 */

void testCell(Cell *c, MNIST_Image *img);

tMax testCellPipelined(Cell *c, MNIST_Image *img, int index, tMax max);

void printWeights(Layer *l);
