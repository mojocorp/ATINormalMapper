#include "NativeDialogs.h"

#ifdef _WIN32
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

#else
#ifdef __APPLE__
#include <Cocoa/Cocoa.h>

bool
GetOpenFileName(const char* caption, const char* filter, char* filename)
{
    NSArray* nsFileTypes = @[ @"nmf", @"tga", @"fs", @"txt" ];
    NSOpenPanel* panel = [NSOpenPanel openPanel];
    [panel setMessage:[NSString stringWithCString:caption encoding:NSUTF8StringEncoding]];
    [panel setAllowsMultipleSelection:NO];
    [panel setCanChooseDirectories:NO];
    [panel setCanChooseFiles:YES];
    [panel setFloatingPanel:YES];
    [panel setAllowedFileTypes:nsFileTypes];
    NSInteger result = [panel runModal];

    if (result == NSModalResponseOK) {
        NSString* nsPath = [panel URLs][0].absoluteString;
        nsPath = [nsPath stringByReplacingOccurrencesOfString:@"file://" withString:@""];
        strcpy(filename, [nsPath UTF8String]);
    }
    return result == NSModalResponseOK;
}

int
MessageBox(const char* text, const char* caption, unsigned int uType)
{
    NSAlert* alert = [[[NSAlert alloc] init] autorelease];
    if (uType & NMB_OK || uType & NMB_OKCANCEL)
        [alert addButtonWithTitle:@"OK"];
    if (uType & NMB_OKCANCEL)
        [alert addButtonWithTitle:@"Cancel"];
    [alert setMessageText:[NSString stringWithCString:caption encoding:NSUTF8StringEncoding]];
    [alert setInformativeText:[NSString stringWithCString:text encoding:NSUTF8StringEncoding]];

    NSInteger returnCode = [alert runModal];

    return returnCode == NSAlertFirstButtonReturn;
}
#else
// Assume Linux
#include <gtk/gtk.h>

static void
initGTK()
{
    static bool initialized = false;
    if (!initialized) {
        int argc = 0;
        char** argv = nullptr;
        gtk_init(&argc, &argv);
        initialized = true;
    }
}

bool
GetOpenFileName(const char* caption, const char* filter, char* filename)
{
    initGTK();
    return true;
}

int
MessageBox(const char* text, const char* caption, unsigned int uType)
{
    initGTK();

    GtkWidget* dialog = gtk_message_dialog_new(
      NULL, (GtkDialogFlags)0, GTK_MESSAGE_QUESTION, GTK_BUTTONS_OK_CANCEL, "%s", text);
    gint response = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);

    return response == GTK_RESPONSE_OK;
}
#endif
#endif
