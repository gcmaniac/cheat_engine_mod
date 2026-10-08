#include <windows.h>
#include "gui.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    (void)hPrevInstance;
    char initial_file[MAX_PATH] = "";

    if (lpCmdLine && *lpCmdLine) {
        const char* p = lpCmdLine;
        while (*p == ' ' || *p == '\t') p++;
        if (*p == '"') {
            p++;
            const char* q = strchr(p, '"');
            if (q) {
                size_t len = (size_t)(q - p);
                if (len >= sizeof(initial_file)) len = sizeof(initial_file) - 1;
                strncpy(initial_file, p, len);
                initial_file[len] = '\0';
            }
        } else {
            strncpy(initial_file, p, sizeof(initial_file) - 1);
            initial_file[sizeof(initial_file) - 1] = '\0';
        }
    }

    return gui_run(hInstance, nCmdShow, initial_file);
}
