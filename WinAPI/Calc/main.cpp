#define _CRT_SECURE_NO_WARNINGS  // добавляем при ошибке С4996
#include<Windows.h>
#include"Resource.h"
//#include"Demansions.h"

#define IDC_EDIT_DISPLAY 999
#define IDC_BUTTON_0	1000
#define IDC_BUTTON_1	1001
#define IDC_BUTTON_2	1002
#define IDC_BUTTON_3	1003
#define IDC_BUTTON_4	1004
#define IDC_BUTTON_5	1005
#define IDC_BUTTON_6	1006
#define IDC_BUTTON_7	1007
#define IDC_BUTTON_8	1008
#define IDC_BUTTON_9	1009
#define IDC_BUTTON_POINT	1010

#define IDC_BUTTON_PLUS		1011
#define IDC_BUTTON_MINUS	1012
#define IDC_BUTTON_ASTER	1013  // *
#define IDC_BUTTON_SLASH	1014  // /

#define IDC_BUTTON_BSP      1015  // Backspace
#define IDC_BUTTON_CLR		1016 // Clear
#define	IDC_BUTTON_EQUAL	1017 // '='


CONST CHAR g_sz_WINDOW_CLASS[] = "Calc_VPD_311";



CONST INT g_i_BUTTON_SIZE = 50; // размер кнопки в пикселах
CONST INT g_i_INTERVAL = 5;		// расстояние между кнопками
CONST INT g_i_BUTTON_DOUBLE_SIZE = g_i_BUTTON_SIZE*2+ g_i_INTERVAL; // размер кнопки в пикселах

CONST INT g_i_SCREEN_WIDTH = g_i_BUTTON_SIZE*5 + g_i_INTERVAL*4;
CONST INT g_i_SCREEN_HEIGHT = 22;

CONST INT g_i_START_X = 10;
CONST INT g_i_START_Y = 10;
CONST INT g_i_BUTTON_START_X = g_i_START_X;
CONST INT g_i_BUTTON_START_Y = g_i_START_X + g_i_SCREEN_HEIGHT + g_i_INTERVAL;
#define BUTTON_SHIFT_X(column) g_i_BUTTON_START_X + (g_i_BUTTON_SIZE + g_i_INTERVAL)*(column)
#define BUTTON_SHIFT_Y(row) g_i_BUTTON_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL)*(row)

CONST INT g_i_WINDOW_WIDTH = g_i_SCREEN_WIDTH + g_i_START_X * 2;
CONST INT g_i_WINDOW_HEIGHT = g_i_START_X + g_i_SCREEN_HEIGHT + g_i_BUTTON_SIZE*4 + g_i_INTERVAL*5;

CONST CHAR* g_OPERATION[] = { "+" ,"-", "*","/" };

INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//INT GetTitleBarHeight(HWND hwnd);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	//1) Регистрация класса окна
	WNDCLASSEX wClass;
	ZeroMemory(&wClass, sizeof(wClass));  // Зануляем
	wClass.style = 0;
	wClass.cbSize = sizeof(wClass);
	wClass.cbClsExtra = 0;
	wClass.cbWndExtra = 0;

	wClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wClass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wClass.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_ARROW));
	wClass.hbrBackground = (HBRUSH)COLOR_WINDOW;

	wClass.hInstance = hInstance;
	wClass.lpszClassName = g_sz_WINDOW_CLASS;
	wClass.lpszMenuName = NULL;
	wClass.lpfnWndProc = (WNDPROC)WndProc;

	if (!RegisterClassEx(&wClass))
	{
		MessageBox(NULL, "Class registration failed", NULL, MB_OK | MB_ICONERROR);
		return 0;
	}
	//2) Создание окна

	HWND hwnd = CreateWindowEx
	(
		NULL,
		g_sz_WINDOW_CLASS,
		g_sz_WINDOW_CLASS,
		WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT,
		g_i_WINDOW_WIDTH+16, g_i_WINDOW_HEIGHT +42,
		NULL,
		NULL,
		hInstance,
		NULL

	);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	//3) Запуск цикла сообщение:

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}
INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	switch (uMsg)
	{
	case WM_CREATE:
	{
		HWND hEdit = CreateWindowEx
		(
			NULL, "Edit", "0",
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT,
			10, 10,
			g_i_SCREEN_WIDTH, g_i_SCREEN_HEIGHT,
			hwnd,
			(HMENU)999,
			GetModuleHandle(NULL),
			NULL
		);
		CHAR sz_digit[2] = {};
		for (int i = 6; i >= 0; i -= 3)
		{
			for (int j = 0; j < 3; j++)
			{
				sz_digit[0] = i + j+'1';
				CreateWindowEx
				(
					NULL, "Button",sz_digit,
					WS_CHILD | WS_VISIBLE,
					g_i_BUTTON_START_X + (g_i_BUTTON_SIZE + g_i_INTERVAL) * j,
					g_i_BUTTON_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL) * (2 - i/3),
					g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
					hwnd,
					(HMENU)(IDC_BUTTON_1 + (i+j)),
					GetModuleHandle(NULL),
					NULL
				);
			}
		}
		CreateWindowEx
		(
			NULL, "Button", "0",
			WS_CHILD | WS_VISIBLE,
			BUTTON_SHIFT_X(0), BUTTON_SHIFT_Y(3),
			/*g_i_BUTTON_START_X, g_i_BUTTON_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL) * 3,*/
			g_i_BUTTON_DOUBLE_SIZE, g_i_BUTTON_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_0,
			GetModuleHandle(NULL),
			NULL
		);
		CreateWindowEx
		(
			NULL, "Button", ".",
			WS_CHILD | WS_VISIBLE,
			BUTTON_SHIFT_X(2), BUTTON_SHIFT_Y(3),
			/*g_i_BUTTON_START_X + (g_i_BUTTON_SIZE + g_i_INTERVAL)*2,
			g_i_BUTTON_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL)*3,*/
			g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_0,
			GetModuleHandle(NULL),
			NULL
		);
		for (int i = 0; i < 4; i++)
		{
			CreateWindowEx
			(
				NULL, "Button", g_OPERATION[i],
				WS_CHILD | WS_VISIBLE,
				BUTTON_SHIFT_X(3), BUTTON_SHIFT_Y(3-i),
				/*g_i_BUTTON_START_X + (g_i_BUTTON_SIZE + g_i_INTERVAL) * 3,
				g_i_BUTTON_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL) * (3 - i),*/
				g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
				hwnd,
				(HMENU)IDC_BUTTON_PLUS + i,
				GetModuleHandle(NULL),
				NULL
			);
			CreateWindowEx
			(
				NULL, "Button", "<-",
				WS_CHILD | WS_VISIBLE,
				BUTTON_SHIFT_X(4), BUTTON_SHIFT_Y(0),
				g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
				hwnd,
				(HMENU)IDC_BUTTON_BSP,
				GetModuleHandle(NULL),
				NULL
			);
			CreateWindowEx
			(
				NULL, "Button", "C",
				WS_CHILD | WS_VISIBLE,
				BUTTON_SHIFT_X(4), BUTTON_SHIFT_Y(1),
				g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
				hwnd,
				(HMENU)IDC_BUTTON_CLR,
				GetModuleHandle(NULL),
				NULL
			);
			CreateWindowEx
			(
				NULL, "Button", "=",
				WS_CHILD | WS_VISIBLE,
				BUTTON_SHIFT_X(4), BUTTON_SHIFT_Y(2),
				g_i_BUTTON_SIZE, g_i_BUTTON_DOUBLE_SIZE,
				hwnd,
				(HMENU)IDC_BUTTON_EQUAL,
				GetModuleHandle(NULL),
				NULL
			);
		
		}

	}
	break;
	case WM_COMMAND:
	{
		HWND hEditDisplay = GetDlgItem(hwnd, IDC_EDIT_DISPLAY);
		CONST INT SIZE = 256;
		CHAR sz_display[MAX_PATH]{};
		CHAR sz_digit[2]{};
		if (LOWORD(wParam) >= IDC_BUTTON_0 && LOWORD(wParam) <= IDC_BUTTON_9)
		{
			sz_digit[0] = LOWORD(wParam) - IDC_BUTTON_0 + '0';
			SendMessage(hEditDisplay, WM_GETTEXT, SIZE, (LPARAM)sz_display);
			if (strlen(sz_display) == 1 && sz_display[0] == '0')
				sz_display[0] = sz_digit[0];
			else
			strcat(sz_display, sz_digit);
			SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_display);
		}
		if (LOWORD(wParam) == IDC_BUTTON_POINT)
		{
			SendMessage(hEditDisplay, WM_GETTEXT, SIZE, (LPARAM)sz_display);//cчитываем содержимое экрана
			if (strchr(sz_display, '.'))break; // чтобы точку можно было поставить только одну
			strcat(sz_display, ".");
			SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_display);
		}
	}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	default: return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return FALSE;
}
