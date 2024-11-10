#define _CRT_SECURE_NO_WARNINGS  // ��������� ��� ������ �4996
#include<Windows.h>
#include<cstdio>  // ����� ������� sprintf(...)
#include<cstring>
#include"resource.h"

CONST CHAR* init_values[] = { "������", "This", "is", "my", "first", "Combo", "Box" };

BOOL CALLBACK DlgProc(HWND nwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK InputDlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
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
		HWND hList = GetDlgItem(hwnd, IDC_LIST1);    // ������� GetDlgItem() ���������� HWND ���������� �������� ����
		for (int i = 0; i < sizeof(init_values) / sizeof(init_values[0]); i++)
		{
			SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)init_values[i]);
		}
	}
	break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			HWND hListBox = GetDlgItem(hwnd, IDC_LIST1);
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE]{};
			CHAR sz_message[SIZE]{};
			INT i = SendMessage(hListBox, LB_GETCURSEL, 0, 0);
			// ��������� LB_GETCURSEL (Current Selection) ���������� ����� ���������� ������ ListBox
			SendMessage(hListBox, LB_GETTEXT, i, (LPARAM)sz_buffer);
			sprintf(sz_message, "�� ������� ����� N%i �� ��������� \"%s\".", i, sz_buffer);
			MessageBox(hwnd, sz_message, "Selected value", MB_OK | MB_ICONINFORMATION);
			break;
		}
		case IDC_BUTTON1:
		{
			// CHAR sz_newItem[256] = "";
				HWND hListBox = GetDlgItem(hwnd, IDC_LIST1);
			if (DialogBoxParam(GetModuleHandle(NULL),  //  GetModuleHandle(Null) - ������� HINSTANCE �������� ������ � DialogBoxParam, ����� ������� �����, ��� ������ ������� ��� ����������� ���� (� ������ ������ IDD_DIALOG2), ����������� � resource.h
				MAKEINTRESOURCE(IDD_DIALOG2), 
				hwnd, InputDlgProc, 
				(LPARAM)hListBox)==IDOK) //sz_newItem) == IDOK)
			{

				//if (sz_newItem[0] != "\0")
			/*	if (strlen(sz_newItem)>0)
				{
					SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)sz_newItem);
				}*/
			}
				break;
		}
		case IDC_BUTTON2: 
		{
			HWND hListBox = GetDlgItem(hwnd, IDC_LIST1);
			INT i = SendMessage(hListBox, LB_GETCURSEL, 0, 0);
			if (i != LB_ERR)
			{
				SendMessage(hListBox, LB_DELETESTRING, i, 0);
			}
			else
			{
				MessageBox(hwnd, "�������� ������� ��� ��������", "������", MB_OK | MB_ICONERROR);
			}
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
BOOL CALLBACK InputDlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hListBox; 
	CHAR sz_buffer[256] = "";
	switch (uMsg)
	{
	case WM_INITDIALOG:
		hListBox = (HWND)lParam;
		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			int count = SendMessage(hListBox, LB_GETCOUNT, 0, 0);
			// ��������� ��������� ����� � ��������� � lParam ��� �������� � �������� ����
			//CHAR* buffer = (CHAR*)lParam;
			GetDlgItemText(hwnd, IDC_EDIT1, sz_buffer, 256);
			if (strlen(sz_buffer) > 0)
			{
				SendMessage(hListBox, LB_INSERTSTRING, (WPARAM)count/*��������� � ����� ������*/, (LPARAM)sz_buffer);
			}
			EndDialog(hwnd, IDOK);
			return TRUE;
		}
		break;
		case IDCANCEL:
			EndDialog(hwnd, IDCANCEL);
			break;
		}
		break;
	}
	return FALSE;
}