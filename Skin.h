#ifndef _Skin_H
#define _Skin_H

#include <windows.h>

BOOL WINAPI Skin_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL Skin_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
void Skin_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void Skin_OnClose(HWND hwnd);

#endif