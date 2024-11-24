//MainWindow
#include<Windows.h>
#include<cstdio>
#include"resource.h"

CONST CHAR g_sz_WINDOW_CLASS[] = "My Main Window";
CHAR sz_buff[256] = {};
HCURSOR hCursor = NULL;
int width{}, height{}, x{}, y{};
int pixel = 7;

INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	//DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc, 0);
	//1)Регистрация класса окна
	WNDCLASSEX wClass;
	ZeroMemory(&wClass, sizeof(wClass));

	wClass.style = 0;
	wClass.cbSize = sizeof(wClass);
	wClass.cbClsExtra = 0;
	wClass.cbWndExtra = 0; // cb .... - CountBytes;

	wClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wClass.hbrBackground = (HBRUSH)COLOR_WINDOW;

	wClass.hInstance = hInstance;
	wClass.lpszMenuName = NULL;
	wClass.lpszClassName = g_sz_WINDOW_CLASS;
	wClass.lpfnWndProc = (WNDPROC) WndProc;
	wClass.style = CS_VREDRAW | CS_HREDRAW;

	if (!RegisterClassEx(&wClass))
	{
		MessageBox(NULL, "Class registration failed","", MB_OK | MB_ICONERROR);
		return 0;
	}
	 //2)Создание окна
	HWND hwnd = CreateWindowEx
	(
		NULL,    // Ex-Style
		g_sz_WINDOW_CLASS, // Class name
		g_sz_WINDOW_CLASS, // Window title
		WS_OVERLAPPEDWINDOW, // Window stile. такой стиль всегда задаётся для главного окна
		CW_USEDEFAULT, CW_USEDEFAULT,   // Position - положение окна при запуске
		CW_USEDEFAULT, CW_USEDEFAULT,   // Size - размер создаваемого окна
		NULL,  // Родительского окна нет
		NULL,  //  hMenu  - для главного окна это ResorceID главного меню, для дочернего окна (элемента какого-то окна) 
		//  это ResourceID соответствующего элемента. По этому ResourceID нужный элемент всегда можно получить
		//    при помощи функции GetDlgItem, если не NULL не сработает
		hInstance,
		NULL

	);
	if (hwnd == NULL)
	{
		MessageBox(NULL, "Window creation failed", "", MB_OK | MB_ICONERROR);
		return 0;
	}
	ShowWindow(hwnd, nCmdShow); // Задает режим отображения окна(Развернуто на весь экран, Свернуто в окно, Свернуто на панель задач .....)
	UpdateWindow(hwnd);  // Прорисовывает окно.

	//3)Запуск цикла сообщений.

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
		HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
		SendMessage(hwnd, WM_SETICON, 0, (LPARAM)hIcon);

		//мульткурсор
		hCursor = LoadCursorFromFile("ANI\\3work.ani");
		if (!hCursor)
		{
			MessageBox(hwnd, "Не удалось загрузить курсор", "Ошибка", MB_ICONERROR);
		}
		//процентнопрозрачное окно
		SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
		// Make this window 95% alpha
		int alfaChanel = 95;
		SetLayeredWindowAttributes(hwnd, 0, (255 * alfaChanel) / 100, LWA_ALPHA);

		//разрешение экрана
		int screenWidth = GetSystemMetrics(SM_CXSCREEN);
		int screenHeight = GetSystemMetrics(SM_CYSCREEN);

		//размещение и размер в процентах относительно экрана
		int sizeWndFromScreen = 75;
		width = screenWidth * sizeWndFromScreen / 100;
		height = screenHeight * sizeWndFromScreen / 100;
		int xPos = (screenWidth - width) / 2;
		int yPos = (screenHeight - height) / 2;
		SetWindowPos(hwnd, NULL, xPos, yPos, width, height, SWP_NOZORDER);
		//break;
	}
	return 0;
	case WM_COMMAND:
		break;
	case WM_PAINT:
	{
		//PAINTSTRUCT ps;
		//HDC hdc = BeginPaint(hwnd, &ps);

		//// получение размеров клиенсткой области
		//RECT rect;
		//GetClientRect(hwnd, &rect);

		////  уменьшение справа и снизу на n пиксерелей
		//rect.right -= pixel;
		//rect.bottom -= pixel;

		//// отрисовка прямоугольника
		//FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));

		//EndPaint(hwnd, &ps);
	}
		break;
	case WM_SIZE:
	{
		// размер в заголовке
		width = LOWORD(lParam);
		height = HIWORD(lParam);
		snprintf(sz_buff, sizeof(sz_buff), " X %d x Y %d : W %d x H %d", x, y, width, height);
		SetWindowText(hwnd, sz_buff);

		InvalidateRect(hwnd, NULL, TRUE); // запрашивает перерисовку
		//break;
	}
	return 0;
	case WM_MOVE:
	{
		// размещение в заголовке
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		snprintf(sz_buff, sizeof(sz_buff), " X %d x Y %d : W %d x H %d", x, y, width, height);
		SetWindowText(hwnd, sz_buff);
		//break;
	}
	return 0;
	case WM_SETCURSOR:
	{
		// получаем координаты курсора в клиентской области
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(hwnd, &pt);

		// получаем клиентский прямоугольник
		RECT rect;
		GetClientRect(hwnd, &rect);
		rect.right -= pixel;
		rect.bottom -= pixel;

		// устанавливаем курсор в зависимости от позиции
		if (PtInRect(&rect, pt))
		{
			SetCursor(hCursor);
		}
		else
		{
			SetCursor(LoadCursor(NULL, IDC_ARROW));
		}
		return TRUE; // указываем, что мы обработали сообщение
		/*SetCursor(hCursor);
		return TRUE;*/
	}
		//break;
	case WM_LBUTTONDOWN:
	{
		//перетаскивание окна 
		PostMessage(hwnd, WM_SYSCOMMAND, SC_MOVE | HTCAPTION, 0);
		break;
	}
	//case WM_MOUSEMOVE:
	//{
	//	// получаем текущие координаты курсора
	//	POINT pt;
	//	pt.x = LOWORD(lParam);
	//	pt.y = HIWORD(lParam);

	//	// получаем клиентский прямоугольник окна
	//	RECT rect;
	//	GetClientRect(hwnd, &rect);

	//	// уменьшаем его размер справа и снизу на n пикселей
	//	rect.right -= pixel;
	//	rect.bottom -= pixel;

	//	// проверяем, находится ли курсор внутри уменьшенного прямоугольника
	//	if (PtInRect(&rect, pt))
	//	{
	//		// если курсор внутри, устанавливаем анимированный курсор
	//		SetCursor(hCursor);
	//	}
	//	else
	//	{
	//		// если курсор вне, устанавливаем стандартный курсор
	//		SetCursor(LoadCursor(NULL, IDC_ARROW));
	//	}
	//}
	//return 0;
	case WM_DESTROY:
	{
		if (hCursor)
		{
			DestroyCursor(hCursor);
		}
		PostQuitMessage(0);
		break;
	}
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	default: return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}