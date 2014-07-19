//#ifndef RORPO_HPP_INCLUDED
//#define RORPO_HPP_INCLUDED

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <omp.h>
#include <cstdlib>
#include <omp.h>

#include "Path_Opening.hpp"

#include "mcimage.h"
#include "mccodimage.h"
#include "lgeodesic.h"


template<typename PixType>
PixType* diff_buffer(PixType* I1, PixType* I2, int image_size)
{
    PixType* res= new PixType[image_size];
    for(int i=0; i<image_size; i++)
    {
        *(res+i)=*(I1+i)-*(I2+i);
    }
    return res;
}

template<typename T>
T* geodilat_modif(T* G, T* R, int connex, int niter, int dimx, int dimy, int dimz)
{
    struct xvimage* imageG;
    struct xvimage* imageR;
    struct xvimage* temp;
    int32_t typepixel;
    
	if (sizeof(T)==1)
   		typepixel = VFF_TYP_1_BYTE;
   	else if (sizeof(T)==2)
		typepixel = VFF_TYP_2_BYTE;
	else if (sizeof(T)==4)
		typepixel = VFF_TYP_4_BYTE;
	else
		return NULL;

    imageG=allocheader(NULL,dimx,dimy,dimz,typepixel);
    imageG->image_data=G;

    imageR=allocheader(NULL,dimx,dimy,dimz,typepixel);
    imageR->image_data=R;

    temp=copyimage(imageG);

    lgeodilat(temp,imageR,connex,niter);

    T* res=(T*)(temp->image_data);

    free(imageR);
    free(imageG);
    free(temp);

    /*for (int i=0;i<dimx*dimy*dimz;i++)
        {
            std::cout<<"res["<<i<<"]="<< static_cast<int>(res[i])<<std::endl;

        }*/

   return res;
}

template<typename T>
T* max_alloc(T* I1, T* I2, int dimx, int dimy, int dimz)
// The result is stored in an other buffer
{
    struct xvimage * image1;
    struct xvimage * image2;
    struct xvimage * image3;
    int32_t typepixel;
    
	if (sizeof(T)==1)
   		typepixel = VFF_TYP_1_BYTE;
   	else if (sizeof(T)==2)
		typepixel = VFF_TYP_2_BYTE;
	else if (sizeof(T)==4)
		typepixel = VFF_TYP_4_BYTE;
	else
		return NULL;

    image1=allocheader(NULL,dimx,dimy,dimz,typepixel);
    image1->image_data=I1;

    image2=allocheader(NULL,dimx,dimy,dimz,typepixel);
    image2->image_data=I2;

    image3=copyimage(image1);

    lmax(image3,image2);

    T* res=(T*)(image3->image_data);

    free(image1);
    free(image2);
    free(image3);

   return res;
}

template<typename T>
int max_crush(T* I1, T* I2, int dimx, int dimy, int dimz)
//The result is stored in I1
{
    struct xvimage * image1;
    struct xvimage * image2;
    int32_t typepixel;
    
    
	if (sizeof(T)==1)
   		typepixel = VFF_TYP_1_BYTE;
   	else if (sizeof(T)==2)
		typepixel = VFF_TYP_2_BYTE;
	else if (sizeof(T)==4)
		typepixel = VFF_TYP_4_BYTE;
	else
		return 1;
		

    image1=allocheader(NULL,dimx,dimy,dimz,typepixel);
    image1->image_data=I1;

    image2=allocheader(NULL,dimx,dimy,dimz,typepixel);
    image2->image_data=I2;

    lmax(image1,image2);

	free(image1);
	free(image2);
	return 0;
}

template<typename T>
T* min_alloc(T* I1, T* I2, int dimx, int dimy, int dimz)
{
    struct xvimage * image1;
    struct xvimage * image2;
    struct xvimage * image3;
    int32_t typepixel;

	if (sizeof(T)==1)
   		typepixel = VFF_TYP_1_BYTE;
   	else if (sizeof(T)==2)
		typepixel = VFF_TYP_2_BYTE;
	else if (sizeof(T)==4)
		typepixel = VFF_TYP_4_BYTE;
	else
		return NULL;
		
    image1=allocheader(NULL,dimx,dimy,dimz,typepixel);
    image1->image_data=I1;

    image2=allocheader(NULL,dimx,dimy,dimz,typepixel);
    image2->image_data=I2;

    image3=copyimage(image1);
    // lmin ecrase image3->image_data
    lmin(image3,image2);

    T* res=(T*)(image3->image_data);

    free(image1);
    free(image2);
    free(image3);

   return res;
}

//The result is stored in I1
template<typename T>
int min_crush(T* I1, T* I2, int dimx, int dimy, int dimz)
{
    struct xvimage * image1;
    struct xvimage * image2;
    int32_t typepixel;
    
	if (sizeof(T)==1)
   		typepixel = VFF_TYP_1_BYTE;
   	else if (sizeof(T)==2)
		typepixel = VFF_TYP_2_BYTE;
	else if (sizeof(T)==4)
		typepixel = VFF_TYP_4_BYTE;
	else
		return 1;

    image1=allocheader(NULL,dimx,dimy,dimz,typepixel);
    image1->image_data=I1;

    image2=allocheader(NULL,dimx,dimy,dimz,typepixel);
    image2->image_data=I2;

    lmin(image1,image2);

    free(image1);
    free(image2);
    return 0;
}

template<typename PixType>
static inline void sort7_sorting_network_simple_swap(PixType ** d){

#define mini(x, y) (x<y?x:y)
#define maxi(x, y) (x<y?y:x)
//#define SWAP(x,y) { const int a = min(d[x], d[y]); const int b = max(d[x], d[y]); d[x] = a; d[y] = b;}
#define SWAP(x,y) { const int a = mini(*d[x], *d[y]); const int b = maxi(*d[x], *d[y]); *d[x] = a; *d[y] = b;}
    SWAP(1, 2);
    SWAP(3, 4);
    SWAP(5, 6);
    SWAP(0, 2);
    SWAP(4, 6);
    SWAP(3, 5);
    SWAP(2, 6);
    SWAP(1, 5);
    SWAP(0, 4);
    SWAP(2, 5);
    SWAP(0, 3);
    SWAP(2, 4);
    SWAP(1, 3);
    SWAP(0, 1);
    SWAP(2, 3);
    SWAP(4, 5);
#undef SWAP
#undef min
#undef max
}


template<typename PixType>
static void sorting(PixType* I1, PixType* I2, PixType* I3, PixType* I4, PixType* I5, PixType* I6, PixType* I7, int N)
{
    PixType* d[7];
    d[0] = I1;
    d[1] = I2;
    d[2] = I3;
    d[3] = I4;
    d[4] = I5;
    d[5] = I6;
    d[6] = I7;

    for (int i=0; i<N; i++) {
        sort7_sorting_network_simple_swap(d);
        for (int j=0;j<7;j++)
            d[j]++;
    }

}


template<typename T>
void Get_RPO_Orientations(T *input_buffer, T *image_dilat, int L, int dimz, int dimy, int dimx, T *res1, T *res2, T *res3, T *res4, T *res5, T *res6, T *res7,int nb_core){


	int image_size=dimx*dimy*dimz;

// ####################  Run PO for each orientation ##############################

	// orientation vector
	std::vector<int> orientation1(3);
	orientation1[0] = 1;
	orientation1[1] = 0;
	orientation1[2] = 0;
	std::vector<int> orientation2(3);
	orientation2[0] = 0;
	orientation2[1] = 1;
	orientation2[2] = 0;
	std::vector<int> orientation3(3);
	orientation3[0] = 0;
	orientation3[1] = 0;
	orientation3[2] = 1;
	std::vector<int> orientation4(3);
	orientation4[0] = 1;
	orientation4[1] = 1;
	orientation4[2] = 1;
	std::vector<int> orientation5(3);
	orientation5[0] = 1;
	orientation5[1] = 1;
	orientation5[2] = -1;
	std::vector<int> orientation6(3);
	orientation6[0] = -1;
	orientation6[1] = 1;
	orientation6[2] = 1;
	std::vector<int> orientation7(3);
	orientation7[0] = -1;
	orientation7[1] = 1;
	orientation7[2] = -1;
	
	std::cout<<"------- RPO computation with scale " <<L<< "-------"<<std::endl;
	// Calling PO for each orientation
	   //omp_set_num_threads(6);
	   #pragma omp parallel sections
	   {
	   #pragma omp section
	   {
		 PO_3D<T>(image_dilat,dimz,dimy,dimx,L,orientation1,res1);
		 std::cout<<"orientation1 1 0 0 : passed"<<std::endl;
	   }
	   #pragma omp section
	   {
		   PO_3D<T>(image_dilat,dimz,dimy,dimx,L,orientation2,res2);
		   std::cout<<"orientation2 0 1 0 : passed"<<std::endl;
	   }
	   #pragma omp section
	   {
		   PO_3D<T>(image_dilat,dimz,dimy,dimx,L,orientation3,res3);
		   std::cout<<"orientation3 0 0 1 : passed"<<std::endl;
	   }
	   #pragma omp section
	   {
		   PO_3D<T>(image_dilat,dimz,dimy,dimx,L,orientation4,res4);
		   std::cout<<"orientation4 1 1 1 : passed"<<std::endl;
	   }
		#pragma omp section
	   {
		   PO_3D<T>(image_dilat,dimz,dimy,dimx,L,orientation5,res5);
		   std::cout<<"orientation5 1 1 -1 : passed"<<std::endl;
	   }
	   #pragma omp section
	  {
		   PO_3D<T>(image_dilat,dimz,dimy,dimx,L,orientation6,res6);
		   std::cout<<"orientation6 -1 1 1 : passed"<<std::endl;
	   }
	   #pragma omp section
	   {
		   PO_3D<T>(image_dilat,dimz,dimy,dimx,L,orientation7,res7);
		   std::cout<<"orientation7 -1 1 -1 : passed"<<std::endl;
		}
	 }
	 std::cout<<"RPO computation completed"<<std::endl;
	
	// Minimum between the computed RPO on the dilation and the initial image
	min_crush(res1, input_buffer, dimx, dimy, dimz);
	min_crush(res2, input_buffer, dimx, dimy, dimz);
	min_crush(res3, input_buffer, dimx, dimy, dimz);
	min_crush(res4, input_buffer, dimx, dimy, dimz);
	min_crush(res5, input_buffer, dimx, dimy, dimz);
	min_crush(res6, input_buffer, dimx, dimy, dimz);
	min_crush(res7, input_buffer, dimx, dimy, dimz);
		
}



template<typename T>
void RORPO(T* input_buffer, T* image_dilat, T* RPO_RPO_limit_orientations, int L, int nb_core, int dimx, int dimy, int dimz)
{

	// ######################################### RORPO classique #########################################
	
	
	int image_size=dimx*dimy*dimz;
   // Buffer for results
    
	T *res0=new T[dimx*dimy*dimz];
	T *res1=new T[dimx*dimy*dimz];
	T *res2=new T[dimx*dimy*dimz];
	T *res3=new T[dimx*dimy*dimz];
	T *res4=new T[dimx*dimy*dimz];
	T *res5=new T[dimx*dimy*dimz];
	T *res6=new T[dimx*dimy*dimz];
	T *output_buffer=new T[dimx*dimy*dimz];

	
	// Compute RPO 
	Get_RPO_Orientations<T>(input_buffer, image_dilat, L, dimz, dimy, dimx, res0, res1, res2, res3, res4, res5, res6, nb_core);


    // allocate memory for sorting results
    T* RPOt0= new T[image_size];
    T* RPOt1= new T[image_size];
    T* RPOt2= new T[image_size];
    T* RPOt3= new T[image_size];
    T* RPOt4= new T[image_size];
    T* RPOt5= new T[image_size];
    T* RPOt6= new T[image_size];


    memcpy(&RPOt0[0], &res0[0],image_size*sizeof(T));
    memcpy(&RPOt1[0], &res1[0],image_size*sizeof(T));
    memcpy(&RPOt2[0], &res2[0],image_size*sizeof(T));
    memcpy(&RPOt3[0], &res3[0],image_size*sizeof(T));
    memcpy(&RPOt4[0], &res4[0],image_size*sizeof(T));
    memcpy(&RPOt5[0], &res5[0],image_size*sizeof(T));
    memcpy(&RPOt6[0], &res6[0],image_size*sizeof(T));

    sorting(RPOt0,RPOt1,RPOt2,RPOt3,RPOt4,RPOt5,RPOt6, image_size);

    T *RPO_RPO=diff_buffer(RPOt6,RPOt3,image_size);

    // ######################################### Limit Orientations Treatment #########################################

	 // ------------------------- Computation of Imin ----------------------------

	 // ---- Imin limit case 4 orientations ----
	 //horizontal + vertical + diag1 + diag4
	 T* Imin4_1=min_alloc(res0,res1,dimx,dimy,dimz);
     min_crush(Imin4_1,res3,dimx,dimy,dimz);
     min_crush(Imin4_1,res6,dimx,dimy,dimz);

     //horizontal + vertical + diag2 + diag3
     T* Imin4_2=min_alloc(res0,res1,dimx,dimy,dimz);
     min_crush(Imin4_2,res4,dimx,dimy,dimz);
     min_crush(Imin4_2,res5,dimx,dimy,dimz);

     //horizontal + profondeur + diag2+ diag4
     T* Imin4_3=min_alloc(res0,res2,dimx,dimy,dimz);
     min_crush(Imin4_3,res4,dimx,dimy,dimz);
     min_crush(Imin4_3,res6,dimx,dimy,dimz);

     //horizontal + profondeur + diag1+ diag3
	 T* Imin4_4=min_alloc(res0,res2,dimx,dimy,dimz);
     min_crush(Imin4_4,res3,dimx,dimy,dimz);
     min_crush(Imin4_4,res5,dimx,dimy,dimz);

     //vertical + profondeur + diag1+ diag2
     T* Imin4_5=min_alloc(res1,res2,dimx,dimy,dimz);
     min_crush(Imin4_5,res3,dimx,dimy,dimz);
     min_crush(Imin4_5,res4,dimx,dimy,dimz);

     //vertical + profondeur + diag3+ diag4
     T* Imin4_6=min_alloc(res1,res2,dimx,dimy,dimz);
     min_crush(Imin4_6,res5,dimx,dimy,dimz);
     min_crush(Imin4_6,res6,dimx,dimy,dimz);

     // Final
     std::cout<<"before max"<<std::endl;
     T* Imin4=max_alloc(Imin4_1,Imin4_2,dimx,dimy,dimz);
     std::cout<<"after max"<<std::endl;
     max_crush(Imin4,Imin4_3,dimx,dimy,dimz);
     max_crush(Imin4,Imin4_4,dimx,dimy,dimz);
     max_crush(Imin4,Imin4_5,dimx,dimy,dimz);
     max_crush(Imin4,Imin4_6,dimx,dimy,dimz);
	 std::cout<<"after max"<<std::endl;
	 // ---- Imin limit case 5 orientations ----
    T* Imin5=min_alloc(res3,res4,dimx,dimy,dimz);
    min_crush(Imin5,res5,dimx,dimy,dimz);
    min_crush(Imin5,res6,dimx,dimy,dimz);

    // ------------------------- Computation of Imin2 ----------------------------
    //geodesic reconstruction of RPO6 in RPO4
    T* RPO6=geodilat_modif(RPOt1,RPOt3,18,-1,dimx,dimy,dimz);

    //geodesic reconstruction of RPO5 in RPO4
    T* RPO5=geodilat_modif(RPOt2,RPOt3,18,-1,dimx,dimy,dimz);

    // ---- Imin2 limit case 4 orientations ----
    T* Imin2_4=min_alloc(Imin4,RPO5,dimx,dimy,dimz);

    // ---- Imin2 limit case 5 orientations ----
    T* Imin2_5=min_alloc(Imin5,RPO6,dimx,dimy,dimz);

    // ------------------------- Final Result --------------------------------

    T* diff_Imin4=diff_buffer(Imin4,Imin2_4,image_size);
    T* diff_Imin5=diff_buffer(Imin5,Imin2_5,image_size);
    
	std::cout<<"test"<<std::endl;
    RPO_RPO_limit_orientations=max_alloc(RPO_RPO,diff_Imin4,dimx,dimy,dimz);
    max_crush(RPO_RPO_limit_orientations,diff_Imin5,dimx,dimy,dimz);
    
    
    /*nifti_image *nim=NULL;
    nim = nifti_image_read("/medit/data/Images/Images_synthetiques/Niveaux_gris/Plan_Spiral_3D/short/plan_spiral_blob_short_5_5_0.9.nii", 1);
    NewPT* image=((NewPT*)(nim->data));
   
 	std::ostringstream write;
	write<< "/medit/home/odyssee/Documents/VIVABRAIN/Programme/Code_RORPO/RORPO_dilat/test/multiscale_"<<L<<".nii";
   
    std::cout<<"Write"<<std::endl;
	nim->data=RPO_RPO_limit_orientations;
	int test=nifti_set_filenames(nim,write.str().c_str(),0,nim->byteorder);
	nifti_image_write(nim);
*/
   // Free memory before ending
   free(Imin2_4);
   free(Imin2_5);
   free(Imin4);
   free(Imin4_1);
   free(Imin4_2);
   free(Imin4_3);
   free(Imin4_4);
   free(Imin4_5);
   free(Imin4_6);
   delete[] diff_Imin4;
   delete[] diff_Imin5;
   free(RPO5);
   free(RPO6);
   delete[] RPO_RPO;

   delete[](res0);
   delete[](res1);
   delete[](res2);
   delete[](res3);
   delete[](res4);
   delete[](res5);
   delete[](res6);

   delete[](RPOt0);
   delete[](RPOt1);
   delete[](RPOt2);
   delete[](RPOt3);
   delete[](RPOt4);
   delete[](RPOt5);
   delete[](RPOt6);
   
}



//Return RORPO multiscale with or without masking
template<typename T>
void Call_RORPO(T* image_modif, T* multiscale, std::vector<int>S_list, int nb_scales, int nb_core, int max_value, int dimx, int dimy, int dimz, int debug_flag, char* mask_buffer=NULL)
{
	
    int image_size=dimx*dimy*dimz;
    
	// -------------------------- Dilation input image by a cube of size 3x3x3 ----------------------------
	T *input_buffer=new T[dimx*dimy*dimz];
	memcpy(&input_buffer[0],&image_modif[0],(dimx*dimy*dimz)*sizeof(T));
	rect3dminmax(image_modif, dimx, dimy, dimz, 3,3,3,false);

	// ####################### Computation of RORPO for each scale ######################

	std::vector<int>::iterator it;
	T *output_buffer=new T[image_size];
	T* RPO_RPO_limit_orientations=new T[image_size];
	for (it=S_list.begin();it!=S_list.end();++it)
	{
	    RORPO<T>(input_buffer, image_modif, RPO_RPO_limit_orientations, *it, nb_core, dimx, dimy, dimz);
	    
		// ----------------- Max of scales ---------------
	    max_crush(multiscale,output_buffer,dimx,dimy,dimz);
	    
	}
	std::cerr<<"after scales"<<std::endl;
    // ----------------- Dynamic Enhancement ---------------
	// Find Max value of output_buffer
	T max_output=multiscale[0];
	std::cerr<<"before boucle for"<<std::endl;
	
	for (int i=0; i<image_size;++i)
	{
	    if (multiscale[i]>max_output)
	        max_output=multiscale[i];
	}

	// Contrast Enhancement
	std::cerr<<"Begining Contrast Enhancement"<<std::endl;
	for (int i=0; i<image_size; ++i)
	{
	    multiscale[i]=static_cast<T>((static_cast<float>(multiscale[i])/static_cast<float>(max_output))*max_value);
	}	
	std::cerr<<"after Contrast Enhancement"<<std::endl;
			    
	min_crush(multiscale,input_buffer,dimx,dimy,dimz);
		
	   
	delete[](output_buffer);
	   
}



// ########################################### CALL FUNCTION ##########################################################

template<typename PixType>
static void RORPO_multiscale(PixType* image, PixType* output, int Smin, float factor, int nb_scales, int nb_core, int dimx, int dimy, int dimz, int debug_flag=-1)
{

// ################################# Scales Computations ###################################

	const int image_size=dimx*dimy*dimz;
	std::vector<int> S_list(nb_scales);
	S_list[0]=Smin;

	if (debug_flag){
	    std::cout<<" "<<std::endl;
		std::cout<<"Scales : ";
		std::cout<<S_list[0]<< " ";
	}
			
	for (int i=1; i<nb_scales;++i)
	{
		S_list[i]=int(Smin*pow(factor,i));
		if (debug_flag){
			std::cout<<S_list[i]<< " ";
			
		}
	}
	std::cout<<std::endl;
	
	/*for (int i=1; i<nb_scales-1;i++)
	{
	    S_list[i]=Smin+i*((factor-Smin)/(nb_scales-1));
	    std::cout<<S_list[i]<< " ";
	}
	S_list[nb_scales-1]=factor;
	std::cout<<factor<< " ";*/
	
	
	
	// ---------- Find min and max value --------
	int min_value=image[0];
	int max_value=image[0];
	
	for (int i=0; i<image_size;++i)
	{
		if (image[i]<min_value)
			min_value=image[i];

		if (image[i]>max_value)
			max_value=image[i];
	}

	if (debug_flag){
		std::cout<<"min value: "<<min_value<<std::endl;
		std::cout<<"max value: "<<max_value<<std::endl;
		std::cout<<" "<<std::endl;
	}

	// Run RORPO
	PixType* multiscale=new PixType[image_size];
	// ############################################### ERRORRRRRR ##########################################
	output=new PixType[image_size];
	memset(multiscale,0,image_size*sizeof(PixType));
	Call_RORPO<PixType>(image, multiscale, S_list, nb_scales,nb_core, max_value, dimx, dimy, dimz, debug_flag);	
	memset(output,multiscale,image_size*sizeof(PixType));
	std::cerr<<"after Call_RORPO"<<std::endl;
}

//#endif // RORPO_INCLUDED














