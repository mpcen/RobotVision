
/////////////////////////////////////////////////////////////////////
// Class:			CCustomRankDlg
// Author:			James Matthews
// Date:			17th February, 2002.
//
// Dialog for the custom rank filter.
//

#ifndef _AFX_CUSTOMRANKDLG_H_
#define _AFX_CUSTOMRANKDLG_H_

class CCustomRankDlg : public CDialog
{
public:
	CCustomRankDlg(CWnd* pParent = NULL);   // standard constructor

	void GetRankInfo(int &, int &);

	//{{AFX_DATA(CCustomRankDlg)
	enum { IDD = IDD_CUSTOM_RANK };
	CComboBox	m_cRank;
	int		m_iNeighbourhood;
	int		m_iRank;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CCustomRankDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CCustomRankDlg)
	afx_msg void OnCloseupNeighbourhood();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}

#endif
