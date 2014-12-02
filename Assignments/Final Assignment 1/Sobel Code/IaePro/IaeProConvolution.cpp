
#include "stdafx.h"
#include "IaeProConvolution.h"

void CIaeProConvolution::Convolute(CDefaultImage &imgInput, CDefaultImage &imgOutput)
{
	ASSERT(m_pdKernel);
	ASSERT(imgInput.IsValid() && imgOutput.IsValid());

	CWaitCursor zzz;

	// Since the loops account for the neighbourhood deflating
	// we'll simply shrink the output, and not offset the input
	// using our pxo...sorta confusing, I know.

	int nbh = int(m_uRadius / 2);						// Neighbourhood.
	double r, g, b, k;
	CDefaultPixel px;
	CString status;

	int sx = imgInput.Width();
	int sy = imgInput.Height();
	int ir, ig, ib;

	CPixelOffset pxo(&imgInput, &imgOutput, CRect(nbh,nbh,nbh,nbh));

	for (int i=0; i<sx-(nbh*2); i++) {
		for (int j=0; j<sy-(nbh*2); j++) {
			r = 0, g = 0, b = 0;

			// Get the neighbourhood and apply the kernel
			for (int x=0; x<=nbh*2; x++) {
				for (int y=0; y<=nbh*2; y++) {
					k = Kernel(x, y);

					px = imgInput(pxo.Input(i+x-nbh,j+y-nbh));	// Urgh..,don't ask.

					r += k * px.R();
					g += k * px.G();
					b += k * px.B();
				}
			}

			r /= m_dDivide, b /= m_dDivide, g /= m_dDivide;

			ir = (r > 255) ? 255 : ((r < 0) ? 0 : int(r));
			ig = (g > 255) ? 255 : ((g < 0) ? 0 : int(g));
			ib = (b > 255) ? 255 : ((b < 0) ? 0 : int(b));

			imgOutput.Pixel(pxo.Output(i,j)).SetRGB(ir,ig,ib);
		}
	}	
}

double CIaeProConvolution::Kernel(int x, int y)
{
//	if (m_bConvolution) {
		return m_pdKernel[(m_uRadius - 1 - x) * m_uRadius + (m_uRadius - 1 - y)];
//	} else {
//		return m_pdKernel[x * m_iRadius + y];
//	}
}

