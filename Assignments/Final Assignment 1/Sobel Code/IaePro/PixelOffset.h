
/////////////////////////////////////////////////////////////////////
// Class:			CPixelOffset
// Author:			James Matthews
// Date:			17th February, 2002.
//
// CPixelOffset is a class that helps ease problems caused by the 
// memory addressing of the images and by region-of-interests. Images
// don't always start at 0,0 and both the input and output images may
// have different top-left coordinates. This class ensures that these
// difficulties can be avoided.
//

#ifndef _PIXELOFFSET_H_
#define _PIXELOFFSET_H_

#if !VIS_OS_IS_NT
#include <VisCore.h>
#include <VisDisplay.h>
#endif // !VIS_OS_IS_NT

class CPixelOffset
{
	public:
		CPixelOffset();
		CPixelOffset(CDefaultImage *, CDefaultImage *, CRect);

		void Offset(CPoint ptOffset) { m_iOffsetX = ptOffset.x; m_iOffsetY = ptOffset.y; }

		inline int IX() { return m_iIx + m_iOffsetX; }
		inline int IY() { return m_iIy + m_iOffsetY; }
		inline int FX() { return m_iFx; }
		inline int FY() { return m_iFy; }

		inline CPoint Input(int x, int y) {
			return CPoint(m_iIx+x, m_iIy+y);
		}

		inline CPoint Output(int x, int y) {
			return CPoint(x-(m_iIx+m_iOffsetX)+m_iWx+m_iIx, y-(m_iIy+m_iOffsetY)+m_iWy+m_iIy);
		}

	protected:
		int	m_iIx, m_iIy;			// Input offsets
		int m_iWx, m_iWy;			// Output offsets (FIXME: change name)
		int m_iFx, m_iFy;			// Width + offset
		int m_iOffsetX, m_iOffsetY;	// Additional offsets
};

#endif