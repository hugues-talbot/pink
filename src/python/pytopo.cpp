/*
  This software is licensed under
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but
  without any warranty to the extent permitted by applicable law.

  (C) UjoImro, 2011-2014
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/
// The pink python wrapper core file

#include "pyexport.hpp"

#include "lhisto.h"
#include "lborder.h"
#include "lseltopo.h"
#include "lgeodesic.h"
#include "lhthiniso.h"
#include "lhtkern3d.h"
#include "ltopotypes.h"
#include "python_doc.h"
#include "lskelpar3d.h"
#include "llambdakern.h"
#include "lmedialaxis.h"
#include "l2dcollapse.h"
#include "l3dcollapse.h"
#include "l2dkhalimsky.h"
#include "l3dkhalimsky.h"
#include "lfiltrestopo.h"
#include "ui_skeleton.hpp"
#include "lfermetrous3d.h"
#include "lattribheight.h"

void skeleton_export();
void skelcurv_export();



// resultdef(
// char_image,
// "houghcircles",
// lhoughcercles,
// (arg("image"), arg("min radius"), arg("pas radius"), arg("nb pas")),
// doc__houghcercles__c__
// );
// 

// 
// functiondef(
// "kern",
// lkern,
// (arg("image"), arg("connex")),
// doc__kern__c__
// );
// 

// // optionally lrao instead of lleviandi
// functiondef(
// "levialdi",
// llevialdi,
// (arg("image"), arg("connex"), arg("n")),
// doc__levialdi__c__
// );
// 


// functiondef(
// "matchellipse",
// lmatchellipse,
// (arg("image"), arg("connex")),
// doc__matchellipse__c__
// );
// 
// functiondef(
// "matchrect",
// lmatchrect,
// (arg("image"), arg("connex"), arg("mode")),
// doc__matchrect__c__
// );
// 
// functiondef(
// "maxdiameter",
// lmaxdiameter,
// (arg("image"), arg("connex")),
// doc__maxdiameter__c__
// );
// 

// functiondef(
// "nbvois",
// lnbvois,
// (arg("image"), arg("connex")),
// doc__nbvois__c__
// );
// 

// functiondef(
// "pthseparatinggray",
// lpthseparatinggray,
// (arg("image"), arg("connex"), arg("h")),
// doc__pthseparatinggray__c__
// );
// 

// functiondef(
// "quasishear",
// lquasishear,
// (arg("image"), arg("theta"), arg("x"), arg("y")),
// doc__quasishear__c__
// );
// 

// // NOTE: this function returns a pointer
// // functiondef(
// // "quasishear",
// // lquasishear2,
// // (arg("image"), arg("theta")),
// // doc__quasishear__c__
// // );
// // 

// // NOTE: recalagerigide returns a list

// functiondef(
// "reconsplateaux",
// lreconsplateaux,
// (arg("image1"), arg("image2"), arg("connexity")),
// doc__reconsplateaux__c__
// );
// 
// functiondef(
// "remspnoise",
// lremspnoise,
// (arg("image"), arg("g"), arg("k")),
// doc__remspnoise__c__
// );
// 

// functiondef(
// "segmentlines",
// lsegmentlignes,
// (arg("image"), arg("connexity"), arg("lower threshold"), arg("higher threshold")),
// doc__segmentlignes__c__
// );
// 

// 
// functiondef(
// "selrect",
// lselrect,
// (arg("image"), arg("x"), arg("y"), arg("w"), arg("h")),
// doc__selrect__c__
// );
// 
// 
// functiondef(
// "alpha",
// lalpha,
// ( arg("image"), arg("connexity"), arg("sign") ),
// doc__alpha__c__
// );
// 
// functiondef(
// "amont",
// lamont,
// ( arg("image"), arg("connexity"), arg("sign") ),
// doc__amont__c__
// );
// 
// 
// functiondef(
// "ellipticite",
// lellipticite,
// (arg("image"), arg("connex")),
// doc__ellipticite__c__
// );
// 
 
// functiondef(
// "surrect",
// lrectangleincluant,
// (arg("image"), arg("connexity")),
// doc__surrect__c__
// );
// 


void
pytopo()
{

  using pink::type;
  using pink::allocdef;
  using pink::exportdef;
  using pink::resultdef;
  using pink::functiondef;
 
  using boost::python::arg;
  using boost::python::def;
  using boost::python::args;
  using boost::python::scope;
  using boost::python::object;
  using boost::python::handle;
  using boost::python::borrowed;

  // we create the topo submodule
  // motivation:
  // http://bfroehle.com/2011/07/boost-python-submodules/
  // http://isolation-nation.blogspot.hu/2008/09/packages-in-python-extension-modules.html
 
  // make "from mypackage.topo import <whatever>" work
  object topo_module(handle<>(borrowed(PyImport_AddModule("libcpp_pink.topo"))));
 
  // make "from mypackage import topo" work
  scope().attr("topo") = topo_module;
 
  // set the current scope to the new sub-module
  scope topo_scope = topo_module;

  functiondef( "simplepair", lsimplepair, (arg("image"), arg("all")), doc__simplepair__c__ );
 
  functiondef( "t26pp", ltopotypes_t26pp, (arg("image")), doc__t26pp__c__ );
 
  functiondef( "t4pp", lt4pp, (arg("image")), doc__t4pp__c__ );
 
  functiondef( "t8pp", lt8pp, (arg("image")), doc__t8pp__c__ );

  functiondef( "tlf", ltlf, (arg("image"), arg("connexmin"), arg("radius")), doc__tlf__c__ );

  functiondef( "surfacerestoration", lsurfacerestoration, (arg("image"), arg("number of maximal iterations"), arg("connexity")), doc__surfacerestoration__c__ );

  functiondef( "hthiniso", lhthiniso, (arg("image"), arg("dmax"), arg("connex"), arg("pixwhratio")), doc__hthiniso__c__ );

  functiondef( "ptinterior", lptinterior, (arg("image"), arg("connex")), doc__ptinterior__c__ );

  functiondef( "ptjunction", lptjunction, (arg("image"), arg("connex")), doc__ptjunction__c__ );

  functiondef( "ptmultiple", lptmultiple, (arg("image"), arg("connex")), doc__ptmultiple__c__ );

  functiondef( "ptseparating", lptseparating, (arg("image"), arg("connex")), doc__ptseparating__c__ );

  functiondef( "ptseparatinggray", lptseparatinggray, (arg("image"), arg("connex")), doc__ptseparatinggray__c__ );

  functiondef( "ptsimple", lptsimple, (arg("image"), arg("connex")), doc__ptsimple__c__ );

  functiondef( "deletecomp", ldeletecomp, (arg("image"), arg("connex"), arg("x"), arg("y"), arg("z")), doc__deletecomp__c__ );

  functiondef( "seltopo", lseltopo, (arg("image"), arg("connexity"), arg("t-"), arg("t+"), arg("tb-"), arg("tb+")), doc__seltopo__c__ );

  functiondef( "selectcomp", lselectcomp, (arg("image"), arg("connex"), arg("x"), arg("y"), arg("z")), doc__selectcomp__c__ );

  functiondef( "fermetrous3d", lfermetrous3dbin, (arg("image"), arg("connex"), arg("hole size")), doc__fermetrous3d__c__ );

  functiondef( "tuf", ltuf, ( arg("image"), arg("min connex"), arg("radius") ), doc__tuf__c__ );

  functiondef( "taf", ltaflambda, ( arg("image"), arg("min connex"), arg("radius"), arg("lambdapics"), arg("lambdapuits") ), doc__taf__c__ );

  functiondef( "seltype3d", l3dseltype, (arg("image"), arg("d-"), arg("d+"),arg("a-"),arg("a+"),arg("b-"),arg("b+")), doc__3dseltopo__c__ );

  functiondef( "skelsurf3d", l3dskelsurf, (arg("image"), arg("nsteps") ), doc__3dskelsurf__c__ );

  functiondef( "thin3d", l3dthin, (arg("image"), arg("nsteps") ), doc__3dthin__c__ );

  functiondef( "border2d", l2dborder, (arg("image") ), doc__2dborder__c__ );

  functiondef( "pardircollapse2d", l2dpardircollapse, (arg("image"), arg("nsteps"), arg("inhibit") ), doc__2dpardircollapse__c__ );

  functiondef( "pardircollapse2d", l2dpardircollapse_short, (arg("image"), arg("nsteps") ), doc__2dpardircollapse__c__ );

  functiondef( "seltype2d", l2dseltype, (arg("image"), arg("d-"), arg("d+"),arg("a-"),arg("a+"),arg("b-"),arg("b+")), doc__2dseltopo__c__ );

  functiondef( "thin2d", l2dthin, (arg("image") ), doc__2dthin__c__ );

  functiondef( "closebeta2d", l2dclosebeta, (arg("image") ), doc__2dmakecomplex__c__ );

  functiondef( "alpha3d", l3dalpha, (arg("image") ), doc__3dalpha__c__ );
 
  functiondef( "beta3d", l3dbeta, (arg("image") ), doc__3dbeta__c__ );
 
  functiondef( "isthmus3d", l3disthmus, (arg("image") ), doc__3disthmus__c__ );
 
  functiondef( "pardircollapse3d", l3dpardircollapse, (arg("image"), arg("nsteps"), arg("inhibit") ), doc__3dpardircollapse__c__ );
 
  functiondef( "pardircollapse3d", l3dpardircollapse_short, (arg("image"), arg("nsteps") ), doc__3dpardircollapse__c__ );
 
  functiondef( "skel_ACK3", lskelACK3, (arg("image"), arg("nsteps"), arg("inhibit")), doc__skel_ACK3__c__ );

  functiondef( "ptisolated", lptisolated, ( arg("image"), arg("connexity") ), doc__ptisolated__c__ );

  functiondef( "ptend", lptend, ( arg("image"), arg("connexity") ), doc__ptend__c__ );

  functiondef( "ptcurve", lptcurve, ( arg("image"), arg("connexity") ), doc__ptcurve__c__ );

  functiondef( "ptjunction", lptjunction, ( arg("image"), arg("connexity") ), doc__ptjunction__c__ );

  functiondef( "selectcomp", lselectcomp, ( arg("image"), arg("connexity"), arg("x"), arg("y"), arg("z") ), doc__selectcomp__c__ );

  functiondef( "holeclosing", lfermetrous3dbin, ( arg("image"), arg("connexity"), arg("holesize") ), doc__holeclosing__c__ );

  functiondef( "holeclosing", lfermetrous3dbin2, ( arg("image"), arg("guiding image") ,arg("connexity"), arg("holesize") ), doc__holeclosing__c__ );
  
  using pink::skel_t;
  
  boost::python::class_< pink::skel_t<index_t> > (
    "skeleton_t",
    "This is the class for manipulating skeletons.", 
    boost::python::init<>( args("self"), "Creates an empty skeleton object." ) ) 
    .def( boost::python::init< uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t > (
            args("self","rs", "cs", "ds", "nbvertex", "nbptcell", "nbadjcell", "connex" ),
            "Creates a skeleton with appropriate parameters" )
      )

    .def( boost::python::init< std::string > ( args("self","filename"), "Reads a skeleton from a file." ) )
 
    .def( "addadjlist", &pink::skel_t<pink::char_image>::addadjlist, ( arg("k"), arg("v") ), "WRITE ME!!!" )

    .def( "addptslist", &pink::skel_t<index_t>::addptslist, ( arg("k"), arg("v") ), "WRITE ME!!!" )

    .def( "nb_adjacent_elts", &pink::skel_t<index_t>::nb_adjacent_elts, ( arg("i") ), "WRITE ME!!!" )

    .def( "writeskel", &pink::skel_t<index_t>::writeskel, ( arg("filename") ), "WRITE ME!!!" )
 
    .def( "skeldelete", &pink::skel_t<index_t>::skeldelete, ( arg("i") ), "WRITE ME!!!" )

    .def( "__repr__", &pink::skel_t<index_t>::repr, args("self"), "Prints the skeleton." )
    ; // pink::skel_t (skeleton)
 
  def( "image2skel", pink::image2skel<pink::char_image>, ( arg("image"), arg("connex"), arg("len") ), doc__pgm2skel__c__ );

  def( "image2skel", pink::image2skel_short<pink::char_image>, ( arg("image"), arg("connex") ), doc__pgm2skel__c__ );

  functiondef( "lambdaskel", llambdakern, ( arg("image"), arg("imcond"), arg("connexity"), arg("lambda") ), doc__lambdaskel__c__ );

  functiondef( "lambdaskel", llambdakern_short, ( arg("image"), arg("connexity"), arg("lambda") ), doc__lambdaskel__c__ );

  functiondef( "plane3d", l3dplane, (arg("a"), arg("b"), arg("c"), arg("d")), doc__3dplane__c__ );

  functiondef( "border", mctopo3d_lborder, (arg("image"), arg("connexity")), doc__border__c__ );

  functiondef( "grayskel", lgrayskel, (arg("image"), arg("cond image"), arg("connex"), arg("lambda")), doc__grayskel__c__ );

  functiondef( "grayskel", lgrayskel_short, (arg("image"), arg("connex"), arg("lambda")), doc__grayskel__c__ );

  exportdef( "invariants2d", l2dinvariants, ( arg("image") ), doc__2dinvariants__c__ );

  exportdef( "invariants3d", l3dinvariants, ( arg("image"), arg("make") ), "WRITE ME!!" /*doc__countvalues__c__*/ );

  exportdef( "countvalues", lcountvalues, ( arg("image"), arg("make") ), "WRITE ME!!" /*doc__countvalues__c__*/ );

  functiondef( "color2d", l2dcolor, (arg("image") ), doc__2dcolor__c__ );

  functiondef( "color3d", l3dcolor, (arg("image") ), doc__3dcolor__c__ );

  functiondef( "makecomplex2d", l2dmakecomplex, (arg("image") ), doc__2dmakecomplex__c__ );

  functiondef( "makecomplex3d", l3dmakecomplex, (arg("image") ), doc__3dmakecomplex__c__ );

  functiondef( "sphere3d", l3dsphere, (arg("image"), arg("x0"), arg("y0"), arg("z0"), arg("radius")), doc__3dsphere__c__ );

  skeleton_export();  

  skelcurv_export();  
  
  import_array();  // numpy initialization

  return; 
}



// LuM end of file
