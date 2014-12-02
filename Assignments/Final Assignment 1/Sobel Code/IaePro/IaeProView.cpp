// IaeProView.cpp : implementation of the CIaeProView class
//

#include "stdafx.h"
#include "IaePro.h"


#include "IaeProDoc.h"
#include "IaeProView.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIaeProView

IMPLEMENT_DYNCREATE(CIaeProView, CScrollView)

BEGIN_MESSAGE_MAP(CIaeProView, CScrollView)
	//{{AFX_MSG_MAP(CIaeProView)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_CREATE()
	ON_COMMAND(ID_IMAGE_SNAPIMAGETOVIEW, OnImageSnapimagetoview)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_SNAPIMAGETOVIEW, OnUpdateImageSnapimagetoview)
	ON_COMMAND(ID_WINDOW_STC, OnWindowSizeToContent)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIaeProView construction/destruction

CIaeProView::CIaeProView()
{
	m_bDragging = FALSE;
	m_bValidROI = FALSE;
}

CIaeProView::~CIaeProView()
{
}

BOOL CIaeProView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CIaeProView drawing

void CIaeProView::OnDraw(CDC* pDC)
{
	CIaeProDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->Lock();
	CDefaultImage image = pDoc->GetImage();		// The image...!

	CDC dc;										// The memory DC, rectangle
	CRect rect;									// memory bitmap and old
	CBitmap bmp, *oldbmp;						// GDI object pointer declarations.
	GetClientRect(&rect);						// Get the bounding size

	rect.UnionRect(rect, image.Rect());			// 

	dc.CreateCompatibleDC(pDC);
	bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	
	oldbmp = dc.SelectObject(&bmp);
	
//	dc.FillSolidRect(rect, RGB(255,255,255));
	dc.FillSolidRect(rect, m_crBackground);

	if (image.IsValid()) {						// Paint the image
		image.DisplayInHdc(dc.m_hDC);
	}

	if (m_bValidROI) {
		dc.DrawDragRect(m_cSelectRect,CSize(1,1),NULL,CSize(-1,-1));
	}

	pDoc->OnImageDisplayed();
	pDoc->Unlock();

	// Blit the bitmap to the real DC.	
	pDC->BitBlt(0,0,rect.Width(),rect.Height(),&dc,0,0,SRCCOPY);
	dc.SelectObject(oldbmp);
}

/////////////////////////////////////////////////////////////////////////////
// CIaeProView printing

BOOL CIaeProView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CIaeProView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CIaeProView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CIaeProView diagnostics

#ifdef _DEBUG
void CIaeProView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CIaeProView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CIaeProDoc* CIaeProView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIaeProDoc)));
	return (CIaeProDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CIaeProView message handlers

void CIaeProView::OnInitialUpdate() 
{
	CIaeProDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	pDoc->Lock();
	
	CalculateScrollBars(pDoc->GetImage());

	// Now get the background colour.
	m_crBackground = ::GetSysColor(COLOR_APPWORKSPACE);
	m_cDropTarget.Register(this);						//AHS

	CScrollView::OnInitialUpdate();
}

BOOL CIaeProView::OnEraseBkgnd(CDC* pDC) 
{
//	return CScrollView::OnEraseBkgnd(pDC);
	return FALSE;
}

void CIaeProView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	if (lHint == IAE_UPDATESCROLLBARS) {
		CIaeProDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		
		CalculateScrollBars(pDoc->GetImage());	
	}

	CView::OnUpdate(pSender, lHint, pHint);
}

void CIaeProView::CalculateScrollBars(CDefaultImage &image)
{
	CSize imagesize = CSize(640,480);
	
	if (image.IsValid()) {	
		m_iCx = imagesize.cx = image.Width();
		m_iCy = imagesize.cy = image.Height();
	}

	((CMDIChildWnd *)GetParentFrame())->MDIRestore();	// FIXME: Way around this?!

	SetScrollSizes(MM_TEXT, imagesize);
	ResizeParentToFit();

	if (image.IsValid()) {
		// From Microsoft ExMFCOpenSave example in VisSDK.

		CFrameWnd* pFrame = GetParentFrame();
		
		if (pFrame != NULL) {
			CRect rectSized;
			pFrame->GetWindowRect(&rectSized);
			
			CWnd *pParent = pFrame->GetParent();
			if (pParent != 0) {
				// May need to adjust rect so that it fits in parent.
				bool fResized = false;
				
				pParent->ScreenToClient(rectSized);
				
				CRect rectParent;
				pParent->GetClientRect(&rectParent);
				
//				assert(rectParent.left <= rectSized.left);
//				assert(rectParent.top <= rectSized.top);
				
				if (rectParent.right < rectSized.right) {
					rectSized.right = rectParent.right;
					rectSized.bottom += GetSystemMetrics(SM_CYVSCROLL) + 1;
					fResized = true;
				}
				
				if (rectParent.bottom < rectSized.bottom) {
					rectSized.bottom = rectParent.bottom;
					rectSized.right += GetSystemMetrics(SM_CXHSCROLL) + 1;
					if (rectParent.right < rectSized.right)
						rectSized.right = rectParent.right;
					fResized = true;
				}
				
				if (fResized)
					pFrame->MoveWindow(&rectSized);
			}
		}
	}
}

void CIaeProView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	Invalidate();

	if (point.x < 0) return;
	if (point.y < 0) return;

	if (point.x > m_iCx) point.x = m_iCx;
	if (point.y > m_iCy) point.y = m_iCy;

	m_cInitial = m_cFinal = point;
	m_cSelectRect.SetRect(point.x, point.y, point.x+1, point.y+1);

	CClientDC dc(this);
	dc.DrawDragRect(m_cSelectRect,CSize(1,1),NULL,CSize(1,1));

	m_bDragging = true;
	m_cLastSelectRect = m_cSelectRect;

	SetCapture();
	
	CScrollView::OnLButtonDown(nFlags, point);
}

void CIaeProView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_bDragging) {
		CSize size = m_cSelectRect.Size();

		CIaeProDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);

		if (size.cx <= 1 || size.cy <= 1) {
			m_bValidROI = FALSE;
			Invalidate();
			m_bValidROI = FALSE;
		} else {
			m_bValidROI = TRUE;
		}
		
		m_bDragging = false;
		ReleaseCapture();

		// Compensate the scrollbars.

		CPoint offset = GetScrollPosition();
		m_cSelectRect.OffsetRect(offset);

		pDoc->SetROI(m_cSelectRect, m_bValidROI);
	}

	CScrollView::OnLButtonUp(nFlags, point);
}

void CIaeProView::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (!m_bDragging) return;

	// Check for square selections. 
	// FIXME: Additionally, use the Control+Shift key to 
	// FIXME: select 2-power dimension (2x2, 4x4, 8x8 etc.)
	// FIXME: for FFT/IFFT stuff.

	if (nFlags & MK_CONTROL) {
		int dimension = min(abs(m_cInitial.x - point.x), abs(m_cInitial.y - point.y));

		if (nFlags & MK_SHIFT) {
			// FFT sizes only.
			if (dimension < 2) dimension = 2;
			else if (dimension < 4) dimension = 4;
			else if (dimension < 8) dimension = 8;
			else if (dimension < 16) dimension = 16;
			else if (dimension < 32) dimension = 32;
			else if (dimension < 64) dimension = 64;
			else if (dimension < 128) dimension = 128;
			else dimension = 256;
		}

		if (point.y < m_cInitial.y)			point.y = m_cInitial.y - dimension;
		else if (point.y > m_cInitial.y)	point.y = m_cInitial.y + dimension;

		if (point.x < m_cInitial.x)			point.x = m_cInitial.x - dimension;
		else if (point.x > m_cInitial.x)	point.x = m_cInitial.x + dimension;
	}

	CPoint offset = GetScrollPosition();

	// Validate range
	point.x = (point.x < 0) ? 0 : ((point.x > (m_iCx - offset.x)) ? m_iCx - offset.x : point.x);
	point.y = (point.y < 0) ? 0 : ((point.y > (m_iCy - offset.y)) ? m_iCy - offset.y : point.y);

	m_cSelectRect.SetRect(m_cInitial.x, m_cInitial.y, point.x, point.y);
	m_cSelectRect.NormalizeRect();

	CClientDC dc(this);
	dc.DrawDragRect(m_cSelectRect,CSize(1,1),m_cLastSelectRect,CSize(1,1));

	m_cLastSelectRect = m_cSelectRect;
	
	CScrollView::OnMouseMove(nFlags, point);
}

BOOL CIaeProView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	BOOL bRet = TRUE;

	if (nHitTest == HTCLIENT) {
		SetCursor(AfxGetApp()->LoadCursor(IDC_CROSSHAIRS));
	} else {
		bRet = CScrollView::OnSetCursor(pWnd, nHitTest, message);
	}

	return bRet;
}


//AHS+++
int CIaeProView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	
	return 0;
}

//////////////////////////////////////////////////////////
//Called when a drop occurs
//////////////////////////////////////////////////////////
BOOL CIaeProView::OnDrop(COleDataObject* pDataObject,
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

BOOL CIaeProView::DropBitmap(COleDataObject* pDataObject,DROPEFFECT dropEffect,CPoint point) 
{
	//NOT_IMPLEMENTED
    return FALSE;
}
DROPEFFECT CIaeProView::OnDragEnter(COleDataObject* pDataObject,
                                      DWORD           dwKeyState,
                                      CPoint point) 
{
	return DROPEFFECT_COPY;
}

//AHS---

void CIaeProView::OnDragLeave() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CScrollView::OnDragLeave();
}

DROPEFFECT CIaeProView::OnDragOver(COleDataObject* pDataObject,
                                     DWORD           dwKeyState,
                                     CPoint          point) 
{
	return DROPEFFECT_COPY;
}

void CIaeProView::OnImageSnapimagetoview() 
{
	// Connects the Image to the View, therfor allows image to be streched or shrunk using the View border

	int n = m_nMapMode;
	CSize size = GetTotalSize();
	if (m_nMapMode != -1){	// MM_SCALETOFIT
		SetScaleToFitSize(size);

	} else {
		SetScrollSizes(MM_TEXT, size);	//MM_
		ResizeParentToFit( );   

	}
}

void CIaeProView::OnUpdateImageSnapimagetoview(CCmdUI* pCmdUI) 
{
	if (m_nMapMode != -1)
		pCmdUI->SetCheck(FALSE);
	else
		pCmdUI->SetCheck(TRUE);
}


void CIaeProView::OnWindowSizeToContent() 
{
	ResizeParentToFit(FALSE);   
}
