#include <stdlib.h>
#include <stdio.h>                /*  Marr-Hildreth.c  (or marrh.c) */
#include <math.h>
#define  PICSIZE 256
#define  MAXMASK 100

         int    pic[PICSIZE][PICSIZE];
         double outpic1x[PICSIZE][PICSIZE];
         double outpic1y[PICSIZE][PICSIZE];         
         double outpic2[PICSIZE][PICSIZE];
         int    edgeflag[PICSIZE][PICSIZE];
         double maskx[MAXMASK][MAXMASK];
         double masky[MAXMASK][MAXMASK];
         double convx[PICSIZE][PICSIZE];
         double convy[PICSIZE][PICSIZE];
         double ival[PICSIZE][PICSIZE];

main(argc,argv)
int argc;
char **argv;
{
        int     i,j,p,q,s,t,mr,centx,centy, garbage;
        double  maskval, xmaskval, ymaskval, sumx, sumy, sig, maxival, minival, maxval, ZEROTOL;
        FILE    *fo1, *fo2,*fp1, *fopen();
        char    *foobar;


        // File input
        argc--; argv++;
        foobar = *argv;
        fp1=fopen(foobar,"rb");

        // File output1
        argc--; argv++;
        foobar = *argv;
        fo1=fopen(foobar,"wb");
        fprintf(fo1, "P5\n256 256\n255\n");

        // File output2
        argc--; argv++;
        foobar = *argv;
        fo2=fopen(foobar,"wb");
        fprintf(fo2, "P5\n256 256\n255\n");

        // Sigma input
        argc--; argv++;
        foobar = *argv;
        sig = atof(foobar);

        // Zero Tolerance
        argc--; argv++;
        foobar = *argv;
        ZEROTOL = atof(foobar);

        mr = (int)(sig * 3);

        centx = (MAXMASK / 2);
        centy = (MAXMASK / 2);

        // This loop is a dirty adjustment to bypass the scanning of the file header
        for(i = 0; i < 20; i++)
          garbage = getc(fp1);

        // Get and store image
        for (i=0;i<256;i++)
        {
          for (j=0;j<256;j++)
          {
            pic[i][j]  =  getc (fp1);

          }
        }

        // Get first x & y-derivatives
        for (p=-mr;p<=mr;p++)
        {
          for (q=-mr;q<=mr;q++)
          {
            xmaskval = -(p*(exp(-(1/2)*(p/sig)*(p/sig))));
            ymaskval = -(q*(exp(-(1/2)*(q/sig)*(q/sig))));

            maskx[p+centy][q+centx] = xmaskval;
            masky[p+centy][q+centx] = ymaskval;
            
            /*
            mask[p][q] = (-p-q) * (1/(sig*sig*sig)) * exp(-((p*p) + (q*q)) / (2 * (sig*sig)));
            */
          }
        }

        // Convolution
        for (i=mr;i<=255-mr;i++)
        {
          for (j=mr;j<=255-mr;j++)
          {
            sumx = 0;
            sumy = 0;
            for (p=-mr;p<=mr;p++)
            {
              for (q=-mr;q<=mr;q++)
              {
                sumx += pic[i+p][j+q] * maskx[p+centy][q+centx];
                sumy += pic[i+p][j+q] * masky[p+centy][q+centx];
              }
            }
            outpic1x[i][j] = sumx;
            outpic1y[i][j] = sumy;
            convx[i][j] = sumx;
            convy[i][j] = sumy;
          }
        }

        //SQRT of Squares form Sobel
        maxival = 0;
        for (i=mr;i<256-mr;i++)
        {
          for (j=mr;j<256-mr;j++)
          {
            ival[i][j]=sqrt((double)((outpic1x[i][j]*outpic1x[i][j]) +
              (outpic1y[i][j]*outpic1y[i][j])));

            if (ival[i][j] > maxival){
              maxival = ival[i][j];

            }

          }
        }

        // Print image
        for (i=0;i<256;i++)
        {
          for (j=0;j<256;j++)
          {
            ival[i][j] = (ival[i][j] / maxival) * 255;
            fprintf(fo1,"%c",(char)((int)(ival[i][j])));
          }
        }

        
}