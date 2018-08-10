#pragma once

//========================================================================
// The purpose of this routine is to read a filename from the user.
// Ideally this comes up as one of those touchy feely windows windows.
//    filename -- the place to put the filename
//========================================================================
bool
GetOpenFileName(const char* caption, const char* filter, char* filename);

enum NativeMessageBoxFlags {
    NMB_OK=0x00000000,
    NMB_OKCANCEL=0x00000001,
    NMB_ICONERROR=0x00000010
};

int
MessageBox(const char* text, const char* caption, unsigned int uType);
