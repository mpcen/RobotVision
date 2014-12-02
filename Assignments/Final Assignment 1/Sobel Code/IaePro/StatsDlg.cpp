// StatsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IaePro.h"
#include "StatsDlg.h"

#include "QuickFilters.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatsDlg dialog


CStatsDlg::CStatsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStatsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStatsDlg)
	m_pszDimensions = _T("-");
	m_pszMean = _T("-");
	m_pszMinMax = _T("-");
	m_pszPixels = _T("-");
	m_pszStdDev = _T("-");
	//}}AFX_DATA_INIT

	m_pStatistics = NULL;
	m_pStdDev = NULL;
}


void CStatsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatsDlg)
	DDX_Text(pDX, IDC_DIMENSIONS, m_pszDimensions);
	DDX_Text(pDX, IDC_MEAN, m_pszMean);
	DDX_Text(pDX, IDC_MINMAX, m_pszMinMax);
	DDX_Text(pDX, IDC_PIXELS, m_pszPixels);
	DDX_Text(pDX, IDC_STDDEV, m_pszStdDev);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStatsDlg, CDialog)
	//{{AFX_MSG_MAP(CStatsDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatsDlg message handlers

BOOL CStatsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	ASSERT(m_pStatistics && m_pStdDev);

	CString format;

	format.Format("%d x %d", m_pStatistics->x, m_pStatistics->y);
	m_pszDimensions = format;

	format.Format("%.3f", m_pStatistics->mean);
	m_pszMean = format;

	format.Format("%d/%d", m_pStatistics->min, m_pStatistics->max);
	m_pszMinMax = format;

	format.Format("%d", m_pStatistics->pixels);
	m_pszPixels = format;
	
	format.Format("%.3f", m_pStdDev->stddev);
	m_pszStdDev = format;
	
	UpdateData(FALSE);

	return TRUE;
}

void CStatsDlg::SetStats(Statistics *ps, StdDev *psd)
{
	m_pStatistics = ps;
	m_pStdDev = psd;
}
