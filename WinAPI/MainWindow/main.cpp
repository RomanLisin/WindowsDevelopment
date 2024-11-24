//MainWindow
#include<Windows.h>
#include"resource.h"

CONST CHAR g_sz_WINDOW_CLASS[] = "My Main Window";
HCURSOR hCursor = NULL;

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

		hCursor = LoadCursorFromFile("ANI\\3work.ani");
		if (!hCursor)
		{
			MessageBox(hwnd, "Не удалось загрузить курсор", "Ошибка", MB_ICONERROR);
		}
		//процентнопрозрачное окно
		//SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
		// Make this window 95% alpha
		//int alfaChanel = 95;
		//SetLayeredWindowAttributes(hwnd, 0, (255 * alfaChanel) / 100, LWA_ALPHA);
		break;
	}
	case WM_COMMAND:
		break;
	case WM_SETCURSOR:
	{
		SetCursor(hCursor);
		return TRUE;
	}
		break;
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