#include "helpers.h"

typedef unsigned char BYTE;
// Grayscale the image by averging the Values of RGB
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            BYTE avg = (image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3;
            image[i][j].rgbtRed = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtBlue = avg;
        }
    }
}
// Sepia the image by using floating-point arithmetic
void sepia(int height, int width, RGBTRIPLE image[height][width]){
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            // Compute sepia values
            BYTE originalRed = image[i][j].rgbtRed;
            BYTE originalGreen = image[i][j].rgbtGreen;
            BYTE originalBlue = image[i][j].rgbtBlue;
            // Compute sepia values using floating-point arithmetic
            float sepiaRed = 0.393 * originalRed + 0.769 * originalGreen + 0.189 * originalBlue;
            float sepiaGreen = 0.349 * originalRed + 0.686 * originalGreen + 0.168 * originalBlue;
            float sepiaBlue = 0.272 * originalRed + 0.534 * originalGreen + 0.131 * originalBlue;
            // Clamp sepia values to the range [0, 255]
            image[i][j].rgbtRed = (BYTE) (sepiaRed > 255 ? 255 : (sepiaRed < 0 ? 0 : sepiaRed));
            image[i][j].rgbtGreen = (BYTE) (sepiaGreen > 255 ? 255 : (sepiaGreen < 0 ? 0 : sepiaGreen));
            image[i][j].rgbtBlue = (BYTE) (sepiaBlue > 255 ? 255 : (sepiaBlue < 0 ? 0 : sepiaBlue));
        }
    }
}
// Reflect image horizontally by swapping Pixels horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width / 2; j++){
            RGBTRIPLE tmp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = tmp;
        }
    }
}
// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width]){
    RGBTRIPLE copy[height][width];
    // Iterate over each pixel in the image
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            int sumRed = 0, sumGreen = 0, sumBlue = 0, count = 0;
            // Iterate over a 3x3 block centered at (i, j)
            for (int di = -1; di <= 1; di++){
                for (int dj = -1; dj <= 1; dj++){
                    int ni = i + di;
                    int nj = j + dj;
                    // Check if the neighboring pixel is within the image boundaries
                    if (ni >= 0 && ni < height && nj >= 0 && nj < width){
                        // Accumulate color values
                        sumRed += image[ni][nj].rgbtRed;
                        sumGreen += image[ni][nj].rgbtGreen;
                        sumBlue += image[ni][nj].rgbtBlue;
                        count++;
                    }
                }
            }
            // Calculate the average color values
            copy[i][j].rgbtRed = sumRed / count;
            copy[i][j].rgbtGreen = sumGreen / count;
            copy[i][j].rgbtBlue = sumBlue / count;
        }
    }
    // Copy the blurred image back to the original image
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            image[i][j] = copy[i][j];
        }
    }
}
