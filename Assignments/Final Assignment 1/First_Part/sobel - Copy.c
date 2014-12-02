#include <stdio.h>                          /* Sobel.c */
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define  BUFFER_SIZE  70
#define  TRUE          1
#define  FALSE         0

        int pic[256][256];
        int outpicx[256][256];
        int outpicy[256][256];
        int maskx[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}};
        void  readHeader(FILE* imgFin);
        int masky[3][3] = {{1,2,1},{0,0,0},{-1,-2,-1}};
        int numRows, numCols;
        int isComment(char *line);
        double maxival;
        void gradient_img(double image_values[256][256]);
        void threshold_low_img(double image_values[256][256]);
        void threshold_high_img(double image_values[256][256]);

main(argc,argv)
int argc;
char **argv;
{
        int i,j,p,q,mr,sum1,sum2;
        double threshold;
        FILE *fo1, *fo2, *fp1, *fopen();
        char *foobar;
        char *version;
        double ival[256][256];
        double ival1[256][256];
        double ival2[256][256];
        //argc--; argv++;
        //foobar = *argv;

        fp1=fopen("G:/Hybrid_Files/Dropbox/Documents/Classes/Classes Spring 2012-2013/Robotics Vision/Assignment/Assignment1/First_Part/20100428_dropbox_jpg.pgm","rb");
        readHeader(fp1);

        for (i=0;i<256;i++)
        { for (j=0;j<256;j++)
                {
                  pic[i][j]  =  getc (fp1);
                  pic[i][j]  &= 0377;
                }
        }

        mr = 1;
        for (i=mr;i<256-mr;i++)
        { for (j=mr;j<256-mr;j++)
          {
             sum1 = 0;
             sum2 = 0;
             for (p=-mr;p<=mr;p++)
             {
                for (q=-mr;q<=mr;q++)
                {
                   sum1 += pic[i+p][j+q] * maskx[p+mr][q+mr];
                   sum2 += pic[i+p][j+q] * masky[p+mr][q+mr];
                }
             }
             outpicx[i][j] = sum1;
             outpicy[i][j] = sum2;
          }
        }

        maxival = 0;
        for (i=mr;i<256-mr;i++)
        { for (j=mr;j<256-mr;j++)
          {
             ival[i][j]=sqrt((double)((outpicx[i][j]*outpicx[i][j]) +
                                      (outpicy[i][j]*outpicy[i][j])));
             ival1[i][j] = sqrt((double)((outpicx[i][j]*outpicx[i][j]) +
                                      (outpicy[i][j]*outpicy[i][j])));
             ival2[i][j] = sqrt((double)((outpicx[i][j]*outpicx[i][j]) +
                                      (outpicy[i][j]*outpicy[i][j])));

             if (ival[i][j] > maxival)
                maxival = ival[i][j];

           }
        }

        gradient_img(ival);
        threshold_low_img(ival1);
        threshold_high_img(ival2);
}

void threshold_high_img(double image_values[256][256]){

        FILE* threshold_high_file;
        threshold_high_file = fopen("G:/Hybrid_Files/Dropbox/Documents/Classes/Classes Spring 2012-2013/Robotics Vision/Assignment/Assignment1/First_Part/high_threshold.pgm", "wb");

        int i, j;
        fprintf(threshold_high_file,"P5\n");
        fprintf(threshold_high_file,"%d %d\n", numRows, numCols);
        fprintf(threshold_high_file,"255\n");


        for (i=0;i<256;i++)
        { for (j=0;j<256;j++)
            {
            image_values[i][j] = (image_values[i][j] / maxival) * 255;

            if ((int)(image_values[i][j]) > 1)
                    image_values[i][j] = 255;
                else image_values[i][j] = 0;


            fprintf(threshold_high_file,"%c",(char)((int)(image_values[i][j])));
            }
        }
}


void threshold_low_img(double image_values[256][256]){

        FILE* threshold_low_file;
        threshold_low_file = fopen("/home/oer/Dropbox/Documents/Classes/Classes Spring 2012-2013/Robotics Vision/Assignment/Assignment1/low_threshold.pgm", "wb");
        //threshold_high_file = fopen("/home/oer/Dropbox/Documents/Classes/Classes Spring 2012-2013/Robotics Vision/Assignment/Assignment1/chess2.pgm", "wb");

        int i, j;
        fprintf(threshold_low_file,"P5\n");
        fprintf(threshold_low_file,"%d %d\n", numRows, numCols);
        fprintf(threshold_low_file,"255\n");


        for (i=0;i<256;i++)
        { for (j=0;j<256;j++)
            {
            image_values[i][j] = (image_values[i][j] / maxival) * 255;

            if ((int)(image_values[i][j]) < 40)
                    image_values[i][j] = 0;
                else image_values[i][j] = 255;


            fprintf(threshold_low_file,"%c",(char)((int)(image_values[i][j])));
            }
        }
}


void gradient_img(double image_values[256][256]){

        FILE* fo1;

        fo1=fopen("/home/oer/Dropbox/Documents/Classes/Classes Spring 2012-2013/Robotics Vision/Assignment/Assignment1/gradient.pgm","wb");

        fprintf(fo1,"P5\n");
        fprintf(fo1,"%d %d\n", numRows, numCols);
        fprintf(fo1,"255\n");

        int i, j;

        for (i=0;i<256;i++)
        { for (j=0;j<256;j++)
            {
            image_values[i][j] = (image_values[i][j] / maxival) * 255;

            fprintf(fo1,"%c",(char)((int)(image_values[i][j])));
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
