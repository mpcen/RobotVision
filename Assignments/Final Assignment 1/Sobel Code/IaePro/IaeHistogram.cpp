// IaeHistogram.cpp : implementation file
//

#include "stdafx.h"
#include "IaePro.h"
#include "IaeHistogram.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define HIST_REFBARSIZE 10

/////////////////////////////////////////////////////////////////////////////
// CIaeHistogram

CIaeHistogram::CIaeHistogram()
{
	m_uFlags = 0;
	m_iType = eiaeluminosity;
}

CIaeHistogram::~CIaeHistogram()
{
}


BEGIN_MESSAGE_MAP(CIaeHistogram, CStatic)
	//{{AFX_MSG_MAP(CIaeHistogram)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIaeHistogram message handlers

void CIaeHistogram::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

 	CRect rect;
	GetWindowRect(rect);
	ScreenToClient(&rect);

	// Adjust height if the reference bar is turned off.
	int height_padding = (m_uFlags & IAEH_NOREFBAR) ? 0 : HIST_REFBARSIZE;

	int startx = rect.TopLeft().x;
	int starty = rect.BottomRight().y - height_padding - 5;
	int height = rect.Height();
	int ey = 0, my = rect.TopLeft().y;
	long increment;

	double scale = height / double(500);
	long *data = NULL;

	// Draw the bordered rectangle.
	dc.FillSolidRect(startx, starty, 256, (rect.Height()-height_padding-5)*-1, 
					GetSysColor(COLOR_3DFACE));
	dc.Draw3dRect(rect.TopLeft().x-1, rect.TopLeft().y, 258, rect.Height()-height_padding-3,
					GetSysColor(COLOR_BTNSHADOW),
					GetSysColor(COLOR_BTNHILIGHT));

	switch (m_iType) {
		case eiaeredChannel:	data = m_cHistoData.hr; increment = RGB(1,0,0); scale = height / double(m_lMaxR); break;
		case eiaegreenChannel:	data = m_cHistoData.hg; increment = RGB(0,1,0); scale = height / double(m_lMaxG); break;
		case eiaeblueChannel:	data = m_cHistoData.hb; increment = RGB(0,0,1); scale = height / double(m_lMaxB); break;
		default:				data = m_cHistoData.hl; increment = RGB(1,1,1); scale = height / double(m_lMaxL); break;
	}

	CPen pen(PS_SOLID, 0, RGB(0,0,0)), ref, *old;
	old = dc.SelectObject(&pen);

	dc.MoveTo(startx, starty);
	
	if (m_iType != eiaeallChannels) {
		for (int i=0; i<256; i++) {
			ey = starty - data[i] * scale;
			ey = (ey < my) ? my : ey;			// Truncate, if necessary.

			dc.LineTo(startx+i, ey);
			dc.MoveTo(startx+i+1, starty);
		}
	} else {
		scale = height / double(max(m_lMaxR, max(m_lMaxG, max(m_lMaxB, m_lMaxL))));

		CPen r(PS_SOLID, 0, RGB(255,0,0));
		CPen g(PS_SOLID, 0, RGB(0,255,0));
		CPen b(PS_SOLID, 0, RGB(0,0,255));

		// Luminosity
		for (int i=0; i<256; i++) {
			ey = starty - m_cHistoData.hl[i] * scale;
			ey = (ey < my) ? my : ey;
			dc.LineTo(startx+i, ey);
		}

		// Red
		dc.MoveTo(startx, starty);
		dc.SelectObject(&r);
		for (i=0; i<256; i++) {
			ey = starty - m_cHistoData.hr[i] * scale;
			ey = (ey < my) ? my : ey;
			dc.LineTo(startx+i, ey);
		}

		// Green
		dc.MoveTo(startx, starty);
		dc.SelectObject(&g);
		for (i=0; i<256; i++) {
			ey = starty - m_cHistoData.hg[i] * scale;
			ey = (ey < my) ? my : ey;
			dc.LineTo(startx+i, ey);
		}

		// Blue
		dc.MoveTo(startx, starty);
		dc.SelectObject(&b);
		for (i=0; i<256; i++) {
			ey = starty - m_cHistoData.hb[i] * scale;
			ey = (ey < my) ? my : ey;
			dc.LineTo(startx+i, ey);
		}
	}

	dc.SelectObject(old);
	
	// Draw reference bar if necessary.
	if (!(m_uFlags & IAEH_NOREFBAR)) {
		dc.Draw3dRect(rect.TopLeft().x-1, rect.BottomRight().y-HIST_REFBARSIZE-1, 258, HIST_REFBARSIZE+2,
						GetSysColor(COLOR_BTNSHADOW),
						GetSysColor(COLOR_BTNHILIGHT));

		starty = rect.BottomRight().y;

		COLORREF cr = RGB(0,0,0);

		for (int i=0; i<256; i++) {
			dc.FillSolidRect(startx+i,starty-HIST_REFBARSIZE,1,HIST_REFBARSIZE,cr);
			
			cr = long(cr) + increment;
		}
	}
}

void CIaeHistogram::SetType(EIaeHistogram type)
{
	m_iType = type;

	Invalidate();
}

void CIaeHistogram::SetImage(CDefaultImage *pImage)
{
	ASSERT(pImage && pImage->IsValid());
	
	VisMap1(m_cHistoData, *pImage);	

	// Find the maxs (for scaling).

	m_lMaxR = m_lMaxG = m_lMaxB = m_lMaxL = 0;

	for (int i=0; i<256; i++) {
		if (m_cHistoData.hr[i] > m_lMaxR) m_lMaxR = m_cHistoData.hr[i];
		if (m_cHistoData.hg[i] > m_lMaxG) m_lMaxG = m_cHistoData.hg[i];
		if (m_cHistoData.hb[i] > m_lMaxB) m_lMaxB = m_cHistoData.hb[i];
		if (m_cHistoData.hl[i] > m_lMaxL) m_lMaxL = m_cHistoData.hl[i];
	}
}
