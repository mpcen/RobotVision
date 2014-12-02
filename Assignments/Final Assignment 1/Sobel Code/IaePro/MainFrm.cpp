// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "IaePro.h"

#include "MainFrm.h"
#include "MyDropTarget.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_IMAGE_EXPLORATIONMODE, OnImageExplorationmode)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_EXPLORATIONMODE, OnUpdateImageExplorationmode)
	ON_COMMAND(ID_SIZE_TO_CONTENT, OnSizeToContent)
	ON_UPDATE_COMMAND_UI(ID_SIZE_TO_CONTENT, OnUpdateSizeToContent)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	m_bExplorationMode = TRUE;
	m_pDroptarget = new CMyDropTarget(this);
}

CMainFrame::~CMainFrame()
{
	delete m_pDroptarget;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
/*	if (!m_wndToolBar.Create(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
*/
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	// Get settings

	m_bExplorationMode = AfxGetApp()->GetProfileInt("Settings", "ExplorationMode", TRUE);

	m_pDroptarget->Register(this);//AHS

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CMDIFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::OnImageExplorationmode() 
{
	m_bExplorationMode = !m_bExplorationMode;

	AfxGetApp()->WriteProfileInt("Settings", "ExplorationMode", m_bExplorationMode);
}

void CMainFrame::OnUpdateImageExplorationmode(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bExplorationMode);
}


BOOL CMainFrame::OnDrop(COleDataObject* pDataObject,
                           DROPEFFECT      dropEffect,
                           CPoint point) 
{
HGLOBAL hg;
HDROP   hdrop;

//, 
    // Get the HDROP data from the data object.
    hg = pDataObject->GetGlobalData ( CF_HDROP );
    
    if ( NULL == hg )
        return FALSE;

    hdrop = (HDROP) GlobalLock ( hg );

    if ( NULL == hdrop )
        {
        GlobalUnlock ( hg );
        return FALSE;
        }

    // Read in the list of files here...

    UINT  uNumFiles = DragQueryFile ( hdrop, -1, NULL, 0 );
	TCHAR	szNextFile [MAX_PATH];

    for ( UINT uFile = 0; uFile < uNumFiles; uFile++ ) {
        // Get the next filename from the HDROP info.

        if ( DragQueryFile ( hdrop, uFile, szNextFile, MAX_PATH ) > 0 ){
			AfxGetApp()->OpenDocumentFile(szNextFile);
		}
	
	}	
	
	
	
	
	
	GlobalUnlock ( hg );

	return TRUE;
}

DROPEFFECT CMainFrame::OnDragEnter(COleDataObject* pDataObject,
                                      DWORD           dwKeyState,
                                      CPoint point) 
{
	return DROPEFFECT_COPY;
}



DROPEFFECT CMainFrame::OnDragOver(COleDataObject* pDataObject,
                                     DWORD           dwKeyState,
                                     CPoint          point) 
{
	return DROPEFFECT_COPY;
}


// CMainFrame::OnWindowCommand() is a menu command handler for
// CMainFrame class, which is a CMDIFrameWnd-derived
// class. It handles menu commands for the Windows pop-up menu.

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{   
   switch (LOWORD(wParam))
   {
   case ID_WINDOW_ARRANGE:   // For Window\Arrange Icons menu item, arrange
      MDIIconArrange();     // all minimized document child windows.
      return TRUE;

   case ID_WINDOW_CASCADE:  // For Window\Cascade menu item, arrange
      MDICascade();         // all the MDI child windows in a cascade format.
      return TRUE;

   case ID_WINDOW_TILE_HORZ:       // For Window\Tile Horizontal menu item, 
      MDITile(MDITILE_HORIZONTAL); // tile MDI child windows so that
      return TRUE;

   case ID_WINDOW_TILE_VERT:       // For Window\Tile Vertical menu item, 
      MDITile(MDITILE_VERTICAL);   // tile MDI child windows so that
      return TRUE;
      
   }

   return CMDIFrameWnd::OnCommand(wParam, lParam);
}

void CMainFrame::OnSizeToContent() 
{
	//Pass on the Message to all active Client Views.
	CWnd* pClientWnd = CWnd::FromHandle(m_hWndMDIClient);

	pClientWnd->SendMessageToDescendants(ID_SIZE_TO_CONTENT);
}

void CMainFrame::OnUpdateSizeToContent(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();	// Should detect if there are any views	
}
