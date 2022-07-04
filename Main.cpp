#include <stdlib.h>
#include "resource.h"
#include "Diagram.h"
#include <iostream>
#include <windows.h> 
#include <time.h>
#include <stdlib.h>
#include <queue>
using namespace std;
#pragma warning(disable:4996)
#define bsize 20 // 다이어그램 크기
#define bsize 20// 다이어그램 크기
#define bx 12// 게임보드크기x
#define by 20// 게임보드크기y
#define sx 4 // 블록 시작위치 x
#define sy 0 // 블록 시작위치 y
#define b2 500 // gameboard2 떨어져 있는 거리
Diagram diagram,diagram2; // 블럭 개체
Diagram diagramA[5]; // 다음블럭 들어있는 배열
Diagram diagram2A[5];
HDC hdc, bhdc, bhdc2, bhdc3; // 비트맵 출력을 위한 붓
HBITMAP block, block2, block3; // 비트맵
HINSTANCE hinstance_; // 프로그램 인스턴스
HWND hWnd_; // 현재 윈도우 핸들
int GameBoard[bx][by]; // 게임보드 1
int GameBoard2[bx][by]; // 게임보드 2
int GameBoardAi[bx][by]; // 게임보드 Ai
int speed = 500; // 게임속도
int speedAi =1000; // Ai 게임속도
int score1 = 0; // 점수 1
int score2 = 0; // 점수 2
int Aiscore = 0; // 점수 Ai
// Ai 점수계산을 위한 임시변수
int newscore = 0; 
int BestMove = 0;
int Aiset = 0;
int set = 0;
int set2 = 0;
int BlockHeight = 0; // 블록높이의 합
int BlankCount = 0; // 블록사이에 있는 빈공간의 개수
int CompliteLine = 0; // 완성된 줄의 개수
int MeetBlock = 0; // 현재 블록이 기존 블럭들과 닿는 면의 개수
int MeetWall = 0; // 현재 블록이 좌우 벽면과 닿는 면의 개수
int MeetFloor = 0; //현재 블록이 바닥면과 닿는 면의 개수
// 폭탄아이템 개수
int bomb1 = 1;
int bombs1 = 2;
int bomb2 = 1;
int bombs2 = 2;

// 7개 블록에 대한 정보
int bvals[7][4][4][4] =
{
	{
		{
			{0,0,1,0}, // l 모양
			{0,0,1,0},
			{0,0,1,0},
			{0,0,1,0}
		},
		{
			{0,0,0,0},
			{0,0,0,0},
			{1,1,1,1},
			{0,0,0,0}
		},
		{
			{0,0,1,0},
			{0,0,1,0},
			{0,0,1,0},
			{0,0,1,0}
		},
		{
			{0,0,0,0},
			{0,0,0,0},
			{1,1,1,1},
			{0,0,0,0}
		}
	},
	{
		{
			{0,0,0,0}, // ㅁ 모양
			{0,1,1,0},
			{0,1,1,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{0,1,1,0},
			{0,1,1,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{0,1,1,0},
			{0,1,1,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{0,1,1,0},
			{0,1,1,0},
			{0,0,0,0}
		}
	},
	{
		{
			{0,0,1,0}, // ㄴ 모양
			{0,0,1,0},
			{0,0,1,1},
			{0,0,0,0}
		},
		{
			{0,1,1,1},
			{0,1,0,0},
			{0,0,0,0},
			{0,0,0,0}
		},
		{
			{0,0,1,1},
			{0,0,0,1},
			{0,0,0,1},
			{0,0,0,0}
		},
		{
			{0,0,0,1},
			{0,1,1,1},
			{0,0,0,0},
			{0,0,0,0}
		}
	},
	{
		{
			{0,0,0,1}, //ㄱ 모양
			{0,0,0,1},
			{0,0,1,1},
			{0,0,0,0}
		},
		{
			{0,1,0,0},
			{0,1,1,1},
			{0,0,0,0},
			{0,0,0,0}
		},
		{
			{0,0,1,1},
			{0,0,1,0},
			{0,0,1,0},
			{0,0,0,0}
		},
		{
			{0,1,1,1},
			{0,0,0,1},
			{0,0,0,0},
			{0,0,0,0}
		}
	},
	{
		{
			{0,0,1,0}, // ㅑㅑ 모양
			{0,0,1,1},
			{0,0,0,1},
			{0,0,0,0}
		},
		{
			{0,0,1,1},
			{0,1,1,0},
			{0,0,0,0},
			{0,0,0,0}
		},
		{
			{0,0,1,0},
			{0,0,1,1},
			{0,0,0,1},
			{0,0,0,0}
		},
		{
			{0,0,1,1},
			{0,1,1,0},
			{0,0,0,0},
			{0,0,0,0}
		}
	},
	{
		{
			{0,0,0,1}, // ㅕㅕ 모양
			{0,0,1,1},
			{0,0,1,0},
			{0,0,0,0}
		},
		{
			{0,1,1,0},
			{0,0,1,1},
			{0,0,0,0},
			{0,0,0,0}
		},
		{
			{0,0,0,1}, // ㅕㅕ 모양
			{0,0,1,1},
			{0,0,1,0},
			{0,0,0,0}
		},
		{
			{0,1,1,0},
			{0,0,1,1},
			{0,0,0,0},
			{0,0,0,0}
		}
	},
	{
		{
			{0,0,0,0}, // ㅗ 모양
			{0,1,1,1},
			{0,0,1,0},
			{0,0,0,0}
		},
		{
			{0,0,1,0},
			{0,0,1,1},
			{0,0,1,0},
			{0,0,0,0}
		},
		{
			{0,0,1,0},
			{0,1,1,1},
			{0,0,0,0},
			{0,0,0,0}
		},
		{
			{0,0,1,0},
			{0,1,1,0},
			{0,0,1,0},
			{0,0,0,0}
		}
	}
}
;
// Ai 점수계산을 위한 임시객체
Diagram Bestdiagram;
Diagram Currentdiagram;
HANDLE hTimer = (HANDLE)SetTimer(hWnd_, 1, speed, NULL);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProc2(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProc3(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProc4(HWND, UINT, WPARAM, LPARAM);

void SetGameboard(); // 게임보드 초기화
void SetGameboard2();
void DrawGameboard(); // 게임보드 그리기
void DrawGameboard2();
void Drawdiagram(); // 현재 다이어그램 그리기
void Drawdiagram2();
void Drawgradation(); // 수직, 수평선 그리기
void Drawgradation2();
int MoveTurn(); // 도형 회전
int MoveTurn2();
int MoveDown(); // 도형 하강
int MoveDown2();
int MoveRight(); // 도형 오른쪽 이동
int MoveRight2();
int MoveLeft(); // 도형 왼쪽 이동
int MoveLeft2();
int MoveEnable_Turn(); // 도형 회전 가능한지 판단
int MoveEnable_Turn2();
int MoveEnable(int xx, int yy); // 도형 이동 가능한지 판단
int MoveEnable2(int xx, int yy);
void Store(); // 도형 게임보드에 저장
void Store2();
void Drawnextdiagram_Turn(); // 회전할 다음 도형 그리기
void Drawnextdiagram_Turn2();
void Drawnextdiagram(int mx, int my); // 이동할 다음 도형 그리기
void Drawnextdiagram2(int mx, int my);
void Drawbackscreen(); // 배경 그리기
void Drawbackscreen2();
void Reset(); // 현재 도형 시작위치로 초기화 및 턴정보 도형 랜덤 초기화
void Reset2();
int Checkline(); // 게임보드에 만들어진 라인이 있는지 검사
int Checkline2();
void Shiftline(int y); //y좌표 위의 도형 아래로 한칸 떨어뜨리기
void Shiftline2(int y);
void Destroyline(int y); //y좌표 라인 파괴
void Destroyline2(int y);

void StartAi(); // 최적의 위치를 찾아서 도형 하강
int CheckboardAi(); // 최적의 위치 점수 계산을 위한 함수
void Delete(); // 현재 도형위치 게임보드에서 삭제
//Vs Com을 구현하기 위한 함수들
int MoveDownAi();
int MoveRightAi(int xx);
int MoveLeftAi(int xx);
int MoveTurnAi(int tt);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszArg, int nCmdShow)
{
	HWND hWnd;
	MSG msg;
	WNDCLASS WndClass,WndClass2, WndClass3, WndClass4;
	hinstance_ = hInstance;

	WndClass.style = NULL; // 메인 윈도우 클래스
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = "Main";

	WndClass2.style = NULL; // 1인용 게임 윈도우 클래스
	WndClass2.lpfnWndProc = WndProc2;
	WndClass2.cbClsExtra = 0;
	WndClass2.cbWndExtra = 0;
	WndClass2.hInstance = hInstance;
	WndClass2.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass2.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass2.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass2.lpszMenuName = NULL;
	WndClass2.lpszClassName = "WND2";

	WndClass3.style = NULL;// 2인용 게임 윈도우 클래스
	WndClass3.lpfnWndProc = WndProc3;
	WndClass3.cbClsExtra = 0;
	WndClass3.cbWndExtra = 0;
	WndClass3.hInstance = hInstance;
	WndClass3.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass3.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass3.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass3.lpszMenuName = NULL;
	WndClass3.lpszClassName = "WND3";

	WndClass4.style = NULL; // VsCom 게임 윈도우 클래스
	WndClass4.lpfnWndProc = WndProc4;
	WndClass4.cbClsExtra = 0;
	WndClass4.cbWndExtra = 0;
	WndClass4.hInstance = hInstance;
	WndClass4.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass4.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass4.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass4.lpszMenuName = NULL;
	WndClass4.lpszClassName = "WND4";

	if (!RegisterClass(&WndClass)) return NULL;
	if (!RegisterClass(&WndClass2)) return NULL;
	if (!RegisterClass(&WndClass3)) return NULL;
	if (!RegisterClass(&WndClass4)) return NULL;

	hWnd = CreateWindow(
		"Main",
		"Tetris",
		WS_OVERLAPPEDWINDOW,
		600,
		300,
		550,
		500,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}
// 메인 윈도우 이벤트 처리 
LRESULT CALLBACK WndProc(HWND hWnd, UINT mesg, WPARAM wParam, LPARAM lParam) 
{
	static HWND hWnd2,hWnd3;
	static HWND hBtnl1, hBtnl2, hBtnl3,hCheatE,hCheatB, hCheatL;
	static HANDLE hThread;
	DWORD dwThreadID;
	PAINTSTRUCT ps;
	switch (mesg)
	{

	case WM_CREATE: // 게임준비
		//비트맵 초기화
		hdc = GetDC(hWnd);
		bhdc = CreateCompatibleDC(hdc);
		bhdc2 = CreateCompatibleDC(hdc);
		block = LoadBitmap(hinstance_, MAKEINTRESOURCE(IDB_BLOCK));
		SelectObject(bhdc, block);
		block2 = LoadBitmap(hinstance_, MAKEINTRESOURCE(IDB_BLANK));
		SelectObject(bhdc2, block2);
		// 게임보드 초기화
		SetGameboard();
		Reset();

		BitBlt(hdc, 0, 0, 30, 30, bhdc, 0, 0, SRCCOPY);

		hBtnl1 = CreateWindow("BUTTON", "1인용", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			30, 350, 100, 100, hWnd, (HMENU)111, hinstance_, NULL); // 버튼 추가	

		hBtnl2 = CreateWindow("BUTTON", "2인용", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			180, 350, 100, 100, hWnd, (HMENU)222, hinstance_, NULL); // 버튼 추가

		hBtnl3 = CreateWindow("BUTTON", "VS COM", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			330, 350, 100, 100, hWnd, (HMENU)333, hinstance_, NULL); // 버튼 추가

		hCheatE = CreateWindow("EDIT", "치트기 입력", WS_VISIBLE | WS_CHILD | WS_BORDER
			| ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE, 30, 290, 300, 30, hWnd,
			(HMENU)444, hinstance_, NULL); // 텍스트 입력란 추가
		hCheatB = CreateWindow("BUTTON", "입력", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			340, 290, 50, 30, hWnd, (HMENU)555, hinstance_, NULL); // 버튼 추가
		hCheatL = CreateWindow("BUTTON", "치트키 목록", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			400, 290, 100, 30, hWnd, (HMENU)666, hinstance_, NULL); // 버튼 추가


		break;
	case WM_PAINT: 
		hdc = BeginPaint(hWnd, &ps);
		// 조작키 설명
		TextOut(hdc, 50, 30, "!!! 조작키 !!!", sizeof("!!! 조작키 !!"));
		TextOut(hdc,50,90,"1P ",sizeof("1"));
		TextOut(hdc, 50, 110, "회전 : W", sizeof("회전 : "));
		TextOut(hdc, 50, 130, "좌로이동 : A", sizeof("좌로이동 : "));
		TextOut(hdc, 50, 150, "우로이동 : D", sizeof("우로이동 : "));
		TextOut(hdc, 50, 170, "아래로 이동 : S", sizeof("아래로 이동 : "));
		TextOut(hdc, 50, 190, "낙하 : SPACE", sizeof("낙하 : SPAC"));

		TextOut(hdc, 250, 90, "2P ", sizeof("1"));
		TextOut(hdc, 250, 110, "회전 : UP", sizeof("회전 :  "));
		TextOut(hdc, 250, 130, "좌로이동 : LEFT", sizeof("좌로이동 :    "));
		TextOut(hdc, 250, 150, "우로이동 : RIGHT", sizeof("우로이동 :     "));
		TextOut(hdc, 250, 170, "아래로 이동 : DOWN", sizeof("아래로 이동 :    "));
		TextOut(hdc, 250, 190, "낙하 : L", sizeof("낙하   "));

		break;
	case WM_COMMAND:
		int id;
		id = LOWORD(wParam); // id 얻기
		if (id == 111) { // 1인용 모드 실행
			hWnd2 = CreateWindow("WND2", "1인용 모드", WS_OVERLAPPEDWINDOW| WS_VISIBLE,
				600,
				300,
				30*bsize, 30 * bsize, hWnd, NULL,hinstance_, NULL);
		}
		else if (id == 222) {	// 2인용 모드 실행
			hWnd3 = CreateWindow("WND3", "2인용 모드", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
				600,
				300,
				1000, 30 * bsize, hWnd, NULL, hinstance_, NULL);
		}
		else if (id == 333) { //Vscom 모드 실행
			hWnd3 = CreateWindow("WND4", "Vs Com 모드", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
				600,
				300,
				1000, 30 * bsize, hWnd, NULL, hinstance_, NULL);

		}
		else if (id == 555) {
			// 치트키 비교
			char sz[1024];
			GetWindowText(hCheatE, sz, 30);
			if (!strcmp(sz, "P1 Bomb")) {
				MessageBox(hWnd_, "Player1 큰폭탄 + 1", "치트키적용완료 !", NULL);
				bomb1 += 1;
			}else if (!strcmp(sz, "P2 Bomb")) {
				MessageBox(hWnd_, "Player2 큰폭탄 + 1", "치트키적용완료 !", NULL);
				bomb2 += 1;
			}
			else if (!strcmp(sz, "P1 sBomb")) {
				MessageBox(hWnd_, "Player2 작은폭탄 + 1", "치트키적용완료 !", NULL);
				bombs1 += 1;
			}
			else if (!strcmp(sz, "P2 sBomb")) {
				MessageBox(hWnd_, "Player2 작은폭탄 + 1", "치트키적용완료 !", NULL);
				bombs2 += 1;
			}
			else if (!strcmp(sz, "Speed down")) {
				MessageBox(hWnd_, "속도 감소 + 0.1", "치트키적용완료 !", NULL);
				speed += 100;
			}
			else if (!strcmp(sz, "Speed up")) {
				MessageBox(hWnd_, "속도 감소 - 0.1", "치트키적용완료 !", NULL);
				speed -= 100;
			}
			else {
				MessageBox(hWnd_, "다시 입력해 주세요.", "치트키적용실패...", NULL);
			}
		}
		else if (id == 666) {
			ShellExecute(NULL, ("open"), ("치트키.txt"), NULL, NULL, SW_SHOW);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return FALSE;
	}
	return DefWindowProc(hWnd, mesg, wParam, lParam);
}

// 1인용 게임 윈도우 이벤트 처리
LRESULT CALLBACK WndProc2(HWND hWnd, UINT mesg, WPARAM wParam, LPARAM lParam)
{
	hWnd_ = hWnd;
	PAINTSTRUCT ps;
	time_t mytime;
	static HANDLE hTimer, hTimer2;
	static char sz[1024];
	switch (mesg)
	{
	case WM_CREATE:
		MessageBox(hWnd_, " 1인용 게임을 시작합니다\n 게임은 바로 시작됩니다", "!! 알림 !!", NULL);
		// 비트맵 초기화
		hdc = GetDC(hWnd);
		bhdc = CreateCompatibleDC(hdc);
		bhdc2 = CreateCompatibleDC(hdc);
		block = LoadBitmap(hinstance_, MAKEINTRESOURCE(IDB_BLOCK));
		SelectObject(bhdc, block);
		block2 = LoadBitmap(hinstance_, MAKEINTRESOURCE(IDB_BLANK));
		SelectObject(bhdc2, block2);
		//시간객체 초기화
		hTimer = (HANDLE)SetTimer(hWnd, 1, speed, NULL);
		SetGameboard();
		break;

	case WM_TIMER:
		switch (wParam) {
		case 1: // 초마다 배경그리기, 블록 하강, 라인체크
			Drawgradation();
			MoveDown();
			Checkline();
			break;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		Drawbackscreen();
		Drawgradation();
		Drawdiagram();
		DrawGameboard();
		hdc = GetDC(hWnd);
		sprintf_s(sz, "현재 점수는 : %d 점 ", score1);
		TextOut(hdc, 300, 50, sz, strlen(sz));
		sprintf_s(sz, "현재 0.%d초 마다 하강 ", speed);
		TextOut(hdc, 300, 70, sz, strlen(sz));
		sprintf_s(sz, "Q 현재 큰폭탄 개수 : %d개 ", bomb1);
		TextOut(hdc, 300, 110, sz, strlen(sz));
		sprintf_s(sz, "E 현재 작은폭탄 개수 : %d개 ", bombs1);
		TextOut(hdc, 300, 130, sz, strlen(sz));


		sprintf_s(sz, "----- 다음나올 블록 ----- ");
		TextOut(hdc, 280, 270, sz, strlen(sz));
		// 다음 나올 도형 출력
		for (int i = 0; i < bx; i++) {
			Rectangle(hdc, 280, 300, 400, 400);
		}
		for (int x = 0; x < 4; x++) {
			for (int y = 0; y < 4; y++) {
				if (bvals[diagramA[(set)%5].BlockNum][diagramA[set % 5].Turn][x][y] != 0) {
					BitBlt(hdc, (15 + x) * bsize, (15 + y) * bsize,
						bsize, bsize, bhdc, 0, 0, SRCCOPY);

				}
			}
		}
		
		break;
		
	case WM_KEYDOWN: // 키보드 이벤트에 따른 이벤트 처리
		Drawgradation();
		switch (wParam) {
		case 'A':
			MoveLeft();
			break;
		case 'D':
			MoveRight();
			break;
		case 'W':
			MoveTurn();
			break;
		case 'S':
			MoveDown();
			break;
		case 'Q':
			if (bomb1 > 0) {
				bomb1--;
				SetGameboard();
			}	
			InvalidateRect(hWnd_, NULL, TRUE);
			break;
		case 'E':
			if (bombs1 > 0) {
				bombs1--;
				Destroyline(19);
				Shiftline(19);
			}
			InvalidateRect(hWnd_, NULL, TRUE);
			break;
		case VK_SPACE:
			while (MoveDown());
			break;
		}
		break;

	case WM_DESTROY: // 윈도우가 파괴되면 필요한 변수들 리셋
		speed = 500;
		score1 = 0;
		Reset();
		bomb1 = 1;
		bomb2 = 1;
		bombs1 = 2;
		bombs2 = 2;
		SetGameboard();
		SetGameboard2();
		return FALSE;
	}
	return DefWindowProc(hWnd, mesg, wParam, lParam);
}
// 2인용 게임 윈도우 이벤트 처리
LRESULT CALLBACK WndProc3(HWND hWnd, UINT mesg, WPARAM wParam, LPARAM lParam)
{
	hWnd_ = hWnd;
	PAINTSTRUCT ps;
	time_t mytime;
	static HANDLE hTimer, hTimer2;
	static char sz[1024];
	switch (mesg)
	{
	case WM_CREATE:
		MessageBox(hWnd_, " 2인용 게임을 시작합니다\n 게임은 바로 시작됩니다", "!! 알림 !!", NULL);
		hdc = GetDC(hWnd);
		bhdc = CreateCompatibleDC(hdc);
		bhdc2 = CreateCompatibleDC(hdc);

		block = LoadBitmap(hinstance_, MAKEINTRESOURCE(IDB_BLOCK));
		SelectObject(bhdc, block);

		block2 = LoadBitmap(hinstance_, MAKEINTRESOURCE(IDB_BLANK));
		SelectObject(bhdc2, block2);
		Reset();
		Reset2();
		SetGameboard();
		SetGameboard2();
		hTimer = (HANDLE)SetTimer(hWnd, 1, speed, NULL);
		
		break;

	case WM_TIMER:
		switch (wParam) {
		case 1:
			Drawgradation2();
			MoveDown();
			MoveDown2();
			Checkline();
			Checkline2();
			break;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		Drawbackscreen2();
		Drawgradation2();
		Drawdiagram2();
		DrawGameboard2();
		sprintf_s(sz, "현재 점수는 : %d 점 ", score1);
		hdc = GetDC(hWnd);
		TextOut(hdc, 300, 50, sz, strlen(sz));
		sprintf_s(sz, "현재 0.%d초 마다 하강 ", speed);
		TextOut(hdc, 300, 70, sz, strlen(sz));
		sprintf_s(sz, "Q 현재 큰 폭탄 개수 : %d개 ", bomb1);
		TextOut(hdc, 300, 110, sz, strlen(sz));
		sprintf_s(sz, "E 현재 작은폭탄 개수 : %d개 ", bombs1);
		TextOut(hdc, 300, 130, sz, strlen(sz));
		sprintf_s(sz, "----- 다음나올 블록 ----- ");
		TextOut(hdc, 280, 270, sz, strlen(sz));
		// 다음 블록 출력
		for (int i = 0; i < bx; i++) {
			Rectangle(hdc, 280, 300, 400, 400);
		}
		for (int x = 0; x < 4; x++) {
			for (int y = 0; y < 4; y++) {
				if (bvals[diagramA[(set) % 5].BlockNum][diagramA[set % 5].Turn][x][y] != 0) {
					BitBlt(hdc, (15 + x) * bsize, (15 + y) * bsize,
						bsize, bsize, bhdc, 0, 0, SRCCOPY);

				}
			}
		}

		sprintf_s(sz, "현재 점수는 : %d 점 ", score2);
		hdc = GetDC(hWnd);
		TextOut(hdc, 800, 50, sz, strlen(sz));
		sprintf_s(sz, "현재 0.%d초 마다 하강 ", speed);
		TextOut(hdc, 800, 70, sz, strlen(sz));
		sprintf_s(sz, "K 현재 큰 폭탄 개수 : %d개 ", bomb2);
		TextOut(hdc, 800, 110, sz, strlen(sz));
		sprintf_s(sz, "J 현재 작은폭탄 개수 : %d개 ", bombs2);
		TextOut(hdc, 800, 130, sz, strlen(sz));
		sprintf_s(sz, "----- 다음나올 블록 ----- ");
		TextOut(hdc, 780, 270, sz, strlen(sz));
		// 다음 블록 출력
		for (int i = 0; i < bx; i++) {
			Rectangle(hdc, 780, 300, 900, 400);
		}
		for (int x = 0; x < 4; x++) {
			for (int y = 0; y < 4; y++) {
				if (bvals[diagram2A[(set2) % 5].BlockNum][diagram2A[set2 % 5].Turn][x][y] != 0) {
					BitBlt(hdc, (40 + x) * bsize, (15 + y) * bsize,
						bsize, bsize, bhdc, 0, 0, SRCCOPY);

				}
			}
		}

		break;
		// 키보드 이벤트에 따른 이벤트 처리
	case WM_KEYDOWN:
		Drawgradation2();
		switch (wParam) {
		case 'K':
			if (bomb2 > 0) {
				bomb2--;
				SetGameboard2();
			}
			InvalidateRect(hWnd_, NULL, TRUE);
			while (MoveDown2());
			break;
		case 'E':
			if (bombs1 > 0) {
				bombs1--;
				Destroyline(19);
				Shiftline(19);
			}
			InvalidateRect(hWnd_, NULL, TRUE);
			break;
		case 'L':
			while (MoveDown2());
			break;
		case VK_LEFT:
			MoveLeft2();		
			break;
		case VK_RIGHT:
			MoveRight2();		
			break;
		case VK_UP:
			MoveTurn2();			
			break;
		case VK_DOWN:
			MoveDown2();		
			break;
		case VK_SPACE:
			while (MoveDown());
			break;
		case 'W':
			MoveTurn();
			break;
		case 'A':
			MoveLeft();
			break;
		case 'S':
			MoveDown();
			break;
		case 'D':
			MoveRight();
			break;
		case 'Q':
			if (bomb1 > 0) {
				bomb1--;
				SetGameboard();
			}
			Reset();
			InvalidateRect(hWnd_, NULL, TRUE);
			break;
		case 'J':
			if (bombs2 > 0) {
				bombs2--;
				Destroyline2(19);
				Shiftline2(19);
			}
			InvalidateRect(hWnd_, NULL, TRUE);
			break;
		}
		break;
		// 윈도우가 파괴되면 필요한 변수 다시 리셋
	case WM_DESTROY:
		speed = 500;
		score1 = 0;
		Reset();
		score2 = 0;
		Reset2();
		bomb1 = 1;
		bomb2 = 1;
		bombs1 = 2;
		bombs2 = 2;
		SetGameboard();
		SetGameboard2();
		return FALSE;
	}
	return DefWindowProc(hWnd, mesg, wParam, lParam);
}
// VS com 게임 윈도우 이벤트 처리
LRESULT CALLBACK WndProc4(HWND hWnd, UINT mesg, WPARAM wParam, LPARAM lParam)
{
	hWnd_ = hWnd;
	PAINTSTRUCT ps;
	time_t mytime;
	static HANDLE hTimer, hTimer2;
	static char sz[1024];
	switch (mesg)
	{
	case WM_CREATE:
		hdc = GetDC(hWnd);
		bhdc = CreateCompatibleDC(hdc);
		bhdc2 = CreateCompatibleDC(hdc);

		block = LoadBitmap(hinstance_, MAKEINTRESOURCE(IDB_BLOCK));
		SelectObject(bhdc, block);

		block2 = LoadBitmap(hinstance_, MAKEINTRESOURCE(IDB_BLANK));
		SelectObject(bhdc2, block2);	
		
		SelectObject(bhdc2, block2);
		speedAi = 3000;
	
		
		hTimer = (HANDLE)SetTimer(hWnd, 1, speed, NULL);
		hTimer2 = (HANDLE)SetTimer(hWnd, 2, speedAi, NULL);
		SetGameboard2();
		break;

	case WM_TIMER:
		switch (wParam) {
		case 1:
			Drawgradation2();
			MoveDown();		
			Checkline();		
			break;
		case 2:	 //초마다 최적의 위치에 블록 하강
			Bestdiagram.BlockNum = diagram2.BlockNum;
			Bestdiagram.Turn = diagram2.Turn;
			Bestdiagram.X = diagram2.X;
			Bestdiagram.Y = diagram2.Y;
			StartAi();	
			Checkline2();	
			break;
		}
		
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		Drawbackscreen2();
		Drawgradation2();
		Drawdiagram2();
		DrawGameboard2();
		hdc = GetDC(hWnd);
		//1
		sprintf_s(sz, "현재 점수는 : %d 점 ", score1);
		TextOut(hdc, 300, 50, sz, strlen(sz));
		sprintf_s(sz, "현재 0.%d초 마다 하강 ", speed);
		TextOut(hdc, 300, 70, sz, strlen(sz));
		sprintf_s(sz, "현재 0.%d초 마다 하강 ", speed);
		TextOut(hdc, 300, 70, sz, strlen(sz));
		sprintf_s(sz, "Q 현재 큰 폭탄 개수 : %d개 ", bomb1);
		TextOut(hdc, 300, 110, sz, strlen(sz));
		sprintf_s(sz, "E 현재 작은폭탄 개수 : %d개 ", bombs1);
		TextOut(hdc, 300, 130, sz, strlen(sz));
		sprintf_s(sz, "----- 다음나올 블록 ----- ");
		TextOut(hdc, 280, 270, sz, strlen(sz));

		for (int i = 0; i < bx; i++) {
			Rectangle(hdc, 280, 300, 400, 400);
		}
		for (int x = 0; x < 4; x++) {
			for (int y = 0; y < 4; y++) {
				if (bvals[diagramA[(set) % 5].BlockNum][diagramA[set % 5].Turn][x][y] != 0) {
					BitBlt(hdc, (15 + x) * bsize, (15 + y) * bsize,
						bsize, bsize, bhdc, 0, 0, SRCCOPY);

				}
			}
		}
		//2
		sprintf_s(sz, "현재점수 : %d 점 ", score2);
		TextOut(hdc, 800, 50, sz, strlen(sz));
		sprintf_s(sz, "BlockHeight : %d", BlockHeight);
		TextOut(hdc, 800, 70, sz, strlen(sz));
		sprintf_s(sz, "BlankCount : %d", BlankCount);
		TextOut(hdc, 800, 90, sz, strlen(sz));
		sprintf_s(sz, "MeetWall : %d", MeetWall);
		TextOut(hdc, 800, 110, sz, strlen(sz));
		sprintf_s(sz, "MeetBlock : %d", MeetBlock);
		TextOut(hdc, 800, 130, sz, strlen(sz));
		sprintf_s(sz, "MeetFloor : %d", MeetFloor);
		TextOut(hdc, 800, 150, sz, strlen(sz));
		sprintf_s(sz, "diagram2 : %d %d %d %d", diagram2.BlockNum, diagram2.Turn, diagram2.X, diagram2.Y);
		TextOut(hdc, 800, 170, sz, strlen(sz));
		sprintf_s(sz, "Aiscore : %d", Aiscore);
		TextOut(hdc, 800, 10, sz, strlen(sz));
		TextOut(hdc, bsize*(bx+25), by*bsize, "여기!", strlen("여기!"));
		TextOut(hdc, b2, by * bsize, "여기!", strlen("여기!"));
		break;

	case WM_KEYDOWN:
		Drawgradation();
		switch (wParam) {
		case 'A':
			MoveLeft();
			break;
		case 'D':
			MoveRight();
			break;
		case 'W':
			MoveTurn();
			break;
		case 'S':
			MoveDown();
			break;
		case VK_SPACE:
			while (MoveDown());	
			break;
		}
		break;

	case WM_DESTROY:
		speed = 500;
		score1 = 0;
		score2 = 0;
		Reset();
		Reset2();
		return FALSE;
	}
	return DefWindowProc(hWnd, mesg, wParam, lParam);
}

#pragma region Tetris // 테트리스 함수


int MoveRightAi(int xx) {	
		for (int x = 0; x < 4; x++) {
			for (int y = 0; y < 4; y++) {
				if (bvals[diagram2.BlockNum][diagram2.Turn][x][y] != 0) {
					if ((diagram2.X + x + 1) >= bx + 25) {
						return 0;
					}
				}
			}
		}
	for (int i = 0; i < xx; i++) { // xx 만큼 이동
		if (!MoveEnable2(1, 0)) {
			Drawnextdiagram2(1, 0);
			diagram2.MoveRight();
			return 1;
		}
	}	
}

int MoveLeftAi(int xx) {
		for (int x = 0; x < 4; x++) {
			for (int y = 0; y < 4; y++) {
				if (bvals[diagram2.BlockNum][diagram2.Turn][x][y] != 0) { // 4x4 배열에 블록의 형태가
					if ((diagram2.X + x) <= 25) { // 그려지는 블록의 x 좌표들 중 0보다 작거나 같으면 return 0
						return 0;
					}
				}
			}
		}
	for (int i = 0; i < xx; i++) {
		if (!MoveEnable2(-1, 0)) { // 블록의 x 축을 -1만큼 이동 했을 때 쌓여있는 블럭이 없는지 검사
			Drawnextdiagram2(-1, 0); // 블록을 x 축을 -1만큼 이동한 블럭 그리기
			diagram2.MoveLeft(); // diagram의 멤버변수 x -1 
			return 1;
		}
	}
}

int MoveTurnAi(int tt) {
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			if (bvals[diagram2.BlockNum][(diagram2.Turn + tt) % 4][x][y] != 0) {
				if ((diagram2.X + x) < 25 || (diagram2.X + x) >= bx + 25 || (diagram2.Y + y + 1) >= by) {
					return 0;
				}
			}
		}
	}
	if (!MoveEnable_Turn2()) { // 블록 회전
		Drawnextdiagram_Turn2();
		diagram2.MoveTurn();;
		return 1;
	}
}

void Delete() {
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			if (bvals[diagram2.BlockNum][diagram2.Turn][x][y] != 0) {
				GameBoard2[diagram2.X + x -25][diagram2.Y + y] = 0;
			}
		}
	}
}

int MoveDownAi() {
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			if (bvals[diagram2.BlockNum][diagram2.Turn][x][y] != 0) {
				if ((diagram2.Y + y + 1) >= by) {
					Store2();
					//MessageBox(hWnd_, "Player1 패배", "게임종료 !", NULL);
					return 0;
				}
			}
		}
	}
	if (!MoveEnable2(0, 1)) {
		Drawnextdiagram2(0, 1);
		diagram2.MoveDown();
		return 1;
	}
	else
	{
		Store2();
		
		return 0;
	}
		
}

int CheckboardAi() {
	newscore = 0;
	int line = 0;
	MeetFloor = 0;
	BlockHeight = 0;
	MeetWall = 0;
	while (MoveDownAi());	
	// 줄이 높을떄
	for (int i = 0; i < by; i++) {  
		int total = 0;
		for (int j = 0; j < bx; j++) {
			total += GameBoard2[j][i];
		}
		if (total != 0) {
			BlockHeight += 1;
		}
	}
	newscore -= (BlockHeight * 1000);
	// 줄이 완성 되었을 때	
	for (int i = 0; i < by; i++) {  
		int total = 0;
		for (int j = 0; j < bx; j++) {
			total += GameBoard2[j][i];
		}
		if (total == bx) { 
			newscore = 55555000;
			break;	
		}
	}

	// 게임보드 점수 계산
	for (int i = 0; i < by; i++) {   
		for (int j = 0; j < bx; j++) {
			if (GameBoard2[j][i] == 1) {
				newscore += line;
			}
		}
		line +=120;
	}
	line = 0;

	// 게임보드 점수 계산
	for (int i = BlockHeight; i >= 0; i--) {
		for (int j = 0; j < bx; j++) {
			if (GameBoard2[j][i] == 1) {
				newscore += line;
			}
		}
		line -=100;
	}
	// 벽에 붙을 때
	for (int i = 0; i < by; i++) {
		for (int j = 0; j < bx; j++) {
			if (GameBoard2[0][i] == 1 || GameBoard2[bx-1][i] ==1) {
				newscore +=100;
			}		
		}
	}

	// 현재 블록이 좌우 벽면과 닿는 면의 개수
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			if (bvals[diagram2.BlockNum][diagram2.Turn][x][y] != 0) {
				if ((diagram2.X + x) <= 25) {
					MeetWall += 1;
				}
				if ((diagram2.X + x) >= 25 + bx - 1) {
					MeetWall += 1;
				}
			}
		}
	}
	//newscore += (MeetWall * 150);
	//바닥에 붙을 때
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			if (bvals[diagram2.BlockNum][diagram2.Turn][x][y] != 0) {
				if ((diagram2.X + y + 1) >= by) {
					MeetFloor += 1;
				}
			}
		}
	}
	//newscore += (MeetFloor * 200);

	for (int i = 0; i < by-1; i++) {
		for (int j = 0; j < bx; j++) {
			if (GameBoard2[j][i] == 1) {
				if (GameBoard2[j][i - 1] != 1) {
					newscore -= 250;
					if (GameBoard2[j][i - 2] != 1) {
						newscore -= 200;
						if (GameBoard2[j][i - 2] != 1) {
							newscore -= 150;
						}
					}
				}
			}				
		}
	}

	// 블록높이의 합 !!!!!
	for (int i = 0; i < by; i++) {  
		int total = 0;
		for (int j = 0; j < bx; j++) {
			total += GameBoard2[j][i];
		}
		if (total != 0) {
			BlockHeight += 1;
		}
		if (i == by - 1) {
			newscore += (total * 1000);
		}
	}
	Delete();
	if (Aiscore <= newscore) {	
  		Aiscore = newscore;	
		Bestdiagram.BlockNum = diagram2.BlockNum;
		Bestdiagram.Turn = diagram2.Turn;
		Bestdiagram.X = diagram2.X;
		Bestdiagram.Y = diagram2.Y;
		Bestdiagram.Y = diagram2.Y;
		if (Aiscore > 55555000) {
			Aiscore -= 55555000;
		}
		return diagram2.X;
	}
}

void CheckboardAi2() {
	newscore = 0;
	//MessageBox(hWnd_, "Player1 패배", "게임종료 !", NULL);
	BlockHeight = 0; // 블록높이의 합
	BlankCount = 0; // 블록사이에 있는 빈공간의 개수
	CompliteLine = 0; // 완성된 줄의 개수
	MeetBlock = 0; // 현재 블록이 기존 블럭들과 닿는 면의 개수
	MeetWall = 0; // 현재 블록이 좌우 벽면과 닿는 면의 개수
	MeetFloor = 0; //현재 블록이 바닥면과 닿는 면의 개수

	while (MoveDownAi());
	// 블록높이의 합 !!!!!
	for (int i = 0; i < by; i++) {  // 줄이 완성 되었을 때	
		int total = 0;
		for (int j = 0; j < bx; j++) {
			total += GameBoard2[j][i];
		}
		if (total != 0 ) {
			BlockHeight += 1;
		}
	}
	
	// 블록사이에 있는 빈공간의 개수

	// 완성된 줄의 개수  !!!!!
	for (int i = 0; i < by; i++) {
		int total = 0;	
		for (int j = 0; j < bx; j++) {
			total += GameBoard2[j][i];
		}
		if (total == bx) {  // 줄이 완성 되었을 때	
			CompliteLine += 1;
		}
	}

	// 현재 블록이 기존 블럭들과 닿는 면의 개수
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			if (bvals[diagram2.BlockNum][diagram2.Turn][x][y] != 0) {
				if (GameBoard2[diagram2.X + x + 1][diagram2.Y + y] != 0) {
					MeetBlock = +1;
					break;
				}
			}

			if (bvals[diagram2.BlockNum][diagram2.Turn][x][y] != 0) {
				if (GameBoard2[diagram2.X + x - 1][diagram2.Y + y] != 0) {
					MeetBlock = +1;
					break;
				}
			}

			if (bvals[diagram2.BlockNum][diagram2.Turn][x][y] != 0) {
				if (GameBoard2[diagram2.X + x][diagram2.Y + 1] != 0) {
					MeetBlock = +1;
					break;
				}
			}
		}
	}

	// 현재 블록이 좌우 벽면과 닿는 면의 개수
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			if (bvals[diagram2.BlockNum][diagram2.Turn][x][y] != 0) {
				if ((diagram2.X + x) <= 25) {
					MeetWall += 1;
				}
				if ((diagram2.X + x) >= 25+bx-1) {
					MeetWall += 1;
				}
			}
		}
	}
	
	//현재 블록이 바닥면과 닿는 면의 개수
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			if (bvals[diagram2.BlockNum][diagram2.Turn][x][y] != 0) {
				if ((diagram2.X + y+1) <= by) {
					MeetFloor += 1;
				}
			}
		}
	}
	
	//newscore = (BlockHeight * (-3.78)) + (BlankCount * (-8.8)) + (CompliteLine * 8.2) + (MeetBlock * 3.7) + (MeetWall * 2.5) + (MeetFloor * (4.0));
	newscore = BlockHeight * (-3.78) + (CompliteLine * 8.2);
	Delete();
	if (Aiscore <= newscore) {
		Aiscore = newscore;
		Bestdiagram.BlockNum = diagram2.BlockNum;
		Bestdiagram.Turn = diagram2.Turn;
		Bestdiagram.X = diagram2.X;
		Bestdiagram.Y = diagram2.Y;
		Bestdiagram.Y = diagram2.Y;
	}
}
int GameValue[bx][by];

void CheckboardAi3() {
	newscore = 0;

	for (int i = 0; i < by; i++) {
		for (int j = 0; j < bx; j++) {
			if (GameBoard2[j][i] == 1) {
				GameValue[j][i]= (i + 1);
			}
		}
	}
	// 게임보드따라서 입력
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 12; j++) {
			if (GameBoard2[j][i] == 1) {
				if (GameBoard2[j - 1][i] == 0) {
					GameValue[j - 1][i] = -(i + 1);
					if (GameBoard2[j + 1][i] == 0) {
						GameValue[j + 1][i] = -(i + 1);
					}
				}
			}
		}
	}
	
	// 겉에 초기화
	for (int i = 0; i < 20; i++) {
		if (GameValue[0][i] == 0) {
			GameValue[0][i] = -(i + 1);
			GameValue[11][i] = -(i + 1);
		}
	}

	for (int i = 0; i < bx; i++) {
		for (int j = 0; j < by; j++) {
			Aiscore += GameValue[i][j];
		}
	}
	if (Aiscore <= newscore) {
		Aiscore = newscore;
		Bestdiagram.BlockNum = diagram2.BlockNum;
		Bestdiagram.Turn = diagram2.Turn;
		Bestdiagram.X = diagram2.X;
		Bestdiagram.Y = diagram2.Y;
		Bestdiagram.Y = diagram2.Y;
	}
}

void StartAi() {	
	BestMove = 0;
	Aiset = 1;
		for (int T = 0; T < 15; T++) { // 회전 15번 시켜보기
			for (int xx = 0; xx < 7; xx++) { // 오른쪽으로 7번 이동해보기
				diagram2.Turn = (T % 4);// 블록회전
				MoveRightAi(xx);
				CheckboardAi(); //블록의 상태를 확인
				diagram2.Y = 0;
				MoveLeftAi(-xx); // 블록위치 원래대로 되돌리기
			}
		}
	
	diagram2.BlockNum = Bestdiagram.BlockNum;
	diagram2.Turn = Bestdiagram.Turn;
	diagram2.X = Bestdiagram.X;
	diagram2.Y = Bestdiagram.Y;
	while(MoveDown2());
}

void DrawNextDiagram() { // 다음 블록 그리기
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			if (bvals[diagram.BlockNum][diagram.Turn][x][y] != 0) {
				BitBlt(hdc, (diagram.X + x) * bsize, (diagram.Y + y) * bsize,
					bsize, bsize, bhdc, 0, 0, SRCCOPY);
				
			}
		}
	}
}

void Destroyline(int y) {
	for (int i = 0; i < bx; i++) {
		GameBoard[i][y] = 0;
	}
	score1++;
	speed = speed / 1.02; // 블록이 부서졌을때 스피드 up
	hTimer = (HANDLE)SetTimer(hWnd_, 1, speed, NULL);
}

void Destroyline2(int y) {
	for (int i = 0; i < bx; i++) {
		GameBoard2[i][y] = 0;
	}
	score2++;
	speed = speed / 1.02;
	hTimer = (HANDLE)SetTimer(hWnd_, 1, speed, NULL);
}

void Shiftline(int y) {
	int Old;
	for (int i = y; i >= 0; i--) {
		for (int j = 0; j < bx; j++) {
			if (i == 0) {
				GameBoard[j][i] = 0;
			}
			else {
				GameBoard[j][i] = GameBoard[j][i - 1];
			}
		}
	}
}
void Shiftline2(int y) {
	int Old;
	for (int i = y; i >= 0; i--) {
		for (int j = 0; j < bx; j++) {
			if (i == 0) {
				GameBoard2[j][i] = 0;
			}
			else {
				GameBoard2[j][i] = GameBoard2[j][i - 1];
			}
		}
	}
}

int Checkline() {
	
	for (int i = 0; i < by; i++) {
		int total = 0;
		for (int j = 0; j < bx; j++) {
			total += GameBoard[j][i];
		}
		if ((i == 1) && (total > 0)) {
			//PostQuitMessage(0);
			KillTimer(hWnd_, 1);
			speed = 0;
			MessageBox(hWnd_, "Player1 패배", "게임종료 !", NULL);		
			return 1;
		}
		if (total == bx) {
			Destroyline(i);
			Shiftline(i);
			InvalidateRect(hWnd_, NULL, TRUE);
			return 1;
		}
		else {
			total = 0;
		}
	}
}

int Checkline2() {
	int total = 0;
	for (int i = 0; i < by; i++) {
		for (int j = 0; j < bx; j++) {
			total += GameBoard2[j][i];
		}
		if ((i == 1) && (total > 0)) {		
			KillTimer(hWnd_, 1);
			KillTimer(hWnd_, 2);
			MessageBox(hWnd_, "Player2 패배", "게임종료 !", NULL);
			speed = 0;
			speedAi = 0;
			return 1;
		}
		if (total == bx) {
			Destroyline2(i);
			Shiftline2(i);
			InvalidateRect(hWnd_, NULL, TRUE);
			return 1;
		}
		else {
			total = 0;
		}
	}
}
void Reset() { // 다음 나올 블록 초기화 하고 배열에서 블럭 정보 꺼내기
	srand(time(NULL));
	diagramA[(set+1)%5].X = 4;
	diagramA[(set + 1) % 5].Y = 0;
	diagramA[(set + 1) % 5].Turn = rand() % 4;
	diagramA[(set + 1) % 5].BlockNum = rand() % 7;
	
	diagram.X = diagramA[set % 5].X;
	diagram.Y = diagramA[set % 5].Y;
	diagram.Turn = diagramA[set % 5].Turn;
	diagram.BlockNum = diagramA[set % 5].BlockNum;
	set++;	
	InvalidateRect(hWnd_, NULL, TRUE);
}

void Reset2() {
	srand(time(NULL));
	if (Aiset == 1) {
		diagram2A[(set2 + 1) % 5].X = 25;
	}
	else {
		diagram2A[(set2 + 1) % 5].X = 25 + 4;
	}
	diagram2A[(set2 + 1) % 5].Y = 0;
	diagram2A[(set2 + 1) % 5].Turn = rand() % 4;
	diagram2A[(set2 + 1) % 5].BlockNum = rand() % 7;

	diagram2.X = diagram2A[set2 % 5].X;
	diagram2.Y = diagram2A[set2 % 5].Y;
	diagram2.Turn = diagram2A[set2 % 5].Turn;
	diagram2.BlockNum = diagram2A[set2 % 5].BlockNum;
	set2++;
	InvalidateRect(hWnd_, NULL, TRUE);	
}

void Drawbackscreen() // 게임보드 그리기
{
	HDC c;
	hdc = GetDC(hWnd_);
	Rectangle(hdc, 0, 0, 12 * bsize, 20 * bsize);
}

void Drawbackscreen2()
{
	HDC c;
	hdc = GetDC(hWnd_);
	Rectangle(hdc, 0, 0, 12 * bsize, 20 * bsize);
	Rectangle(hdc, b2, 0, 12 * bsize+b2, 20 * bsize);
}

void Drawnextdiagram(int mx, int my) {

	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) { // 현재 블록위치를 지우기
			if (bvals[diagram.BlockNum][diagram.Turn][x][y] != 0) {
				BitBlt(hdc, (diagram.X + x) * bsize, (diagram.Y + y) * bsize,
					bsize, bsize, bhdc2, 0, 0, SRCCOPY);
			}
		}
	}

	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) { // 다음 위치에 그리기
			if (bvals[diagram.BlockNum][diagram.Turn][x][y] != 0) {
				BitBlt(hdc, (diagram.X + mx + x) * bsize, (diagram.Y + my + y) * bsize,
					bsize, bsize, bhdc, 0, 0, SRCCOPY);
			}
		}
	}

}

void Drawnextdiagram2(int mx, int my) {
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			if (bvals[diagram2.BlockNum][diagram2.Turn][x][y] != 0) {
				BitBlt(hdc, (diagram2.X + x) * bsize, (diagram2.Y + y) * bsize,
					bsize, bsize, bhdc2, 0, 0, SRCCOPY);
			}
		}
	}

	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			if (bvals[diagram2.BlockNum][diagram2.Turn][x][y] != 0) {
				BitBlt(hdc, (diagram2.X + mx + x) * bsize, (diagram2.Y + my + y) * bsize,
					bsize, bsize, bhdc, 0, 0, SRCCOPY);
			}
		}
	}
}

void Drawnextdiagram_Turn() {
	for (int x = 0; x < 4; x++) {  // 현재 블록 위치 지우기
		for (int y = 0; y < 4; y++) {
			if (bvals[diagram.BlockNum][diagram.Turn][x][y] != 0) {
				BitBlt(hdc, (diagram.X + x) * bsize, (diagram.Y + y) * bsize,
					bsize, bsize, bhdc2, 0, 0, SRCCOPY);
			}
		}
	}
	for (int x = 0; x < 4; x++) { // 다음 블록 위치 그리기
		for (int y = 0; y < 4; y++) {
			if (bvals[diagram.BlockNum][(diagram.Turn + 1) % 4][x][y] != 0) {
				BitBlt(hdc, (diagram.X + x) * bsize, (diagram.Y + y) * bsize,
					bsize, bsize, bhdc, 0, 0, SRCCOPY);
			}
		}
	}
} 

void Drawnextdiagram_Turn2() {
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			if (bvals[diagram2.BlockNum][diagram2.Turn][x][y] != 0) {
				BitBlt(hdc, (diagram2.X + x) * bsize, (diagram2.Y + y) * bsize,
					bsize, bsize, bhdc2, 0, 0, SRCCOPY);
			}
		}
	}
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			if (bvals[diagram2.BlockNum][(diagram2.Turn + 1) % 4][x][y] != 0) {
				BitBlt(hdc, (diagram2.X + x) * bsize, (diagram2.Y + y) * bsize,
					bsize, bsize, bhdc, 0, 0, SRCCOPY);
			}
		}
	}
}

void Store() {
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			if (bvals[diagram.BlockNum][diagram.Turn][x][y] != 0) {
				GameBoard[diagram.X + x][diagram.Y + y] = 1; // 현재 블록위치 게임보드에 저장
			}
		}
	}
}

void Store2() {

	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			if (bvals[diagram2.BlockNum][diagram2.Turn][x][y] != 0) {
				GameBoard2[diagram2.X + x -25][diagram2.Y + y] = 1;
			}
		}
	}
}

int MoveEnable(int xx, int yy) {
	
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			if (bvals[diagram.BlockNum][diagram.Turn][x][y] != 0) {
				if (GameBoard[diagram.X + x + xx][diagram.Y + y + yy] != 0) { // 게임보드에 블록이 쌓여있는지 검사
					return 1;
				}
			}
		}
	}
	return 0;
}

int MoveEnable2(int xx, int yy) {

	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			if (bvals[diagram2.BlockNum][diagram2.Turn][x][y] != 0) {
				if (GameBoard2[diagram2.X-25 + x + xx][diagram2.Y + y + yy] != 0) {
					return 1;
				}
			}
		}
	}
	return 0;
}

int MoveEnable_Turn() {
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			if (bvals[diagram.BlockNum][(diagram.Turn + 1) % 4][x][y] != 0) {
				if (GameBoard[diagram.X + x][diagram.Y + y] != 0) {
					return 1;
				}
			}
		}
	}
	return 0;
}

int MoveEnable_Turn2() {
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			if (bvals[diagram2.BlockNum][(diagram2.Turn + 1) % 4][x][y] != 0) {
				if (GameBoard2[diagram2.X + x-25][diagram2.Y + y] != 0) {
					return 1;
				}
			}
		}
	}
	return 0;
}
int MoveLeft() {
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			if (bvals[diagram.BlockNum][diagram.Turn][x][y] != 0) { // 4x4 배열에 블록의 형태가
				if ((diagram.X + x) <= 0) { // 그려지는 블록의 x 좌표들 중 0보다 작거나 같으면 return 0
					return 0;
				}
			}
		}
	}

	if (!MoveEnable(-1, 0)) { // 블록의 x 축을 -1만큼 이동 했을 때 쌓여있는 블럭이 없는지 검사
		Drawnextdiagram(-1, 0); // 블록을 x 축을 -1만큼 이동한 블럭 그리기
		diagram.MoveLeft(); // diagram의 멤버변수 x -1 
		return 1;
	}
}

int MoveLeft2() {
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			if (bvals[diagram2.BlockNum][diagram2.Turn][x][y] != 0) { // 4x4 배열에 블록의 형태가
				if ((diagram2.X + x) <= 25) { // 그려지는 블록의 x 좌표들 중 0보다 작거나 같으면 return 0
					return 0;
				}
			}
		}
	}

	if (!MoveEnable2(-1, 0)) { // 블록의 x 축을 -1만큼 이동 했을 때 쌓여있는 블럭이 없는지 검사
		Drawnextdiagram2(-1, 0); // 블록을 x 축을 -1만큼 이동한 블럭 그리기
		diagram2.MoveLeft(); // diagram의 멤버변수 x -1 
		return 1;
	}
}

int MoveRight() {
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			if (bvals[diagram.BlockNum][diagram.Turn][x][y] != 0) {
				if ((diagram.X + x + 1) >= bx) {
					return 0;
				}
			}
		}
	}

	if (!MoveEnable(1, 0)) {
		Drawnextdiagram(1, 0);
		diagram.MoveRight();
		return 1;
	}
}

int MoveRight2() {
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			if (bvals[diagram2.BlockNum][diagram2.Turn][x][y] != 0) {
				if ((diagram2.X + x + 1) >= bx+25) {
					return 0;
				}
			}
		}
	}

	if (!MoveEnable2(1, 0)) {
		Drawnextdiagram2(1, 0);
		diagram2.MoveRight();
		return 1;
	}
}

int MoveDown() {
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			if (bvals[diagram.BlockNum][diagram.Turn][x][y] != 0) {
				if ((diagram.Y + y + 1) >= by) {
					Store();
					Reset();
					return 0;
				}
			}
		}
	}
	if (!MoveEnable(0, 1)) {
		Drawnextdiagram(0, 1);
		diagram.MoveDown();
		return 1;
	}
	else
	{
		Store();
		Reset();
		return 0;
	}
}

int MoveDown2() {
	 // 2번 블록
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			if (bvals[diagram2.BlockNum][diagram2.Turn][x][y] != 0) {
				if ((diagram2.Y + y + 1) >= by) {
					Store2();
					Reset2();			
					return 0;
				}
			}
		}
	}
	
	if (!MoveEnable2(0, 1)) {
		Drawnextdiagram2(0, 1);
		diagram2.MoveDown();
		return 1;
	}
	else
	{
		Store2();
		Reset2();
		return 0;
	}
}

int MoveTurn() {

	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			if (bvals[diagram.BlockNum][(diagram.Turn + 1) % 4][x][y] != 0) {
				if ((diagram.X + x) < 0 || (diagram.X + x) >= bx || (diagram.Y + y + 1) >= by) {
					return 0;
				}
			}
		}
	}
	if (!MoveEnable_Turn()) {
		Drawnextdiagram_Turn();
		diagram.MoveTurn();;
		return 1;
	}
}

int MoveTurn2() {

	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			if (bvals[diagram2.BlockNum][(diagram2.Turn + 1) % 4][x][y] != 0) {
				if ((diagram2.X + x) < 25 || (diagram2.X + x) >= bx+25 || (diagram2.Y + y + 1) >= by) {
					return 0;
				}
			}
		}
	}
	if (!MoveEnable_Turn2()) {
		Drawnextdiagram_Turn2();
		diagram2.MoveTurn();;
		return 1;
	}
}

void Drawgradation2() {
	for (int i = 0; i < bx; i++) {
		MoveToEx(hdc, i * bsize+b2, 0, nullptr);
		LineTo(hdc, i * bsize + b2, by * bsize);
	}
	for (int i = 0; i < by; i++) {
		MoveToEx(hdc, b2, i * bsize , nullptr);
		LineTo(hdc, bx * bsize + b2, i * bsize);
	}

	for (int i = 0; i < bx; i++) {
		MoveToEx(hdc, i * bsize, 0, nullptr);
		LineTo(hdc, i * bsize, by * bsize);
	}
	for (int i = 0; i < by; i++) {
		MoveToEx(hdc, 0, i * bsize, nullptr);
		LineTo(hdc, bx * bsize, i * bsize);
	}
}

void Drawgradation() {
	for (int i = 0; i < bx; i++) {
		MoveToEx(hdc, i * bsize, 0, nullptr);
		LineTo(hdc, i * bsize, by * bsize);
	}
	for (int i = 0; i < by; i++) {
		MoveToEx(hdc, 0, i * bsize, nullptr);
		LineTo(hdc, bx * bsize, i * bsize);
	}
}

void Drawdiagram2() {
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			if (bvals[diagram.BlockNum][diagram.Turn][x][y] != 0) {
				BitBlt(hdc, (diagram.X + x) * bsize, (diagram.Y + y) * bsize,
					bsize, bsize, bhdc, 0, 0, SRCCOPY);
			}
		}
	}
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			if (bvals[diagram2.BlockNum][diagram2.Turn][x][y] != 0) {
				BitBlt(hdc, (diagram2.X + x) * bsize, (diagram2.Y + y) * bsize,
					bsize, bsize, bhdc, 0, 0, SRCCOPY);
			}
		}
	}
}

void Drawdiagram() {
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			if (bvals[diagram.BlockNum][diagram.Turn][x][y] != 0) {
				BitBlt(hdc, (diagram.X + x) * bsize, (diagram.Y + y) * bsize,
					bsize, bsize, bhdc, 0, 0, SRCCOPY);
			}
		}
	}
}

void SetGameboard() {
	for (int i = 0; i < bx; i++) {
		for (int j = 0; j < by; j++) {
			GameBoard[i][j] = 0;
		}
	}
}

void SetGameboard2() {
	for (int i = 0; i < bx; i++) {
		for (int j = 0; j < by; j++) {
			GameBoard2[i][j] = 0;
		}
	}

	for (int i = 0; i < bx; i++) {
		for (int j = 0; j < by; j++) {
			GameValue[i][j] = 0;
		}
	}

	for (int i = 0; i < by; i++) {
		GameValue[0][i] = -(i+1);
		GameValue[bx-1][i] = -(i + 1);
	}

	diagram2.X += 25;
}

void DrawGameboard() {
	for (int i = 0; i < bx; i++) {
		for (int j = 0; j < by; j++) {
			if (GameBoard[i][j] == 1) {
				BitBlt(hdc, i * bsize, j * bsize,
					bsize, bsize, bhdc, 0, 0, SRCCOPY);
			}
		}
	}
}

void DrawGameboard2() {
	for (int i = 0; i < bx; i++) {
		for (int j = 0; j < by; j++) {
			if (GameBoard[i][j] == 1) {
				BitBlt(hdc, i * bsize, j * bsize,
					bsize, bsize, bhdc, 0, 0, SRCCOPY);
			}
		}
	}

	for (int i = 0; i < bx; i++) {
		for (int j = 0; j < by; j++) {
			if (GameBoard2[i][j] == 1) {
				BitBlt(hdc, i * bsize+b2, j * bsize,
					bsize, bsize, bhdc, 0, 0, SRCCOPY);
			}
		}
	}
}

#pragma endregion