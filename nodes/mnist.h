#include <stdint.h>
#include <stdio.h>

#define MNIST_TESTING_SET_IMAGE_FILE_NAME "data/t10k-images-idx3-ubyte" // MNIST image testing file in the data folder
#define MNIST_MAX_TESTING_IMAGES 10000 // number of images+labels in the TEST file/s
#define MNIST_IMG_WIDTH 28  // image width in pixels
#define MNIST_IMG_HEIGHT 28 // image height in pixels
#define NUMBER_OF_INPUT_CELLS 784   ///< use 28*28 input cells (= number of pixels per MNIST image)

typedef struct MNIST_Image MNIST_Image;
typedef struct MNIST_ImageFileHeader MNIST_ImageFileHeader;
typedef uint8_t MNIST_Label;

typedef struct Cell Cell;
typedef struct Vector Vector;
typedef struct tMax tMax;

typedef struct Message Message;


struct MNIST_Image {
    uint8_t pixel[28 * 28];
};

struct Cell {
    float input [NUMBER_OF_INPUT_CELLS];
    float weight[NUMBER_OF_INPUT_CELLS];
    float output;
};


struct tMax {
    float val;
    uint8_t idx;
};

struct MNIST_ImageFileHeader {
    uint32_t magicNumber;
    uint32_t maxImages;
    uint32_t imgWidth;
    uint32_t imgHeight;
};

struct Message {
    MNIST_Image img;
    tMax max;
};
