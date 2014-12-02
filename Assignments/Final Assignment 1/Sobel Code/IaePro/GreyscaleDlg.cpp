// GreyscaleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "iaepro.h"
#include "GreyscaleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGreyscaleDlg dialog


CGreyscaleDlg::CGreyscaleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGreyscaleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGreyscaleDlg)
	m_iGreyscale = 0;
	//}}AFX_DATA_INIT
}


void CGreyscaleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGreyscaleDlg)
	DDX_CBIndex(pDX, IDC_GREYSCALE, m_iGreyscale);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGreyscaleDlg, CDialog)
	//{{AFX_MSG_MAP(CGreyscaleDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGreyscaleDlg message handlers

int CGreyscaleDlg::GetGreyscale()
{
	return m_iGreyscale;
}
