
#ifndef _IAEPROFFT_H_
#define _IAEPROFFT_H_

#define POWER 16
#define IAE_FFTMAXSIZE (1<<POWER)

#if !VIS_OS_IS_NT
#include <VisCore.h>
#include <VisDisplay.h>
#endif // !VIS_OS_IS_NT

class CIaeProFFT 
{
	public:
		CIaeProFFT();

		void	FFT(CDefaultImage *, CDefaultImage *);

	protected:
		double	m_dCosTable[IAE_FFTMAXSIZE*5/8];
		double	*m_dSinTable;

		bool	PowerOf2(int);
		void	FFT(double *in_re, double *);
};

#endif