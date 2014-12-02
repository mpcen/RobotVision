// HistogramDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IaePro.h"
#include "HistogramDlg.h"

#include "PixelOffset.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define HIST_REFBARSIZE 10

/////////////////////////////////////////////////////////////////////////////
// CHistogramDlg dialog


CHistogramDlg::CHistogramDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHistogramDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHistogramDlg)
	m_iType = 0;
	//}}AFX_DATA_INIT

	m_pImage = NULL;
}


void CHistogramDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHistogramDlg)
	DDX_Control(pDX, IDC_HISTOGRAM, m_cHistogram);
	DDX_CBIndex(pDX, IDC_HISTOTYPE, m_iType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHistogramDlg, CDialog)
	//{{AFX_MSG_MAP(CHistogramDlg)
	ON_CBN_SELCHANGE(IDC_HISTOTYPE, OnSelchangeHistotype)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHistogramDlg message handlers

BOOL CHistogramDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	return TRUE;
}

void CHistogramDlg::SetImage(CDefaultImage *pImage)
{
	ASSERT(pImage->IsValid());
	CWaitCursor zzz;

	m_cHistogram.SetImage(pImage);
}

void CHistogramDlg::OnSelchangeHistotype() 
{
	UpdateData();

	EIaeHistogram histogram;

	switch (m_iType) {
		case 0: histogram = eiaeluminosity; break;
		case 1: histogram = eiaeredChannel; break;
		case 2: histogram = eiaegreenChannel; break;
		case 3: histogram = eiaeblueChannel; break;
		case 4: histogram = eiaeallChannels; break;
	}

	m_cHistogram.SetType(histogram);
}
