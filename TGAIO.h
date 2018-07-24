/******************************************************************************
 *  TGAIO.h -- Targa file IO descriptions
 ******************************************************************************
 $Header: //depot/3darg/Tools/NormalMapper/TGAIO.h#4 $
 ******************************************************************************
 *  (C) 2000 ATI Research, Inc.  All rights reserved.
 ******************************************************************************/

#ifndef __TGAIO__H
#define __TGAIO__H

#include <Types.h>

#include <stdio.h>

//Targa header info
#pragma pack (push)
#pragma pack (1)	//dont pad the following struct

typedef struct _TGAHeaderInfo
{
   uint8 idlen;    //length of optional identification sequence
   uint8 cmtype;   //indicates whether a palette is present
   uint8 imtype;   //image data type (e.g., uncompressed RGB)
   uint16 cmorg;    //first palette index, if present
   uint16 cmcnt;    //number of palette entries, if present
   uint8 cmsize;   //number of bits per palette entry
   uint16 imxorg;   //horiz pixel coordinate of lower left of image
   uint16 imyorg;   //vert pixel coordinate of lower left of image
   uint16 imwidth;  //image width in pixels
   uint16 imheight; //image height in pixels
   uint8 imdepth;  //image color depth (bits per pixel)
   uint8 imdesc;   //image attribute flags
}TGAHeaderInfo;

typedef struct _pixel
{
   uint8 red;
   uint8 blue;
   uint8 green;
   uint8 alpha;
} pixel;

#pragma pack (pop)

extern bool TGAWriteImage (FILE* fp, int width, int height, int bpp,
                           uint8* image, bool aSwapRedBlue = false);
extern bool TGAReadImage (FILE* fp, int* width, int* height, int* bitDepth,
                          uint8** pixels);

#endif // __TGAIO__H
