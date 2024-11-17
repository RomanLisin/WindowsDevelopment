#define _CRT_SECURE_NO_WARNINGS  // ��������� ��� ������ �4996
#include<Windows.h>
#include"resource.h"
#include<cstdio>
//#include<debugapi.h>


CONST CHAR* g_VALUES[] = { "This","is","my", "first","List","Box" };

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgProcAddItem(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//BOOL CALLBACK DlgProcListEdit(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
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
			SetFocus(hListBox);
		}
		//break;
		/*case WM_SETFOCUS:
		{
			
		}
			break;*/
		//case WM_KEYDOWN:
		//{
		//	HWND hCurrentFocus = GetFocus();
		//	if (hCurrentFocus == GetDlgItem(hwnd, IDC_LIST))
		//	{   
		//		switch (HIWORD(wParam))
		//		{
		//			/*int code = (int)wParam;
		//			HDC hdc = GetDC(hwnd);*/
		//			case VK_RETURN:
		//			{
		//				HWND hListBox = GetDlgItem(hwnd, IDC_LIST);
		//				INT i = SendMessage(hListBox, LB_GETCURSEL, 0, 0);
		//				if (i != LB_ERR)
		//				{
		//					SendMessage(hListBox, LB_DELETESTRING, i, 0);
		//				}
		//				break;
		//			}
		//				// 3) �������� ���������� ListBox:
		//			//HWND hListBox = GetDlgItem(hParent, IDC_LIST);
		//			case VK_DELETE:
		//			{
		//																// 2) �������� ������������ ����
		//																//HWND hParent = GetParent(hwnd);
		//				HWND hListBox = GetDlgItem(hwnd, IDC_LIST);
		//				INT i = SendMessage(hListBox, LB_GETCURSEL, 0, 0);
		//				if(i != LB_ERR)
		//				{
		//					SendMessage(hListBox, LB_DELETESTRING, i, 0);
		//				}
		//				//return DLGC_WANTALLKEYS;
		//			//MessageBox(hwnd, "Delete pressed!", "Debug", MB_OK);
		//			return TRUE;
		//			}
		//		}
		///*wchar_t buffer[80];
		//swprintf(buffer, 80, L"���: %d        ", code, code);
		//TextOut(hdc,40, 160, (LPCSTR)buffer, (int)wcslen(buffer));
		//ReleaseDC(hwnd, hdc);*/
		//	}
		//}

//**/**/**/**/  �.3) ����������� ������ �������
		case WM_COMMAND:
		switch (LOWORD(wParam))
		{
			case IDC_BUTTON_REMOVE:
			{
				HWND hListBox = GetDlgItem(hwnd, IDC_LIST);
				INT i = SendMessage(hListBox, LB_GETCURSEL, 0, 0);
				SendMessage(hListBox, LB_DELETESTRING, i, 0);
			} // DONE
			break;
			/*case LBN_SETFOCUS:
			{
				HWND hFocus = (HWND)wParam;
				if (hFocus == GetDlgItem(hwnd, IDC_LIST))
				{
					OutputDebugString("ListBox ������� �����\n");
					MessageBox(hwnd, "ListBox ������� �����", "Info", MB_OK | MB_ICONINFORMATION);
				}
			}
			break;*/
			/*case LBN_DBLCLK:
			{
				HWND hListBox = GetDlgItem(hwnd, IDC_LIST);
				INT i = SendMessage(hListBox, LB_GETCURSEL, 0, 0);
				if (i != LB_ERR)
				{
					CONST INT SIZE = 256;
					CHAR sz_buffer[SIZE]{};
					SendMessage(hListBox, LB_GETTEXT, i, (LPARAM)sz_buffer);

					CHAR sz_message[SIZE]{};
					sprintf(sz_message, "�� ������� �������: \"%s\".", sz_buffer);
					MessageBox(hwnd, sz_message, "Double Click", MB_OK | MB_ICONINFORMATION);
				}
			}
			break;*/
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
			case IDCANCEL: EndDialog(hwnd, 0);
				break;
		}
			
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

				INT index = SendMessage(hListBox, LB_FINDSTRING, -1, (LPARAM)sz_buffer);
				//**/**/**/**/ �.1) ��������� ����������� ���������� ���������� ��������
				if (index == LB_ERR)

					// 4) ��������� ����� � ListBox:
					SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)sz_buffer);

			}
		//break;  // �������, ���� �����, ����� ���������� ������ ���� �������
		case IDCANCEL: EndDialog(hwnd, 0); break;
		}
		break;
	case WM_CLOSE: EndDialog(hwnd, 0);
		//break;
	}
	return FALSE;
}
// 2) �������� ������������ ����:
//HWND hParent = GetParent(hwnd);

// 3) �������� ���������� ListBox:
// HWND hListBox = GetDlgItem(hParent, IDC_LIST);
//BOOL CALLBACK DlgProcListEdit(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//	switch (uMsg)
//	{
//	case WM_KEYDOWN:
//		HWND hCurrentFocus = GetFocus();
//		switch (wParam)
//		{
//		case VK_DELETE:
//			break;
//		case VK_RETURN:
//			break;
//		}
//		break;
//	}
//	return FALSE;
//}