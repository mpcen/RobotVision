#include <stdlib.h>
#include <stdio.h>                /*  Marr-Hildreth.c  (or marrh.c) */
#include <math.h>
#define  PICSIZE 256
#define  MAXMASK 100

         int    pic[PICSIZE][PICSIZE];
         double outpic1[PICSIZE][PICSIZE];
         double outpic2[PICSIZE][PICSIZE];
         int    edgeflag[PICSIZE][PICSIZE];
         double maskx[MAXMASK][MAXMASK];
         double masky[MAXMASK][MAXMASK];
         double mask[MAXMASK][MAXMASK];
         double conv[PICSIZE][PICSIZE];

main(argc,argv)
int argc;
char **argv;
{
        int     i,j,p,q,s,t,mr,centx,centy;
        double  maskval, maskvalx, maskvaly,sum,sig,maxival,minival,maxval,ZEROTOL;
        FILE    *fo1, *fo2,*fp1, *fopen();
        char    *foobar;

        argc--; argv++;
        foobar = *argv;
        fp1=fopen(foobar,"rb");

        argc--; argv++;
        foobar = *argv;
        fo1=fopen(foobar,"wb");

        fprintf(fo1, "P5\n256 256\n255\n");

        argc--; argv++;
        foobar = *argv;
        fo2=fopen(foobar,"wb");

        fprintf(fo2, "P5\n256 256\n255\n");

        argc--; argv++;
        foobar = *argv;
        sig = atof(foobar);

        argc--; argv++;
        foobar = *argv;
        ZEROTOL = atof(foobar);

        mr = (int)(sig * 3);

        centx = (MAXMASK / 2);
        centy = (MAXMASK / 2);

        for (i=0;i<256;i++)
        { for (j=0;j<256;j++)
                {
                  pic[i][j]  =  getc (fp1);
                  pic[i][j]  &= 0377;
                }
        }

        for (p=-mr;p<=mr;p++)
        {  for (q=-mr;q<=mr;q++)
           {
            maskvalx = -(p*(exp(-(1/2)*(p/sig)*(p/sig))));
            maskvaly = -(q*(exp(-(1/2)*(q/sig)*(q/sig))));
            (mask[p+centy][q+centx]) = maskvalx;
           }
        }



        for (i=mr;i<=255-mr;i++)
        { for (j=mr;j<=255-mr;j++)
          {
             sum = 0;
             for (p=-mr;p<=mr;p++)
             {
                for (q=-mr;q<=mr;q++)
                {
                   sum += pic[i+p][j+q] * mask[p+centy][q+centx];
                }
             }
             outpic1[i][j] = sum;
             conv[i][j] = sum;
          }
        }

        
}