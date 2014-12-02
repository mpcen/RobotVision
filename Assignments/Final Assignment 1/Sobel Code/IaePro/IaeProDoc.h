// IaeProDoc.h : interface of the CIaeProDoc class
//
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_IAEPRODOC_H__B6281B10_11DB_11D6_887A_00304F10C7F8__INCLUDED_)
#define AFX_IAEPRODOC_H__B6281B10_11DB_11D6_887A_00304F10C7F8__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#if !VIS_OS_IS_NT
#include <VisCore.h>
#include <VisDisplay.h>
#endif // !VIS_OS_IS_NT

#include "IaeProRank.h"

#define IAE_UPDATESCROLLBARS 0x00000004

class CIaeProDoc : public CDocument
{
protected: // create from serialization only
	CIaeProDoc();
	DECLARE_DYNCREATE(CIaeProDoc)

// Attributes
public:
	// This flag is used to avoid extra forground image processing.
	BOOL HasCurrentImageBeenDisplayed(void) const
		{ return m_fImageDisplayed; }
	
	// The view calls this method to notify the document the document
	// when an image is displayed.
	// UNDONE:  Do the right thing when there are multiple views.
	// UNDONE:  Add an image ID argument?
	void OnImageDisplayed(void);

// Operations
public:
	CPoint GetTopLeft() 
		{ return m_image.Rect().TopLeft(); }
	CDefaultImage &GetImage()
		{ return m_image; }

	void Lock(void)
		{ m_critsect.Enter(); }

	void Unlock(void)
		{ m_critsect.Leave(); }

	// This function is will process the image and update the document.
	void SetImage(CDefaultImage& refimage);

	// This function will copy the image from the image handler and
	// then call SetImage.  It is called by the view when we
	// want to do the processing in the forground thread.
	void ProcessNextImage(void);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIaeProDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetROI(CRect &, BOOL);
	virtual ~CIaeProDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CRect m_cROIRect;

	CVisCritSect m_critsect;
	CDefaultImage m_image;
	CDefaultImage m_imgUndo;

	BOOL m_fFirstImage;
	BOOL m_fImageDisplayed;
	BOOL m_bValidROI;

	CDefaultImage *GetDestination();
	
	void InvalidateAllViews(void);


// Generated message map functions
protected:
	void DoRank(IaeRankEnum, int radius=1, int rank=-1);
	CDefaultImage * GetInput();
	void DisplayAtDestination(CDefaultImage *, BOOL, BOOL copy=FALSE);
	void CreateNewImage(CDefaultImage &);
	//{{AFX_MSG(CIaeProDoc)
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	afx_msg void OnAnalyzeHistogram();
	afx_msg void OnFilterEdgedetectionSobel();
	afx_msg void OnAnalyzeGreyscale();
	afx_msg void OnImageInvert();
	afx_msg void OnAnalyzeFft();
	afx_msg void OnAnalyzeThreshold();
	afx_msg void OnFilterDitherFloydsteinbergerrordiffusion();
	afx_msg void OnImageDuplicate();
	afx_msg void OnFilterNoiseImpulsenoise();
	afx_msg void OnEditCopy();
	afx_msg void OnFilterConvolutionMean();
	afx_msg void OnFilterConvolutionSharpen();
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnFilterRankMedian();
	afx_msg void OnFilterRankMinimum();
	afx_msg void OnFilterRankMaximum();
	afx_msg void OnImageStatistics();
	afx_msg void OnUpdateDefaultImage(CCmdUI* pCmdUI);
	afx_msg void OnFilterRankCustom();
	afx_msg void OnFilterConvolutionCustom();
	afx_msg void OnFilterColourRgbrotation();
	afx_msg void OnFilterConvolutionGaussian();
	afx_msg void OnFilterDitherThresholdmatrix();
	afx_msg void OnFilterSkindetection();
	afx_msg void OnFilterColourGreyscale();
	afx_msg void OnEditPaste();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CIaeProDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IAEPRODOC_H__B6281B10_11DB_11D6_887A_00304F10C7F8__INCLUDED_)
