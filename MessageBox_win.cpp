#include "MessageBox.h"

#include <windows.h>

int
MessageBox(const char* text, const char* caption, unsigned int uType)
{
    return MessageBox(NULL, text, caption, uType);
}
