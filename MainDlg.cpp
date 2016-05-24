#ifndef _WIN32_WINNT

#endif
//#define LWA_COLORKEY 0x0502
#define _WIN32_WINNT 0x0502
#include "stdafx.h"
#include <windows.h>
#include "WindowsX.h"
#include "MainDlg.h"
#include "resource.h"
#include <commdlg.h>
#include <shlobj.h>
#include <iostream.h>
#include "mmsystem.h"
#include <commctrl.h>  // TBM_SETPOSͷ�ļ�
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "shellapi.h"	// ���� NOTIFYICONDATA
#include "SkinH.h"  //Ƥ����
#include "Skin.h"
#include "Setting.h"
#include "tchar.h"
#include "WinUser.h"

#include "AFXWIN.H"
#include "URLmon.h"

#pragma comment(lib,"URLmon.lib ") 


//#include "strsafe.h"
//#include "specstrings.h"
//#include "WinNT.h"
//#include "SkinH.h"
//#pragma comment(lib, "specstrings.lib")
#pragma comment(lib, "strsafe.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"SkinH.lib")  //Ƥ����


//---------------------------�궨��-------------------------------------------------------
#define ORDER 0  //˳�򲥷�
#define SINGLELOOP 1  //����ѭ��
#define LISTLOOP 2  //�б�ѭ��
#define RANDOM 3  //�������
#define WM_MYMESSAGE WM_USER+100
//--------------------------------ȫ�ֱ�������--------------------------------------
TCHAR szFileName[80*MAX_PATH]; //Ҫ��ӵ��ļ�ȫ������ͬʱѡ����ļ�����ȫ���������� ÿ���ļ�ȫ���á�\n���ָ��Ȼ��һά���鵫����ʾ��ʱ���ǿ����ж��еġ�
static char szOpenFileNames[80*MAX_PATH];				// ��ʼ��ʱ������ȡ�ļ����ı���
static OPENFILENAME ofn;                               
TCHAR szPath[MAX_PATH];									// �ļ�·�����������ļ���
TCHAR shortPath[MAX_PATH];                               //��·��������MIC�����Ĳ���
static char szFileName_Path[255*2][MAX_PATH];			// ��¼�����ľ���·���������ļ���
TCHAR songname[256]	;							//���ڲ��Ÿ�����������·���ͺ�׺��
static int numOnOpenFile = 0;							// ��¼���ļ�ʱ���������
static int numOnList = 0;								// ��¼�б�����ʾ���������
static int numPlayMusicNow = 0;							// ���ڲ��Ÿ��������
static int playStatus;                                  // ��ǰ��������״̬��1 ��ʾ���ڲ���״̬��0 ��ʾ��ͣ״̬  2��ʾֹͣ
TCHAR musicTotalTime[256];								// ��¼һ�׸����ʱ��	
static int totalSecond;									// ��¼һ�׸��������
static int totalMinute;									// ��¼һ�׸���ܷ�����
TCHAR strTime[256] = "";								// ��Ŀ���ŵ�ǰʱ��
TCHAR curTime[256] = "";								//���ص�ǰ����ʱ��	
long curLength;	//	   curLength = atoi(curTime);		//���ǵ�ǰʱ����ַ���ת��ΪINT��

long allLength;											//�����ܳ��ȵĳ�������ʽ
TCHAR singer[256];                                      //ͨ��MP3�ӿ��ҵ��ĸ�����
//HWND hwndMusicSilder;									// ���Ÿ����Ľ��������
HWND hwndVolumeSilder;								// �������ƵĽ��������
int voice;//����
int voicecopy;//                                     ����ֵ
int mode = ORDER;										//��ǰ״̬�趨Ϊ˳�򲥷�
TCHAR musicTotalLength[256];							//�����ܳ��ȣ�����Ϊ��λ  �ַ���ʽ
TCHAR DEFAULTLISTFILE[MAX_PATH];						// ����Ĭ��·��
//HWND hwnd;	
static int totalMusicCount;							// ��¼�б�ĸ���������
HINSTANCE hApp;
static HMENU hmenu1;
TCHAR *Skin11="";
TCHAR *Setting11="";
int skinindex;
TCHAR Skinroad[256];		//Ƥ��·��
int Extendmode=0;          //������ʽ
int Minimode=0;             //������ʽ
HWND hwndlist;
TCHAR Listallname[256*100][256];   //�б����и�����
HWND hSearchEdit,hSearchbutton;
	  
int MusicListflag=1;                    //��ͬ�б��Ӧ��ֵͬ
 struct SET{
	int soundmodeparent;      //�����ڵ�����ģʽֵ
	TCHAR shortPathtoChild[MAX_PATH];    //�͵��Ӵ��ڵĶ�·��
	
}Sset;// �����á��Ӵ�����ĸ���ֵ
 TCHAR firstxml[256];             //��һ��XML�����ļ�
 TCHAR EDITsong[256],EDITsinger[256],STR1[256];

//-------------���ڸ�ʵĶ���-----------(��Ϊ�Ǻ��ڼ��룬Ϊ�˲����ң����Էָ��)------------------

#define ID_TIMER 1000//��ʱ����ʶ��
typedef struct song{
	long time;
	char lrc[100];
	
}Slrc;//��Ÿ����Ϣ�Ľṹ��



Slrc *ps1,*ps2,*head2,*tmp,*tmp2; // ps1,ps2��ָ��̬����Ĵ洢�����Ϣ���ڴ棬ps2ʼ��ָ����ڴ�Ŀ�ʼ����
char *p,*head1,*poslrc;
static bool signStart = 0, signStartLrc = 0;
TCHAR geciEDIT11[256*100],geciEDIT33[256*100];  //EDIT11��EDIT33�ĸ�ʣ�һά����  \r\n  �ָ���ʾ
TCHAR geci[256*100][256]; //��ά����Ѹ�ʱ�������,ȫ����ʣ�  
LONG lrctime[256*100] ;//ȫ����ʵ�ʱ��
TCHAR lyrics[256];  //��ǰĿ¼·��+����ļ���+��׺
TCHAR lyricsX[256]; //  ���������ƣ�ֻ��������һ��Ŀ¼
int flag;// ����ļ��Ƿ���ڵ��жϱ�־
int temp;//����Ƿ�������жϱ�־
int lrcindex; //�������
int indexE33;//EDIT33ר��
int k;//  һ����K�и��
long curLengthEDIT11=0; // ʱ��Ƚϣ���ֹ�����˸
long curLengthEDIT22=0;
long curLengthEDIT33=0;
//------------------------------------������----------------------------------------
#ifndef _WIN32_WINNT
#define LWA_COLORKEY 0x5000
#endif
#include "afxwin.h"
#include <objbase.h> 
#define RGB_TEXT_SING RGB(255,0,255)			// ������������ɫ���ۺ�ɫ
#define RGB_TEXT_NOT_SING RGB(100,255,0)		// δ������������ɫ������ɫ
#define RGB_MEMDC_BRUSH RGB(0,0,0)				// �ڴ�DC��ˢ��ɫ����ɫ

// ���ڵ�x,y���꣬��ȣ��߶�
#define WND_ORG_X 10
#define WND_ORG_Y 650
#define WND_WIDTH 1000
#define WND_HEIGHT 100

// ����Ŀ�ȣ��߶�
#define FONT_WIDTH 53
#define FONT_HEIGHT 25
HDC hdc,m_hMemdc;
SIZE sz;					// UpdateLayeredWindow()�����õ��Ĳ���
POINT ptDC, ptMemDC;		// ��ĻDC���꣨��Ӧ�ó���,�ڴ�DC����
BLENDFUNCTION m_Blend;		// UpdateLayeredWindow()�����õ��Ĳ����ṹ��
RECT rectWndStart, rectLrc, rectSing;// ���ھ��������ڴ�DC(������ȫ�����)�������򣬳����ĸ�ʾ�������
HBRUSH hbrushMemDC;			// �ڴ�DC��ˢ���
typedef BOOL (WINAPI* functionpointer)(HWND,HDC,POINT*,SIZE*,HDC,POINT*,COLORREF,BLENDFUNCTION*,DWORD); 
functionpointer UpdateLayeredWindow;
HMODULE h = LoadLibrary("User32.dll");
//UpdateLayeredWindow = (functionpointer)GetProcAddress(h, "UpdateLayeredWindow");



//---------------------------------����----------------------------------------------
HFONT hfont1 = CreateFont(25,15,0,0,800,FALSE,FALSE,0, GB2312_CHARSET /*ANSI_CHARSET*/, 
						 OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,NONANTIALIASED_QUALITY/*��ȥ���*/ /*ANTIALIASED_QUALITY ȥ���,ƽ������*/,
		DEFAULT_PITCH | FF_SWISS, "����_GB2312");
HFONT hfontEDIT22 = CreateFont(28,13,0,0,900,FALSE,FALSE,0, RUSSIAN_CHARSET /*ANSI_CHARSET*/, 
						  OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,NONANTIALIASED_QUALITY/*��ȥ���*/ /*ANTIALIASED_QUALITY ȥ���,ƽ������*/,
		DEFAULT_PITCH | FF_SWISS, "EDIT22");
HFONT hfontEDIT11 = CreateFont(24,10,0,0,550,FALSE,FALSE,0, RUSSIAN_CHARSET /*ANSI_CHARSET*/, 
								OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,NONANTIALIASED_QUALITY/*��ȥ���*/ /*ANTIALIASED_QUALITY ȥ���,ƽ������*/,
		DEFAULT_PITCH | FF_MODERN, "EDIT11");
HFONT hfontcannotshow = CreateFont(20,10,0,0,700,FALSE,FALSE,0, GB2312_CHARSET /*ANSI_CHARSET*/, 
							   OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,NONANTIALIASED_QUALITY/*��ȥ���*/ /*ANTIALIASED_QUALITY ȥ���,ƽ������*/,
		DEFAULT_PITCH | FF_MODERN, "cannotshow");

HINSTANCE hInst;
HWND hWnd11,hWnd22,hEdit11,hEdit22;



HDC  hDCEdit11,hDCEdit22;
HWND hWndEdit11,hWndEdit22;



HWND hWnd,hButton,hEdit;
const RECT wndRect={0,0,250,160}; // window rectangle
char editStr[100];
char wndText[100];
COLORREF testColor=RGB(255,0,0);
HDC  hDCEdit;
HWND hWndEdit;
//--------------------------------�Զ��庯������--------------------------------------
BOOL WINAPI Main_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL Main_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
void AddFile(HWND hwnd);
void AddFloder(HWND hwnd);
void Play(HWND hwnd);
void Pause(HWND hwnd);
void Stop(HWND hwnd);
void LastSong(HWND hwnd);
void NextSong(HWND hwnd);
void MusicTotalTime(HWND hwnd);
void Main_OnHScroll(HWND hwnd, HWND hwndCtl, UINT code, int pos);
void autoNext(HWND hwnd);
void CALLBACK ProgressBar(HWND hwnd,UINT message,UINT iTimerId,DWORD dwTime);
void SaveList(HWND hwnd,TCHAR *cFileName);
void ReadList(HWND hwnd, TCHAR *cFileName);
void MinimizeToTray(HWND hwnd);
void Delete(HWND hwnd);
void DeleteAll(HWND hwnd);
void GetSongMessage(HWND hwnd);
void Location(HWND hwnd);
void LRC(void);
void ShowLyrics(void);
BOOL JudgeLrc(void);
void ShowEDIT11(HWND hwnd);
void InitFont(HWND hwnd);
void  Mute(HWND hwnd);
void Extend(HWND hwnd);
void Mini(HWND hwnd);
void SearchLocal(HWND hwnd);
void DeskLrc(HWND hwnd);
void FirstXML(HWND hwnd);
void analyzeFirstXML(HWND hwnd);
void FirstXMLwast(HWND hwnd);
void OnLine(HWND hwnd,LPCTSTR lpFmt);

HBRUSH SetEditColor11(HWND hWndOwner11,HDC hDCEdit11,HWND hWndEdit11);
HBRUSH SetEditColor22(HWND hWndOwner22,HDC hDCEdit22,HWND hWndEdit22);
HBRUSH SetEditColor(HWND hWndOwner,HDC hDCEdit,HWND hWndEdit);



//----------------------- �ص���������ȡ��ǰ����ʱ��,���û������ĵ�ǰֵ-----------------------------------
//  �˺�����Ҫ���ڳ�ʼ���Ի����ж���curTimePro�������磺SetTimer(hwnd, 0, 1000, curTimePro); 

void CALLBACK curTimePro(HWND hwnd, UINT message, UINT iTemerID, DWORD dwTime)
{
	TCHAR str[256];
	TCHAR shortPath[MAX_PATH];
	GetShortPathName(szFileName_Path[numPlayMusicNow], shortPath, sizeof(shortPath));

	wsprintf(str, "status %s position", shortPath);
	//int wsprintf( ����LPTSTR lpOut, ��������������Ϊ1024�ֽ� ����LPCTSTR lpFmt,  ��ʽ�ַ��� ����... ������Ĳ���;��
	mciSendString(str, curTime, sizeof(curTime)/sizeof(TCHAR), NULL);//mciSendString(��play ***��,"",0,NULL);
	curLength = atoi(curTime);
	totalMinute = curLength / 60000;//����ʱ����  ���֡�������
	totalSecond = (curLength / 1000) % 60;//����ʱ����  ���롱������
	wsprintf(strTime, "%.2d:%.2d",totalMinute, totalSecond);// ��Ŀ���ŵ�ǰʱ�� ��ȷ��
	SetDlgItemText(hwnd,IDC_MusicNowTime,strTime);//����ǰ����ʱ���ӡ��IDC_TimeStart��

	allLength= atoi(musicTotalLength);	
	// ���û�������ֵ����Сֵ�Լ���ʱλ��
	HWND hwSilder= GetDlgItem(hwnd,IDC_ProgressBar);//IDC_ProgressBar�ľ������ΪhwSlider
	SendMessage(hwSilder, TBM_SETRANGEMAX, (WPARAM)TRUE, allLength);//TBM_SETRANGEMAX�����û����߼�λ�õ����ֵ�����Ƿ��ػ�
	SendMessage(hwSilder, TBM_SETRANGEMIN, (WPARAM)TRUE, 0);//TBM_SETRANGEMIN�����û����߼�λ�õ���Сֵ�����Ƿ��ػ�
	SendMessage(hwSilder, TBM_SETPOS, (WPARAM)TRUE, curLength);//TBM_SETPOS�����û�����߼�λ�ã����Ƿ��ػ�    



	TCHAR szCommand[MAX_PATH+10];
	TCHAR szReturn[MAX_PATH];
	ZeroMemory(szCommand, sizeof(szCommand)/sizeof(TCHAR));
	//C����������һ���ṹ�����֮����ʹ���������֮ǰҪ������memset���Ѹ���λ���㡣ʹ��C�������õĺ���memset����ɣ�
	//memset(&s1,sizeof(s1),0);
	//����������ʹ��ZeroMemory����������ZeroMemory��ʵ�����Ǻ�������ʵ��memset�ĺ궨��
	//ZeroMemory��ʵ����memset��ֽ�ϻ����ѣ�����һ��ȫ�µĺ�����
	wsprintf(szCommand, TEXT("status %s mode"), shortPath);
	mciSendString(szCommand, szReturn, sizeof(szReturn)/sizeof(TCHAR), NULL);
	//lstrcmp�ַ����Աȣ�����京���ǣ���szReturn==stoppedʱ�Զ�������һ��   */
	if (0 == lstrcmp(szReturn, TEXT("stopped")))
	{											
		autoNext(hwnd);
	}     //������е�ĩβ�� ���Զ�������һ��
} 

//-----------------------------ʱ��ص���������----------------------------------------
void CALLBACK Deskshow(HWND hwnd, UINT message, UINT iTemerID, DWORD dwTime)
{
	rectWndStart.left = WND_ORG_X, rectWndStart.top = WND_ORG_Y;
	rectWndStart.right = rectWndStart.left+WND_WIDTH, rectWndStart.bottom = rectWndStart.top+WND_HEIGHT;
	
	hdc = GetDC(NULL);
	m_hMemdc = CreateCompatibleDC(NULL);	//�����ڴ�DC
	// ע�⣬�˳��򴴽�����ʱ�����岻Ӧȥ��ݣ�������� UpdateLayeredWindow ����ʱ����������������ɫ��
	// ��Ȼ����ȥ��ݣ����԰�����ı���ɫ����Ϊ��������ɫ��������� UpdateLayeredWindow ����ʱ���˵�����ɫ���ɣ�
	// ������OK���������������ɫ�����壬�ʲ��ð죬������岻Ӧȥ��ݡ�
	HFONT hfont = CreateFont(FONT_WIDTH,FONT_HEIGHT,0,0,FW_BOLD,FALSE,FALSE,0, GB2312_CHARSET /*ANSI_CHARSET*/, 
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,NONANTIALIASED_QUALITY/*��ȥ���*/ /*ANTIALIASED_QUALITY ȥ���,ƽ������*/,
		DEFAULT_PITCH | FF_SWISS, "����_GB2312");
	SelectObject(m_hMemdc, hfont);
	HBITMAP hbitmap = CreateCompatibleBitmap(hdc, WND_ORG_X+WND_WIDTH, WND_HEIGHT);
	SelectObject(m_hMemdc, hbitmap);		// ѡ���ڴ�DC
	//	SetBkColor(m_hMemdc,RGB_MEMDC_BRUSH);	// ���ñ�����ɫ
	SetBkMode(m_hMemdc, TRANSPARENT);		// ���ñ���ɫΪ͸��
	hbrushMemDC = CreateSolidBrush(RGB_MEMDC_BRUSH);//��ɫ��ˢ���Ա�ˢ��͸������״̬�µ��ڴ�DC
	SelectObject(m_hMemdc, hbrushMemDC);
	//��ʼ��BLENDFUNCTION�ṹ�壬UpdateLayeredWindow()�����õ��Ĳ����ṹ��
	m_Blend.AlphaFormat = 1;
	m_Blend.BlendFlags = 0;
	m_Blend.BlendOp = 0;
	m_Blend.SourceConstantAlpha = 255;
	int lenLrc = 0, lenSing = 0;	// ��ǰ������ڴ�DC�еĳ��ȣ������ĸ�ʵĳ���
	SIZE tmpSz;
	if(curLength >= ps2->time && curLength <= (ps2+1)->time && ps1 != ps2+1)// ��������ͷ��ʼ�����ҲҪ��ͷ��ʼ
	{
		ps1 = ps2;
	}
	if(curLength >= ps1->time)
	{
		rectLrc.left = rectWndStart.left, rectLrc.right = rectWndStart.right;   
		rectLrc.top = 0, rectLrc.bottom = rectLrc.top+FONT_WIDTH;
		FillRect(m_hMemdc, &rectLrc, hbrushMemDC);                        //������
		
		// ���»�ȡ��ǰ��ʵ��ܳ��ȣ����ֵ����Ҳ����ʹ�� GetTextExtentPoint32 ��ȡ��
					lenLrc = strlen(ps1->lrc) * FONT_HEIGHT;
		GetTextExtentPoint32(m_hMemdc,ps1->lrc,strlen(ps1->lrc),&tmpSz);
		lenLrc = tmpSz.cx;
		rectLrc.left = (rectWndStart.right-rectWndStart.left) / 2 - lenLrc/2;
		rectLrc.right = (rectWndStart.right-rectWndStart.left) / 2 + lenLrc/2;            //��ʾ��εķ�Χ
		
		SetTextColor(m_hMemdc,RGB_TEXT_NOT_SING);	// ����δ�����ĸ����ɫ
		TextOut(m_hMemdc,rectLrc.left,0,ps1->lrc,strlen(ps1->lrc));              //��ӡ��û�����ĸ��
		ps1++;
		lenSing = (curLength-(ps1-1)->time) * 1.0 / (ps1->time-(ps1-1)->time) * (rectLrc.right-rectLrc.left);    //�����ĳ���
		rectSing.left = rectLrc.left, rectSing.right = lenSing+rectSing.left;
		rectSing.top = 0, rectSing.bottom = sz.cy;
		SetTextColor(m_hMemdc,RGB_TEXT_SING);	// ���ó����ĸ����ɫ
		DrawText(m_hMemdc,(ps1-1)->lrc,strlen((ps1-1)->lrc),&rectSing,DT_LEFT);
		
		ptDC.x = rectWndStart.left, ptDC.y = WND_ORG_Y, ptMemDC.x = rectWndStart.left, ptMemDC.y = 0;
				sz.cx = rectWndStart.right-rectWndStart.left, sz.cy = FONT_WIDTH;
		UpdateLayeredWindow(hwnd,hdc,&ptDC,&sz, m_hMemdc,&ptMemDC,RGB_MEMDC_BRUSH,&m_Blend,0x1 );
		int signStart =1;


	}
//	UpdateLayeredWindow(hwnd,hdc,&ptDC,&sz, m_hMemdc,&ptMemDC,RGB_MEMDC_BRUSH,&m_Blend,1);
}


//----------------------ʱ��ص�-EDIT11��ˢ��--------------------------
void CALLBACK LrcEDIT11(HWND hwnd, UINT message, UINT iTemerID, DWORD dwTime)
{	
	tmp2=head2;

	HWND hEDIT11=GetDlgItem(hwnd,IDC_EDIT11);
//	SetWindowText(hwnd,""); //��վɸ��
	if(curLength >= ps2->time && curLength <= (ps2+1)->time && ps1 != ps2+1)// ��������ͷ��ʼ�����ҲҪ��ͷ��ʼ
	{
		ps1 = ps2;
		lrcindex=0;
		//indexE33=-1;
	}

	if(curLength >= lrctime[lrcindex])
	{
	int j;	
	ZeroMemory(geciEDIT11,256*100);
	
	


	SetWindowText(hEDIT11,""); //��վɸ��

//	ZeroMemory(geciEDIT33,256*100);
	for(j=lrcindex+1;j<lrcindex+6;j++)
	{	
		strcat(geciEDIT11,geci[j]);					
	}
	curLengthEDIT11=lrctime[lrcindex+1];
	SetDlgItemText(hwnd,IDC_EDIT11,geciEDIT11);		
	//indexE33=lrcindex;
	lrcindex++;

	}
//	else lrcindex++;	
}
//--------------------------ʱ��ص�-EDIT22��ˢ��----------------------------------
void CALLBACK LrcEDIT22(HWND hwnd, UINT message, UINT iTemerID, DWORD dwTime)
{
	tmp2=head2;
	
	HWND hEDIT22=GetDlgItem(hwnd,IDC_EDIT22);

	
	

//	SetWindowText(hwnd,""); //��վɸ��
	if(curLength >= ps2->time && curLength <= (ps2+1)->time && ps1 != ps2+1)// ��������ͷ��ʼ�����ҲҪ��ͷ��ʼ
	{
		ps1 = ps2;
		lrcindex=0;
	}
	if(curLength > lrctime[lrcindex])
	{
//	int now;
	TCHAR geciEDIT22[256];
	ZeroMemory(geciEDIT22,256);
	wsprintf(geciEDIT22,"%s",geci[lrcindex]);

	SendMessage(hEDIT22, WM_SETFONT, (WPARAM)hfontEDIT22, MAKELPARAM(TRUE,0));
	SetWindowText(hEDIT22,""); //��վɸ��
	SetDlgItemText(hwnd,IDC_EDIT22,geciEDIT22);	
	
	
	
	}


}
//---------------------------ʱ��ص�-EDIT33ˢ��--------------------------------\------
void CALLBACK LrcEDIT33(HWND hwnd, UINT message, UINT iTemerID, DWORD dwTime)
{
	tmp2=head2;
	HWND hEDIT33=GetDlgItem(hwnd,IDC_EDIT33);
	int h;
	SetWindowText(hEDIT33,""); //��վɸ��

	ZeroMemory(geciEDIT33,256*100);
	if(lrcindex<6&&lrcindex>1)
	{
	
//		SetWindowText(hEDIT33,""); //��վɸ��
		
		
		for(h=0;h<=5-lrcindex;h++)
		{
			strcat(geciEDIT33,"\r\n");
		}
		for(h=0;h<lrcindex-1;h++)
		{
			strcat(geciEDIT33,geci[h]);
		}
		curLengthEDIT33=lrctime[lrcindex+1];
		SendMessage(hEDIT33, WM_SETFONT, (WPARAM)hfontEDIT11, MAKELPARAM(TRUE,0));
		SetDlgItemText(hwnd,IDC_EDIT33,geciEDIT33);
	}
	if(lrcindex>=6)
	{
	//	SetWindowText(hEDIT33,""); //��վɸ��
		for(h=lrcindex-6;h<lrcindex-1;h++)
		{
			strcat(geciEDIT33,geci[h]);
		}
		SendMessage(hEDIT33, WM_SETFONT, (WPARAM)hfontEDIT11, MAKELPARAM(TRUE,0));
		SetDlgItemText(hwnd,IDC_EDIT33,geciEDIT33);
	}

}
//----------------------------------���߳� �����--------------------------------------------
DWORD WINAPI ThreadLrcshow(LPVOID lpParam)
{
	HWND hwnd=(HWND)lpParam;
	
	SetTimer(hwnd,2,100,LrcEDIT11);
	SetTimer(hwnd,3,100,LrcEDIT22);
	SetTimer(hwnd,4,100,LrcEDIT33);  
	
	
	
	
	return 0;
}
 //-------------------------Main_Proc  �Ի������Ϣ������-------------------- ------
BOOL WINAPI Main_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
/*	RECT rt11,rt22;	
	HDC  hDCEdit11;
    HWND hWndEdit11;
	HDC  hDCEdit22;
    HWND hWndEdit22;
	GetClientRect(hWnd, &rt11);
	GetClientRect(hWnd, &rt22); */

/*	RECT rt;	
	HDC  hDCEdit;
    HWND hWndEdit;
	GetClientRect(hWnd, &rt);   */





    switch(uMsg)
    {
        HANDLE_MSG(hWnd, WM_INITDIALOG, Main_OnInitDialog);
        HANDLE_MSG(hWnd, WM_COMMAND, Main_OnCommand);
		HANDLE_MSG(hWnd,WM_CLOSE, Main_OnClose);
		HANDLE_MSG(hWnd,WM_HSCROLL, Main_OnHScroll);  // Slider������Ϣ
		

		case WM_MYMESSAGE:
		{
		//	if(lParam == WM_LBUTTONDOWN)     //������ͼ�������  
			if(lParam==WM_LBUTTONDBLCLK)
			{
				ShowWindow(hWnd,SW_SHOW);
			}
			else if(lParam == WM_RBUTTONDOWN)				//������ͼ�����һ�
			{
				HMENU hmenu;			
				hmenu=LoadMenu(hApp,(LPCTSTR)IDR_MENU3);    
				hmenu=GetSubMenu(hmenu,0);   
				POINT pos;
				GetCursorPos(&pos);       
				SetForegroundWindow(hWnd);                       
				TrackPopupMenu(hmenu,TPM_LEFTALIGN|TPM_RIGHTBUTTON,pos.x,pos.y,0,hWnd,NULL);								
			}
		}
		break; 
		case WM_CONTEXTMENU:                //�б��Ҽ������˵�
			{
				
				HWND hwndlst=GetDlgItem(hWnd,IDC_MusicList);
				if((HWND)wParam==hwndlst)
				{                   
					HMENU hmenu;
					hmenu=LoadMenu(hApp,(LPCTSTR)IDR_MENU4);    
					hmenu=GetSubMenu(hmenu,0);       
					SetForegroundWindow(hWnd);  
					TrackPopupMenuEx(hmenu,TPM_LEFTALIGN|TPM_TOPALIGN,
						GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam),hWnd,NULL);
					hmenu1=GetMenu(hWnd);
				}
			}
		break; 
	
		case WM_PAINT:						//�ػ洰�� ��Ϣ��ע������UpdateWindow���ڣ�������Ӧ�ó���ʱ������Ϣִ������
			PAINTSTRUCT ps;					//����洢���ƴ����õ���Ϣ
			BeginPaint(hWnd,&ps);			//���ƴ��ڣ����Զ����PAINTSTRUCT�ṹ��
			
			// �������ַ�ʽ���������»��Ƹ�ʣ�ע��ps1��ָ����һ���ʵģ��������ǻ��Ƶ�ǰ���
			// ��ʽ1��������Ϣ��ע�� ps1--
			signStart==1 ? ps1--,SendMessage(hWnd,WM_TIMER,0,0) : 0;
			// ��ʽ2��ֱ�ӻ��ƣ�ע�� ps1-1
			//			hdc = GetDC(hwnd);
			//			signStart==1 ? TextOut(hdc,0,0,(ps1-1)->lrc,strlen((ps1-1)->lrc)) : 0;
			//			ReleaseDC(hwnd,hdc);
			
			EndPaint(hWnd,&ps);				//��������
			break;
/*		case	WM_CTLCOLORSTATIC:
				{	
			/*	hWndEdit11=(HWND)lParam;
				hDCEdit11=(HDC)wParam;
				SetTextColor(hDCEdit11,RGB(174,221,129));
				SetBkMode(hDCEdit11, TRANSPARENT); //���ֵı���͸��		
				return (LRESULT)SetEditColor11(hWnd,hDCEdit11,hWndEdit11); 
				hWndEdit22=(HWND)lParam;
				hDCEdit22=(HDC)wParam;
				SetTextColor(hDCEdit22,RGB(174,221,129));
				SetBkMode(hDCEdit22, TRANSPARENT); //���ֵı���͸�� 
				return (LRESULT)SetEditColor22(hWnd,hDCEdit22,hWndEdit22);  

					hWndEdit=(HWND)lParam;
					hDCEdit=(HDC)wParam;
					SetTextColor(hDCEdit,RGB(255,0,0));
					SetBkMode(hDCEdit,TRANSPARENT);
					return (LRESULT)SetEditColor(hWnd,hDCEdit,hWndEdit);
			
				}     */
		 
	
    }
	
    return FALSE;
}



//----------------------------��ʼ���Ի���------------------------------------------------------------------
BOOL Main_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	
//	SetTimer(hwnd,1,1000,curTimePro);
	InitFont(hwnd);
	TCHAR cTemp[MAX_PATH],cFullPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, cTemp);
	//ʹ��GetShortPathNameʱ������ȷ��ԭ·�����ļ����ڣ����򷵻ؿ��ִ�
	//��˴˴���ת��·�������������б��ļ��������뵽ȫ�ֱ���DEFAULTLISTFILE�� 
	GetShortPathName(cTemp,cFullPath,MAX_PATH);
	wsprintf(DEFAULTLISTFILE,"%s\\.lst",cFullPath); 

	ReadList(hwnd,DEFAULTLISTFILE);	 //��ȡ�б���Ϣ
	// ��ʼ��ʱ�䣬����ʱ�����Ŀ��ʱ�䶼Ϊ��
	strcpy(strTime, "00:00");
	strcpy(musicTotalTime, "00:00");
	SetDlgItemText(hwnd, IDC_MusicNowTime, strTime);
	SetDlgItemText(hwnd,IDC_MusicTotalTime,musicTotalTime);
	// ��ʼ������
	hwndVolumeSilder= GetDlgItem(hwnd, IDC_Volume);
	SendMessage(hwndVolumeSilder, TBM_SETRANGEMAX, (WPARAM)TRUE, 1000);  // �������Ϊ400
	SendMessage(hwndVolumeSilder, TBM_SETRANGEMIN, (WPARAM)TRUE, 0);  // ��СֵΪ0
//	SendMessage(hwndVolumeSilder, TBM_SETPAGESIZE, 0, (LPARAM)100);	  //���õ����������ֵ
//	SendMessage(hwndVolumeSilder,TBM_SETPAGESIZE, 0, (LPARAM)100);	
	SendMessage(hwndVolumeSilder, TBM_SETPOS, (WPARAM)TRUE,voice);   // ��ʼ��Ϊ����
	//��ʼ������ģʽ
	HWND hwCombo1 = GetDlgItem(hwnd, IDC_PlayMode);
	ComboBox_InsertString(hwCombo1, -1, TEXT("˳�򲥷�"));//indexΪ-1ʱָ����˳�򲥷Ų��뵽�б�ĩβ 
	ComboBox_InsertString(hwCombo1, -1, TEXT("����ѭ��"));//indexΪ-1ʱָ��������ѭ�����뵽�б�ĩβ
	ComboBox_InsertString(hwCombo1, -1, TEXT("�б�ѭ��"));//indexΪ-1ʱָ�����б�ѭ�����뵽�б�ĩβ
	ComboBox_InsertString(hwCombo1, -1, TEXT("�������"));//indexΪ-1ʱָ����������Ų��뵽�б�ĩβ
	ComboBox_SetCurSel(hwCombo1, 0);  //Ĭ��ѡ��˳�򲥷�ģʽ   
	HWND hwButtonPause = GetDlgItem(hwnd, IDC_Pause);  // �õ�IDC_PAUSE��ť�ľ��
	SetWindowText(hwButtonPause, TEXT("4"));  // ����ť�ϵ����ָ�Ϊ�����š�
	
	 hwndlist=GetDlgItem(hwnd,IDC_MusicList);
	  hSearchEdit=GetDlgItem(hwnd,IDC_SearchEdit);
	  hSearchbutton=GetDlgItem(hwnd,IDC_Searchbutton);
	 ShowWindow(hSearchEdit,SW_HIDE);
	 ShowWindow(hSearchbutton,SW_HIDE);
	 HWND hNETSearch=GetDlgItem(hwnd,IDC_EDITsongname);
	 HWND hGO=GetDlgItem(hwnd,IDC_GO);
	 ShowWindow(hNETSearch,SW_HIDE);
	 ShowWindow(hGO,SW_HIDE);



	  

	

	
//	SendMessage(hwndlist, WM_SETFONT, (WPARAM)hfontlistbox, MAKELPARAM(TRUE,0));


    return TRUE;
}
//------------------------------------------------------------------------------------

void Main_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{	
	HMENU hmenu;			
	hmenu=LoadMenu(hApp,(LPCTSTR)IDR_MENU2);    
	hmenu=GetSubMenu(hmenu,0);
	
    switch(id)
    {
        case IDC_AddFile:
			{
			AddFile(hwnd);//����ļ�
			}
			break;
		case IDC_AddFloder://����ļ���
			{
			AddFloder(hwnd);	
			}
			break;
		case IDC_Add:// �����ӣ������˵�ѡ��
			{
			/*	HMENU hmenu;			
				hmenu=LoadMenu(hApp,(LPCTSTR)IDR_MENU2);    
				hmenu=GetSubMenu(hmenu,0);  */ 
				POINT pos;
				GetCursorPos(&pos);       
				SetForegroundWindow(hwnd);                       
				TrackPopupMenu(hmenu,TPM_LEFTALIGN|TPM_BOTTOMALIGN|TPM_RIGHTBUTTON,pos.x,pos.y,0,hwnd,NULL);	
			}
			break;
		case IDC_DeleteAll: //��ղ����б�
			{
			DeleteAll(hwnd);
			}
			break;
		case IDC_LastSong://��һ��
			{
			LastSong(hwnd);
			}
		break;
		case IDC_NextSong://��һ��
			{
			NextSong(hwnd);
			Play(hwnd);
			}
			break;
		case IDC_Skin:   //����ѡ��Ƥ�����Ӵ���
			{
		
			HINSTANCE hInstance=(HINSTANCE)GetWindowLong(hwnd,GWL_HINSTANCE);
		//	DialogBox(hInstance,MAKEINTRESOURCE(IDD_Skin),NULL,Skin_Proc);
			skinindex=	DialogBoxParam(hInstance,MAKEINTRESOURCE(IDD_Skin),NULL,Skin_Proc,(LPARAM)Skin11);
			
				if(0==skinindex)
				{
					wsprintf(Skinroad,"skin\\ľ�ʷ��.she");
				}
				if(1==skinindex)
				{
					wsprintf(Skinroad,"skin\\���̱߿�.she");
				}
				if(2==skinindex)
				{
					wsprintf(Skinroad,"skin\\����A.she");
				}
				if(3==skinindex)
				{
					wsprintf(Skinroad,"skin\\����B.she");
				}
				if(4==skinindex)
				{
					wsprintf(Skinroad,"skin\\�ۺ�A.she");
				}
				if(5==skinindex)
				{
					wsprintf(Skinroad,"skin\\�ۺ�B.she");
				}
				if(6==skinindex)
				{
					wsprintf(Skinroad,"skin\\������.she");
				}
				if(7==skinindex)
				{
					wsprintf(Skinroad,"skin\\������ɫ.she");
				}
				if(8==skinindex)
				{
					wsprintf(Skinroad,"skin\\����.she");
				}
				if(9==skinindex)
				{
					wsprintf(Skinroad,"skin\\ǳ��.she");
				}
				if(10==skinindex)
				{
					wsprintf(Skinroad,"skin\\�Ϻ�.she");
				}
			}
			break;
		case IDC_MusicList:	//˫�� ����						
			{
			if(codeNotify==LBN_DBLCLK)
				{
				Play(hwnd);
				}
			}
			break; 
		case IDC_Location://��λ��ǰ����
			{
			Location(hwnd);
			}
			break;
		case IDC_Pause://��ͣ
			{
			Pause(hwnd);
			if(0==playStatus)
				{
				HWND hwButtonPause = GetDlgItem(hwnd, IDC_Pause);  // �õ�IDC_PAUSE��ť�ľ��
				SetWindowText(hwButtonPause, TEXT("4"));  // ����ť�ϵ����ָ�Ϊ�����š�
				}
			}
			break;
		case IDC_Voice:  //  ����
			{
				Mute(hwnd);
			}
			break;
		case IDC_GetSongMessage://��ȡ������Ϣ
			{
			GetSongMessage(hwnd);
			}
			break;
	
			//----------�����ǹ������̵���ʽ�˵��İ�ť
		case IDR_NextSong:
			{
				NextSong(hwnd);
				Play(hwnd);
			}
			break;
		case IDR_LastSong:
			{
				LastSong(hwnd);
				Play(hwnd);
			}
			break;
		case IDR_Recover:
			{
				ShowWindow(hwnd,SW_SHOW);
			}
			break;
		case IDR_Close:
			{
				SaveList(hwnd,DEFAULTLISTFILE);
				EndDialog(hwnd, 0);
			}
			break;
		case IDR_Pause:
			{
				Pause(hwnd);				
			}
			break;
			//------�������б��Ҽ������˵�	
		case IDR_Play:
			{
				Play(hwnd);
			}
			break;
		case IDR_Delete:
			{
				Delete(hwnd);
			}
			break;
		case IDR_DeleteAll:
			{
				DeleteAll(hwnd);
			}
			break;
		case IDR_GetSongMessage:
			{
				GetSongMessage(hwnd);
			
			}
			break;
		case IDR_Location:
			{
				Location(hwnd);
			}
			break;
		case IDC_KuGou:
			{

			/*	HDC hDClrc=GetDC(hwnd);
				SIZE sz;
				SelectObject(hDClrc,hfontEDIT11);
				SetBkMode (hDClrc, TRANSPARENT) ;
			//	TCHAR j[33]="DSFAFAFDSFAFDS";  
				GetTextExtentPoint32(hDClrc,ps1->lrc,strlen(ps1->lrc),&sz);				
				BeginPath(hDClrc);
				TextOut(hDClrc,310,310,ps1->lrc,strlen(ps1->lrc));			
			EndPath(hDClrc);
			SelectObject (hDClrc, CreateHatchBrush (HS_DIAGCROSS, RGB (255, 0, 0))) ;
			SetBkColor (hDClrc, RGB (0, 0, 255)) ;					
			SetBkMode (hDClrc, OPAQUE) ;
			StrokeAndFillPath (hDClrc) ;
			DeleteObject (SelectObject (hDClrc, GetStockObject (WHITE_BRUSH))) ;
			SelectObject (hDClrc, GetStockObject (SYSTEM_FONT)) ;			
			DeleteObject (hfontEDIT11) ;	 */	
				
				MessageBox(hwnd,singer,singer,MB_OK);
			}
			break;
		case IDC_Extend:
			{
			Extend(hwnd);
			}
			break;
		case IDC_Mini:
			{
			Mini(hwnd);
			}
			break;
		case IDC_SearchLocal:
			{
				ShowWindow(hSearchEdit,SW_SHOW);
				ShowWindow(hSearchbutton,SW_SHOW);
				SetWindowPos(hwndlist,0,122,75,296,380,SWP_NOMOVE|SWP_NOZORDER);
			}
			break;
		case IDC_Searchbutton:
			{
			SearchLocal(hwnd);
			ShowWindow(hSearchEdit,SW_HIDE);
			ShowWindow(hSearchbutton,SW_HIDE);
			SetWindowPos(hwndlist,0,122,75,296,400,SWP_NOMOVE|SWP_NOZORDER);
			}
			break;
		case IDC_NetSearch:
			{
			HWND hNETSearch=GetDlgItem(hwnd,IDC_EDITsongname);
			HWND hGO=GetDlgItem(hwnd,IDC_GO);
			ShowWindow(hNETSearch,SW_SHOW);
			ShowWindow(hGO,SW_SHOW);
			}
			break;
		case IDC_GO:
			{
			HWND hNETSearch=GetDlgItem(hwnd,IDC_EDITsongname);
			HWND hGO=GetDlgItem(hwnd,IDC_GO);

			OnLine(hwnd,"http://mp3.baidu.com/m?f=ms&tn=baidump3&ct=134217728&lf=&rn=&word=%s&lm=-1");
			SetDlgItemText(hwnd,IDC_EDITsongname,"");
			ShowWindow(hNETSearch,SW_HIDE);
			ShowWindow(hGO,SW_HIDE);

			}
	
        default:
		break;
    }
}
//--------------------------���ϵͳ��С����Ϣ--------------------------------------------------����̫�����ʲô�ã������ţ�
void Main_OnSysCommand(HWND hwnd, UINT cmd, int x, int y)
{
	if(SC_MINIMIZE == cmd)
	{
	 ShowWindow(hwnd,SW_HIDE);
	 return;
	}
}


//----------------------------------�رմ���---------------------------------------------------
void Main_OnClose(HWND hwnd)
{	
	int ret = MessageBox(hwnd, TEXT("��С��������"), TEXT("��ʾ"), MB_YESNO|MB_ICONQUESTION);
	if(IDYES == ret)
	{
		MinimizeToTray(hwnd);
		
		ShowWindow(hwnd,SW_HIDE);
	}
	else if(IDNO == ret)
	{  
	SaveList(hwnd,DEFAULTLISTFILE);
	 
    EndDialog(hwnd, 0);

	}
}




//------------------------------��������--------------------------------------------------------------
//-----------------------------�������--------------------------------------------------------
void AddFile(HWND hwnd)
{
									// ���б�����ʾ����������
	int flagJudgeRepeat = 0;										// �ж�Ҫ��ӵĸ����Ƿ��Ѿ������б��еı�־λ
	ZeroMemory(&ofn,sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = szOpenFileNames;
	ofn.lpstrFile[0] = TEXT('\0');
	ofn.nMaxFile = sizeof(szOpenFileNames);
	ofn.lpstrFilter = TEXT("����֧���ļ�(*.wav,*.mp3,*.wma)\0*.wav;*.mp3;*.wma\0wave�ļ�(*.wav)\0*.wav\0mp3�ļ�(*.mp3)\0*.mp3\0wma�ļ�(*.wma)\0*.wma\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.hwndOwner = hwnd;
	ofn.Flags = OFN_EXPLORER |OFN_PATHMUSTEXIST | OFN_ALLOWMULTISELECT;
	if (GetOpenFileName(&ofn)) //GetOpenFileName�������أ�����ֵ��BOOL������TRUE��ʾ�û�ѡ�����ļ����������FALSE�ͱ�ʾ�û�ѡ���ˡ�ȡ������ť
	{
	

		
			lstrcpyn(szPath, szOpenFileNames, ofn.nFileOffset);

			szPath[ofn.nFileOffset] = '\0';
			int nLen = lstrlen(szPath);

			if( szPath[nLen-1] != '\\' )								//���ѡ�˶���ļ�,��������'\\'
			{
				lstrcat(szPath, TEXT("\\"));
			}

			char *p = szOpenFileNames + ofn.nFileOffset;				//��ָ���Ƶ���һ���ļ�
			
			while(*p)
			{   
				lstrcpy(szFileName_Path[numOnOpenFile],szPath);			//��·�����Ƹ�szFileName_Path
				lstrcat(szFileName_Path[numOnOpenFile],p);				//�����ļ���   
				numOnList++;											// �б�����ʾ����ż�һ
				
				for (int i=0; i<numOnOpenFile; i++)
				{
					// �ж��Ƿ����ظ��ĸ���
					if (0 == strcmp(szFileName_Path[numOnOpenFile],szFileName_Path[i]))
					{
						numOnOpenFile--;	
						numOnList--;		 
						flagJudgeRepeat = 1;							// �ظ���־λ��1		
					} 
				}
				if (0 == flagJudgeRepeat)								// û���ظ�   
				{	
					HWND hwndlist = GetDlgItem(hwnd, IDC_MusicList);//�õ��б����
					TCHAR drive[_MAX_DRIVE];
					TCHAR dir[_MAX_DIR];
					TCHAR fname[_MAX_FNAME];
					TCHAR ext[_MAX_EXT];
					_splitpath(szFileName_Path[numOnOpenFile], drive, dir, fname, ext );  //���ļ�ȫ·���ֽ�
					
					//wsprintf(songname,"%s",fname);
				
					int ListCount=ListBox_GetCount(hwndlist);//�õ��б����Ŀ��
				
				
					ListBox_InsertString(hwndlist,ListCount,fname);  //���б�����ļ���	
				
					p += lstrlen(p) +1;									//������һ���ļ�
					numOnOpenFile++; 
				}
				else
				{
					flagJudgeRepeat = 0;
					p += lstrlen(p) +1;									//������һ���ļ�
					numOnOpenFile++;  
				}
			}
	}	
}


//-------------------------����ļ���--------------------------------------------------
void AddFloder(HWND hwnd)
{
		TCHAR pBuffer_Directory[MAX_PATH];								//����ļ���Ϣ
		BROWSEINFO sBrowseInfo;
		LPCITEMIDLIST lpitem;
		WIN32_FIND_DATA sfd;

		ZeroMemory(&sBrowseInfo,sizeof(BROWSEINFO));
		sBrowseInfo.hwndOwner=hwnd;
		sBrowseInfo.lpszTitle="��ѡ���ļ���";
		sBrowseInfo.ulFlags=BIF_RETURNONLYFSDIRS;

		lpitem=SHBrowseForFolder(&sBrowseInfo);
		if (NULL==lpitem)												//�ж��Ƿ��Ѿ�ѡ���ļ���
		return;

		SHGetPathFromIDList(lpitem,pBuffer_Directory);					//����ļ��е�·��
			
		TCHAR szFile[MAX_PATH];
		strcpy(szFile,pBuffer_Directory);
//		TCHAR *postfixName[3]={"\\*.mp3","\\*.wav","\\*.wma"};   //3�������ʾ3�ֺ�׺��(�ȷ��ţ������Ժ��ܲ���ʵ��wav ��wma��ö�ٹ���)
//		TCHAR *pBuffer_Directory1[3];//������һ������ǰ��pBuffer_Directory
/*		int i; //����ѭ���ṹ
		for(i=0;i<2;i++)
		{   */
	

		TCHAR szFile_ToShow[256];  //��ʾ���б��ϵ��ַ���

		lstrcat(pBuffer_Directory,"\\*.mp3");							//ö�ٹ�������(ֻʵ��MP3)
		HANDLE hfindfile=FindFirstFile(pBuffer_Directory,&sfd);
		if (INVALID_HANDLE_VALUE==hfindfile)							//�ж��ļ��Ƿ���Ч
		return;

		
		do
		
			{
							
			if (sfd.cFileName[0]=='.')
			continue;
			lstrcat(szFileName_Path[numOnOpenFile],szFile);
			lstrcat(szFileName_Path[numOnOpenFile],"\\");
			lstrcat(szFileName_Path[numOnOpenFile],sfd.cFileName);     //sfd.cFileName�� �ļ��������к�׺��
			lstrcat(szFileName,szFile);
			lstrcat(szFileName,"\\");
			lstrcat(szFileName,sfd.cFileName); 
		
			numOnList++;
			HWND hwndlist = GetDlgItem(hwnd, IDC_MusicList);//�õ��б����
			TCHAR drive[_MAX_DRIVE];
			TCHAR dir[_MAX_DIR];
			TCHAR fname[_MAX_FNAME];
			TCHAR ext[_MAX_EXT];
        	_splitpath( szFileName, drive, dir, fname, ext );  //���ļ�ȫ·���ֽ� */
			
		
			int ListCount=ListBox_GetCount(hwndlist);//�õ��б����Ŀ�� 
			wsprintf(szFile_ToShow,"%d. %s",numOnList,fname);	//���ļ�ǰ�ӱ��
			
				ListBox_InsertString(hwndlist,ListCount,fname);  //���б�����ļ���			
			
			
			numOnOpenFile++;	
			}
	
		while (FindNextFile(hfindfile,&sfd));

		
}
//----------------------------------��������-----------------------------------------------------
void Play(HWND hwnd)
{
	Stop(hwnd);// ��ֹͣ��ǰ����
	HWND hwButtonPause = GetDlgItem(hwnd, IDC_Pause);  // �õ�IDC_PAUSE��ť�ľ��
	SetTimer(hwnd,1,1000,curTimePro);

	HWND hEDIT11=GetDlgItem(hwnd,IDC_EDIT11);
	HWND hEDIT22=GetDlgItem(hwnd,IDC_EDIT22);
	HWND hEDIT33=GetDlgItem(hwnd,IDC_EDIT33);
	HWND hcannotshow=GetDlgItem(hwnd,IDC_Cannotshow);
//	SetWindowText(hEDIT33,""); //����֮ǰ�������һ�� ���
//	SetWindowText(hwnd,""); //����֮ǰ�������һ�� ���
	SetWindowText(hEDIT33,""); //����֮ǰ�������һ�� ���
	SetWindowText(hEDIT22,""); //����֮ǰ�������һ�� ���
	SetWindowText(hEDIT11,""); //����֮ǰ�������һ�� ���
	
//	TCHAR str1[256] = "";
//	TCHAR str2[256] = "";
//	TCHAR shortPath[MAX_PATH];
	HWND hwndlist = GetDlgItem(hwnd, IDC_MusicList);  // �õ�ListBox���

	
		
		TCHAR str1[256] = "";
		TCHAR str2[256] = "";
		int index = ListBox_GetCurSel(hwndlist);
		ListBox_GetText(hwndlist, index, str1);  // �õ�������
		GetShortPathName(szFileName_Path[index], shortPath, sizeof(shortPath));  // ��ö�·��
		GetShortPathName(szFileName_Path[index], (Sset.shortPathtoChild), sizeof(Sset.shortPathtoChild));  // ��ö�·���͵������á�����
		wsprintf(str2, "play %s", shortPath);
		numPlayMusicNow=index;
		mciSendString(str2, "", 0, NULL);  // ����ѡ�и���
		playStatus=1;  //����Ϊ���ڲ���״̬
		SetWindowText(hwButtonPause, TEXT(";"));  // ����ť�ϵ���������Ϊ����ͣ��
		MusicTotalTime(hwnd);
		SendMessage (GetDlgItem(hwnd,IDC_MusicList), LB_GETTEXT, numPlayMusicNow,(LPARAM)str1);
		SetDlgItemText(hwnd, IDC_PlayEdit, str1);					// ��ʾ���ڲ��Ÿ���������
		voice = SendMessage(GetDlgItem(hwnd,IDC_Volume), TBM_GETPOS, 0, 0); //�õ���ǰ���������ֵ
		//TBM_GETPOS�����ص�ǰ����λ���������ֵ���߼�λ�ã�
		TCHAR szVoice[256];
		wsprintf(szVoice, "setaudio %s volume to %d", shortPath, voice); //���������õ������ֵ
		mciSendString(szVoice, (LPTSTR)NULL, 0, 0);	
		wsprintf(songname,"%s",str1);  //�õ����������ŵ�ȫ�ֱ����������

	BOOL FLAG;
	FLAG=JudgeLrc();
	if(FLAG==TRUE)   //�ж��Ƿ��и���ļ����в���ʾ
	{	

		SetWindowText(hEDIT22,"");

		ShowEDIT11(hwnd);
	/*	SetTimer(hwnd,2,100,LrcEDIT11);
		SetTimer(hwnd,3,100,LrcEDIT22);
		SetTimer(hwnd,4,100,LrcEDIT33);  */
	
		ShowWindow(hEDIT11,SW_SHOWNA);
		ShowWindow(hEDIT22,SW_SHOWNA);
		ShowWindow(hEDIT33,SW_SHOWNA);
		ShowWindow(hcannotshow,SW_HIDE);
		CreateThread(NULL,0,ThreadLrcshow,hwnd,0,0);
		SendMessage(hEDIT11, WM_SETFONT, (WPARAM)hfontEDIT11, MAKELPARAM(TRUE,0));
		
	//	SetTimer(hwnd,5,100,Deskshow);
	
		
	}
	else if(FALSE==FLAG)
	{
//	SendMessage(hEDIT11, WM_SETFONT, (WPARAM)hfontEDIT11, MAKELPARAM(TRUE,0));
//	SetWindowText(hEDIT33,""); //����֮ǰ�������һ�� ���
//	SetWindowText(hEDIT22,""); //����֮ǰ�������һ�� ���
//	SetWindowText(hEDIT11,""); //����֮ǰ�������һ�� ���
	ShowWindow(hEDIT11,SW_HIDE);
	ShowWindow(hEDIT22,SW_HIDE);
	ShowWindow(hEDIT33,SW_HIDE);
	ShowWindow(hcannotshow,SW_SHOW);
	SetDlgItemText(hwnd, IDC_Cannotshow, "�޷��ҵ���ʣ����ֶ�����");


//	analyzeFirstXML(hwnd);
	SendMessage(hcannotshow, WM_SETFONT, (WPARAM)hfontcannotshow, MAKELPARAM(TRUE,0));
//	Sleep(1);
	}



}
//--------------------------------��ͣ------------------------------------------------------------
void Pause(HWND hwnd)
{
	TCHAR str1[256] = "";
	TCHAR str2[256] = "";
	TCHAR shortPath[MAX_PATH];
	HWND hwndlist = GetDlgItem(hwnd, IDC_MusicList);  // �õ�ListBox���
	HWND hwButtonPause = GetDlgItem(hwnd, IDC_Pause);  // �õ�IDC_PAUSE��ť�ľ��
	int index = ListBox_GetCurSel(hwndlist);//�õ���ǰѡ�������
	ListBox_GetText(hwndlist, index, str1);  // �õ�������
	wsprintf(songname,"%s",str1);
	
	GetShortPathName(szFileName_Path[numPlayMusicNow], shortPath, sizeof(shortPath));  // ��ö�·��
	if (1 == playStatus)  // ������ڲ���״̬
	{
		//	SetWindowText(hwButtonPause, TEXT("����"));  // ����ť�ϵ����ָ�Ϊ��������
		
		wsprintf(str2, "pause %s", shortPath);  // pause����
		mciSendString(str2, "", 0, NULL);  // ��ͣѡ�и���  */
		playStatus=0;				
	}
	
	else if(0==playStatus)
	{
		SetWindowText(hwButtonPause, TEXT(";"));  // ����ť�ϵ����ָ�Ϊ����ͣ��
		wsprintf(str2, "resume %s", shortPath);  // resume����Ϊ��������
		mciSendString(str2, "", 0, NULL); 
		playStatus=1;
		
	}  
	
	
}
//--------------------------------ֹͣ����------------------------------------------------------------
void Stop(HWND hwnd)
{
		TCHAR str[256];
		TCHAR shortPath[MAX_PATH];
		GetShortPathName(szFileName_Path[numPlayMusicNow], shortPath, sizeof(shortPath));
		wsprintf(str, "close %s", shortPath);
		mciSendString(str, "", 0, NULL);
		playStatus=2;


} 
//--------------------------------������һ��------------------------------------------------------------
void LastSong(HWND hwnd)
{
		Stop(hwnd);
		TCHAR str1[256] = "";
		TCHAR str2[256] = "";

		HWND hwndlist = GetDlgItem(hwnd, IDC_MusicList);  // �õ�ListBox���
		int index = ListBox_GetCurSel(hwndlist);
		int all = ListBox_GetCount(hwndlist);  // �б�������
		if (index > 0)
		{
			ListBox_GetText(hwndlist, index-1, str1);  // �õ�������
			ListBox_SetCurSel(hwndlist, index-1);
		}
		else//��Ϊ��һ�׸裬��index==0ʱ���������һ��
		{
			ListBox_GetText(hwndlist, all-1, str1);  // �õ�������
			ListBox_SetCurSel(hwndlist, all-1);
		}	

}
//--------------------------------������һ��------------------------------------------------------------
void NextSong(HWND hwnd)
{
		Stop(hwnd);
		TCHAR str1[256] = "";
		TCHAR str2[256] = "";

		HWND hwndlist = GetDlgItem(hwnd, IDC_MusicList);  // �õ�ListBox���
		int index = ListBox_GetCurSel(hwndlist);
		int all = ListBox_GetCount(hwndlist);  // �б�������
		if (index ==all-1)   //�����ѡ���б����һ��
		{
			ListBox_GetText(hwndlist, 0, str1);  // �õ�������
			ListBox_SetCurSel(hwndlist, 0);
		}
		else
		{
			ListBox_GetText(hwndlist, index+1, str1);  // �õ�������
			ListBox_SetCurSel(hwndlist, index+1);
		}	

}
//--------------------------------��¼һ�׸���ʱ��------------------------------------------------------------
void MusicTotalTime(HWND hwnd)
{	
//	TCHAR musicTotalLength[256];
	int intSecond,intMinute;
	TCHAR str[256];
	TCHAR shortPath[MAX_PATH];
	GetShortPathName(szFileName_Path[numPlayMusicNow], shortPath, sizeof(shortPath));
	wsprintf(str, "status %s length", shortPath);

	mciSendString(str,musicTotalLength,sizeof(musicTotalLength)/sizeof(TCHAR),NULL);//��ø����ĳ���
	totalSecond = atoi(musicTotalLength)/1000;						//��ʱ�䣨���룩����1000�õ��������������� 

	intMinute = totalSecond / 60;									//����������6�õ��������������� 
	intSecond = (totalSecond - intMinute*60);						//��������ȥ������������*60�õ�ʣ������ 

	wsprintf(musicTotalTime,"%02d:%02d",intMinute,intSecond);		//02s��ʾʱ����ӻ���������λ�����㲹0�� 
	SetDlgItemText(hwnd,IDC_MusicTotalTime,musicTotalTime);
}  


//------------------------------���麯��������������--------------------------------
void Main_OnHScroll(HWND hwnd, HWND hwndCtl, UINT code, int pos)
{    
	SetTimer(hwnd,1,1000,curTimePro);
	TCHAR shortPath[MAX_PATH];
	GetShortPathName(szFileName_Path[numPlayMusicNow], shortPath, sizeof(shortPath));
	switch(GetDlgCtrlID(hwndCtl)) //�õ��ؼ�ID
	{ 
		//������������
	case IDC_Volume:
		{        
			voice = SendMessage(GetDlgItem(hwnd,IDC_Volume), TBM_GETPOS, 0, 0); //�õ���ǰ���������ֵ
				//TBM_GETPOS�����ص�ǰ����λ���������ֵ���߼�λ�ã�
			TCHAR szVoice[256];
			wsprintf(szVoice, "setaudio %s volume to %d", shortPath, voice); //���������õ������ֵ
			mciSendString(szVoice, (LPTSTR)NULL, 0, 0);	
		}
		break;
	case IDC_ProgressBar: //�����Ľ�����
		{
			
			if(SB_ENDSCROLL == code)
			{
				long lPosition = SendMessage(GetDlgItem(hwnd,IDC_ProgressBar), TBM_GETPOS, 0, 0); //�õ���ǰ�Ľ���ֵ
				//itoa(lPosition, curTime, 10);
				voice = SendMessage(GetDlgItem(hwnd,IDC_Volume), TBM_GETPOS, 0, 0); //�õ���ǰ���������ֵ
				//TBM_GETPOS�����ص�ǰ����λ���������ֵ���߼�λ�ã�
				TCHAR strPos[256];
				wsprintf(strPos, "play %s from %ld", shortPath, lPosition);
				mciSendString(strPos, (LPTSTR)NULL, 0, 0);												    
				mciSendString(strPos, (LPTSTR)NULL, 0, 0); 
				voice = SendMessage(GetDlgItem(hwnd,IDC_Volume), TBM_GETPOS, 0, 0); //�õ���ǰ���������ֵ
				//TBM_GETPOS�����ص�ǰ����λ���������ֵ���߼�λ�ã�
				TCHAR szVoice[256];
				wsprintf(szVoice, "setaudio %s volume to %d", shortPath, voice); //���������õ������ֵ  �ٴ���������Ϊ�˷�ֹ��ת���ȵ�ʱ������������
				mciSendString(szVoice, (LPTSTR)NULL, 0, 0);	
			
				
			}
        }
		break;
	}
}

//-------------------------------�Զ�ת����һ��---------------------------------------------
void autoNext(HWND hwnd)
{
	srand(time(0));
	mode = ComboBox_GetCurSel(GetDlgItem(hwnd, IDC_PlayMode));  // ��ȡ��ǰ�Ĳ���ģʽ
	Stop(hwnd);
	TCHAR str1[256] = "";
	TCHAR str2[256] = "";
	HWND hwndlist = GetDlgItem(hwnd, IDC_MusicList);  // �õ�ListBox���
	int index = ListBox_GetCurSel(hwndlist);  //�õ��������ڲ��ŵĸ���
	int all = ListBox_GetCount(hwndlist);  // �б�������
	if (1 == mode)   //����ǵ���ѭ��
	{
		ListBox_GetText(hwndlist, (index)%all, str1);   //str1==0
		ListBox_SetCurSel(hwndlist, index);
		Play(hwnd);
	}

	if (2 == mode)
	{
		NextSong(hwnd);
		Play(hwnd);
		
	}
	if (3 == mode)
	{
		int iRand = rand()%all;
		ListBox_GetText(hwndlist, iRand, str1);
		ListBox_SetCurSel(hwndlist, iRand);
		Play(hwnd);
	}
	else if(0==mode)
	{

		if (index ==all-1)   //�����ѡ���б����һ��
		{
			ListBox_GetText(hwndlist, index, str1);  // �õ�������
			ListBox_SetCurSel(hwndlist, index);
			Stop(hwnd);
		}
		else
		{
			ListBox_GetText(hwndlist, index+1, str1);  // �õ�������
			ListBox_SetCurSel(hwndlist, index+1);
			Play(hwnd);
		}	
	}


} 
//------------------------�����б����Ϣ��Ӳ��----------------------------------------
void SaveList(HWND hwnd, TCHAR *cFileName)
{
	TCHAR fileName[MAX_PATH];
	FILE *fp = fopen(cFileName,"wb");				// �򿪱�������ļ�  
    if(fopen(cFileName,"wb")==NULL)
    {
		MessageBox(hwnd,"���������б�ʧ�ܣ�","��Ǹ",MB_OK|MB_ICONEXCLAMATION); 
	}
	else
	{		
		fwrite(&numOnOpenFile,sizeof(int),1,fp);		// ���������������Ϣ
		fwrite(&mode,sizeof(int),1,fp);					// ���沥��ģʽ
		fwrite(&numOnList,sizeof(int),1,fp);			// �����¼�б�����ʾ���������
		fwrite(&voice,sizeof(int),1,fp);				// �������� 
	//	fwrite(&iTransparencyType,sizeof(int),1,fp);	// ����͸���� 
		fwrite(&Skinroad,sizeof(Skinroad),1,fp);        // ����Ƥ��
		fread(&(Sset.soundmodeparent),sizeof(int),1,fp);		// ��������ģʽ
		for (int i=0; i<numOnOpenFile; i++)				// �������и�����·�������� 
		{
			SendMessage(GetDlgItem(hwnd,IDC_MusicList),LB_GETTEXT,i,(LPARAM)fileName);
			fwrite(&fileName,sizeof(fileName),1,fp);
			fwrite(&szFileName_Path[i],sizeof(szFileName_Path[i]),1,fp);
		}
	}
	fclose(fp);
}






//---------------------------��С��������------------------------�о����Ǻܺ��ã��������Ժ�Ľ�-----------------
void MinimizeToTray(HWND hwnd) 
{
	NOTIFYICONDATA nid;
	nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	nid.hWnd = hwnd;
	nid.uID = IDI_ICON;
	nid.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP;
	nid.uCallbackMessage = WM_MYMESSAGE;								//�Զ�����Ϣ
	HINSTANCE hin = (HINSTANCE)GetWindowLong(hwnd,GWL_HINSTANCE);		//��ó�����
	nid.hIcon = LoadIcon(hin,MAKEINTRESOURCE(1));
	lstrcpy(nid.szTip,TEXT("MPlayer"));

	Shell_NotifyIcon(NIM_ADD,&nid);
	ShowWindow(hwnd,SW_HIDE);											//���ش���
}
//-----------------------��ȡ�б���Ϣ----------------------------------------
void ReadList(HWND hwnd, TCHAR *cFileName)
{
	TCHAR fileName[MAX_PATH];
	FILE *fp = fopen(cFileName,"rb");
    if (fopen(cFileName,"rb")==NULL)
    {
		MessageBox(hwnd,"��ȡ�����б�ʧ�ܣ�","��ʾ",MB_OK|MB_ICONEXCLAMATION);
	}
    else
    {
		fread(&numOnOpenFile,sizeof(int),1,fp);			// ��ȡ������������Ϣ
		fread(&mode,sizeof(int),1,fp);				// ��ȡ����ģʽ
		fread(&numOnList,sizeof(int),1,fp);				// ��ȡ��¼�б�����ʾ���������
		fread(&voice,sizeof(int),1,fp);				// ��ȡ����
//		fread(&iTransparencyType,sizeof(int),1,fp);		// ��ȡ͸����
		fread(&Skinroad,sizeof(Skinroad),1,fp);	       //��ȡƤ��
		fread(&(Sset.soundmodeparent),sizeof(int),1,fp);				// ��ȡ����ģʽ
		SkinH_AttachEx(Skinroad,"1");
		SendMessage(hwndVolumeSilder, TBM_SETPOS, (WPARAM)TRUE,voice); 

		for (int i=0;i<numOnOpenFile;i++)
		{
			fread(&fileName,sizeof(fileName),1,fp);
			HWND hwndlist=GetDlgItem(hwnd,IDC_MusicList);
			int ListCount=ListBox_GetCount(hwndlist);
			ListBox_InsertString(hwndlist,ListCount,fileName);
			//SendMessage(GetDlgItem(hwnd,IDC_MusicList),LB_ADDSTRING,0,(LPARAM)fileName);   
			fread(&szFileName_Path[i],sizeof(szFileName_Path[i]),1,fp);	    
		}
	}
	fclose(fp);
}

//_------------------------------------ɾ��ѡ�и���--------------------------------------------	  
	void Delete(HWND hwnd)
{
	HWND hwndlist = GetDlgItem(hwnd, IDC_MusicList);
	int selCount = ListBox_GetCurSel(hwndlist);
	if (selCount ==numPlayMusicNow)  // ���ɾ�����ǵ�ǰ���ڲ��ŵģ�ֹͣ�������֣�ѡ����һ��
	{
		Stop(hwnd);
	}
	int i;
	for (i = selCount; i < ListBox_GetCount(hwndlist) - 1; i++)  // ����ŵ�·����Ϣ����ǰ��
	{
		strcpy(szFileName_Path[i], szFileName_Path[i+1]);
	}
	strcpy(szFileName_Path[i], "");// ��ԭ��������Ϊ�� 
	ListBox_DeleteString(hwndlist, selCount);  // ���б���ɾ��ѡ����
	if(numPlayMusicNow== selCount)//���ڲ��ŵĸ�����ŵ���Ҫɾ���ĸ�����ţ�ѡ���б��е�һ������
	{
		numPlayMusicNow=0;
		ListBox_SetCurSel(hwndlist,numPlayMusicNow);  // ѡ���б������ڲ��ŵĸ���
	}
	if (numPlayMusicNow > selCount)//���ڲ��ŵĸ�����Ŵ���Ҫɾ���ĸ�����ţ���Ȼѡ���б������ڲ��ŵĸ���
	{
		numPlayMusicNow--;
		ListBox_SetCurSel(hwndlist,selCount);  // ѡ���б������ڲ��ŵĸ���
	}
	ListBox_SetCurSel(hwndlist,selCount);  // ѡ���б������ڲ��ŵĸ��� */
	numOnOpenFile--;  
}  
//----------------------��ղ����б�------------------------------------------
void DeleteAll(HWND hwnd)
{
	HWND hwndlist = GetDlgItem(hwnd,IDC_MusicList);
	int count = ListBox_GetCount(hwndlist);
	int i;   //����ѭ���ṹ�����

	for(i=count;i>=0;i--)
	{
	ListBox_DeleteString(hwndlist,i);	
	}  
	Stop(hwnd);
	strcpy(strTime, "00:00");
	strcpy(musicTotalTime, "00:00");
	SetDlgItemText(hwnd, IDC_MusicNowTime, strTime);
	SetDlgItemText(hwnd,IDC_MusicTotalTime,musicTotalTime);    //��ո���ʱ����ʾ
	numOnOpenFile = 0;									// ��ԭ����¼���ļ�ʱ������������
	numOnList = 0;										// ��ԭ���б��������
	numPlayMusicNow = 0;	                            // ��ԭ�����ڲ��ŵĸ���������
	SendDlgItemMessage(hwnd,IDC_ProgressBar,TBM_SETPOS,true,(LPARAM)NULL);		// ���������ϻؿ�ʼλ��
	SetDlgItemText(hwnd, IDC_PlayEdit,TEXT(""));   // ���ڲ��Ÿ�����Ŀ ���
	
	FILE *fp;				//���ڲ���������б�
	if((fp=fopen("C:\\Users\\Administrator\\Documents\\�б���.doc","w"))==NULL)
	{
		MessageBox(hwnd,"����б�ʧ��","��Ǹ",MB_OK|MB_ICONEXCLAMATION); 
		exit(0);
	}
	fclose(fp);

}
//------------------------------��ȡ������Ϣ--------------(��ʱ����ID3V1)-----------------------
void GetSongMessage(HWND hwnd)
{	
	HWND hwndlist = GetDlgItem(hwnd, IDC_MusicList);
	int index=ListBox_GetCurSel(hwndlist);
	TCHAR str[256];
	struct tagID3V1
	{
		char Header[4];  // ��ǩͷ�����ǡ�TAG��������Ϊû�б�ǩ
		char Title[31];  // ����
		char Singer[31];  // ����
		char Album[31];  // ר��

	}ID3V1;

	FILE *fp;
	fp = fopen(szFileName_Path[index], "r");
	fseek(fp,-128L,SEEK_END);
	fread(ID3V1.Header, 3, 1, fp);

	// �жϱ�ǩͷ���ޡ�TAG����û������Ϊû�б�ǩ
	if (0 == memcmp(ID3V1.Header, "TAG", 3))
		{
		ID3V1.Header[3] = '\0';

		fread(ID3V1.Title, 30, 1, fp);
		ID3V1.Title[30]='\0';

		fread(ID3V1.Singer, 30, 1, fp);
		ID3V1.Singer[30]='\0';

		fread(ID3V1.Album, 30, 1, fp);
		ID3V1.Album[30]='\0';	    
			
		wsprintf(str, "���� %s\n\n���� %s\n\nר�� %s\n", ID3V1.Title, ID3V1.Singer, ID3V1.Album);
		MessageBox(hwnd, str, TEXT("��Ŀ��Ϣ"), MB_OK);
		}
	else
		{

			MessageBox(hwnd, TEXT("�˸�����δ����ID3V1��ǩ"), TEXT("��Ŀ��Ϣ"), MB_OK);
		}
	fclose(fp);
	
}
//--------------------------��λ��ǰ����---------------------------------------------------------
void Location(HWND hwnd)
{
HWND hwndlist = GetDlgItem(hwnd,IDC_MusicList);
ListBox_SetCurSel(hwndlist,numPlayMusicNow);
}
//----------------------------�жϸ���Ƿ����-------------------------------------
BOOL JudgeLrc(void)
{
	wsprintf(lyrics,"lrc\\%s.lrc",songname);
	wsprintf(lyricsX,"..\\%s",lyrics);
	FILE *fp;
	
	if((fp=fopen(lyrics,"r")) == NULL && (fp=fopen(lyricsX,"r")) == NULL)
	{	
		return FALSE;
		
		
	} 
	else 
	{
		return TRUE;
		
	}
	
	fclose(fp);
}

//-----------------------------�������--------------------------------------------
void LRC(void)
{
	//�򿪸���ļ�
//	TCHAR lyrics[256];  //��ǰĿ¼·��+����ļ���+��׺
//	TCHAR lyricsX[256]; //  ���������ƣ�ֻ��������һ��Ŀ¼
	//wsprintf(lyrics,"lrc\\%s.lrc",songname);
	//wsprintf(lyricsX,"..\\%s",lyrics);
	FILE *fp;

	if((fp=fopen(lyrics,"r")) == NULL && (fp=fopen(lyricsX,"r")) == NULL)
	{			
		MessageBox(NULL,"��ʴ򿪴���","����",MB_OK);
		exit(1);//exit(1)�����쳣�˳���exit(0)���������˳�
	} 
	//��̬�����ڴ�,�洢����ĸ����Ϣ
	if((p=(char *)calloc(1024,sizeof(char))) == NULL)//calloc������ڴ棨���ڴ棩�Զ���ʼ��Ϊ0
	{
		MessageBox(NULL,"�޷������ڴ�ռ䣡","����",MB_OK);
		exit(1);
	}
	//��̬��������ַ����������ռ�
	if((ps2 = ps1 = (struct song *)calloc(120,sizeof(struct song)))==NULL)
	{
		MessageBox(NULL,"�޷������ڴ�ռ䣡","����",MB_OK);
		exit(1);
	}
	
	head1 = p;
	head2 = ps1;
	int flag;
	flag = 1;
	while(!feof(fp))
	{
		p = head1;//ÿ�ζ�ȡ����ʼ��p
		fscanf(fp,"%c",p);
		while(!(*p==10))
		{
			p++;
			fscanf(fp,"%c",p);
		}
		*p = 0;
		p = head1;
		while(*p != 0)//���û��ǰ�ߵ�������Ϣ��������
		{
			if(*p==':' && *(p+1)==']')
			{
				p=head1;
				flag=0;
				break;
			}
			p++;
			flag=1;
		}		
		if(flag == 1)//��ʿ�ʼ
		{
			flag = 0;
			p = head1;
			while(*p != 0)
			{
				if(*p==']' && *(p+1)!='[')
					
				{
					poslrc = p+1;
					break;
				}
				p++;
			}
			p = head1;
			while(*p != '[')	p++;
			while(*p=='[' && *(p+1) == '0')//ʱ�����
			{
				if(*(p+6) == '.')	
				{
					(*ps1).time = (*(p+1)-'0')*600000+(*(p+2)-'0')*60000+(*(p+4)-'0')*10000
						+(*(p+5)-'0')*1000+(*(p+7)-'0')*100+(*(p+8)-'0')*10;
					p+=10;
				}
				else
				{	
					(*ps1).time = (*(p+1)-'0')*600000+(*(p+2)-'0')*60000+(*(p+4)-'0')*10000
						+(*(p+5)-'0')*1000;
					p+=8;
				}
				strcpy(ps1->lrc,poslrc);
				ps1++;
			}
		}
	}	
	fclose(fp);

	//�������ð�ݷ���
	long tmptime;
	char tmplrc[100];
	for(tmp=head2; tmp<ps1-1; tmp++)
		for(tmp2=head2; tmp2<ps1-1; tmp2++)
		{
			if((*tmp2).time > (tmp2+1)->time)
			{
				tmptime = (*tmp2).time;		strcpy(tmplrc,(*tmp2).lrc);
				(*tmp2).time = (tmp2+1)->time;	strcpy((*tmp2).lrc,(tmp2+1)->lrc);
				(tmp2+1)->time = tmptime;	strcpy((tmp2+1)->lrc,tmplrc);
			}
		}
		ps1=head2;

	int i;			
	//TCHAR geci[256][256];  //��ά����Ѹ�ʱ�������
	 k=tmp2-head2;    //һ����K�и��
	for(i=0;i<k;i++)
	{
		wsprintf(geci[i],"%s\r\n",ps1->lrc);
		lrctime[i]=ps1->time;
		ps1++;
	}

		
}
//--------------------------------------------��ʾ���-----------------------------------
  /*void ShowLyrics(void)
{
	LRC();
	int i;			
	TCHAR geci[256][256];
	int k=tmp2-head2;	
	for(i=0;i<k;i++)
	{
		wsprintf(geci[i],"%s\r\n",ps1->lrc);	
		ps1++;
	}
	//	TCHAR geci1[256*100];

	ZeroMemory(geci1,256*100);
	TCHAR C[256]="\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n";
	wsprintf(geci1,"%s",C);
	for(i=0;i<k;i++)
	{
		strcat(geci1,geci[i]);				
	}						
	//	SetDlgItemText(hwnd, IDC_EDIT1, geci1);
}   */
//------------------------���Ⱦɫ-------------------------------------------
   /*void colorLrc(HWND hwnd)
{
	/*HWND	hEdit0=GetDlgItem(hwnd,IDC_ShowLyrics);
	//hEdit0=(HWND)SetFont(&hfont1,true);
	HDC hdc1=GetDC(hEdit0);
	SetTextColor(hdc1,RGB(255,3,0));
	SelectObject(hdc1, hfont1);
//	SendDlgItemMessage(hEdit0, IDC_ShowLyrics, WM_SETFONT, (WPARAM)hdc1, TRUE);
	SendMessage(hEdit0, WM_CTLCOLOREDIT, (WPARAM)hdc1, MAKELPARAM(TRUE,0));
	SendMessage(hEdit0, WM_SETFONT, (WPARAM)hfont1, MAKELPARAM(TRUE,0));   */
/*	HWND hEdit01=GetDlgItem(hwnd,IDC_EDIT3);
	HDC hdc2=GetDC(hEdit01);
	SetTextColor(hdc2,RGB(255,3,0));
	TextOut(hdc2,0,0,ps1->lrc,strlen(ps1->lrc));  


//	SetTextColor((HDC)hEdit0,RGB(255,3,0));
}   */
//---------------------------------��ʾEDIT11�ĸ��-------------------------------------
void ShowEDIT11(HWND hwnd)
{	

	LRC();
	int j;			
	
	ZeroMemory(geciEDIT11,256*100);
	for(j=0;j<5;j++)
	{	
		strcat(geciEDIT11,geci[j]);					
	}
	SetDlgItemText(hwnd,IDC_EDIT11,geciEDIT11);
}
//---------------------------�����ɫ  EDIT11---------------------------------------------------
HBRUSH SetEditColor11(HWND hWndOwner11,HDC hDCEdit11,HWND hWndEdit11)
{
	HBRUSH hbr;
	
	hEdit11=GetDlgItem(hWndOwner11,IDC_EDIT11);
	hDCEdit11=GetDC(hEdit11);
    if(hWndEdit11==hEdit11)
    {
		SetTextColor(hDCEdit11,testColor);
		SetBkMode(hDCEdit11, TRANSPARENT); //���ֵı���͸��
		hbr =CreateSolidBrush(RGB(118,77,57));
		//		hbr=GetStockBrush(NULL_BRUSH);
		return hbr;
    }
    else
		return NULL;
}
//---------------------------�����ɫ  EDIT22---------------------------------------------------
HBRUSH SetEditColor22(HWND hWndOwner22,HDC hDCEdit22,HWND hWndEdit22)
{
	HBRUSH hbr;
	
	hEdit22=GetDlgItem(hWndOwner22,IDC_EDIT22);
	hDCEdit22=GetDC(hEdit22);
    if(hWndEdit22==hEdit22)
    {
		SetTextColor(hDCEdit22,testColor);
		SetBkMode(hDCEdit22, TRANSPARENT); //���ֵı���͸��
	//	hbr =CreateSolidBrush(RGB(118,77,57));
			hbr=GetStockBrush(NULL_BRUSH);
		return hbr;
    }
    else
		return NULL;
}
//-------------------------------------------------------------------------------------------
HBRUSH SetEditColor(HWND hWndOwner,HDC hDCEdit,HWND hWndEdit)
{
	HBRUSH hbr;
	
	hEdit=GetDlgItem(hWndOwner,IDC_EDIT22);
	hDCEdit=GetDC(hEdit);
    if(hWndEdit==hEdit)
    {
		SetTextColor(hDCEdit,testColor);
		SetBkMode(hDCEdit, TRANSPARENT); //���ֵı���͸��
	//	hbr=GetStockBrush(NULL_BRUSH);
		hbr =CreateSolidBrush(RGB(118,77,57));
		return hbr;
		//hbr=GetStockBrush(NULL_BRUSH);
		//SelectObject(hDCEdit,hbr);
	}
	else
	return NULL;
}
//----------------��ťͼ��----------------------------------------------------------------
void InitFont(HWND hwnd){
	HFONT hFont= CreateFont( 
		25, //�߶�
		37, //���
		0, //��ת�Ƕ�
		0, //����
		60, //�������
		0, //б��
		0, //�»���
		0, //�Ӵ�
		DEFAULT_CHARSET, //�ַ���
		OUT_DEFAULT_PRECIS, //����
		CLIP_DEFAULT_PRECIS, //���÷�ʽ
		DEFAULT_QUALITY, //��Ⱦ����
		DEFAULT_PITCH|FF_DONTCARE, //������, //������
		"Webdings" //������
		); 
	SendMessage(GetDlgItem(hwnd,IDC_Pause),WM_SETFONT,(WPARAM)hFont,0);
	SendMessage(GetDlgItem(hwnd,IDC_NextSong),WM_SETFONT,(WPARAM)hFont,0);
	SendMessage(GetDlgItem(hwnd,IDC_LastSong),WM_SETFONT,(WPARAM)hFont,0);
	SendMessage(GetDlgItem(hwnd,IDC_JudgeShow),WM_SETFONT,(WPARAM)hFont,0);
//	SendMessage(GetDlgItem(hwnd,IDC_Location),WM_SETFONT,(WPARAM)hFont,0);
	

}
//-------------------����----------------------------------------------------
void  Mute(HWND hwnd)
{				
			//	int voicecopy;
				TCHAR shortPath[MAX_PATH];
				TCHAR szVoice[256];
				GetShortPathName(szFileName_Path[numPlayMusicNow], shortPath, sizeof(shortPath));
				HWND hVoice = GetDlgItem(hwnd, IDC_Voice);  
				if(voice>0)
				{				
		
				voicecopy=voice;
				wsprintf(szVoice, "setaudio %s volume to %d", shortPath, 0); //���������õ������ֵ
				mciSendString(szVoice, (LPTSTR)NULL, 0, 0);	
				SendMessage(hwndVolumeSilder, TBM_SETPOS, (WPARAM)TRUE,0); 
				
				}
			/*	else if (voice==0)
				{	
				SetWindowText(hVoice, TEXT("U"));
				wsprintf(szVoice, "setaudio %s volume to %d", shortPath, voicecopy); //���������õ������ֵ
				mciSendString(szVoice, (LPTSTR)NULL, 0, 0);
				SendMessage(hwndVolumeSilder, TBM_SETPOS, (WPARAM)TRUE,voicecopy);
				}   */							
}
//---------------------------------�򻯴�������չ------------------------------------------------------------
void Extend(HWND hwnd)
{
	if(Extendmode==0)
	{	
	SetWindowPos(hwnd,0,122,75,330,585,SWP_NOMOVE|SWP_NOZORDER);
	SetWindowText(GetDlgItem(hwnd,IDC_Extend),">");
	Extendmode=1;
	}
	else
		if(Extendmode==1)
		{
		SetWindowText(GetDlgItem(hwnd,IDC_Extend),"<");
		SetWindowPos(hwnd,0,122,75,910,585,SWP_NOMOVE|SWP_NOZORDER);
		Extendmode=0;
		}

}
//----------------------����ģʽ----------------------------------------
void Mini(HWND hwnd)
{
	if(Minimode==0)
	{	
		SetWindowPos(hwnd,0,122,75,330,130,SWP_NOMOVE|SWP_NOZORDER);
		
		Minimode=1;
	}
	else
		if(Minimode==1)
		{
			
			SetWindowPos(hwnd,0,122,75,910,585,SWP_NOMOVE|SWP_NOZORDER);
			Minimode=0;
		}

}
//------------------------------�����б�ģ������-----------------------------
void SearchLocal(HWND hwnd)
{	
	int i;
//	SetWindowPos(hwndlist,0,122,75,330,500,SWP_NOMOVE|SWP_NOZORDER);
//	TCHAR Listallname[256*100][256];   //�б����и�����
	for(i=0;i<numOnList;i++)
	{
	 ListBox_GetText(hwndlist,i,Listallname[i]);
	}
	
	
	TCHAR szSearch[MAX_PATH],szTempSearch[MAX_PATH];
	memset(szSearch,0,sizeof(szSearch));
	memset(szTempSearch,0,sizeof(szTempSearch));               //һ��EDIT���  һ���Ǳ���Listbox   ����һά����
	HWND hSearchEdit=GetDlgItem(hwnd,IDC_SearchEdit);
	int	iTextLen=GetWindowTextLength(hSearchEdit);            //��ȡ�������ַ�������
	if(!iTextLen)
	{
		MessageBeep(0);
		MessageBox(hwnd,TEXT("������Ҫ���ҵĸ���"),TEXT("��ʾ"),MB_OK|MB_ICONEXCLAMATION);
		
	}
	GetDlgItemText(hwnd,IDC_SearchEdit,szSearch,sizeof(szSearch));
	_tcsupr(szSearch);

	for(i=0;i<numOnList;i++)
	{
		_tcscpy(szTempSearch,Listallname[i]);
		_tcsupr(szTempSearch);
		if(_tcsstr(szTempSearch,szSearch))
		{
			ListBox_SetCurSel(hwndlist,i);
			SendMessage(hwndlist,LB_SETCARETINDEX,i,FALSE);
			break;
		}
	}
	if(i==totalMusicCount)
	{
	
		MessageBeep(0);
		MessageBox(hwnd,TEXT("û���ҵ���Ҫ�ĸ���"),TEXT("��ʾ"),MB_OK|MB_ICONEXCLAMATION);		
	}
}
//-------------------------��ʵ�һ������XML--------------------------------------------------
void FirstXMLwast(HWND hwnd)
{
			
			wsprintf(firstxml,"http://mp3.baidu.com/dev/api/?tn=getinfo&ct=0&ie=utf-8&word=%s&format=xml",songname);
			URLDownloadToFile(NULL,firstxml,"firstxmlwast.xml",0,NULL);
			
		//	DeleteFile("firstxmlwast.xml");
}
void FirstXML(HWND hwnd)
{
	
	wsprintf(firstxml,"http://mp3.baidu.com/dev/api/?tn=getinfo&ct=0&ie=utf-8&word=%s&format=xml",songname);
	URLDownloadToFile(NULL,firstxml,"firstxml.xml",0,NULL);
	
	//	DeleteFile("firstxmlwast.xml");
}
void analyzeFirstXML(HWND hwnd)
{
	TCHAR *str2,str5[256];
	FILE *p;
	int i=0;
	p=fopen("firstxml.xml","r");

				while(!feof(p))
				{
					ZeroMemory(str5, sizeof(str5)/sizeof(TCHAR));
					fgets(str5,485,p);
					if((str2=strstr(str5,"<singer>"))!=NULL)
					{
						str2=str2+17;
						for(;*str2!=']';str2++)
						{
							singer[i]=*str2;
							i++;
							
						}
						singer[i]='\0';
					
						
					}				
				}  
			fclose(p);
			DeleteFile("firstxml.xml");
}
//---------------------------------------�ֶ�����--------------------------------------------
 void OnLine(HWND hwnd,LPCTSTR lpFmt)
 {
         TCHAR StrMusicOnline[MAX_PATH];
         GetDlgItemText(hwnd,IDC_EDIT1,EDITsong,sizeof(EDITsong)/sizeof(TCHAR));
         wsprintf(StrMusicOnline,lpFmt,EDITsong);
         ShellExecute(hwnd,"open",StrMusicOnline,NULL,NULL,SW_NORMAL);        
 }