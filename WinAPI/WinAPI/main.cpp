#include<Windows.h>
#include "resource.h"


CONST CHAR* init_value = "Text here";
//#define MESSAGE_BOX
BOOL CALLBACK DlgProc(HWND  hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
// первый параметр дескриптор экземпляра приложения, второй не использ., третий предоставляет доступ к переменным cmd, режим отображения окна , иногда указывается в качестве второго аргумента функции ShowWindow(hwnd, iShow)
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{


#ifdef MESSAGE_BOX
	MessageBox
	(NULL,
		"Hello WinAPI!\nЭто окно сообщения",
		"Info", MB_YESNOCANCEL | MB_ICONINFORMATION | MB_HELP | MB_DEFBUTTON2
		| MB_SYSTEMMODAL
	);
#endif // MESSAGE_BOX

	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc, 0);
	return 0;
}
BOOL CALLBACK DlgProc(HWND  hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		HWND hEdit1 = GetDlgItem(hwnd, IDC_EDIT1);
		HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
		SendMessage(hwnd, WM_SETICON, 0, (LPARAM)hIcon);
		SetWindowText(hEdit1, init_value); // первоначальный текст
	}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK: MessageBox(hwnd, "Была нажата кнока OK", "Info", MB_OK | MB_ICONINFORMATION); break;
		case IDC_BUTTON1:
		{
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE]{};
			// получаем текст из первого окна редактирования
			HWND hEdit1 = GetDlgItem(hwnd, IDC_EDIT1);
			GetWindowText(hEdit1, sz_buffer, SIZE);

			// устанавливаем текст во второе окно редактирования
			HWND hEdit2 = GetDlgItem(hwnd, IDC_EDIT2);
			SetWindowText(hEdit2, sz_buffer);
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