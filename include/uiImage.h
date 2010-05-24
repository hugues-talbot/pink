#ifndef UIIMAGE_H_
#define UIIMAGE_H_
/*
This software is ment to be free.
(C) UjoImro, 2007
ujoimro@gmail.com
*/
/* ujoimro, 2006 */
/** \file uiImage.h
 * \brief Header file for uiImage.cpp.
 */
/* image reading utility */
/* not neccesserily included in final version */

/* Line 12+1 */

const int longestline = 100;

void uiWriteImage ( const pink::ujoi<float> & image, const string & filename );
void uiWriteFloatImage3D( const pink::ujoi<float> & image, const string & filename );
		
//// in fact next two functions should not be exported
void uiWriteImage2D ( const pink::ujoi<float> & image, const string & filename );
void uiWriteImage3D ( const pink::ujoi<float> & image, const string & filename );



#endif /*UIIMAGE_H_*/
/* LuM end of file */

