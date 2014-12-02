
#if !defined(AFX_STATSDLG_H__CE7E5D40_24A5_11D6_887A_00304F10C7F8__INCLUDED_)
#define AFX_STATSDLG_H__CE7E5D40_24A5_11D6_887A_00304F10C7F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StatsDlg.h : header file
//

#include "QuickFilters.h"

class CStatsDlg : public CDialog
{
// Construction
public:
	void SetStats(Statistics *, StdDev *);
	CStatsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CStatsDlg)
	enum { IDD = IDD_STATISTICS };
	CString	m_pszDimensions;
	CString	m_pszMean;
	CString	m_pszMinMax;
	CString	m_pszPixels;
	CString	m_pszStdDev;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStatsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	Statistics	*m_pStatistics;
	StdDev		*m_pStdDev;

	//{{AFX_MSG(CStatsDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATSDLG_H__CE7E5D40_24A5_11D6_887A_00304F10C7F8__INCLUDED_)
