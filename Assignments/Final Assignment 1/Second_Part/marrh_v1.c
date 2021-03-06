#include <stdio.h>                  /*  Marr-Hildreth.c  (or marrh.c) */
#include <math.h>


#include <stdlib.h>
#include <string.h>
#include <ctype.h>



#define  PICSIZE 256
#define  MAXMASK 100
#define  BUFFER_SIZE  70
#define  TRUE          1
#define  FALSE         0

         int    pic[PICSIZE][PICSIZE];
         double outpic1[PICSIZE][PICSIZE];
         double outpic2[PICSIZE][PICSIZE];
         double mag[PICSIZE][PICSIZE];
         double final[PICSIZE][PICSIZE];
         int    edgeflag[PICSIZE][PICSIZE];
         double xmask[MAXMASK][MAXMASK];
         double ymask[MAXMASK][MAXMASK];
         double xconv[PICSIZE][PICSIZE];
         double conv[PICSIZE][PICSIZE];
         double yconv[PICSIZE][PICSIZE];
         double cand[PICSIZE][PICSIZE];
         double   histogram[PICSIZE];
         double percentPic[PICSIZE][PICSIZE];
         int slope;
         int numRows, numCols;
         void  readHeader(FILE* imgFin);
         int isComment(char *line);

main(argc,argv)
int argc;
char **argv;
{
        int     i,j,p,q,s,t,mr,centx,centy,less, HIGH, LOW;
        int uno = 1;
        double  xmaskval, ymaskval,sum,sumx,sumy,sig,maxival,minival,maxval,ZEROTOL, percent, total_amount_numberOfPixs, number_higherpixs,flag;
        FILE    *fo1, *fo2,*fp1, *fopen(), *fo3;
        char    *foobar;

        printf("Please enter a percert (decimal notation) : ");
        scanf("%lf", &percent);

        argc--; argv++;
        foobar = *argv;
        fp1=fopen("/Users/mannymartinez/Desktop/Assignment1/Second_Part/garb34.pgm","rb");
        readHeader(fp1);

        argc--; argv++;
        foobar = *argv;
        fo1=fopen("/Users/mannymartinez/Desktop/Assignment1/Second_Part/garb34_1.pgm","wb");
        fo2=fopen("/Users/mannymartinez/Desktop/Assignment1/Second_Part/garb34_2.pgm","wb");
        fo3=fopen("/Users/mannymartinez/Desktop/Assignment1/Second_Part/garb34_3.pgm","wb");

        argc--; argv++;
        foobar = *argv;
        //fo2=fopen("/home/oer/Dropbox/Documents/Classes/Classes Spring 2012-2013/Robotics Vision/Assignment/Assignment1/garb34_2.pgm","wb");

        argc--; argv++;
        foobar = *argv;
        //sig = atof(foobar);
        sig = 1.0;

        argc--; argv++;
        foobar = *argv;
        ZEROTOL = atof(foobar);

        //flexible size masks
        mr = (int)(sig * 3);
        centx = (MAXMASK / 2);
        centy = (MAXMASK / 2);

        for (i=0;i<256;i++)
        { for (j=0;j<256;j++)
                {
                  pic[i][j]  =  getc (fp1);
                  pic[i][j] &= 0377;
                }
        }

        //derivatives to get masks
        for (q=-mr;q<=mr;q++)
        {  for (p=-mr;p<=mr;p++)
           {
              xmaskval = (-1 * q ) * (exp(-1*(((p*p)+(q*q))/(2*(sig*sig)))));
              (xmask[p+centy][q+centx]) = xmaskval;

              ymaskval = (-1 * p ) * (exp(-1*(((p*p)+(q*q))/(2*(sig*sig)))));
              (ymask[p+centy][q+centx]) = ymaskval;
           }
        }

        //convolution
        for (i=mr;i<256-mr;i++)
        { for (j=mr;j<256-mr;j++)
          {
             sumx = 0;
             sumy = 0;
             for (q=-mr;q<=mr;q++)
             {
                for (p=-mr;p<=mr;p++)
                {
                   sumx += pic[i+p][j+q] * xmask[p+centy][q+centx];
                   sumy += pic[i+p][j+q] * ymask[p+centy][q+centx];
                }
             }
             xconv[i][j] = sumx;
             yconv[i][j] = sumy;
          }
        }

        maxival = 0;

        for (i=mr;i<256-mr;i++)
        { for (j=mr;j<256-mr;j++)
          {
             mag[i][j]=sqrt((double)((xconv[i][j]*xconv[i][j]) + (yconv[i][j]*yconv[i][j])));

             if (mag[i][j] > maxival)
                maxival = mag[i][j];
           }
        }

//        for (i=mr;i<256-mr;i++)
//        { for (j=mr;j<256-mr;j++)
//          {
//              histogram[(int)mag[i][j]]++ ;
//          }
//        }
//
//
//
//        number_higherpixs = percent * numRows * numCols;
//
//        printf("total_amount_numberOfPixs: %lf \n", total_amount_numberOfPixs);
//        printf("percent: %lf \n", percent);
//        printf("number_higherpixs: %lf \n", number_higherpixs);


        fprintf(fo1,"P5\n");
        fprintf(fo1,"%d %d\n", numRows, numCols);
        fprintf(fo1,"255\n");

        for (i=0;i<256;i++)
        { for (j=0;j<256;j++)
            {

//            if (mag[i][j]< 50)
                mag[i][j] = (mag[i][j] / maxival) * 255;
            fprintf(fo1,"%c",(char)((int)(mag[i][j])));
            }
        }




        for(i=mr;i<256-mr;i++){
           for(j=mr;j<256-mr;j++){


              if(xconv[i][j] == 0.0) {
                 xconv[i][j] = 0.000001;
               }

              slope = yconv[i][j]/xconv[i][j];

                if( (slope <= 0.4142)&&(slope > -0.4142)){

                 if((mag[i][j] > mag[i][j-1])&& (mag[i][j] > mag[i][j+1])){
                    cand[i][j] = 255;
                  }
               }

              else if( (slope <= 2.4142)&&(slope > 0.4142)){
                 if((mag[i][j] > mag[i+1][j+1])&&(mag[i][j] > mag[i-1][j-1])){
                     cand[i][j] = 255;
                   }
               }

                else if( (slope <= -0.4142)&&(slope > -2.4142)){
                 if((mag[i][j] > mag[i-1][j+1])&&(mag[i][j] > mag[i+1][j-1])){
                     cand[i][j] = 255;
                   }

               }else{
                 if((mag[i][j] > mag[i-1][j])&&(mag[i][j] > mag[i+1][j])){
                     cand[i][j] = 255;
                  }
                }
             }
          }


        for(i=0; i< 256; i++){
            for(j=0; j< 256; j++){

                        if (cand[i][j] == 255){

                            if (mag[i][j] > 110){
                                    cand[i][j] = 0;
                                    final[i][j] = 255;
                            }

                            else if (mag[i][j]< 40){
                                cand[i][j] = 0;
                                final[i][j] = 0;
                            }
                        }
                    }
                }

                int middlepart = TRUE;

                while (middlepart){

                    middlepart = FALSE;

                    for(i=0; i<256; i++){
                        for(j=0; j<256; j++){

                            if (cand[i][j] == 255){

                                for (p=-uno;p<=uno;p++)
                                {  for (q=-uno;q<=uno;q++)
                                    {
                                        if (final[i+p][j+q] == 255){
                                            cand[i][j] = 0;
                                            final[i][j] = 255;
                                            middlepart = TRUE;
                                         }
                                    }
                                }
                            }
                        }
                    }
                }


        fprintf(fo2,"P5\n");
        fprintf(fo2,"%d %d\n", numRows, numCols);
        fprintf(fo2,"255\n");

        for (i=0;i<256;i++)
        { for (j=0;j<256;j++)
            {
                fprintf(fo2,"%c",(char)((int)(final[i][j])));
            }

        }
}


void readHeader(FILE* imgFin)
{
        int  haveReadImgID   = FALSE;
        int  haveReadImgSize = FALSE;
        int  haveReadMaxVal  = FALSE;
        char line[BUFFER_SIZE];

        while(!(haveReadImgID && haveReadImgSize && haveReadMaxVal))
        {
                fgets(line, BUFFER_SIZE, imgFin);

                if((strlen(line) == 0) || (strlen(line) == 1))
                        continue;

                if(isComment(line))
                        continue;

                if(!(haveReadImgID))
                {
                        //readImgID(line);
                        haveReadImgID = TRUE;
                }
                else if(!(haveReadImgSize))
                {
                        sscanf(line, "%d %d", &numRows, &numCols);
                        //readImgSize(line);
                        haveReadImgSize = TRUE;
                }
                else if(!(haveReadMaxVal))
                {
                        //readMaxVal(line);
                        haveReadMaxVal = TRUE;
                }
        }
}

int isComment(char *line)
{
        if(line[0] == '#')
                return(TRUE);

        return(FALSE);
}
