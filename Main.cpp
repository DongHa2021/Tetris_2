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
#define bsize 20 // ���̾�׷� ũ��
#define bsize 20// ���̾�׷� ũ��
#define bx 12// ���Ӻ���ũ��x
#define by 20// ���Ӻ���ũ��y
#define sx 4 // ��� ������ġ x
#define sy 0 // ��� ������ġ y
#define b2 500 // gameboard2 ������ �ִ� �Ÿ�
Diagram diagram,diagram2; // �� ��ü
Diagram diagramA[5]; // ������ ����ִ� �迭
Diagram diagram2A[5];
HDC hdc, bhdc, bhdc2, bhdc3; // ��Ʈ�� ����� ���� ��
HBITMAP block, block2, block3; // ��Ʈ��
HINSTANCE hinstance_; // ���α׷� �ν��Ͻ�
HWND hWnd_; // ���� ������ �ڵ�
int GameBoard[bx][by]; // ���Ӻ��� 1
int GameBoard2[bx][by]; // ���Ӻ��� 2
int GameBoardAi[bx][by]; // ���Ӻ��� Ai
int speed = 500; // ���Ӽӵ�
int speedAi =1000; // Ai ���Ӽӵ�
int score1 = 0; // ���� 1
int score2 = 0; // ���� 2
int Aiscore = 0; // ���� Ai
// Ai ��������� ���� �ӽú���
int newscore = 0; 
int BestMove = 0;
int Aiset = 0;
int set = 0;
int set2 = 0;
int BlockHeight = 0; // ��ϳ����� ��
int BlankCount = 0; // ��ϻ��̿� �ִ� ������� ����
int CompliteLine = 0; // �ϼ��� ���� ����
int MeetBlock = 0; // ���� ����� ���� ����� ��� ���� ����
int MeetWall = 0; // ���� ����� �¿� ����� ��� ���� ����
int MeetFloor = 0; //���� ����� �ٴڸ�� ��� ���� ����
// ��ź������ ����
int bomb1 = 1;
int bombs1 = 2;
int bomb2 = 1;
int bombs2 = 2;

// 7�� ��Ͽ� ���� ����
int bvals[7][4][4][4] =
{
	{
		{
			{0,0,1,0}, // l ���
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
			{0,0,0,0}, // �� ���
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
			{0,0,1,0}, // �� ���
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
			{0,0,0,1}, //�� ���
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
			{0,0,1,0}, // ���� ���
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
			{0,0,0,1}, // �Ť� ���
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
			{0,0,0,1}, // �Ť� ���
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
			{0,0,0,0}, // �� ���
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
// Ai ��������� ���� �ӽð�ü
Diagram Bestdiagram;
Diagram Currentdiagram;
HANDLE hTimer = (HANDLE)SetTimer(hWnd_, 1, speed, NULL);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProc2(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProc3(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProc4(HWND, UINT, WPARAM, LPARAM);

void SetGameboard(); // ���Ӻ��� �ʱ�ȭ
void SetGameboard2();
void DrawGameboard(); // ���Ӻ��� �׸���
void DrawGameboard2();
void Drawdiagram(); // ���� ���̾�׷� �׸���
void Drawdiagram2();
void Drawgradation(); // ����, ���� �׸���
void Drawgradation2();
int MoveTurn(); // ���� ȸ��
int MoveTurn2();
int MoveDown(); // ���� �ϰ�
int MoveDown2();
int MoveRight(); // ���� ������ �̵�
int MoveRight2();
int MoveLeft(); // ���� ���� �̵�
int MoveLeft2();
int MoveEnable_Turn(); // ���� ȸ�� �������� �Ǵ�
int MoveEnable_Turn2();
int MoveEnable(int xx, int yy); // ���� �̵� �������� �Ǵ�
int MoveEnable2(int xx, int yy);
void Store(); // ���� ���Ӻ��忡 ����
void Store2();
void Drawnextdiagram_Turn(); // ȸ���� ���� ���� �׸���
void Drawnextdiagram_Turn2();
void Drawnextdiagram(int mx, int my); // �̵��� ���� ���� �׸���
void Drawnextdiagram2(int mx, int my);
void Drawbackscreen(); // ��� �׸���
void Drawbackscreen2();
void Reset(); // ���� ���� ������ġ�� �ʱ�ȭ �� ������ ���� ���� �ʱ�ȭ
void Reset2();
int Checkline(); // ���Ӻ��忡 ������� ������ �ִ��� �˻�
int Checkline2();
void Shiftline(int y); //y��ǥ ���� ���� �Ʒ��� ��ĭ ����߸���
void Shiftline2(int y);
void Destroyline(int y); //y��ǥ ���� �ı�
void Destroyline2(int y);

void StartAi(); // ������ ��ġ�� ã�Ƽ� ���� �ϰ�
int CheckboardAi(); // ������ ��ġ ���� ����� ���� �Լ�
void Delete(); // ���� ������ġ ���Ӻ��忡�� ����
//Vs Com�� �����ϱ� ���� �Լ���
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

	WndClass.style = NULL; // ���� ������ Ŭ����
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = "Main";

	WndClass2.style = NULL; // 1�ο� ���� ������ Ŭ����
	WndClass2.lpfnWndProc = WndProc2;
	WndClass2.cbClsExtra = 0;
	WndClass2.cbWndExtra = 0;
	WndClass2.hInstance = hInstance;
	WndClass2.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass2.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass2.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass2.lpszMenuName = NULL;
	WndClass2.lpszClassName = "WND2";

	WndClass3.style = NULL;// 2�ο� ���� ������ Ŭ����
	WndClass3.lpfnWndProc = WndProc3;
	WndClass3.cbClsExtra = 0;
	WndClass3.cbWndExtra = 0;
	WndClass3.hInstance = hInstance;
	WndClass3.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass3.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass3.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass3.lpszMenuName = NULL;
	WndClass3.lpszClassName = "WND3";

	WndClass4.style = NULL; // VsCom ���� ������ Ŭ����
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
// ���� ������ �̺�Ʈ ó�� 
LRESULT CALLBACK WndProc(HWND hWnd, UINT mesg, WPARAM wParam, LPARAM lParam) 
{
	static HWND hWnd2,hWnd3;
	static HWND hBtnl1, hBtnl2, hBtnl3,hCheatE,hCheatB, hCheatL;
	static HANDLE hThread;
	DWORD dwThreadID;
	PAINTSTRUCT ps;
	switch (mesg)
	{

	case WM_CREATE: // �����غ�
		//��Ʈ�� �ʱ�ȭ
		hdc = GetDC(hWnd);
		bhdc = CreateCompatibleDC(hdc);
		bhdc2 = CreateCompatibleDC(hdc);
		block = LoadBitmap(hinstance_, MAKEINTRESOURCE(IDB_BLOCK));
		SelectObject(bhdc, block);
		block2 = LoadBitmap(hinstance_, MAKEINTRESOURCE(IDB_BLANK));
		SelectObject(bhdc2, block2);
		// ���Ӻ��� �ʱ�ȭ
		SetGameboard();
		Reset();

		BitBlt(hdc, 0, 0, 30, 30, bhdc, 0, 0, SRCCOPY);

		hBtnl1 = CreateWindow("BUTTON", "1�ο�", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			30, 350, 100, 100, hWnd, (HMENU)111, hinstance_, NULL); // ��ư �߰�	

		hBtnl2 = CreateWindow("BUTTON", "2�ο�", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			180, 350, 100, 100, hWnd, (HMENU)222, hinstance_, NULL); // ��ư �߰�

		hBtnl3 = CreateWindow("BUTTON", "VS COM", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			330, 350, 100, 100, hWnd, (HMENU)333, hinstance_, NULL); // ��ư �߰�

		hCheatE = CreateWindow("EDIT", "ġƮ�� �Է�", WS_VISIBLE | WS_CHILD | WS_BORDER
			| ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE, 30, 290, 300, 30, hWnd,
			(HMENU)444, hinstance_, NULL); // �ؽ�Ʈ �Է¶� �߰�
		hCheatB = CreateWindow("BUTTON", "�Է�", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			340, 290, 50, 30, hWnd, (HMENU)555, hinstance_, NULL); // ��ư �߰�
		hCheatL = CreateWindow("BUTTON", "ġƮŰ ���", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			400, 290, 100, 30, hWnd, (HMENU)666, hinstance_, NULL); // ��ư �߰�


		break;
	case WM_PAINT: 
		hdc = BeginPaint(hWnd, &ps);
		// ����Ű ����
		TextOut(hdc, 50, 30, "!!! ����Ű !!!", sizeof("!!! ����Ű !!"));
		TextOut(hdc,50,90,"1P ",sizeof("1"));
		TextOut(hdc, 50, 110, "ȸ�� : W", sizeof("ȸ�� : "));
		TextOut(hdc, 50, 130, "�·��̵� : A", sizeof("�·��̵� : "));
		TextOut(hdc, 50, 150, "����̵� : D", sizeof("����̵� : "));
		TextOut(hdc, 50, 170, "�Ʒ��� �̵� : S", sizeof("�Ʒ��� �̵� : "));
		TextOut(hdc, 50, 190, "���� : SPACE", sizeof("���� : SPAC"));

		TextOut(hdc, 250, 90, "2P ", sizeof("1"));
		TextOut(hdc, 250, 110, "ȸ�� : UP", sizeof("ȸ�� :  "));
		TextOut(hdc, 250, 130, "�·��̵� : LEFT", sizeof("�·��̵� :    "));
		TextOut(hdc, 250, 150, "����̵� : RIGHT", sizeof("����̵� :     "));
		TextOut(hdc, 250, 170, "�Ʒ��� �̵� : DOWN", sizeof("�Ʒ��� �̵� :    "));
		TextOut(hdc, 250, 190, "���� : L", sizeof("����   "));

		break;
	case WM_COMMAND:
		int id;
		id = LOWORD(wParam); // id ���
		if (id == 111) { // 1�ο� ��� ����
			hWnd2 = CreateWindow("WND2", "1�ο� ���", WS_OVERLAPPEDWINDOW| WS_VISIBLE,
				600,
				300,
				30*bsize, 30 * bsize, hWnd, NULL,hinstance_, NULL);
		}
		else if (id == 222) {	// 2�ο� ��� ����
			hWnd3 = CreateWindow("WND3", "2�ο� ���", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
				600,
				300,
				1000, 30 * bsize, hWnd, NULL, hinstance_, NULL);
		}
		else if (id == 333) { //Vscom ��� ����
			hWnd3 = CreateWindow("WND4", "Vs Com ���", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
				600,
				300,
				1000, 30 * bsize, hWnd, NULL, hinstance_, NULL);

		}
		else if (id == 555) {
			// ġƮŰ ��
			char sz[1024];
			GetWindowText(hCheatE, sz, 30);
			if (!strcmp(sz, "P1 Bomb")) {
				MessageBox(hWnd_, "Player1 ū��ź + 1", "ġƮŰ����Ϸ� !", NULL);
				bomb1 += 1;
			}else if (!strcmp(sz, "P2 Bomb")) {
				MessageBox(hWnd_, "Player2 ū��ź + 1", "ġƮŰ����Ϸ� !", NULL);
				bomb2 += 1;
			}
			else if (!strcmp(sz, "P1 sBomb")) {
				MessageBox(hWnd_, "Player2 ������ź + 1", "ġƮŰ����Ϸ� !", NULL);
				bombs1 += 1;
			}
			else if (!strcmp(sz, "P2 sBomb")) {
				MessageBox(hWnd_, "Player2 ������ź + 1", "ġƮŰ����Ϸ� !", NULL);
				bombs2 += 1;
			}
			else if (!strcmp(sz, "Speed down")) {
				MessageBox(hWnd_, "�ӵ� ���� + 0.1", "ġƮŰ����Ϸ� !", NULL);
				speed += 100;
			}
			else if (!strcmp(sz, "Speed up")) {
				MessageBox(hWnd_, "�ӵ� ���� - 0.1", "ġƮŰ����Ϸ� !", NULL);
				speed -= 100;
			}
			else {
				MessageBox(hWnd_, "�ٽ� �Է��� �ּ���.", "ġƮŰ�������...", NULL);
			}
		}
		else if (id == 666) {
			ShellExecute(NULL, ("open"), ("ġƮŰ.txt"), NULL, NULL, SW_SHOW);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return FALSE;
	}
	return DefWindowProc(hWnd, mesg, wParam, lParam);
}

// 1�ο� ���� ������ �̺�Ʈ ó��
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
		MessageBox(hWnd_, " 1�ο� ������ �����մϴ�\n ������ �ٷ� ���۵˴ϴ�", "!! �˸� !!", NULL);
		// ��Ʈ�� �ʱ�ȭ
		hdc = GetDC(hWnd);
		bhdc = CreateCompatibleDC(hdc);
		bhdc2 = CreateCompatibleDC(hdc);
		block = LoadBitmap(hinstance_, MAKEINTRESOURCE(IDB_BLOCK));
		SelectObject(bhdc, block);
		block2 = LoadBitmap(hinstance_, MAKEINTRESOURCE(IDB_BLANK));
		SelectObject(bhdc2, block2);
		//�ð���ü �ʱ�ȭ
		hTimer = (HANDLE)SetTimer(hWnd, 1, speed, NULL);
		SetGameboard();
		break;

	case WM_TIMER:
		switch (wParam) {
		case 1: // �ʸ��� ���׸���, ��� �ϰ�, ����üũ
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
		sprintf_s(sz, "���� ������ : %d �� ", score1);
		TextOut(hdc, 300, 50, sz, strlen(sz));
		sprintf_s(sz, "���� 0.%d�� ���� �ϰ� ", speed);
		TextOut(hdc, 300, 70, sz, strlen(sz));
		sprintf_s(sz, "Q ���� ū��ź ���� : %d�� ", bomb1);
		TextOut(hdc, 300, 110, sz, strlen(sz));
		sprintf_s(sz, "E ���� ������ź ���� : %d�� ", bombs1);
		TextOut(hdc, 300, 130, sz, strlen(sz));


		sprintf_s(sz, "----- �������� ��� ----- ");
		TextOut(hdc, 280, 270, sz, strlen(sz));
		// ���� ���� ���� ���
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
		
	case WM_KEYDOWN: // Ű���� �̺�Ʈ�� ���� �̺�Ʈ ó��
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

	case WM_DESTROY: // �����찡 �ı��Ǹ� �ʿ��� ������ ����
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
// 2�ο� ���� ������ �̺�Ʈ ó��
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
		MessageBox(hWnd_, " 2�ο� ������ �����մϴ�\n ������ �ٷ� ���۵˴ϴ�", "!! �˸� !!", NULL);
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
		sprintf_s(sz, "���� ������ : %d �� ", score1);
		hdc = GetDC(hWnd);
		TextOut(hdc, 300, 50, sz, strlen(sz));
		sprintf_s(sz, "���� 0.%d�� ���� �ϰ� ", speed);
		TextOut(hdc, 300, 70, sz, strlen(sz));
		sprintf_s(sz, "Q ���� ū ��ź ���� : %d�� ", bomb1);
		TextOut(hdc, 300, 110, sz, strlen(sz));
		sprintf_s(sz, "E ���� ������ź ���� : %d�� ", bombs1);
		TextOut(hdc, 300, 130, sz, strlen(sz));
		sprintf_s(sz, "----- �������� ��� ----- ");
		TextOut(hdc, 280, 270, sz, strlen(sz));
		// ���� ��� ���
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

		sprintf_s(sz, "���� ������ : %d �� ", score2);
		hdc = GetDC(hWnd);
		TextOut(hdc, 800, 50, sz, strlen(sz));
		sprintf_s(sz, "���� 0.%d�� ���� �ϰ� ", speed);
		TextOut(hdc, 800, 70, sz, strlen(sz));
		sprintf_s(sz, "K ���� ū ��ź ���� : %d�� ", bomb2);
		TextOut(hdc, 800, 110, sz, strlen(sz));
		sprintf_s(sz, "J ���� ������ź ���� : %d�� ", bombs2);
		TextOut(hdc, 800, 130, sz, strlen(sz));
		sprintf_s(sz, "----- �������� ��� ----- ");
		TextOut(hdc, 780, 270, sz, strlen(sz));
		// ���� ��� ���
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
		// Ű���� �̺�Ʈ�� ���� �̺�Ʈ ó��
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
		// �����찡 �ı��Ǹ� �ʿ��� ���� �ٽ� ����
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
// VS com ���� ������ �̺�Ʈ ó��
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
		case 2:	 //�ʸ��� ������ ��ġ�� ��� �ϰ�
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
		sprintf_s(sz, "���� ������ : %d �� ", score1);
		TextOut(hdc, 300, 50, sz, strlen(sz));
		sprintf_s(sz, "���� 0.%d�� ���� �ϰ� ", speed);
		TextOut(hdc, 300, 70, sz, strlen(sz));
		sprintf_s(sz, "���� 0.%d�� ���� �ϰ� ", speed);
		TextOut(hdc, 300, 70, sz, strlen(sz));
		sprintf_s(sz, "Q ���� ū ��ź ���� : %d�� ", bomb1);
		TextOut(hdc, 300, 110, sz, strlen(sz));
		sprintf_s(sz, "E ���� ������ź ���� : %d�� ", bombs1);
		TextOut(hdc, 300, 130, sz, strlen(sz));
		sprintf_s(sz, "----- �������� ��� ----- ");
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
		sprintf_s(sz, "�������� : %d �� ", score2);
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
		TextOut(hdc, bsize*(bx+25), by*bsize, "����!", strlen("����!"));
		TextOut(hdc, b2, by * bsize, "����!", strlen("����!"));
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

#pragma region Tetris // ��Ʈ���� �Լ�


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
	for (int i = 0; i < xx; i++) { // xx ��ŭ �̵�
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
				if (bvals[diagram2.BlockNum][diagram2.Turn][x][y] != 0) { // 4x4 �迭�� ����� ���°�
					if ((diagram2.X + x) <= 25) { // �׷����� ����� x ��ǥ�� �� 0���� �۰ų� ������ return 0
						return 0;
					}
				}
			}
		}
	for (int i = 0; i < xx; i++) {
		if (!MoveEnable2(-1, 0)) { // ����� x ���� -1��ŭ �̵� ���� �� �׿��ִ� ���� ������ �˻�
			Drawnextdiagram2(-1, 0); // ����� x ���� -1��ŭ �̵��� �� �׸���
			diagram2.MoveLeft(); // diagram�� ������� x -1 
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
	if (!MoveEnable_Turn2()) { // ��� ȸ��
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
					//MessageBox(hWnd_, "Player1 �й�", "�������� !", NULL);
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
	// ���� ������
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
	// ���� �ϼ� �Ǿ��� ��	
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

	// ���Ӻ��� ���� ���
	for (int i = 0; i < by; i++) {   
		for (int j = 0; j < bx; j++) {
			if (GameBoard2[j][i] == 1) {
				newscore += line;
			}
		}
		line +=120;
	}
	line = 0;

	// ���Ӻ��� ���� ���
	for (int i = BlockHeight; i >= 0; i--) {
		for (int j = 0; j < bx; j++) {
			if (GameBoard2[j][i] == 1) {
				newscore += line;
			}
		}
		line -=100;
	}
	// ���� ���� ��
	for (int i = 0; i < by; i++) {
		for (int j = 0; j < bx; j++) {
			if (GameBoard2[0][i] == 1 || GameBoard2[bx-1][i] ==1) {
				newscore +=100;
			}		
		}
	}

	// ���� ����� �¿� ����� ��� ���� ����
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
	//�ٴڿ� ���� ��
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

	// ��ϳ����� �� !!!!!
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
	//MessageBox(hWnd_, "Player1 �й�", "�������� !", NULL);
	BlockHeight = 0; // ��ϳ����� ��
	BlankCount = 0; // ��ϻ��̿� �ִ� ������� ����
	CompliteLine = 0; // �ϼ��� ���� ����
	MeetBlock = 0; // ���� ����� ���� ����� ��� ���� ����
	MeetWall = 0; // ���� ����� �¿� ����� ��� ���� ����
	MeetFloor = 0; //���� ����� �ٴڸ�� ��� ���� ����

	while (MoveDownAi());
	// ��ϳ����� �� !!!!!
	for (int i = 0; i < by; i++) {  // ���� �ϼ� �Ǿ��� ��	
		int total = 0;
		for (int j = 0; j < bx; j++) {
			total += GameBoard2[j][i];
		}
		if (total != 0 ) {
			BlockHeight += 1;
		}
	}
	
	// ��ϻ��̿� �ִ� ������� ����

	// �ϼ��� ���� ����  !!!!!
	for (int i = 0; i < by; i++) {
		int total = 0;	
		for (int j = 0; j < bx; j++) {
			total += GameBoard2[j][i];
		}
		if (total == bx) {  // ���� �ϼ� �Ǿ��� ��	
			CompliteLine += 1;
		}
	}

	// ���� ����� ���� ����� ��� ���� ����
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

	// ���� ����� �¿� ����� ��� ���� ����
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
	
	//���� ����� �ٴڸ�� ��� ���� ����
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
	// ���Ӻ������ �Է�
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
	
	// �ѿ� �ʱ�ȭ
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
		for (int T = 0; T < 15; T++) { // ȸ�� 15�� ���Ѻ���
			for (int xx = 0; xx < 7; xx++) { // ���������� 7�� �̵��غ���
				diagram2.Turn = (T % 4);// ���ȸ��
				MoveRightAi(xx);
				CheckboardAi(); //����� ���¸� Ȯ��
				diagram2.Y = 0;
				MoveLeftAi(-xx); // �����ġ ������� �ǵ�����
			}
		}
	
	diagram2.BlockNum = Bestdiagram.BlockNum;
	diagram2.Turn = Bestdiagram.Turn;
	diagram2.X = Bestdiagram.X;
	diagram2.Y = Bestdiagram.Y;
	while(MoveDown2());
}

void DrawNextDiagram() { // ���� ��� �׸���
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
	speed = speed / 1.02; // ����� �μ������� ���ǵ� up
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
			MessageBox(hWnd_, "Player1 �й�", "�������� !", NULL);		
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
			MessageBox(hWnd_, "Player2 �й�", "�������� !", NULL);
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
void Reset() { // ���� ���� ��� �ʱ�ȭ �ϰ� �迭���� �� ���� ������
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

void Drawbackscreen() // ���Ӻ��� �׸���
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
		for (int y = 0; y < 4; y++) { // ���� �����ġ�� �����
			if (bvals[diagram.BlockNum][diagram.Turn][x][y] != 0) {
				BitBlt(hdc, (diagram.X + x) * bsize, (diagram.Y + y) * bsize,
					bsize, bsize, bhdc2, 0, 0, SRCCOPY);
			}
		}
	}

	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) { // ���� ��ġ�� �׸���
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
	for (int x = 0; x < 4; x++) {  // ���� ��� ��ġ �����
		for (int y = 0; y < 4; y++) {
			if (bvals[diagram.BlockNum][diagram.Turn][x][y] != 0) {
				BitBlt(hdc, (diagram.X + x) * bsize, (diagram.Y + y) * bsize,
					bsize, bsize, bhdc2, 0, 0, SRCCOPY);
			}
		}
	}
	for (int x = 0; x < 4; x++) { // ���� ��� ��ġ �׸���
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
				GameBoard[diagram.X + x][diagram.Y + y] = 1; // ���� �����ġ ���Ӻ��忡 ����
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
				if (GameBoard[diagram.X + x + xx][diagram.Y + y + yy] != 0) { // ���Ӻ��忡 ����� �׿��ִ��� �˻�
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
			if (bvals[diagram.BlockNum][diagram.Turn][x][y] != 0) { // 4x4 �迭�� ����� ���°�
				if ((diagram.X + x) <= 0) { // �׷����� ����� x ��ǥ�� �� 0���� �۰ų� ������ return 0
					return 0;
				}
			}
		}
	}

	if (!MoveEnable(-1, 0)) { // ����� x ���� -1��ŭ �̵� ���� �� �׿��ִ� ���� ������ �˻�
		Drawnextdiagram(-1, 0); // ����� x ���� -1��ŭ �̵��� �� �׸���
		diagram.MoveLeft(); // diagram�� ������� x -1 
		return 1;
	}
}

int MoveLeft2() {
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			if (bvals[diagram2.BlockNum][diagram2.Turn][x][y] != 0) { // 4x4 �迭�� ����� ���°�
				if ((diagram2.X + x) <= 25) { // �׷����� ����� x ��ǥ�� �� 0���� �۰ų� ������ return 0
					return 0;
				}
			}
		}
	}

	if (!MoveEnable2(-1, 0)) { // ����� x ���� -1��ŭ �̵� ���� �� �׿��ִ� ���� ������ �˻�
		Drawnextdiagram2(-1, 0); // ����� x ���� -1��ŭ �̵��� �� �׸���
		diagram2.MoveLeft(); // diagram�� ������� x -1 
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
	 // 2�� ���
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