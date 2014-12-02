// MainFrm.h : interface of the CMainFrame class
//
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__B6281B0C_11DB_11D6_887A_00304F10C7F8__INCLUDED_)
#define AFX_MAINFRM_H__B6281B0C_11DB_11D6_887A_00304F10C7F8__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
class CMyDropTarget;
class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

	BOOL ExplorationMode() { return m_bExplorationMode; }

	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

	BOOL m_bExplorationMode;

	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnImageExplorationmode();
	afx_msg void OnUpdateImageExplorationmode(CCmdUI* pCmdUI);
	afx_msg void OnSizeToContent();
	afx_msg void OnUpdateSizeToContent(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	CMyDropTarget	*m_pDroptarget;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__B6281B0C_11DB_11D6_887A_00304F10C7F8__INCLUDED_)
