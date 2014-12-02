
#include "stdafx.h"
#include "IaeProFft.h"
#include "PixelOffset.h"
#include "IaeProDebug.h"
#include "resource.h"

#include <math.h>

#define PI 3.141592653589793238462643383279502884197169399375105820974 // Think that's accurate enough?

CIaeProFFT::CIaeProFFT()
{
	for (int i=0; i<IAE_FFTMAXSIZE*5/8; i++)
	{
		double theta = -2.0 * PI * double(i) / double(IAE_FFTMAXSIZE/2);
		
		m_dCosTable[i] = cos(theta);
	}

	m_dSinTable = &(m_dCosTable[IAE_FFTMAXSIZE/8]);
}

int br(const int i)
{
	int j, k = 0;

	for (j=0; j<POWER; j++)
	if ((i >> j) & 1)
		k = k | (1<<(POWER-j-1));

	return k;
}

bool CIaeProFFT::PowerOf2(int num)
{
	int t = 2;
	for (int i=0; i<POWER; i++) {
		if (num == t) return true;

		t <<= 1;
	}

	return false;
}

void CIaeProFFT::FFT(CDefaultImage *imgInput, CDefaultImage *imgOutput)
{
	ASSERT(imgInput && imgOutput && 
		   imgInput->IsValid() && imgOutput->IsValid());

	double *real = new double[65536], 
		   *imag = new double[65536];

	IaeProAssert(real && imag, IDS_FFTMEM);

	memset(real, 0, sizeof(double) * 65536);
	memset(imag, 0, sizeof(double) * 65536);

	int ind = 0;
	int sx = imgOutput->Width();
	int sy = imgOutput->Height();

	IaeProAssert(PowerOf2(sx) && PowerOf2(sy), IDS_FFTDIMERROR);

	CPixelOffset pxo(imgInput, imgOutput, CRect(0,0,0,0));

	for (int i=0; i<sx; i++) {
		for (int j=0; j<sy; j++) {
			real[br(ind++)] = double(imgInput->Pixel(pxo.Input(i,j)).R());
		}
	}

	FFT(&(real[0]), &(imag[0]));

	BYTE grey;
	ind = 0;

	double C = 255 / log10(sqrt(real[0] * real[0] + imag[0]*imag[0])+1);

	for (i=0; i<sx; i++) {
		for (int j=0; j<sy; j++) {
			grey = BYTE(C * log10(sqrt(real[ind] * real[ind] + imag[ind]*imag[ind])+1));
			
			imgOutput->Pixel(pxo.Output(i,j)).SetRGB(grey, grey, grey);

			ind++;
		}
	}

	delete [] real;
	delete [] imag;
}

/////////////////////////////////////////////////////////////////////
// Code based on FFT code by Emil Mikulic at:
//    http://members.optushome.com.au/darkmoon7/code/fft/fft3.c
//

void CIaeProFFT::FFT(double *in_re, double *in_im)
{
	int blocks, points, i, j, offset;
	double t_re, t_im, top_r, top_i, btm_r, btm_i;

	for (blocks=IAE_FFTMAXSIZE/2, points=2; blocks>=1; blocks/=2, points*=2) {
		for (i=0; i<blocks; i++)
		{
			offset = points * i;

			for (j=0; j<points/2; j++)
			{
				t_re = m_dCosTable[j * blocks / 2];
				t_im = m_dSinTable[j * blocks / 2];

				top_r = in_re[j+offset];
				top_i = in_im[j+offset];

				btm_r = in_re[j+points/2+offset]*t_re -
					in_im[j+points/2+offset]*t_im;

				btm_i = in_re[j+points/2+offset]*t_im +
					in_im[j+points/2+offset]*t_re;			

				in_re[j+offset] = top_r + btm_r;
				in_im[j+offset] = top_i + btm_i;

				in_re[j+points/2+offset] = top_r - btm_r;
				in_im[j+points/2+offset] = top_i - btm_i;
			}
		}
	}
}
