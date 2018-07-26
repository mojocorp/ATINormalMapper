// This program converts a height map into a Normal map for bumpmapping.
// It assumes the height field is in the red channel of a 24 or 32 bit
// targa image.

#include <math.h>
#include <stdio.h>
#include <windows.h>
#include <NativeDialogs.h>

// Targa header info
#pragma pack(push)
#pragma pack(1) // dont pad the following struct

typedef struct _TGAHeaderInfo
{
    BYTE idlen;    // length of optional identification sequence
    BYTE cmtype;   // indicates whether a palette is present
    BYTE imtype;   // image data type (e.g., uncompressed RGB)
    WORD cmorg;    // first palette index, if present
    WORD cmcnt;    // number of palette entries, if present
    BYTE cmsize;   // number of bits per palette entry
    WORD imxorg;   // horiz pixel coordinate of lower left of image
    WORD imyorg;   // vert pixel coordinate of lower left of image
    WORD imwidth;  // image width in pixels
    WORD imheight; // image height in pixels
    BYTE imdepth;  // image color depth (bits per pixel)
    BYTE imdesc;   // image attribute flags
} TGAHeaderInfo;

#pragma pack(pop)

typedef struct _pixel
{
    BYTE red;
    BYTE blue;
    BYTE green;
    BYTE alpha;
} pixel;

int gWidth, gHeight;

void
WritePixel(pixel* image, pixel* pix, int x, int y)
{
    *(image + gWidth * y + x) = *pix;
}

void
ReadPixel(pixel* image, pixel* pix, int x, int y)
{
    *pix = *(image + gWidth * y + x);
}

BYTE
PackFloatInByte(float in)
{
    return (BYTE)((in + 1.0f) / 2.0f * 255.0f);
}

int WINAPI
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
    char buff[256 + _MAX_PATH];
    TGAHeaderInfo TGAHeader;
    FILE *finput, *foutput;
    DWORD bytesRead;
    int x, y;
    unsigned char* descBytes;
    pixel pix, *srcImage, *dstImage;
    float dX, dY, nX, nY, nZ, oolen;

    // Loop until the user cancels the open dialog
    while (TRUE) {
        // Either take the input and output filename from the command line
        // or grab it from the user.
        char inFilename[_MAX_PATH];
        inFilename[0] = '\0';
        char outFilename[_MAX_PATH];
        if (!GetOpenFileName(
              "TGAtoDOT3 Open File", "Targa(*.TGA)\0*.TGA\0All(*.*)\0*.*\0", inFilename)) {
            return 0;
        }
        strcpy(outFilename, inFilename);
        char* dot = strrchr(outFilename, '.');
        if (dot == NULL) {
            strcat(outFilename, "DOT3.tga");
        } else {
            strcpy(dot, "DOT3.tga");
        }

        // Put filename in here
        if ((finput = fopen(inFilename, "rb")) == NULL) {
            sprintf(buff, "Unable to open input TGA file: %s", inFilename);
            MessageBox(buff, "Error", NMB_OK | NMB_ICONERROR);
            continue;
        }

        // Open output file
        if ((foutput = fopen(outFilename, "wb")) == NULL) {
            sprintf(buff, "Unable to open output TGA file: %s", inFilename);
            MessageBox(buff, "Error", NMB_OK | NMB_ICONERROR);
            continue;
        }

        // Read TARGA header.
        if ((bytesRead = fread(&TGAHeader, sizeof(unsigned char), sizeof(TGAHeader), finput)) !=
            sizeof(TGAHeader)) {
            MessageBox("Bad Targa header", "Error", NMB_OK | NMB_ICONERROR);
            continue;
        }

        // Write to output file TARGA header
        if ((bytesRead = fwrite(&TGAHeader, sizeof(unsigned char), sizeof(TGAHeader), foutput)) !=
            sizeof(TGAHeader)) {
            MessageBox("Bad Targa header writing out", "Error", NMB_OK | NMB_ICONERROR);
            continue;
        }

        descBytes = (unsigned char*)malloc(sizeof(unsigned char) * TGAHeader.idlen);
        if (descBytes == NULL) {
            MessageBox("Unable to allocate enough memory.", "Error", NMB_OK | NMB_ICONERROR);
            continue;
        }

        // Steal descriptive bytes at end of header
        if ((bytesRead = fread(descBytes, sizeof(unsigned char), TGAHeader.idlen, finput)) !=
            TGAHeader.idlen) {
            MessageBox("Couldn't seek past Targa header", "Error", NMB_OK | NMB_ICONERROR);
            continue;
        }

        if ((bytesRead = fwrite(descBytes, sizeof(unsigned char), TGAHeader.idlen, foutput)) !=
            TGAHeader.idlen) {
            MessageBox("Bad Targa descriptive data writing out", "Error", NMB_OK | NMB_ICONERROR);
            continue;
        }

        gWidth = TGAHeader.imwidth;
        gHeight = TGAHeader.imheight;

        // allocate storage
        srcImage = (pixel*)malloc(sizeof(pixel) * gHeight * gWidth);
        dstImage = (pixel*)malloc(sizeof(pixel) * gHeight * gWidth);

        if ((srcImage == NULL) || (dstImage == NULL)) {
            MessageBox("Unable to allocate enough memory.", "Error", NMB_OK | NMB_ICONERROR);
            continue;
        }

        for (y = 0; y < gHeight; y++) {
            for (x = 0; x < gWidth; x++) {
                fread(&pix.blue, sizeof(BYTE), 1, finput);
                fread(&pix.green, sizeof(BYTE), 1, finput);
                fread(&pix.red, sizeof(BYTE), 1, finput);

                if (TGAHeader.imdepth == 32)
                    fread(&pix.alpha, sizeof(BYTE), 1, finput);
                else
                    pix.alpha = 0xcc;

                WritePixel(srcImage, &pix, x, y);
            }
        }

        for (y = 0; y < gHeight; y++) {
            for (x = 0; x < gWidth; x++) {
                // Do Y Sobel filter
                ReadPixel(srcImage, &pix, (x - 1 + gWidth) % gWidth, (y + 1) % gHeight);
                dY = ((float)pix.red) / 255.0f * -1.0f;

                ReadPixel(srcImage, &pix, x % gWidth, (y + 1) % gHeight);
                dY += ((float)pix.red) / 255.0f * -2.0f;

                ReadPixel(srcImage, &pix, (x + 1) % gWidth, (y + 1) % gHeight);
                dY += ((float)pix.red) / 255.0f * -1.0f;

                ReadPixel(srcImage, &pix, (x - 1 + gWidth) % gWidth, (y - 1 + gHeight) % gHeight);
                dY += ((float)pix.red) / 255.0f * 1.0f;

                ReadPixel(srcImage, &pix, x % gWidth, (y - 1 + gHeight) % gHeight);
                dY += ((float)pix.red) / 255.0f * 2.0f;

                ReadPixel(srcImage, &pix, (x + 1) % gWidth, (y - 1 + gHeight) % gHeight);
                dY += ((float)pix.red) / 255.0f * 1.0f;

                // Do X Sobel filter
                ReadPixel(srcImage, &pix, (x - 1 + gWidth) % gWidth, (y - 1 + gHeight) % gHeight);
                dX = ((float)pix.red) / 255.0f * -1.0f;

                ReadPixel(srcImage, &pix, (x - 1 + gWidth) % gWidth, y % gHeight);
                dX += ((float)pix.red) / 255.0f * -2.0f;

                ReadPixel(srcImage, &pix, (x - 1 + gWidth) % gWidth, (y + 1) % gHeight);
                dX += ((float)pix.red) / 255.0f * -1.0f;

                ReadPixel(srcImage, &pix, (x + 1) % gWidth, (y - 1 + gHeight) % gHeight);
                dX += ((float)pix.red) / 255.0f * 1.0f;

                ReadPixel(srcImage, &pix, (x + 1) % gWidth, y % gHeight);
                dX += ((float)pix.red) / 255.0f * 2.0f;

                ReadPixel(srcImage, &pix, (x + 1) % gWidth, (y + 1) % gHeight);
                dX += ((float)pix.red) / 255.0f * 1.0f;

                // Cross Product of components of gradient reduces to
                nX = -dX;
                nY = -dY;
                nZ = 1;

                // Normalize
                oolen = 1.0f / ((float)sqrt(nX * nX + nY * nY + nZ * nZ));
                nX *= oolen;
                nY *= oolen;
                nZ *= oolen;

                pix.red = (BYTE)PackFloatInByte(nX);
                pix.green = (BYTE)PackFloatInByte(nY);
                pix.blue = (BYTE)PackFloatInByte(nZ);

                WritePixel(dstImage, &pix, x, y);
            }
        }

        for (y = 0; y < gHeight; y++) {
            for (x = 0; x < gWidth; x++) {
                ReadPixel(dstImage, &pix, x, y);

                fwrite(&pix.blue, sizeof(BYTE), 1, foutput);
                fwrite(&pix.green, sizeof(BYTE), 1, foutput);
                fwrite(&pix.red, sizeof(BYTE), 1, foutput);

                if (TGAHeader.imdepth == 32)
                    fwrite(&pix.alpha, sizeof(BYTE), 1, foutput);
            }
        }

        free(srcImage);
        free(dstImage);
        free(descBytes);

        fclose(finput);  // close the input file
        fclose(foutput); // close the output file

        sprintf(buff, "Success! New TGA file: %s", outFilename);
        MessageBox(buff, "Success", NMB_OK);
    }
    return (0);
}
