
/////////////////////////////////////////////////////////////////////
// Class:			CIaeProConvolution
// Author:			James Matthews
// Date:			17th February, 2002.
//
// This class performs simple convolution and correlation operations
// on the picture. It uses CPixelNeighbours to increase its speed,
// but doesn't employ any other optimizations.
//

#ifndef _IAEPRO_CONVOLUTION_H_
#define _IAEPRO_CONVOLUTION_H_

#if !VIS_OS_IS_NT
#include <VisCore.h>
#include <VisDisplay.h>
#endif // !VIS_OS_IS_NT

#include "PixelOffset.h"

class IaeKernel 
{
	public:
		double *pdKernel;
		double	dDivide;
		UINT	uRadius;

	IaeKernel() { pdKernel = 0; }
	~IaeKernel() { if (pdKernel) delete [] pdKernel; pdKernel = 0; }
};

class CIaeProConvolution
{
	public:
		CIaeProConvolution() : m_pdKernel(NULL) {};
		~CIaeProConvolution() {};

		void Convolute(CDefaultImage &, CDefaultImage &);
		void Correlate(CDefaultImage &, CDefaultImage &);
		
		void SetKernel(double *pdKernel, UINT uRadius, double dDivide) 
			{ m_pdKernel = pdKernel; m_uRadius = uRadius; m_dDivide = dDivide; }
		void SetKernel(IaeKernel *pKernel)
			{ m_pdKernel = pKernel->pdKernel; m_uRadius = pKernel->uRadius; m_dDivide = pKernel->dDivide; }

	protected:
		double	m_dDivide;
		double	*m_pdKernel;

		UINT	m_uRadius;
		
		double	Kernel(int, int);

};

#endif