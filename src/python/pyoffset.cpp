// UjoImro, 2009
// This software is meant to be free
// boost python wrapper

#include <pink_python.h>

#undef error
#define error(msg) {stringstream fullmessage; fullmessage << "in pymedianfilter.cpp: " << msg; call_error(fullmessage.str());}

using namespace boost::python;
using namespace pink;




















































// LuM end of file
