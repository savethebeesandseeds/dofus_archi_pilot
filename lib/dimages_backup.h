#include "dconfig.h"

enum dcolors_t {
    dRED, 
    dGREEN,
    dBLUE
};

/* Function to read a BMP file and return its pixel data and dimensions */
BYTE* LoadBitmapFile(const char *filename, BITMAPINFOHEADER *bitmapInfoHeader) {
    FILE *filePtr; /* file pointer */
    BITMAPFILEHEADER bitmapFileHeader; /* bitmap file header */
    BYTE *bitmapImage; /* bitmap image data */
    int imageIdx = 0; /* image index counter */
    unsigned char tempRGB; /* temporary storage for swapping RGB values */

    /* Open filename in read binary mode */
    filePtr = fopen(filename, "rb");
    if (filePtr == NULL) {
        return NULL;
    }

    /* Read the bitmap file header */
    fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);

    /* Verify that this is a .BMP file by checking bitmap id */
    if (bitmapFileHeader.bfType != 0x4D42) {
        fclose(filePtr);
        return NULL;
    }

    /* Read the bitmap info header */
    fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);

    /* Move file pointer to the beginning of bitmap data */
    fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

    /* Allocate memory for bitmap image data */
    bitmapImage = (BYTE*)malloc(bitmapInfoHeader->biSizeImage);

    /* Verify memory allocation */
    if (!bitmapImage) {
        free(bitmapImage);
        fclose(filePtr);
        return NULL;
    }

    /* Read in the bitmap image data */
    fread(bitmapImage, bitmapInfoHeader->biSizeImage, 1, filePtr);

    /* Swap the R and B values to get RGB since the bitmap color format is in BGR */
    for (imageIdx = 0; imageIdx < bitmapInfoHeader->biSizeImage; imageIdx += 3) {
        tempRGB = bitmapImage[imageIdx];
        bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
        bitmapImage[imageIdx + 2] = tempRGB;
    }

    /* Close the file and return the bitmap image data */
    fclose(filePtr);
    return bitmapImage;
}

/* Function to compute the combined RGB histogram of an .bmp image */
/* `resolution` specifies the quantization levels per channel */
void ComputeCombinedRGBHistogram(BYTE *imageData, int width, int height, long *histogram, int resolution) {
    int histogramSize = resolution * resolution * resolution; /* Calculate histogram size based on resolution */
    
    /* Initialize histogram */
    for (int i = 0; i < histogramSize; i++) {
        histogram[i] = 0;
    }

    int quantizationFactor = 256 / resolution; /* Calculate quantization factor */

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = (y * width + x) * 3;
            /* Quantize each color channel */
            BYTE red = imageData[index + 2] / quantizationFactor;
            BYTE green = imageData[index + 1] / quantizationFactor;
            BYTE blue = imageData[index] / quantizationFactor;

            /* Combine the quantized values into a single index */
            /* Adjust bit shifts based on resolution */
            int combinedIndex = (red << (2 * (int)log2(resolution))) + (green << (int)log2(resolution)) + blue;

            histogram[combinedIndex]++;
        }
    }
}

/* Function to compare two RGB histograms based on non-zero bin matches */
int CompareCombinedRGBHistogramsNonZeroMatch(long *histogram_A, long *histogram_B) {
    int matchCount = 0;
    for (int i = 1; i < config_histagram_size(); i++) {
        if (histogram_A[i] > 0 && histogram_B[i] > 0) matchCount++;
    }
    return matchCount;
}

/* Function to compare two RGB histograms based on non-zero bin matches */
int CompareCombinedRGBHistogramsAoverB(long *histogram_A, long *histogram_B) {
    long min_v = config_histogram_min_val();
    long max_v = config_histogram_max_val();
    int matchCount = 0;
    for (int i = 1; i < config_histagram_size(); i++) { /* starts at 1 to not include absolite black, that is use as mask*/
        if(histogram_B[i] >= min_v && histogram_B[i] <= max_v) {
            if (histogram_A[i] <= histogram_B[i]) {
                matchCount += 1;
            }
        }
    }
    return matchCount;
}


/* Compute histogram from filePath */
int compute_fileImage_histogram(const char* filePath, long *histogram) {
    BITMAPINFOHEADER bitmapInfoHeader;
    BYTE *imageData = NULL;
    #pragma omp critical
    {
        imageData = LoadBitmapFile(filePath, &bitmapInfoHeader);
    }
    if (imageData == NULL) {
        log_error("Error loading image [%s] to compute histogram: \n\t%s\n", filePath, win_error_message());
        free(imageData);
        return -1;
    }

    ComputeCombinedRGBHistogram(imageData, bitmapInfoHeader.biWidth, bitmapInfoHeader.biHeight, histogram, config_histagram_resolution());
    free(imageData);

    return 0;
}