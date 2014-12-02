
/////////////////////////////////////////////////////////////////////
// Class:			CHistogramDlg
// Author:			James Matthews
// Date:			17th February, 2002.
//
// The histogram dialog. This demonstrates most of the IaeHistogram
// control capabilities.
//

#ifndef _AFX_HISTOGRAMDLG_H_
#define _AFX_HISTOGRAMDLG_H_

#if !VIS_OS_IS_NT
#include <VisCore.h>
#include <VisDisplay.h>
#endif // !VIS_OS_IS_NT

#include "IaeHistogram.h"

class CHistogramDlg : public CDialog
{
public:
	CHistogramDlg(CWnd* pParent = NULL);

	void	SetImage(CDefaultImage *);

	//{{AFX_DATA(CHistogramDlg)
	enum { IDD = IDD_HISTOGRAM };
	CIaeHistogram	m_cHistogram;
	int		m_iType;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CHistogramDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	CDefaultImage	*m_pImage;
	
	//{{AFX_MSG(CHistogramDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeHistotype();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}

#endif
