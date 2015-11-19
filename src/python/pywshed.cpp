/*
  This software is licensed under
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but
  without any warranty to the extent permitted by applicable law.

  (C) UjoImro, 2011
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/
// The pink python wrapper core file

#include "llpemeyer.h"
//#include "llpemeyer.h"
#include "lwshedtopo.h"
#include "pink_python.h"
#include "powerwatsegm.h"
#include "ldynamique_grimaud.h"

#include <boost/preprocessor/slot/counter.hpp>

using namespace pink;
using boost::python::arg;
using boost::python::def;

namespace wshed
{
  

  UI_WRAP_FUNCTION(
    "watershed",
    lwshedtopobin,
    ( arg("image"), arg("mark"), arg("connexity") ),
    doc__watershed__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

// by HT for Claire
  UI_WRAP_FUNCTION(
    "watershedMeyer2",
    llpemeyer_llpemeyer2,
    ( arg("markers"), arg("image"), arg("mask"), arg("connexity") ),
    doc__watershedMeyer2__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

// by MC
  UI_WRAP_FUNCTION(
    "watershedMeyer2lab",
    llpemeyer2lab,
    ( arg("markers"), arg("image"), arg("mask"), arg("connexity") ),
    doc__watershedMeyer2lab__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

// by MC
  UI_WRAP_FUNCTION(
    "watershedMeyer2labnomask",
    llpemeyer2lab_nomask,
    ( arg("markers"), arg("image"), arg("connexity") ),
    doc__watershedMeyer2lab__c__
    );
# include BOOST_PP_UPDATE_COUNTER()

// by HT for Claire
    // Note: here the output image is the first image
  UI_WRAP_FUNCTION(
    "watershedMeyer2NM",
    llpemeyer2_nomask,
    ( arg("markers"), arg("image"), arg("connexity") ),
    doc__watershedMeyer2__c__
      );

# include BOOST_PP_UPDATE_COUNTER()
    // by HT for Eloise (ah les femmes !)
    // Note the output image is the first image in the list
    UI_WRAP_FUNCTION(
        "watershedMeyerlabnoline",
        llpemeyer_llpemeyersanslignelab,
        ( arg("markers"), arg("image"), arg("mask"), arg("connectivity") ),
        "Pixel-based, labeled 2D Meyer watershed without separating lines\n"
        "  mask is mandatory\n"
        "  markers must be INT32\n"
        "  image must be UINT8\n"
        "Connectivity must be only 4 or 8"// missing doc
        );

# include BOOST_PP_UPDATE_COUNTER()    
    UI_WRAP_FUNCTION(
        "watershedMeyer3Dlabnoline",
        llpemeyer_llpemeyer3dsanslignelab,
        ( arg("markers"), arg("image"), arg("mask"), arg("connectivity") ),
        "Pixel-based, labelled 3D Meyer watershed without separating surfaces\n"
        "  mask is mandatory\n"
        "  markers must be INT32\n"
        "  image must be UINT8\n"
        "Connectivity must be only 6, 18 or 26"// missing doc
        );

    
    
# include BOOST_PP_UPDATE_COUNTER()

  UI_WRAP_FUNCTION(
    "wshedval",
    lwshedval,
    (arg("image"), arg("connex")),
    doc__wshedval__c__
    );
# include BOOST_PP_UPDATE_COUNTER()


    UI_WRAP_RESULT(
        int_image,
        "powerwatershedCol",
        compute_power_watershed_col,
        (arg("image_r"), arg("image_g"), arg("image_b"), arg("seeds"), arg("geodesic"), arg("multiple")),
        "WRITE ME!!!"
        //doc__powerwshedval__c__
        );

# include BOOST_PP_UPDATE_COUNTER()
    
    UI_WRAP_RESULT(
        int_image,
        "powerwatershedBW",
        compute_power_watershed_bw,
        (arg("image_bw"), arg("seeds"), arg("geodesic"), arg("multiple")),
        "WRITE ME!!!"
        //doc__powerwshedval__c__
        );

# include BOOST_PP_UPDATE_COUNTER()

} /* namespace pywshed */

using namespace wshed;

void pywshed()
{
  CALL_EXPORTED_FUNCTIONS(BOOST_PP_COUNTER);
}



// LuM end of file
