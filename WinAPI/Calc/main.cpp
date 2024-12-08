#define _CRT_SECURE_NO_WARNINGS  // добавляем при ошибке С4996
#include<Windows.h>
#include"Resource.h"
#include"Dimensions.h"
#include<float.h>
#include<cstdio>

CONST CHAR g_sz_WINDOW_CLASS[] = "Calc_VPD_311";

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
		for (int i = 6; i >= 0; i -= 3)  //отвечает за ряды кнопок сверху вниз ,  i равнo : 6, 3, 0. Это три ряда(3 строки).
		{
			for (int j = 0; j < 3; j++) //отвечает за кнопки в одном ряду слева направо,j равнo : 0, 1, 2. Это три кнопки в строке
			{
				sz_digit[0] = i + j+'1'; // создаёт текст кнопки
				/*	i + j — порядковый номер кнопки(0, 1, 2 и так далее)
					'1' — добавление, чтобы сдвинуть номера в диапазон '1' — '9'
					первая кнопка (i = 6, j = 0) : sz_digit[0] = 6 + 0 + '1' = '7'
					вторая кнопка (i = 6, j = 1) : sz_digit[0] = 6 + 1 + '1' = '8' и т.д. */
				CreateWindowEx
				(
					NULL, "Button",sz_digit,
					WS_CHILD | WS_VISIBLE,
					g_i_BUTTON_START_X + (g_i_BUTTON_SIZE + g_i_INTERVAL) * j,
					g_i_BUTTON_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL) * (2 - i/3),
					g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
					hwnd,
					(HMENU)(IDC_BUTTON_1 + i+j),  //ID дочернего окна позиция кнопки по горизонтали зависит от j (индекс в ряду)
					GetModuleHandle(NULL),				// позиция кнопки по вертикали зависит от i (номера ряда)
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
			(HMENU)IDC_BUTTON_POINT,
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
				(HMENU)(IDC_BUTTON_PLUS + i),  /// чтобы минус не работал как Backspace
				GetModuleHandle(NULL),
				NULL
			);
		}
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
	break;
	case WM_COMMAND:
	{
		static DOUBLE a = DBL_MIN;
		static DOUBLE b = DBL_MIN;
		static WORD operation = 0;
		static BOOL input = FALSE;
		static BOOL operation_input = FALSE;
		static BOOL equal_pressed = FALSE;
	

		SetFocus(hwnd); // без него не работает esc  чтобы всегда работала клава
		HWND hEditDisplay = GetDlgItem(hwnd, IDC_EDIT_DISPLAY);
		CONST INT SIZE = 256;
		CHAR sz_display[SIZE]{};
		CHAR sz_digit[2]{};
		if (LOWORD(wParam) >= IDC_BUTTON_0 && LOWORD(wParam) <= IDC_BUTTON_9)
		{
			// if was input operation we need screen сбрасываем
			{
				if (operation_input || equal_pressed)SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)"");
				operation_input = equal_pressed = FALSE;  // чтобы вводились двузначные
			}
			sz_digit[0] = LOWORD(wParam) - IDC_BUTTON_0 + '0';
			SendMessage(hEditDisplay, WM_GETTEXT, SIZE, (LPARAM)sz_display);
			if (strlen(sz_display) == 1 && sz_display[0] == '0')
				sz_display[0] = sz_digit[0];
			else
				strcat(sz_display, sz_digit);
			SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_display);
			input = TRUE;

		}
		if (LOWORD(wParam) == IDC_BUTTON_POINT)
		{
			SendMessage(hEditDisplay, WM_GETTEXT, SIZE, (LPARAM)sz_display);//cчитываем содержимое экрана
			if (strchr(sz_display, '.'))break; // чтобы точку можно было поставить только одну
				strcat(sz_display, ".");
				SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_display);
		}
		if (LOWORD(wParam) == IDC_BUTTON_BSP)
		{
			SendMessage(hEditDisplay, WM_GETTEXT, SIZE, (LPARAM)sz_display);
			if (strlen(sz_display) > 1)
				sz_display[strlen(sz_display) - 1] = 0;
			else
				sz_display[0] = '0';
			SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_display);
		}
		if (LOWORD(wParam) == IDC_BUTTON_CLR)
		{
			a = b = DBL_MIN;
			operation = 0;
			input = operation_input = FALSE;
			SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)"0");
		}
		///////////////////////////////////////////////////    hard code    ////////////////////
		if (LOWORD(wParam) >= IDC_BUTTON_PLUS && LOWORD(wParam) <= IDC_BUTTON_SLASH)
		{
			SendMessage(hEditDisplay, WM_GETTEXT, SIZE, (LPARAM)sz_display);
			if (input && a == DBL_MIN) a = atof(sz_display);
			//input = FALSE; // устанавливаем когда
			if (operation && input)
				SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_EQUAL), 0);
			operation = LOWORD(wParam);
			//прежде чем ввели новую операцию нужно выполнить старую
			operation_input = TRUE;
			equal_pressed = FALSE;
		}
		if (LOWORD(wParam) == IDC_BUTTON_EQUAL)
		{
			SendMessage(hEditDisplay, WM_GETTEXT, SIZE, (LPARAM)sz_display);
			// для того, чтобы выполнить операцию нам надо считать сообщение с экрана
			if (input) b = atof(sz_display);
			input = FALSE;
			switch (operation)
			{
			case IDC_BUTTON_PLUS: a += b; break;
			case IDC_BUTTON_MINUS: a -= b; break;
			case IDC_BUTTON_ASTER: a *= b; break;
			case IDC_BUTTON_SLASH: a /= b; break;
			}
			operation_input = FALSE;
			equal_pressed = TRUE;
			sprintf(sz_display, "%g", a);
			SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_display);  
		}
	}
		break;
	case WM_KEYDOWN:
	{
		if (wParam >= '0' && wParam <= '9')
		{
			SendMessage(hwnd, WM_COMMAND, LOWORD(wParam - '0' + IDC_BUTTON_0), 0);
		}
		if (wParam >= 0x60 && wParam <= 0x69)
		{
			SendMessage(hwnd, WM_COMMAND, LOWORD(wParam - 0x60 + IDC_BUTTON_0), 0);
		}
		switch (wParam)
		{
	
		case VK_OEM_PERIOD: // SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_POINT), 0); break;  // точка , которая возле буквы Ю   // от разных кейсов может обрабатывать один и тот же код
		case VK_DECIMAL: SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_POINT), 0); break;  // точка , которая NUMPAD
		case VK_BACK: SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_BSP), 0); break;
		case VK_ESCAPE:SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_CLR), 0); break;
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

INT GetTitleBarHeight(HWND hwnd)
{
	RECT window_rect;
	RECT client_rect;
	GetWindowRect(hwnd, &window_rect);
	GetClientRect(hwnd, &client_rect);
	INT title_bar_height = (window_rect.bottom - window_rect.top) - (client_rect.bottom - client_rect.top);
	return title_bar_height;
}
