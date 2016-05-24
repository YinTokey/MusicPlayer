#include "stdafx.h"
#include <windows.h>
#include <windowsx.h>
#include "resource.h"
#include "MainDlg.h"
#include "Setting.h"
#include "mmsystem.h"
#include "DeskLrc.h"
//int soundmode;
//TCHAR *shortPathx;
typedef struct song{
	long time;
	char lrc[100];
	
}Slrc;//存放歌词信息的结构体
Slrc *lrcshow;


BOOL WINAPI DeskLrc_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        HANDLE_MSG(hWnd, WM_INITDIALOG, DeskLrc_OnInitDialog);
        HANDLE_MSG(hWnd, WM_COMMAND, DeskLrc_OnCommand);
		HANDLE_MSG(hWnd,WM_CLOSE, DeskLrc_OnClose);
    }
	
    return FALSE;
}

BOOL DeskLrc_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{

	SetWindowPos(hwnd,NULL,500,550,150,150,SWP_NOSIZE);
//	Received=(SET* )lParam;
	POINT pt; RECT rect;GetCursorPos(&pt);//获取点击时的坐标
	GetWindowRect(hwnd, &rect);
	pt.x -= rect.left;
	pt.y -= rect.top;
	TCHAR sz[60], className[20];
	HWND hwndGet = ChildWindowFromPointEx(hwnd, pt, CWP_ALL);//获取子窗口的窗口句柄
	
	
	
	lrcshow=(Slrc*)lParam;
	HDC hDClrc=GetDC(hwndGet);
//	TCHAR J[23]="DDD";
	TextOut(hDClrc,200,200,lrcshow->lrc,strlen(lrcshow->lrc));


//	MessageBox(hwndlrc,(lrcshow+3)->lrc,TEXT(""),MB_OK);

    return TRUE;
}

void DeskLrc_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
  /*  switch(id)
    {

	
		default:
		break;
    }  */
}

void DeskLrc_OnClose(HWND hwnd)
{

       EndDialog(hwnd, 0);

} 

