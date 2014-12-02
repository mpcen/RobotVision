// IaeProView.h : interface of the CIaeProView class
//
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_IAEPROVIEW_H__B6281B12_11DB_11D6_887A_00304F10C7F8__INCLUDED_)
#define AFX_IAEPROVIEW_H__B6281B12_11DB_11D6_887A_00304F10C7F8__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <afxole.h>

class CDragDropMgr;

class CIaeProView : public CScrollView
{
protected: // create from serialization only
	CIaeProView();
	DECLARE_DYNCREATE(CIaeProView)

// Attributes
public:
	CIaeProDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIaeProView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual void OnDragLeave();
	protected:
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	//}}AFX_VIRTUAL
    BOOL DropBitmap(COleDataObject* pDataObject,DROPEFFECT dropEffect,CPoint point);

// Implementation
public:
	virtual ~CIaeProView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	int		m_iCx, m_iCy;
	BOOL	m_bDragging, m_bValidROI;
	COLORREF	m_crBackground;

	CPoint	m_cInitial, m_cFinal;
	CRect	m_cSelectRect, m_cLastSelectRect;

	void CalculateScrollBars(CDefaultImage &);

	COleDropTarget	m_cDropTarget;

	//{{AFX_MSG(CIaeProView)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnImageSnapimagetoview();
	afx_msg void OnUpdateImageSnapimagetoview(CCmdUI* pCmdUI);
	afx_msg void OnImageSizetoimage();
	afx_msg void OnWindowSizeToContent();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

#ifndef _DEBUG  // debug version in IaeProView.cpp
inline CIaeProDoc* CIaeProView::GetDocument()
   { return (CIaeProDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IAEPROVIEW_H__B6281B12_11DB_11D6_887A_00304F10C7F8__INCLUDED_)
