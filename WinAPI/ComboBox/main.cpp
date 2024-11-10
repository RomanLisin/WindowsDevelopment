#define _CRT_SECURE_NO_WARNINGS  // добавляем при ошибке С4996
#include<Windows.h>
#include<cstdio>  // чтобы работал sprintf(...)
#include"resource.h"

CONST CHAR* init_values[] = { "Привет", "This", "is", "my", "first", "Combo", "Box" };

BOOL CALLBACK DlgProc(HWND nwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{

	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc, 0);
	return 0;

}
BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		{
			HWND hCombo = GetDlgItem(hwnd, IDC_COMBO1);    // функция GetDlgItem() возвращает HWND указанного дочерего окна
			for (int i = 0; i < sizeof(init_values) / sizeof(init_values[0]); i++)
			{
				SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)init_values[i]);
			}
		}
			break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			HWND hCombo = GetDlgItem(hwnd, IDC_COMBO1);
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE]{};
			CHAR sz_message[SIZE]{};
			INT i = SendMessage(hCombo, CB_GETCURSEL, 0, 0);
			// Сообщение CB_GETCURSEL (Current Selection) возвращает номер выбранного пункта ComboBox
			SendMessage(hCombo, CB_GETLBTEXT, i, (LPARAM)sz_buffer);
			sprintf(sz_message, "Вы выбрали пункт N%i со значением \"%s\".", i, sz_buffer);
			MessageBox(hwnd, sz_message, "Selected value", MB_OK | MB_ICONINFORMATION);
			break;
		}
		case IDCANCEL: EndDialog(hwnd, 0); break;
		}
		break;
	case WM_CLOSE:
		EndDialog(hwnd, 0);
		break;
	}
	return FALSE;
}