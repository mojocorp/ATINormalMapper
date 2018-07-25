#pragma once

//========================================================================
// The purpose of this routine is to read a filename from the user.
// Ideally this comes up as one of those touchy feely windows windows.
//    filename -- the place to put the filename
//========================================================================
bool
GetOpenFileName(const char* caption, const char* filter, char* filename);
