/*
  This software is licensed under
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but
  without any warranty to the extent permitted by applicable law.

  (C) UjoImro, 2009-2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/
#include "liar_segmentation.h"
#include "pink_python.h"

#include <boost/preprocessor/slot/counter.hpp>


using namespace boost::python;
using namespace pink;


#if 0 // this function is not used !! 
namespace pink {
    namespace python {
        // seeded region growing
        // Adams & Bischof PAMI 16(6) 641--647 1994
        
        template <class image_t>
        image_t liarsrgrow3d
        (
            const char_image & src,  	/**< [in] input image */
            const int_image & mark,  	/**< [in] seed (a.k.a. markers) image */
            const int metric,           /**< [in] metric used */
            const int borders,          /**< [in] boolean showing borders or not */
            const int connectivity,     /**< [in] connectivity */
            const int showgrey          /**< [in] showing average grey-levels or not */
            )
        {

            char_image src_cp = src.clone();
            int_image mark_cp = mark.clone();


            // The low-level function imsrgrow3d return 0 to indicate success
            // HT 20121119

            struct xvimage *mysrc = src_cp.get_output();
            struct xvimage *myseeds   = mark_cp.get_output();
            if (imsrgrow3d (mysrc, myseeds, metric, borders, connectivity, showgrey) )
            {
                pink_error("function imsrgrow3d failed");
            }
            return mark;
        } /* liarimsrgrow3d */
    } /* namespace python */
} /* namespace pink */

#endif // 0 THIS FUNCTION IS NOT USED

namespace liarseg
{

// UI_EXPORT_FUNCTION(
//   srgrow,
//   pink::python::liarsrgrow3d,
//   ( arg("src"), arg("mark"),arg("metric"), arg("connectivity"), arg("showgrey") ),
//   "1D, 2D and 3D Segmentation by Seeded Region Growing method\n"
//   " src = grey-level source images\n"
//   " mark = int32 markers image\n"
//   " metric = 0 (absolute) or 1 (Eudlidean) difference between pixel and regions\n"
//   " connectivity = 2 (1D) ; 4/8 (2D) or (6-18-26) (3D)\n"
//   " showgrey: output consist of regions averages\n"
//   // end of the documenation
//   );

    // Please note: the output image is the first image in the list
    // the type of the image is not meant to change.
UI_WRAP_FUNCTION(
    "srgrow",
    imsrgrow3d,
    (  arg("markers"), arg("input"), arg("metric"), arg("borders"), arg("connectivity"), arg("showgrey") ),
    "1D, 2D and 3D Segmentation by Seeded Region Growing method\n"
    " mark = int32 markers image\n"
    " src = grey-level source images\n"
    " metric = 0 (absolute) or 1 (Eudlidean) difference between pixel and regions\n"
    " connectivity = 2 (1D) ; 4/8 (2D) or (6-18-26) (3D)\n"
    " showgrey: output consist of regions averages\n"
    );
    
# include BOOST_PP_UPDATE_COUNTER()
} /* namespace liarseg */

using namespace liarseg;

void pyliarseg()
{
    CALL_EXPORTED_FUNCTIONS(BOOST_PP_COUNTER);
}

// LuM end of file


