#include "stdafx.h"
#include <windows.h>
#include <windowsx.h>
#include "resource.h"
#include "MainDlg.h"
#include "Setting.h"
#include "mmsystem.h"
//int soundmode;
//TCHAR *shortPathx;
TCHAR SetRoad[MAX_PATH];  //����Ĭ��·��
struct SET{
	int soundmode;      //�����ڵ�����ģʽֵ
	TCHAR shortPathx[MAX_PATH];    //�͵��Ӵ��ڵĶ�·��
} *Received;             //���ܸ�������Ϣ�Ľṹ��
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
	//ʹ��GetShortPathNameʱ������ȷ��ԭ·�����ļ����ڣ����򷵻ؿ��ִ�
	//��˴˴���ת��·�������������б��ļ��������뵽ȫ�ֱ���DEFAULTLISTFILE�� 
	GetShortPathName(cT,cFull,MAX_PATH);
	wsprintf(SetRoad,"%s\\.lst",cFull); 
	
//	ReadSet(hwnd,SetRoad);	 //��ȡ�б���Ϣ

	SetWindowPos(hwnd,NULL,500,300,150,150,SWP_NOSIZE);
	HWND hwCombosoundmode = GetDlgItem(hwnd, IDC_Combosoundmode);//
	ComboBox_InsertString(hwCombosoundmode, -1, TEXT("������"));
	ComboBox_InsertString(hwCombosoundmode, -1, TEXT("������"));
	ComboBox_InsertString(hwCombosoundmode, -1, TEXT("������"));	
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
//-----------------------��������---------------------------  
void SaveSet(HWND hwnd, TCHAR *cSet)
{
//	TCHAR Set[MAX_PATH];
	FILE *fp = fopen(cSet,"wb");			
    if(fopen(cSet,"wb")==NULL)
    {
		MessageBox(hwnd,"��������ʧ�ܣ�","��Ǹ",MB_OK|MB_ICONEXCLAMATION); 
	}
	else
	{		
		fwrite(&(Received->soundmode),sizeof(int),1,fp);
	}
	fclose(fp);
}
//------------------��ȡ����-------------------------------------------
void ReadSet(HWND hwnd, TCHAR *cSet)
{
//	TCHAR Set[MAX_PATH];
	FILE *fp = fopen(cSet,"wb");				
   /* if(fopen(cSet,"wb")==NULL)
    {
		MessageBox(hwnd,"��ȡ����ʧ�ܣ�","��Ǹ",MB_OK|MB_ICONEXCLAMATION); 
	}
	else
	{	*/	
		fopen(cSet,"wb");
		fwrite(&(Received->soundmode),sizeof(int),1,fp);
	
	fclose(fp);
}