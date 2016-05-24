#ifndef _Setting_H
#define _Setting_H

#include <windows.h>

BOOL WINAPI Setting_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL Setting_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
void Setting_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void Setting_OnClose(HWND hwnd);

#endif