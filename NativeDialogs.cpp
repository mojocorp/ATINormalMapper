#include "NativeDialogs.h"

#include <windows.h>

bool
GetOpenFileName(const char* caption, const char* filter, char* filename)
{
    HWND hWnd = NULL;
    OPENFILENAME ofn; // common dialog box structure

    // Initialize OPENFILENAME
    ZeroMemory(&ofn, sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = filename;
    ofn.nMaxFile = _MAX_PATH;
    ;
    ofn.lpstrFilter = filter;
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.lpstrFileTitle = (char*)caption;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    // Display the Open dialog box.
    return GetOpenFileName(&ofn);
}

int
MessageBox(const char* text, const char* caption, unsigned int uType)
{
    return MessageBox(NULL, text, caption, uType);
}
