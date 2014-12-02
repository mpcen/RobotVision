// MyDropTarget.cpp: implementation of the CMyDropTarget class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "MyDropTarget.h"
#include "MainFrm.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//********************************************************************
// If you don't have a recent Platform SDK installed, you'll get linker
// errors on CLSID_DragDropHelper and IID_IDropTargetHelper, and you
// won't have the definition of IDropTargetHelper.  Uncomment the
// following stuff to fix that.

struct IDropTargetHelper : public IUnknown
{
    // IUnknown methods
    STDMETHOD (QueryInterface)(THIS_ REFIID riid, void **ppv) PURE;
    STDMETHOD_(ULONG, AddRef) ( THIS ) PURE;
    STDMETHOD_(ULONG, Release) ( THIS ) PURE;

    // IDropTargetHelper
    STDMETHOD (DragEnter)(THIS_ HWND hwndTarget, IDataObject* pDataObject,
                          POINT* ppt, DWORD dwEffect) PURE;
    STDMETHOD (DragLeave)(THIS) PURE;
    STDMETHOD (DragOver)(THIS_ POINT* ppt, DWORD dwEffect) PURE;
    STDMETHOD (Drop)(THIS_ IDataObject* pDataObject, POINT* ppt,
                     DWORD dwEffect) PURE;
    STDMETHOD (Show)(THIS_ BOOL fShow) PURE;
};

// {4657278A-411B-11d2-839A-00C04FD918D0}
extern "C" const GUID __declspec(selectany) CLSID_DragDropHelper = 
    { 0x4657278a, 0x411b, 0x11d2, { 0x83, 0x9a, 0x0, 0xc0, 0x4f, 0xd9, 0x18, 0xd0 }};

// {4657278B-411B-11d2-839A-00C04FD918D0}
extern "C" const GUID __declspec(selectany) IID_IDropTargetHelper = 
    { 0x4657278b, 0x411b, 0x11d2, { 0x83, 0x9a, 0x0, 0xc0, 0x4f, 0xd9, 0x18, 0xd0 }};

//********************************************************************


//////////////////////////////////////////////////////////////////////
// Construction/destruction

CMyDropTarget::CMyDropTarget  ( CWnd* pParent ) : m_pParent(pParent), 
                                                     m_piDropHelper(NULL),
                                                     m_bUseDnDHelper(false)
{
    // Create an instance of the shell DnD helper object.

    if ( SUCCEEDED( CoCreateInstance ( CLSID_DragDropHelper, NULL, 
                                       CLSCTX_INPROC_SERVER,
                                       IID_IDropTargetHelper, 
                                       (void**) &m_piDropHelper ) ))
        {
        m_bUseDnDHelper = true;
        }
}

CMyDropTarget::~CMyDropTarget()
{
    if ( NULL != m_piDropHelper )
        m_piDropHelper->Release();
}


//////////////////////////////////////////////////////////////////////
// IDropTarget methods

DROPEFFECT CMyDropTarget::OnDragEnter ( CWnd* pWnd, COleDataObject* pDataObject,
		                                DWORD dwKeyState, CPoint point )
{
	return ((CMainFrame *)m_pParent)->OnDragEnter(pDataObject, dwKeyState, point);
}

DROPEFFECT CMyDropTarget::OnDragOver ( CWnd* pWnd, COleDataObject* pDataObject,
		                               DWORD dwKeyState, CPoint point )
{
	return ((CMainFrame *)m_pParent)->OnDragOver(pDataObject, dwKeyState, point);

}

BOOL CMyDropTarget::OnDrop ( CWnd* pWnd, COleDataObject* pDataObject,
		                     DROPEFFECT dropEffect, CPoint point )
{
	return ((CMainFrame *)m_pParent)->OnDrop(pDataObject, dropEffect, point);
};

void CMyDropTarget::OnDragLeave ( CWnd* pWnd )
{
};
