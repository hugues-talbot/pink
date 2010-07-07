/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by aplicable law.
  
  (C) UjoImro, 2007-2009
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#ifdef __cplusplus

#include <pink.h>

#undef error
#define error(msg) {stringstream fullmessage; fullmessage << "in uiFileIO.cpp: " << msg; call_error(fullmessage.str());}


using namespace std;

namespace pink {

  int file_size( const string & filename )
  {
    int result;
    ifstream file;

    file.open( filename.c_str(), ios::binary );
    file.seekg( 0, ios::end );
    
    result = file.tellg();

    file.seekg( 0, ios::beg );
    file.close(); 
    
    return result;
  } /* file_size */

} /* namespace pink */








#endif /*__cplusplus*/
/* LuM end of file */
