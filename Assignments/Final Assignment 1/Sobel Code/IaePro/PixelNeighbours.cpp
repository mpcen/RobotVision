
#include "stdafx.h"
#include "PixelNeighbours.h"

void CPixelNeighbours::Initialize(CDefaultImage *image, int n)
{
	ASSERT(image && image->IsValid());

	m_cPixels.clear();
	m_cPixels.resize(n);

	m_cNextRow.clear();
	m_cNextRow.resize(n);

	m_pImage = image;
	m_iNeighbourhood = n;
}

void CPixelNeighbours::MoveAcross()
{
	ASSERT(m_pImage);

	int len = m_iNeighbourhood * m_iNeighbourhood;

	for (int i=1; i<len; i++) {
		m_cPixels[i-1] = m_cPixels[i];
	}

	// Now read in the new values.
//	m_cPixels[m_iNeighbourhood - 1] = m_pImage->Pixel(
}