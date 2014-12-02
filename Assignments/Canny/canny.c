#include <stdlib.h>
#include <stdio.h>                /*  Marr-Hildreth.c  (or marrh.c) */
#include <math.h>
#define  PICSIZE 256
#define  MAXMASK 100

         int    pic[PICSIZE][PICSIZE];
         double outpic1x[PICSIZE][PICSIZE];
         double outpic1y[PICSIZE][PICSIZE];
         double outpic2[PICSIZE][PICSIZE];
         double edgeflag[PICSIZE][PICSIZE];
         double maskx[MAXMASK][MAXMASK];
         double masky[MAXMASK][MAXMASK];
         double convx[PICSIZE][PICSIZE];
         double convy[PICSIZE][PICSIZE];
         double final[PICSIZE][PICSIZE];
         double ival[PICSIZE][PICSIZE];

main(argc,argv)
int argc;
char **argv;
{
        int     i,j,p,q,s,t,mr,centx,centy, garbage;
        double  slope, maskval, xmaskval, ymaskval, sumx, sumy, sig, maxival, minival, maxval, ZEROTOL;
        FILE    *fo1, *fo2, *fo3, *fp1, *fopen();
        char    *foobar;
        int boolean = 1;
        double newmaxval;

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

        // File output3
        argc--; argv++;
        foobar = *argv;
        fo3=fopen(foobar,"wb");
        fprintf(fo3, "P5\n256 256\n255\n");

        // Sigma input
        argc--; argv++;
        foobar = *argv;
        sig = atof(foobar);

        // Zero Tolerance
        argc--; argv++;
        foobar = *argv;
        ZEROTOL = atof(foobar);
        minival = .35 * ZEROTOL;

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

        /*
        printf("Max mag value = %.2lf\n", maxival);
        minival = 0.35*maxival;
        printf("Min mag value = %.2lf\n", minival);
        */

        newmaxval = 0;
        // Print image
        for (i=0;i<256;i++)
        {
          for (j=0;j<256;j++)
          {
            ival[i][j] = (ival[i][j] / maxival) * 255;
            if(ival[i][j] > newmaxval)
              newmaxval = ival[i][j];
            fprintf(fo1,"%c",(char)((int)(ival[i][j])));
          }
        }

        /*
        printf("maxval = %.2lf\n", newmaxval);
        minival = .35 * newmaxval;
        printf("minival = %.2lf\n", minival);*/

        // Part two:
        // 4 Directions (NW-SE, N-S, SW-NE, E-W)
        // For each direction we will ask whether the center magnitude
          // exceeds the magnitude values of the neighbors on each side.
          // If the center value > greater than either, mark as peak.
        for(i = mr; i < 256-mr; i++){
          for(j = mr; j < 256-mr; j++){

            if((convx[i][j]) == 0.0){
              convx[i][j] = 0.00001;
            }
            slope = convy[i][j]/convx[i][j];
            if((slope <= .4142)&&(slope > -.4142)){
              if((ival[i][j] > ival[i][j-1])&&(ival[i][j] > ival[i][j+1])){
                edgeflag[i][j] = 255;
              }
            }
            else if((slope <= 2.4142)&&(slope > .4142)){
              if((ival[i][j] > ival[i-1][j-1])&&(ival[i][j] >ival[i+1][j+1])){
                edgeflag[i][j] = 255;
              }
            }
            else if((slope <= -.4142)&&(slope > -2.4142)){
              if((ival[i][j] > ival[i+1][j-1])&&(ival[i][j] > ival[i-1][j+1])){
                edgeflag[i][j] = 255;
              }
            }
            else{
              if((ival[i][j] > ival[i-1][j])&&(ival[i][j] > ival[i+1][j])){
                edgeflag[i][j] = 255;
              }
            }
          }
        }

        // Print image for fo2
        for (i=0;i<256;i++){
          for (j=0;j<256;j++){
            if(edgeflag[i][j] == 255){
              fprintf(fo2, "%c", (char)((int)(edgeflag[i][j])));
            }
            else{
              //edgeflag[i][j] = 0;
              fprintf(fo2, "%c", (char)((int)(edgeflag[i][j])));
            }
          }
        }

        /*
         * Part 3:
         * Applied to Mag image but only to places that have shown up as peaks.
         * Will have 2 thresholds, a HIGH and a LOW.
         * If Mag exceeds HI, pass to final.
         * If Mag is between HI and LO, then check if geographically
         *  adjacent to a position that is in final. If yes, pass to final.
         *  If Mag lower than LO, do not pass to final.*/

        printf("HI = %.2lf\n", ZEROTOL);
        printf("LO = %.2lf\n", minival);

        // Initialize final array to zeros
        for(i = 0; i < 256; i++)
          for(j = 0; j < 256; j++)
            final[i][j] = 0;

        for(i = 0; i < 256; i++){
          for(j = 0; j < 256; j++){
            // Only interested in pixels that show up as peaks
            if(edgeflag[i][j] == 255){
              // if mag exceeds high
              if(ival[i][j] > ZEROTOL){
                //printf("exceeded high");
                // pass to final
                final[i][j] = 255;
                edgeflag[i][j] = 0;
              }
              // if mag is less than low, don't pass to final
              else if(ival[i][j] < minival){
                  edgeflag[i][j] = 0;
                  final[i][j] = 0;
              }
            }
          }
        }

                boolean = 1;
                while(boolean == 1){
                  boolean = 0;
                  for(i = 0; i < 256; i++){
                    for(j = 0; j < 256; j++){
                      if(edgeflag[i][j] == 255){
                        for(p = -1; p <= 1; p++){
                          for(q = -1; q <= 1; q++){
                            if(final[i+p][j+q] == 255){
                              final[i][j] = 255;
                              edgeflag[i][j] = 0;
                              boolean = 1;
                            }
                          }
                        }
                      }
                    }
                  }
                }

        // Print image for fo3
        for (i=0;i<256;i++){
          for (j=0;j<256;j++){
            if(final[i][j] == 255)
              fprintf(fo3, "%c", (char)((int)(final[i][j])));
            else{
              final[i][j] = 0;
              fprintf(fo3, "%c", (char)((int)(final[i][j])));
            }
          }
        }


}
