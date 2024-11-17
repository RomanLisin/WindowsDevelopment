#define _CRT_SECURE_NO_WARNINGS  // ��������� ��� ������ �4996
#include<Windows.h>
#include"resource.h"
#include<cstdio>

CONST CHAR* g_VALUES[] = { "This","is","my", "first","List","Box" };


BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgProcAddItem(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK EditDialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void EditListBoxItem(HWND hwnd);
void DeleteListBoxItem(HWND hwnd);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	/*
	---------------------------
	WinMain
	hInstance - ��������� ������ ���������� � ������ (��������� *.exe-�����)
	hPrevInst - ���������� ���������� ��������� (Deprecated, ������������� � Windows 98).
	LPSTR lpCmdLine:
	LPSTR - LongPointer to String (��������� �� ������ ��������� const char*)
	lpCmdLine - ���������� ������ ������� ����������
	nCmdShow - ����� ����������� ����: �������� � ����, ���������� �� ���� �����, �������� � ����� �����......
	*/
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc, 0);
	return 0;

}
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
			{
				SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)g_VALUES[i]);
			}
			HWND hButtonAdd = GetDlgItem(hwnd, IDC_BUTTON_ADD);
			SetFocus(hButtonAdd);
			SendMessage(hwnd, DM_SETDEFID, IDC_BUTTON_ADD, 0); // ����� ������������� '��������' �� ���������
			//SetFocus(hListBox);
		}
		//break;
		case WM_COMMAND:

			switch (LOWORD(wParam))
			{
				case IDC_LIST:
				{
					if (HIWORD(wParam) == LBN_DBLCLK)
					{
						EditListBoxItem(hwnd);
					}
				}
					break;
					//**/**/**/**/  �.3) ����������� ������ �������
				case IDC_BUTTON_REMOVE: DeleteListBoxItem(hwnd);
				//{
				//	HWND hListBox = GetDlgItem(hwnd, IDC_LIST);
				//	INT i = SendMessage(hListBox, LB_GETCURSEL, 0, 0);
				//	SendMessage(hListBox, LB_DELETESTRING, i, 0);
				//} // DONE
				break;

				case IDC_BUTTON_ADD:
					DialogBoxParam(GetModuleHandle(NULL) /*�������� hinstance*/
						, MAKEINTRESOURCE(IDD_DIALOG_ADD_ITEM), hwnd, DlgProcAddItem, 0);
					break;

				case IDOK:
				{
					HWND hListBox = GetDlgItem(hwnd, IDC_LIST);
					CONST INT SIZE = 256;
					CHAR sz_buffer[SIZE]{};
					INT i = SendMessage(hListBox, LB_GETCURSEL, 0, 0);

					CHAR sz_message[SIZE]{};
					sprintf(sz_message, "�� ������� ������� N%i �� ��������� \"%s\".", i, sz_buffer);
					MessageBox(hwnd, sz_message, "Info", MB_OK | MB_ICONINFORMATION);
				}
				//break;  // ���� � ���, �� ������������� ���� ��������� �����������, ���� ��� , �� ����� ���������� ������ ����� �����������
				case IDCANCEL: EndDialog(hwnd, 0); break;
					break;
				case WM_CLOSE:
					EndDialog(hwnd, 0);
					break;
			}
		case WM_SYSKEYDOWN:
		{
				if (HIWORD(wParam) == VK_RETURN)
				{
					EditListBoxItem(hwnd);
					return TRUE;
				}
				else if (HIWORD(wParam) == VK_DELETE)
				{	
					DeleteListBoxItem(hwnd);
					return TRUE;
				}
				return 0;
			
		}
		case WM_CONTEXTMENU:
		{
			HWND hListBox = GetDlgItem(hwnd, IDC_LIST); // ������������� ������ ListBox
			if ((HWND)wParam == hListBox) { // ���������, ��� ������ ���� ��� � ListBox
				//// �������� ������ ���������� ��������
				//int selectedIndex = SendMessage(hListBox, LB_GETCURSEL, 0, 0);
				//if (selectedIndex != LB_ERR) { // ���������, ��� ������� ������
				//	// ������� ��������� �������
				//	SendMessage(hListBox, LB_DELETESTRING, selectedIndex, 0);
				//}
				DeleteListBoxItem(hwnd);
			}
			return 0;
		}
	}
	return FALSE;
}

void EditListBoxItem(HWND hwnd) 
{
	HWND hListBox = GetDlgItem(hwnd, IDC_LIST);
	int selIndex = SendMessage(hListBox, LB_GETCURSEL, 0, 0);

	if (selIndex != LB_ERR) {
		CHAR buffer[256] = { 0 };
		// �������� ����� ���������� ��������
		SendMessage(hListBox, LB_GETTEXT, selIndex, (LPARAM)buffer);

		// ���������� ������ ��������������
		if (DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_ADD_ITEM), hwnd, EditDialogProc, (LPARAM)buffer) == IDOK) 
		{
			//**/**/**/**/ �.1) ��������� ����������� ���������� ���������� ��������
			INT index = SendMessage(hListBox, LB_FINDSTRING, -1, (LPARAM)buffer);
			if (index == LB_ERR)
			{
				// ��������� ����� � ������
				//SendMessage(hListBox, LB_DELETESTRING, selectedIndex, 0);
				DeleteListBoxItem(hwnd);
				SendMessage(hListBox, LB_INSERTSTRING, selIndex, (LPARAM)buffer);
			}
		}
	}
}

void DeleteListBoxItem(HWND hwnd) {
	HWND hListBox = GetDlgItem(hwnd, IDC_LIST);
	int selIndex = SendMessage(hListBox, LB_GETCURSEL, 0, 0);

	if (selIndex != LB_ERR) {
		// ������� ��������� �������
		SendMessage(hListBox, LB_DELETESTRING, selIndex, 0);
	}
}

BOOL CALLBACK DlgProcAddItem(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:

//**/**/**/**/  �.2) ��� �������� ���� '�������� ��������' ����� ������ ���� �� ��������� ����;
	{
		HWND hListBox = GetDlgItem(hwnd, IDC_EDIT_ADD_ITEM);
		SetFocus(hListBox);
	}  // DONE

	break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			//1) ������ ����� �� EditControl:
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE]{};
			HWND hEdit = GetDlgItem(hwnd, IDC_EDIT_ADD_ITEM);
			SendMessage(hEdit, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);

			// 2) �������� ������������ ����:
			HWND hParent = GetParent(hwnd);

			// 3) �������� ���������� ListBox:
			HWND hListBox = GetDlgItem(hParent, IDC_LIST);

//**/**/**/**/ �.1) ��������� ����������� ���������� ���������� ��������
			INT index = SendMessage(hListBox, LB_FINDSTRING, -1, (LPARAM)sz_buffer);
			if (index == LB_ERR)
				// 4) ��������� ����� � ListBox:
				SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)sz_buffer);
		}
		//break;  // �������, ���� �����, ����� ���������� ������ ���� �������
		case IDCANCEL: EndDialog(hwnd, 0); break;
		}
		break;
	case WM_KEYDOWN:
	{
		// 2) �������� ������������ ����:
		HWND hParent = GetParent(hwnd);
		if (GetFocus() == GetDlgItem(hParent, IDC_LIST))
		{
			if (wParam == VK_RETURN)
			{
				EditListBoxItem(hwnd);
				return TRUE;
			}
			else if (wParam == VK_DELETE)
			{
				DeleteListBoxItem(hwnd);
				return TRUE;
			}
		}
	}
		break;
	case WM_CLOSE: EndDialog(hwnd, 0);
		//break;
	}
	return FALSE;
}
BOOL CALLBACK EditDialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
	static CHAR* editBuffer;

	switch (uMsg) 
	{
		case WM_INITDIALOG:
		{
			editBuffer = (CHAR*)lParam;
			SetDlgItemText(hwnd, IDC_EDIT_ADD_ITEM, editBuffer);
			return TRUE;
		}

		case WM_COMMAND:
		{
			if (LOWORD(wParam) == IDOK)
			{
				GetDlgItemText(hwnd, IDC_EDIT_ADD_ITEM, editBuffer, 256);
				EndDialog(hwnd, IDOK);
				return TRUE;
			}
			else if (LOWORD(wParam) == IDCANCEL) {
				EndDialog(hwnd, IDCANCEL);
				return TRUE;
			}
		}
		break;
	}
	return FALSE;
}