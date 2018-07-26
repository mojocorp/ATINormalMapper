#pragma once

//========================================================================
// The purpose of this routine is to read a filename from the user.
// Ideally this comes up as one of those touchy feely windows windows.
//    filename -- the place to put the filename
//========================================================================
int
GetOpenFileName(const char* caption, const char* filter, char* filename);

enum MessageBoxFlags {
    MB_OK=0x00000000L,
    MB_OKCANCEL=0x00000001L,
    MB_ICONERROR=0x00000010L
};

int
MessageBox(const char* text, const char* caption, unsigned int uType);
