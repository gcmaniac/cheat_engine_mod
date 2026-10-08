#ifndef GUI_H
#define GUI_H

#include <windows.h>

#define ID_BTN_BROWSE   1001
#define ID_BTN_PROCESS  1002
#define ID_EDIT_FILE    1003
#define ID_EDIT_ORI     1004
#define ID_EDIT_REP     1005
#define ID_EDIT_LOG     1006
#define ID_COMBO_LANG   1007

/* Initialize and run GUI window */
int gui_run(HINSTANCE hInstance, int nCmdShow, const char* initial_file);

#endif /* GUI_H */
