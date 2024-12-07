#define _CRT_SECURE_NO_WARNINGS  // добавляем при ошибке С4996
#include<Windows.h>
#include"Resource.h"
#include"Dimensions.h"
#include"string"

CONST CHAR g_sz_WINDOW_CLASS[] = "Calc_VPD_311";

CONST CHAR* g_OPERATION[] = { "+" ,"-", "*","/" };

DOUBLE* d_numbers = NULL;
CHAR* c_symbols = NULL;

template<typename T>
void ExpandArray(T*& array, INT& capacity, INT initialSize = 10);

//CHAR* GetButtonText(HWND hButton);

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
					(HMENU)(IDC_BUTTON_1 + i+j),
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
				(HMENU)IDC_BUTTON_PLUS + i,
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
		HWND hEditDisplay = GetDlgItem(hwnd, IDC_EDIT_DISPLAY);
		CONST INT SIZE = 256;
		CHAR sz_display[SIZE]{};
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
		if (LOWORD(wParam) == IDC_BUTTON_BSP)
		{
			SendMessage(hEditDisplay, WM_GETTEXT, SIZE, (LPARAM)sz_display); // считываем строку
			if (strlen(sz_display) > 0)  sz_display[strlen(sz_display) - 1] = '\0';
			SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_display);
		}
		if (LOWORD(wParam) == IDC_BUTTON_CLR)
		{
			SendMessage(hEditDisplay, WM_GETTEXT, SIZE, (LPARAM)sz_display); // считываем строку
			sz_display[0] = '\0';
			SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_display);
		}
		if (LOWORD(wParam) == IDC_BUTTON_EQUAL)
		{
			SendMessage(hEditDisplay, WM_GETTEXT, SIZE, (LPARAM)sz_display); // считываем строку
			//sz_display = Calc(sz_display);
			SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_display);
		}
		

		if (LOWORD(wParam) == IDC_BUTTON_PLUS)
		{
			GetWindowText(GetDlgItem(hwnd, IDC_BUTTON_PLUS), sz_digit, sizeof(sz_digit) / sizeof(CHAR));
			//memcpy( /*to*/ sz_digit, /*from*/ GetButtonText(GetDlgItem(hwnd, IDC_BUTTON_PLUS)), sizeof(sz_digit));// /*capacity * sizeof(T)*/); // побайтовое копирование, игнорируя типы данных массива
			SendMessage(hEditDisplay, WM_GETTEXT, SIZE, (LPARAM)sz_display); // считываем строку
			if (sz_display[strlen(sz_display) - 1] == '+')break;  // не даёт поставить следующий повторяющийся знак операции
			strcat(sz_display, sz_digit);
			//sz_display[1] = '\0';
			SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_display);
		}
		if (LOWORD(wParam) == IDC_BUTTON_ASTER)
		{
			GetWindowText(GetDlgItem(hwnd, IDC_BUTTON_ASTER), sz_digit, sizeof(sz_digit) / sizeof(CHAR));
			//memcpy( /*to*/ sz_digit, /*from*/ GetButtonText(GetDlgItem(hwnd, IDC_BUTTON_ASTER)), sizeof(sz_digit));// побайтовое копирование, игнорируя типы данных массива
			SendMessage(hEditDisplay, WM_GETTEXT, SIZE, (LPARAM)sz_display); // считываем строку
			strcat(sz_display, sz_digit);
		
			SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_display);
		}
		if (LOWORD(wParam) == IDC_BUTTON_SLASH)
		{
			GetWindowText(GetDlgItem(hwnd, IDC_BUTTON_SLASH), sz_digit, sizeof(sz_digit) / sizeof(CHAR)+1);
			//memcpy( /*to*/ sz_digit, /*from*/ GetButtonText(GetDlgItem(hwnd, IDC_BUTTON_ASTER)), sizeof(sz_digit));// побайтовое копирование, игнорируя типы данных массива
			SendMessage(hEditDisplay, WM_GETTEXT, SIZE, (LPARAM)sz_display); // считываем строку
			strcat(sz_display, sz_digit);

			SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_display);
		}
	}
		break;
	case WM_CHAR:
	{
		HWND hEditDisplay = GetDlgItem(hwnd, IDC_EDIT_DISPLAY);
		CONST INT SIZE = 256;
		CHAR sz_display[SIZE]{};
		
		INT numIndex = 0, symIndex = 0;
		INT numCapacity = 0, symCapapcity = 0;
		std::string currentNumber;  // промежуточная строка

		CHAR inputChar = static_cast<CHAR>(LOWORD(wParam));
		
		if (inputChar == '+' || inputChar == '-' || inputChar == '*' || inputChar == '/')
		{
			if (!currentNumber.empty())
			{
				if (numIndex >= numCapacity) ExpandArray(d_numbers, numCapacity);
				d_numbers[numIndex++] == std::atof(currentNumber.c_str());  //converts str into a double, then returns that value. str must start with a valid number, but can be terminated with any non-numerical character
				currentNumber.clear();
			}
			if (symIndex >= symCapapcity) ExpandArray(c_symbols, numCapacity);
			c_symbols[symIndex++] == inputChar;

		}
		else if (isdigit(inputChar) || inputChar == '.')
		{
			currentNumber += inputChar;
		}

		
	}
	break;
	case WM_DESTROY:
		delete[] d_numbers;
		delete[] c_symbols;
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

// функция для увеличения размера массива
template<typename T>
void ExpandArray(T*& array, INT& capacity, INT initialSize)
{
	INT newCapacity = capacity == 0 ? initialSize : capacity * 2;
	T* newArray = new T[newCapacity];
	if (array)
	{
		memcpy( /*to*/ newArray, /*from*/ array, capacity * sizeof(T)); // побайтовое копирование, игнорируя типы данных массива
		delete[] array;
	}
	array = newArray;
	capacity = newCapacity;
}
//CHAR* GetButtonText(HWND hButton)
//{
//	CHAR txtBuff[256];
//	INT txtLength = GetWindowText(hButton, txtBuff, sizeof(txtBuff) / sizeof(CHAR));
//	if (txtLength > 0) return txtBuff;
//	else MessageBox(NULL, "Failed to get button text or text is empty", "Error", MB_OK);
//}
