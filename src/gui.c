#include "gui.h"
#include "ct_processor.h"
#include "lang.h"
#include <commdlg.h>
#include <shellapi.h>
#include <stdio.h>
#include <string.h>

/* Ensure compatibility with Win95/98 OPENFILENAME size */
#ifndef OPENFILENAME_SIZE_VERSION_400
#define OPENFILENAME_SIZE_VERSION_400 76
#endif

#define PLACEHOLDER_ORI "\"pcsx2-qt.exe\"+0317C7E8"
#define PLACEHOLDER_REP "\"pcsx2-qt.exe\"+031FD968"

static HWND g_hMainWnd = NULL;
static HWND g_hLblFile = NULL;
static HWND g_hEditFile = NULL;
static HWND g_hBtnBrowse = NULL;
static HWND g_hLblLang = NULL;
static HWND g_hComboLang = NULL;
static HWND g_hLblOri = NULL;
static HWND g_hEditOri = NULL;
static HWND g_hLblRep = NULL;
static HWND g_hEditRep = NULL;
static HWND g_hBtnProcess = NULL;
static HWND g_hLblStatus = NULL;
static HWND g_hEditLog = NULL;

static int g_bOriPlaceholder = 1;
static int g_bRepPlaceholder = 1;

static char g_exeDir[MAX_PATH] = "";

static void get_executable_directory(char* out_dir, size_t out_size) {
    char path[MAX_PATH];
    char* last_sep;
    GetModuleFileNameA(NULL, path, sizeof(path));
    path[sizeof(path) - 1] = '\0';
    last_sep = strrchr(path, '\\');
    if (!last_sep) last_sep = strrchr(path, '/');
    if (last_sep) {
        *last_sep = '\0';
        strncpy(out_dir, path, out_size - 1);
        out_dir[out_size - 1] = '\0';
    } else {
        strncpy(out_dir, ".", out_size - 1);
        out_dir[out_size - 1] = '\0';
    }
}

static int g_hasProcessed = 0;

static void update_ui_texts(HWND hWnd) {
    SetWindowTextA(hWnd, _T("app_title"));
    if (g_hLblFile) SetWindowTextA(g_hLblFile, _T("file_label"));
    if (g_hBtnBrowse) SetWindowTextA(g_hBtnBrowse, _T("browse_btn"));
    if (g_hLblLang) SetWindowTextA(g_hLblLang, _T("lang_label"));
    if (g_hLblOri) SetWindowTextA(g_hLblOri, _T("ptr_ori_label"));
    if (g_hLblRep) SetWindowTextA(g_hLblRep, _T("ptr_rep_label"));
    if (g_hBtnProcess) SetWindowTextA(g_hBtnProcess, _T("process_btn"));
    if (g_hLblStatus) SetWindowTextA(g_hLblStatus, _T("status_label"));
    if (!g_hasProcessed && g_hEditLog) {
        SetWindowTextA(g_hEditLog, _T("ready_log"));
    }
}

static void browse_ct_file(HWND hWnd) {
    OPENFILENAMEA ofn;
    char szFile[MAX_PATH] = "";

    memset(&ofn, 0, sizeof(ofn));
    /* Use OPENFILENAME_SIZE_VERSION_400 for Win95/98/NT4/XP/7/10 universal compatibility */
    ofn.lStructSize = OPENFILENAME_SIZE_VERSION_400;
    ofn.hwndOwner = hWnd;
    ofn.lpstrFilter = "Cheat Engine Table (*.CT)\0*.CT\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrTitle = (strcmp(lang_get_current(), "en") == 0) ?
                     "Select Cheat Engine Table (.CT)" :
                     "Pilih File Cheat Engine Table (.CT)";
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;

    if (GetOpenFileNameA(&ofn)) {
        SetWindowTextA(g_hEditFile, szFile);
    }
}

static void log_append(const char* text) {
    int len = GetWindowTextLengthA(g_hEditLog);
    SendMessageA(g_hEditLog, EM_SETSEL, (WPARAM)len, (LPARAM)len);
    SendMessageA(g_hEditLog, EM_REPLACESEL, 0, (LPARAM)text);
}

static void on_process_clicked(HWND hWnd) {
    char filePath[MAX_PATH];
    char ptrOri[256];
    char ptrRep[256];
    CTProcessResult res;
    char log_buf[1024];

    GetWindowTextA(g_hEditFile, filePath, sizeof(filePath));
    GetWindowTextA(g_hEditOri, ptrOri, sizeof(ptrOri));
    GetWindowTextA(g_hEditRep, ptrRep, sizeof(ptrRep));

    if (strlen(filePath) == 0) {
        MessageBoxA(hWnd, _T("warn_select_file"), _T("warn_title"), MB_OK | MB_ICONWARNING);
        SetFocus(g_hBtnBrowse);
        return;
    }

    if (strlen(ptrOri) == 0) {
        MessageBoxA(hWnd, _T("warn_fill_ori"), _T("warn_title"), MB_OK | MB_ICONWARNING);
        SetFocus(g_hEditOri);
        return;
    }

    if (strlen(ptrRep) == 0) {
        MessageBoxA(hWnd, _T("warn_fill_rep"), _T("warn_title"), MB_OK | MB_ICONWARNING);
        SetFocus(g_hEditRep);
        return;
    }

    g_hasProcessed = 1;
    SetWindowTextA(g_hEditLog, _T("log_start"));

    snprintf(log_buf, sizeof(log_buf), "%s: %s\r\n%s: %s\r\n%s: %s\r\n\r\n",
             _T("log_input_file"), filePath,
             _T("log_ptr_ori"), ptrOri,
             _T("log_ptr_rep"), ptrRep);
    log_append(log_buf);

    if (!ct_process_file(filePath, ptrOri, ptrRep, g_exeDir, &res)) {
        snprintf(log_buf, sizeof(log_buf), "[ERROR] %s\r\n", res.error_message);
        log_append(log_buf);
        MessageBoxA(hWnd, res.error_message, _T("error_title"), MB_OK | MB_ICONERROR);
        return;
    }

    /* Format difference for display with localized decimal word */
    char delta_str[64];
    if (res.delta >= 0) {
        snprintf(delta_str, sizeof(delta_str), "+%I64X (+%I64d %s)",
                 (unsigned __int64)res.delta, (long long)res.delta, _T("decimal_word"));
    } else {
        snprintf(delta_str, sizeof(delta_str), "-%I64X (%I64d %s)",
                 (unsigned __int64)(-res.delta), (long long)res.delta, _T("decimal_word"));
    }

    snprintf(log_buf, sizeof(log_buf),
             "%s\r\n"
             "%s %s\r\n"
             "%s %s\r\n"
             "%s %d %s\r\n"
             "%s %d %s\r\n"
             "%s %s\r\n\r\n"
             "%s",
             _T("log_result_title"),
             _T("log_target_module"), res.target_module[0] ? res.target_module : _T("all_modules"),
             _T("log_delta"), delta_str,
             _T("log_total_found"), res.pointers_found, _T("unit_found"),
             _T("log_pointers_modified"), res.pointers_modified, _T("unit_modified"),
             _T("log_output_file"), res.output_filepath,
             _T("log_success"));
    log_append(log_buf);

    char msg[512];
    snprintf(msg, sizeof(msg), _T("dialog_finished"),
             delta_str, res.pointers_modified, res.pointers_found, res.output_filepath);
    MessageBoxA(hWnd, msg, _T("success_title"), MB_OK | MB_ICONINFORMATION);
}

static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE: {
            HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

            /* File Selection Label */
            g_hLblFile = CreateWindowA("STATIC", _T("file_label"),
                                       WS_CHILD | WS_VISIBLE,
                                       15, 15, 360, 18, hWnd, NULL, NULL, NULL);

            /* Language Selector Label & ComboBox */
            g_hLblLang = CreateWindowA("STATIC", _T("lang_label"),
                                       WS_CHILD | WS_VISIBLE | SS_RIGHT,
                                       380, 15, 60, 18, hWnd, NULL, NULL, NULL);

            g_hComboLang = CreateWindowA("COMBOBOX", "",
                                         WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_TABSTOP,
                                         445, 11, 105, 100, hWnd, (HMENU)ID_COMBO_LANG, NULL, NULL);

            SendMessageA(g_hComboLang, CB_ADDSTRING, 0, (LPARAM)"ID - Indonesia");
            SendMessageA(g_hComboLang, CB_ADDSTRING, 0, (LPARAM)"EN - English");
            SendMessageA(g_hComboLang, CB_SETCURSEL, (strcmp(lang_get_current(), "en") == 0) ? 1 : 0, 0);

            g_hEditFile = CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT", "",
                                         WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | WS_TABSTOP,
                                         15, 35, 420, 24, hWnd, (HMENU)ID_EDIT_FILE, NULL, NULL);

            g_hBtnBrowse = CreateWindowA("BUTTON", _T("browse_btn"),
                                         WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP,
                                         445, 34, 105, 26, hWnd, (HMENU)ID_BTN_BROWSE, NULL, NULL);

            /* Pointer Original */
            g_hLblOri = CreateWindowA("STATIC", _T("ptr_ori_label"),
                                      WS_CHILD | WS_VISIBLE,
                                      15, 75, 535, 18, hWnd, NULL, NULL, NULL);

            g_hEditOri = CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT", PLACEHOLDER_ORI,
                                        WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | WS_TABSTOP,
                                        15, 95, 535, 24, hWnd, (HMENU)ID_EDIT_ORI, NULL, NULL);

            /* Pointer Pengganti */
            g_hLblRep = CreateWindowA("STATIC", _T("ptr_rep_label"),
                                      WS_CHILD | WS_VISIBLE,
                                      15, 135, 535, 18, hWnd, NULL, NULL, NULL);

            g_hEditRep = CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT", PLACEHOLDER_REP,
                                        WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | WS_TABSTOP,
                                        15, 155, 535, 24, hWnd, (HMENU)ID_EDIT_REP, NULL, NULL);

            /* Process Button */
            g_hBtnProcess = CreateWindowA("BUTTON", _T("process_btn"),
                                          WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | WS_TABSTOP,
                                          15, 195, 535, 36, hWnd, (HMENU)ID_BTN_PROCESS, NULL, NULL);

            /* Log / Status Output */
            g_hLblStatus = CreateWindowA("STATIC", _T("status_label"),
                                         WS_CHILD | WS_VISIBLE,
                                         15, 245, 300, 18, hWnd, NULL, NULL, NULL);

            g_hEditLog = CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT",
                                         _T("ready_log"),
                                         WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_READONLY | ES_AUTOVSCROLL,
                                         15, 265, 535, 190, hWnd, (HMENU)ID_EDIT_LOG, NULL, NULL);

            /* Apply system GUI font to all child controls */
            EnumChildWindows(hWnd, (WNDENUMPROC)(void*)SendMessageA, (LPARAM)WM_SETFONT);
            HWND hChild = GetWindow(hWnd, GW_CHILD);
            while (hChild) {
                SendMessageA(hChild, WM_SETFONT, (WPARAM)hFont, TRUE);
                hChild = GetWindow(hChild, GW_HWNDNEXT);
            }

            /* Enable Drag & Drop */
            DragAcceptFiles(hWnd, TRUE);
            break;
        }

        case WM_CTLCOLOREDIT: {
            HDC hdc = (HDC)wParam;
            HWND hCtrl = (HWND)lParam;
            if ((hCtrl == g_hEditOri && g_bOriPlaceholder) ||
                (hCtrl == g_hEditRep && g_bRepPlaceholder)) {
                SetTextColor(hdc, RGB(128, 128, 128)); /* Grayed placeholder */
                SetBkColor(hdc, RGB(255, 255, 255));
                return (LRESULT)GetStockObject(WHITE_BRUSH);
            }
            SetTextColor(hdc, RGB(0, 0, 0)); /* Normal text */
            SetBkColor(hdc, RGB(255, 255, 255));
            return (LRESULT)GetStockObject(WHITE_BRUSH);
        }

        case WM_DROPFILES: {
            HDROP hDrop = (HDROP)wParam;
            char droppedFile[MAX_PATH];
            if (DragQueryFileA(hDrop, 0, droppedFile, sizeof(droppedFile))) {
                SetWindowTextA(g_hEditFile, droppedFile);
            }
            DragFinish(hDrop);
            break;
        }

        case WM_COMMAND: {
            int wmId = LOWORD(wParam);
            int wmEvent = HIWORD(wParam);

            if (wmId == ID_COMBO_LANG && wmEvent == CBN_SELCHANGE) {
                int sel = (int)SendMessageA(g_hComboLang, CB_GETCURSEL, 0, 0);
                if (sel == 1) {
                    lang_set("en");
                } else {
                    lang_set("id");
                }
                update_ui_texts(hWnd);
            } else if (wmId == ID_EDIT_ORI) {
                if (wmEvent == EN_SETFOCUS) {
                    if (g_bOriPlaceholder) {
                        g_bOriPlaceholder = 0;
                        SetWindowTextA(g_hEditOri, "");
                        InvalidateRect(g_hEditOri, NULL, TRUE);
                    }
                } else if (wmEvent == EN_KILLFOCUS) {
                    if (GetWindowTextLengthA(g_hEditOri) == 0) {
                        g_bOriPlaceholder = 1;
                        SetWindowTextA(g_hEditOri, PLACEHOLDER_ORI);
                        InvalidateRect(g_hEditOri, NULL, TRUE);
                    }
                }
            } else if (wmId == ID_EDIT_REP) {
                if (wmEvent == EN_SETFOCUS) {
                    if (g_bRepPlaceholder) {
                        g_bRepPlaceholder = 0;
                        SetWindowTextA(g_hEditRep, "");
                        InvalidateRect(g_hEditRep, NULL, TRUE);
                    }
                } else if (wmEvent == EN_KILLFOCUS) {
                    if (GetWindowTextLengthA(g_hEditRep) == 0) {
                        g_bRepPlaceholder = 1;
                        SetWindowTextA(g_hEditRep, PLACEHOLDER_REP);
                        InvalidateRect(g_hEditRep, NULL, TRUE);
                    }
                }
            } else if (wmId == ID_BTN_BROWSE) {
                browse_ct_file(hWnd);
            } else if (wmId == ID_BTN_PROCESS) {
                on_process_clicked(hWnd);
            }
            break;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProcA(hWnd, msg, wParam, lParam);
    }
    return 0;
}

int gui_run(HINSTANCE hInstance, int nCmdShow, const char* initial_file) {
    WNDCLASSEXA wc;
    HWND hWnd;
    MSG msg;

    lang_init("id");
    get_executable_directory(g_exeDir, sizeof(g_exeDir));

    memset(&wc, 0, sizeof(wc));
    wc.cbSize = sizeof(WNDCLASSEXA);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIconA(hInstance, MAKEINTRESOURCEA(101));
    wc.hIconSm = (HICON)LoadImageA(hInstance, MAKEINTRESOURCEA(101), IMAGE_ICON, 16, 16, 0);
    wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
    wc.lpszClassName = "CheatEnginePointerAdapterWnd";

    if (!RegisterClassExA(&wc)) {
        MessageBoxA(NULL, "Gagal mendaftarkan window class!", "Error", MB_ICONERROR);
        return 1;
    }

    hWnd = CreateWindowExA(
        0,
        "CheatEnginePointerAdapterWnd",
        _T("app_title"),
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 580, 515,
        NULL, NULL, hInstance, NULL
    );

    if (!hWnd) {
        MessageBoxA(NULL, "Gagal membuat jendela utama!", "Error", MB_ICONERROR);
        return 1;
    }

    g_hMainWnd = hWnd;

    /* Set window icons */
    if (wc.hIcon) SendMessageA(hWnd, WM_SETICON, ICON_BIG, (LPARAM)wc.hIcon);
    if (wc.hIconSm) SendMessageA(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)wc.hIconSm);

    /* Pre-fill initial file if provided */
    if (initial_file && *initial_file) {
        SetWindowTextA(g_hEditFile, initial_file);
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    /* Set focus initially to Browse button so placeholder text remains grayed */
    SetFocus(g_hBtnBrowse);

    while (GetMessageA(&msg, NULL, 0, 0)) {
        if (!IsDialogMessageA(hWnd, &msg)) {
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }
    }

    return (int)msg.wParam;
}
