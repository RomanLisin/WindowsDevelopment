#define _CRT_SECURE_NO_WARNINGS  // добавляем при ошибке С4996
#include<Windows.h>
#include"Resource.h"
#include"Dimensions.h"
#include<float.h>
#include<iostream>
#include<stdio.h>



CONST CHAR g_sz_WINDOW_CLASS[] = "Calc_VPD_311";

CONST CHAR* g_OPERATION[] = { "+" ,"-", "*","/" };

CONST CHAR* nonDigitButton[] = { "point","plus","minus","aster","slash","bsp","clr","equal" };
//CHAR* skin[256]{};
HFONT g_hFont = NULL;

INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

VOID SetSkin(HWND hwnd, CONST CHAR skin[]);
VOID LoadCustomFont(HWND hwnd, CONST CHAR skin[]);
VOID CleanupFont();

enum COLOR { BLUE, YELLOW, GRЕY };
enum ELEMENT { BACKGROUND, SCREEN, TEXT };
HFONT g_Font = NULL;
CONST COLORREF g_COLORS[][3] =
{
	{ RGB(145, 37, 201), RGB(0, 188, 213), RGB(1, 1, 1) }, // square blue
	{ RGB(90, 139, 153), RGB(50, 100, 0), RGB(0, 255, 50) },
	{ RGB(135, 132, 123), RGB(209, 244, 0) /*экран метал*/, RGB(1, 1, 1)}
};

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
		g_i_WINDOW_WIDTH+16, g_i_WINDOW_HEIGHT +52,
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
	static CONST CHAR font[] = "light_led_display-7";
	static COLOR color = COLOR::YELLOW;
	static HMENU hMenuMain, hMenuEdit;

	switch (uMsg)
	{
	case WM_CREATE:
	{
		HINSTANCE hInstance = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);
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
		if (g_hFont)
		{
			DeleteObject(g_hFont); // удаляем старый шрифт
		}
		g_hFont = CreateFont
		(
			g_i_FONT_SIZE,                // высота символов
			0,                       // ширина символов
			0,                       // угол ориентации
			0,                       // угол начертания
			FW_NORMAL,               //	толщина шрифта
			FALSE,                   // курсив
			FALSE,                   // подчеркивание
			FALSE,                   // зачеркивание
			DEFAULT_CHARSET,         // набор символов
			OUT_DEFAULT_PRECIS,      // точность вывода
			CLIP_DEFAULT_PRECIS,     // точность отсеченияd
			DEFAULT_QUALITY,         // качество вывода
			DEFAULT_PITCH | FF_SWISS,// шаг шрифта и семейство
			"light_led_display-7"                  // имя шрифта
		);
		SendMessage(hEdit, WM_SETFONT, (WPARAM)g_hFont, TRUE);
		LoadCustomFont(hwnd, "light_led_display-7");

		CHAR sz_digit[2] = {};
		for (int i = 6; i >= 0; i -= 3)  //отвечает за ряды кнопок сверху вниз ,  i равнo : 6, 3, 0. Это три ряда(3 строки).
		{
			for (int j = 0; j < 3; j++) //отвечает за кнопки в одном ряду слева направо,j равнo : 0, 1, 2. Это три кнопки в строке
			{
				sz_digit[0] = i + j + '1'; // создаёт текст кнопки
				/*	i + j — порядковый номер кнопки(0, 1, 2 и так далее)
					'1' — добавление, чтобы сдвинуть номера в диапазон '1' — '9'
					первая кнопка (i = 6, j = 0) : sz_digit[0] = 6 + 0 + '1' = '7'
					вторая кнопка (i = 6, j = 1) : sz_digit[0] = 6 + 1 + '1' = '8' и т.д. */
				CreateWindowEx
				(
					NULL, "Button", sz_digit,
					WS_CHILD | WS_VISIBLE | BS_BITMAP,
					g_i_BUTTON_START_X + (g_i_BUTTON_SIZE + g_i_INTERVAL) * j,
					g_i_BUTTON_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL) * (2 - i / 3),
					g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
					hwnd,
					(HMENU)(IDC_BUTTON_1 + i + j),  //ID дочернего окна позиция кнопки по горизонтали зависит от j (индекс в ряду)
					GetModuleHandle(NULL),				// позиция кнопки по вертикали зависит от i (номера ряда)
					NULL
				);
			}
		}
		// 1) 
		HWND hButton_0 = CreateWindowEx
		(
			NULL, "Button", "0",
			WS_CHILD | WS_VISIBLE | BS_BITMAP, // без BITMAP отображается дефолтный стиль
			BUTTON_SHIFT_X(0), BUTTON_SHIFT_Y(3),
			/*g_i_BUTTON_START_X, g_i_BUTTON_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL) * 3,*/
			g_i_BUTTON_DOUBLE_SIZE, g_i_BUTTON_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_0,
			GetModuleHandle(NULL),
			NULL
		);
		// 2) Загрузить картинку из файла:
		HBITMAP bmpButton_0 = (HBITMAP)LoadImage(NULL, "ButtonsBMP\\square_blue\\button_0.bmp", IMAGE_BITMAP, g_i_BUTTON_DOUBLE_SIZE, g_i_BUTTON_SIZE, LR_LOADFROMFILE);
		// 3) Установить картинку на кнопку
		SendMessage(hButton_0, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmpButton_0);
		HWND hButton_Point = CreateWindowEx
		(
			NULL, "Button", ".",
			WS_CHILD | WS_VISIBLE | BS_BITMAP,
			BUTTON_SHIFT_X(2), BUTTON_SHIFT_Y(3),
			/*g_i_BUTTON_START_X + (g_i_BUTTON_SIZE + g_i_INTERVAL)*2,
			g_i_BUTTON_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL)*3,*/
			g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_POINT,
			GetModuleHandle(NULL),
			NULL
		);
		// 2) Загрузить картинку из файла:
		HBITMAP bmpButton_Point = (HBITMAP)LoadImage(NULL, "ButtonsBMP\\square_blue\\button_point.bmp", IMAGE_BITMAP, g_i_BUTTON_DOUBLE_SIZE, g_i_BUTTON_SIZE, LR_LOADFROMFILE);
		// 3) Установить картинку на кнопку
		SendMessage(hButton_Point, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmpButton_Point);
		for (int i = 0; i < 4; i++)
		{
			CreateWindowEx
			(
				NULL, "Button", g_OPERATION[i],
				WS_CHILD | WS_VISIBLE | BS_BITMAP,
				BUTTON_SHIFT_X(3), BUTTON_SHIFT_Y(3 - i),
				/*g_i_BUTTON_START_X + (g_i_BUTTON_SIZE + g_i_INTERVAL) * 3,
				g_i_BUTTON_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL) * (3 - i),*/
				g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
				hwnd,
				(HMENU)(IDC_BUTTON_PLUS + i),  /// (скобки ) чтобы минус не работал как Backspace, без скобок - дублирует Backspace
				GetModuleHandle(NULL),
				NULL
			);
		}
		CreateWindowEx
		(
			NULL, "Button", "<-",
			WS_CHILD | WS_VISIBLE | BS_BITMAP,
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
			WS_CHILD | WS_VISIBLE | BS_BITMAP,
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
			WS_CHILD | WS_VISIBLE | BS_BITMAP,
			BUTTON_SHIFT_X(4), BUTTON_SHIFT_Y(2),
			g_i_BUTTON_SIZE, g_i_BUTTON_DOUBLE_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_EQUAL,
			GetModuleHandle(NULL),
			NULL
		);
		// 2) Загрузить картинку из файла:
		bmpButton_0 = (HBITMAP)LoadImage(NULL, "ButtonsBMP\\button_0.bmp", IMAGE_BITMAP, g_i_BUTTON_DOUBLE_SIZE, g_i_BUTTON_SIZE, LR_LOADFROMFILE);
		// 3) Установить картинку на кнопку
		SendMessage(hButton_0, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmpButton_0);
		SetSkin(hwnd, "square_blue");  // устанавливает в каждую кнопку соответствующую иконку
	
		hMenuMain = LoadMenu(hInstance, "MenuMain");
		SetMenu(hwnd, hMenuMain);
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

		// обработка команд меню, смена цветовой схемы
		switch (LOWORD(wParam))
		{
		case CM_SQUARE_BLUE:
			color = BLUE;
			break;
		case CM_SQUARE_YELLOW:
			color = YELLOW;
			break;
		}

		SetFocus(hwnd); // без него не работает esc,  чтобы всегда работала клава
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
			sz_digit[0] /*<- ASCII '49'*/ = LOWORD(wParam) /*(1000-1015) see resource.h*/ - IDC_BUTTON_0 /*1000*/ + '0' /*'48' ASCII*/;
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
		// принудительная перерисовка окна
		InvalidateRect(hwnd, NULL, TRUE);
		UpdateWindow(hwnd);
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
	case WM_ERASEBKGND:
	{
		HDC hdc = (HDC)wParam;
		//получаем размеры клиентской области
		RECT rect;
		GetClientRect(hwnd, &rect);
		// создаем кисть для фона
		HBRUSH hBrush = CreateSolidBrush(g_COLORS[color][ELEMENT::BACKGROUND]);
		SetClassLongPtr(hwnd, GCLP_HBRBACKGROUND, (LONG_PTR)hBrush);
		// используем кисть для заполнения фона
		FillRect(hdc, &rect, hBrush);
		// deleteObject(hBrush);

		return 1;
	}
	break;
	case WM_CTLCOLOREDIT:
	{
		HDC hdc = (HDC)wParam;
		// Установка параметров фона и текста
		SetBkMode(hdc, OPAQUE);
		SetBkColor(hdc, g_COLORS[color][ELEMENT::SCREEN]);
		SetTextColor(hdc, g_COLORS[color][ELEMENT::TEXT]);
		return 0;
	}
	break;
	case WM_CONTEXTMENU:
	{
		HMENU hMainMenu = CreatePopupMenu();
		HMENU hSubMenu = CreatePopupMenu();
		HMENU hFontMenu = CreatePopupMenu();

		InsertMenu(hMainMenu, 0, MF_BYPOSITION | MF_STRING, CM_EXIT, "Exit");
		InsertMenu(hMainMenu, 0, MF_BYPOSITION | MF_STRING, 0, NULL);
		InsertMenu(hMainMenu, 0, MF_BYPOSITION | MF_POPUP, (UINT_PTR)hFontMenu, "Fonts");
		InsertMenu(hMainMenu, 0, MF_BYPOSITION | MF_POPUP, (UINT_PTR)hSubMenu, "Skins");

		InsertMenu(hSubMenu, 0, MF_BYPOSITION | MF_STRING, 0, "New model");
		InsertMenu(hSubMenu, 0, MF_BYPOSITION | MF_STRING, 0, "Metal mistral");
		InsertMenu(hSubMenu, 0, MF_BYPOSITION | MF_STRING, 0, "Square blue");

		InsertMenu(hFontMenu, 0, MF_BYPOSITION | MF_STRING, CM_FONT_DIGITGRAF, "Digit graf");
		InsertMenu(hFontMenu, 0, MF_BYPOSITION | MF_STRING, CM_FONT_LIGHTLED, "Light led");

		switch
			(
				TrackPopupMenuEx
				(
					hMainMenu, TPM_BOTTOMALIGN | TPM_LEFTALIGN | TPM_RETURNCMD,
					LOWORD(lParam), HIWORD(lParam),
					hwnd,
					NULL
				)
				)
		{
		case CM_FONT_DIGITGRAF:
			LoadCustomFont(hwnd, "01-digitgraphics");
			break;
		case CM_FONT_LIGHTLED:
			LoadCustomFont(hwnd, "light-led-display-7");
			break;
		case CM_EXIT:
			DestroyWindow(hwnd);
			break;
		case CM_SQUARE_BLUE:
		{
			SetSkin(hwnd, "square_blue");
			color = BLUE;
			break;
		}
		case CM_METAL_MISTRAL:
		{
			SetSkin(hwnd, "metal_mistral");
			color = GRЕY;
		}
			break;
		case CM_NEW_MODEL:
			color = YELLOW;
			break;
		}

		HDC hdc = GetDC(hwnd);
		HDC hdcEdit = GetDC(GetDlgItem(hwnd, IDC_EDIT_DISPLAY));

		SendMessage(hwnd, WM_CTLCOLOREDIT, (WPARAM)hdcEdit, 0);

		ReleaseDC(hwnd, hdcEdit);
		ReleaseDC(hwnd, hdc);

		CHAR sz_buffer[MAX_PATH]{};

		SendMessage(GetDlgItem(hwnd, IDC_EDIT_DISPLAY), WM_GETTEXT, MAX_PATH, (LPARAM)sz_buffer);
		SendMessage(GetDlgItem(hwnd, IDC_EDIT_DISPLAY), WM_SETTEXT, 0, (LPARAM)sz_buffer);

	}
	break;
	case WM_DESTROY:
		CleanupFont();
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	default: return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return FALSE;
}

VOID SetSkin(HWND hwnd, CONST CHAR skin[])
{
	CHAR sz_pathToFile[MAX_PATH]{};
	for (int i = IDC_BUTTON_0; i <= IDC_BUTTON_EQUAL; i++)
	{
		HWND hButton = GetDlgItem(hwnd, i);
		CHAR subTxtButton[256]{};

		if (i > IDC_BUTTON_9)
		{
			int index = i - IDC_BUTTON_POINT;
			if (index >= 0 && index < sizeof(nonDigitButton) / sizeof(nonDigitButton[0]))
			{
				sprintf(subTxtButton, "%s", nonDigitButton[index]);
			}
			else
			{
				std::cerr << "Index out of bounds for nonDigitButton array" << std::endl;
				continue;
			}
		}
		else
		{
			sprintf(subTxtButton, "%d", i - IDC_BUTTON_0);
		}

		RECT rect;
		int width = 0, height = 0;
		if (GetClientRect(hButton, &rect))
		{
			width = rect.right - rect.left;
			height = rect.bottom - rect.top;
		}
		else
		{
			std::cerr << "Failed to get button size" << std::endl;
			continue;
		}

		sprintf(sz_pathToFile, "ButtonsBMP\\%s\\button_%s.bmp", (LPSTR)skin, subTxtButton);

		HANDLE hImage = LoadImage(
			GetModuleHandle(NULL),
			sz_pathToFile,
			IMAGE_BITMAP,
			width,
			height,
			LR_LOADFROMFILE
		);

		if (!hImage)
		{
			std::cerr << "Failed to load image: " << sz_pathToFile << std::endl;
			continue;
		}

		HANDLE hOldImage = (HANDLE)SendMessage(hButton, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hImage);
		if (hOldImage)
		{
			DeleteObject(hOldImage);
		}
	}
}
VOID LoadCustomFont(HWND hwnd, CONST CHAR fontName[])
{	
	CHAR sz_pathToFile[MAX_PATH]{};
	HWND hEdit = GetDlgItem(hwnd, IDC_EDIT_DISPLAY);
	// Загружаем шрифт
		//if (g_hFont)
		//{
		//	DeleteObject(g_hFont); // удаляем старый шрифт
		//}
	
	//sprintf(sz_pathToFile,"Fonts\\%s\\%s.ttf", fontName, fontName);
	std::string pathToFile = "C:\\Users\\rls\\source\\repos\\WindowsDevelopment\\WinAPI\\Calc\\Fonts\\";
	//std::string fontName = "Arial"; // Пример имени шрифта

	std::string fullPath = pathToFile + fontName + "\\" + fontName + ".ttf";

	// Если нужно в char*
	strcpy(sz_pathToFile, fullPath.c_str());
	//MessageBox(NULL, sz_pathToFile, "Path to Font", MB_OK);
	if (AddFontResourceEx(sz_pathToFile, FR_PRIVATE, NULL) > 0)
	{
		// Создаем шрифт
		g_hFont = CreateFont
		(
			g_i_FONT_SIZE,                // Высота шрифта
			0,                 // Ширина шрифта (автоматическая)
			0,                 // Угол наклона
			0,                 // Ориентация
			FW_NORMAL,         // Толщина
			FALSE,             // Курсив
			FALSE,             // Подчеркивание
			FALSE,             // Зачеркнутый
			DEFAULT_CHARSET,   // Набор символов
			OUT_DEFAULT_PRECIS,// Точность вывода
			CLIP_DEFAULT_PRECIS,// Точность отсечения
			DEFAULT_QUALITY,   // Качество
			DEFAULT_PITCH | FF_DONTCARE, // Шаг шрифта
			(LPSTR)fontName  // Имя шрифта
		);

		// Устанавливаем шрифт в edit-панель
		SendMessage(hEdit, WM_SETFONT, (WPARAM)g_hFont, TRUE);
	}
	//else 
	/*{
		MessageBox(NULL, "Не удалось загрузить шрифт", "Ошибка", MB_OK | MB_ICONERROR);
	}*/
}
VOID CleanupFont()
{
	if (g_hFont) 
	{
		DeleteObject(g_hFont);
		g_hFont = NULL;
	}
}
