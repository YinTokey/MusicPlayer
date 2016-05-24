#ifndef _DeskLrc_H
#define _DeskLrc_H

#include <windows.h>

BOOL WINAPI DeskLrc_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL DeskLrc_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
void DeskLrc_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void DeskLrc_OnClose(HWND hwnd);

#endif