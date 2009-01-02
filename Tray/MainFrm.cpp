// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "VBPlus.h"
#include "Hook.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_NOTIFYICON	(WM_USER + 1)

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_NOTIFYICON, OnNotifyIcon)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: Add your specialized creation code here
	SetIcon(LoadIcon(AfxGetInstanceHandle(), (LPCTSTR)IDR_MAINFRAME), TRUE);
	GetMenu()->GetSubMenu(0)->SetDefaultItem(ID_APP_ABOUT);

	InstallHook();

	NOTIFYICONDATA nid;
	nid.cbSize = sizeof(nid);
	nid.hWnd = *this;
	nid.uID = 0;
	nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	nid.uCallbackMessage = WM_NOTIFYICON;
	nid.hIcon = GetIcon(TRUE);
	lstrcpy(nid.szTip, TEXT("VB Plus!"));
	Shell_NotifyIcon(NIM_ADD, &nid);

	return 0;
}

void CMainFrame::OnDestroy() 
{
	CFrameWnd::OnDestroy();

	// TODO: Add your message handler code here
	NOTIFYICONDATA nid;
	nid.cbSize = sizeof(nid);
	nid.hWnd = *this;
	nid.uID = 0;
	nid.uFlags = 0;
	Shell_NotifyIcon(NIM_DELETE, &nid);

	UninstallHook();
}

LRESULT CMainFrame::OnNotifyIcon(WPARAM wParam, LPARAM lParam)
{
	switch ( lParam )
	{
	case WM_LBUTTONDBLCLK:
		SetForegroundWindow();
		SendMessage(WM_COMMAND, ID_APP_ABOUT);
		break;

	case WM_RBUTTONUP:
		SetForegroundWindow();
		CPoint pt;
		GetCursorPos(&pt);
		GetMenu()->GetSubMenu(0)->TrackPopupMenu(0, pt.x, pt.y, this);
		break;
	}

	return 0;
}
