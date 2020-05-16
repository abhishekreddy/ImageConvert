/*
* ImageConvert.c
*
* Image Converter Tool for MIPI 8, 10, 12 and 14 bit to PLAIN16
*
* Copyright (C) 2020 Abhishek Reddy kondaveeti
*
* Contributor: Abhishek Reddy Kondaveeti <abhishek.kondaveeti@gmail.com>
*
*
* This file is licensed under the terms of the GNU General Public License
* version 2. This program is licensed "as is" without any warranty of any
* kind, whether express or implied.
*
*/

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <sys/stat.h>

const char* path = "TestPlain16.raw";

int GetBytesPeLine(int width, int bitWidth)
{
    int lineWidthInBytes= 0;

    switch(bitWidth)
    {
        case 8:
            lineWidthInBytes = width;
            break;
        case 10:
            lineWidthInBytes = width * 5 / 4;
            break;
        case 12:
            lineWidthInBytes = width * 3 / 2;
            break;
        case 14:
            lineWidthInBytes = width * 7 / 4;
            break;
        case 16:
            lineWidthInBytes = width * 2;
            break;
        default:
            printf("ImageConvert: Invalid Bit Width %d\n", bitWidth);
            return -1;
    }
    return lineWidthInBytes;
}

int ConvertMIPI10ToRaw16(FILE* pFile, int width, int height, int alignmentInBytes, uint16_t* pOutputBuffer)
{
    uint16_t readBytesPerLine             = GetBytesPeLine(width, 10);
    uint16_t readBytesPerLineWithPadding  = readBytesPerLine;
    uint8_t* pReadBytesPerLine            = NULL;
    int      lineNumber                   = 0;
    int      readBytes                    = 0;
    int      writtenPixels                = 0;
    uint16_t pixelValue                   = 0;
    uint16_t pixelSplitValue              = 0;
    uint16_t actualWrittenPixels          = 0;

    // In case of aignemnet requiremnet do padding adjustment
    if (0 < alignmentInBytes)
    {
        readBytesPerLineWithPadding = readBytesPerLine + (alignmentInBytes - (readBytesPerLine % alignmentInBytes));
    }

    printf("ImageConvert: Image Width %d Height %d BitsPerPixels %d PaddedWidthinBytes %d\n",
            width, height, 10, readBytesPerLineWithPadding);
    pReadBytesPerLine = (uint8_t*)(malloc(sizeof(uint8_t) * readBytesPerLineWithPadding));

    if (NULL != pReadBytesPerLine)
    {
        while (!feof(pFile) && (lineNumber < height))
        {
            if (readBytesPerLineWithPadding == fread(pReadBytesPerLine, 1, (readBytesPerLineWithPadding), pFile))
            {
                for (readBytes = 0; readBytes < readBytesPerLine; readBytes = readBytes + 5)
                {
                    pixelValue      = *(pReadBytesPerLine + readBytes);

                    pixelSplitValue = *(pReadBytesPerLine + readBytes + 4);
                    pOutputBuffer[writtenPixels++] = ((pixelValue << 2) | (pixelSplitValue & 0x3));

                    pixelValue                     = *(pReadBytesPerLine + readBytes + 1);
                    pOutputBuffer[writtenPixels++] = ((pixelValue << 2) | ((pixelSplitValue >> 2)& 0x3));

                    pixelValue                     = *(pReadBytesPerLine + readBytes + 2);
                    pOutputBuffer[writtenPixels++] = ((pixelValue << 2) | ((pixelSplitValue >> 4) & 0x3));

                    pixelValue                     = *(pReadBytesPerLine + readBytes + 3);
                    pOutputBuffer[writtenPixels++] = ((pixelValue << 2) | ((pixelSplitValue >> 6) & 0x3));
                }
                lineNumber++;
            }
        }
        free(pReadBytesPerLine);
    }
    else
    {
        printf("ImageConvert: Memory allocation Failed Unable To Convert Image\n!!!");
        return -1;
    }
    return 0;
}

int ConvertMIPI12ToRaw16(FILE* pFile, int width, int height, int alignmentInBytes, uint16_t* pOutputBuffer)
{
    uint16_t readBytesPerLine             = GetBytesPeLine(width, 12);
    uint16_t readBytesPerLineWithPadding  = readBytesPerLine;
    uint8_t* pReadBytesPerLine            = NULL;
    int      lineNumber                   = 0;
    int      readBytes                    = 0;
    int      writtenPixels                = 0;
    uint16_t pixelValue                   = 0;
    uint16_t pixelSplitValue              = 0;
    uint16_t actualWrittenPixels          = 0;

    // In case of aignemnet requiremnet do padding adjustment
    if (0 < alignmentInBytes)
    {
        readBytesPerLineWithPadding = readBytesPerLine + (alignmentInBytes - (readBytesPerLine % alignmentInBytes));
    }

    printf("ImageConvert: Image Width %d Height %d BitsPerPixels %d PaddedWidthinBytes %d\n",
            width, height, 12, readBytesPerLineWithPadding);
    pReadBytesPerLine = (uint8_t*)(malloc(sizeof(uint8_t) * readBytesPerLineWithPadding));

    if (NULL != pReadBytesPerLine)
    {
        while (!feof(pFile) && (lineNumber < height))
        {
            if (readBytesPerLineWithPadding == fread(pReadBytesPerLine, 1, (readBytesPerLineWithPadding), pFile))
            {
                for (readBytes = 0; readBytes < readBytesPerLine; readBytes = readBytes + 3)
                {
                    pixelValue      = *(pReadBytesPerLine + readBytes);
                    pixelSplitValue = *(pReadBytesPerLine + readBytes +2);
                    pOutputBuffer[writtenPixels++] = ((pixelValue << 4) | (pixelSplitValue & 0xF));

                    pixelValue                     = *(pReadBytesPerLine + readBytes + 1);
                    pOutputBuffer[writtenPixels++] = ((pixelValue << 4) | ((pixelSplitValue >> 4) & 0xF));
                }
                lineNumber++;
            }
        }
        free(pReadBytesPerLine);
    }
    else
    {
        printf("ImageConvert: Memory allocation Failed Unable To Convert Image\n!!!");
        return -1;
    }
    return 0;
}

int ConvertMIPI14ToRaw16(FILE* pFile, int width, int height, int alignmentInBytes, uint16_t* pOutputBuffer)
{
    uint16_t readBytesPerLine             = GetBytesPeLine(width, 14);
    uint16_t readBytesPerLineWithPadding  = readBytesPerLine;
    uint8_t* pReadBytesPerLine            = NULL;
    int      lineNumber                   = 0;
    int      readBytes                    = 0;
    int      writtenPixels                = 0;
    uint16_t pixelValue                   = 0;
    uint32_t pixelSplitValue              = 0;
    uint16_t actualWrittenPixels          = 0;

    // In case of aignemnet requiremnet do padding adjustment
    if (0 < alignmentInBytes)
    {
        readBytesPerLineWithPadding = readBytesPerLine + (alignmentInBytes - (readBytesPerLine % alignmentInBytes));
    }

    printf("ImageConvert: Image Width %d Height %d BitsPerPixels %d PaddedWidthinBytes %d\n",
            width, height, 14, readBytesPerLineWithPadding);
    pReadBytesPerLine = (uint8_t*)(malloc(sizeof(uint8_t) * readBytesPerLineWithPadding));

    if (NULL != pReadBytesPerLine)
    {
        while (!feof(pFile) && (lineNumber < height))
        {
            if (readBytesPerLineWithPadding == fread(pReadBytesPerLine, 1, (readBytesPerLineWithPadding), pFile))
            {
                for (readBytes = 0; readBytes < readBytesPerLine; readBytes = readBytes + 7)
                {
                    pixelValue      = *(pReadBytesPerLine + readBytes);
                    pixelSplitValue = 0;
                    pixelSplitValue = *(pReadBytesPerLine + readBytes + 4);
                    pixelSplitValue = (pixelSplitValue << 8) | *(pReadBytesPerLine + readBytes + 5);
                    pixelSplitValue = (pixelSplitValue << 8) | *(pReadBytesPerLine + readBytes + 6);
                    pOutputBuffer[writtenPixels++] = ((pixelValue << 6) | ((pixelSplitValue >> 18)  & 0x3F));

                    pixelValue                     = *(pReadBytesPerLine + readBytes + 1);
                    pOutputBuffer[writtenPixels++] = ((pixelValue << 6) | ((pixelSplitValue >> 12) & 0x3F));

                    pixelValue                     = *(pReadBytesPerLine + readBytes + 2);
                    pOutputBuffer[writtenPixels++] = ((pixelValue << 6) | ((pixelSplitValue >> 6) & 0x3F));

                    pixelValue                     = *(pReadBytesPerLine + readBytes + 3);
                    pOutputBuffer[writtenPixels++] = ((pixelValue << 6) | (pixelSplitValue & 0x3F));
                }
                lineNumber++;
            }
        }
        free(pReadBytesPerLine);
    }
    else
    {
        printf("ImageConvert: Memory allocation Failed Unable To Convert Image\n!!!");
        return -1;
    }
    return 0;
}

int ConvertMIPI8ToRaw16(FILE* pFile, int width, int height, int alignmentInBytes, uint16_t* pOutputBuffer)
{
    uint16_t readBytesPerLine             = GetBytesPeLine(width, 8);
    uint16_t readBytesPerLineWithPadding  = readBytesPerLine;
    uint8_t* pReadBytesPerLine            = NULL;
    int      lineNumber                   = 0;
    int      readBytes                    = 0;
    int      writtenPixels                = 0;
    uint16_t pixelValue                   = 0;
    uint16_t actualWrittenPixels          = 0;

    // In case of aignemnet requiremnet do padding adjustment
    if (0 < alignmentInBytes)
    {
        readBytesPerLineWithPadding = readBytesPerLine + (alignmentInBytes - (readBytesPerLine % alignmentInBytes));
    }

    printf("ImageConvert: Image Width %d Height %d BitsPerPixels %d PaddedWidthinBytes %d\n",
            width, height, 8, readBytesPerLineWithPadding);
    pReadBytesPerLine = (uint8_t*)(malloc(sizeof(uint8_t) * readBytesPerLineWithPadding));

    if (NULL != pReadBytesPerLine)
    {
        while (!feof(pFile) && (lineNumber < height))
        {
            if (readBytesPerLineWithPadding == fread(pReadBytesPerLine, 1, (readBytesPerLineWithPadding), pFile))
            {
                for (readBytes = 0; readBytes < readBytesPerLine; readBytes++)
                {
                    pixelValue                     = *(pReadBytesPerLine + readBytes);
                    pOutputBuffer[writtenPixels++] = pixelValue;
                }
                lineNumber++;
            }
        }
        free(pReadBytesPerLine);
    }
    else
    {
        printf("ImageConvert: Memory allocation Failed Unable To Convert Image\n!!!");
        return -1;
    }
    return 0;
}

int CheckInputFileValidity(const char* pInputFilepath, int bitwiidth, int alignmentinBytes, int width, int height)
{
    struct stat   fileStats;
    long long int expectedFileSize;
    int           lineWidthInBytes = 0;

    if (stat(pInputFilepath, &fileStats) == -1)
    {
        printf("ImageConvert: Error in Getting File Stats!!!!\n");
        return -1;
    }
    else
    {
        lineWidthInBytes = GetBytesPeLine(width, bitwiidth);

        if (0 < lineWidthInBytes)
        {
            if (0 < alignmentinBytes)
            {
                lineWidthInBytes += alignmentinBytes - (lineWidthInBytes % alignmentinBytes);
            }
            expectedFileSize = lineWidthInBytes * height;

            if (expectedFileSize > fileStats.st_size)
            {
                printf("ImageConvert: Error in Inpout File Size ..Minimum Expected File size %lld Bytes , Actaul File Size is %lld Bytes\n",
                       expectedFileSize, (long long)fileStats.st_size);
                return -1;
            }
            else
            {
                printf("ImageConvert: Input File size %lld is as expected!!!!\n",
                       (long long)fileStats.st_size);
            }
        }
        else
        {
            printf("ImageConvert: Getting Expected File size error!!!\n");
            return -1;
        }

        return 0;
    }
}

int main(int argc, char **argv)
{
    FILE*       pFile               = NULL;
    FILE*       pWriteFile          = NULL;
    int         width;
    int         height;
    int         bitWidth;
    int         alignmentInBytes    = 0;
    int         actualWrittenPixels = 0;
    struct stat sb;
    uint16_t*   pWriteBuffer        = NULL;


    if (NULL != argv[1])
    {
        printf("ImageConvert: Input MIPI Raw File %s\n", argv[1]);

        if (argc < 6)
        {
            printf("ImageConvert: Invalid Args ..Please follow the below format\n");
            printf("ImageConvert: <filename> <Width> <Height> <BitWidth> <alignmentInBytes>\n");
            return -1;
        }
        else
        {
            sscanf(argv[2], "%d", &width);
            sscanf(argv[3], "%d", &height);
            sscanf(argv[4], "%d", &bitWidth);
            sscanf(argv[5], "%d", &alignmentInBytes);

            if (0 > width)
            {
                printf("ImageConvert: Invalid Width %d\n", width);
                return -1;
            }

            if (0 > height)
            {
                printf("ImageConvert: Invalid Height %d\n", height);
                return -1;
            }

            if ((8 != bitWidth) && (10 != bitWidth) && (12 != bitWidth) && (14 != bitWidth) && (16 != bitWidth))
            {
                printf("ImageConvert: Invalid Bit Width %d, BitWidth can only be 8, 10, 12, 14 and 16\n", bitWidth);
                return -1;
            }

            if (16 == bitWidth)
            {
                printf("ImageConvert: Input Image is already in 16bit Format\n");
                return 0;
            }
        }


        pFile = fopen(argv[1], "rb");

        if (NULL == pFile)
        {
            printf("ImageConvert: Failed to open file %s\n", argv[1]);
            return -1;
        }
        else
        {
            if (0 > CheckInputFileValidity(argv[1], bitWidth, alignmentInBytes, width, height))
            {
                fclose(pFile);
                return -1;
            }

            pWriteFile = fopen(path, "wb");

            if (NULL == pWriteFile)
            {
                printf("ImageConvert: Failed to Open Write File!!!\n");
                fclose(pFile);
                return -1;
            }
            else
            {

                pWriteBuffer = (uint16_t*)(malloc(sizeof(uint16_t) * width * height));

                if (NULL != pWriteBuffer)
                {
                    switch (bitWidth)
                    {
                        case 8:
                            if (0 <= ConvertMIPI8ToRaw16(pFile, width, height, alignmentInBytes, pWriteBuffer))
                            {
                                actualWrittenPixels = fwrite(pWriteBuffer, sizeof(uint16_t), width * height, pWriteFile);
                            }
                            break;
                        case 10:
                            if (0 <= ConvertMIPI10ToRaw16(pFile, width, height, alignmentInBytes, pWriteBuffer))
                            {
                                actualWrittenPixels = fwrite(pWriteBuffer, sizeof(uint16_t), width * height, pWriteFile);
                            }
                            break;
                        case 12:
                            if (0 <= ConvertMIPI12ToRaw16(pFile, width, height, alignmentInBytes, pWriteBuffer))
                            {
                                actualWrittenPixels = fwrite(pWriteBuffer, sizeof(uint16_t), width * height, pWriteFile);
                            }
                            break;
                        case 14:
                            if (0 <= ConvertMIPI14ToRaw16(pFile, width, height, alignmentInBytes, pWriteBuffer))
                            {
                                actualWrittenPixels = fwrite(pWriteBuffer, sizeof(uint16_t), width * height, pWriteFile);
                            }
                            break;
                        default:
                            break;
                    }

                    fclose(pFile);
                    fclose(pWriteFile);
                    free(pWriteBuffer);
                    if (stat(path, &sb) == -1)
                    {
                        printf("ImageConvert: stat error");
                    }
                    else
                    {
                        printf("ImageConvert: Actual File size: %lld bytes\n",(long long) sb.st_size);
                    }
                }
                else
                {
                    printf("ImageConvert: Memory allocation Failed!!! Unable to Convert to Plain16\n");
                    fclose(pFile);
                    fclose(pWriteFile);
                    return -1;
                }
            }
        }
    }
    else
    {
        printf("Enter File Path!!!!\n");
    }
}

