#  include <stdio.h>
#  include <stdint.h>
#  include <sys/stat.h>
#  include <fcntl.h>
#  include <stdlib.h>
#  include <math.h>
#  include <vector>
#  include <iostream>

#  include "mcimage.h"
#  include "mccodimage.h"
#  include "lfindline.hxx"

using std::cout;
using std::endl;


/* =============================================================== */
    int32_t mean2(uint16_t *Image, int colsize, int rowsize)
/* =============================================================== */
 {
  int i, l;
  int64_t p=0, n=0;
  l=  colsize*rowsize-34*rowsize;


  for (i=rowsize ; i<l; i++)
   {
       if (Image[i]>98)
       {
           p+=abs(Image[i]);
           n++;
       }
   }

   if (n > 0) p=p/n;
   return(p);
 }


/* =============================================================== */
    int32_t mean3(uint16_t *Image, int colsize, int rowsize)
/* =============================================================== */
 {
  int i, l, m, n;
  int64_t p=0, q=0;
  l=  colsize*rowsize-34*rowsize;
  n= rowsize;

  for (i=n+1 ; i<l; i++)
   {
       if (Image[i]>0)
       {
           m=(Image[i-n-1]!=0)+(Image[i-n]!=0)+(Image[i-n+1]!=0)+(Image[i-1]!=0)+(Image[i+1]!=0)
                +(Image[i+n-1]!=0)+(Image[i+n]!=0)+(Image[i+n+1]!=0);
           p+=abs(Image[i])*m;
           q+=m;
       }
   }

   if (q > 0) p=p/q;
   return(p);
 }


/* =============================================================== */
    int32_t mean4(uint16_t *Image, int colsize, int rowsize)
/* =============================================================== */
 {
  int i, l, m, n;
  int64_t p=0, q=0;
  l=  colsize*rowsize-34*rowsize;
  n= rowsize;

  for (i=n+1 ; i<l; i++)
   {
       if (Image[i]>0)
       {
           m=(Image[i-n-1]!=Image[i])+(Image[i-n]!=Image[i])+(Image[i-n+1]!=Image[i])+(Image[i-1]!=Image[i])+(Image[i+1]!=Image[i])
                +(Image[i+n-1]!=Image[i])+(Image[i+n]!=Image[i])+(Image[i+n+1]!=Image[i]);
           p+=abs(Image[i])*m;
           q+=m;
       }
   }

   p=p/q;
   return(p);
 }


/* =============================================================== */
    void clean1(uint16_t *Image, int colsize, int rowsize)
/* =============================================================== */
 {
  int i;
  size_t l;
  int32_t p;
  p=mean2(Image, colsize, rowsize);
  //p=5000;
  l=colsize*rowsize;


 for (i=1 ; i<l; i++)
   {
        if (Image[i]-p>=0)
        {
            Image[i]=Image[i]-p;
        }
        else
        {
            Image[i]=0;
        }
   }


 }


/* =============================================================== */
void clean2(uint16_t *Image, int colsize, int rowsize)
/* =============================================================== */
 {
  int i;
  size_t l;
  int32_t p;
  //uint16_t *Image2;
  p=mean3(Image, colsize, rowsize);
  l=colsize*rowsize;
  //Image2=Image;

 for (i=1 ; i<l; i++)
   {
        if (Image[i]-p>=0)
        {
            Image[i]=Image[i]-p;
        }
        else
        {
            Image[i]=0;
        }
   }

    //return(Image2);
 }

 /* =============================================================== */
  void  clean3(uint16_t *Image, int colsize, int rowsize)
 /* =============================================================== */
 {
  int i;
  size_t l;
  int32_t p;
  p=mean4(Image, colsize, rowsize);
  l=colsize*rowsize;


    for (i=0 ; i<l; i++)
   {
        if (Image[i]-p>=0)
        {
            Image[i]=Image[i]-p;
        }
        else
        {
            Image[i]=0;
        }
   }
 }


 /* =============================================================== */
  void  clean4(uint16_t *Image, int colsize, int rowsize)
 /* =============================================================== */
 {
  int i, j;
  size_t l;
  int32_t m, y, p;
  p=mean3(Image, colsize, rowsize);

  for (i=0 ; i<colsize-1; i++)
  {
      for (j=0; j<rowsize-1; j++)
      {
          y= i*rowsize+j;
          if ( (y<colsize*rowsize) && Image[y]>0)
          {
//              m=Image[y-rowsize-1]+Image[y-rowsize]+Image[y-rowsize+1]+Image[y]+Image[y+1]+Image[y-1]
//                +Image[y+rowsize-1]+Image[y+rowsize]+Image[y+rowsize+1];
              m=Image[y]+Image[y+1]+Image[y+rowsize]+Image[y+rowsize+1];
              m=m/9;
              Image[y]=m;
          }
          else
          {
              Image[y]=0;
          }

       }
  }


 }


/* =============================================================== */
    int short2long(xvimage *image, xvimage *result)
/* =============================================================== */
 {
     // on transforme l'image de uint16_t a unint32_t pour pouvoir appliquer les opération de morphologie mathématique
    uint16_t *I1;
    int32_t *I2;
    int32_t typemax=VFF_TYP_4_BYTE;
    index_t i, rs, cs, ds, N;

    I1=USHORTDATA(image);
    rs=rowsize(image);
    cs=colsize(image);
    ds=depth(image);
    N=rs*cs*ds;

    I2 = SLONGDATA(result);

    for (i=0 ; i<N; i++)
    {
        I2[i]=(int32_t)I1[i];
    }

     /// !!!!!!!! jamais freeimage(*image);

     return 1;

 }


 /* =============================================================== */
    int long2short(xvimage *image, xvimage *result)
/* =============================================================== */
 {
     // on transforme l'image de uint16_t a unint32_t pour pouvoir appliquer les opération de morphologie mathématique
    xvimage *im;
    int32_t *I1;
    uint16_t *I2;
    int32_t typemax=VFF_TYP_2_BYTE;
    index_t i, rs, cs, ds, N;

    I1=SLONGDATA(image);
    rs=rowsize(image);
    cs=colsize(image);
    ds=depth(image);
    N=rs*cs*ds;

    I2 = USHORTDATA(result);

    for (i=0 ; i<N; i++)
    {
        I2[i]=(uint16_t)I1[i];
    }

     /// !!!!!!!!!  freeimage(*image);

    return 1;
 }


/* =============================================================== */
    void binning(xvimage **image)
/* =============================================================== */
 {
  xvimage *im;
  uint16_t *Image, *I2;
  int32_t typemax=VFF_TYP_2_BYTE;
  index_t i, j, rs, cs, ds, N;
  int32_t m, a=0, y;
  //I2=(uint16_t *)malloc(colsize*rowsize/4 * sizeof(uint16_t));
  Image=USHORTDATA(*image);
  rs=rowsize(*image);
  cs=colsize(*image);
  ds=depth(*image);
  N=rs*cs*ds;
  im = allocimage(NULL, rs/2, cs/2, ds, typemax);
  I2=USHORTDATA(im);

  for (i=0 ; i<cs; i+=2)
  {
      for (j=0; j<rs; j+=2)
      {
          y= i*rs+j;
          m=Image[y]+Image[y+1]+Image[y+rs]+Image[y+rs+1];
          m=m/4;
          I2[a]=m;
          a++;
      }
   }


  freeimage(*image);
  *image = im;
 }


/* =============================================================== */
    int32_t mean_long(int32_t *Image, int colsize, int rowsize)
/* =============================================================== */
 {
  int i, l;
  int64_t p=0, n=0;
  l=  colsize*rowsize;


  for (i=rowsize ; i<l; i++)
   {
       if (Image[i]>0)
       {
           p+=abs(Image[i]);
           n++;
       }
   }

   p=p/n;
   return(p);
 }


 /* =============================================================== */
    void clean_long(int32_t *Image, int colsize, int rowsize)
/* =============================================================== */
 {
  int i;
  size_t l;
  int32_t p;
  //p=80;
  p=mean_long(Image, colsize, rowsize);
  l=colsize*rowsize;


 for (i=0 ; i<l; i++)
   {
        if (Image[i]-p>0)
        {
            Image[i]=Image[i]-p;
        }
        else
        {
            Image[i]=0;
        }
   }


 }


/* =============================================================== */
  void  clean_long2(int32_t *Image, int colsize, int rowsize)
 /* =============================================================== */
 {
  int i, j;
  size_t l;
  int32_t m, y, p;


  for (i=0 ; i<colsize-1; i++)
  {
      for (j=0; j<rowsize-1; j++)
      {
          y= i*rowsize+j;
          if ( (y<colsize*rowsize) && Image[y]>0)
          {
              m=Image[y-rowsize-1]+Image[y-rowsize]+Image[y-rowsize+1]+Image[y]+Image[y+1]+Image[y-1]+Image[y+rowsize-1]+Image[y+rowsize]+Image[y+rowsize+1];
              m=m/9;
              Image[y]=m;
          }
          else
          {
              Image[y]=0;
          }

       }
  }


 }



 /* ============================================================================================== */
void get_edge_long(int32_t *Image, vector<int32_t> *Bord1, vector<int32_t> *Bord2, vector<int32_t> *Bord3,
                vector<int32_t> *Bord4,int rowsize, int colsize)
  /* =========================================================================================== */
{
    int a, b, c, d;
    int m, n;
    n=rowsize;
    m=colsize;
    int32_t q, r ,s, p;

//
//    // we look for a lit pixel on the first line
//    for (a=n, s=0; a<n*2; a++,s++)
//    {
//        if ( (Image[a]>0) && (Image[a-1]>0) && (Image[a+1]>0) && (Image[a-2]>0) && (Image[a+2]>0))
//        {
//            //cout << "point sur la premiere ligne "<< s << endl;
//            Bord1->push_back(s);
//        }
//    }

    // we look for a lit pixel on the first column
    for (c=450, q=0; c<m*n; c=c+n,q++)
     {
         if ( (Image[c]>0) && (Image[c-n]>0) && (Image[c+n]>0) && (Image[c-2*n]>0) && (Image[c+2*n]>0))
         {
             cout << "point sur la  premiere colonne "<< q << endl;
             Bord2->push_back(q);
         }
     }

    // we look for a lit pixel on the last colum
    for (d=n-1-45, r=0; d<n*m; d=d+n,r++)
     {
         if ( (Image[d]>0) && (Image[d-n]>0) && (Image[d+n]>0) && (Image[d-2*n]>0) && (Image[d+2*n]>0) )
         {
             cout << "point sur la derniere colonne "<< r << endl;
             Bord3->push_back(r);
         }
     }

////         // we look for a lit pixel on the last line
//    for (b=m*n-1-34*n,p=n-1-34; b>=(m-1)*n-34*n;b--,p--)
//     {
//         if ( (Image[b]>0) && (Image[b-1]>0) && (Image[b+1]>0) && (Image[d-2]>0) && (Image[d+2]>0) )
//         {
//             //cout << "point sur la  derniere ligne "<< p << endl;
//             Bord4->push_back(p);
//         }
//     }


}



/* =============================================================== */
void get_best_line_long(int32_t *I, vector<int32_t> Bord2, vector<int32_t> Bord3, vector<int32_t> Bord1,
               vector<int32_t> Bord4,int rowsize, int colsize, int w)
/* =============================================================== */
{
    int i, j;
    int32_t y1, x1=33;
    int32_t best_y1=0, best_x2=0, best_y2=0;
    double nb=0;
    double nb_max=0;


    for (i=0; i<Bord2.size(); i++)
    {
        y1=Bord2[i];

//        for (j=0; j<Bord1.size(); j++) //line between bord2 and bord1
//        {
//            nb=bresen_test_long(I,x1,y1,Bord1[j], 1, rowsize,colsize, w);
//            if (nb>nb_max)
//                {
//                    nb_max=nb;
//                    best_y1=y1;
//                    best_x2=Bord1[j];
//                    best_y2=1;
//                }
//        }


        for (j=0; j<Bord3.size(); j++) //line between bord2 and bord3
        {
             nb=bresen_test_long(I,x1,y1,rowsize, Bord3[j], rowsize-33,colsize, w);
             if (nb> nb_max)
                    {
                        nb_max=nb;
                        best_y1=y1;
                        best_x2=rowsize-33;
                        best_y2= Bord3[j];
                    }
        }

//        for (j=1; j<=Bord4.size(); j++) //line between bord2 and bord4
//        {
//            nb=bresen_test_long(I,x1,y1,Bord4[j], colsize-35, rowsize,colsize, w);
//            if (nb>nb_max)
//                    {
//                        nb_max=nb;
//                        best_y1=y1;
//                        best_x2=Bord4[j];
//                        best_y2=colsize-35;
//                    }
//        }
    }


    // Draw the bresenham best line in the image
     cout << "ligne entre le point (33,"<< best_y1<< ") et le point ("<< best_x2<<"," << best_y2 << ")"<< endl;
     //bresen_final(I2, x1, best_y1, best_x2, best_y2, rowsize,colsize, w);

}



/* ============================================================================================== */
double bresen_test_long(int32_t *Image,int32_t x1,int32_t y1,int32_t x2,int32_t y2,int rowsize, int colsize, int w)
/* ============================================================================================== */
{
/**
ARGUMENTS:
	int x1:      x starting coordinate
	int y1:      y starting corrdinate
	int x1:      x last coordinate
	int y1:      y last corrdinate
	int colsize:  width of the image
	int rowsize: height of the image

DISCRIPTION:
        This function makes a Bresenham line between two point.

HISTORY:
        Written by Hugues Talbot/Juliette Charpentier	31 mars 2011
TESTS:
        The core of this program has been written in 1991.
        Many tests including purify.
**/

    int32_t       dx, dy;
    int32_t       i, e, nb=0, dist=0;
    int32_t       incx, incy, inc1, inc2;
    int32_t       x, y;  /* the actual positions */

    dx = abs(x1 - x2);
    dy = abs(y1 - y2);


    incx = 1;
    if (x2 < x1)
    {
        incx = -1;
    }

    incy = 1;
    if (y2 < y1)
    {
        incy = -1;
    }

    /* starting position */
    if(x1 > x2)
      {
            x = x2;
            y = y2;
      }
    else
      {
            x = x1;
            y = y1;
      }




    if (dx > dy)
    {
        e = 2*dy ;
        inc1 = 2*(dy-dx);
        inc2 = 2*dy;
        for (i = 0 ; i < dx ; i++) {
            if (e >= 0) {
                nb+= query_long(Image,x,y,w,rowsize,colsize);
                dist++;
                y += incy;
                e += inc1;
            }
            else
            {
                nb+= query_long(Image,x,y,w,rowsize,colsize);
                dist++;
                e += inc2;
            }
             x += incx;
        }
    }
    else
    {
        e = 2*dx ;
        inc1 = 2*(dx-dy);
        inc2 = 2*dx;
        for (i = 0 ; i < dy ; i++)
        {
            if (e >= 0)
            {
                nb+= query_long(Image,x,y,w,rowsize,colsize);
                dist++;
                x += incx;
                e += inc1;
            }
            else
            {
                nb+= query_long(Image,x,y,w,rowsize,colsize);
                dist++;
                e += inc2;
            }
            y += incy;
        }

	}

    return((double)nb/dist);

    }



/* ============================================================================================== */
int32_t query_long(int32_t *Image,int32_t x,int32_t y,int w,int rowsize, int colsize)
/* ============================================================================================== */
{
int i;
int32_t nb=0;

for (i=-w; i<=w; i++)
{
    if ( ((y+i)*rowsize+x<rowsize*colsize) && ((y+i)*rowsize+x>=0)  )
    {
        if (Image[(y+i)*rowsize+x]>0)
        {
            nb=nb+1;
        }
    }
}

return(nb);
}



 /* ============================================================================================== */
uint16_t *get_edge(uint16_t *Image,uint16_t *Image2, vector<int32_t> *Bord1, vector<int32_t> *Bord2, vector<int32_t> *Bord3,
                vector<int32_t> *Bord4,int rowsize, int colsize)
  /* =========================================================================================== */
{
    int a, b, c, d;
    int m, n;
    n=rowsize;
    m=colsize;
    int32_t q, r ,s, p;

    //clean1(Image, colsize, rowsize);
    clean3(Image, colsize, rowsize);
    clean4(Image, colsize, rowsize);
    clean4(Image, colsize, rowsize);


    // we look for a lit pixel on the first line
//    for (a=n, s=0; a<n*2; a++,s++)
//    {
//        if ( (Image[a]>0) && (Image[a-1]>0) && (Image[a+1]>0) && (Image[a-2]>0) && (Image[a+2]>0))
//        {
//            //cout << "point sur la premiere ligne "<< s << endl;
//            Bord1->push_back(s);
//        }
//    }

    // we look for a lit pixel on the first column
    for (c=583, q=0; c<m*n; c=c+n,q++)
     {
         if ( (Image[c]>0) && (Image[c-n]>0) && (Image[c+n]>0) && (Image[c-2*n]>0) && (Image[c+2*n]>0))
         {
             cout << "point sur la  premiere colonne "<< q << endl;
             Bord2->push_back(q);
         }
     }

    // we look for a lit pixel on the last colum
    for (d=n-1-200, r=0; d<n*m; d=d+n,r++)
     {
         if ( (Image[d]>0) && (Image[d-n]>0) && (Image[d+n]>0) && (Image[d-2*n]>0) && (Image[d+2*n]>0) )
         {
             cout << "point sur la derniere colonne "<< r << endl;
             Bord3->push_back(r);
         }
     }

//         // we look for a lit pixel on the last line
//    for (b=m*n-1-34*n,p=n-1-34; b>=(m-1)*n-34*n;b--,p--)
//     {
//         if ( (Image[b]>0) && (Image[b-1]>0) && (Image[b+1]>0) && (Image[d-2]>0) && (Image[d+2]>0) )
//         {
//             //cout << "point sur la  derniere ligne "<< p << endl;
//             Bord4->push_back(p);
//         }
//    }

//  if ( (Bord2->empty()==1) || (Bord3->empty()==1))
//    {
////        clean4(Image2, colsize, rowsize);
////        clean2(Image2, colsize, rowsize);
//        a=0, b=0, c=0, d=0;
//
////        for (a=n, s=0; a<n*2; a++,s++)
////        {
////            if ( (Image[a]>0) && (Image[a-1]>0) && (Image[a+1]>0) && (Image[a-2]>0) && (Image[a+2]>0))
////            {
////                //cout << "point sur la premiere ligne "<< s << endl;
////                Bord1->push_back(s);
////            }
////        }
////
////         for (b=m*n-1-34*n,p=n-1-34; b>=(m-1)*n-34*n;b--,p--)
////         {
////             if ( (Image[b]>0) && (Image[b-1]>0) && (Image[b+1]>0) && (Image[d-2]>0) && (Image[d+2]>0) )
////             {
////                 //cout << "point sur la  derniere ligne "<< p << endl;
////                 Bord4->push_back(p);
////             }
////        }
//
//
//        for (c=60, q=0; c<m*n; c=c+n,q++)
//         {
//             if ( (Image2[c]>0) && (Image2[c-n]>0) && (Image2[c+n]>0) && (Image2[c-2*n]>0) && (Image2[c+2*n]>0))
//             {
//                 cout << "point sur la  premiere colonne "<< q << endl;
//                 Bord2->push_back(q);
//             }
//         }
//
//    // we look for a lit pixel on the last colum
//        for (d=n-1-35, r=0; d<n*m; d=d+n,r++)
//         {
//             if ( (Image2[d]>0) && (Image2[d-n]>0) && (Image2[d+n]>0) && (Image2[d-2*n]>0) && (Image2[d+2*n]>0) )
//             {
//                 cout << "point sur la derniere colonne "<< r << endl;
//                 Bord3->push_back(r);
//             }
//         }
//         return(Image2);
//    }
//    else if ( (Bord2->empty()==1) || (Bord3->empty()==1))
//    {
//        clean1(Image2, colsize, rowsize);
//        clean3(Image2, colsize, rowsize);
//        for (c=35, q=0; c<m*n; c=c+n,q++)
//         {
//             if ( (Image2[c]>0) && (Image2[c-n]>0) && (Image2[c+n]>0) && (Image2[c-2*n]>0) && (Image2[c+2*n]>0))
//             {
//                 //cout << "point sur la  premiere colonne "<< q << endl;
//                 Bord2->push_back(q);
//             }
//         }
//
//    // we look for a lit pixel on the last colum
//        for (d=n-1-35, r=0; d<n*m; d=d+n,r++)
//         {
//             if ( (Image2[d]>0) && (Image2[d-n]>0) && (Image2[d+n]>0) && (Image2[d-2*n]>0) && (Image2[d+2*n]>0) )
//             {
//                 //cout << "point sur la derniere colonne "<< r << endl;
//                 Bord3->push_back(r);
//             }
//         }
//         return(Image2);
//    }
//    else
//    {
        return(Image);
//    }

}



/* =============================================================== */
void get_best_line(uint16_t *I,uint16_t *I2, vector<int32_t> Bord2, vector<int32_t> Bord3, vector<int32_t> Bord1,
               vector<int32_t> Bord4,int rowsize, int colsize, int w)
/* =============================================================== */
{
    int i, j;
    int32_t y1, x1=33;
    int32_t best_y1=0, best_x2=0, best_y2=0;
    double nb=0;
    double nb_max=0;



     for (i=0; i<Bord2.size(); i++)
    {
        y1=Bord2[i];
        cout << "on en est au point: "<< y1 << endl;

//        for (j=0; j<Bord1.size(); j++) //line between bord2 and bord1
//        {
//            nb=bresen_test(I,x1,y1,Bord1[j], 1, rowsize,colsize, w);
//            if (nb>nb_max)
//                {
//                    nb_max=nb;
//                    best_y1=y1;
//                    best_x2=Bord1[j];
//                    best_y2=1;
//                }
//        }


        for (j=0; j<Bord3.size(); j++) //line between bord2 and bord3
        {
             nb=bresen_test(I,x1,y1,rowsize, Bord3[j], rowsize-33,colsize, w);
             if (nb> nb_max)
                    {
                        nb_max=nb;
                        best_y1=y1;
                        best_x2=rowsize-33;
                        best_y2= Bord3[j];
                    }
        }

//        for (j=1; j<=Bord4.size(); j++) //line between bord2 and bord4
//        {
//            nb=bresen_test(I,x1,y1,Bord4[j], colsize-35, rowsize,colsize, w);
//            if (nb>nb_max)
//                    {
//                        nb_max=nb;
//                        best_y1=y1;
//                        best_x2=Bord4[j];
//                        best_y2=colsize-35;
//                    }
//        }
    }


    // Draw the bresenham best line in the image
     cout << "ligne entre le point (33,"<< best_y1<< ") et le point ("<< best_x2<<"," << best_y2 << ")"<< endl;
     //bresen_final(I2, x1, best_y1, best_x2, best_y2, rowsize,colsize, w);

}

/* ============================================================================================== */
int32_t query(uint16_t *Image,int32_t x,int32_t y,int w,int rowsize, int colsize)
/* ============================================================================================== */
{
int32_t i;
int32_t nb=0;

for (i=-w; i<=w; i++)
{
    if ( ((y+i)*rowsize+x<rowsize*colsize) && ((y+i)*rowsize+x>=0)  )
    {
        if (Image[(y+i)*rowsize+x]>0)
        {
            nb=nb+1;
        }
    }
}

return(nb);
}




/* ============================================================================================== */
double bresen_test(uint16_t *Image,int32_t x1,int32_t y1,int32_t x2,int32_t y2,int rowsize, int colsize, int w)
/* ============================================================================================== */
{
/**
ARGUMENTS:
	int x1:      x starting coordinate
	int y1:      y starting corrdinate
	int x1:      x last coordinate
	int y1:      y last corrdinate
	int colsize:  width of the image
	int rowsize: height of the image

DISCRIPTION:
        This function makes a Bresenham line between two point.

HISTORY:
        Written by Hugues Talbot/Juliette Charpentier	31 mars 2011
TESTS:
        The core of this program has been written in 1991.
        Many tests including purify.
**/

    int32_t       dx, dy;
    int32_t       i, e, nb=0, dist=0;
    int32_t       incx, incy, inc1, inc2;
    int32_t       x, y;  /* the actual positions */

    dx = abs(x1 - x2);
    dy = abs(y1 - y2);
    //dist= sqrt(dx^2+dy^2); // Ca marche pas avec cette def de dist

    incx = 1;
    if (x2 < x1)
    {
        incx = -1;
    }

    incy = 1;
    if (y2 < y1)
    {
        incy = -1;
    }

    /* starting position */
    if(x1 > x2)
      {
            x = x2;
            y = y2;
      }
    else
      {
            x = x1;
            y = y1;
      }




    if (dx > dy)
    {
        e = 2*dy ;
        inc1 = 2*(dy-dx);
        inc2 = 2*dy;
        for (i = 0 ; i < dx ; i++) {
            if (e >= 0) {
                nb+= query(Image,x,y,w,rowsize,colsize);
                dist++;
                y += incy;
                e += inc1;
            }
            else
            {
                nb+= query(Image,x,y,w,rowsize,colsize);
                e += inc2;
                dist++;
            }
             x += incx;
        }
    }
    else
    {
        e = 2*dx ;
        inc1 = 2*(dx-dy);
        inc2 = 2*dx;
        for (i = 0 ; i < dy ; i++)
        {
            if (e >= 0)
            {
                nb+= query(Image,x,y,w,rowsize,colsize);
                dist++;
                x += incx;
                e += inc1;
            }
            else
            {
                nb+= query(Image,x,y,w,rowsize,colsize);
                dist++;
                e += inc2;
            }
            y += incy;
        }

	}

    return((double)nb/dist);

    }



/* ============================================================================================== */
void black_line(uint16_t *Image,int32_t x,int32_t y,int w,int rowsize, int colsize)
/* ============================================================================================== */
{
int i;

for (i=-w; i<=w; i++)
{
    // (y*rowsize+x+i)<sizeof(Image)
    if  ((y+i)*rowsize+x<rowsize*colsize)
    {
        Image[(y+i)*rowsize+x]=0;
    }
}

}






/* ============================================================================================== */
void bresen_final(uint16_t *Image,int32_t x1,int32_t y1,int32_t x2,int32_t y2,int rowsize,int colsize, int w)
/* ============================================================================================== */
{
/**
ARGUMENTS:
	int x1:      x starting coordinate
	int y1:      y starting corrdinate
	int x1:      x last coordinate
	int y1:      y last corrdinate
	int colsize:  width of the image
	int rowsize: height of the image

DISCRIPTION:
        This function makes a Bresenham line between two point.

HISTORY:
        Written by Hugues Talbot/Juliette Charpentier	31 mars 2011
TESTS:
        The core of this program has been written in 1991.
        Many tests including purify.
**/

    int32_t       dx, dy;
    int32_t       i, e, nb=0;
    int32_t       incx, incy, inc1, inc2;
    int32_t       x, y;  /* the actual positions */

    dx = abs(x1 - x2);
    dy = abs(y1 - y2);

    incx = 1;
    if (x2 < x1) incx = -1;
    incy = 1;
    if (y2 < y1) incy = -1;

    /* starting position */
    if(x1 > x2)
      {
            x = x2;
            y = y2;
      }
    else
      {
            x = x1;
            y = y1;
      }




        if (dx > dy)
        {
            e = 2*dy - dx;
            inc1 = 2*(dy-dx);
            inc2 = 2*dy;
            for (i = 0 ; i < dx ; i++) {
                if (e >= 0)
                {
                    black_line(Image,x,y,w,rowsize,colsize);
                    y += incy;
                    e += inc1;
                }
                else
                {
                    black_line(Image,x,y,w,rowsize,colsize);
                    e += inc2;
                }
                x += incx;
            }
        }
        else
        {
            e = 2*dx- dy;
            inc1 = 2*(dx-dy);
            inc2 = 2*dx;
            for (i = 0 ; i < dy ; i++)
            {
                if (e >= 0)
                {
                    black_line(Image,x,y,w,rowsize,colsize);
                    x += incx;
                    e += inc1;
                }
                else
                {
                    black_line(Image,x,y,w,rowsize,colsize);
                    e += inc2;
                }
                y += incy;
            }

        }

    }



/* =============================================================== */
void mask_line(uint16_t *Image,int32_t x1,int32_t y1,int32_t x2,int32_t y2,int rowsize,int colsize, int w)
/* =============================================================== */
{
    double a, b, c;
    int x, y;
    a=(double)(y2-y1)/(double)(x2-x1);
    b=(double)y2-(double)a*x2;


    for (y=0; y<colsize; y++)
    {
        for (x=0; x<rowsize; x++)
        {
            if  ( (y<a*x+b+w) && (y>a*x+b-w) )
            {
                Image[y*rowsize+x]=10000;
            }
            else
            {
                Image[y*rowsize+x]=0;
            }
        }
    }
}



    /* =============================================================== */
    int new_edge1(uint16_t *Image,int colsize, int rowsize)
/* =============================================================== */
{
    int i, l, n=colsize;
    int b1=0, b4=0;
    l=  colsize*rowsize;

    for (i=rowsize/2 ; i<l; i=i+rowsize, b1++)
    {
       if (Image[i]>98)
       {
           break;
       }
    }


    for (i=l-rowsize/2 ; i>0; i-=rowsize, b4++)
    {
       if (Image[i]>98)
       {
           break;
       }
    }

    n=n-b4-b1;
    return(n);

}


/* =============================================================== */
    int new_edge2(uint16_t *Image, int colsize, int rowsize)
/* =============================================================== */
{
    int i, j, l, n=rowsize;
    int b2=0, b3=0;
    l=  colsize*rowsize;


    for (i=(colsize/2)*rowsize ; i<l; i++, b2++)
    {
       if (Image[i]>98)
       {
           break;
       }
    }

    for (i=(colsize/2)*rowsize ; i>0; i--, b3++)
    {
       if (Image[i]>98)
       {
           break;
       }
    }


    n=n-b3-b2;
    return(n);

}
