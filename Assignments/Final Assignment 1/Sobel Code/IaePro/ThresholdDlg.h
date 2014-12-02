#if !defined(AFX_THRESHOLDDLG_H__3ACE0E80_1A8A_11D6_887A_00304F10C7F8__INCLUDED_)
#define AFX_THRESHOLDDLG_H__3ACE0E80_1A8A_11D6_887A_00304F10C7F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ThresholdDlg.h : header file
//

#include "IaeProDoc.h"
#include "IaeHistogram.h"

#if !VIS_OS_IS_NT
#include <VisCore.h>
#include <VisDisplay.h>
#endif // !VIS_OS_IS_NT

/////////////////////////////////////////////////////////////////////////////
// CThresholdDlg dialog

class CThresholdDlg : public CDialog
{
// Construction
public:
	CThresholdDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CThresholdDlg)
	enum { IDD = IDD_THRESHOLD };
	CIaeHistogram	m_cHistogram;
	CSliderCtrl	m_cThreshSlider;
	int		m_iThreshold;
	//}}AFX_DATA

	void SetImage(CDefaultImage *pImage) { m_cBuffer.Copy(*pImage); m_pImage = pImage; }
	void SetDocument(CIaeProDoc *pDoc) { m_pDocument = pDoc; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThresholdDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDefaultImage *m_pImage;
	CDefaultImage m_cBuffer;

	CIaeProDoc *m_pDocument;

	// Generated message map functions
	//{{AFX_MSG(CThresholdDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnReleasedcaptureThreshslider(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_THRESHOLDDLG_H__3ACE0E80_1A8A_11D6_887A_00304F10C7F8__INCLUDED_)
