#include <stdio.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    // Check for correct usage
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    // Open the forensic image
    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }
    uint8_t buffer[512];
    int jpegCount = 0;
    FILE *jpeg = NULL;

    // Read 512 bytes at a time until end of file
    while (fread(buffer, sizeof(uint8_t), 512, file) == 512)
    {
        // Check if the first few bytes match the JPEG signature
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && buffer[3] >= 0xe0 && buffer[3] <= 0xef)
        {
            if (jpeg != NULL)
            {
                fclose(jpeg);
            }
            // Create a new JPEG file
            char filename[32]; // Ensure enough space for the filename
            sprintf(filename, "%03i.jpg", jpegCount);
            jpeg = fopen(filename, "w");

            // Check if the new file can be created
            if (jpeg == NULL)
            {
                printf("Could not create file.\n");
                fclose(file);
                return 1;
            }

            jpegCount++;
        }
        // Write the buffer to the JPEG file if it's open
        if (jpeg != NULL)
        {
            fwrite(buffer, sizeof(uint8_t), 512, jpeg);
        }
    }
    if (jpeg != NULL)
    {
        fclose(jpeg);
    }
    // close forensic image
    fclose(file);

    return 0;
}
