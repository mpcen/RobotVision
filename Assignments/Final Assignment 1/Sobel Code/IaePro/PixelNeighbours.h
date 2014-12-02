
/////////////////////////////////////////////////////////////////////
// Class:			CPixelNeighbours
// Author:			James Matthews
// Date:			17th February, 2002.
//
// CPixelNeighbours is designed to help optimize neighbourhood 
// operations by moving common data between iterations avoiding 
// redundant image access.
//

#ifndef _PIXEL_NEIGHBOURS_H_
#define _PIXEL_NEIGHBOURS_H_

#if !VIS_OS_IS_NT
#include <VisCore.h>
#include <VisDisplay.h>
#endif // !VIS_OS_IS_NT

class CPixelNeighbours
{
	public:
		CPixelNeighbours() {;}
		~CPixelNeighbours() {;}

		CDefaultPixel operator[](int i) { return m_cPixels[i]; }

		void Initialize(CDefaultImage *, int);
		void MoveAcross();
		void MoveDown();
		void Finish();

	protected:
		std::vector<CDefaultPixel> m_cPixels;
		std::vector<CDefaultPixel> m_cNextRow;

		int		m_iNeighbourhood;

		CDefaultImage *m_pImage;
};

#endif