#define _CRT_SECURE_NO_WARNINGS  // добавляем при ошибке С4996
#include<Windows.h>
#include"resource.h"
#include<cstdio>

CONST CHAR* g_VALUES[] = { "This","is","my", "first","List","Box" };

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{

	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc, 0);
	return 0;

}
BOOL CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_INITDIALOG:
		{
			HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOUCE(IDI_ICON1));
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
			case IDCANCEL: EndDialog(hwnd, 0); break;
			}
			break;
		case WM_CLOSE:
			EndDialog(hwnd, 0);
			break;
	}
	return FALSE;
}