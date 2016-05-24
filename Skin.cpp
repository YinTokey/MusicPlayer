#include "stdafx.h"
#include <windows.h>
#include <windowsx.h>
#include "resource.h"
#include "Skin.h"
#include "SkinH.h"
#include "MainDlg.h"
#pragma comment(lib,"SkinH.lib")  //Ƥ����
int Skinstyle;//Ƥ����ʽ
TCHAR Skin[256];//��

BOOL WINAPI Skin_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        HANDLE_MSG(hWnd, WM_INITDIALOG, Skin_OnInitDialog);
        HANDLE_MSG(hWnd, WM_COMMAND, Skin_OnCommand);
		HANDLE_MSG(hWnd,WM_CLOSE, Skin_OnClose);
    }

    return FALSE;
}

BOOL Skin_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	SetWindowPos(hwnd,NULL,500,300,150,150,SWP_NOSIZE);
	HWND hwComboskin = GetDlgItem(hwnd, IDC_ComboSkin);
	ComboBox_InsertString(hwComboskin, -1, TEXT("ľ�ʷ��"));//     0
	ComboBox_InsertString(hwComboskin, -1, TEXT("���̱߿�"));//     1
	ComboBox_InsertString(hwComboskin, -1, TEXT("����A"));//        2
	ComboBox_InsertString(hwComboskin, -1, TEXT("����B"));//        3
	ComboBox_InsertString(hwComboskin, -1, TEXT("�ۺ�A"));//        4
	ComboBox_InsertString(hwComboskin, -1, TEXT("�ۺ�B"));//        5
	ComboBox_InsertString(hwComboskin, -1, TEXT("������"));//       6
	ComboBox_InsertString(hwComboskin, -1, TEXT("������ɫ"));//     7
	ComboBox_InsertString(hwComboskin, -1, TEXT("����"));//         8
	ComboBox_InsertString(hwComboskin, -1, TEXT("ǳ��"));//         9       
	ComboBox_InsertString(hwComboskin, -1, TEXT("�Ϻ�"));//         10
	ComboBox_SetCurSel(hwComboskin, 0); 
/*	TCHAR *Skin0=(TCHAR*)lParam;
	SetWindowLong(hwnd,GWL_USERDATA,(LPARAM)Skin0);
	MessageBox(hwnd,Skin0,Skin0,MB_YESNO);  */
    return TRUE;
}

void Skin_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	HINSTANCE hInstance=(HINSTANCE)GetWindowLong(hwnd,GWL_HINSTANCE);
	
    switch(id)
    {
        case IDC_Applied:
		{
			Skinstyle = ComboBox_GetCurSel(GetDlgItem(hwnd, IDC_ComboSkin));
			if(0==Skinstyle)
				wsprintf(Skin,"skin\\ľ�ʷ��.she");
			SkinH_AttachEx(Skin,"1");
			if(1==Skinstyle)
				wsprintf(Skin,"skin\\���̱߿�.she");
			SkinH_AttachEx(Skin,"1");
			if(2==Skinstyle)
				wsprintf(Skin,"skin\\����A.she");
			SkinH_AttachEx(Skin,"1");
			if(3==Skinstyle)
				wsprintf(Skin,"skin\\����B.she");
			SkinH_AttachEx(Skin,"1");
			if(4==Skinstyle)
				wsprintf(Skin,"skin\\�ۺ�A.she");
			SkinH_AttachEx(Skin,"1");
			if(5==Skinstyle)
				wsprintf(Skin,"skin\\�ۺ�B.she");
			SkinH_AttachEx(Skin,"1");
			if(6==Skinstyle)
				wsprintf(Skin,"skin\\������.she");
			SkinH_AttachEx(Skin,"1");
			if(7==Skinstyle)
				wsprintf(Skin,"skin\\������ɫ.she");
			SkinH_AttachEx(Skin,"1");
			if(8==Skinstyle)
				wsprintf(Skin,"skin\\����.she");
			SkinH_AttachEx(Skin,"1");
			if(9==Skinstyle)
				wsprintf(Skin,"skin\\ǳ��.she");
			SkinH_AttachEx(Skin,"1");
			if(10==Skinstyle)
				wsprintf(Skin,"skin\\�Ϻ�.she");
			SkinH_AttachEx(Skin,"1");
			//	DialogBoxParam(hInstance,MAKEINTRESOURCE(IDD_MAIN),NULL,Main_Proc,(LPARAM)&Skin);
//			TCHAR fileName[MAX_PATH];

			EndDialog(hwnd, Skinstyle);
		}
        break;
		
        default:
		break;
    }
}

void Skin_OnClose(HWND hwnd)
{
    EndDialog(hwnd, 0);
}