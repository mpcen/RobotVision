// IaeProDoc.cpp : implementation of the CIaeProDoc class
//

#include "stdafx.h"
#include "IaePro.h"

#include "MainFrm.h"
#include "IaeProDoc.h"

#include "PixelOffset.h"
#include "QuickFilters.h"
#include "IaeProConvolution.h"
#include "IaeProRank.h"
#include "IaeProFft.h"
#include "IaeProException.h"

// Dialog includes

#include "HistogramDlg.h"
#include "ThresholdDlg.h"
#include "StatsDlg.h"
#include "CustomRankDlg.h"
#include "CustomConvDlg.h"
#include "GreyscaleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIaeProDoc

IMPLEMENT_DYNCREATE(CIaeProDoc, CDocument)

BEGIN_MESSAGE_MAP(CIaeProDoc, CDocument)
	//{{AFX_MSG_MAP(CIaeProDoc)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_ANALYZE_HISTOGRAM, OnAnalyzeHistogram)
	ON_COMMAND(ID_FILTER_EDGEDETECTION_SOBEL, OnFilterEdgedetectionSobel)
	ON_COMMAND(ID_ANALYZE_GREYSCALE, OnAnalyzeGreyscale)
	ON_COMMAND(ID_IMAGE_INVERT, OnImageInvert)
	ON_COMMAND(ID_ANALYZE_FFTIFFT, OnAnalyzeFft)
	ON_COMMAND(ID_ANALYZE_THRESHOLD, OnAnalyzeThreshold)
	ON_COMMAND(ID_FILTER_DITHER_FLOYDSTEINBERGERRORDIFFUSION, OnFilterDitherFloydsteinbergerrordiffusion)
	ON_COMMAND(ID_IMAGE_DUPLICATE, OnImageDuplicate)
	ON_COMMAND(ID_FILTER_NOISE_IMPULSENOISE, OnFilterNoiseImpulsenoise)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_FILTER_CONVOLUTION_MEAN, OnFilterConvolutionMean)
	ON_COMMAND(ID_FILTER_CONVOLUTION_SHARPEN, OnFilterConvolutionSharpen)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_FILTER_RANK_MEDIAN, OnFilterRankMedian)
	ON_COMMAND(ID_FILTER_RANK_MINIMUM, OnFilterRankMinimum)
	ON_COMMAND(ID_FILTER_RANK_MAXIMUM, OnFilterRankMaximum)
	ON_COMMAND(ID_IMAGE_STATISTICS, OnImageStatistics)
	ON_UPDATE_COMMAND_UI(ID_ANALYZE_FFTIFFT, OnUpdateDefaultImage)
	ON_COMMAND(ID_FILTER_RANK_CUSTOM, OnFilterRankCustom)
	ON_COMMAND(ID_FILTER_CONVOLUTION_CUSTOM, OnFilterConvolutionCustom)
	ON_COMMAND(ID_FILTER_COLOUR_RGBROTATION, OnFilterColourRgbrotation)
	ON_COMMAND(ID_FILTER_CONVOLUTION_GAUSSIAN, OnFilterConvolutionGaussian)
	ON_COMMAND(ID_FILTER_DITHER_THRESHOLDMATRIX, OnFilterDitherThresholdmatrix)
	ON_COMMAND(ID_FILTER_SKINDETECTION, OnFilterSkindetection)
	ON_COMMAND(ID_FILTER_COLOUR_GREYSCALE, OnFilterColourGreyscale)
	ON_UPDATE_COMMAND_UI(ID_ANALYZE_GREYSCALE, OnUpdateDefaultImage)
	ON_UPDATE_COMMAND_UI(ID_ANALYZE_HISTOGRAM, OnUpdateDefaultImage)
	ON_UPDATE_COMMAND_UI(ID_ANALYZE_THRESHOLD, OnUpdateDefaultImage)
	ON_UPDATE_COMMAND_UI(ID_FILTER_CONVOLUTION_MEAN, OnUpdateDefaultImage)
	ON_UPDATE_COMMAND_UI(ID_FILTER_CONVOLUTION_SHARPEN, OnUpdateDefaultImage)
	ON_UPDATE_COMMAND_UI(ID_FILTER_DITHER_FLOYDSTEINBERGERRORDIFFUSION, OnUpdateDefaultImage)
	ON_UPDATE_COMMAND_UI(ID_FILTER_EDGEDETECTION_SOBEL, OnUpdateDefaultImage)
	ON_UPDATE_COMMAND_UI(ID_FILTER_NOISE_IMPULSENOISE, OnUpdateDefaultImage)
	ON_UPDATE_COMMAND_UI(ID_FILTER_RANK_MAXIMUM, OnUpdateDefaultImage)
	ON_UPDATE_COMMAND_UI(ID_FILTER_RANK_MEDIAN, OnUpdateDefaultImage)
	ON_UPDATE_COMMAND_UI(ID_FILTER_RANK_MINIMUM, OnUpdateDefaultImage)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_DUPLICATE, OnUpdateDefaultImage)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_INVERT, OnUpdateDefaultImage)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_STATISTICS, OnUpdateDefaultImage)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CIaeProDoc, CDocument)
	//{{AFX_DISPATCH_MAP(CIaeProDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//      DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IIaePro to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {B6281B04-11DB-11D6-887A-00304F10C7F8}
static const IID IID_IIaePro =
{ 0xb6281b04, 0x11db, 0x11d6, { 0x88, 0x7a, 0x0, 0x30, 0x4f, 0x10, 0xc7, 0xf8 } };

BEGIN_INTERFACE_MAP(CIaeProDoc, CDocument)
	INTERFACE_PART(CIaeProDoc, IID_IIaePro, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIaeProDoc construction/destruction

CIaeProDoc::CIaeProDoc()
  : m_fFirstImage(true),
	m_fImageDisplayed(false)
{
	m_bValidROI = FALSE;

	EnableAutomation();

	AfxOleLockApp();

}

CIaeProDoc::~CIaeProDoc()
{
	AfxOleUnlockApp();
}

BOOL CIaeProDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CIaeProDoc diagnostics

#ifdef _DEBUG
void CIaeProDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CIaeProDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CIaeProDoc helper functions

void CIaeProDoc::ProcessNextImage(void) 
{
	m_fImageDisplayed = false;
}

void CIaeProDoc::InvalidateAllViews(void) 
{
	// Helper function to invalidate all views.
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = GetNextView(pos);
		pView->Invalidate(FALSE);
	}
}

void CIaeProDoc::SetImage(CDefaultImage& refimage)
{
	CDefaultImage imageT = refimage;

	if (imageT.IsValid())
	{

		// TODO:  Add image processing code that does not modify the document
		// (but may modify local variables) here.

		Lock();

		// Copy the image to the document.
		// TODO:  Add code to copy any local variables containing image
		// processing results here.
		// (Note that this code is within a critical section.)
		m_image = imageT;

		Unlock();
	}
}

void CIaeProDoc::OnImageDisplayed(void) 
{
	m_fImageDisplayed = true;
}


/////////////////////////////////////////////////////////////////////////////
// CIaeProDoc commands

BOOL CIaeProDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	BOOL fRet = TRUE;

	try
	{
		CDefaultImage imageT;
		if (!imageT.FReadFile(lpszPathName)) {
			if (imageT.PixFmt() == evispixfmtGrayByte) {
				// Try using a color image and then copying the pixels to
				// our grayscale image.
				CVisRGBAByteImage imageRGBA;
				imageRGBA.ReadFile(lpszPathName);

				imageT.Allocate(imageRGBA.Rect());
			} else {
				// Try using a gray image and then copying the pixels to
				// our color image.
				CVisGrayByteImage imageGray;
				imageGray.ReadFile(lpszPathName);

				imageT.Allocate(imageGray.Rect());

				imageGray.CopyPixelsTo(imageT);
			}
		}

		SetImage(imageT);

	} catch (CVisFileIOError&) {
		CString str = "An error occurred while reading the file. Please make sure it is a valid BMP, JPEG or GIF format.";
		
		MessageBox(AfxGetMainWnd()->m_hWnd, str,
			"Error Reading File", MB_ICONWARNING);

		fRet = FALSE;
	}

	return fRet;
}

BOOL CIaeProDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	BOOL fRet = TRUE;
	
	try {
		m_image.WriteFile(lpszPathName);
	} catch(CVisFileIOError& e) {
		AfxMessageBox(e.FullMessage());

		fRet = FALSE;
	}

	return fRet;
}

void CIaeProDoc::OnFileSave() 
{

	if (m_image.IsValid())
		CDocument::OnFileSave();
	else
		AfxMessageBox("There is no image to save.");

}

void CIaeProDoc::OnFileSaveAs() 
{
	if (m_image.IsValid())
		CDocument::OnFileSaveAs();
	else
		AfxMessageBox("There is no image to save.");
}

void CIaeProDoc::OnAnalyzeHistogram() 
{
	CHistogramDlg dlg;

	dlg.SetImage(GetInput());
	dlg.DoModal();
}

void CIaeProDoc::OnFilterEdgedetectionSobel() 
{
	CDefaultImage buffer;
	CDefaultImage *inputImage = GetInput();		// Get the input image (could be a ROI).
	CDefaultImage *newimage = GetDestination();	// Get the destination.

	CRect dir = newimage->Rect();				// Retrieve, deflate and set new rect
	dir.DeflateRect(0,0,2,2);					//
	newimage->SetRect(dir);						// Set the new dimension for output.

	buffer.Copy(*inputImage);
	VisMap2(GreyscaleRMY(), buffer, buffer);	// Convert to greyscale

//	CPixelOffset pxo(inputImage, newimage, CRect(0,0,2,2));
	CPixelOffset pxo(&buffer, newimage, CRect(0,0,2,2));

	short gc;
	int a,b,c,d,e,f,g,h,z;

	float sobscale = 1;
	float offsetval = 0;

	int i = 0, j = 0;
	int final_i = buffer.Width() - 2;
	int final_j = buffer.Height() - 2;

	for (j = 0; j < final_j; j++) {
		a = buffer.Pixel(pxo.Input(i,j)).R();			// Read leftmost 6 in.
		b = buffer.Pixel(pxo.Input(i+1,j)).R();		//
		d = buffer.Pixel(pxo.Input(i,j+1)).R();		//
		f = buffer.Pixel(pxo.Input(i,j+2)).R();		//
		g = buffer.Pixel(pxo.Input(i+1,j+2)).R();		//
		z = buffer.Pixel(pxo.Input(i,j)).R();			// Cache centre point (not used in algorithm).

		for (i=0; i<final_i; i++) {
			c = buffer.Pixel(pxo.Input(i+2,j)).R();	// Read the rightmost 3 pixels.
			e = buffer.Pixel(pxo.Input(i+2,j+1)).R();	// 
			h = buffer.Pixel(pxo.Input(i+2,j+2)).R();	//

			int hor = (a+d+f) - (c+e+h);	// The Sobel algorithm
			if (hor < 0) hor = -hor;
			int vert = (a+b+c) - (f+g+h);
			if (vert < 0) vert = -vert;
			gc = short(hor + vert);
			short gc = (short) (sobscale * (hor + vert));
			gc = (short) (gc + offsetval);

			gc = (gc > 255) ? 0 : 255 - gc;	// Truncate and inverse
			
			newimage->Pixel(pxo.Output(i,j)).SetRGB(gc,gc,gc);

			a = b;							// Move the necessary pixels
			b = c;							// across into their new positions.
			d = z;							// This saves a lot of time since we
			f = g;							// don't need to access the pixel
			g = h;							// data again.
			z = e;							//
		}

		i = 0;
	}	

	DisplayAtDestination(newimage, TRUE);

	UpdateAllViews(NULL);
}

void CIaeProDoc::OnAnalyzeGreyscale() 
{
	CDefaultImage *destImage = GetDestination();
	
	VisMap2(GreyscaleRMY(), m_image, *destImage);

	DisplayAtDestination(destImage, TRUE);
}

void CIaeProDoc::OnImageInvert() 
{
	CDefaultImage *destImage = GetDestination();
	
	VisMap2(Invert(), m_image, *destImage);

	DisplayAtDestination(destImage, TRUE);
}

void CIaeProDoc::CreateNewImage(CDefaultImage &newImageRef)
{
	CIaeProDoc *pNewImage = reinterpret_cast<CIaeProDoc *>(
		reinterpret_cast<CIaeProApp *>(AfxGetApp())->CreateNewDoc());

	ASSERT(pNewImage);

	pNewImage->SetImage(newImageRef);
	pNewImage->UpdateAllViews(NULL, IAE_UPDATESCROLLBARS, NULL);

}

void CIaeProDoc::OnAnalyzeFft() 
{
	CDefaultImage buffer;
	CDefaultImage *inputImage = GetInput();		// Get the input image (could be a ROI).
	CDefaultImage *newimage = GetDestination();	// Get the destination.

	buffer.Copy(*inputImage);
	VisMap2(GreyscaleRMY(), buffer, buffer);	// Convert to greyscale

	CIaeProFFT fft;
	try {
		fft.FFT(&buffer, newimage);
	} catch (CIaeProException &e) {
		MessageBox(AfxGetMainWnd()->m_hWnd, e.GetError(), "IaePro Error", MB_ICONWARNING | MB_OK);

		delete newimage;
		return;
	}

	DisplayAtDestination(newimage, TRUE);

	UpdateAllViews(NULL);
}

void CIaeProDoc::OnAnalyzeThreshold() 
{
	CThresholdDlg dlg;
	
	dlg.SetImage(&m_image);						// FIXME: ROI thresholding?
	dlg.SetDocument(this);

	dlg.DoModal();
}

void CIaeProDoc::OnFilterDitherFloydsteinbergerrordiffusion() 
{
	CWaitCursor zzz;

	CDefaultImage buffer;
	CDefaultImage *inputImage = GetInput();		// Get the input image (could be a ROI).
	CDefaultImage *newimage = GetDestination();	// Get the destination.

	CRect dir = newimage->Rect();
	dir.DeflateRect(0,0,1,1);
	newimage->SetRect(dir);						// Set the new dimension for output.

	buffer.Copy(*inputImage);
	VisMap2(GreyscaleRMY(), buffer, buffer);	// Convert to greyscale

	CPixelOffset pxo(inputImage, newimage, CRect(0,0,1,1));

	int e = 0;
	int i = 0;
	int j = 0;
	int final_i = buffer.Width();
	int final_j = buffer.Height() - 1;

	int ed1, ed2, ed3, ed4;						// Error diffusion
	UCHAR value;

	for (j = 0; j < final_j; j++) {
		value = buffer.Pixel(pxo.Input(i,j)).R();

		for (i = 1; i < final_i; i++) {
			if (value < 127) {
				newimage->Pixel(pxo.Output(i,j)).SetRGB(0,0,0);
				e = value;
			} else {
				newimage->Pixel(pxo.Output(i,j)).SetRGB(255,255,255);
				e = value - 255;
			}

			CDefaultPixel &px1 = buffer.Pixel(pxo.Input(i+1,j));
			CDefaultPixel &px2 = buffer.Pixel(pxo.Input(i-1, j+1));
			CDefaultPixel &px3 = buffer.Pixel(pxo.Input(i,j+1));
			CDefaultPixel &px4 = buffer.Pixel(pxo.Input(i+1,j+1));

			ed1 = px1.R();
			ed2 = px2.R();
			ed3 = px3.R();
			ed4 = px4.R();

			ed1 += (7.0 * e)/16.0;				// Calculate error diffusion
			ed2 += (3.0 * e)/16.0;
			ed3 += (5.0 * e)/16.0;
			ed4 += (1.0 * e)/16.0;

			ed1 = (ed1 < 0) ? 0 : ((ed1 > 255) ? 255 : ed1);
			ed2 = (ed2 < 0) ? 0 : ((ed2 > 255) ? 255 : ed2);
			ed3 = (ed3 < 0) ? 0 : ((ed3 > 255) ? 255 : ed3);
			ed4 = (ed4 < 0) ? 0 : ((ed4 > 255) ? 255 : ed4);

			px1.SetRGB(ed1,ed1,ed1);
			px2.SetRGB(ed2,ed2,ed2);
			px3.SetRGB(ed3,ed3,ed3);
			px4.SetRGB(ed4,ed4,ed4);

			value = ed1;						// This is our new value.
		}

		i= 0;
	}

	DisplayAtDestination(newimage, TRUE);

	UpdateAllViews(NULL);
}

CDefaultImage *CIaeProDoc::GetInput()
{
	CDefaultImage *destImage;
	
	if (m_bValidROI) {
		destImage = new CDefaultImage(m_cROIRect);
		destImage->Copy(m_image.SubImage(m_cROIRect));
	} else {
		destImage = &m_image;
	}

	return destImage;
}


CDefaultImage *CIaeProDoc::GetDestination()
{
	CDefaultImage *destImage = NULL;
	CMainFrame* pMfm = STATIC_DOWNCAST(CMainFrame, AfxGetMainWnd());

	if (m_bValidROI) {
		if (pMfm->ExplorationMode()) {
			destImage = new CDefaultImage;
			destImage->Copy(m_image.SubImage(m_cROIRect));
		} else {
			destImage = new CDefaultImage;
			*destImage = m_image.SubImage(m_cROIRect);
			m_imgUndo.Copy(m_image);
		}
	} else if (pMfm->ExplorationMode()) {
		destImage = new CDefaultImage;
		destImage->Copy(m_image);
	} else {
//		destImage = new CDefaultImage;
//		destImage->Copy(m_image);
		destImage = &m_image;
		m_imgUndo.Copy(m_image);
	}

	ASSERT(destImage);

	return destImage;
}

void CIaeProDoc::DisplayAtDestination(CDefaultImage *destImage, BOOL bDelete, BOOL bCopy/*=FALSE*/)
{
	CMainFrame* pMfm = STATIC_DOWNCAST(CMainFrame, AfxGetMainWnd());

	if (pMfm->ExplorationMode()) {
		CreateNewImage(*destImage);

		if (bDelete) delete destImage;

	} else {

		UpdateAllViews(NULL);
	}
}

void CIaeProDoc::SetROI(CRect &rectROI, BOOL bValidROI)
{
	// We need to compensate for the top-left coordinates of
	// the image. This isn't necessary in the view class, since
	// all coordinates are relative to (0,0).

	if (bValidROI) {
		m_cROIRect = rectROI;
		m_cROIRect.OffsetRect(GetTopLeft());
	}

	m_bValidROI = bValidROI;
}

void CIaeProDoc::OnImageDuplicate() 
{
	// Have to use modified versions of GetDestination and 
	// DisplayAtDestination, since a duplicate image must 
	// ALWAYS be in a new window.
	
	CDefaultImage *destImage;
	CMainFrame* pMfm = STATIC_DOWNCAST(CMainFrame, AfxGetMainWnd());

	if (m_bValidROI) {
		destImage = new CDefaultImage;
		destImage->Copy(m_image.SubImage(m_cROIRect));
	} else {
		destImage = new CDefaultImage;
		destImage->Copy(m_image);
	}

	CreateNewImage(*destImage);
	
	delete destImage;
}

void CIaeProDoc::OnFilterNoiseImpulsenoise() 
{
	CDefaultImage *destImage = GetDestination();
	
	VisMap2(ImpulseNoise(5), m_image, *destImage);

	DisplayAtDestination(destImage, TRUE);
}

void CIaeProDoc::OnEditCopy() 
{
	// FIXME: This may be the wrong way to copy data AND the outputted
	// FIXME: image contains too many pixels (eg. Sobel results).

	CWnd *wnd = AfxGetMainWnd();

	ASSERT(wnd);

	CDC bmpdc;
	CClientDC dc(wnd);
	bmpdc.CreateCompatibleDC(wnd->GetDC());

	wnd->OpenClipboard();
	EmptyClipboard();
	SetClipboardData(CF_BITMAP, m_image.HBitmapCreateForHdc(bmpdc.m_hDC));
	CloseClipboard();
}

void CIaeProDoc::OnFilterConvolutionMean() 
{
	CDefaultImage buffer;
	CDefaultImage *inputImage = GetInput();		// Get the input image (could be a ROI).
	CDefaultImage *newimage = GetDestination();	// Get the destination.

	buffer.Copy(*inputImage);

	// Yes, convolution isn't the best way
	// to perform a mean operation, but it
	// is easy to integrate into IaePro.

	double dMean[9] = { 1, 1, 1,
		                1, 1, 1, 
						1, 1, 1 };

	CIaeProConvolution conv;
	conv.SetKernel(dMean, 3, 9);
	conv.Convolute(*inputImage, *newimage);
	
	DisplayAtDestination(newimage, TRUE);

	UpdateAllViews(NULL);
}

void CIaeProDoc::OnFilterConvolutionSharpen() 
{
	CDefaultImage buffer;
	CDefaultImage *inputImage = GetInput();		// Get the input image (could be a ROI).
	CDefaultImage *newimage = GetDestination();	// Get the destination.

	CRect dir = newimage->Rect();
	dir.DeflateRect(1,1,1,1);
	newimage->SetRect(dir);						// Set the new dimension for output.

	buffer.Copy(*inputImage);

	double dSharpen[9] = { -1, -1, -1,
		                   -1,  9, -1,
						   -1, -1, -1 };

	CIaeProConvolution conv;
	conv.SetKernel(dSharpen, 3, 1);
	conv.Convolute(*inputImage, *newimage);
	
	DisplayAtDestination(newimage, TRUE);

	UpdateAllViews(NULL);	
}

void CIaeProDoc::OnEditUndo() 
{
	CDefaultImage temp;

	temp.Copy(m_image);				// Copy existing picture over.
	m_image.Copy(m_imgUndo);		// Undo!
	m_imgUndo.Copy(temp);			// Undo now holds filtered data (redo).

	UpdateAllViews(NULL);
}

void CIaeProDoc::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_imgUndo.IsValid());	
}

void CIaeProDoc::OnFilterRankMedian() 
{
	DoRank(IAEPRO_MEDIAN);
}

void CIaeProDoc::OnFilterRankMinimum() 
{
	DoRank(IAEPRO_MINIMUM);
}

void CIaeProDoc::OnFilterRankMaximum() 
{
	DoRank(IAEPRO_MAXIMUM);
}

void CIaeProDoc::DoRank(IaeRankEnum type, int radius/*=1*/, int rank/*=-1*/)
{
	CWaitCursor zzz;

	CDefaultImage buffer;
	CDefaultImage *inputImage = GetInput();		// Get the input image (could be a ROI).
	CDefaultImage *newimage = GetDestination();	// Get the destination.

	buffer.Copy(*inputImage);

	CIaeProRank rk;
	rk.RankFilter(*inputImage, *newimage, radius, type, rank);
	
	DisplayAtDestination(newimage, TRUE, TRUE);

	UpdateAllViews(NULL);	
}

void CIaeProDoc::OnImageStatistics() 
{
	CWaitCursor zzz;

	CStatsDlg dlg;
	Statistics stats;
	StdDev stddev(&stats);

	CDefaultImage buffer;
	CDefaultImage *inputImage = GetInput();		// Get the input image (could be a ROI).

	buffer.Copy(*inputImage);
	VisMap2(GreyscaleRMY(), buffer, buffer);	// Convert to greyscale
	
	VisMap1(stats, buffer);						// Compute some simple stats

	stats.x = buffer.Width();					// Width
	stats.y = buffer.Height();					// Height
	stats.pixels = stats.x * stats.y;			// Number of pixels
	stats.mean = stats.sum/double(stats.pixels);// Mean

	VisMap1(stddev, buffer);					// Now compute the standard deviation
	stddev.postCalculate();						// Does necessary post image calculations
	
	dlg.SetStats(&stats, &stddev);
	dlg.DoModal();
}

void CIaeProDoc::OnUpdateDefaultImage(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_image.IsValid());
}

void CIaeProDoc::OnFilterRankCustom() 
{
	CCustomRankDlg dlg;
	int neighbour, rank;

	int retval = dlg.DoModal();					// Open the dialog

	if (retval == IDOK) { 
		dlg.GetRankInfo(neighbour, rank);
		DoRank(IAEPRO_CUSTOM, neighbour, rank);
	}
}

void CIaeProDoc::OnFilterConvolutionCustom() 
{
	CCustomConvDlg dlg;

	if (dlg.DoModal() == IDOK) {
		IaeKernel kernel;

		dlg.RetrieveInfo(kernel);

		CDefaultImage buffer;
		CDefaultImage *inputImage = GetInput();
		CDefaultImage *newimage = GetDestination();

		buffer.Copy(*inputImage);

		CIaeProConvolution conv;
		conv.SetKernel(&kernel);
		conv.Convolute(*inputImage, *newimage);
		
		DisplayAtDestination(newimage, TRUE);

		UpdateAllViews(NULL);
	}
}

void CIaeProDoc::OnFilterColourRgbrotation() 
{
	CDefaultImage *destImage = GetDestination();
	
	VisMap2(RGBRotate(), m_image, *destImage);

	DisplayAtDestination(destImage, TRUE);
}

void CIaeProDoc::OnFilterConvolutionGaussian() 
{
	CDefaultImage buffer;
	CDefaultImage *inputImage = GetInput();		// Get the input image (could be a ROI).
	CDefaultImage *newimage = GetDestination();	// Get the destination.

	CRect dir = newimage->Rect();
	dir.DeflateRect(1,1,1,1);
	newimage->SetRect(dir);						// Set the new dimension for output.

	buffer.Copy(*inputImage);

	double dGaussian[81] = { 0, 0,      0,      0,      0,      0,      0,      0,      0,
		                     0, 0,      0.0002, 0.0011, 0.0018, 0.0011, 0.0002, 0,      0,
				 			 0, 0.0002, 0.0029, 0.0131, 0.0215, 0.0131, 0.0029, 0.0002, 0,
							 0, 0.0011, 0.0131, 0.0586, 0.0965, 0.0586, 0.0131, 0.0011, 0,
							 0, 0.0018, 0.0215, 0.0965, 0.1592, 0.0965, 0.0215, 0.0018, 0,
							 0, 0.0011, 0.0131, 0.0586, 0.0965, 0.0586, 0.0131, 0.0011, 0,
				 			 0, 0.0002, 0.0029, 0.0131, 0.0215, 0.0131, 0.0029, 0.0002, 0,
		                     0, 0,      0.0002, 0.0011, 0.0018, 0.0011, 0.0002, 0,      0,
							 0, 0,      0,      0,      0,      0,      0,      0,      0  };

	CIaeProConvolution conv;
	conv.SetKernel(dGaussian, 9, 1);
	conv.Convolute(*inputImage, *newimage);
	
	DisplayAtDestination(newimage, TRUE);

	UpdateAllViews(NULL);	
	
}

void CIaeProDoc::OnFilterDitherThresholdmatrix() 
{
	CWaitCursor zzz;

	CDefaultImage buffer;
	CDefaultImage *inputImage = GetInput();		// Get the input image (could be a ROI).
	CDefaultImage *newimage = GetDestination();	// Get the destination.

	buffer.Copy(*inputImage);
	VisMap2(GreyscaleRMY(), buffer, buffer);	// Convert to greyscale

	int dither[16] = { 0, 128, 32, 160, 192, 64, 224, 96, 48, 176, 16, 144, 240, 112, 208, 80 };

	CPixelOffset pxo(inputImage, newimage, CRect(0,0,1,1));

	int final_i = buffer.Width();
	int final_j = buffer.Height();

	UCHAR value;

	for (int j = 0; j < final_j; j++) {
		for (int i = 0; i < final_i; i++) {
			value = buffer.Pixel(pxo.Input(i,j)).R();
			
			if (value <= dither[(j % 4) * 4 + (i % 4)]) {
				newimage->Pixel(pxo.Output(i, j)).SetRGB(0,0,0);
			} else {
				newimage->Pixel(pxo.Output(i, j)).SetRGB(255,255,255);
			}
		}
	}

	DisplayAtDestination(newimage, TRUE);

	UpdateAllViews(NULL);	
}

void CIaeProDoc::OnFilterSkindetection() 
{
	CDefaultImage buffer1, buffer2;
	CVisImage<CVisRGBADoublePixel> IRgByImage;
	CDefaultImage *inputImage = GetInput();		// Get the input image (could be a ROI).
	CDefaultImage *newimage = GetDestination();	// Get the destination.

	CRect dir = newimage->Rect();
	dir.DeflateRect(0,0,1,1);
	newimage->SetRect(dir);						// Set the new dimension for output.

	buffer1.Copy(*inputImage);
	IRgByImage.Allocate(dir.Width(), dir.Height());

	// Phase 1: RGB to IRgBy and IRgBy to hue

	VisMap2(IRgBy(), buffer1, IRgByImage);
	VisMap2(IRgBy2Hue(), IRgByImage, buffer1);

	// Phase 2: Mean filter
	
	buffer2.Copy(buffer1);

	double dMean[9] = { 1,1,1,1,1,1,1,1,1 };

	CIaeProConvolution conv;
	conv.SetKernel(dMean, 3, 9);
	conv.Convolute(buffer1, buffer2);
	
	// Phase 3: AND two images together.

	VisMap3(And(), *inputImage, buffer2, *newimage);
	
	DisplayAtDestination(newimage, TRUE);

	UpdateAllViews(NULL);	

}

void CIaeProDoc::OnFilterColourGreyscale() 
{
	CGreyscaleDlg dlg;
	
	if (dlg.DoModal() == IDOK) {
		int gs = dlg.GetGreyscale();
		CDefaultImage *destImage = GetDestination();
	
		switch (gs) {
			case igsBt709:   VisMap2(GreyscaleBT709(), m_image, *destImage); break;
			case igsY:	     VisMap2(GreyscaleY(), m_image, *destImage); break;
			case igsRminusY: VisMap2(GreyscaleRMY(), m_image, *destImage); break;
		}

		DisplayAtDestination(destImage, TRUE);
	}
}

void CIaeProDoc::OnEditPaste() 
{
// NOT FINNISHED
	CWnd *wnd = AfxGetMainWnd();

	ASSERT(wnd);

	wnd->OpenClipboard();

	HBITMAP handle = (HBITMAP)GetClipboardData(CF_BITMAP);
	CBitmap * bm = CBitmap::FromHandle(handle);


	CloseClipboard();

	// TODO: Add your command handler code here
	
}
