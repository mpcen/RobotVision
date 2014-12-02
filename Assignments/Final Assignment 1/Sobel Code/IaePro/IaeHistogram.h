
/////////////////////////////////////////////////////////////////////
// Class:			CIaeHistogram
// Author:			James Matthews
// Date:			17th February, 2002.
//
// This is a very simple class that displays a histogram for a
// given image. It isn't written with generality in mind and will
// only accomodate for height, NOT width (set at 258 pixels).
//

#ifndef _AFX_IAEHISTOGRAM_H_
#define _AFX_IAEHISTOGRAM_H_

#include "QuickFilters.h"

#define IAEH_NOREFBAR	0x00000001

enum EIaeHistogram
{
	eiaeluminosity,
	eiaeredChannel,
	eiaegreenChannel,
	eiaeblueChannel,
	eiaeallChannels
};

class CIaeHistogram : public CStatic
{
public:
	CIaeHistogram();

	//{{AFX_VIRTUAL(CIaeHistogram)
	//}}AFX_VIRTUAL

public:
	void SetImage(CDefaultImage *);
	void SetType(EIaeHistogram);
	void SetFlags(UINT flag) { m_uFlags = flag; };
	UINT GetFlags() { return m_uFlags; }

	virtual ~CIaeHistogram();

protected:
	CRect			m_cHistRect;

	UINT			m_uFlags;
	long			m_lMaxR, m_lMaxG, m_lMaxB, m_lMaxL;

	EIaeHistogram	m_iType;
	Histogram		m_cHistoData;
		
	//{{AFX_MSG(CIaeHistogram)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}

#endif
