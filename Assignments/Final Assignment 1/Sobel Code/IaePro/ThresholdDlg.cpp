// ThresholdDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IaePro.h"
#include "ThresholdDlg.h"

#include "QuickFilters.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CThresholdDlg dialog


CThresholdDlg::CThresholdDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CThresholdDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CThresholdDlg)
	m_iThreshold = 0;
	//}}AFX_DATA_INIT
}


void CThresholdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CThresholdDlg)
	DDX_Control(pDX, IDC_HISTOGRAM, m_cHistogram);
	DDX_Control(pDX, IDC_THRESHSLIDER, m_cThreshSlider);
	DDX_Slider(pDX, IDC_THRESHSLIDER, m_iThreshold);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CThresholdDlg, CDialog)
	//{{AFX_MSG_MAP(CThresholdDlg)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_THRESHSLIDER, OnReleasedcaptureThreshslider)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CThresholdDlg message handlers

BOOL CThresholdDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_cThreshSlider.SetRange(0,255);
	m_cThreshSlider.SetPageSize(1);
	m_cThreshSlider.SetPos(128);

	ASSERT(m_pImage);

	m_cHistogram.SetImage(m_pImage);		// Set the image (histogram calculated).
	m_cHistogram.SetFlags(IAEH_NOREFBAR);	// Remove reference bar
	
	return TRUE;
}

void CThresholdDlg::OnReleasedcaptureThreshslider(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ASSERT(m_pImage);

	UpdateData();

	VisMap2(SimpleThreshold(m_iThreshold), m_cBuffer, *m_pImage);
	m_pDocument->UpdateAllViews(NULL);
	
	*pResult = 0;
}

void CThresholdDlg::OnCancel() 
{
	m_pImage->Copy(m_cBuffer);
	m_pDocument->UpdateAllViews(NULL);
	
	CDialog::OnCancel();
}
