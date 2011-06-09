#  include <stdio.h>
#  include <stdint.h>
#  include <sys/stat.h>
#  include <fcntl.h>
#  include <stdlib.h>
#  include <math.h>
#  include <vector>
#  include <iostream>

#  include "ldror_char.hxx"
#  include "ldror.hxx"
#  include "lfindline.hxx"
#  include "lfindline_char.hxx"



using std::cout;
using std::endl;



/* =============================================================== */
void divide_image_short(uint16_t *I, uint16_t *I_high_left, uint16_t *I_high_right, uint16_t *I_low_left, uint16_t *I_low_right, int colsize, int rowsize)
/* =============================================================== */
{
    int x, y, a=0, b=0, c=0, d=0;

    for (y=0; y<(colsize/2); y++)
    {
        for (x=0 ; x<(rowsize/2); x++,a++)
        {
            I_high_left[a]=I[y*rowsize+x];
        }
        for ( /* x = rowsize/2 */ ; x<rowsize; x++, b++)
        {
            I_high_right[b]=I[y*rowsize+x];
        }
    }

    for (y=colsize/2; y<colsize; y++)
    {
        for (x=0; x<(rowsize/2); x++,c++)
        {
            I_low_left[c]=I[y*rowsize+x];
        }
        for ( /* x = rowsize/2 */ ; x<rowsize; x++,d++)
        {
            I_low_right[d]=I[y*rowsize+x];
        }
    }

}



/* =============================================================== */
void create_query_tree_short(uint16_t *I,tree *T, int colsize, int rowsize)
/* =============================================================== */
{
    int p;
    uint16_t x, y;
    int a, b;
    uint16_t *I1,*I2,*I3,*I4;
    int32_t p1, p2, p3, p4;
    a=colsize%2;
    b=rowsize%2;


    if (min(rowsize/2, colsize/2)==1)
    {
        //cout << "On est dans une feuille" << endl;
        p = 0;
        for (y=0; y<colsize; y++)
        {
            for (x=0; x<rowsize; x++)
            {
                if (I[y*rowsize+x]!=0)
                {
                    p++;
                    //cout << "Ah, enfin quelquechose: " << p << " " ;
                }
            }
        }

        AddElt(T,0, p);

    }
    else
    {
        T->high_left = (tree*)calloc(1,sizeof(tree));
        T->high_right= (tree*)calloc(1,sizeof(tree));
        T->low_left= (tree*)calloc(1,sizeof(tree));
        T->low_right= (tree*)calloc(1,sizeof(tree));

        T->high_left->rowsize=rowsize/2;
        T->high_right->rowsize=(rowsize+b)/2;
        T->low_left->rowsize=rowsize/2;
        T->low_right->rowsize=(rowsize+b)/2;

        T->high_left->colsize=colsize/2;
        T->high_right->colsize=colsize/2;
        T->low_left->colsize=(colsize+a)/2;
        T->low_right->colsize=(colsize+a)/2;

        I1=(uint16_t *)malloc(colsize*rowsize/4 * sizeof(uint16_t));
        I2=(uint16_t *)malloc(colsize*(rowsize+b)/4 * sizeof(uint16_t)) ;
        I3=(uint16_t *)malloc((colsize+a)*rowsize/4 * sizeof(uint16_t));
        I4=(uint16_t *)malloc((colsize+a)*(rowsize+b)/4 * sizeof(uint16_t));

        divide_image_short(I,I1,I2,I3,I4,colsize,rowsize);

        create_query_tree_short(I1,T->high_left,colsize/2,rowsize/2);
        create_query_tree_short(I2,T->high_right,colsize/2,(rowsize+b)/2);
        create_query_tree_short(I3,T->low_left,(colsize+a)/2,rowsize/2);
        create_query_tree_short(I4,T->low_right,(colsize+a)/2,(rowsize+b)/2);

        AddElt(T,0, query2(T));
        //cout <<  query2(T) << " ; " ;

    }

}




/* =============================================================== */
void get_line_short(uint16_t *I,tree *T, vector<int32_t> Bord2, vector<int32_t> Bord3, vector<int32_t> Bord1,
               vector<int32_t> Bord4,int rowsize, int colsize, int w)
/* =============================================================== */
{
    int i, j;
    int32_t y1, x1=0;
    int32_t best_y1=0, best_x2=0, best_y2=0;
    int32_t nb1=0,nb2=0,n;
    int32_t nb_max=0;




     for (i=0; i<Bord2.size(); i++)
    {
        y1=Bord2[i];

//        for (j=0; j<Bord1.size(); j++) //line between bord2 and bord1
//        {
//            nb=bresen(I,x1,y1,Bord1[j], 0, rowsize, w);
//            if (nb>nb_max)
//                {
//                    nb_max=nb;
//                    best_y1=y1;
//                    best_x2=Bord1[j];
//                    best_y2=rowsize;
//                }
//        }


        for (j=0; j<Bord3.size(); j++) //line between bord2 and bord3
        {

             nb1=count23(T,y1-w,Bord3[j]-w,colsize,rowsize);
             nb2=count23(T,y1+w,Bord3[j]+w,colsize,rowsize);
             if (abs(nb1-nb2)> nb_max)
                    {
                        nb_max=abs(nb1-nb2);
                        best_y1=y1;
                        best_x2=rowsize;
                        best_y2= Bord3[j];
                    }
        }

//        for (j=1; j<=Bord4.size(); j++) //line between bord2 and bord4
//        {
//            nb=bresen(I,x1,y1,Bord4[j], rowsize, rowsize, w);
//            if (nb>nb_max)
//                    {
//                        nb_max=nb;
//                        best_y1=y1;
//                        best_x2=Bord4[j];
//                        best_y2=rowsize;
//                    }
//        }
    }


    // Draw the bresenham best line in the image
    cout << "ligne entre le point (0,"<< best_y1<< ") et le point ("<< best_x2<<"," << best_y2 << ")"<< endl;
    //bresen_final(I, x1, best_y1, best_x2, best_y2, rowsize,colsize, w);
    mask_line(I, x1, best_y1, best_x2, best_y2, rowsize,colsize, w);

}



