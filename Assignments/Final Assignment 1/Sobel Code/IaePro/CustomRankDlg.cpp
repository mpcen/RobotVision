// CustomRankDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IaePro.h"
#include "CustomRankDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCustomRankDlg dialog

static iNeighbourData[] = { 3, 5, 7, 11, 15, 25 };

CCustomRankDlg::CCustomRankDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCustomRankDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCustomRankDlg)
	m_iNeighbourhood = -1;
	m_iRank = -1;
	//}}AFX_DATA_INIT
}


void CCustomRankDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCustomRankDlg)
	DDX_Control(pDX, IDC_RANK, m_cRank);
	DDX_CBIndex(pDX, IDC_NEIGHBOURHOOD, m_iNeighbourhood);
	DDX_CBIndex(pDX, IDC_RANK, m_iRank);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCustomRankDlg, CDialog)
	//{{AFX_MSG_MAP(CCustomRankDlg)
	ON_CBN_CLOSEUP(IDC_NEIGHBOURHOOD, OnCloseupNeighbourhood)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCustomRankDlg message handlers

void CCustomRankDlg::OnCloseupNeighbourhood() 
{
	UpdateData();
	
	CString str;
	int totalpx = iNeighbourData[m_iNeighbourhood] * iNeighbourData[m_iNeighbourhood];
	int median = totalpx / 2;

	m_cRank.ResetContent();

	for (int i=0; i<totalpx; i++) {
		str.Format("%d", i+1);

		if (i == median) str += " (Median)";

		m_cRank.AddString(str);
	}

	m_iRank = median;

	UpdateData(FALSE);
}

void CCustomRankDlg::GetRankInfo(int &n, int &r)
{
//	UpdateData();

	n = iNeighbourData[m_iNeighbourhood] / 2;	// FIXME: ...if you change Rank stuff.
	r = m_iRank;
}

void CCustomRankDlg::OnOK() 
{
	UpdateData();
	
	CDialog::OnOK();
}
