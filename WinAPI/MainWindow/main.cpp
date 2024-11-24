//MainWindow
#include<Windows.h>
#include"resource.h"

CONST CHAR g_sz_WINDOW_CLASS[] = "My Main Window";
HCURSOR hCursor = NULL;

INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	//DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc, 0);
	//1)����������� ������ ����
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
	 //2)�������� ����
	HWND hwnd = CreateWindowEx
	(
		NULL,    // Ex-Style
		g_sz_WINDOW_CLASS, // Class name
		g_sz_WINDOW_CLASS, // Window title
		WS_OVERLAPPEDWINDOW, // Window stile. ����� ����� ������ ������� ��� �������� ����
		CW_USEDEFAULT, CW_USEDEFAULT,   // Position - ��������� ���� ��� �������
		CW_USEDEFAULT, CW_USEDEFAULT,   // Size - ������ ������������ ����
		NULL,  // ������������� ���� ���
		NULL,  //  hMenu  - ��� �������� ���� ��� ResorceID �������� ����, ��� ��������� ���� (�������� ������-�� ����) 
		//  ��� ResourceID ���������������� ��������. �� ����� ResourceID ������ ������� ������ ����� ��������
		//    ��� ������ ������� GetDlgItem, ���� �� NULL �� ���������
		hInstance,
		NULL

	);
	if (hwnd == NULL)
	{
		MessageBox(NULL, "Window creation failed", "", MB_OK | MB_ICONERROR);
		return 0;
	}
	ShowWindow(hwnd, nCmdShow); // ������ ����� ����������� ����(���������� �� ���� �����, �������� � ����, �������� �� ������ ����� .....)
	UpdateWindow(hwnd);  // ������������� ����.

	//3)������ ����� ���������.

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
			MessageBox(hwnd, "�� ������� ��������� ������", "������", MB_ICONERROR);
		}
		//������������������� ����
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