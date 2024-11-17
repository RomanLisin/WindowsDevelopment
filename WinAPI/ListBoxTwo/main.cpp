#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include<cstdio>
#include"resource.h"

CONST CHAR* g_VALUES[] = { "This", "is", "my", "first", "List", "Box" };

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgProcAddItem(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgProcAlterItem(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc, 0);
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
		SendMessage(hwnd, WM_SETICON, 0, (LPARAM)hIcon);

		HWND hListBox = GetDlgItem(hwnd, IDC_LIST);
		for (int i = 0; i < sizeof(g_VALUES) / sizeof(g_VALUES[0]); i++)
			SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)g_VALUES[i]);
		
	}
	break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_LIST:
		{
			//Notifications HIWORD(wParam)
			// ResourceID  LOWORD(wParam)
			if (HIWORD(wParam) == LBN_DBLCLK)// ListBoxNotification_DoubleCick
				DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_ADD_ITEM), hwnd, DlgProcAlterItem, 0);
		}
		break;
		case IDC_BUTTON_ADD:
			DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_ADD_ITEM), hwnd, DlgProcAddItem, 0);
			break;
		case IDC_BUTTON_REMOVE:
		{
			HWND hListBox = GetDlgItem(hwnd, IDC_LIST);
			INT i = SendMessage(hListBox, LB_GETCURSEL, 0, 0);
			SendMessage(hListBox, LB_DELETESTRING, i, 0);
		}
		break;
		case IDOK:
		{
			HWND hListBox = GetDlgItem(hwnd, IDC_LIST);
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE]{};
			INT i = SendMessage(hListBox, LB_GETCURSEL, 0, 0);
			SendMessage(hListBox, LB_GETTEXT, i, (LPARAM)sz_buffer);

			CHAR sz_message[SIZE]{};
			sprintf(sz_message, "Элемент %i значение\"%s\".", i, sz_buffer);
			MessageBox(hwnd, sz_message, "Info", MB_OK | MB_ICONINFORMATION);
		}
		break;
		case IDCANCEL: EndDialog(hwnd, 0); break;
		}
		break;
	case WM_VKEYTOITEM:
		switch (LOWORD(wParam))
		{
		case VK_RETURN:SendMessage(hwnd, WM_COMMAND, MAKEWPARAM(IDC_LIST, LBN_DBLCLK), (LPARAM)GetDlgItem(hwnd, IDC_LIST)); break;
		
		case VK_DELETE:SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_REMOVE), 0);break;
		}
		break; 
		//return DefDlgProc(hwnd, uMsg, wParam, lParam);
	case WM_CLOSE:
		EndDialog(hwnd, 0);
		break;
	}
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL CALLBACK DlgProcAddItem(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		SetFocus(GetDlgItem(hwnd, IDC_EDIT_ADD_ITEM));
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		
		case IDOK:
		{
			//1)
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE]{};
			HWND hEdit = GetDlgItem(hwnd, IDC_EDIT_ADD_ITEM);
			SendMessage(hEdit, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);

			//2)
			HWND hParent = GetParent(hwnd);

			//3) 
			HWND hListBox = GetDlgItem(hParent, IDC_LIST);

			//4) 
			if (SendMessage(hListBox,LB_FINDSTRINGEXACT, -1,(LPARAM)sz_buffer)==LB_ERR)
			{
				SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)sz_buffer);
			}
			else  // Чтобы окно не закрывалось при закрытии окна сообщения снизу
			{   
				INT answer = MessageBox
				(
					hwnd, "This is exist, do you want to add any more ",
					"Question",
					MB_YESNO | MB_ICONQUESTION
				);
				if (answer == IDYES)break;
			}
		}
		case IDCANCEL: EndDialog(hwnd, 0); break;
		}
		break;
	case WM_CLOSE:EndDialog(hwnd, 0);
	}
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL CALLBACK DlgProcAlterItem(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)"Change adding");
		HWND hEdit = GetDlgItem(hwnd, IDC_EDIT_ADD_ITEM);
		HWND hParent = GetParent(hwnd);
		HWND hListBox = GetDlgItem(hParent, IDC_LIST);
		INT i = SendMessage(hListBox, LB_GETCURSEL, 0, 0);
		CONST INT SIZE = 256;
		CHAR sz_buffer[SIZE]{};
		SendMessage(hListBox, LB_GETTEXT, i, (LPARAM)sz_buffer);
		SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)sz_buffer);
		SetFocus(hEdit);
		INT length = SendMessage(hEdit, WM_GETTEXTLENGTH, 0, 0); //получаем размер строки
		SendMessage(hEdit, EM_SETSEL, length, length); // Выделяем весь текст
		//SendMessage(hEdit, EM_REPLACESEL, 0, length);
	}

		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			HWND hEdit = GetDlgItem(hwnd, IDC_EDIT_ADD_ITEM);
			HWND hParent = GetParent(hwnd);
			HWND hListBox = GetDlgItem(hParent, IDC_LIST);
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE]{};
			SendMessage(hEdit, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
			INT i = SendMessage(hListBox, LB_GETCURSEL, 0, 0);
			SendMessage(hListBox, LB_DELETESTRING, i, 0);
			SendMessage(hListBox, LB_INSERTSTRING, i, (LPARAM)sz_buffer);

		}
			//break;  чтобы сразу закрыть окно
		case IDCANCEL: EndDialog(hwnd, 0); break;
		}
		break;
	case WM_CLOSE: EndDialog(hwnd, 0); break;
	}
	return FALSE;
}