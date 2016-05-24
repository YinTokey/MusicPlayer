#include "stdafx.h"
#include <windows.h>
#include <windowsx.h>
#include "resource.h"
#include "MainDlg.h"
#include "Setting.h"
#include "mmsystem.h"
//int soundmode;
//TCHAR *shortPathx;
TCHAR SetRoad[MAX_PATH];  //保存默认路径
struct SET{
	int soundmode;      //父窗口的声道模式值
	TCHAR shortPathx[MAX_PATH];    //送到子窗口的短路径
} *Received;             //接受父窗口消息的结构体
//void ReadSet(HWND hwnd, TCHAR *cSet);
//void SaveSet(HWND hwnd, TCHAR *cSet);

BOOL WINAPI Setting_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        HANDLE_MSG(hWnd, WM_INITDIALOG, Setting_OnInitDialog);
        HANDLE_MSG(hWnd, WM_COMMAND, Setting_OnCommand);
		HANDLE_MSG(hWnd,WM_CLOSE, Setting_OnClose);
    }
	
    return FALSE;
}

BOOL Setting_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	TCHAR cT[MAX_PATH],cFull[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, cT);
	//使用GetShortPathName时，必须确保原路径及文件存在，否则返回空字串
	//因此此处先转换路径，再连接上列表文件名，存入到全局变量DEFAULTLISTFILE中 
	GetShortPathName(cT,cFull,MAX_PATH);
	wsprintf(SetRoad,"%s\\.lst",cFull); 
	
//	ReadSet(hwnd,SetRoad);	 //读取列表信息

	SetWindowPos(hwnd,NULL,500,300,150,150,SWP_NOSIZE);
	HWND hwCombosoundmode = GetDlgItem(hwnd, IDC_Combosoundmode);//
	ComboBox_InsertString(hwCombosoundmode, -1, TEXT("立体声"));
	ComboBox_InsertString(hwCombosoundmode, -1, TEXT("左声道"));
	ComboBox_InsertString(hwCombosoundmode, -1, TEXT("右声道"));	
	ComboBox_SetCurSel(hwCombosoundmode, Received->soundmode);  

//	Received=(SET* )lParam;


    return TRUE;
}

void Setting_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    switch(id)
    {
	case IDC_OK:
		{
		Received->soundmode = ComboBox_GetCurSel(GetDlgItem(hwnd, IDC_Combosoundmode));
	
		TCHAR str1[256];
		if(Received->soundmode==0)
		{
		wsprintf(str1,"Setaudio %s source to stereo",Received->shortPathx);
		
		} 
		if(Received->soundmode==1)
		{
			wsprintf(str1,"Setaudio %s source to left",Received->shortPathx);
		} 
		if(Received->soundmode==2)
		{
			wsprintf(str1,"Setaudio %s source to right",Received->shortPathx);
		} 
		mciSendString(	str1, NULL,0,0);  
		 EndDialog(hwnd, 0);
		}
        break;
	case IDC_Cancel:
		{
			MessageBox(hwnd,Received->shortPathx,TEXT(""),MB_OK);
			// EndDialog(hwnd, Received->soundmode);
		}
		break;
	default:
		break;
    }
}

void Setting_OnClose(HWND hwnd)
{

       EndDialog(hwnd, 0);

} 
//-----------------------保存设置---------------------------  
void SaveSet(HWND hwnd, TCHAR *cSet)
{
//	TCHAR Set[MAX_PATH];
	FILE *fp = fopen(cSet,"wb");			
    if(fopen(cSet,"wb")==NULL)
    {
		MessageBox(hwnd,"保存设置失败！","抱歉",MB_OK|MB_ICONEXCLAMATION); 
	}
	else
	{		
		fwrite(&(Received->soundmode),sizeof(int),1,fp);
	}
	fclose(fp);
}
//------------------读取设置-------------------------------------------
void ReadSet(HWND hwnd, TCHAR *cSet)
{
//	TCHAR Set[MAX_PATH];
	FILE *fp = fopen(cSet,"wb");				
   /* if(fopen(cSet,"wb")==NULL)
    {
		MessageBox(hwnd,"读取设置失败！","抱歉",MB_OK|MB_ICONEXCLAMATION); 
	}
	else
	{	*/	
		fopen(cSet,"wb");
		fwrite(&(Received->soundmode),sizeof(int),1,fp);
	
	fclose(fp);
}