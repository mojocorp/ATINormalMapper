#pragma once

#define MB_OK 0x00000000L
#define MB_ICONERROR 0x00000010L

int
MessageBox(const char* text, const char* caption, unsigned int uType);
