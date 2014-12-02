
#include "stdafx.h"
#include "PixelOffset.h"

CPixelOffset::CPixelOffset()
{
	m_iIx = m_iIy = m_iWx = m_iWy = m_iOffsetX = m_iOffsetY = m_iFx = m_iFy = 0;
}

CPixelOffset::CPixelOffset(CDefaultImage *pInput, CDefaultImage *pOutput, CRect rectShrink)
{
	m_iIx = m_iIy = m_iWx = m_iWy = m_iOffsetX = m_iOffsetY = m_iFx = m_iFy = 0;

	CRect ir = pInput->Rect();
	CRect or = pOutput->Rect();
	
	ir.DeflateRect(rectShrink);
	or.DeflateRect(rectShrink);

	pOutput->SetRect(or);						// Set the new dimension for output.

	m_iIx = ir.TopLeft().x;						// Where does the input image region start?
	m_iIy = ir.TopLeft().y;
	m_iWx = or.TopLeft().x;						// Where does the output image region start?
	m_iWy = or.TopLeft().y;
	m_iFx = pInput->Width() + m_iIx;			// Compensate for offsets (with ROI).
	m_iFy = pInput->Height() + m_iIy;
}

