//////////////////////////////////////////////////////////////////////
//
// MyDropTarget.h: interface for the CMyDropTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYDROPTARGET_H__04E739F2_5474_4A14_97CB_BDF035E0B8B2__INCLUDED_)
#define AFX_MYDROPTARGET_H__04E739F2_5474_4A14_97CB_BDF035E0B8B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxole.h>
 
struct IDropTargetHelper;   // forward reference, in case the latest PSDK isn't installed.

class CMyDropTarget : public COleDropTarget  
{
public:
    CMyDropTarget( CWnd * pList );
	virtual ~CMyDropTarget();

	DROPEFFECT OnDragEnter ( CWnd* pWnd, COleDataObject* pDataObject,
		                     DWORD dwKeyState, CPoint point );

	DROPEFFECT OnDragOver ( CWnd* pWnd, COleDataObject* pDataObject,
		                    DWORD dwKeyState, CPoint point );

	BOOL OnDrop ( CWnd* pWnd, COleDataObject* pDataObject,
		          DROPEFFECT dropEffect, CPoint point );

	void OnDragLeave ( CWnd* pWnd );


protected:
    CWnd*         m_pParent;
    IDropTargetHelper* m_piDropHelper;
    bool               m_bUseDnDHelper;

    BOOL ReadHdropData ( COleDataObject* pDataObject );
};

#endif // !defined(AFX_MYDROPTARGET_H__04E739F2_5474_4A14_97CB_BDF035E0B8B2__INCLUDED_)
