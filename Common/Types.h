//========================================================================================================================================================
// Types.h -- Variable prototypes
//========================================================================================================================================================
// $File: //depot/3darg/Demos/Sushi/RunTime/Types.h $ $Revision: #13 $ $Author: gosselin $
//========================================================================================================================================================
// (C) 2002 ATI Research, Inc., All rights reserved.
//========================================================================================================================================================

#ifndef __Types_h
#define __Types_h

static char sgCopyrightString[] = "\r\n\r\n(C) 2002 ATI Research, Inc.\r\n\r\n";

// DEFINES
// ===============================================================================================================================================
#ifdef TRUE
#undef TRUE
#endif
#define TRUE 1

#ifdef FALSE
#undef FALSE
#endif
#define FALSE 0

// TYPEDEFS
// ==============================================================================================================================================
// Signed
typedef char bool8;
typedef char char8;

typedef char int8;
typedef short int16;
typedef int int32;

#ifdef _WIN32
typedef __int64 int64;
#else
typedef long long int64;
#endif
typedef float float32;
typedef double float64;

// Unsigned
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;

// String
typedef const char cstr;
#ifdef _WIN32
typedef unsigned __int64 uint64;
#else
typedef unsigned long long uint64;
#endif

#endif
