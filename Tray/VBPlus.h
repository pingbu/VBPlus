// VBPlus.h : main header file for the VBPlus application
//

#if !defined(AFX_VBPLUS_H__D9C27392_3E5D_437B_8EE3_6E9519D51BF0__INCLUDED_)
#define AFX_VBPLUS_H__D9C27392_3E5D_437B_8EE3_6E9519D51BF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CVBPlusApp:
// See VBPlus.cpp for the implementation of this class
//

class CVBPlusApp : public CWinApp
{
public:
	CVBPlusApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVBPlusApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

public:
	//{{AFX_MSG(CVBPlusApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VBPLUS_H__D9C27392_3E5D_437B_8EE3_6E9519D51BF0__INCLUDED_)
