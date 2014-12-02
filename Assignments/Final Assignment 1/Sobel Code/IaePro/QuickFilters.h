
/////////////////////////////////////////////////////////////////////
// Author:			James Matthews
// Date:			17th February, 2002.
//
// This file holds all the quick filters - that is, filters that are
// applied on a per-pixel basis, no neighbourhood data is required.
// IaePro uses the Vision SDK VisMapX functions, passing these structures
// as a functor. See the Vision SDK for more information.
//


#ifndef _QUICKFILTERS_H_
#define _QUICKFILTERS_H_

#if !VIS_OS_IS_NT
#include <VisCore.h>
#include <VisDisplay.h>
#endif // !VIS_OS_IS_NT

#include <math.h>

enum IaeGreyscales {
	igsBt709,
	igsY,
	igsRminusY,
};

inline round(double px) { return UCHAR(floor(px + 0.5)); }

struct GreyscaleBT709 {
	UCHAR value;
	inline void operator() (CDefaultPixel &px, CDefaultPixel &ds) 
		{ value = UCHAR(0.2125*px.R() + 0.7154*px.G() + 0.0721*px.B()); ds.SetRGB(value, value, value); }
};

struct GreyscaleY {
	UCHAR value;
	inline void operator() (CDefaultPixel &px, CDefaultPixel &ds) 
		{ value = UCHAR(0.299*px.R() + 0.587*px.G() + 0.114*px.B()); ds.SetRGB(value, value, value); }
};

struct GreyscaleRMY {
	UCHAR value;
	inline void operator() (CDefaultPixel &px, CDefaultPixel &ds)
		{ value = UCHAR(0.5*px.R() + 0.419*px.G() + 0.081*px.B()); ds.SetRGB(value, value, value); }
};

struct Invert {
	inline void operator() (CDefaultPixel &px, CDefaultPixel &ds) 
		{ ds.SetRGB(~px.R(), ~px.G(), ~px.B()); }
};

struct SimpleThreshold {
	UCHAR threshold, col;
	inline SimpleThreshold(UCHAR value) : threshold(value) {};
	inline void operator() (CDefaultPixel &px, CDefaultPixel &ds) 
		{ col = (px.R() < threshold) ? 0 : 255; ds.SetRGB(col,col,col); }
};

struct ImpulseNoise {
	UCHAR percent, perdiv2, val;
	inline ImpulseNoise(UCHAR per) : percent(per), perdiv2(per/2) { srand(unsigned(time(NULL))); };
	inline void operator() (CDefaultPixel &px, CDefaultPixel &ds) {
		val = rand() % 100;
		if (val < perdiv2) ds.SetRGB(255,255,255);  
		else if (val < percent) ds.SetRGB(0,0,0);
	}
};

struct RGBRotate {
	inline void operator() (CDefaultPixel &px, CDefaultPixel &ds) 
		{ ds.SetRGB(px.G(), px.B(), px.R()); }
};

struct Histogram {
	long hr[256], hg[256], hb[256], hl[256];

	inline Histogram() 
		{ memset(hr, 0, sizeof(hr)); memset(hg, 0, sizeof(hg)); memset(hb, 0, sizeof(hb)); memset(hl, 0, sizeof(hl)); }
	inline void operator() (CDefaultPixel &px) {
		hr[px.R()]++;
		hg[px.G()]++;
		hb[px.B()]++;
		hl[round(0.5*px.R() + 0.419*px.G() + 0.081*px.B())]++;	// Uses R - Y;
	}
};

struct Statistics {
	double mean;
	long pixels, sum, min, max, pxr, x, y;

	inline Statistics() { max = pixels = sum = 0; min = 256; };
	inline void operator() (CDefaultPixel &px) {
		pxr = px.R();
		sum += pxr;
		min = (pxr < min) ? pxr : min;
		max = (pxr > max) ? pxr : max;
	}
};

struct StdDev {
	double sum, stddev;
	Statistics *pStats;

	inline StdDev(Statistics *ps) { pStats = ps; sum = 0; }
	inline void operator() (CDefaultPixel &px) 
		{	sum += ((px.R() - pStats->mean)	* (px.R() - pStats->mean));	}
	inline void postCalculate()
		{	sum /= (pStats->pixels - 1); stddev = sqrt(sum);	}
};

struct IRgBy {
	double lr, lg, lb;
	inline double L(UCHAR x) { return 105.0 * log10(x + 1); }

	inline void operator() (CDefaultPixel &px, CVisRGBADoublePixel &ds) {
		lr = L(px.R()), lg = L(px.G()), lb = L(px.B());
		ds.SetRGB( (lr - lg < 0) ? 0 : lr - lg,
				   lb - (lg + lr) / 2.0f,
				   (lr+lg+lb) / 3.0f );
	}
};

struct IRgBy2Hue {
	UCHAR hue;
	inline void operator() (CVisRGBADoublePixel &px, CDefaultPixel &ds) {
		hue = UCHAR(atan2(px.G(),px.B()) * (180 / 3.141592654f));

		ds.SetRGB(hue, hue, hue);
	}
};

struct And {
	inline void operator() (CDefaultPixel &px1, CDefaultPixel &px2, CDefaultPixel &ds) {
		
		ds.SetRGB(px1.R() & px2.R(), px1.G() & px2.G(), px1.B() & px2.B());
	}
};

#endif