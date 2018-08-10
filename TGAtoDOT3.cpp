// This program converts a height map into a Normal map for bumpmapping.
// It assumes the height field is in the red channel of a 24 or 32 bit
// targa image.
#ifdef _WIN32
#include <windows.h>
#endif

#include <math.h>
#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <NativeDialogs.h>
#include <TGAIO.h>

int gWidth, gHeight;

void
WritePixel(uint8* image, int bpp, const pixel* pix, int x, int y)
{
    const int idx = (x + y * gWidth) * (bpp / 8);
    if (bpp >= 8) {
        image[idx + 0] = pix->blue;
    }
    if (bpp >= 16) {
        image[idx + 1] = pix->green;
    }
    if (bpp >= 24) {
        image[idx + 2] = pix->red;
    }
    if (bpp >= 32) {
        image[idx + 3] = pix->alpha;
    }
}

uint8
PackFloatInByte(float in)
{
    return (uint8)((in + 1.0f) / 2.0f * 255.0f);
}

#ifdef _WIN32
int WINAPI
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
#else
int
main(int argc, char** argv)
#endif
{
    char buff[256 + 1024];
    FILE *finput, *foutput;

    pixel pix;

    // Loop until the user cancels the open dialog
    while (TRUE) {
        // Either take the input and output filename from the command line
        // or grab it from the user.
        char inFilename[1024];
        inFilename[0] = '\0';
        char outFilename[1024];
        if (!GetOpenFileName("TGAtoDOT3 Open File", "TGA\0*.TGA\0All\0*.*\0", inFilename)) {
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

        uint8* srcImage;
        int bpp;
        if (!TGAReadImage(finput, &gWidth, &gHeight, &bpp, &srcImage)) {
            MessageBox("Unable to read image data", "Error", NMB_OK | NMB_ICONERROR);
            fclose(finput);
            continue;
        }
        fclose(finput); // close the input file

        // allocate storage
        uint8* dstImage = (uint8*)malloc(gHeight * gWidth * (bpp / 8));

        if (dstImage == NULL) {
            MessageBox("Unable to allocate enough memory.", "Error", NMB_OK | NMB_ICONERROR);
            continue;
        }

        for (int y = 0; y < gHeight; y++) {
            for (int x = 0; x < gWidth; x++) {
                // Do Y Sobel filter
                TGAReadPixel(
                  srcImage, gWidth, bpp / 8, &pix, (x - 1 + gWidth) % gWidth, (y + 1) % gHeight);
                float dY = ((float)pix.red) / 255.0f * -1.0f;

                TGAReadPixel(srcImage, gWidth, bpp / 8, &pix, x % gWidth, (y + 1) % gHeight);
                dY += ((float)pix.red) / 255.0f * -2.0f;

                TGAReadPixel(srcImage, gWidth, bpp / 8, &pix, (x + 1) % gWidth, (y + 1) % gHeight);
                dY += ((float)pix.red) / 255.0f * -1.0f;

                TGAReadPixel(srcImage,
                             gWidth,
                             bpp / 8,
                             &pix,
                             (x - 1 + gWidth) % gWidth,
                             (y - 1 + gHeight) % gHeight);
                dY += ((float)pix.red) / 255.0f * 1.0f;

                TGAReadPixel(
                  srcImage, gWidth, bpp / 8, &pix, x % gWidth, (y - 1 + gHeight) % gHeight);
                dY += ((float)pix.red) / 255.0f * 2.0f;

                TGAReadPixel(
                  srcImage, gWidth, bpp / 8, &pix, (x + 1) % gWidth, (y - 1 + gHeight) % gHeight);
                dY += ((float)pix.red) / 255.0f * 1.0f;

                // Do X Sobel filter
                TGAReadPixel(srcImage,
                             gWidth,
                             bpp / 8,
                             &pix,
                             (x - 1 + gWidth) % gWidth,
                             (y - 1 + gHeight) % gHeight);
                float dX = ((float)pix.red) / 255.0f * -1.0f;

                TGAReadPixel(
                  srcImage, gWidth, bpp / 8, &pix, (x - 1 + gWidth) % gWidth, y % gHeight);
                dX += ((float)pix.red) / 255.0f * -2.0f;

                TGAReadPixel(
                  srcImage, gWidth, bpp / 8, &pix, (x - 1 + gWidth) % gWidth, (y + 1) % gHeight);
                dX += ((float)pix.red) / 255.0f * -1.0f;

                TGAReadPixel(
                  srcImage, gWidth, bpp / 8, &pix, (x + 1) % gWidth, (y - 1 + gHeight) % gHeight);
                dX += ((float)pix.red) / 255.0f * 1.0f;

                TGAReadPixel(srcImage, gWidth, bpp / 8, &pix, (x + 1) % gWidth, y % gHeight);
                dX += ((float)pix.red) / 255.0f * 2.0f;

                TGAReadPixel(srcImage, gWidth, bpp / 8, &pix, (x + 1) % gWidth, (y + 1) % gHeight);
                dX += ((float)pix.red) / 255.0f * 1.0f;

                // Cross Product of components of gradient reduces to
                float nX = -dX;
                float nY = -dY;
                float nZ = 1;

                // Normalize
                float oolen = 1.0f / ((float)sqrt(nX * nX + nY * nY + nZ * nZ));
                nX *= oolen;
                nY *= oolen;
                nZ *= oolen;

                pix.red = (uint8)PackFloatInByte(nX);
                pix.green = (uint8)PackFloatInByte(nY);
                pix.blue = (uint8)PackFloatInByte(nZ);

                WritePixel(dstImage, bpp, &pix, x, y);
            }
        }

        // Open output file
        if ((foutput = fopen(outFilename, "wb")) == NULL) {
            sprintf(buff, "Unable to open output TGA file: %s", inFilename);
            MessageBox(buff, "Error", NMB_OK | NMB_ICONERROR);
            continue;
        }

        if (!TGAWriteImage(foutput, gWidth, gHeight, bpp, dstImage, true)) {
            MessageBox("Unable to write output file", "Error", NMB_OK | NMB_ICONERROR);
            fclose(foutput);
            continue;
        }
        fclose(foutput); // close the output file

        free(srcImage);
        free(dstImage);

        sprintf(buff, "Success! New TGA file: %s", outFilename);
        MessageBox(buff, "Success", NMB_OK);
    }
    return (0);
}
