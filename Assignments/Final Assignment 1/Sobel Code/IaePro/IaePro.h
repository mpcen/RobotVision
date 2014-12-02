// IaePro.h : main header file for the IAEPRO application
//

#if !defined(AFX_IAEPRO_H__B6281B07_11DB_11D6_887A_00304F10C7F8__INCLUDED_)
#define AFX_IAEPRO_H__B6281B07_11DB_11D6_887A_00304F10C7F8__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CIaeProApp:
// See IaePro.cpp for the implementation of this class
//

class CIaeProApp : public CWinApp
{
public:
	CDocument *CreateNewDoc();
	CIaeProApp();
	// This is the connection to the primary device
	// for getting live images

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIaeProApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	COleTemplateServer m_server;
	CVisMultiDocTemplate *m_pVisDocTemplate;
		// Server object for document creation

	//{{AFX_MSG(CIaeProApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IAEPRO_H__B6281B07_11DB_11D6_887A_00304F10C7F8__INCLUDED_)
