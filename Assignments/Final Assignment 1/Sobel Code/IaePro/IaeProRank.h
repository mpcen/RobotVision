
/////////////////////////////////////////////////////////////////////
// Class:			CIaeProRank
// Author:			James Matthews
// Date:			17th February, 2002.
//
// This class performs ranks operations on the picture. It uses 
// CPixelNeighbours to increase its speed.
//

#ifndef _IAEPRO_RANK_H_
#define _IAEPRO_RANK_H_

#if !VIS_OS_IS_NT
#include <VisCore.h>
#include <VisDisplay.h>
#endif // !VIS_OS_IS_NT

enum IaeRankEnum {
	IAEPRO_MEDIAN,
	IAEPRO_MINIMUM,
	IAEPRO_MAXIMUM,
	IAEPRO_CUSTOM
};

class CIaeProRank
{
	public:
		CIaeProRank() {};

		void RankFilter(CDefaultImage &, CDefaultImage &, int, IaeRankEnum, int iRank = -1);

	protected:
		CDefaultPixel Rank(CDefaultPixel *, int, int);

		static int ComparePixels(const void *, const void *);
};

#endif
