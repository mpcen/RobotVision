
#include "stdafx.h"

#include "IaeProRank.h"
#include "PixelOffset.h"

void CIaeProRank::RankFilter(CDefaultImage &imgInput, CDefaultImage &imgOutput, int iRadius, 
							 IaeRankEnum rank, int iRank/*=-1*/)
{

	int ni = iRadius * 2 + 1;					// Neighbourhood
	int ns = ni * ni;							// Neighbourhood size.
	int pos = 0;

	CWaitCursor zzz;

	switch (rank) {
		case IAEPRO_MEDIAN:		iRank = int(floor(ns / 2 + 0.5)); break;
		case IAEPRO_MINIMUM:	iRank = 0; break;
		case IAEPRO_MAXIMUM:	iRank = ns - 1; break;
		case IAEPRO_CUSTOM:		ASSERT(iRank != -1); break;
	}
	
	int sx = imgInput.Width();
	int sy = imgInput.Height();

	CDefaultImage buffer;
	buffer.Copy(imgInput);

	CDefaultPixel *pxNeighbour = new CDefaultPixel[ns];
	CDefaultPixel *pxBuffer    = new CDefaultPixel[ns];		// Buffer for destructive sort

	CPixelOffset pxo(&buffer, &imgOutput, CRect(0,0,iRadius*2,iRadius*2));

	int i = 0, j = 0, spos = 0;

	for (j=0; j<sy-(iRadius*2); j++) {
		spos = pos = 0;
		for (int x=0; x<ni; x++) {
			for (int y=0; y<ni; y++) {
				pxNeighbour[pos++] = buffer.Pixel(pxo.Input(x+i,y+j));
			}
		}

		for (i=0; i<sx-(iRadius*2); i++) {
			memcpy(pxBuffer, pxNeighbour, sizeof(CDefaultPixel *) * ns);	// Copy to buffer.

			imgOutput.Pixel(pxo.Output(i,j)) = Rank(pxBuffer, ns, iRank);	// Use buffer to find rank element.

			// Now read the new column in place of the old, replaceable data.
			
			pos = spos;

			for (int y=0; y<ni; y++) {
				pxNeighbour[pos++] = buffer.Pixel(pxo.Input(ni+i,y+j));
			}

			spos = (spos + ni) % ns;
		}

		i = 0;
	}

	delete [] pxBuffer;
	delete [] pxNeighbour;
}

CDefaultPixel CIaeProRank::Rank(CDefaultPixel *cr, int size, int rank)
{
	qsort(cr, size, sizeof(cr), ComparePixels);

	return cr[rank];
}

int CIaeProRank::ComparePixels(const void *rgb1, const void *rgb2)
{
	// We increment by one to skip the alpha field (RGBA, therefore A is first).
	
	return memcmp((char *)rgb1+1, (char *)rgb2+1, 3);
}
