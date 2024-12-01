#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include<cstdio>

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

#define IDC_BUTTON_BSP      1015  // Backspace  ->
#define IDC_BUTTON_CLR		1016 // Clear   AC
#define	IDC_BUTTON_EQUAL	1017 // '='


CONST CHAR g_sz_WINDOW_CLASS[] = "Calc_VPD_311";

CONST INT g_i_SCREEN_WIDTH = 400;
CONST INT g_i_SCREEN_HEIGHT = 22;


CONST INT g_i_BUTTON_SIZE = 50; // размер кнопки в пикселах
CONST INT g_i_INTERVAL = 5;		// расстояние между кнопками

CONST INT g_i_START_X = 10;
CONST INT g_i_START_Y = 10;
CONST INT g_i_BUTTON_START_X = g_i_START_X;
CONST INT g_i_BUTTON_START_Y = g_i_START_X + g_i_SCREEN_HEIGHT + g_i_INTERVAL;

HWND g_hButtons[18]; // Массив для кнопок 0-9 и other
HWND hEdit; //   edit control

INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	//1) Регистрация класса окна
	WNDCLASSEX wClass;
	ZeroMemory(&wClass, sizeof(wClass));  // Зануляем
	wClass.style = 0;
	wClass.cbSize = sizeof(wClass);
	wClass.cbClsExtra = 0;
	wClass.cbWndExtra = 0;

	wClass.hIcon = (HICON)LoadImage(NULL, "ICO\\CalcEx.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE,
		LR_LOADFROMFILE);
	wClass.hIconSm = (HICON)LoadImage(NULL, "ICO\\CalcExSm.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE,
		LR_LOADFROMFILE);
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
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		300 + 4 * (g_i_BUTTON_SIZE + g_i_INTERVAL),
		200 + 4 * (g_i_BUTTON_SIZE + g_i_INTERVAL), // добавляем место для кнопок
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
		hEdit = CreateWindowEx
		(
			NULL, "Edit", "0",
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT,
			10, 10,
			400, 22,
			hwnd,
			(HMENU)1000,
			GetModuleHandle(NULL),
			NULL
		);
		// создание кнопок
		CONST CHAR* bLabels[]=
		{		
			"7","8","9", "/",
			"4","5","6","*",
			"1","2","3","-",
			"0",".","=","+",
			"AC","->"
		};
		INT Buttons = 18;
		INT rows = 5;
		INT cols = 4;
		for (INT i = 0; i < 18; ++i)
		{
			INT row = i / cols; // номер строки
			INT col= i % cols; // номер столбца

			// расчёт позиции кнопки
			INT x = g_i_BUTTON_START_X + col * (g_i_BUTTON_SIZE + g_i_INTERVAL);
			INT y = g_i_BUTTON_START_Y + row * (g_i_BUTTON_SIZE + g_i_INTERVAL);

			// создание кнопок 
				g_hButtons[i] = CreateWindowEx
				(
					0, "Button", bLabels[i],
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, // | BS_OWNERDRAW,
					x, y,
					g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
					hwnd,
					(HMENU)(IDC_BUTTON_0 + i),
					GetModuleHandle(NULL),
					NULL
				);
		}
	}
		break;
	case WM_COMMAND:
		if (HIWORD(wParam) == BN_CLICKED)
		{
			INT b_ID = LOWORD(wParam);
			CHAR sz_buff[256];
			sprintf(sz_buff, "Нажата кнопка %d", b_ID);
			MessageBox(hwnd, sz_buff, "Info", MB_OK);
		}
		break;
	case WM_GETMINMAXINFO:
	{
		MINMAXINFO* nmi = (MINMAXINFO*)lParam;
		nmi->ptMinTrackSize.x = g_i_START_X * 3 + (g_i_BUTTON_SIZE + g_i_INTERVAL) * 4;
		nmi->ptMinTrackSize.y = 355;
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