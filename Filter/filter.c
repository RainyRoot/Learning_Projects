#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "helpers.h"

int main(int argc, char *argv[]){
    // Get filter flag and check validity
    char *filters = "bgrs";
    char filter = getopt(argc, argv, filters);
    if (filter == '?'){
        printf("Invalid filter.\n");
        return 1;
        }
    // Ensure only one filter
    if (getopt(argc, argv, filters) != -1){
        printf("Only one filter allowed.\n");
        return 2;
    }
    // Ensure proper usage
    if (argc != optind + 2){
        printf("Usage: ./filter [flag] infile outfile\n");
        return 3;
    }
    // Read and manage filenames
    char *infile = argv[optind]; //Read infile bit for bit
    char *outfile = argv[optind + 1]; //Print infile bit for bit
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL){
        printf("Could not open %s.\n", infile);
        return 4;
    }
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL){
        fclose(inptr);
        printf("Could not create %s.\n", outfile);
        return 5;
    }
    // Read infile's BITMAP-Header
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);
    // Ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || bi.biBitCount != 24 || bi.biCompression != 0){
        fclose(outptr);
        fclose(inptr);
        printf("Unsupported file format.\n");
        return 6;
    }
    // Get image's dimensions
    int height = abs(bi.biHeight);
    int width = bi.biWidth;
    // Allocate memory for image
    RGBTRIPLE(*image)[width] = malloc(height * sizeof(RGBTRIPLE[width]));
    if (image == NULL) {
        printf("Not enough memory to store image.\n");
        fclose(outptr);
        fclose(inptr);
        return 7;
    }
    // Determine padding for scanlines
    int padding = (4 - (width * sizeof(RGBTRIPLE)) % 4) % 4;
    for (int i = 0; i < height; i++){
        fread(image[i], sizeof(RGBTRIPLE), width, inptr);
        fseek(inptr, padding, SEEK_CUR);
    }
    // Filter image
    switch (filter){
        case 'b': blur(height, width, image);
            break;
        case 'g': grayscale(height, width, image);
            break;
        case 'r': reflect(height, width, image);
            break;
        case 's': sepia(height, width, image);
            break;
    }
    // Write outfile BITMAP-Header
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);
    // Write new pixels to outfile
    for (int i = 0; i < height; i++){
        // Write row to outfile
        fwrite(image[i], sizeof(RGBTRIPLE), width, outptr);
        // Write padding at end of row
        for (int k = 0; k < padding; k++){
            fputc(0x00, outptr);
        }
    }
    free(image);
    fclose(inptr);
    fclose(outptr);
    return 0;
}
