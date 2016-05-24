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
#include <commctrl.h>  // TBM_SETPOS头文件
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "shellapi.h"	// 引入 NOTIFYICONDATA
#include "SkinH.h"  //皮肤包
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
#pragma comment(lib,"SkinH.lib")  //皮肤库


//---------------------------宏定义-------------------------------------------------------
#define ORDER 0  //顺序播放
#define SINGLELOOP 1  //单曲循环
#define LISTLOOP 2  //列表循环
#define RANDOM 3  //随机播放
#define WM_MYMESSAGE WM_USER+100
//--------------------------------全局变量定义--------------------------------------
TCHAR szFileName[80*MAX_PATH]; //要添加的文件全名，若同时选多个文件，则全包含在这里 每个文件全名用“\n”分割，虽然是一维数组但是显示的时候是可以有多行的。
static char szOpenFileNames[80*MAX_PATH];				// 初始化时用来获取文件名的变量
static OPENFILENAME ofn;                               
TCHAR szPath[MAX_PATH];									// 文件路径，不包含文件名
TCHAR shortPath[MAX_PATH];                               //短路径，用于MIC函数的操作
static char szFileName_Path[255*2][MAX_PATH];			// 记录歌曲的绝对路径，包含文件名
TCHAR songname[256]	;							//正在播放歌曲名（不含路径和后缀）
static int numOnOpenFile = 0;							// 记录打开文件时歌曲的序号
static int numOnList = 0;								// 记录列表上显示歌曲的序号
static int numPlayMusicNow = 0;							// 正在播放歌曲的序号
static int playStatus;                                  // 当前歌曲播放状态，1 表示正在播放状态，0 表示暂停状态  2表示停止
TCHAR musicTotalTime[256];								// 记录一首歌的总时间	
static int totalSecond;									// 记录一首歌的总秒数
static int totalMinute;									// 记录一首歌的总分钟数
TCHAR strTime[256] = "";								// 曲目播放当前时间
TCHAR curTime[256] = "";								//返回当前播放时间	
long curLength;	//	   curLength = atoi(curTime);		//这是当前时间的字符串转化为INT型

long allLength;											//歌曲总长度的长整型形式
TCHAR singer[256];                                      //通过MP3接口找到的歌手名
//HWND hwndMusicSilder;									// 播放歌曲的进度条句柄
HWND hwndVolumeSilder;								// 音量控制的进度条句柄
int voice;//音量
int voicecopy;//                                     辅助值
int mode = ORDER;										//当前状态设定为顺序播放
TCHAR musicTotalLength[256];							//歌曲总长度，毫秒为单位  字符形式
TCHAR DEFAULTLISTFILE[MAX_PATH];						// 保存默认路径
//HWND hwnd;	
static int totalMusicCount;							// 记录列表的歌曲的总数
HINSTANCE hApp;
static HMENU hmenu1;
TCHAR *Skin11="";
TCHAR *Setting11="";
int skinindex;
TCHAR Skinroad[256];		//皮肤路径
int Extendmode=0;          //拉伸样式
int Minimode=0;             //迷你样式
HWND hwndlist;
TCHAR Listallname[256*100][256];   //列表所有歌名字
HWND hSearchEdit,hSearchbutton;
	  
int MusicListflag=1;                    //不同列表对应不同值
 struct SET{
	int soundmodeparent;      //父窗口的声道模式值
	TCHAR shortPathtoChild[MAX_PATH];    //送到子窗口的短路径
	
}Sset;// “设置”子窗口里的各项值
 TCHAR firstxml[256];             //第一个XML分析文件
 TCHAR EDITsong[256],EDITsinger[256],STR1[256];

//-------------关于歌词的定义-----------(因为是后期加入，为了不混乱，所以分割开来)------------------

#define ID_TIMER 1000//计时器标识符
typedef struct song{
	long time;
	char lrc[100];
	
}Slrc;//存放歌词信息的结构体



Slrc *ps1,*ps2,*head2,*tmp,*tmp2; // ps1,ps2均指向动态分配的存储歌词信息的内存，ps2始终指向该内存的开始处。
char *p,*head1,*poslrc;
static bool signStart = 0, signStartLrc = 0;
TCHAR geciEDIT11[256*100],geciEDIT33[256*100];  //EDIT11，EDIT33的歌词，一维数组  \r\n  分割显示
TCHAR geci[256*100][256]; //二维数组把歌词保存起来,全部歌词！  
LONG lrctime[256*100] ;//全部歌词的时间
TCHAR lyrics[256];  //当前目录路径+歌词文件名+后缀
TCHAR lyricsX[256]; //  跟上面类似，只不过是上一级目录
int flag;// 歌词文件是否存在的判断标志
int temp;//歌词是否滚动的判断标志
int lrcindex; //歌词索引
int indexE33;//EDIT33专用
int k;//  一共有K行歌词
long curLengthEDIT11=0; // 时间比较，防止歌词闪烁
long curLengthEDIT22=0;
long curLengthEDIT33=0;
//------------------------------------桌面歌词----------------------------------------
#ifndef _WIN32_WINNT
#define LWA_COLORKEY 0x5000
#endif
#include "afxwin.h"
#include <objbase.h> 
#define RGB_TEXT_SING RGB(255,0,255)			// 唱过的字体颜色：粉红色
#define RGB_TEXT_NOT_SING RGB(100,255,0)		// 未唱过的字体颜色：黄绿色
#define RGB_MEMDC_BRUSH RGB(0,0,0)				// 内存DC画刷颜色：黑色

// 窗口的x,y坐标，宽度，高度
#define WND_ORG_X 10
#define WND_ORG_Y 650
#define WND_WIDTH 1000
#define WND_HEIGHT 100

// 字体的宽度，高度
#define FONT_WIDTH 53
#define FONT_HEIGHT 25
HDC hdc,m_hMemdc;
SIZE sz;					// UpdateLayeredWindow()函数用到的参数
POINT ptDC, ptMemDC;		// 屏幕DC坐标（非应用程序）,内存DC坐标
BLENDFUNCTION m_Blend;		// UpdateLayeredWindow()函数用到的参数结构体
RECT rectWndStart, rectLrc, rectSing;// 窗口矩形区域，内存DC(或用作全部歌词)矩形区域，唱过的歌词矩形区域
HBRUSH hbrushMemDC;			// 内存DC画刷句柄
typedef BOOL (WINAPI* functionpointer)(HWND,HDC,POINT*,SIZE*,HDC,POINT*,COLORREF,BLENDFUNCTION*,DWORD); 
functionpointer UpdateLayeredWindow;
HMODULE h = LoadLibrary("User32.dll");
//UpdateLayeredWindow = (functionpointer)GetProcAddress(h, "UpdateLayeredWindow");



//---------------------------------字体----------------------------------------------
HFONT hfont1 = CreateFont(25,15,0,0,800,FALSE,FALSE,0, GB2312_CHARSET /*ANSI_CHARSET*/, 
						 OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,NONANTIALIASED_QUALITY/*不去锯齿*/ /*ANTIALIASED_QUALITY 去锯齿,平滑字体*/,
		DEFAULT_PITCH | FF_SWISS, "楷体_GB2312");
HFONT hfontEDIT22 = CreateFont(28,13,0,0,900,FALSE,FALSE,0, RUSSIAN_CHARSET /*ANSI_CHARSET*/, 
						  OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,NONANTIALIASED_QUALITY/*不去锯齿*/ /*ANTIALIASED_QUALITY 去锯齿,平滑字体*/,
		DEFAULT_PITCH | FF_SWISS, "EDIT22");
HFONT hfontEDIT11 = CreateFont(24,10,0,0,550,FALSE,FALSE,0, RUSSIAN_CHARSET /*ANSI_CHARSET*/, 
								OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,NONANTIALIASED_QUALITY/*不去锯齿*/ /*ANTIALIASED_QUALITY 去锯齿,平滑字体*/,
		DEFAULT_PITCH | FF_MODERN, "EDIT11");
HFONT hfontcannotshow = CreateFont(20,10,0,0,700,FALSE,FALSE,0, GB2312_CHARSET /*ANSI_CHARSET*/, 
							   OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,NONANTIALIASED_QUALITY/*不去锯齿*/ /*ANTIALIASED_QUALITY 去锯齿,平滑字体*/,
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
//--------------------------------自定义函数声明--------------------------------------
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



//----------------------- 回调函数，读取当前播放时间,设置滑动条的当前值-----------------------------------
//  此函数需要现在初始化对话框中定义curTimePro！！！如：SetTimer(hwnd, 0, 1000, curTimePro); 

void CALLBACK curTimePro(HWND hwnd, UINT message, UINT iTemerID, DWORD dwTime)
{
	TCHAR str[256];
	TCHAR shortPath[MAX_PATH];
	GetShortPathName(szFileName_Path[numPlayMusicNow], shortPath, sizeof(shortPath));

	wsprintf(str, "status %s position", shortPath);
	//int wsprintf( 　　LPTSTR lpOut, 输出缓冲区，最大为1024字节 　　LPCTSTR lpFmt,  格式字符串 　　... 需输出的参数;）
	mciSendString(str, curTime, sizeof(curTime)/sizeof(TCHAR), NULL);//mciSendString(“play ***”,"",0,NULL);
	curLength = atoi(curTime);
	totalMinute = curLength / 60000;//定义时间中  “分”的由来
	totalSecond = (curLength / 1000) % 60;//定义时间中  “秒”的由来
	wsprintf(strTime, "%.2d:%.2d",totalMinute, totalSecond);// 曲目播放当前时间 的确定
	SetDlgItemText(hwnd,IDC_MusicNowTime,strTime);//将当前播放时间打印到IDC_TimeStart中

	allLength= atoi(musicTotalLength);	
	// 设置滑块的最大值，最小值以及当时位置
	HWND hwSilder= GetDlgItem(hwnd,IDC_ProgressBar);//IDC_ProgressBar的句柄定义为hwSlider
	SendMessage(hwSilder, TBM_SETRANGEMAX, (WPARAM)TRUE, allLength);//TBM_SETRANGEMAX：设置滑块逻辑位置的最大值，及是否重画
	SendMessage(hwSilder, TBM_SETRANGEMIN, (WPARAM)TRUE, 0);//TBM_SETRANGEMIN：设置滑块逻辑位置的最小值，及是否重画
	SendMessage(hwSilder, TBM_SETPOS, (WPARAM)TRUE, curLength);//TBM_SETPOS：设置滑块的逻辑位置，及是否重画    



	TCHAR szCommand[MAX_PATH+10];
	TCHAR szReturn[MAX_PATH];
	ZeroMemory(szCommand, sizeof(szCommand)/sizeof(TCHAR));
	//C语言中声明一个结构体变量之后，在使用这个变量之前要首先用memset来把各个位清零。使用C语言内置的函数memset来完成：
	//memset(&s1,sizeof(s1),0);
	//在这里我们使用ZeroMemory“函数”，ZeroMemory其实并不是函数，其实是memset的宏定义
	//ZeroMemory其实就是memset，纸老虎而已，不是一个全新的函数。
	wsprintf(szCommand, TEXT("status %s mode"), shortPath);
	mciSendString(szCommand, szReturn, sizeof(szReturn)/sizeof(TCHAR), NULL);
	//lstrcmp字符串对比，本语句含义是：当szReturn==stopped时自动播放下一曲   */
	if (0 == lstrcmp(szReturn, TEXT("stopped")))
	{											
		autoNext(hwnd);
	}     //如果运行到末尾， 就自动播放下一曲
} 

//-----------------------------时间回调，桌面歌词----------------------------------------
void CALLBACK Deskshow(HWND hwnd, UINT message, UINT iTemerID, DWORD dwTime)
{
	rectWndStart.left = WND_ORG_X, rectWndStart.top = WND_ORG_Y;
	rectWndStart.right = rectWndStart.left+WND_WIDTH, rectWndStart.bottom = rectWndStart.top+WND_HEIGHT;
	
	hdc = GetDC(NULL);
	m_hMemdc = CreateCompatibleDC(NULL);	//创建内存DC
	// 注意，此程序创建字体时，字体不应去锯齿，否则调用 UpdateLayeredWindow 函数时会出现字体边上有杂色。
	// 当然，若去锯齿，可以把字体的背景色设置为与字体颜色相近，调用 UpdateLayeredWindow 函数时过滤掉背景色即可；
	// 但卡拉OK歌词利用了两种颜色的字体，故不好办，因此字体不应去锯齿。
	HFONT hfont = CreateFont(FONT_WIDTH,FONT_HEIGHT,0,0,FW_BOLD,FALSE,FALSE,0, GB2312_CHARSET /*ANSI_CHARSET*/, 
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,NONANTIALIASED_QUALITY/*不去锯齿*/ /*ANTIALIASED_QUALITY 去锯齿,平滑字体*/,
		DEFAULT_PITCH | FF_SWISS, "楷体_GB2312");
	SelectObject(m_hMemdc, hfont);
	HBITMAP hbitmap = CreateCompatibleBitmap(hdc, WND_ORG_X+WND_WIDTH, WND_HEIGHT);
	SelectObject(m_hMemdc, hbitmap);		// 选入内存DC
	//	SetBkColor(m_hMemdc,RGB_MEMDC_BRUSH);	// 设置背景颜色
	SetBkMode(m_hMemdc, TRANSPARENT);		// 设置背景色为透明
	hbrushMemDC = CreateSolidBrush(RGB_MEMDC_BRUSH);//黑色画刷，以便刷新透明背景状态下的内存DC
	SelectObject(m_hMemdc, hbrushMemDC);
	//初始化BLENDFUNCTION结构体，UpdateLayeredWindow()函数用到的参数结构体
	m_Blend.AlphaFormat = 1;
	m_Blend.BlendFlags = 0;
	m_Blend.BlendOp = 0;
	m_Blend.SourceConstantAlpha = 255;
	int lenLrc = 0, lenSing = 0;	// 当前歌词在内存DC中的长度，唱过的歌词的长度
	SIZE tmpSz;
	if(curLength >= ps2->time && curLength <= (ps2+1)->time && ps1 != ps2+1)// 若歌曲从头开始，歌词也要从头开始
	{
		ps1 = ps2;
	}
	if(curLength >= ps1->time)
	{
		rectLrc.left = rectWndStart.left, rectLrc.right = rectWndStart.right;   
		rectLrc.top = 0, rectLrc.bottom = rectLrc.top+FONT_WIDTH;
		FillRect(m_hMemdc, &rectLrc, hbrushMemDC);                        //填充矩形
		
		// 以下获取当前歌词的总长度（大概值）。也可以使用 GetTextExtentPoint32 获取。
					lenLrc = strlen(ps1->lrc) * FONT_HEIGHT;
		GetTextExtentPoint32(m_hMemdc,ps1->lrc,strlen(ps1->lrc),&tmpSz);
		lenLrc = tmpSz.cx;
		rectLrc.left = (rectWndStart.right-rectWndStart.left) / 2 - lenLrc/2;
		rectLrc.right = (rectWndStart.right-rectWndStart.left) / 2 + lenLrc/2;            //歌词矩形的范围
		
		SetTextColor(m_hMemdc,RGB_TEXT_NOT_SING);	// 设置未唱过的歌词颜色
		TextOut(m_hMemdc,rectLrc.left,0,ps1->lrc,strlen(ps1->lrc));              //打印出没唱过的歌词
		ps1++;
		lenSing = (curLength-(ps1-1)->time) * 1.0 / (ps1->time-(ps1-1)->time) * (rectLrc.right-rectLrc.left);    //唱过的长度
		rectSing.left = rectLrc.left, rectSing.right = lenSing+rectSing.left;
		rectSing.top = 0, rectSing.bottom = sz.cy;
		SetTextColor(m_hMemdc,RGB_TEXT_SING);	// 设置唱过的歌词颜色
		DrawText(m_hMemdc,(ps1-1)->lrc,strlen((ps1-1)->lrc),&rectSing,DT_LEFT);
		
		ptDC.x = rectWndStart.left, ptDC.y = WND_ORG_Y, ptMemDC.x = rectWndStart.left, ptMemDC.y = 0;
				sz.cx = rectWndStart.right-rectWndStart.left, sz.cy = FONT_WIDTH;
		UpdateLayeredWindow(hwnd,hdc,&ptDC,&sz, m_hMemdc,&ptMemDC,RGB_MEMDC_BRUSH,&m_Blend,0x1 );
		int signStart =1;


	}
//	UpdateLayeredWindow(hwnd,hdc,&ptDC,&sz, m_hMemdc,&ptMemDC,RGB_MEMDC_BRUSH,&m_Blend,1);
}


//----------------------时间回调-EDIT11的刷新--------------------------
void CALLBACK LrcEDIT11(HWND hwnd, UINT message, UINT iTemerID, DWORD dwTime)
{	
	tmp2=head2;

	HWND hEDIT11=GetDlgItem(hwnd,IDC_EDIT11);
//	SetWindowText(hwnd,""); //清空旧歌词
	if(curLength >= ps2->time && curLength <= (ps2+1)->time && ps1 != ps2+1)// 若歌曲从头开始，歌词也要从头开始
	{
		ps1 = ps2;
		lrcindex=0;
		//indexE33=-1;
	}

	if(curLength >= lrctime[lrcindex])
	{
	int j;	
	ZeroMemory(geciEDIT11,256*100);
	
	


	SetWindowText(hEDIT11,""); //清空旧歌词

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
//--------------------------时间回调-EDIT22的刷新----------------------------------
void CALLBACK LrcEDIT22(HWND hwnd, UINT message, UINT iTemerID, DWORD dwTime)
{
	tmp2=head2;
	
	HWND hEDIT22=GetDlgItem(hwnd,IDC_EDIT22);

	
	

//	SetWindowText(hwnd,""); //清空旧歌词
	if(curLength >= ps2->time && curLength <= (ps2+1)->time && ps1 != ps2+1)// 若歌曲从头开始，歌词也要从头开始
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
	SetWindowText(hEDIT22,""); //清空旧歌词
	SetDlgItemText(hwnd,IDC_EDIT22,geciEDIT22);	
	
	
	
	}


}
//---------------------------时间回调-EDIT33刷新--------------------------------\------
void CALLBACK LrcEDIT33(HWND hwnd, UINT message, UINT iTemerID, DWORD dwTime)
{
	tmp2=head2;
	HWND hEDIT33=GetDlgItem(hwnd,IDC_EDIT33);
	int h;
	SetWindowText(hEDIT33,""); //清空旧歌词

	ZeroMemory(geciEDIT33,256*100);
	if(lrcindex<6&&lrcindex>1)
	{
	
//		SetWindowText(hEDIT33,""); //清空旧歌词
		
		
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
	//	SetWindowText(hEDIT33,""); //清空旧歌词
		for(h=lrcindex-6;h<lrcindex-1;h++)
		{
			strcat(geciEDIT33,geci[h]);
		}
		SendMessage(hEDIT33, WM_SETFONT, (WPARAM)hfontEDIT11, MAKELPARAM(TRUE,0));
		SetDlgItemText(hwnd,IDC_EDIT33,geciEDIT33);
	}

}
//----------------------------------子线程 ，歌词--------------------------------------------
DWORD WINAPI ThreadLrcshow(LPVOID lpParam)
{
	HWND hwnd=(HWND)lpParam;
	
	SetTimer(hwnd,2,100,LrcEDIT11);
	SetTimer(hwnd,3,100,LrcEDIT22);
	SetTimer(hwnd,4,100,LrcEDIT33);  
	
	
	
	
	return 0;
}
 //-------------------------Main_Proc  对话框的消息处理函数-------------------- ------
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
		HANDLE_MSG(hWnd,WM_HSCROLL, Main_OnHScroll);  // Slider滑块消息
		

		case WM_MYMESSAGE:
		{
		//	if(lParam == WM_LBUTTONDOWN)     //在托盘图标上左击  
			if(lParam==WM_LBUTTONDBLCLK)
			{
				ShowWindow(hWnd,SW_SHOW);
			}
			else if(lParam == WM_RBUTTONDOWN)				//在托盘图标上右击
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
		case WM_CONTEXTMENU:                //列表右键弹出菜单
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
	
		case WM_PAINT:						//重绘窗口 消息【注】因有UpdateWindow存在，故启动应用程序时，此消息执行两次
			PAINTSTRUCT ps;					//定义存储绘制窗口用的信息
			BeginPaint(hWnd,&ps);			//绘制窗口，会自动填充PAINTSTRUCT结构体
			
			// 以下两种方式都可以重新绘制歌词，注意ps1是指向下一句歌词的，而现在是绘制当前歌词
			// 方式1：发送消息，注意 ps1--
			signStart==1 ? ps1--,SendMessage(hWnd,WM_TIMER,0,0) : 0;
			// 方式2：直接绘制，注意 ps1-1
			//			hdc = GetDC(hwnd);
			//			signStart==1 ? TextOut(hdc,0,0,(ps1-1)->lrc,strlen((ps1-1)->lrc)) : 0;
			//			ReleaseDC(hwnd,hdc);
			
			EndPaint(hWnd,&ps);				//结束绘制
			break;
/*		case	WM_CTLCOLORSTATIC:
				{	
			/*	hWndEdit11=(HWND)lParam;
				hDCEdit11=(HDC)wParam;
				SetTextColor(hDCEdit11,RGB(174,221,129));
				SetBkMode(hDCEdit11, TRANSPARENT); //文字的背景透明		
				return (LRESULT)SetEditColor11(hWnd,hDCEdit11,hWndEdit11); 
				hWndEdit22=(HWND)lParam;
				hDCEdit22=(HDC)wParam;
				SetTextColor(hDCEdit22,RGB(174,221,129));
				SetBkMode(hDCEdit22, TRANSPARENT); //文字的背景透明 
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



//----------------------------初始化对话框------------------------------------------------------------------
BOOL Main_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	
//	SetTimer(hwnd,1,1000,curTimePro);
	InitFont(hwnd);
	TCHAR cTemp[MAX_PATH],cFullPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, cTemp);
	//使用GetShortPathName时，必须确保原路径及文件存在，否则返回空字串
	//因此此处先转换路径，再连接上列表文件名，存入到全局变量DEFAULTLISTFILE中 
	GetShortPathName(cTemp,cFullPath,MAX_PATH);
	wsprintf(DEFAULTLISTFILE,"%s\\.lst",cFullPath); 

	ReadList(hwnd,DEFAULTLISTFILE);	 //读取列表信息
	// 初始化时间，播放时间和曲目总时间都为零
	strcpy(strTime, "00:00");
	strcpy(musicTotalTime, "00:00");
	SetDlgItemText(hwnd, IDC_MusicNowTime, strTime);
	SetDlgItemText(hwnd,IDC_MusicTotalTime,musicTotalTime);
	// 初始化音量
	hwndVolumeSilder= GetDlgItem(hwnd, IDC_Volume);
	SendMessage(hwndVolumeSilder, TBM_SETRANGEMAX, (WPARAM)TRUE, 1000);  // 音量最大为400
	SendMessage(hwndVolumeSilder, TBM_SETRANGEMIN, (WPARAM)TRUE, 0);  // 最小值为0
//	SendMessage(hwndVolumeSilder, TBM_SETPAGESIZE, 0, (LPARAM)100);	  //设置点击跳动进度值
//	SendMessage(hwndVolumeSilder,TBM_SETPAGESIZE, 0, (LPARAM)100);	
	SendMessage(hwndVolumeSilder, TBM_SETPOS, (WPARAM)TRUE,voice);   // 初始化为音量
	//初始化播放模式
	HWND hwCombo1 = GetDlgItem(hwnd, IDC_PlayMode);
	ComboBox_InsertString(hwCombo1, -1, TEXT("顺序播放"));//index为-1时指：将顺序播放插入到列表末尾 
	ComboBox_InsertString(hwCombo1, -1, TEXT("单曲循环"));//index为-1时指：将单曲循环插入到列表末尾
	ComboBox_InsertString(hwCombo1, -1, TEXT("列表循环"));//index为-1时指：将列表循环插入到列表末尾
	ComboBox_InsertString(hwCombo1, -1, TEXT("随机播放"));//index为-1时指：将随机播放插入到列表末尾
	ComboBox_SetCurSel(hwCombo1, 0);  //默认选择顺序播放模式   
	HWND hwButtonPause = GetDlgItem(hwnd, IDC_Pause);  // 得到IDC_PAUSE按钮的句柄
	SetWindowText(hwButtonPause, TEXT("4"));  // 将按钮上的文字改为“播放”
	
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
			AddFile(hwnd);//添加文件
			}
			break;
		case IDC_AddFloder://添加文件夹
			{
			AddFloder(hwnd);	
			}
			break;
		case IDC_Add:// 点击添加，弹出菜单选项
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
		case IDC_DeleteAll: //清空播放列表
			{
			DeleteAll(hwnd);
			}
			break;
		case IDC_LastSong://上一曲
			{
			LastSong(hwnd);
			}
		break;
		case IDC_NextSong://下一曲
			{
			NextSong(hwnd);
			Play(hwnd);
			}
			break;
		case IDC_Skin:   //呼出选择皮肤的子窗口
			{
		
			HINSTANCE hInstance=(HINSTANCE)GetWindowLong(hwnd,GWL_HINSTANCE);
		//	DialogBox(hInstance,MAKEINTRESOURCE(IDD_Skin),NULL,Skin_Proc);
			skinindex=	DialogBoxParam(hInstance,MAKEINTRESOURCE(IDD_Skin),NULL,Skin_Proc,(LPARAM)Skin11);
			
				if(0==skinindex)
				{
					wsprintf(Skinroad,"skin\\木质风格.she");
				}
				if(1==skinindex)
				{
					wsprintf(Skinroad,"skin\\草绿边框.she");
				}
				if(2==skinindex)
				{
					wsprintf(Skinroad,"skin\\纯黑A.she");
				}
				if(3==skinindex)
				{
					wsprintf(Skinroad,"skin\\纯黑B.she");
				}
				if(4==skinindex)
				{
					wsprintf(Skinroad,"skin\\粉红A.she");
				}
				if(5==skinindex)
				{
					wsprintf(Skinroad,"skin\\粉红B.she");
				}
				if(6==skinindex)
				{
					wsprintf(Skinroad,"skin\\淡咖啡.she");
				}
				if(7==skinindex)
				{
					wsprintf(Skinroad,"skin\\淡雅橘色.she");
				}
				if(8==skinindex)
				{
					wsprintf(Skinroad,"skin\\蓝紫.she");
				}
				if(9==skinindex)
				{
					wsprintf(Skinroad,"skin\\浅蓝.she");
				}
				if(10==skinindex)
				{
					wsprintf(Skinroad,"skin\\紫黑.she");
				}
			}
			break;
		case IDC_MusicList:	//双击 播放						
			{
			if(codeNotify==LBN_DBLCLK)
				{
				Play(hwnd);
				}
			}
			break; 
		case IDC_Location://定位当前播放
			{
			Location(hwnd);
			}
			break;
		case IDC_Pause://暂停
			{
			Pause(hwnd);
			if(0==playStatus)
				{
				HWND hwButtonPause = GetDlgItem(hwnd, IDC_Pause);  // 得到IDC_PAUSE按钮的句柄
				SetWindowText(hwButtonPause, TEXT("4"));  // 将按钮上的文字改为“播放”
				}
			}
			break;
		case IDC_Voice:  //  静音
			{
				Mute(hwnd);
			}
			break;
		case IDC_GetSongMessage://获取歌曲信息
			{
			GetSongMessage(hwnd);
			}
			break;
	
			//----------以下是关于托盘弹出式菜单的按钮
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
			//------以下是列表右键弹出菜单	
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
//--------------------------检测系统最小化消息--------------------------------------------------（不太清楚做什么用，先留着）
void Main_OnSysCommand(HWND hwnd, UINT cmd, int x, int y)
{
	if(SC_MINIMIZE == cmd)
	{
	 ShowWindow(hwnd,SW_HIDE);
	 return;
	}
}


//----------------------------------关闭窗口---------------------------------------------------
void Main_OnClose(HWND hwnd)
{	
	int ret = MessageBox(hwnd, TEXT("最小化托盘吗"), TEXT("提示"), MB_YESNO|MB_ICONQUESTION);
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




//------------------------------函数定义--------------------------------------------------------------
//-----------------------------添加音乐--------------------------------------------------------
void AddFile(HWND hwnd)
{
									// 在列表上显示出来的名字
	int flagJudgeRepeat = 0;										// 判断要添加的歌曲是否已经存在列表中的标志位
	ZeroMemory(&ofn,sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = szOpenFileNames;
	ofn.lpstrFile[0] = TEXT('\0');
	ofn.nMaxFile = sizeof(szOpenFileNames);
	ofn.lpstrFilter = TEXT("所有支持文件(*.wav,*.mp3,*.wma)\0*.wav;*.mp3;*.wma\0wave文件(*.wav)\0*.wav\0mp3文件(*.mp3)\0*.mp3\0wma文件(*.wma)\0*.wma\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.hwndOwner = hwnd;
	ofn.Flags = OFN_EXPLORER |OFN_PATHMUSTEXIST | OFN_ALLOWMULTISELECT;
	if (GetOpenFileName(&ofn)) //GetOpenFileName函数返回，返回值是BOOL，等于TRUE表示用户选择了文件，如果等于FALSE就表示用户选择了【取消】按钮
	{
	

		
			lstrcpyn(szPath, szOpenFileNames, ofn.nFileOffset);

			szPath[ofn.nFileOffset] = '\0';
			int nLen = lstrlen(szPath);

			if( szPath[nLen-1] != '\\' )								//如果选了多个文件,则必须加上'\\'
			{
				lstrcat(szPath, TEXT("\\"));
			}

			char *p = szOpenFileNames + ofn.nFileOffset;				//把指针移到第一个文件
			
			while(*p)
			{   
				lstrcpy(szFileName_Path[numOnOpenFile],szPath);			//将路径复制给szFileName_Path
				lstrcat(szFileName_Path[numOnOpenFile],p);				//加上文件名   
				numOnList++;											// 列表上显示的序号加一
				
				for (int i=0; i<numOnOpenFile; i++)
				{
					// 判断是否有重复的歌曲
					if (0 == strcmp(szFileName_Path[numOnOpenFile],szFileName_Path[i]))
					{
						numOnOpenFile--;	
						numOnList--;		 
						flagJudgeRepeat = 1;							// 重复标志位置1		
					} 
				}
				if (0 == flagJudgeRepeat)								// 没有重复   
				{	
					HWND hwndlist = GetDlgItem(hwnd, IDC_MusicList);//得到列表框句柄
					TCHAR drive[_MAX_DRIVE];
					TCHAR dir[_MAX_DIR];
					TCHAR fname[_MAX_FNAME];
					TCHAR ext[_MAX_EXT];
					_splitpath(szFileName_Path[numOnOpenFile], drive, dir, fname, ext );  //将文件全路径分解
					
					//wsprintf(songname,"%s",fname);
				
					int ListCount=ListBox_GetCount(hwndlist);//得到列表框条目数
				
				
					ListBox_InsertString(hwndlist,ListCount,fname);  //向列表插入文件名	
				
					p += lstrlen(p) +1;									//移至下一个文件
					numOnOpenFile++; 
				}
				else
				{
					flagJudgeRepeat = 0;
					p += lstrlen(p) +1;									//移至下一个文件
					numOnOpenFile++;  
				}
			}
	}	
}


//-------------------------添加文件夹--------------------------------------------------
void AddFloder(HWND hwnd)
{
		TCHAR pBuffer_Directory[MAX_PATH];								//存放文件信息
		BROWSEINFO sBrowseInfo;
		LPCITEMIDLIST lpitem;
		WIN32_FIND_DATA sfd;

		ZeroMemory(&sBrowseInfo,sizeof(BROWSEINFO));
		sBrowseInfo.hwndOwner=hwnd;
		sBrowseInfo.lpszTitle="请选择文件夹";
		sBrowseInfo.ulFlags=BIF_RETURNONLYFSDIRS;

		lpitem=SHBrowseForFolder(&sBrowseInfo);
		if (NULL==lpitem)												//判断是否已经选择文件夹
		return;

		SHGetPathFromIDList(lpitem,pBuffer_Directory);					//获得文件夹的路径
			
		TCHAR szFile[MAX_PATH];
		strcpy(szFile,pBuffer_Directory);
//		TCHAR *postfixName[3]={"\\*.mp3","\\*.wav","\\*.wma"};   //3个数组表示3种后缀名(先放着，看看以后能不能实现wav 和wma的枚举过滤)
//		TCHAR *pBuffer_Directory1[3];//保存下一步操作前的pBuffer_Directory
/*		int i; //用于循环结构
		for(i=0;i<2;i++)
		{   */
	

		TCHAR szFile_ToShow[256];  //显示到列表上的字符串

		lstrcat(pBuffer_Directory,"\\*.mp3");							//枚举过滤条件(只实现MP3)
		HANDLE hfindfile=FindFirstFile(pBuffer_Directory,&sfd);
		if (INVALID_HANDLE_VALUE==hfindfile)							//判断文件是否有效
		return;

		
		do
		
			{
							
			if (sfd.cFileName[0]=='.')
			continue;
			lstrcat(szFileName_Path[numOnOpenFile],szFile);
			lstrcat(szFileName_Path[numOnOpenFile],"\\");
			lstrcat(szFileName_Path[numOnOpenFile],sfd.cFileName);     //sfd.cFileName是 文件名（带有后缀）
			lstrcat(szFileName,szFile);
			lstrcat(szFileName,"\\");
			lstrcat(szFileName,sfd.cFileName); 
		
			numOnList++;
			HWND hwndlist = GetDlgItem(hwnd, IDC_MusicList);//得到列表框句柄
			TCHAR drive[_MAX_DRIVE];
			TCHAR dir[_MAX_DIR];
			TCHAR fname[_MAX_FNAME];
			TCHAR ext[_MAX_EXT];
        	_splitpath( szFileName, drive, dir, fname, ext );  //将文件全路径分解 */
			
		
			int ListCount=ListBox_GetCount(hwndlist);//得到列表框条目数 
			wsprintf(szFile_ToShow,"%d. %s",numOnList,fname);	//在文件前加编号
			
				ListBox_InsertString(hwndlist,ListCount,fname);  //向列表插入文件名			
			
			
			numOnOpenFile++;	
			}
	
		while (FindNextFile(hfindfile,&sfd));

		
}
//----------------------------------播放音乐-----------------------------------------------------
void Play(HWND hwnd)
{
	Stop(hwnd);// 先停止当前播放
	HWND hwButtonPause = GetDlgItem(hwnd, IDC_Pause);  // 得到IDC_PAUSE按钮的句柄
	SetTimer(hwnd,1,1000,curTimePro);

	HWND hEDIT11=GetDlgItem(hwnd,IDC_EDIT11);
	HWND hEDIT22=GetDlgItem(hwnd,IDC_EDIT22);
	HWND hEDIT33=GetDlgItem(hwnd,IDC_EDIT33);
	HWND hcannotshow=GetDlgItem(hwnd,IDC_Cannotshow);
//	SetWindowText(hEDIT33,""); //播放之前先清空上一首 歌词
//	SetWindowText(hwnd,""); //播放之前先清空上一首 歌词
	SetWindowText(hEDIT33,""); //播放之前先清空上一首 歌词
	SetWindowText(hEDIT22,""); //播放之前先清空上一首 歌词
	SetWindowText(hEDIT11,""); //播放之前先清空上一首 歌词
	
//	TCHAR str1[256] = "";
//	TCHAR str2[256] = "";
//	TCHAR shortPath[MAX_PATH];
	HWND hwndlist = GetDlgItem(hwnd, IDC_MusicList);  // 得到ListBox句柄

	
		
		TCHAR str1[256] = "";
		TCHAR str2[256] = "";
		int index = ListBox_GetCurSel(hwndlist);
		ListBox_GetText(hwndlist, index, str1);  // 得到歌曲名
		GetShortPathName(szFileName_Path[index], shortPath, sizeof(shortPath));  // 获得短路径
		GetShortPathName(szFileName_Path[index], (Sset.shortPathtoChild), sizeof(Sset.shortPathtoChild));  // 获得短路径送到“设置”窗口
		wsprintf(str2, "play %s", shortPath);
		numPlayMusicNow=index;
		mciSendString(str2, "", 0, NULL);  // 播放选中歌曲
		playStatus=1;  //设置为正在播放状态
		SetWindowText(hwButtonPause, TEXT(";"));  // 将按钮上的文字设置为“暂停”
		MusicTotalTime(hwnd);
		SendMessage (GetDlgItem(hwnd,IDC_MusicList), LB_GETTEXT, numPlayMusicNow,(LPARAM)str1);
		SetDlgItemText(hwnd, IDC_PlayEdit, str1);					// 显示正在播放歌曲的名字
		voice = SendMessage(GetDlgItem(hwnd,IDC_Volume), TBM_GETPOS, 0, 0); //得到当前滑块的音量值
		//TBM_GETPOS：返回当前滑块位置所代表的值（逻辑位置）
		TCHAR szVoice[256];
		wsprintf(szVoice, "setaudio %s volume to %d", shortPath, voice); //将音量设置到滑块的值
		mciSendString(szVoice, (LPTSTR)NULL, 0, 0);	
		wsprintf(songname,"%s",str1);  //得到歌曲名，放到全局变量方便调用

	BOOL FLAG;
	FLAG=JudgeLrc();
	if(FLAG==TRUE)   //判断是否有歌词文件，有才显示
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
//	SetWindowText(hEDIT33,""); //播放之前先清空上一首 歌词
//	SetWindowText(hEDIT22,""); //播放之前先清空上一首 歌词
//	SetWindowText(hEDIT11,""); //播放之前先清空上一首 歌词
	ShowWindow(hEDIT11,SW_HIDE);
	ShowWindow(hEDIT22,SW_HIDE);
	ShowWindow(hEDIT33,SW_HIDE);
	ShowWindow(hcannotshow,SW_SHOW);
	SetDlgItemText(hwnd, IDC_Cannotshow, "无法找到歌词，请手动搜索");


//	analyzeFirstXML(hwnd);
	SendMessage(hcannotshow, WM_SETFONT, (WPARAM)hfontcannotshow, MAKELPARAM(TRUE,0));
//	Sleep(1);
	}



}
//--------------------------------暂停------------------------------------------------------------
void Pause(HWND hwnd)
{
	TCHAR str1[256] = "";
	TCHAR str2[256] = "";
	TCHAR shortPath[MAX_PATH];
	HWND hwndlist = GetDlgItem(hwnd, IDC_MusicList);  // 得到ListBox句柄
	HWND hwButtonPause = GetDlgItem(hwnd, IDC_Pause);  // 得到IDC_PAUSE按钮的句柄
	int index = ListBox_GetCurSel(hwndlist);//得到当前选项的索引
	ListBox_GetText(hwndlist, index, str1);  // 得到歌曲名
	wsprintf(songname,"%s",str1);
	
	GetShortPathName(szFileName_Path[numPlayMusicNow], shortPath, sizeof(shortPath));  // 获得短路径
	if (1 == playStatus)  // 如果处于播放状态
	{
		//	SetWindowText(hwButtonPause, TEXT("继续"));  // 将按钮上的文字改为“继续”
		
		wsprintf(str2, "pause %s", shortPath);  // pause命令
		mciSendString(str2, "", 0, NULL);  // 暂停选中歌曲  */
		playStatus=0;				
	}
	
	else if(0==playStatus)
	{
		SetWindowText(hwButtonPause, TEXT(";"));  // 将按钮上的文字改为“暂停”
		wsprintf(str2, "resume %s", shortPath);  // resume命令为继续播放
		mciSendString(str2, "", 0, NULL); 
		playStatus=1;
		
	}  
	
	
}
//--------------------------------停止播放------------------------------------------------------------
void Stop(HWND hwnd)
{
		TCHAR str[256];
		TCHAR shortPath[MAX_PATH];
		GetShortPathName(szFileName_Path[numPlayMusicNow], shortPath, sizeof(shortPath));
		wsprintf(str, "close %s", shortPath);
		mciSendString(str, "", 0, NULL);
		playStatus=2;


} 
//--------------------------------播放上一曲------------------------------------------------------------
void LastSong(HWND hwnd)
{
		Stop(hwnd);
		TCHAR str1[256] = "";
		TCHAR str2[256] = "";

		HWND hwndlist = GetDlgItem(hwnd, IDC_MusicList);  // 得到ListBox句柄
		int index = ListBox_GetCurSel(hwndlist);
		int all = ListBox_GetCount(hwndlist);  // 列表中总项
		if (index > 0)
		{
			ListBox_GetText(hwndlist, index-1, str1);  // 得到歌曲名
			ListBox_SetCurSel(hwndlist, index-1);
		}
		else//当为第一首歌，即index==0时，跳到最后一首
		{
			ListBox_GetText(hwndlist, all-1, str1);  // 得到歌曲名
			ListBox_SetCurSel(hwndlist, all-1);
		}	

}
//--------------------------------播放下一曲------------------------------------------------------------
void NextSong(HWND hwnd)
{
		Stop(hwnd);
		TCHAR str1[256] = "";
		TCHAR str2[256] = "";

		HWND hwndlist = GetDlgItem(hwnd, IDC_MusicList);  // 得到ListBox句柄
		int index = ListBox_GetCurSel(hwndlist);
		int all = ListBox_GetCount(hwndlist);  // 列表中总项
		if (index ==all-1)   //如果所选是列表最后一项
		{
			ListBox_GetText(hwndlist, 0, str1);  // 得到歌曲名
			ListBox_SetCurSel(hwndlist, 0);
		}
		else
		{
			ListBox_GetText(hwndlist, index+1, str1);  // 得到歌曲名
			ListBox_SetCurSel(hwndlist, index+1);
		}	

}
//--------------------------------记录一首歌总时间------------------------------------------------------------
void MusicTotalTime(HWND hwnd)
{	
//	TCHAR musicTotalLength[256];
	int intSecond,intMinute;
	TCHAR str[256];
	TCHAR shortPath[MAX_PATH];
	GetShortPathName(szFileName_Path[numPlayMusicNow], shortPath, sizeof(shortPath));
	wsprintf(str, "status %s length", shortPath);

	mciSendString(str,musicTotalLength,sizeof(musicTotalLength)/sizeof(TCHAR),NULL);//获得歌曲的长度
	totalSecond = atoi(musicTotalLength)/1000;						//总时间（毫秒）除以1000得到（整数）总秒数 

	intMinute = totalSecond / 60;									//总秒数除以6得到（整数）分钟数 
	intSecond = (totalSecond - intMinute*60);						//总秒数减去（整数）分钟*60得到剩余秒数 

	wsprintf(musicTotalTime,"%02d:%02d",intMinute,intSecond);		//02s表示时间分钟或秒数共两位数不足补0； 
	SetDlgItemText(hwnd,IDC_MusicTotalTime,musicTotalTime);
}  


//------------------------------滑块函数，进度与音量--------------------------------
void Main_OnHScroll(HWND hwnd, HWND hwndCtl, UINT code, int pos)
{    
	SetTimer(hwnd,1,1000,curTimePro);
	TCHAR shortPath[MAX_PATH];
	GetShortPathName(szFileName_Path[numPlayMusicNow], shortPath, sizeof(shortPath));
	switch(GetDlgCtrlID(hwndCtl)) //得到控件ID
	{ 
		//控制音量滑块
	case IDC_Volume:
		{        
			voice = SendMessage(GetDlgItem(hwnd,IDC_Volume), TBM_GETPOS, 0, 0); //得到当前滑块的音量值
				//TBM_GETPOS：返回当前滑块位置所代表的值（逻辑位置）
			TCHAR szVoice[256];
			wsprintf(szVoice, "setaudio %s volume to %d", shortPath, voice); //将音量设置到滑块的值
			mciSendString(szVoice, (LPTSTR)NULL, 0, 0);	
		}
		break;
	case IDC_ProgressBar: //歌曲的进度条
		{
			
			if(SB_ENDSCROLL == code)
			{
				long lPosition = SendMessage(GetDlgItem(hwnd,IDC_ProgressBar), TBM_GETPOS, 0, 0); //得到当前的进度值
				//itoa(lPosition, curTime, 10);
				voice = SendMessage(GetDlgItem(hwnd,IDC_Volume), TBM_GETPOS, 0, 0); //得到当前滑块的音量值
				//TBM_GETPOS：返回当前滑块位置所代表的值（逻辑位置）
				TCHAR strPos[256];
				wsprintf(strPos, "play %s from %ld", shortPath, lPosition);
				mciSendString(strPos, (LPTSTR)NULL, 0, 0);												    
				mciSendString(strPos, (LPTSTR)NULL, 0, 0); 
				voice = SendMessage(GetDlgItem(hwnd,IDC_Volume), TBM_GETPOS, 0, 0); //得到当前滑块的音量值
				//TBM_GETPOS：返回当前滑块位置所代表的值（逻辑位置）
				TCHAR szVoice[256];
				wsprintf(szVoice, "setaudio %s volume to %d", shortPath, voice); //将音量设置到滑块的值  再次设音量是为了防止跳转进度的时候声音被重置
				mciSendString(szVoice, (LPTSTR)NULL, 0, 0);	
			
				
			}
        }
		break;
	}
}

//-------------------------------自动转向下一曲---------------------------------------------
void autoNext(HWND hwnd)
{
	srand(time(0));
	mode = ComboBox_GetCurSel(GetDlgItem(hwnd, IDC_PlayMode));  // 获取当前的播放模式
	Stop(hwnd);
	TCHAR str1[256] = "";
	TCHAR str2[256] = "";
	HWND hwndlist = GetDlgItem(hwnd, IDC_MusicList);  // 得到ListBox句柄
	int index = ListBox_GetCurSel(hwndlist);  //得到现在正在播放的歌曲
	int all = ListBox_GetCount(hwndlist);  // 列表中总项
	if (1 == mode)   //如果是单曲循环
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

		if (index ==all-1)   //如果所选是列表最后一项
		{
			ListBox_GetText(hwndlist, index, str1);  // 得到歌曲名
			ListBox_SetCurSel(hwndlist, index);
			Stop(hwnd);
		}
		else
		{
			ListBox_GetText(hwndlist, index+1, str1);  // 得到歌曲名
			ListBox_SetCurSel(hwndlist, index+1);
			Play(hwnd);
		}	
	}


} 
//------------------------保存列表等信息到硬盘----------------------------------------
void SaveList(HWND hwnd, TCHAR *cFileName)
{
	TCHAR fileName[MAX_PATH];
	FILE *fp = fopen(cFileName,"wb");				// 打开保存歌曲文件  
    if(fopen(cFileName,"wb")==NULL)
    {
		MessageBox(hwnd,"保存音乐列表失败！","抱歉",MB_OK|MB_ICONEXCLAMATION); 
	}
	else
	{		
		fwrite(&numOnOpenFile,sizeof(int),1,fp);		// 保存歌曲的数量信息
		fwrite(&mode,sizeof(int),1,fp);					// 保存播放模式
		fwrite(&numOnList,sizeof(int),1,fp);			// 保存记录列表上显示歌曲的序号
		fwrite(&voice,sizeof(int),1,fp);				// 保存音量 
	//	fwrite(&iTransparencyType,sizeof(int),1,fp);	// 保存透明度 
		fwrite(&Skinroad,sizeof(Skinroad),1,fp);        // 保存皮肤
		fread(&(Sset.soundmodeparent),sizeof(int),1,fp);		// 保存声道模式
		for (int i=0; i<numOnOpenFile; i++)				// 保存所有歌曲的路径，名字 
		{
			SendMessage(GetDlgItem(hwnd,IDC_MusicList),LB_GETTEXT,i,(LPARAM)fileName);
			fwrite(&fileName,sizeof(fileName),1,fp);
			fwrite(&szFileName_Path[i],sizeof(szFileName_Path[i]),1,fp);
		}
	}
	fclose(fp);
}






//---------------------------最小化到托盘------------------------感觉不是很好用，先留着以后改进-----------------
void MinimizeToTray(HWND hwnd) 
{
	NOTIFYICONDATA nid;
	nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	nid.hWnd = hwnd;
	nid.uID = IDI_ICON;
	nid.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP;
	nid.uCallbackMessage = WM_MYMESSAGE;								//自定义消息
	HINSTANCE hin = (HINSTANCE)GetWindowLong(hwnd,GWL_HINSTANCE);		//获得程序句柄
	nid.hIcon = LoadIcon(hin,MAKEINTRESOURCE(1));
	lstrcpy(nid.szTip,TEXT("MPlayer"));

	Shell_NotifyIcon(NIM_ADD,&nid);
	ShowWindow(hwnd,SW_HIDE);											//隐藏窗口
}
//-----------------------读取列表信息----------------------------------------
void ReadList(HWND hwnd, TCHAR *cFileName)
{
	TCHAR fileName[MAX_PATH];
	FILE *fp = fopen(cFileName,"rb");
    if (fopen(cFileName,"rb")==NULL)
    {
		MessageBox(hwnd,"读取音乐列表失败！","提示",MB_OK|MB_ICONEXCLAMATION);
	}
    else
    {
		fread(&numOnOpenFile,sizeof(int),1,fp);			// 读取歌曲的数量信息
		fread(&mode,sizeof(int),1,fp);				// 读取播放模式
		fread(&numOnList,sizeof(int),1,fp);				// 读取记录列表上显示歌曲的序号
		fread(&voice,sizeof(int),1,fp);				// 读取音量
//		fread(&iTransparencyType,sizeof(int),1,fp);		// 读取透明度
		fread(&Skinroad,sizeof(Skinroad),1,fp);	       //读取皮肤
		fread(&(Sset.soundmodeparent),sizeof(int),1,fp);				// 读取声道模式
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

//_------------------------------------删除选中歌曲--------------------------------------------	  
	void Delete(HWND hwnd)
{
	HWND hwndlist = GetDlgItem(hwnd, IDC_MusicList);
	int selCount = ListBox_GetCurSel(hwndlist);
	if (selCount ==numPlayMusicNow)  // 如果删除的是当前正在播放的，停止播放音乐，选中下一首
	{
		Stop(hwnd);
	}
	int i;
	for (i = selCount; i < ListBox_GetCount(hwndlist) - 1; i++)  // 将存放的路径信息依次前移
	{
		strcpy(szFileName_Path[i], szFileName_Path[i+1]);
	}
	strcpy(szFileName_Path[i], "");// 将原来最后的设为空 
	ListBox_DeleteString(hwndlist, selCount);  // 从列表中删除选中项
	if(numPlayMusicNow== selCount)//正在播放的歌曲序号等于要删除的歌曲序号，选中列表中第一个歌曲
	{
		numPlayMusicNow=0;
		ListBox_SetCurSel(hwndlist,numPlayMusicNow);  // 选中列表中正在播放的歌曲
	}
	if (numPlayMusicNow > selCount)//正在播放的歌曲序号大于要删除的歌曲序号，依然选中列表中正在播放的歌曲
	{
		numPlayMusicNow--;
		ListBox_SetCurSel(hwndlist,selCount);  // 选中列表中正在播放的歌曲
	}
	ListBox_SetCurSel(hwndlist,selCount);  // 选中列表中正在播放的歌曲 */
	numOnOpenFile--;  
}  
//----------------------清空播放列表------------------------------------------
void DeleteAll(HWND hwnd)
{
	HWND hwndlist = GetDlgItem(hwnd,IDC_MusicList);
	int count = ListBox_GetCount(hwndlist);
	int i;   //利用循环结构来清空

	for(i=count;i>=0;i--)
	{
	ListBox_DeleteString(hwndlist,i);	
	}  
	Stop(hwnd);
	strcpy(strTime, "00:00");
	strcpy(musicTotalTime, "00:00");
	SetDlgItemText(hwnd, IDC_MusicNowTime, strTime);
	SetDlgItemText(hwnd,IDC_MusicTotalTime,musicTotalTime);    //清空歌曲时间显示
	numOnOpenFile = 0;									// 将原来记录打开文件时歌曲的序号清空
	numOnList = 0;										// 将原来列表的序号清空
	numPlayMusicNow = 0;	                            // 将原来正在播放的歌曲序号清空
	SendDlgItemMessage(hwnd,IDC_ProgressBar,TBM_SETPOS,true,(LPARAM)NULL);		// 将滑动条拖回开始位置
	SetDlgItemText(hwnd, IDC_PlayEdit,TEXT(""));   // 正在播放歌曲栏目 清空
	
	FILE *fp;				//从内部真正清空列表
	if((fp=fopen("C:\\Users\\Administrator\\Documents\\列表保存.doc","w"))==NULL)
	{
		MessageBox(hwnd,"清空列表失败","抱歉",MB_OK|MB_ICONEXCLAMATION); 
		exit(0);
	}
	fclose(fp);

}
//------------------------------获取歌曲信息--------------(暂时仅限ID3V1)-----------------------
void GetSongMessage(HWND hwnd)
{	
	HWND hwndlist = GetDlgItem(hwnd, IDC_MusicList);
	int index=ListBox_GetCurSel(hwndlist);
	TCHAR str[256];
	struct tagID3V1
	{
		char Header[4];  // 标签头必须是“TAG”否则认为没有标签
		char Title[31];  // 标题
		char Singer[31];  // 作者
		char Album[31];  // 专辑

	}ID3V1;

	FILE *fp;
	fp = fopen(szFileName_Path[index], "r");
	fseek(fp,-128L,SEEK_END);
	fread(ID3V1.Header, 3, 1, fp);

	// 判断标签头有无“TAG”，没有则认为没有标签
	if (0 == memcmp(ID3V1.Header, "TAG", 3))
		{
		ID3V1.Header[3] = '\0';

		fread(ID3V1.Title, 30, 1, fp);
		ID3V1.Title[30]='\0';

		fread(ID3V1.Singer, 30, 1, fp);
		ID3V1.Singer[30]='\0';

		fread(ID3V1.Album, 30, 1, fp);
		ID3V1.Album[30]='\0';	    
			
		wsprintf(str, "标题 %s\n\n作者 %s\n\n专辑 %s\n", ID3V1.Title, ID3V1.Singer, ID3V1.Album);
		MessageBox(hwnd, str, TEXT("曲目信息"), MB_OK);
		}
	else
		{

			MessageBox(hwnd, TEXT("此歌曲里未包含ID3V1标签"), TEXT("曲目信息"), MB_OK);
		}
	fclose(fp);
	
}
//--------------------------定位当前播放---------------------------------------------------------
void Location(HWND hwnd)
{
HWND hwndlist = GetDlgItem(hwnd,IDC_MusicList);
ListBox_SetCurSel(hwndlist,numPlayMusicNow);
}
//----------------------------判断歌词是否存在-------------------------------------
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

//-----------------------------解析歌词--------------------------------------------
void LRC(void)
{
	//打开歌词文件
//	TCHAR lyrics[256];  //当前目录路径+歌词文件名+后缀
//	TCHAR lyricsX[256]; //  跟上面类似，只不过是上一级目录
	//wsprintf(lyrics,"lrc\\%s.lrc",songname);
	//wsprintf(lyricsX,"..\\%s",lyrics);
	FILE *fp;

	if((fp=fopen(lyrics,"r")) == NULL && (fp=fopen(lyricsX,"r")) == NULL)
	{			
		MessageBox(NULL,"歌词打开错误！","错误",MB_OK);
		exit(1);//exit(1)程序异常退出，exit(0)程序正常退出
	} 
	//动态分配内存,存储读入的歌词信息
	if((p=(char *)calloc(1024,sizeof(char))) == NULL)//calloc分配的内存（堆内存）自动初始化为0
	{
		MessageBox(NULL,"无法申请内存空间！","错误",MB_OK);
		exit(1);
	}
	//动态分配接受字符串的连续空间
	if((ps2 = ps1 = (struct song *)calloc(120,sizeof(struct song)))==NULL)
	{
		MessageBox(NULL,"无法申请内存空间！","错误",MB_OK);
		exit(1);
	}
	
	head1 = p;
	head2 = ps1;
	int flag;
	flag = 1;
	while(!feof(fp))
	{
		p = head1;//每次读取都初始化p
		fscanf(fp,"%c",p);
		while(!(*p==10))
		{
			p++;
			fscanf(fp,"%c",p);
		}
		*p = 0;
		p = head1;
		while(*p != 0)//如果没有前边的作曲信息，就跳过
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
		if(flag == 1)//歌词开始
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
			while(*p=='[' && *(p+1) == '0')//时间分析
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

	//歌词排序（冒泡法）
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
	//TCHAR geci[256][256];  //二维数组把歌词保存起来
	 k=tmp2-head2;    //一共有K行歌词
	for(i=0;i<k;i++)
	{
		wsprintf(geci[i],"%s\r\n",ps1->lrc);
		lrctime[i]=ps1->time;
		ps1++;
	}

		
}
//--------------------------------------------显示歌词-----------------------------------
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
//------------------------歌词染色-------------------------------------------
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
//---------------------------------显示EDIT11的歌词-------------------------------------
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
//---------------------------歌词颜色  EDIT11---------------------------------------------------
HBRUSH SetEditColor11(HWND hWndOwner11,HDC hDCEdit11,HWND hWndEdit11)
{
	HBRUSH hbr;
	
	hEdit11=GetDlgItem(hWndOwner11,IDC_EDIT11);
	hDCEdit11=GetDC(hEdit11);
    if(hWndEdit11==hEdit11)
    {
		SetTextColor(hDCEdit11,testColor);
		SetBkMode(hDCEdit11, TRANSPARENT); //文字的背景透明
		hbr =CreateSolidBrush(RGB(118,77,57));
		//		hbr=GetStockBrush(NULL_BRUSH);
		return hbr;
    }
    else
		return NULL;
}
//---------------------------歌词颜色  EDIT22---------------------------------------------------
HBRUSH SetEditColor22(HWND hWndOwner22,HDC hDCEdit22,HWND hWndEdit22)
{
	HBRUSH hbr;
	
	hEdit22=GetDlgItem(hWndOwner22,IDC_EDIT22);
	hDCEdit22=GetDC(hEdit22);
    if(hWndEdit22==hEdit22)
    {
		SetTextColor(hDCEdit22,testColor);
		SetBkMode(hDCEdit22, TRANSPARENT); //文字的背景透明
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
		SetBkMode(hDCEdit, TRANSPARENT); //文字的背景透明
	//	hbr=GetStockBrush(NULL_BRUSH);
		hbr =CreateSolidBrush(RGB(118,77,57));
		return hbr;
		//hbr=GetStockBrush(NULL_BRUSH);
		//SelectObject(hDCEdit,hbr);
	}
	else
	return NULL;
}
//----------------按钮图案----------------------------------------------------------------
void InitFont(HWND hwnd){
	HFONT hFont= CreateFont( 
		25, //高度
		37, //宽度
		0, //旋转角度
		0, //方向
		60, //线条宽度
		0, //斜体
		0, //下划线
		0, //加粗
		DEFAULT_CHARSET, //字符集
		OUT_DEFAULT_PRECIS, //精度
		CLIP_DEFAULT_PRECIS, //剪裁方式
		DEFAULT_QUALITY, //渲染质量
		DEFAULT_PITCH|FF_DONTCARE, //字体族, //字体族
		"Webdings" //字体名
		); 
	SendMessage(GetDlgItem(hwnd,IDC_Pause),WM_SETFONT,(WPARAM)hFont,0);
	SendMessage(GetDlgItem(hwnd,IDC_NextSong),WM_SETFONT,(WPARAM)hFont,0);
	SendMessage(GetDlgItem(hwnd,IDC_LastSong),WM_SETFONT,(WPARAM)hFont,0);
	SendMessage(GetDlgItem(hwnd,IDC_JudgeShow),WM_SETFONT,(WPARAM)hFont,0);
//	SendMessage(GetDlgItem(hwnd,IDC_Location),WM_SETFONT,(WPARAM)hFont,0);
	

}
//-------------------静音----------------------------------------------------
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
				wsprintf(szVoice, "setaudio %s volume to %d", shortPath, 0); //将音量设置到滑块的值
				mciSendString(szVoice, (LPTSTR)NULL, 0, 0);	
				SendMessage(hwndVolumeSilder, TBM_SETPOS, (WPARAM)TRUE,0); 
				
				}
			/*	else if (voice==0)
				{	
				SetWindowText(hVoice, TEXT("U"));
				wsprintf(szVoice, "setaudio %s volume to %d", shortPath, voicecopy); //将音量设置到滑块的值
				mciSendString(szVoice, (LPTSTR)NULL, 0, 0);
				SendMessage(hwndVolumeSilder, TBM_SETPOS, (WPARAM)TRUE,voicecopy);
				}   */							
}
//---------------------------------简化窗口与扩展------------------------------------------------------------
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
//----------------------迷你模式----------------------------------------
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
//------------------------------本地列表模糊搜索-----------------------------
void SearchLocal(HWND hwnd)
{	
	int i;
//	SetWindowPos(hwndlist,0,122,75,330,500,SWP_NOMOVE|SWP_NOZORDER);
//	TCHAR Listallname[256*100][256];   //列表所有歌名字
	for(i=0;i<numOnList;i++)
	{
	 ListBox_GetText(hwndlist,i,Listallname[i]);
	}
	
	
	TCHAR szSearch[MAX_PATH],szTempSearch[MAX_PATH];
	memset(szSearch,0,sizeof(szSearch));
	memset(szTempSearch,0,sizeof(szTempSearch));               //一个EDIT里的  一个是遍历Listbox   都是一维数组
	HWND hSearchEdit=GetDlgItem(hwnd,IDC_SearchEdit);
	int	iTextLen=GetWindowTextLength(hSearchEdit);            //获取搜索栏字符串长度
	if(!iTextLen)
	{
		MessageBeep(0);
		MessageBox(hwnd,TEXT("请输入要查找的歌曲"),TEXT("提示"),MB_OK|MB_ICONEXCLAMATION);
		
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
		MessageBox(hwnd,TEXT("没有找到需要的歌曲"),TEXT("提示"),MB_OK|MB_ICONEXCLAMATION);		
	}
}
//-------------------------歌词第一步下载XML--------------------------------------------------
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
//---------------------------------------手动搜索--------------------------------------------
 void OnLine(HWND hwnd,LPCTSTR lpFmt)
 {
         TCHAR StrMusicOnline[MAX_PATH];
         GetDlgItemText(hwnd,IDC_EDIT1,EDITsong,sizeof(EDITsong)/sizeof(TCHAR));
         wsprintf(StrMusicOnline,lpFmt,EDITsong);
         ShellExecute(hwnd,"open",StrMusicOnline,NULL,NULL,SW_NORMAL);        
 }