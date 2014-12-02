// CustomConvDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IaePro.h"
#include "CustomConvDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCustomConvDlg dialog


CCustomConvDlg::CCustomConvDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCustomConvDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCustomConvDlg)
	m_szKernel = _T("");
	//}}AFX_DATA_INIT

	m_cCourier.CreatePointFont(100, "Courier New");
}


void CCustomConvDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCustomConvDlg)
	DDX_Control(pDX, IDC_KERNELTEXT, m_cKernelInfo);
	DDX_Control(pDX, IDC_KERNELCOMBO, m_cKernelCombo);
	DDX_Text(pDX, IDC_KERNELTEXT, m_szKernel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCustomConvDlg, CDialog)
	//{{AFX_MSG_MAP(CCustomConvDlg)
	ON_CBN_EDITCHANGE(IDC_KERNELCOMBO, OnEditchangeKernelcombo)
	ON_CBN_SELENDOK(IDC_KERNELCOMBO, OnSelendokKernelcombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCustomConvDlg message handlers

BOOL CCustomConvDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	TCHAR	szPath[_MAX_FNAME];
	::GetModuleFileName(NULL, szPath, _MAX_FNAME);

	TCHAR *exepos = _tcsrchr(szPath, '\\');
	if (exepos) exepos[0] = '\0';	

	CString str = szPath;
	m_szPath = str + "\\kernels\\";						// Store
	str += "\\kernels\\*.iak";

	m_cKernelCombo.Dir(0, str);			// Populate the combobox with kernel files.
	m_cKernelInfo.SetFont(&m_cCourier);

	return TRUE;
}

void CCustomConvDlg::OnEditchangeKernelcombo() 
{
	// TODO: Add your control notification handler code here
	
}

BOOL CCustomConvDlg::ReadKernel(const TCHAR *szFilename)
{
	using namespace std;

	if (_tcsicmp(szFilename, "") == 0 || szFilename == NULL) return FALSE;

	ifstream kernel(szFilename);
	if (!kernel) return FALSE;

	TCHAR	tchar;
	int		tint;
	bool	tbool = true;
	TCHAR	line[1024];

	// Skip over any initial comments
	while (tbool) {
		kernel.get(tchar);
		if (tchar == '#') {
			kernel.getline(line, 1024);
			continue;
		}

		tbool = false;
	}

	kernel >> tint;

	if (tint > 15 || (tint % 2 == 0)) return FALSE;

	m_iRadius = tint;
	m_iMagnitude = m_iRadius * m_iRadius;
	m_pdKernel = new double[m_iMagnitude];

	for (int i=0; i<m_iMagnitude; i++) {
		kernel >> tint;

		m_pdKernel[i] = tint;
	}

	kernel >> tint;
	m_iDivide = tint;

	m_bKernelLoaded = true;

	return TRUE;
}

void CCustomConvDlg::OnSelendokKernelcombo() 
{
	CString szFilename;
	
	m_cKernelCombo.GetWindowText(szFilename);
	m_bKernelLoaded = FALSE;

	ReadKernel(m_szPath + szFilename);

	if (m_bKernelLoaded) {
		int tempi = 0;
		CString temp = "";

		m_szKernel = "";

		for (int i=0; i<m_iRadius; i++) {
			for (int j=0; j<m_iRadius; j++) {
				temp.Format("%+.2f ", m_pdKernel[tempi++]);

				m_szKernel += temp;
			}

			m_szKernel += "\r\n";
		}

		UpdateData(FALSE);
	} else {
		m_szKernel = "Kernel loading failed!";

		UpdateData(FALSE);
	}
}

void CCustomConvDlg::RetrieveInfo(IaeKernel &iaeK)
{
	iaeK.pdKernel = m_pdKernel;
	iaeK.dDivide  = m_iDivide;
	iaeK.uRadius  = m_iRadius;
}
