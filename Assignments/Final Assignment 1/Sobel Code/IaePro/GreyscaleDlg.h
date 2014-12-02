#if !defined(AFX_GREYSCALEDLG_H__CD81A601_E6C3_4DCC_8934_83E8B495A438__INCLUDED_)
#define AFX_GREYSCALEDLG_H__CD81A601_E6C3_4DCC_8934_83E8B495A438__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GreyscaleDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGreyscaleDlg dialog

class CGreyscaleDlg : public CDialog
{
// Construction
public:
	CGreyscaleDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGreyscaleDlg)
	enum { IDD = IDD_GREYSCALE };
	int		m_iGreyscale;
	//}}AFX_DATA

	int GetGreyscale();

	//{{AFX_VIRTUAL(CGreyscaleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:

	// Generated message map functions
	//{{AFX_MSG(CGreyscaleDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GREYSCALEDLG_H__CD81A601_E6C3_4DCC_8934_83E8B495A438__INCLUDED_)
