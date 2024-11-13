﻿#define _CRT_SECURE_NO_WARNINGS  // добавляем при ошибке С4996
#include<Windows.h>
#include"resource.h"
#include<cstdio>

CONST CHAR* g_VALUES[] = { "This","is","my", "first","List","Box" };

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgProcAddItem(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
		/*
		---------------------------
		WinMain
		hInstance - экземпляр нашего приложения в памяти (экземпляр *.exe-файла)
		hPrevInst - предыдущий запущенный экземпляр (Deprecated, использовался в Windows 98).
		LPSTR lpCmdLine:
		LPSTR - LongPointer to String (Указатель на строку наподобие const char*)
		lpCmdLine - коммандная строка запуска приложения
		nCmdShow - режим отображения окна: Свернуто в окно, Развернуто на весь экран, свернуто в пнель задач......
		*/
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc, 0);
	return 0;

}
BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_INITDIALOG:
		{
			HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
			SendMessage(hwnd, WM_SETICON, 0, (LPARAM)hIcon );

			HWND hListBox = GetDlgItem(hwnd, IDC_LIST);
			for (int i = 0; i < sizeof(g_VALUES) / sizeof(g_VALUES[0]); i++)
			{
				SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)g_VALUES[i] );
			}	
		}
			break;
		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
			case IDC_BUTTON_ADD:
				DialogBoxParam(GetModuleHandle(NULL) /*получаем hinstance*/
					, MAKEINTRESOURCE(IDD_DIALOG_ADD_ITEM), hwnd, DlgProcAddItem, 0);
				break;

			case IDOK:
			{
				HWND hListBox = GetDlgItem(hwnd, IDC_LIST);
				CONST INT SIZE = 256;
				CHAR sz_buffer[SIZE]{};
				INT i = SendMessage(hListBox, LB_GETCURSEL, 0, 0);

				CHAR sz_message[SIZE]{};
				sprintf(sz_message, "Вы выбрали элемент N%i со значением \"%s\".", i, sz_buffer);
				MessageBox(hwnd, sz_message, "Info", MB_OK | MB_ICONINFORMATION);
			}
			//break;  // если с ним, то выскакивающее окно сообщение повторяется, если без , то после добавления окошко сразу закрывается
			case IDCANCEL: EndDialog(hwnd, 0); break;
			}
			break;
		case WM_CLOSE:
			EndDialog(hwnd, 0);
			break;
	}
	return FALSE;
}
BOOL CALLBACK DlgProcAddItem(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:

		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			//1) Читаем тексе из EditControl:
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE]{};
			HWND hEdit = GetDlgItem(hwnd, IDC_EDIT_ADD_ITEM);
			SendMessage(hEdit, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);

			// 2) Получаем родительское окно:
			HWND hParent = GetParent(hwnd);

			// 3) Получаем дескриптор ListBox:
			HWND hListBox = GetDlgItem(hParent, IDC_LIST);

			// 4) Добавляем текст в ListBox:
			SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)sz_buffer);

		}
		//break;  // убираем, если нужно, чтобы добавлялся только один элемент
		case IDCANCEL: EndDialog(hwnd, 0); break;
		}
		break;
		case WM_CLOSE: EndDialog(hwnd, 0);
		//break;
	}
	return FALSE;
}