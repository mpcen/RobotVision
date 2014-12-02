
/////////////////////////////////////////////////////////////////////
// Class:			CCustomConvDlg
// Author:			James Matthews
// Date:			17th February, 2002.
//
// Dialog for the custom convolution dialog.
//

#ifndef _AFX_CUSTOMCONVDLG_H_
#define _AFX_CUSTOMCONVDLG_H_

#include "IaeProConvolution.h"

class CCustomConvDlg : public CDialog
{
public:
	void RetrieveInfo(IaeKernel &);
	CString m_szPath;
	CCustomConvDlg(CWnd* pParent = NULL);   // standard constructor

	//{{AFX_DATA(CCustomConvDlg)
	enum { IDD = IDD_CUSTOM_CONVOLUTION };
	CEdit	m_cKernelInfo;
	CComboBox	m_cKernelCombo;
	CString	m_szKernel;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CCustomConvDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	UINT	m_iRadius;
	int		m_iDivide;
	int		m_iMagnitude;
	double *m_pdKernel;

	BOOL	m_bKernelLoaded;

	CFont	m_cCourier;

	BOOL ReadKernel(const TCHAR *szFilename);

	//{{AFX_MSG(CCustomConvDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnEditchangeKernelcombo();
	afx_msg void OnSelendokKernelcombo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif
