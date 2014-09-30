/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2014
  BME-Infokom
  ujoimro@gmail.com
*/

#ifndef LSURIMP_H_
#define LSURIMP_H_

#ifdef __cplusplus
extern "C" {
#endif

int32_t lsurimp( const xvimage * grayscale, 
                 const xvimage * binary, 
                 const char * filename );

#ifdef __cplusplus
} // extern "C"
#endif

#endif // LSURIMP_H_
// LuM end of file
