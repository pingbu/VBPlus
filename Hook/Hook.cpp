#define _WIN32_WINDOWS 0x0500

#include <windows.h>
#include "Hook.h"


HMODULE g_hModule;

HHOOK g_hCallWndProcHook;
HHOOK g_hMouseHook;

DWORD g_dwTlsActiveScroll;

int g_nWheelScrollLines;


BOOL WINAPI DllMain(
	HINSTANCE hinstDLL,  // handle to DLL module
	DWORD fdwReason,     // reason for calling function
	LPVOID lpvReserved   // reserved
)
{
	if ( DLL_PROCESS_ATTACH == fdwReason )
	{
		g_hModule = hinstDLL;
		g_dwTlsActiveScroll = TlsAlloc();

		DWORD dwType;
		TCHAR szData[16];
		DWORD cbData = sizeof(szData);

		HKEY hKey = NULL;
		RegOpenKey(HKEY_CURRENT_USER, TEXT("Control Panel\\Desktop"), &hKey);
		RegQueryValueEx(hKey, TEXT("WheelScrollLines"), NULL, &dwType, (LPBYTE)szData, &cbData);
		RegCloseKey(hKey);

		g_nWheelScrollLines = atoi(szData);
	}
	else if ( DLL_PROCESS_DETACH == fdwReason )
	{
		TlsFree(g_dwTlsActiveScroll);
	}

	return TRUE;
}

LRESULT CALLBACK CallWndProc(
	int nCode,      // hook code
	WPARAM wParam,  // current-process flag
	LPARAM lParam   // address of structure with message data
)
{
	LRESULT lResult = CallNextHookEx(g_hMouseHook, nCode, wParam, lParam);

	LPCWPSTRUCT lpCWP = (LPCWPSTRUCT)lParam;

	if ( nCode >= 0 && SBM_SETSCROLLINFO == lpCWP->message )
	{
		TCHAR szClassName[MAX_PATH];
		GetClassName(lpCWP->hwnd, szClassName, MAX_PATH);

		if ( 0 == lstrcmpi(szClassName, TEXT("ScrollBar")) )
		{
			GetClassName(GetParent(lpCWP->hwnd), szClassName, MAX_PATH);

			if ( 0 == lstrcmpi(szClassName, TEXT("VbaWindow")) )
				if ( SBS_VERT & GetWindowLong(lpCWP->hwnd, GWL_STYLE) )
					TlsSetValue(g_dwTlsActiveScroll, lpCWP->hwnd);
		}
	}

	return lResult;
}

typedef struct {
	MOUSEHOOKSTRUCT MOUSEHOOKSTRUCT;
	DWORD mouseData;
} MOUSEHOOKSTRUCTEX, *PMOUSEHOOKSTRUCTEX;

LRESULT CALLBACK MouseProc(
	int nCode,      // hook code
	WPARAM wParam,  // message identifier
	LPARAM lParam   // mouse coordinates
)
{
	LRESULT lResult = CallNextHookEx(g_hMouseHook, nCode, wParam, lParam);

	if ( nCode >= 0 && WM_MOUSEWHEEL == wParam )
	{
		LPMOUSEHOOKSTRUCT lpMouse = (LPMOUSEHOOKSTRUCT)lParam;

		TCHAR szClassName[MAX_PATH];
		GetClassName(lpMouse->hwnd, szClassName, MAX_PATH);

		if ( 0 == lstrcmpi(szClassName, TEXT("VbaWindow")) )
		{
			PMOUSEHOOKSTRUCTEX lpMouseEx = (PMOUSEHOOKSTRUCTEX)lParam;
			short zDelta = (short)HIWORD(lpMouseEx->mouseData) / 120;
			lParam = (LPARAM)TlsGetValue(g_dwTlsActiveScroll);

			if ( g_nWheelScrollLines > 0 )
			{
				zDelta *= g_nWheelScrollLines;

				if ( zDelta < 0 ) while ( zDelta ++ )
					SendMessage(lpMouse->hwnd, WM_VSCROLL, MAKEWORD(SB_LINEDOWN, 0), lParam);
				else while ( zDelta -- )
					SendMessage(lpMouse->hwnd, WM_VSCROLL, MAKEWORD(SB_LINEUP, 0), lParam);
			}
			else
			{
				if ( zDelta < 0 ) while ( zDelta ++ )
					SendMessage(lpMouse->hwnd, WM_VSCROLL, MAKEWORD(SB_PAGEDOWN, 0), lParam);
				else while ( zDelta -- )
					SendMessage(lpMouse->hwnd, WM_VSCROLL, MAKEWORD(SB_PAGEUP, 0), lParam);
			}
		}
	}

	return lResult;
}

void InstallHook()
{
	g_hCallWndProcHook = SetWindowsHookEx(WH_CALLWNDPROC, CallWndProc, g_hModule, NULL);
	g_hMouseHook = SetWindowsHookEx(WH_MOUSE, MouseProc, g_hModule, NULL);
}

void UninstallHook()
{
	UnhookWindowsHookEx(g_hMouseHook);
	UnhookWindowsHookEx(g_hCallWndProcHook);
}
