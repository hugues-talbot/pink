// This file has been generated automatically by the 
// python_documenter, part of the PInK package (UjoImro, 2011)
// the date of generation: 2013-12-09T14:43:42Z
// warning: DO NOT EDIT. All your changes will be lost at 
// the next generation

#ifndef PINK__PYTHON__DOCUMENTER__HPP__
#define PINK__PYTHON__DOCUMENTER__HPP__
#define doc__dilatbin3d__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__maxdiameter__c__ "  maxdiameter.c\n"         \
"\n"         \
" maximal diameter of a binary image\n"         \
"\n"         \
"Usage: maxdiameter in connex out\n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"Find two points x, y in a set X (binary image in) such that \n"         \
"d(x,y) is greater or equal to d(v,w) for any two points v, w in X.\n"         \
"The distance d is the Euclidean distance.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: geo\n"         \
"  geo\n"         \
"\n"         \
" Gu Jun\n"         \
"\n"         \
"\n"


#define doc__surimp__c__ "  surimp.c\n"         \
"\n"         \
" superimposition of a binary image with a grayscale image\n"         \
"\n"         \
"Usage: surimp i1.ndg i2.bin out.ppm\n"         \
"\n"         \
"Description: Superimposes the binary image i2.bin (in red) to the \n"         \
"grayscale image i1.bin. The result is stored as a color image out.ppm.\n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: convert\n"         \
"  convert\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__3dbeta__c__ "  3dbeta.c\n"         \
"\n"         \
" beta-dilation of a 3d khalimsky order \n"         \
"\n"         \
"Usage: 3dbeta in.pgm out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Let X be the set of elements of the order  in.pgm, \n"         \
"the result is the union of beta(x) for all x in X.\n"         \
"\n"         \
"Types supported: byte 3d\n"         \
"\n"         \
"Category: orders\n"         \
"  orders\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__skel_CKG_map__c__ "  skel_CKG_map.c\n"         \
"\n"         \
" topogical map from parallel 2D and 3D binary guided thinning\n"         \
"\n"         \
"Usage: skel_CKG_map prio.pgm in.pgm out.pgm\n"         \
"\n"         \
"Description: Topological map from parallel 2D and 3D \n"         \
"binary guided thinning based on critical kernels. \n"         \
"The parameter  in.pgm specifies the set\n"         \
"(object) to be thinned. The parameter  prio.pgm specifies the priority\n"         \
"function. \n"         \
"\n"         \
"Warning: The object must not have any point on the frame of the image.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d, long 2d, long 3d, float 2d, float 3d\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__affine__c__ "  affine.c\n"         \
"\n"         \
" affine transformation of a 2D image\n"         \
"\n"         \
"Usage: affine in.pgm {trans.lst | hx hy theta tx ty} out.pgm\n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"Applies to in.pgm an affine tranformation composed of a zoom \n"         \
"(factors  hx,  hy), a rotation (angle  theta in degrees, \n"         \
"center 0,0), and a translation (vector  tx,  ty). \n"         \
"Alternatively, the transformation parameters may be given \n"         \
"in a type \"e\" list file (see doc/formats.txt).\n"         \
"\n"         \
"Method: interpolation.\n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: geo\n"         \
"  geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__countvalues__c__ "  countvalues.c\n"         \
"\n"         \
" counts the number of distinct values of an image or a region\n"         \
"\n"         \
"Usage: countvalues in.pgm [mask.pgm] out.list\n"         \
"\n"         \
"Description:\n"         \
"Counts the number of distinct values of  im.pgm (masked by the binary image\n"         \
" mask.pgm, if given) and saves it in file  out.list .\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d\n"         \
"\n"         \
"Category: histo\n"         \
"  histo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__htkern3dbin__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__dilation__c__ "  dilation.c\n"         \
"\n"         \
" morphological dilation by a plane structuring element\n"         \
"\n"         \
"Usage: dilation in.pgm se.pgm out.pgm\n"         \
"\n"         \
"Description:\n"         \
"The (plane) structuring element is given by the non-null values in  se.pgm, \n"         \
"its origin (wrt the point (0,0) of  se.pgm ) is given by a comment line in the file se.pgm. \n"         \
"\n"         \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n"         \
"\n"         \
"Category: morpho\n"         \
"  morpho\n"         \
"\n"         \
" Michel Couprie 2002\n"         \
"\n"         \
"Example:\n"         \
"\n"         \
"dilation cells ~/Pink/masks/carre5.pgm cells_dilat\n"         \
"\n"         \
"\n"         \
"  \n"         \
"      \n"         \
"  \n"         \
"  cells\n"         \
"      cells_dilat\n"         \
"  \n"         \
"\n"         \
"\n"         \
"\n"         \
"\n"


#define doc__blob__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__hthiniso__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__skel_MK2__c__ "  skel_MK2.c\n"         \
"\n"         \
" parallel 2D binary ultimate skeleton, symmetrical\n"         \
"\n"         \
"Usage: skel_MK2 in.pgm nsteps [inhibit] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Parallel 2D binary thinning or ultimate skeleton (symmetrical). The parameter  nsteps gives,\n"         \
"if positive, the number of parallel thinning steps to be processed.\n"         \
"If the value given for  nsteps equals -1, the thinning is continued\n"         \
"until stability.\n"         \
"\n"         \
"If the parameter  inhibit is given and is a binary image name,\n"         \
"then the points of this image will be left unchanged. \n"         \
"\n"         \
"Warning: The object must not have any point on the frame of the image.\n"         \
"\n"         \
"References:\n"         \
"[BC08] Gilles Bertrand and Michel Couprie, Two-dimensional thinning algorithms based on critical kernels. Journal of Mathematical Imaging and Vision, 31(1). 2008. pp.35â€“56.\n"         \
"http://igm.univ-mlv.fr/LIGM/internal_report/pdf/2006_02.v2.pdf\n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__gradcol___notused_c__ "WRITE ME!!!\n"         \
"\n"


#define doc__identifyplane__c__ "  identifyplane.c\n"         \
"\n"         \
" identification of a best matching plane from a set of 3D points\n"         \
"\n"         \
"Usage: identifyplane in.list out.list\n"         \
"\n"         \
"Description:\n"         \
"Identifies the parameters (a, b, c, d) of the equation of the 3D plane:\n"         \
"ax+by+cz+d=0 that minimizes the least square error between this plane \n"         \
"and the given points. Method: basic linear regression.\n"         \
"\n"         \
"The result is given as a file that contains a list of 5 numbers: a, b, c, d the parameters of the equation, and e the least square error.\n"         \
"\n"         \
"Types supported: list 3D\n"         \
"\n"         \
"Category: geo\n"         \
"  geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__pgmascmode__c__ "  pgmascmode.c\n"         \
"\n"         \
" converts a pgm file to ascii mode\n"         \
"\n"         \
"Usage: pgmascmode filename [outfilename | \"wh\" outfilename]\n"         \
"\n"         \
"Description: Converts a pgm file to ascii mode.\n"         \
"If only parameter  filename is given, then the result\n"         \
"is also named  filename .\n"         \
"If the keyword \"wh\" is used as second parameter (stands for \"without header\"), then \n"         \
"an ascii file without header is produced.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n"         \
"\n"         \
"Category: convert\n"         \
"  convert\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__randrgb__c__ "  randrgb.c\n"         \
"\n"         \
" generates random colors from gray levels\n"         \
"\n"         \
"Usage: randrgb in.pgm out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Generates a color image where each gray level is replaced by a randomly\n"         \
"chosen color\n"         \
"\n"         \
"Types supported: byte 2d, int32_t 2d\n"         \
"\n"         \
"Category: convert\n"         \
"  convert\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__2dflowskeleton__c__foret__ "  void Morsify(graphe * g, boolean * Vh, TYP_VSOM epsilon)\n"         \
"     g (entrÃ©e) : un graphe pondÃ©rÃ© sur les sommets.\n"         \
"     TODO\n"         \
"     ne s'applique qu'aux graphes sans circuit\n"         \
"\n"         \
"\n"


#define doc__ptcurve__c__ "  ptcurve.c\n"         \
"\n"         \
" detects curve points in a binary image\n"         \
"\n"         \
"Usage: ptcurve in.pgm connex out.pgm\n"         \
"\n"         \
"Description:\n"         \
"An curve point is a white point, which has exactly 2 white n-neighbours \n"         \
"that are not n-neighbours to each other. In other words, Tn = 2 and \n"         \
"each neighboring n-connected component is made of exactly one point\n"         \
"(n = 4, 8 (2d) or 6, 18, 26 (3d), as set by the parameter  connex)\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" Michel Couprie 2002\n"         \
"\n"         \
"\n"


#define doc__erosfast__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__diffimages__c__ "  diffimages.c\n"         \
"\n"         \
" pixelwise absolute value difference of 2 images\n"         \
"\n"         \
"Usage: diffimages in1.pgm in2.pgm out.pgm\n"         \
"\n"         \
"Description:\n"         \
"For each pixel x, out[x] = abs (in1[x] - in2[x]). \n"         \
"Images must be of the same type and same dimensions.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n"         \
"\n"         \
"Category: arith\n"         \
"  arith\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__relabel__c__ "  relabel.c\n"         \
"\n"         \
" eliminates 'voids' in the labelling\n"         \
"\n"         \
"Usage: relabel in.pgm [out.pgm]\n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"Modifies the labels of image  in.pgm in such a way that the resulting labels are consecutive integers.\n"         \
"\n"         \
"If  out.pgm is not specified, then out.pgm = in.pgm.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d\n"         \
"\n"         \
"Category: histo\n"         \
" histo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__erosfast3d__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__lfdilat3d_rect__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__reconsdilat3d__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__drawcurve__c__ "  drawcurve.c\n"         \
"\n"         \
" draw line segments which are specified by a text file\n"         \
"\n"         \
"Usage: drawline in.pgm curve.txt out.pgm\n"         \
"\n"         \
"Description:\n"         \
"The file  curve.txt contains a list of points under the format:\n"         \
"c nb_points\n"         \
"x1 y1\n"         \
"x2 y2\n"         \
"x3 y3\n"         \
"...\n"         \
"\n"         \
"Types supported: byte 2D\n"         \
"\n"         \
"Category: draw\n"         \
" draw\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__asftndg__c__ "  asftndg.c\n"         \
"\n"         \
" alternate sequential filter controled by topology\n"         \
"\n"         \
"Usage: asftndg in.pgm {c.pgm cc.pgm|null null} connex rmax out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Alternate sequential filter controled by topology for multilevel images.\n"         \
"Disc- or Ball-shaped structuring elements of increasing radius,\n"         \
"ranging from 1 to rmax,\n"         \
"are generated. Let D1...Dn be these structuring elements, sorted by increasing radius.\n"         \
"Let I(0) = in.pgm, the ith intermediate result I(i) is obtained by the \n"         \
"homotopic pseudo-closing of the homotopic pseudo-opening of I(i-1) by the structuring element Di. \n"         \
"Two images  c.pgm and  cc.pgm can be given to serve as constraints. \n"         \
"The image  c.pgm must be 0 except for the points x that are to be preserved,\n"         \
"which can be set to 255 or to I[x].\n"         \
"The image  cc.pgm must be 255 except for the points x that are to be preserved,\n"         \
"which can be set to 0 or to I[x].\n"         \
"The result out.pgm contains the final result I(n).\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: morpho, topogray\n"         \
"  morpho, topogray\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__lambdamedialaxisauto__c__ "  lambdamedialaxis.c\n"         \
"\n"         \
" discrete lambda-medial axis transform\n"         \
"\n"         \
"Usage: lambdamedialaxis in.pgm [lambda] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Discrete lambda-medial axis, as defined in [CCT09], \n"         \
"of the binary image  X contained in  in.pgm.\n"         \
"\n"         \
"By default, the output is a float image representing the function x->lambda(x).\n"         \
"\n"         \
"If the parameter  lambda is given (bash interface only, not python), the output is a binary image obtained by thresholding the aforementioned function at the value  lambda .\n"         \
"\n"         \
"If the parameter  lambda is given and set to 0, the output is a binary image obtained by thresholding the aforementioned function at the automaticaly defined (k-means) value  lambda . \n"         \
"\n"         \
" References: \n"         \
"[CCT09] John Chaussard, Michel Couprie, and Hugues Talbot. A discrete lambda-medial axis. 15th Discrete Geometry for Computer Imagery (DGCI'09). Lecture Notes in Computer Science.   2009.  pp. 1â€“12. To appear.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: morpho\n"         \
"  morpho\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__2dseltopo__c__ "  2dseltopo.c\n"         \
"\n"         \
" selects points in the 2d khalimsky grid according to a local topological criterion \n"         \
"\n"         \
"Usage: 2dseltopo in d- d+ a- a+ b- b+ out\n"         \
"\n"         \
"Description:\n"         \
"  Selects the elements x of the Khalimsky object K\n"         \
"  which satisfy the following inequalities : \n"         \
"\n"         \
"   d1  \n"         \
"\n"         \
"   a1  \n"         \
"\n"         \
"   b1  \n"         \
"\n"         \
"  where\n"         \
"\n"         \
"  d(x) = dimension of x\n"         \
"\n"         \
"  a(x) = number of elements under x of dimension d(x) - 1\n"         \
"\n"         \
"  b(x) = number of elements over x of dimension d(x) + 1\n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: orders\n"         \
" orders\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__watershedMeyer_regular__c__ "  watershedMeyer.c\n"         \
"\n"         \
" watershed transformation (Meyer's algorithm)\n"         \
"\n"         \
"Usage: watershedMeyer in mark {bgmark|null} {roi|null} connex out\n"         \
"\n"         \
"Description:\n"         \
"Performs the watershed transformation on the image in, taking the\n"         \
"set of markers in mark. \n"         \
"If this parameter is present, bgmark\n"         \
"is used as a set of markers for the background.\n"         \
"If this parameter is present, roi\n"         \
"indicates the region of interest on which the operation is performed.\n"         \
"The parameter connex gives the adjacency relation (4,8 in 2D; 6,18,26 in 3D) \n"         \
"for the makers.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: connect\n"         \
" connect\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__yuv2rgb__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__skel_CK3a__c__ "  skel_CK3a.c\n"         \
"\n"         \
" parallel 3D binary curvilinear skeleton based on 1D isthmuses\n"         \
"\n"         \
"Usage: skel_CK3a in.pgm nsteps [inhibit] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Parallel 3D binary thinning or curvilinear skeleton based on 1D isthmuses. \n"         \
"The parameter  nsteps gives,\n"         \
"if positive, the number of parallel thinning steps to be processed.\n"         \
"If the value given for  nsteps equals -1, the thinning is continued\n"         \
"until stability.\n"         \
"\n"         \
"If the parameter  inhibit is given and is a binary image name,\n"         \
"then the points of this image will be left unchanged. \n"         \
"\n"         \
"Warning: The object must not have any point on the frame of the image.\n"         \
"\n"         \
"Types supported: byte 3d\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__ellipticite__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__2elisp__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__hthick__c__ "  hthick.c\n"         \
"\n"         \
" grayscale homotopic thickening\n"         \
"\n"         \
"Usage: hthick in.pgm {imcond.pgm|null} connex niter out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Grayscale homotopic thickening (refs. [BEC97, CBB01]).\n"         \
"The parameter  connex gives the connectivity used for the minima;\n"         \
"possible choices are 4, 8 in 2D and 6, 26 in 3D.\n"         \
"Let F be the function corresponding to the input image  in.pgm.\n"         \
"Let G be the function corresponding to the input image \n"         \
" imcond.pgm, or the blanck function (constant 255) if the keyword  null is used.\n"         \
"The algorithm is the following:\n"         \
"\n"         \
"\n"         \
"Repeat niter times:\n"         \
"    L = {(p,d) such that p is constructible for F and d = delta+(p,F)}\n"         \
"    While L not empty\n"         \
"       extract a couple (p,d) from L\n"         \
"       F(p) = min{ d, delta+(p,F), G(p) }    \n"         \
"Result: F\n"         \
"\n"         \
"\n"         \
"If  niter is set to -1, then the operator iterates until stability.\n"         \
"\n"         \
"References: \n"         \
"[BEC97] G. Bertrand, J. C. Everat and M. Couprie: \"Image segmentation through operators based upon topology\",  Journal of Electronic Imaging, Vol.&nbsp;6, No.&nbsp;4, pp.&nbsp;395-405, 1997.\n"         \
"[CBB01] M. Couprie, F.N. Bezerra, Gilles Bertrand: \"Topological operators for\n"         \
"grayscale image processing\",  Journal of Electronic Imaging, Vol.&nbsp;10, No.&nbsp;4, pp.&nbsp;1003-1015, 2001.\n"         \
"\n"         \
"Types supported: byte 2D, byte 3D.\n"         \
"\n"         \
"Category: topogray\n"         \
"  topogray\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__skelpar3d__c__ "  skelpar3d.c\n"         \
"\n"         \
" parallel 3D binary skeleton\n"         \
"\n"         \
"Usage: skelpar3d in.pgm algorithm nsteps [inhibit] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"Parallel 3D binary thinning or skeleton. \n"         \
"\n"         \
"The parameter  nsteps gives, if positive, the number of parallel\n"         \
"thinning steps to be processed.  If the value given for  nsteps\n"         \
"equals -1, the thinning is continued until stability.\n"         \
"\n"         \
"For some algorithms (16 to ?? ), the parameter  nsteps has a\n"         \
"different meaning which is related to the notion of persistence (see\n"         \
"[REF A AJOUTER]). It plays the role of a filtering parameter.\n"         \
"\n"         \
"The parameter  algorithm is a numerical code\n"         \
"indicating which method will be used for the thinning.\n"         \
"The possible choices are:\n"         \
" 0: ultimate, without constraint (MK3a)\n"         \
" 1: curvilinear, symmetric, based on 1D isthmus (CK3a)\n"         \
" 2: medial axis preservation (AK3) - parameter inhibit represents the minimal radius of medial axis balls which are considered\n"         \
" 3: ultimate (MK3) - if nsteps = -2, returns the topological distance\n"         \
" 4: curvilinear based on ends (EK3)\n"         \
" 5: curvilinear based on ends, with end reconstruction (CK3b)\n"         \
" 6: topological axis (not homotopic)\n"         \
" 7: curvilinear, based on residual points and 2D isthmus (CK3)\n"         \
" 8: ultimate, asymmetric (AMK3)\n"         \
" 9: curvilinear, asymmetric, based on thin 1D isthmus (ACK3a)\n"         \
" 10: curvilinear, asymmetric, based on 3D and 2D residuals (ACK3)\n"         \
" 11: surface, symmetric, based on residual points (RK3)\n"         \
" 12: surface, symmetric, based on 2D isthmuses (SK3)\n"         \
" 13: ultimate, directional, (DK3)\n"         \
" 14: surface, directional, based on residual points (DRK3)\n"         \
" 15: surface, directional, based on 2D isthmuses (DSK3)\n"         \
" 16: curvilinear, asymmetric, based on thin 1D isthmus with persistence (ACK3p)\n"         \
" 17: surface, asymmetric, based on thin 2D isthmus with persistence (ASK3p)\n"         \
" 18: curvilinear, symmetric, based on 1D isthmus with persistence (CK3p)\n"         \
" 19: surface, symmetric, based on 2D isthmus with persistence (SK3p)\n"         \
" 20: surface and curvilinear, symmetric, based on 1D and 2D isthmus with persistence (SCK3p)\n"         \
" 21: surface, symmetric, based on residual points (RK3), variant (uses 26-connectivity to define residual points)\n"         \
"\n"         \
"\n"         \
"In modes other than 2, if the parameter  inhibit is given and is a\n"         \
"binary image name, then the points of this image will be left\n"         \
"unchanged.\n"         \
"\n"         \
"The following codes give access to auxiliary functions, for isthmus detection. \n"         \
"Parameter  nsteps will not be used in this case (any value can be given).\n"         \
"\n"         \
" 100: 1D isthmus points\n"         \
"\n"         \
"Warning: The object must not have any point on the frame of the image.\n"         \
"\n"         \
"The following codes give access to auxiliary functions, for isthmus detection. \n"         \
"Parameter  nsteps will not be used in this case (any value can be given).\n"         \
"\n"         \
" 100: 1D isthmus points\n"         \
"\n"         \
"Warning: The object must not have any point on the frame of the image.\n"         \
"\n"         \
"Types supported: byte 3d\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__split__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__fitcircle__c__ "  fitcircle.c\n"         \
"\n"         \
" fits a circle which locally minimizes gray levels\n"         \
"\n"         \
"Usage: fitcircle in.pgm x y r [translate] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Identifies the circle C which locally minimizes the mean gray level \n"         \
"of the image  in.pgm in the vincinity of the circle\n"         \
"of parameters  x ,  y and  r.\n"         \
"If the parameter  translate is added, then the minimization is\n"         \
"done only for dimensions x and y.\n"         \
"Draws this circle in  out.pgm .\n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: geo\n"         \
"  geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__lambdaskel__c__ "  lambdaskel.c\n"         \
"\n"         \
" grayscale filtered topological skeleton\n"         \
"\n"         \
"Usage: lambdaskel in.pgm {imcond.pgm|null} connex lambda out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Filtered topological skeleton for 2D grayscale images. \n"         \
"The parameter  connex gives the connectivity used for the minima;\n"         \
"possible choices are 4 and 8.\n"         \
"The parameter  lambda is a contrast parameter (positive integer).\n"         \
"Let F be the function corresponding to the input image  in.pgm.\n"         \
"The parameter  imcond.pgm is a constraint function G.\n"         \
"The algorithm is the following:\n"         \
"\n"         \
"\n"         \
"Repeat until stability:\n"         \
"    Select a point p which is lambda-destructible for F or a peak\n"         \
"        such that F(p) > G(p) and such that F(p) is minimal\n"         \
"    F(p) = alpha-(p,F)\n"         \
"Result: F\n"         \
"\n"         \
"\n"         \
"Reference: \n"         \
"M. Couprie, F.N. Bezerra, Gilles Bertrand: \"Topological operators for\n"         \
"grayscale image processing\",  Journal of Electronic Imaging, Vol.&nbsp;10, No.&nbsp;4, pp.&nbsp;1003-1015, 2001.\n"         \
"\n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: topogray\n"         \
"  topogray\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__extrude__c__ "  extrude.c\n"         \
"\n"         \
" catenates a sequence of 2d (resp. 1d) images, made of the repetition of the same image, into a 3d (resp. 2d) image\n"         \
"\n"         \
"Usage: extrude in.pgm nbrep [xdim ydim zdim] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Catenates a sequence of 2d (resp. 1d) images, made of the repetition of the same image, into a 3d (resp. 2d) image.\n"         \
"\n"         \
"Types supported: byte 1d, 2d\n"         \
"\n"         \
"Category:  convert\n"         \
"   convert\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__min__c__ "  min.c\n"         \
"\n"         \
" minimum of 2 images\n"         \
"\n"         \
"Usage: min in1.pgm in2.pgm out.pgm\n"         \
"\n"         \
"Description:\n"         \
"For each pixel x, out[x] = min{in1[x], in2[x]}. \n"         \
"Images must be of the same type and same dimensions.\n"         \
"\n"         \
"Types supported: byte 2D, byte 3D\n"         \
"\n"         \
"Category: arith\n"         \
"  arith\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__3dseltopo__c__ "  3dseltopo.c\n"         \
"\n"         \
" selects points in the 3d khalimsky grid according to a local topological criterion \n"         \
"\n"         \
"Usage: 3dseltopo in d- d+ a- a+ b- b+ out\n"         \
"\n"         \
"Description:\n"         \
"  Selects the elements x of the Khalimsky volume K\n"         \
"  which satisfy the following inequalities : \n"         \
"\n"         \
"   d1  \n"         \
"\n"         \
"   a1  \n"         \
"\n"         \
"   b1  \n"         \
"\n"         \
"  where\n"         \
"\n"         \
"  d(x) = dimension of x\n"         \
"\n"         \
"  a(x) = number of elements under x of dimension d(x) - 1\n"         \
"\n"         \
"  b(x) = number of elements over x of dimension d(x) + 1\n"         \
"\n"         \
"Types supported: byte 3d\n"         \
"\n"         \
"Category: orders\n"         \
" orders\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__lferode3d_rect__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__ui_any_to_complex__c__ "\n"         \
"    ui_any_to_complex.cpp\n"         \
"\n"         \
"    Creates a complex image from two images (one representing\n"         \
"   the real, and the other the imaginary part of the complex image)\n"         \
"   \n"         \
"   Usage: !!!!!!!\n"         \
"   \n"         \
"   Description: The two images can be of any type, but they need to be of the same time.\n"         \
"\n"         \
"   NOTE: If you use complex images, than some information will\n"         \
"   be lost, as the resulting image will be float-complex anyway. Also\n"         \
"   if you use integers, some information may dissapear.\n"         \
"\n"         \
"    Laszlo Marak, 2011\n"         \
"\n"         \
"\n"         \
" \n"         \
"\n"


#define doc__identifyparabola2__c__ "  identifyparabola2.c\n"         \
"\n"         \
" identification of a best matching parabola from a set of 2D points\n"         \
"\n"         \
"Usage: identifyparabola2 in.list out.list\n"         \
"\n"         \
"Description:\n"         \
"Identifies the parameters (a,b) of the equation of the 2D parabola:\n"         \
"ax^2+b=y that minimizes the least square error between this parabola \n"         \
"and the given points. Method: basic linear regression.\n"         \
"\n"         \
"Types supported: list 1D, list 2D\n"         \
"\n"         \
"Category: geo\n"         \
"  geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__waterfall__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__raw2pgm__c__ "  raw2pgm.c\n"         \
"\n"         \
" converts from raw format into pgm format\n"         \
"\n"         \
"Usage: raw2pgm in.raw rs cs ds headersize datatype littleendian [xdim ydim zdim] out.pgm\n"         \
"\n"         \
"Description: Converts from raw format into pgm format. \n"         \
"\n"         \
"Parameters:\n"         \
"  in.pgm : source file in raw format\n"         \
"  rs (int32_t): row size (number of voxels in a row)\n"         \
"  cs (int32_t): column size (number of voxels in a column)\n"         \
"  ds (int32_t): number of planes\n"         \
"  headersize (int32_t): size of the header in bytes (information in the header will be ignored)\n"         \
"  datatype (int32_t): (1 for byte, 2 for short int, 4 for long int, 5 for float)\n"         \
"  littleendian (int32_t) 1: littleendian, 0: bigendian. Usual choice is 0.\n"         \
"  xdim (float, optional) : gap (in the real world) between two adjacent voxels in a row.  \n"         \
"  ydim (float, optional) : gap (in the real world) between two adjacent voxels in a column.  \n"         \
"  zdim (float, optional) : gap (in the real world) between two adjacent planes.  \n"         \
"\n"         \
"Types supported: byte 2D-3D, int16_t 2D-3D, int32_t 2D-3D, float 2D-3D\n"         \
"\n"         \
" Signed integers are not supported.\n"         \
"\n"         \
"Category: convert\n"         \
" convert\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__skelendlab__c__ "  skelendlab.c\n"         \
"\n"         \
" homotopic skeleton of a 2d or 3d label image with dynamic detection of end points\n"         \
"\n"         \
"Usage: skelendlab in.pgm connex [n] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Homotopic skeletonization by iterative removal of simple, non-end points. \n"         \
"Each label is treated separately as a binary image.\n"         \
"Breadth-first strategy.\n"         \
"During the first  n iterations (default 0), the end points\n"         \
"are removed as well.\n"         \
"If  n = -1, the end points are always removed.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" This operator is very sensitive to small contour irregularities. \n"         \
"See skelcurv for a more robust operator. Useful for reducing quasi-curvilinear objects.\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__GAwatershed__c__ "  GAwatershed.c\n"         \
"\n"         \
" Compute the watershed of a 4-connected edge-weighted graph (a GA)\n"         \
"\n"         \
"Usage: GAwatershed in.ga out.pgm [type]\n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"Compute the watershed of a 4-connected edge-weighted graph (a GA).\n"         \
"\n"         \
"If type = 0, then outputs a watershed by an M-border algorithm. The\n"         \
"file out.pgm is a GA.\n"         \
"\n"         \
"If type = 1, then outputs a watershed by a non-recursive algorithm\n"         \
"based on streams. The file out.pgm is a GA (this is the default) (not\n"         \
"available for floats).\n"         \
"\n"         \
"If type = 2 outputs a watershed by a recursive algorithm based on\n"         \
"streams. The file out.pgm is a GA (not available for floats).\n"         \
"\n"         \
"If type = 3 outputs an M-border watershed. The file out.pgm is a GA\n"         \
"(not available for floats).\n"         \
"\n"         \
"If type = 4 outputs a flow mapping. The file out.pgm is a long integer\n"         \
"image that represents a labeled partition induced by a watershed cut\n"         \
"of the input GA (not available for floats).\n"         \
"\n"         \
"\n"         \
"\n"         \
"Types supported: GA byte 2D, GA float 2D\n"         \
"\n"         \
"Category: GA\n"         \
"  GA\n"         \
"\n"         \
" Jean Cousty\n"         \
"\n"         \
"\n"


#define doc__simplepair__c__ "  simplepair.c\n"         \
"\n"         \
" detects simple pairs in a 3D binary image\n"         \
"\n"         \
"Usage: simplepair in.pgm [all] out.pgm\n"         \
"\n"         \
"Note:  in the python front-end the parameter 'all' is mandatory, if 0 at most \n"         \
"one simple pair is found, if it's 1 all simple pairs are looked for\n"         \
"\n"         \
"Description:\n"         \
"Detects simple pairs in a 3D binary image. \n"         \
"If the optional parameter  all is used, then all simple pairs are searched. \n"         \
"Otherwise, only one pair (if any) is given.\n"         \
"\n"         \
"Reference: \n"         \
"[PCB08] Nicolas Passat, Michel Couprie, and Gilles Bertrand:\n"         \
"\"Minimal simple pairs in the 3D cubic grid\", in\n"         \
"Journal of Mathematical Imaging and Vision, 32 (3), pp.&nbsp;239â€“249, November 2008.\n"         \
"\n"         \
"Types supported: byte 3d\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" Michel Couprie 2008\n"         \
"\n"         \
"\n"


#define doc__xor__c__ "  xor.c\n"         \
"\n"         \
" pixelwise xor of two images\n"         \
"\n"         \
"Usage: xor in1.pgm in2.pgm out.pgm\n"         \
"\n"         \
"Description:\n"         \
"For each pixel x, out[x] = in1[x] xor in2[x]. \n"         \
"Images must be of the same type and same dimensions.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: arith\n"         \
"  arith\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__ungerflow__cpp__ "\n"         \
"    ungerflow.cpp\n"         \
"\n"         \
"    Noise filtering with the Ungerflow algorithm [1].\n"         \
"   \n"         \
"   Usage: ungerflow( (char_image)source and sink, (float_image)gradient image, (float_image)lambda image, (float_image)guidence image, (int)number of iterations, (float)tau, (float)theta) -> float_image \n"         \
"   \n"         \
"   Description: \n"         \
"\n"         \
"   The input image is noise filtered according to the following\n"         \
"   relation:\n"         \
"   \n"         \
"      v}\n"         \
"        g(x)| u|d + \n"         \
"        + \n"         \
"        \n"         \
"       \n"         \
"     \n"         \
"   \n"         \
"   Types supported: float 2d, float 3d, float 4d, ..., float xd\n"         \
"   \n"         \
"   Category: signal, development\n"         \
"     signal, development\n"         \
"\n"         \
"   References:\n"         \
"   [1] Horst Bischof Markus Unger, Thomas Pock. Interactive\n"         \
"   globally optimal image segmentation. Technical Report ICGTR08/02,\n"         \
"   Institute for Computer Graphics and Vision Graz University of\n"         \
"   Technology, Austria, July 2008. \n"         \
"   [2] M. Unger, T. Pock, and H. Bischof. Continuous\n"         \
"   globally optimal image segmentation with local constraints. In\n"         \
"   Computer Vision Winter Workshop 2008, Moravske Toplice, Slovenija,\n"         \
"   February 2008.\n"         \
"\n"         \
"   Examples: can be found in 'tutorial/python/ungerflow'\n"         \
"\n"         \
"    Laszlo Marak, 2009-2010\n"         \
"\n"         \
"\n"         \
" \n"         \
"\n"


#define doc__reconseros3d___notused_c__ "WRITE ME!!!\n"         \
"\n"


#define doc__skelpruning__c__ "  skelpruning.c\n"         \
"\n"         \
" pruning of \"short end branches\" in a curvilinear skeleton\n"         \
"\n"         \
"Usage: skelpruning in.skel length out.pgm\n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"The skeleton found in  is searched for \"small\" branches which satisfy the following criteria:\n"         \
" Branch has exactly one end\n"         \
" Branch length is less than or equal to  length parameter.\n"         \
"\n"         \
"Parameter  length is a number pixels.\n"         \
"\n"         \
"Matching arcs are written in  out.pgm.\n"         \
"\n"         \
"Types supported: skel 2d, skel 3d\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" Michel Couprie 2011\n"         \
"\n"         \
"\n"


#define doc__ellipseincl__c__ "  ellipseincl.c\n"         \
"\n"         \
" ellipse identification and drawing from spare points\n"         \
"\n"         \
"Usage: ellipseincl in.pgm pts.list filled out.pgm\n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: geo\n"         \
"  geo\n"         \
"\n"         \
" Michel Couprie and Yskandar Hamam\n"         \
"\n"         \
"\n"


#define doc__drawscene__c__ "  drawscene.c\n"         \
"\n"         \
" draw a 3D scene which is specified by a text file into a 3D pgm image\n"         \
"\n"         \
"Usage: drawscene in.pgm scene.3sc out.pgm\n"         \
"\n"         \
"Description:\n"         \
"The file  scene.3sc contains a scene under the 3Dscene format.\n"         \
"\n"         \
"Types supported: byte 3D\n"         \
"\n"         \
"Category: draw geo\n"         \
"  draw geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__pgm2skel__c__ "  pgm2skel.c\n"         \
"\n"         \
" decomposition of a curvilinear skeleton into isolated points, end points, curves and junctions\n"         \
"\n"         \
"Usage: pgm2skel in.pgm connex [len] out.skel\n"         \
"\n"         \
"Description:\n"         \
"The skeleton found in  in.pgm is decomposed into isolated points, end points, curves and junctions ;\n"         \
"and its description is stored in  out.skel .\n"         \
"The parameter  connex sets the adjacency relation used for the object\n"         \
"(4, 8 (2d) or 6, 18, 26 (3d)).\n"         \
"\n"         \
"The optional parameter  indicates the minimum length \n"         \
"(in pixels/voxels) of a curve. \n"         \
"\n"         \
"If a set of curve points with less than  points, then: \n"         \
"\n"         \
"a) if it contains at least one end point it will be eliminated\n"         \
"(together with its end point(s)),\n"         \
"\n"         \
"b) otherwise it will be considered as part of a junction.  \n"         \
"\n"         \
"If this parameter is given, then isolated points will be eliminated.\n"         \
"\n"         \
"The format of the file  is described in\n"         \
"PINKDIR/doc/formats.txt (section: Curvilinear skeletons).\n"         \
"\n"         \
" Points at the border of the image will be ignored.\n"         \
"\n"         \
" IMPORTANT LIMITATION: \n"         \
"different junctions in the original image must not be in direct\n"         \
"contact with each other (i.e., connected) otherwise they will be\n"         \
"considered as a single junction. To prevent this to occur, one can\n"         \
"increase image resolution.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" Michel Couprie 2004, 2009\n"         \
"\n"         \
"\n"


#define doc__hma__c__ "\n"         \
" Higher-resolution Medial Axis (HMA) extraction\n"         \
"\n"         \
"Usage: hma in.pgm [mode] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Higher-resolution Medial Axis (HMA) of the binary image  Xh, where Xh has\n"         \
"been obtained by H(X), X being the original object and H, the H-transform.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: morpho\n"         \
"  morpho\n"         \
"\n"         \
" Andre Vital Saude\n"         \
"\n"         \
"\n"


#define doc__directions__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__interpolate__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__closeball__c__ "  closeball.c\n"         \
"\n"         \
" morphological binary closing by a ball\n"         \
"\n"         \
"Usage: closeball in.pgm r [dist] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"The closing consists in a dilation (dilatball) followed by an erosion (erosball).\n"         \
"The structuring element for this closing is a ball (or a disc) of radius  r.\n"         \
"The erosion and dilation are computed by thresholding a distance map.\n"         \
"The distance used depends on the optional parameter  dist (default is 0) :\n"         \
" 0: rounded euclidean distance\n"         \
" 2: chamfer distance\n"         \
" 3: exact quadratic euclidean distance\n"         \
" 4: 4-distance in 2d\n"         \
" 8: 8-distance in 2d\n"         \
" 6: 6-distance in 3d\n"         \
" 18: 18-distance in 3d\n"         \
" 26: 26-distance in 3d\n"         \
"\n"         \
" The input image  in.pgm must be a binary image. No test is done.\n"         \
"\n"         \
"Types supported: binary byte 2D, byte 3D\n"         \
"\n"         \
"Category: morpho\n"         \
"  morpho\n"         \
"\n"         \
" Michel Couprie 2002\n"         \
"\n"         \
"Example:\n"         \
"\n"         \
"closeball cells 3 cells_closeball\n"         \
"\n"         \
"\n"         \
"  \n"         \
"      \n"         \
"  \n"         \
"  cells\n"         \
"      cells_closeball\n"         \
"  \n"         \
"\n"         \
"\n"         \
"\n"         \
"\n"


#define doc__tuf__c__ "  tuf.c\n"         \
"\n"         \
" topological upper filter\n"         \
"\n"         \
"Usage: tuf in.pgm connexmin r out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Topological upper filter. Performs the homotopic thinning controlled by\n"         \
"a radius r, followed by a peak deletion, and a homotopic reconstruction under\n"         \
"the original image.\n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: topogray\n"         \
"  topogray\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__moyenne__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__hthinbin__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__egalise__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__openball__c__ "  openball.c\n"         \
"\n"         \
" morphological binary opening by a ball\n"         \
"\n"         \
"Usage: openball in.pgm r [dist] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"The structuring element for this opening is a ball (or a disc) of radius  r.\n"         \
"The opening consists in a erosion (erosball) followed by an dilation (dilatball).\n"         \
"The erosion and dilation are computed by thresholding a distance map.\n"         \
"The distance used depends on the optional parameter  dist (default is 0) :\n"         \
" 0: rounded euclidean distance\n"         \
" 2: chamfer distance\n"         \
" 3: exact quadratic euclidean distance\n"         \
" 4: 4-distance in 2d\n"         \
" 8: 8-distance in 2d\n"         \
" 6: 6-distance in 3d\n"         \
" 18: 18-distance in 3d\n"         \
" 26: 26-distance in 3d\n"         \
"\n"         \
" The input image  in.pgm must be a binary image. No test is done.\n"         \
"\n"         \
"Types supported: byte 2D, byte 3D\n"         \
"\n"         \
"Category: morpho\n"         \
"  morpho\n"         \
"\n"         \
" Michel Couprie 2002\n"         \
"\n"         \
"Example:\n"         \
"\n"         \
"openball cells 3 cells_openball\n"         \
"\n"         \
"\n"         \
"  \n"         \
"      \n"         \
"  \n"         \
"  cells\n"         \
"      cells_openball\n"         \
"  \n"         \
"\n"         \
"\n"         \
"\n"         \
"\n"


#define doc__heightselnb__c__ "  heightselnb.c\n"         \
"\n"         \
" height based segmentation by selection of a given number or components\n"         \
"\n"         \
"Usage: heightselnb in.pgm connex nb out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Segmentation by selection of a given number or components, based on a \n"         \
"height (contrast) criterion. \n"         \
"Connexity = connex ; desired number of components = nb.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: connect\n"         \
" connect\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__lambdamedialaxis2__c__ "  lambdamedialaxis2.c\n"         \
"\n"         \
" discrete lambda-medial axis transform\n"         \
"\n"         \
"Usage: lambdamedialaxis2 in.pgm auxlambda lambda out.pgm\n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"Experimental\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: morpho\n"         \
"  morpho\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__skeleucl__c__ "  skeleucl.c\n"         \
"\n"         \
" Euclidean binary skeleton\n"         \
"\n"         \
"Usage: skeleucl in.pgm connex [inhibit] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Euclidean binary skeleton.\n"         \
"\n"         \
"The parameter  connex indicates the connectivity of the binary object.\n"         \
"Possible choices are 4, 8 in 2d and 6, 26 in 3d.\n"         \
"\n"         \
"If the parameter  inhibit is given and is a binary image name,\n"         \
"then the points of this image (constraint set) will be left unchanged. \n"         \
"\n"         \
"This operator is usually used with a constraint set (parameter  inhibit) that is a subset of the exact Euclidean medial axis (see operator medialaxis). \n"         \
"\n"         \
"Here is an example using the whole medial axis as constraint set:\n"         \
"\n"         \
"\n"         \
"medialaxis test.pgm 3 _1\n"         \
"threshold _1 1 _2\n"         \
"skeleucl test.pgm 8 _2 result.pgm\n"         \
"\n"         \
"\n"         \
"Intersesting subsets are obtained by filtering the medial axis, either based on the ball radiuses, or based on the bisector angle (see operator bisector). Below is a script showing how to proceed:\n"         \
"\n"         \
"\n"         \
"#!/bin/sh\n"         \
"USAGE=\"Usage: $0 in seuilR (in [1..infnty[) seuilA (in [0.001..pi]) out\"\n"         \
"if [ $# -ne 4 ]\n"         \
"then\n"         \
"	echo $USAGE\n"         \
"        exit\n"         \
"fi\n"         \
"medialaxis $1 3 /tmp/skel2_tmp_m\n"         \
"threshold /tmp/skel2_tmp_m 1 /tmp/skel2_tmp_m1\n"         \
"skeleucl $1 8 /tmp/skel2_tmp_m1 /tmp/skel2_tmp_s\n"         \
"threshold /tmp/skel2_tmp_m $2 /tmp/skel2_tmp_ms\n"         \
"distc $1 3 /tmp/skel2_tmp_d\n"         \
"bisector /tmp/skel2_tmp_d /tmp/skel2_tmp_ms /tmp/skel2_tmp_a\n"         \
"threshold /tmp/skel2_tmp_a $3 /tmp/skel2_tmp_i\n"         \
"skeleton /tmp/skel2_tmp_s /tmp/skel2_tmp_d 8 /tmp/skel2_tmp_i $4\n"         \
"rm -f /tmp/skel2_tmp_*\n"         \
"\n"         \
"\n"         \
"References: \n"         \
"[CCZ07] M. Couprie, D. Coeurjolly and R. Zrour: \"Discrete bisector function and Euclidean skeleton in 2D and 3D\", Image and Vision Computing, Vol.&nbsp;25, No.&nbsp;10, pp.&nbsp;1543-1556, 2007.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__axones___notused_c__ "WRITE ME!!!\n"         \
"\n"


#define doc__graph2ps__c__ "  graph2ps.c\n"         \
"\n"         \
" draws a planar graph into a ps file\n"         \
"\n"         \
"Usage: graph2ps in.graph out.ps\n"         \
"\n"         \
"Description:\n"         \
"Draws a planar graph into a ps file.\n"         \
"\n"         \
"Types supported: graph\n"         \
"\n"         \
"Category: convert\n"         \
"  convert\n"         \
"\n"         \
" Michel Couprie 2009\n"         \
"\n"         \
"\n"


#define doc__skel_SK3_pers__c__ "  skel_SK3_pers.c\n"         \
"\n"         \
" topological persistence of 1D isthmuses, based on parallel 3D asymetric thinning\n"         \
"\n"         \
"Usage: skel_SK3_pers in.pgm out.pgm\n"         \
"\n"         \
"Description: Topological persistence of 1D isthmuses, based on \n"         \
"parallel 3D asymetric thinning.\n"         \
"\n"         \
"When a point x is detected as a 1D isthmus, a counter p(x) is\n"         \
"associated to this point and initialized with value 1. This counter is\n"         \
"incremented a each iteration as long as x is still an isthmus. When this point x is\n"         \
"eventually deleted, the value of the counter is freezed.\n"         \
"\n"         \
" The object must not have any point on the frame of the image.\n"         \
"\n"         \
"Types supported: byte 3d\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__surrect__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__deletepeaks__c__ "  deletepeaks.c\n"         \
"\n"         \
" deletes peaks in a grayscale image \n"         \
"\n"         \
"Usage: deletepeaks filein.pgm [mask] connexmin fileout.pgm\n"         \
"\n"         \
"Description:\n"         \
"Deletes peaks in a grayscale image. A point p is a peak if T+(p) = 0\n"         \
"(see ref. [BEC97]).\n"         \
"\n"         \
"References: \n"         \
"[BEC97] G. Bertrand, J. C. Everat and M. Couprie: \"Image segmentation through operators based upon topology\",  Journal of Electronic Imaging, Vol.&nbsp;6, No.&nbsp;4, pp.&nbsp;395-405, 1997.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: topogray\n"         \
"  topogray\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__grid__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__souzabanon__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__surf___notused_c__ "  surf.c\n"         \
"\n"         \
" topographic view of a 2D image\n"         \
"\n"         \
"Usage: surf in.pgm mode out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Creates a gray-scale image\n"         \
"with shading view 2D gray-scale image as a\n"         \
"topographical surface. Useful to view\n"         \
"distance transforms and results of morphological\n"         \
"operators.\n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: convert\n"         \
"  convert\n"         \
"\n"         \
" Roberto Lotufo\n"         \
"\n"         \
"\n"


#define doc__drawcircle_notused_c__ "  drawellipse.c\n"         \
"\n"         \
" Draws an ellipse\n"         \
"\n"         \
"Usage: drawellipse in.pgm x1 y1 x2 y2 X1 Y1 X2 Y2 [filled] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Draws an  ellipse specified by four points, e.g. the extremities of the small axis \n"         \
"(x1, y1, x2, y2) and the extremities of the big axis \n"         \
"(X1, Y1, X2, Y2).\n"         \
"The ellipse is surimposed to the contents of in.pgm.\n"         \
"If the (optional) int32_t parameter  filled is set (1), then the ellipse is filled.\n"         \
"Otherwise it is empty (default).\n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: draw geo\n"         \
"  draw geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__reconseros___notused_c__ "WRITE ME!!!\n"         \
"\n"


#define doc__gradinf3d___notused_c__ "WRITE ME!!!\n"         \
"\n"


#define doc__voronoilabelling__c__ "  voronoilabelling.c\n"         \
"\n"         \
" voronoi labelling (and squared Euclidean distance transform)\n"         \
"\n"         \
"Usage: voronoilabelling in.pgm [dist.pgm] vor.pgm\n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"Types supported: byte 2d,  byte 3d\n"         \
"\n"         \
"Category: morpho\n"         \
"  morpho\n"         \
"\n"         \
"\n"         \
"\n"


#define doc__dynrecons__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__convol__c__ "  convol.c\n"         \
"\n"         \
" convolution\n"         \
"\n"         \
"Usage: convol in.pgm kernel.pgm [mode] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Convolution of  in.pgm by kernel.pgm.\n"         \
"The result is a float image.\n"         \
"Depending on the value given for the (optional) parameter mode:\n"         \
"   mode = 0 (default) : naive algorithm. \n"         \
"      The image  in.pgm is considered as null out of its support. \n"         \
"   mode = 1 : naive algorithm. \n"         \
"      The boundary of image  in.pgm is extended outside its support.\n"         \
"   mode = 2 : convolution using the FFT.\n"         \
"      The image  in.pgm is considered as null out of its support. \n"         \
"   mode = 3 : convolution using the FFT.\n"         \
"      The boundary of image  in.pgm is extended outside its support.\n"         \
"\n"         \
"Types supported: byte 2d, int32_t 2d, float 2d\n"         \
"\n"         \
"Category: signal\n"         \
"  signal\n"         \
"\n"         \
"\n"         \
"Naive convolution algorithm is in O(|in|*|kernel|). \n"         \
"For large kernels, use FFT version which is in \n"         \
"O(n log n) where n = max(|in|,|kernel|)\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__convol3__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__watershedthin__c__ "  watershedthin.c\n"         \
"\n"         \
" Thin watershed transformation (Meyer's algorithm on  perfect fusion grid)\n"         \
"\n"         \
"Usage: watershedthin in mark {bgmark|null} {roi|null} parite out\n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"Performs the watershed transformation on the image in,\n"         \
"considered to be on a perfect fusion grid, taking the set of markers in\n"         \
"mark.  Returns the divide set.\n"         \
"\n"         \
"If this parameter is present, bgmark is used as a\n"         \
"set of markers for the background.  If this parameter is present,\n"         \
"roi indicates the region of interest on which the operation is\n"         \
"performed.  The parameter parite (0 or 1) gives the \n"         \
"the translation of the fusion grid.\n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: connect\n"         \
" connect\n"         \
"\n"         \
" Jean Cousty\n"         \
"\n"         \
"\n"


#define doc__2dmakecomplex__c__ "  2dmakecomplex.c\n"         \
"\n"         \
" makes a cellular complex from the given set\n"         \
"\n"         \
"Usage: 2dmakecomplex in.pgm [i] out.pgm\n"         \
"\n"         \
"Note:  in python for the i option use function 'l2dclosebeta'\n"         \
"\n"         \
"Description:\n"         \
"Makes a cellular complex from the given set, \n"         \
"by performing the topological closure.\n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: orders\n"         \
"  orders\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__ptselectgray__c__ "  ptselectgray.c\n"         \
"\n"         \
" detects points in a 2D or 3D grayscale image corresponding to a given criterion \n"         \
"\n"         \
"Usage: ptselectgray in.pgm connex mint+ maxt+ mint-- maxt-- out.pgm\n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"Select all points p such that the condition:\n"         \
"mint+ \n"         \
"is satisfied.\n"         \
"\n"         \
"The parameter  connex gives the connectivity used for the minima;\n"         \
"possible choices are 4, 8 in 2D and 6, 26 in 3D.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d \n"         \
"\n"         \
"Category: topogray\n"         \
"  topogray\n"         \
"\n"         \
" Nicolas Combaret 2006\n"         \
"\n"         \
"\n"


#define doc__barycentre__c__ "  barycentre.c\n"         \
"\n"         \
" compute the center of gravity of each connected component\n"         \
"\n"         \
"Usage: barycentre in.pgm connex out.pgm\n"         \
"\n"         \
"Description: \n"         \
"Compute the center of gravity of each connected component.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: geo\n"         \
"  geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__reconsplateaux__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__average1__c__ "  average1.c\n"         \
"\n"         \
" return the average of the pixel values of an image\n"         \
"\n"         \
"Usage: average1 in.pgm [mask.pgm] out.list\n"         \
"\n"         \
"Description:\n"         \
"This function returns (in the list out.list) \n"         \
"the average of the pixel values of the image  in.pgm .\n"         \
"If the optional parameter  mask.pgm is given, then only the \n"         \
"values which correspond to non-null points of mask are averaged.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n"         \
"\n"         \
"Category: signal stats\n"         \
"  signal stats\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__kern__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__segmenti__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__visu3d__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__ppm2GA__c__ "  ppm2GA.c\n"         \
"\n"         \
" Computes an edge-weighted graph from an color .ppm image\n"         \
"\n"         \
"Usage: ppm2GA im.ppm param out.ga \n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"Computes a GA (an edge-weighted graph) from a color image. The values\n"         \
"of an edge linking two pixels {x,y} is computed according to the\n"         \
"parameter param.\n"         \
" \n"         \
"If param = 0, the maximum, over the RGB chanels, of absolute\n"         \
"differences of intensities between x and y is used.\n"         \
"\n"         \
"If param = 1, the Euclidean distance between the values of x and y is\n"         \
"used.\n"         \
"\n"         \
"If param = 2, fuzzy affinity between x and y is used.\n"         \
"\n"         \
"If im.ppm is a 2D (resp. 3D) image, then out.ga is a 2D (resp\n"         \
"3D GA), that is a 2D 4-connected edge-weighted graph (resp. a 2D\n"         \
"6-connected edge-weighted graph).\n"         \
"\n"         \
"\n"         \
"Types supported: ppm 2D\n"         \
"\n"         \
"Category: \n"         \
"  GA\n"         \
"\n"         \
" Jean Cousty\n"         \
"\n"         \
"\n"


#define doc__threshold__c__ "  threshold.c\n"         \
"\n"         \
" simple threshold\n"         \
"\n"         \
"Usage: threshold in.pgm n [n2] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"If n2 is not present, for each pixel x, out[x] = if (in[x] \n"         \
"\n"         \
"If n2 is present, for each pixel x, out[x] = if (n \n"         \
"\n"         \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n"         \
"\n"         \
"Category: arith\n"         \
"  arith\n"         \
"\n"         \
" Michel Couprie 2010\n"         \
"\n"         \
"\n"


#define doc__car2pol__c__ "  car2pol.c\n"         \
"\n"         \
" converts a 2d image from cartesian to polar coordinates\n"         \
"\n"         \
"Usage: in x_center y_center out \n"         \
"\n"         \
"Description:\n"         \
"Converts a 2d image from cartesian to polar coordinates.\n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: convert\n"         \
"  convert\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__drawsplineorient__c__ "  drawsplineorient.c\n"         \
"\n"         \
" draw spline orientations in a vector field\n"         \
"\n"         \
"Usage: drawsplineorient in.pgm in.spline out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Draws spline orientations in a vector field. \n"         \
"The spline is specified by its control points in a text file.\n"         \
"The parameter  in.pgm gives a vector field into which the spline is to be drawn.\n"         \
"The file format for  in.spline is the following for 2D:\n"         \
"\n"         \
"c n+1 (where n+1 denotes the number of control points)\n"         \
"x1 y1\n"         \
"...\n"         \
"xn+1 yn+1\n"         \
"C0X1 C0Y1 C1X1 C1Y1 C2X1 C2Y1 C3X1 C3Y1\n"         \
"...\n"         \
"C0Xn C0Yn C1Xn C1Yn C2Xn C2Yn C3Xn C3Yn\n"         \
"\n"         \
"and in the 3D case:\n"         \
"\n"         \
"C n+1 (where n+1 denotes the number of control points)\n"         \
"x1 y1 z1\n"         \
"...\n"         \
"xn+1 yn+1 zn+1\n"         \
"C0X1 C0Y1 C0Z1 C1X1 C1Y1 C1Z1 C2X1 C2Y1 C2Z1 C3X1 C3Y1 C3Z1\n"         \
"...\n"         \
"C0Xn C0Yn C0Zn C1Xn C1Yn C1Zn C2Xn C2Yn C2Zn C3Xn C3Yn C3Zn\n"         \
"\n"         \
"Types supported: spline 2D, spline 3D\n"         \
"\n"         \
"Category: draw geo\n"         \
"  draw geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__changename__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__3dinvariants__c__ "  3dinvariants.c\n"         \
"\n"         \
" computes the numbers of connected components,\n"         \
"cavities and tunnels of a 3D cubical complex\n"         \
"\n"         \
"Usage: 3dinvariants in.pgm\n"         \
"\n"         \
"Description:\n"         \
"Computes the numbers of connected components,\n"         \
"cavities and tunnels of a 3D cubical complex\n"         \
"\n"         \
"Types supported: byte 3d\n"         \
"\n"         \
"Category: orders\n"         \
"  orders\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__despuits3d__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__worms__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__selectcomp__c__ "  selectcomp.c\n"         \
"\n"         \
" selects a connected component\n"         \
"\n"         \
"Usage: selectcomp in.pgm connex x y z out.pgm\n"         \
"\n"         \
"Description:\n"         \
"The connected component of the binary image in.pgm\n"         \
"(according to the connectivity connex)\n"         \
"which contains the point (x,y,z)\n"         \
"is preserved. All other points are deleted.\n"         \
"Possible values for  connex are 4, 8 (2D), 6, 18, 26, 60, 260 (3D).\n"         \
"Values 60 and 260 correspond to restrictions of 6 and 26 connectivities to the \n"         \
"current xy plane.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: connect\n"         \
" connect\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__bmp2ppm__c__ "  bmp2ppm.c\n"         \
"\n"         \
" \n"         \
"\n"         \
"Usage: \n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"Types supported: byte 2D\n"         \
"\n"         \
"Category: \n"         \
"  \n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__t4pp__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__curvetangents__cxx__ "  curvetangents.c\n"         \
"\n"         \
" computes the tangents of a digital curve\n"         \
"\n"         \
"Usage: curvetangents curve.list [mode masksize] tangents.list\n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"Computes the tangents of a curve which is specified by the ordered list of its points. The output file is a list of normalized tangent vectors, all with origin 0.\n"         \
"\n"         \
"Types supported: curve 2D, curve 3D\n"         \
"\n"         \
"Category: geo\n"         \
" geo\n"         \
"\n"         \
" Michel Couprie\n"         \
" Paulin Sanselme\n"         \
"\n"         \
"\n"


#define doc__hls2rgb__c__ "  hls2rgb.c\n"         \
"\n"         \
" converts from hls to rgb color representation\n"         \
"\n"         \
"Usage: hls2rgb H.pgm L.pgm S.pgm out.ppm\n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"Constructs 1 ppm from 3 pgm files:\n"         \
"   H.pgm : Hue (int32_t - 0 to 359)\n"         \
"   L.pgm : Luminance (byte)\n"         \
"   S.pgm : Saturation (byte)\n"         \
"Some particular values of HUE:\n"         \
"     0  red\n"         \
"    60  yellow\n"         \
"   120  green\n"         \
"   180  cyan\n"         \
"   240  blue\n"         \
"   300  magenta\n"         \
"Based on Foley, Van Damm & al: \"Computer Graphics\", 2nd ed., p. 595\n"         \
"\n"         \
"Types supported: int32_t 2D (hue), byte 2D (others)\n"         \
"\n"         \
"Types supported: byte 2D\n"         \
"\n"         \
"Category: convert\n"         \
"  convert\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__segmentlignes__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__dilatballnum__c__ "  dilatballnum.c\n"         \
"\n"         \
" morphological grayscale dilation by an Euclidean ball\n"         \
"\n"         \
"Usage: dilatballnum in.pgm r out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Morphological grayscale dilation by an Euclidean ball.\n"         \
"The parameter  r must a positive number (int or float), \n"         \
"the structuring element for this dilation is a ball (or a disc) of radius  r.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: morpho\n"         \
"  morpho\n"         \
"\n"         \
" Michel Couprie 2006\n"         \
"\n"         \
"\n"         \
"\n"


#define doc__long2float__c__ "  long2float.c\n"         \
"\n"         \
" converts a \"long\" image to a \"float\" image\n"         \
"\n"         \
"Usage: long2float in [out]\n"         \
"\n"         \
"Description: For each pixel x, out[x] = (float)in[x]\n"         \
"\n"         \
"If the last argument  out is omitted, then out = in.\n"         \
"\n"         \
"Types supported: long 2d, long 3d.\n"         \
"\n"         \
"Category: convert\n"         \
" convert\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__circular_measure__cpp__ "WRITE ME!!!\n"         \
"\n"


#define doc__drawlines__c__ "  drawlines.c\n"         \
"\n"         \
" draw line segments which are specified by a text file\n"         \
"\n"         \
"Usage: drawlines in.pgm vect.txt out.pgm\n"         \
"\n"         \
"Description:\n"         \
"The file  vect.txt contains a list of line segments under the format:\n"         \
"l nb_segments\n"         \
"x11 y11 x12 y12\n"         \
"x21 y21 x22 y22\n"         \
"x31 y31 x32 y32\n"         \
"...\n"         \
"\n"         \
"Types supported: byte 2D\n"         \
"\n"         \
"Category: draw\n"         \
"  draw\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__histosieve__c__ "  histosieve.c\n"         \
"\n"         \
" eliminates points with values that are seldom in the histogram\n"         \
"\n"         \
"Usage: histosieve in.pgm val [out.pgm]\n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"The points which value appear strictly less than  val times in the image are eliminated.\n"         \
"\n"         \
"If  out.pgm is not specified, then out.pgm = in.pgm.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d\n"         \
"\n"         \
"Category: histo\n"         \
" histo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__shrinkondisk__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__zoomint__c__ "  zoomint.c\n"         \
"\n"         \
" zoom by an integer factor\n"         \
"\n"         \
"Usage: zoomint in.pgm  {f | fx fy fz} [nofill] out.pgm\n"         \
"\n"         \
"Description: \n"         \
"The zoom factor(s) can be unique ( f ) of there can be one zoom factor\n"         \
"per image dimension ( fx,  fy,  fz ). In the case of a 2d image, set  fz\n"         \
"to 1.\n"         \
"The zoom parameter may be a positive or negative integer.\n"         \
"If it is positive, the image is expanded in the corresponding \n"         \
"direction. The pixels are replicated in order to fill all the space,\n"         \
"except if the parameter nofill is used.\n"         \
"If it is negative, the image is shrinked the corresponding \n"         \
"directions, by undersampling.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: geo\n"         \
"  geo\n"         \
"\n"         \
" Michel Couprie, Christophe Doublier\n"         \
"\n"         \
"\n"


#define doc__lferode_line__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__samplespline__c__ "  samplespline.c\n"         \
"\n"         \
" samples a cubic spline according to its curvilinear abcissa\n"         \
"\n"         \
"Usage: samplespline in.txt nsamples out.txt\n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"This program computes a regular sampling of the spline given in in.txt.\n"         \
"The result is a list of nsamples points, which are stored in out.txt.\n"         \
"\n"         \
"Types supported: list 2D, list 3D\n"         \
"\n"         \
"Category: draw geo\n"         \
"  draw geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__lambdamedialaxis__c__ "  lambdamedialaxis.c\n"         \
"\n"         \
" discrete lambda-medial axis transform\n"         \
"\n"         \
"Usage: lambdamedialaxis in.pgm [lambda] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Discrete lambda-medial axis, as defined in [CCT09], \n"         \
"of the binary image  X contained in  in.pgm.\n"         \
"\n"         \
"By default, the output is a float image representing the function x->lambda(x).\n"         \
"\n"         \
"If the parameter  lambda is given (bash interface only, not python), the output is a binary image obtained by thresholding the aforementioned function at the value  lambda .\n"         \
"\n"         \
"References: \n"         \
"[CCT09] John Chaussard, Michel Couprie, and Hugues Talbot. A discrete lambda-medial axis. 15th Discrete Geometry for Computer Imagery (DGCI'09). Lecture Notes in Computer Science.   2009.  pp. 1â€“12. To appear.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: morpho\n"         \
"  morpho\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__lenoir__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__add__c__ "  add.c\n"         \
"\n"         \
" pixelwise addition of two images\n"         \
"\n"         \
"Usage: add in1.pgm in2.pgm out.pgm\n"         \
"\n"         \
"Description:\n"         \
"For each pixel x, out[x] = in1[x] + in2[x]. If both images are byte images,\n"         \
"and if out[x] exceeds 255, then out[x] is set to 255.\n"         \
"Images must be of the same dimensions.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n"         \
"\n"         \
"Category: arith\n"         \
"  arith\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__heightminima__c__ "  heightminima.c\n"         \
"\n"         \
" h-minima operator\n"         \
"\n"         \
"Usage: heightminima in.pgm connex height out.pgm\n"         \
"\n"         \
"Description:\n"         \
"h-minima with connexity connex and depth height.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: connect\n"         \
" connect\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__heightmaxima__c__ "  heightmaxima.c\n"         \
"\n"         \
" h-maxima operator\n"         \
"\n"         \
"Usage: heightmaxima in.pgm connex height out.pgm\n"         \
"\n"         \
"Description:\n"         \
"h-maxima with connexity connex and depth height.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: connect\n"         \
" connect\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__average__c__ "  average.c\n"         \
"\n"         \
" pixelwise (weighted) average of two images\n"         \
"\n"         \
"Usage: average in1.pgm in2.pgm [alpha] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"The default value for the optional parameter  alpha is 0.5 . \n"         \
"For each pixel x, out[x] = alpha * in1[x] + (1 - alpha) * in2[x]. \n"         \
"If both images are byte images,\n"         \
"and if out[x] exceeds 255, then out[x] is set to 255.\n"         \
"Images must be of the same dimensions.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n"         \
"\n"         \
"Category: arith\n"         \
"  arith\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__taf__c__sav__ "  taf.c\n"         \
"\n"         \
" topological alternating filter\n"         \
"\n"         \
"Usage: taf in.pgm connexmin rayon out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Topological alternating filter (cf. CB04: to appear)\n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: topo\n"         \
"  topo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__hdilatball__c__ "  hdilatball.c\n"         \
"\n"         \
" topologically controlled dilation\n"         \
"\n"         \
"Usage: hdilatball in.pgm radius dist connex out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Performs a topologically controlled dilation, that is, a homotopic thickening \n"         \
"constrained by the dilation of the input object.\n"         \
"\n"         \
"The parameter  radius gives the radius of the ball which is \n"         \
"used as structuring element for the dilation.\n"         \
"\n"         \
"The parameter  dist is a numerical code\n"         \
"indicating the distance which is used to compute the dilation.\n"         \
"The possible choices are:\n"         \
" 0: approximate euclidean distance\n"         \
" 1: approximate quadratic euclidean distance\n"         \
" 2: chamfer distance\n"         \
" 3: exact quadratic euclidean distance\n"         \
" 4: 4-distance in 2d\n"         \
" 8: 8-distance in 2d\n"         \
" 6: 6-distance in 3d\n"         \
" 18: 18-distance in 3d\n"         \
" 26: 26-distance in 3d\n"         \
"\n"         \
"The parameter  connex indicates the connectivity of the binary object.\n"         \
"Possible choices are 4, 8 in 2d and 6, 26 in 3d.\n"         \
"\n"         \
"Let X be the set corresponding to the input image  in.pgm, and let\n"         \
"Xbar denote its complementary set. The algorithm is the following:\n"         \
"\n"         \
"\n"         \
"I = dilatball(X, dist, r)\n"         \
"Repeat:\n"         \
"    Select a point x in [Xbar inter I] such that dist(x,X) is minimal\n"         \
"    If x is simple for X then\n"         \
"        X = X union {x}\n"         \
"	I = I  {x}\n"         \
"Until [Xbar inter I] = emptyset\n"         \
"Result: X\n"         \
"\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"         \
"\n"


#define doc__hello__cpp__ "WRITE ME!!!\n"         \
"\n"


#define doc__genbicol__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__lpemeyer___notused_c__ "WRITE ME!!!\n"         \
"\n"


#define doc__2dinvariants__c__ "  2dinvariants.c\n"         \
"\n"         \
" computes the numbers of connected components,\n"         \
"cavities and tunnels of a 2D Khalimsky order\n"         \
"\n"         \
"Usage: 2dinvariants in.pgm\n"         \
"\n"         \
"Description:\n"         \
"Computes the numbers of connected components\n"         \
"and holes of a 2D Khalimsky order\n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: orders\n"         \
"  orders\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__findline__cxx__ "  findline.cxx\n"         \
"\n"         \
" find line into an image using bresenham definition\n"         \
"w is the digital line size.\n"         \
"\n"         \
"Usage: findline in.fits {w}  out.pgm\n"         \
"\n"         \
"Description:\n"         \
"On commence pas netoyer l'image afin de retirer le bruit de fond et d'ameliorer le contraste.\n"         \
"\n"         \
"On determine ensuite les points sur le bord de l'image qui correspondent potentiellement Ã  une droite.\n"         \
"\n"         \
"On trace les droites de Bresenham entre les differents points des bords. Pour chacunes des droites on calcule\n"         \
"le nombre de points non nul present sur la droite. La droites contenant les plus de points non nul doit correspondre\n"         \
"Ã  la trace du satellite.\n"         \
"\n"         \
"L'image finale est une image noire, avec une droite blanche correspondant Ã  la trace du sattelite.\n"         \
"\n"         \
"\n"         \
"Parameters:\n"         \
"  in.fits : source file in fits format\n"         \
"\n"         \
"\n"         \
" Juliette Charpentier\n"         \
"\n"         \
"\n"


#define doc__taf__c__ "  taf.c\n"         \
"\n"         \
" topological alternating filter\n"         \
"\n"         \
"Usage: taf in.pgm connexmin r [lambdap [lambdaw]] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Topological alternating filter. Performs alternatively topological upper filter (tuf)\n"         \
"and topological lower filter (tlf) with increasing radius until radius r.\n"         \
"\n"         \
"If given, parameter lambdap (resp. lambdaw) is an integer which \n"         \
"indicates that peaks (resp. wells) of height greater than this value must be \n"         \
"preserved. \n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: topogray\n"         \
"  topogray\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__erosbin3d__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__identifyparabola3__c__ "  identifyparabola3.c\n"         \
"\n"         \
" identification of a best matching parabola from a set of 2D points\n"         \
"\n"         \
"Usage: identifyparabola3 in.list out.list\n"         \
"\n"         \
"Description:\n"         \
"Identifies the parameters (a,b,c) of the equation of the 2D parabola:\n"         \
"ax^2+bx+c=y that minimizes the least square error between this parabola \n"         \
"and the given points. Method: basic linear regression.\n"         \
"\n"         \
"Types supported: list 1D, list 2D\n"         \
"\n"         \
"Category: geo\n"         \
"  geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__skel_CK3__c__ "  skel_CK3.c\n"         \
"\n"         \
" parallel 3D binary curvilinear skeleton\n"         \
"\n"         \
"Usage: skel_CK3 in.pgm nsteps [inhibit] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Parallel 3D binary thinning or curvilinear skeleton. The parameter  nsteps gives,\n"         \
"if positive, the number of parallel thinning steps to be processed.\n"         \
"If the value given for  nsteps equals -1, the thinning is continued\n"         \
"until stability.\n"         \
"\n"         \
"If the parameter  inhibit is given and is a binary image name,\n"         \
"then the points of this image will be left unchanged. \n"         \
"\n"         \
"Warning: The object must not have any point on the frame of the image.\n"         \
"\n"         \
"Types supported: byte 3d\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__genbini__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__curve2segments__cxx__ "  curve2segments.cxx\n"         \
"\n"         \
" computes a covering of a digital curve by digital straigh line segments\n"         \
"\n"         \
"Usage: curve2segments curve.list [tolerance] segments.list\n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"Computes a covering of a digital curve by digital straight line segments. \n"         \
"The input file contains a curve which is specified by the ordered list of its points.\n"         \
"The output file is an ordered list of points, which are the extremities of the segments.\n"         \
"\n"         \
"Parameter  tolerance gives the thickness of the digital straight line segments. Default value is 1.\n"         \
"\n"         \
" Curves must be 8-curves in 2D of 26-curves in 3D. Greedy algorithm.\n"         \
"\n"         \
"Types supported: curve 2D, curve 3D\n"         \
"\n"         \
"Category: geo\n"         \
" geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__rotate3dbin__c__ "  rotate3dbin.c\n"         \
"\n"         \
" rotation of an image\n"         \
"\n"         \
"Usage: rotate3dbin in.pgm theta axis [c1 c2] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Method: truncated real rotation.\n"         \
"Rotates the image  in.pgm of the angle  theta (in degrees), \n"         \
"around the straight line defined by parameters  axis,  c1 and  c2, parallel to one of the main axes x, y, z.\n"         \
"Parameter  axis may be either x, y or z.\n"         \
"If  axis = x, then the rotation axis is defined by y =  c1, z =  c2.\n"         \
"If  axis = y, then the rotation axis is defined by x =  c1, z =  c2.\n"         \
"If  axis = z, then the rotation axis is defined by x =  c1, y =  c2.\n"         \
"\n"         \
"If  in.pgm is not a structuring element, and if parameters  c1 and  c2 are omitted then the default values 0, 0 are assumed and the resulting image size is computed such that no loss of information occur. Otherwise, no resize is made.\n"         \
"\n"         \
"If  in.pgm is a structuring element, then parameters  c1 and  c2 are ignored and the coordinates of the rotation axis are taken among the ones of the origin of the structuring element.\n"         \
"\n"         \
"Types supported: byte 3d\n"         \
"\n"         \
"Category: geo\n"         \
"  geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__granulometry__c__ "  granulometry.c\n"         \
"\n"         \
" granulometry of a binary image\n"         \
"\n"         \
"Usage: granulometry in.pgm [rmin] rmax out.list\n"         \
"\n"         \
"Description:\n"         \
"Let X be the set of non-null points in in.pgm.\n"         \
"This operator computes the area (volume in 3d) of the opening of X by \n"         \
"euclidean balls of increasing radius, ranging from 1 \n"         \
"(or rmin if this parameter is specified) to rmax. \n"         \
"The result out.list contains a list of couples r a where \n"         \
"r is a radius and a is the corresponding area (or volume).\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: morpho\n"         \
"  morpho\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"         \
"\n"


#define doc__skel_AMK__c__ "  skel_AMK.c\n"         \
"\n"         \
" parallel 2D or 3D binary skeleton, ultimate, asymmetric\n"         \
"\n"         \
"Usage: skel_AMK in.pgm nsteps [inhibit] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Parallel 2D or 3D binary ultimate, asymmetric skeleton. The parameter  nsteps gives,\n"         \
"if positive, the number of parallel thinning steps to be processed.\n"         \
"If the value given for  nsteps equals -1, the thinning is continued\n"         \
"until stability.\n"         \
"\n"         \
"If the parameter  inhibit is given and is a binary image name,\n"         \
"then the points of this image will be left unchanged. \n"         \
"\n"         \
"The algorithm used here is indeed a variant of alg. NK2 described in [BC08], which allows for an arbitrary (and even null) constraint set.\n"         \
"\n"         \
"Warning: The object must not have any point on the frame of the image.\n"         \
"\n"         \
"References:\n"         \
"[BC08] Gilles Bertrand and Michel Couprie, Two-dimensional thinning algorithms based on critical kernels. Journal of Mathematical Imaging and Vision, 31(1). 2008. pp.35â€“56.\n"         \
"http://igm.univ-mlv.fr/LIGM/internal_report/pdf/2006_02.v2.pdf\n"         \
"\n"         \
"[XXXX] TODO: add reference for 3D\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__seuil__c__ "  seuil.c\n"         \
"\n"         \
" simple threshold\n"         \
"\n"         \
"Usage: seuil in.pgm n [n2] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"If n2 is not present, for each pixel x, out[x] = if (in[x] \n"         \
"\n"         \
"If n2 is present, for each pixel x, out[x] = if (n \n"         \
"\n"         \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n"         \
"\n"         \
"Category: arith\n"         \
"  arith\n"         \
"\n"         \
" Michel Couprie 1997\n"         \
"\n"         \
"\n"


#define doc__skelpar__c__ "  skelpar.c\n"         \
"\n"         \
" parallel binary skeleton\n"         \
"\n"         \
"Usage: skelpar in.pgm algorithm nsteps [inhibit] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Parallel binary thinning or skeleton. The parameter  nsteps gives,\n"         \
"if positive, the number of parallel thinning steps to be processed.\n"         \
"If the value given for  nsteps equals -1, the thinning is continued\n"         \
"until stability.\n"         \
"\n"         \
"The parameter  algorithm is a numerical code\n"         \
"indicating which method will be used for the thinning.\n"         \
"The possible choices are:\n"         \
" 0: Pavlidis 1981\n"         \
" 1: Eckhardt and Maderlechner 1993\n"         \
" 2: Couprie and Bertrand, ultimate (MK2)\n"         \
" 3: Couprie and Bertrand, curvilinear\n"         \
" 4: Couprie and Bertrand, curvilinear with reconstruction property (AK2)\n"         \
" 5: Rutovitz 1966 [DOES NOT PRESERVE TOPOLOGY]\n"         \
" 6: Zhang and Wang 1988 [DOES NOT PRESERVE TOPOLOGY]\n"         \
" 7: Han, La and Rhee\n"         \
" 8: Guo and Hall 1992 (1)\n"         \
" 9: Guo and Hall 1992 (2)\n"         \
" 10: Guo and Hall 1992 (3)\n"         \
" 11: Chin and Wan and Stover and Iverson 1987\n"         \
" 12: Jang and Chin 1992 [DOES NOT PRESERVE TOPOLOGY]\n"         \
" 13: Jang and Chin (correction mc)\n"         \
" 14: MNS preservation\n"         \
" 15: Jang and Chin 1993 (reconstruction)\n"         \
" 16: Choy, Choy and Siu 1995 [DOES NOT PRESERVE TOPOLOGY]\n"         \
" 17: Bernard and Manzanera 1999\n"         \
" 18: Holt et al. 1987\n"         \
" 19: Hall 1989\n"         \
" 20: Wu and Tsai 1992 [DOES NOT PRESERVE TOPOLOGY]\n"         \
" 21: Manzanera and Bernard (variant by GB)\n"         \
" 22: Couprie and Bertrand, ultimate asymmetrical with medial axis (NK2)\n"         \
" 23: Bertrand curvilinear with reconstruction, asymmetrical\n"         \
" 24: Bertrand curvilinear with reconstruction, symmetrical\n"         \
" 25: Rosenfeld directionnel\n"         \
" 26: Nemeth et Palagyi, 2009 (1)\n"         \
" 27: Nemeth et Palagyi, 2009 (2)\n"         \
" 28: Nemeth et Palagyi, 2009 (3)\n"         \
" 29: Couprie and Bertrand, ultimate, asymmetrical (NK2)\n"         \
" 30: Couprie and Bertrand, ultimate, symmetrical (MK2b)\n"         \
" 31: Couprie and Bertrand, curvilinear, symmetrical, based on 1D isthmuses (CK2)\n"         \
"\n"         \
"If the parameter  inhibit is given and is a binary image name,\n"         \
"then the points of this image will be left unchanged.\n"         \
"\n"         \
"Warning: The object must not have any point on the frame of the image.\n"         \
"\n"         \
"References:\n"         \
"[BC08] Gilles Bertrand and Michel Couprie, Two-dimensional thinning algorithms based on critical kernels. Journal of Mathematical Imaging and Vision, 31(1). 2008. pp.35â€“56.\n"         \
"http://igm.univ-mlv.fr/LIGM/internal_report/pdf/2006_02.v2.pdf\n"         \
"\n"         \
"[COU05] M. Couprie, Note on fifteen 2D parallel thinning algorithms,\n"         \
"IGM2006-01}, Universit de Marne-la-Vall 2006,\n"         \
"http://igm.univ-mlv.fr/LabInfo/rapportsInternes/2006/01.pdf\n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__pgm2slc__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__float2byte__c__ "  float2byte.c\n"         \
"\n"         \
" converts a \"float\" image to a \"byte\" image\n"         \
"\n"         \
"Usage: float2byte in.pgm [mode] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"Depending on the value given for the (optional) parameter mode:\n"         \
"   mode = 0 (default) : for all x, out[x] = min(255, arrondi(in[x])).\n"         \
"   mode = 1 : for all x, out[x] = arrondi(in[x]) modulo 256.\n"         \
"   mode = 2 : scales values in the range 0-255.\n"         \
"   mode = 4 : truncation of the square root in the range 0-255.\n"         \
"   mode = 5 : truncation of the log in the range 0-255.\n"         \
"\n"         \
"Types supported: float 2d, float 3d\n"         \
"\n"         \
"Category: convert\n"         \
"  convert\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__skel_ACK3c__c__ "  skel_ACK3c.c\n"         \
"\n"         \
" topological persistence of 1D isthmuses, based on parallel 3D asymetric thinning\n"         \
"\n"         \
"Usage: skel_ACK3c in.pgm out.pgm\n"         \
"\n"         \
"Description: Topological persistence of 1D isthmuses, based on \n"         \
"parallel 3D asymetric thinning.\n"         \
"\n"         \
"When a point x is detected as a 1D isthmus, a counter p(x) is\n"         \
"associated to this point and initialized with value 1. This counter is\n"         \
"incremented a each iteration as long as x is still an isthmus. When this point x is\n"         \
"eventually deleted, the value of the counter is freezed.\n"         \
"\n"         \
" The object must not have any point on the frame of the image.\n"         \
"\n"         \
"Types supported: byte 3d\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__3dkhalimskize__c__ "  3dkhalimskize.c\n"         \
"\n"         \
" converts a 3D image into a 3D Khalimsky order\n"         \
"\n"         \
"Usage: 3dkhalimskize in.pgm {6|26|h|m|M|a|R|b} out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Converts a 3D image into a 3D Khalimsky order.\n"         \
"\n"         \
"Modes 6, 26 and h are for binary images. \n"         \
"\n"         \
"Modes m, M and a are for grayscale images. \n"         \
"\n"         \
"Mode = \n"         \
"    6 : emulates 6-connectivity, idem \"miss\", \n"         \
"    26 : emulates 26-connectivity (\"hit\" transform followed by a kernel transform),   \n"         \
"    h : \"hit\",\n"         \
"    m : min, \n"         \
"    M : max,\n"         \
"    a : average,\n"         \
"    R : reverse,\n"         \
"    b : border.\n"         \
"\n"         \
"Types supported: byte 3d, long 3d, float 3d\n"         \
"\n"         \
"Category: orders\n"         \
"  orders\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__seuilhistoglobal__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__drawsplinesorient__c__ "  drawsplinesorient.c\n"         \
"\n"         \
" draw spline orientations in a vector field (multiple spline version)\n"         \
"\n"         \
"Usage: drawsplinesorient in.pgm in.splines out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Draws spline orientations in a vector field. \n"         \
"The spline is specified by its control points in a text file.\n"         \
"The parameter  in.pgm gives a vector field into which the spline is to be drawn.\n"         \
"\n"         \
"The file  splines.txt contains a list of splines under the format:\n"         \
"d nb_splines\n"         \
"nb_points_spline_1  x11 y11  x12 y12 ...\n"         \
"nb_points_spline_2  x21 y21  x22 y22 ...\n"         \
"nb_points_spline_3  x31 y31  x32 y32 ...\n"         \
"...\n"         \
"or, in 3D:\n"         \
"D nb_splines\n"         \
"nb_points_spline_1  x11 y11 z11  x12 y12 z12 ...\n"         \
"nb_points_spline_2  x21 y21 z21  x22 y22 z22 ...\n"         \
"nb_points_spline_3  x31 y31 z31  x32 y32 z32 ...\n"         \
"...\n"         \
"\n"         \
"Types supported: spline 2D, spline 3D\n"         \
"\n"         \
"Category: draw geo\n"         \
"  draw geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__section__c__ "  section.c\n"         \
"\n"         \
" computes a 2d cross section from a 3d image\n"         \
"\n"         \
"Usage: section in.pgm x0 y0 z0 x1 y1 z1 x2 y2 z2 out.pgm\n"         \
"\n"         \
"Description: \n"         \
"The points P0 (  x0,  y0,  z0 ), P1 (  x1,  y1,  z1 ) and\n"         \
"P2 (  x2,  y2,  z2 ) form a basis for a plane in the 3d space. \n"         \
"The intersection of this plane with the domain of the 3d image  in.pgm\n"         \
"form the domain of the 2d image  out.pgm .\n"         \
"The grayscale values of  out.pgm are obtained by trilinear interpolation from\n"         \
"the grayscale values of  in.pgm .\n"         \
"The coordinates of the basis for the plane are expressed in \"real world\" dimensions.\n"         \
"\n"         \
"Types supported: byte 3d, int32_t 3d\n"         \
"\n"         \
"Category: geo\n"         \
"  geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__fermetrous3d__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__exp__c__ "  exp.c\n"         \
"\n"         \
" pixelwise exponentiation\n"         \
"\n"         \
"Usage: exp in.pgm out.pgm\n"         \
"\n"         \
"Description:\n"         \
"For each pixel x, out[x] = exp(in[x]).\n"         \
"\n"         \
"Types supported: float 2d, float 3d\n"         \
"\n"         \
"Category: arith\n"         \
"  arith\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__autocrop__c__ "  autocrop.c\n"         \
"\n"         \
" eliminates null regions at the periphery of an image\n"         \
"\n"         \
"Usage: autocrop in.pgm [threshold [out.pgm]]\n"         \
"\n"         \
"Description:\n"         \
"Selects the minimum rectangle (cuboid in 3d) \n"         \
"that contain all values of the image  in.pgm strictly above the  threshold (default 0).\n"         \
"\n"         \
"If  out.pgm is not specified, then out.pgm = in.pgm.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: geo\n"         \
"  geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__dilatball__c__ "  dilatball.c\n"         \
"\n"         \
" morphological binary dilation by a ball\n"         \
"\n"         \
"Usage: dilatball in.pgm r [dist] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Morphological binary dilation by a metric ball.\n"         \
"If the parameter  r is a positive integer, then\n"         \
"the structuring element for this dilation is a ball (or a disc) of radius  r.\n"         \
"In this case the result of the dilation is defined by Y = {x; d(x,X) \n"         \
"represents the original point set (non-null pixels in the original image).\n"         \
"If the parameter  r is -1, then \n"         \
"the structuring element for each point x is a ball (or a disc) of radius  F(x),\n"         \
"where F is the greyscale image  in.pgm .\n"         \
"\n"         \
"The dilation is computed by thresholding a distance map.\n"         \
"The distance used depends on the optional parameter  dist (default is 0) :\n"         \
" 0: rounded Euclidean distance\n"         \
" 1: approximate quadratic Euclidean distance\n"         \
" 2: chamfer distance\n"         \
" 3: exact quadratic Euclidean distance\n"         \
" 4: 4-distance in 2d\n"         \
" 8: 8-distance in 2d\n"         \
" 6: 6-distance in 3d\n"         \
" 18: 18-distance in 3d\n"         \
" 26: 26-distance in 3d\n"         \
"\n"         \
" Except for the case r=-1, the input image  in.pgm must be a binary image. No test is done.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: morpho\n"         \
"  morpho\n"         \
"\n"         \
" Michel Couprie 2002\n"         \
"\n"         \
"Example:\n"         \
"\n"         \
"dilatball cells 4 cells_dilatball\n"         \
"\n"         \
"\n"         \
"  \n"         \
"      \n"         \
"  \n"         \
"  cells\n"         \
"      cells_dilatball\n"         \
"  \n"         \
"\n"         \
"\n"         \
"\n"         \
"\n"


#define doc__ac2pink__c__ "  ac2pink.c\n"         \
"\n"         \
" converts a 3D scene from AC3D format to Pink 3Dscene format\n"         \
"\n"         \
"Usage: ac2pink in.ac out.3sc\n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"Reads the file in.ac. This file must be in the AC3D format.\n"         \
"Generates the output file out.3sc in the 3Dscene format : \n"         \
"\n"         \
"FILE ::= '3Dscene' &lt;N&gt; &lt;OBJ&gt;*\n"         \
"OBJ ::= &lt;LINE&gt; | &lt;CLOSEDLINE&gt; | &lt;SPLINE&gt; | &lt;CLOSEDSPLINE&gt; | &lt;POLYGON&gt;\n"         \
"LINE ::= 'line' &lt;N&gt; &lt;POINT&gt;*\n"         \
"CLOSEDLINE ::= 'closedline' &lt;N&gt; &lt;POINT&gt;*\n"         \
"SPLINE ::= 'spline' &lt;N&gt; &lt;POINT&gt;*\n"         \
"CLOSEDSPLINE ::= 'closedspline' &lt;N&gt; &lt;POINT&gt;*\n"         \
"POLYGON ::= 'polygon' &lt;N&gt; &lt;POINT&gt;*\n"         \
"N ::= ascii_coded_integer\n"         \
"POINT ::= ascii_coded_float ascii_coded_float ascii_coded_float\n"         \
"\n"         \
"Types supported: AC3D\n"         \
"\n"         \
"Category: convert\n"         \
"  convert\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__opening__c__ "  opening.c\n"         \
"\n"         \
" morphological opening by a plane structuring element\n"         \
"\n"         \
"Usage: opening in.pgm se.pgm out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Morphological opening by a plane structuring element.\n"         \
"The (plane) structuring element is given by the non-null values in  se.pgm, \n"         \
"its origin (wrt the point (0,0) of  se.pgm ) \n"         \
"is given by a comment line in the file se.pgm. \n"         \
"\n"         \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n"         \
"\n"         \
"Category: morpho\n"         \
"  morpho\n"         \
"\n"         \
" Michel Couprie 2002\n"         \
"\n"         \
"\n"         \
"\n"


#define doc__distgeo__c__ "  distgeo.c\n"         \
"\n"         \
" geodesic distance\n"         \
"\n"         \
"Usage: distgeo in.pgm mask.pgm mode out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Geodesic distance to the object X defined by the binary image  in.pgm ,\n"         \
"in the object Y defined by the binary image  mask.pgm .\n"         \
"\n"         \
"The distance d used depends on the parameter  mode :\n"         \
" 4: 4-distance in 2d\n"         \
" 8: 8-distance in 2d\n"         \
" 6: 6-distance in 3d\n"         \
" 18: 18-distance in 3d\n"         \
" 26: 26-distance in 3d\n"         \
"\n"         \
"Types supported: byte 2D, byte 3D\n"         \
"\n"         \
"Category: morpho\n"         \
"   morpho\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__meshregul__c__sav__ "  meshregul.c\n"         \
"\n"         \
" mesh smoothing\n"         \
"\n"         \
"Usage: meshregul in mode [param1 [param2]] out\n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"Mesh smoothing. \n"         \
"The possible choices for parameter mode are:\n"         \
" 0: Laplacian smoothing (param1 = number of iterations, default value 5)\n"         \
" 1: Hamam method [HC06] (param1 = theta, default value 1.0)\n"         \
" 2: Vollmer, Mencl and Mueller method [VMM] (param1 = alpha, param2 = beta, default values 0.1 and 0.6 resp.)\n"         \
" 3: Hamam method [HC06], variant: matrix A used instead of AA (param1 = theta, default value 1.0)\n"         \
" 4: Hamam method [HC06], variant: theta = infty (param1 = number of iterations, default value 5)\n"         \
" 5: Hamam method [HC06], conjugate gradient algorithm (param1 = theta, default value 1.0)\n"         \
"\n"         \
"[HC06] Y. Hamam \"\", to appear, 2006\n"         \
"\n"         \
"Types supported: mesh 3d\n"         \
"\n"         \
"Category: mesh3d\n"         \
"  mesh3d\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__skel_MK3__c__ "  skel_MK3.c\n"         \
"\n"         \
" parallel 3D binary ultimate skeleton\n"         \
"\n"         \
"Usage: skel_MK3 in.pgm nsteps [inhibit] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Parallel 3D binary thinning or ultimate skeleton. The parameter  nsteps gives,\n"         \
"if positive, the number of parallel thinning steps to be processed.\n"         \
"If the value given for  nsteps equals -1, the thinning is continued\n"         \
"until stability.\n"         \
"\n"         \
"If the parameter  inhibit is given and is a binary image name,\n"         \
"then the points of this image will be left unchanged. \n"         \
"\n"         \
"Warning: The object must not have any point on the frame of the image.\n"         \
"\n"         \
"Types supported: byte 3d\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__saliency__c__ "  saliency.c\n"         \
"\n"         \
" salient watershed transformation\n"         \
"\n"         \
"Usage: saliency in connex out\n"         \
"\n"         \
"Description:\n"         \
"Performs the salient watershed transformation on the image in, \n"         \
"The parameter connex gives the adjacency relation (4 or 6b in 2D).\n"         \
"\n"         \
"The image out is a int32_t image (int32_t)\n"         \
"\n"         \
"In 4 connectivity, the output image is a line graph in the khalimsky\n"         \
"grid. In 6b connectivity, the result is on the pixels themselves, with\n"         \
"the gammab neighborhood.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: connect\n"         \
" connect\n"         \
"\n"         \
" Laurent Najman\n"         \
"\n"         \
"\n"


#define doc__lfopen_line__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__seuil2__c__ "  seuil2.c\n"         \
"\n"         \
" double threshold\n"         \
"\n"         \
"Usage: seuil2 in.pgm smin smax vmin vmax out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Every pixel   \n"         \
"Every pixel >=  smax gets value  vmax \n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: arith\n"         \
"  arith\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__3dmakepolygons__c__ "  3dmakepolygons.c\n"         \
"\n"         \
" identifies polygons from a labelled pure 2D cellular complex\n"         \
"\n"         \
"Usage: 3dmakepolygons lab.pgm border.pgm psubdiv pmerge out.vtk\n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"The input  lab.pgm contains a labelled pure 2D complex, where each label \n"         \
"marks a set of 2-facets which is a manifold with boundary\n"         \
"\n"         \
"psubdiv: this parameter governs the subdivision of polygon edges \n"         \
"pmerge: TODO\n"         \
"\n"         \
"Types supported: byte 3d\n"         \
"\n"         \
"Category: orders\n"         \
"  orders\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__mcube_tmp__c__ "  mcube.c\n"         \
"\n"         \
" topologically correct \"marching cubes\"-like algorithm\n"         \
"\n"         \
"Usage: mcube in.pgm threshold nregul obj_id format [connex] out\n"         \
"\n"         \
"Description:\n"         \
"Generates a 3d mesh from the binary or grayscale image  in.pgm .\n"         \
"\n"         \
"The original image is first thresholded (parameter  threshold, values 0 and 1\n"         \
"both fit for a binary image). Then the method described in [Lac96] is applied to\n"         \
"generate the 3d mesh. The resulting mesh is then smoothed by  nregul steps of \n"         \
"laplacian smoothing. The parameter  obj_id is used to tag the generated mesh.\n"         \
"\n"         \
"The parameter  format indicate the format of the output file \n"         \
"(choices are POV, POVB, COL, MCM, AC, GL, VTK, RAW).\n"         \
"The keyword POVB corresponds to a bare Povray mesh: \n"         \
"a header and a footer must be catenated in order to make a full Povray scene. \n"         \
"The keyword RAW is the exchange format for the \"mesh\" software: see http://mesh.berlios.de/\n"         \
"\n"         \
"The optional parameter  connex indicates the connexity used for the object.\n"         \
"Possible values are 6 and 26 (default).\n"         \
"\n"         \
"[Lac96] J.-O. Lachaud, \"Topologically defined iso-surfaces\", DGCI'96, LNCS 1176 (245--256), Springer Verlag, 1996.\n"         \
"\n"         \
"Types supported: byte 3d\n"         \
"\n"         \
"Category: mesh3d\n"         \
"  mesh3d\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__3dplane__c__ "  3dplane.c\n"         \
"\n"         \
" Builds the discretization of a plane in the 3d Khalimsky space.\n"         \
"\n"         \
"Usage: 3dplane in.pgm a b c d out.pgm\n"         \
"\n"         \
"Description:\n"         \
"  Draws into the Khalimsky volume  k, the discretization of the plane\n"         \
"  defined by the equation:  a x +  b y +  c z +  d = 0\n"         \
"\n"         \
"Types supported: byte 3d\n"         \
"\n"         \
"Category: orders\n"         \
"  orders\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__pgmrawmode__c__ "  pgmrawmode.c\n"         \
"\n"         \
" converts a pgm file to raw mode\n"         \
"\n"         \
"Usage: pgmrawmode filename [outfilename]\n"         \
"\n"         \
"Description: Converts a pgm file to raw mode.\n"         \
"If only parameter  filename is given, then the result\n"         \
"is also named  filename .\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: convert\n"         \
"  convert\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__bmp2pgm__c__ "  bmp2pgm.c\n"         \
"\n"         \
" \n"         \
"\n"         \
"Usage: \n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"Types supported: byte 2D\n"         \
"\n"         \
"Category: convert\n"         \
"  convert\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__distc__c__ "  distc.c\n"         \
"\n"         \
" distance transform (internal)\n"         \
"\n"         \
"Usage: distc in.pgm mode out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Distance to the complementary of the object X defined by the binary \n"         \
"image  in.pgm .\n"         \
"The result function DX(x) is defined by: DX(x) = min {d(x,y), y not in X}.\n"         \
"\n"         \
"The distance d used depends on the parameter  mode :\n"         \
" 0: euclidean distance (rounded to the nearest int32)\n"         \
" 1: approximate quadratic euclidean distance (Danielsson)\n"         \
" 2: chamfer distance ([5,7] in 2D; [4,5,6] in 3D)\n"         \
" 3: exact quadratic euclidean distance (int32)\n"         \
" 4: 4-distance in 2d\n"         \
" 5: exact euclidean distance (float)\n"         \
" 8: 8-distance in 2d\n"         \
" 6: 6-distance in 3d\n"         \
" 18: 18-distance in 3d\n"         \
" 26: 26-distance in 3d\n"         \
" 40: 4-distance in 2d (byte coded ouput)\n"         \
" 80: 8-distance in 2d (byte coded ouput)\n"         \
" 60: 6-distance in 3d (byte coded ouput)\n"         \
" 180: 18-distance in 3d (byte coded ouput)\n"         \
" 260: 26-distance in 3d (byte coded ouput)\n"         \
"\n"         \
"The output  out.pgm is of type int32_t for modes \n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: morpho\n"         \
"  morpho\n"         \
"\n"         \
" Michel Couprie, Xavier Daragon\n"         \
"\n"         \
"\n"


#define doc__hthin__c__ "  hthin.c\n"         \
"\n"         \
" grayscale homotopic thinning\n"         \
"\n"         \
"Usage: hthin in.pgm {imcond.pgm|null} connex niter out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Grayscale homotopic thinning (refs. [BEC97, CBB01]).\n"         \
"The parameter  connex gives the connectivity used for the minima;\n"         \
"possible choices are 4, 8 in 2D and 6, 26 in 3D.\n"         \
"Let F be the function corresponding to the input image  in.pgm.\n"         \
"Let G be the function corresponding to the input image \n"         \
" imcond.pgm, or the null function if the keyword  null is used.\n"         \
"The algorithm is the following:\n"         \
"\n"         \
"\n"         \
"Repeat niter times:\n"         \
"    L = {(p,d) such that p is destructible for F and d = delta-(p,F)}\n"         \
"    While L not empty\n"         \
"       extract a couple (p,d) from L\n"         \
"       F(p) = max{ d, delta-(p,F), G(p) }    \n"         \
"Result: F\n"         \
"\n"         \
"\n"         \
"If  niter is set to -1, then the operator iterates until stability.\n"         \
"\n"         \
"References:\n"         \
"\n"         \
"[BEC97] G. Bertrand, J. C. Everat and M. Couprie: \"Image segmentation through operators based upon topology\",  Journal of Electronic Imaging, Vol.&nbsp;6, No.&nbsp;4, pp.&nbsp;395-405, 1997.\n"         \
"\n"         \
"[CBB01] M. Couprie, F.N. Bezerra, Gilles Bertrand: \"Topological operators for\n"         \
"grayscale image processing\",  Journal of Electronic Imaging, Vol.&nbsp;10, No.&nbsp;4, pp.&nbsp;1003-1015, 2001.\n"         \
"\n"         \
"Types supported: byte 2D, byte 3D.\n"         \
"\n"         \
"Category: topogray\n"         \
"  topogray\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"Example:\n"         \
"\n"         \
"hthin ur1 null 4 10 ur1_hthin\n"         \
"minima ur1_hthin 4 ur1_hthin_m\n"         \
"\n"         \
"\n"         \
"  \n"         \
"      \n"         \
"      \n"         \
"  \n"         \
"  ur1\n"         \
"      ur1_hthin\n"         \
"      ur1_hthin_m\n"         \
"  \n"         \
"\n"         \
"\n"         \
"\n"


#define doc__crestsegment__c__ "  crestsegment.c\n"         \
"\n"         \
" measures the height of crests in a thinned grayscale image\n"         \
"\n"         \
"Usage: crestsegment skel.pgm connex niter base incr out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Let F be the input image (parameter skel.pgm), and\n"         \
"let R be the output image (parameter out.pgm).\n"         \
"\n"         \
"TO BE WRITTEN\n"         \
"\n"         \
"Reference: \n"         \
"\"Topological operators for the detection of\n"         \
"curvilinar structures in grayscale images\", in preparation, 2003.\n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: topogray\n"         \
"  topogray\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__segment3di__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__histoazimuth__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__temp___notused_c__ "WRITE ME!!!\n"         \
"\n"


#define doc__isometry__c__ "  isometry.c\n"         \
"\n"         \
" basic isometries in 3D \n"         \
"\n"         \
"Usage: isometry in MODE out\n"         \
"\n"         \
"Description:\n"         \
"Basic isometries in 2D and 3D.\n"         \
"Available modes: xzy, yxz, yzx, zxy, zyx, (3D), and\n"         \
"0: identity,\n"         \
"1: rotation 90 degrees clockwise,\n"         \
"2: rotation 180 degrees clockwise,\n"         \
"3: rotation 270 degrees clockwise,\n"         \
"4: symmetry / vertical axis,\n"         \
"5: symmetry / horizontal axis.\n"         \
"\n"         \
"Types supported: byte 2D, byte 3D\n"         \
"\n"         \
"Category: geo\n"         \
"  geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__ball___notused_c__ "WRITE ME!!!\n"         \
"\n"


#define doc__pgm2vtk__c__ "  pgm2vtk.c\n"         \
"\n"         \
" generates a vtk file from a 3D image\n"         \
"\n"         \
"Usage: pgm2vtk in.pgm mode out.vtk\n"         \
"\n"         \
"Description:\n"         \
"Create a vtk image from a 3D pgm image.\n"         \
"If mode = \n"         \
"     1: surfels (2-faces) from a binary khalimsky grid\n"         \
"     2: triangulated surfels (2-faces) from a binary khalimsky grid\n"         \
"     3: voxels (cubes) from a binary image in Z3\n"         \
"     4: minimal grid from a binary image in Z3\n"         \
"     5: voxels (octaedrons) from a binary image in Z3\n"         \
"     6: linels (1-faces) from a binary khalimsky grid\n"         \
"\n"         \
"Types supported: byte 3d\n"         \
"\n"         \
"Category: convert\n"         \
"  convert\n"         \
"\n"         \
" Michel Couprie (2005)\n"         \
"\n"         \
"\n"


#define doc__dir__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__sup__c__ "  sup.c\n"         \
"\n"         \
" pixelwise sup predicate\n"         \
"\n"         \
"Usage: sup in1.pgm in2.pgm out.pgm\n"         \
"\n"         \
"Description:\n"         \
"For each pixel x, if in1[x] >= in2[x] then out[x] = 255 else out[x] = 0.\n"         \
"Images must be of the same type and same dimensions.\n"         \
"\n"         \
"Types supported: byte 2D, byte 3D\n"         \
"\n"         \
"Category: arith\n"         \
"  arith\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__lthick__c__ "  lthick.c\n"         \
"\n"         \
" grayscale leveling thickening\n"         \
"\n"         \
"Usage: lthick in.pgm {imcond.pgm|null} connex niter out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Grayscale leveling thickening (refs. [BEC97, CBB01]).\n"         \
"The parameter  connex gives the connectivity used for the minima;\n"         \
"possible choices are 4, 8 in 2D and 6, 26 in 3D.\n"         \
"Let F be the function corresponding to the input image  in.pgm.\n"         \
"Let G be the function corresponding to the input image \n"         \
" imcond.pgm, or the blanck function (constant 255) if the keyword  null is used.\n"         \
"The algorithm is the following:\n"         \
"\n"         \
"\n"         \
"Repeat niter times:\n"         \
"    L = {(p,a) such that T++(p,F) = 1 and a = alpha+(p,F)}\n"         \
"    While L not empty\n"         \
"       extract a couple (p,a) from L\n"         \
"       F(p) = min{ a, alpha+(p,F), G(p) }    \n"         \
"Result: F\n"         \
"\n"         \
"\n"         \
"If  niter is set to -1, then the operator iterates until stability.\n"         \
"\n"         \
"References: \n"         \
"[BEC97] G. Bertrand, J. C. Everat and M. Couprie: \"Image segmentation through operators based upon topology\",  Journal of Electronic Imaging, Vol.&nbsp;6, No.&nbsp;4, pp.&nbsp;395-405, 1997.\n"         \
"[CBB01] M. Couprie, F.N. Bezerra, Gilles Bertrand: \"Topological operators for\n"         \
"grayscale image processing\",  Journal of Electronic Imaging, Vol.&nbsp;10, No.&nbsp;4, pp.&nbsp;1003-1015, 2001.\n"         \
"\n"         \
"Types supported: byte 2D, byte 3D.\n"         \
"\n"         \
"Category: topogray\n"         \
"  topogray\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__recalagerigide__c__ "  recalagerigide.c\n"         \
"\n"         \
" rigid registration of two closed contours\n"         \
"\n"         \
"Usage: recalagerigide in1 in2 out\n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"Rigid registration of two closed contours. \n"         \
"Let X and Y be two point sets, given respectively by  in1 and  in2.\n"         \
"This procedure identifies the parameters of a rigid deformation R such that\n"         \
"the \"distance\" between R(X) and Y is a local minimum.\n"         \
"The distance D(Z,Y) between two sets Z and Y is defined by:\n"         \
"\n"         \
"D(Z,Y) = sum { d2(z,Y) ; for all z in Z }\n"         \
"\n"         \
"d2(z,Y) = min { d2(z,y) ; for all y in Y }\n"         \
"\n"         \
"d2(z,y) = (z - y)^2 ; i.e., the square or the Euclidean distance between z and y.\n"         \
"\n"         \
"The rigid deformation R is defined as the composition (in this order) \n"         \
"of scalings, rotations and translations. \n"         \
"\n"         \
"In 2d, the parameters are:\n"         \
" sx : parameter for the scaling in direction x\n"         \
" sy : parameter for the scaling in direction y\n"         \
" theta : angle (in radians) of the rotation around the barycenter of X\n"         \
" tx : parameter for the translation in direction x\n"         \
" ty : parameter for the translation in direction y\n"         \
"\n"         \
"In 3d, the parameters are:\n"         \
" sx : parameter for the scaling in direction x\n"         \
" sy : parameter for the scaling in direction y\n"         \
" sz : parameter for the scaling in direction z\n"         \
" theta : angle (in radians) of the rotation around the parallel to the the z axis passing by the barycenter of X\n"         \
" phi : angle (in radians) of the rotation around the parallel to the the y axis passing by the barycenter of X\n"         \
" tx : parameter for the translation in direction x\n"         \
" ty : parameter for the translation in direction y\n"         \
" tz : parameter for the translation in direction z\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: geo\n"         \
"  geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__gradinf__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__catgif__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__temp2d___notused_c__ "WRITE ME!!!\n"         \
"\n"


#define doc__dilatfast3d___notused_c__ "WRITE ME!!!\n"         \
"\n"


#define doc__quasishear__c__ "  quasishear.c\n"         \
"\n"         \
" quasi shear rotation\n"         \
"\n"         \
"Usage: quasishear in.pgm theta [x y] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"rotates the image of the angle  theta (in degrees).\n"         \
"If a point (x,y) is given, then the result is croped to the original\n"         \
"image size, otherwise a new image with sufficient size is created.\n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: geo\n"         \
"  geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__log__c__ "  log.c\n"         \
"\n"         \
" pixelwise logarithm\n"         \
"\n"         \
"Usage: log in.pgm out.pgm\n"         \
"\n"         \
"Description:\n"         \
"For each pixel x, out[x] = log(in[x]).\n"         \
"\n"         \
"Types supported: float 2d, float 3d\n"         \
"\n"         \
"Category: arith\n"         \
"  arith\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__skel_CK3_pers__c__ "  skel_CK3_pers.c\n"         \
"\n"         \
" topological persistence of 1D isthmuses, based on parallel 3D asymetric thinning\n"         \
"\n"         \
"Usage: skel_CK3_pers in.pgm out.pgm\n"         \
"\n"         \
"Description: Topological persistence of 1D isthmuses, based on \n"         \
"parallel 3D asymetric thinning.\n"         \
"\n"         \
"When a point x is detected as a 1D isthmus, a counter p(x) is\n"         \
"associated to this point and initialized with value 1. This counter is\n"         \
"incremented a each iteration as long as x is still an isthmus. When this point x is\n"         \
"eventually deleted, the value of the counter is freezed.\n"         \
"\n"         \
" The object must not have any point on the frame of the image.\n"         \
"\n"         \
"Types supported: byte 3d\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__randpoints__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__histolisse__c__ "  histolisse.c\n"         \
"\n"         \
" smoothed histogram of the gray values\n"         \
"\n"         \
"Usage: histolisse in.pgm L [mask.pgm] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Generates a binary image which contains the graphical representation\n"         \
"of the smoothed histogram of  in.pgm (masked by the binary image\n"         \
" mask.pgm, if given).  L : smoothing coefficient, 0\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: histo\n"         \
"  histo\n"         \
"\n"         \
" Cedric Allene 2003\n"         \
"\n"         \
"\n"


#define doc__point__c__ "  point.c\n"         \
"\n"         \
" modifies the value of a given point \n"         \
"\n"         \
"Usage: point in.pgm x y z v out.pgm\n"         \
"\n"         \
"Description:\n"         \
"The value of the point (x,y,z)\n"         \
"is changed to v.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d\n"         \
"\n"         \
"Category: geo\n"         \
"  geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__fits2pgm__c__ "  fits2pgm.c\n"         \
"\n"         \
" converts from fits format into pgm format\n"         \
"\n"         \
"Usage: fits2pgm in.fits out.pgm\n"         \
"\n"         \
"Description: Converts from fits format into pgm format.\n"         \
"\n"         \
"Parameters:\n"         \
"  in.fits : source file in fits format\n"         \
"\n"         \
"Category: convert\n"         \
" convert\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__drawfieldlists__c__ "  drawfieldlists.c\n"         \
"\n"         \
" draw line segments that represent a vector field (represented by two lists)\n"         \
"\n"         \
"Usage: drawfieldlists in1.list in2.list in.pgm [len] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"The files  in1.pgm,  in2.pgm represent a vector field, with the origins of vectors in  in1.pgm and their directions in  in2.pgm. This operator draws in the image  in.pgm a straight line segment of length  len (default 10) for each vector.\n"         \
"\n"         \
"Types supported: byte 2D, byte 3D\n"         \
"\n"         \
"Category: draw\n"         \
"  draw\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__rgb2ppm__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__pgm2bmp__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__shencastan__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__mult__c__ "  mult.c\n"         \
"\n"         \
" pixelwise multiplication of two images\n"         \
"\n"         \
"Usage: mult in1.pgm in2.pgm out.pgm\n"         \
"\n"         \
"Description:\n"         \
"For each pixel x, out[x] = in1[x] * in2[x]. If both images are byte images,\n"         \
"and if out[x] exceeds 255, then out[x] is set to 255.\n"         \
"Images must be of the same type and same dimensions.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n"         \
"\n"         \
"Category: arith\n"         \
"  arith\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__distsets__c__ "  distsets.c\n"         \
"\n"         \
" distance between sets\n"         \
"\n"         \
"Usage: distsets in1.pgm in1.pgm mode [cut] out.list\n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"Computes the distance between the object X defined by the binary image\n"         \
" in1.pgm and the object Y defined by the binary image  in2.pgm .\n"         \
"\n"         \
"Stores the result (a number) in the file  out.list .\n"         \
"\n"         \
"The used pointwise distance is the exact Euclidean distance (float).\n"         \
"\n"         \
"The definition of the set distance used depends on the parameter  mode :\n"         \
" 0: Hausdorff\n"         \
" 1: Baddeley, order 1\n"         \
" 2: Baddeley, order 2\n"         \
" 3: Dubuisson-Jain\n"         \
"\n"         \
"The optional parameter  cut is required only for Baddeley distances. \n"         \
"\n"         \
" The input images  in1.pgm and  in2.pgm must be binary images. No test is done.\n"         \
"\n"         \
"Types supported: byte 2d,  byte 3d\n"         \
"\n"         \
"Category: morpho\n"         \
"  morpho\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__histo2__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__minima__c__ "  minima.c\n"         \
"\n"         \
" regional minima \n"         \
"\n"         \
"Usage: in.pgm connex out.pgm\n"         \
"\n"         \
"Description: \n"         \
"Selects the regional minima of a grayscale image with connexity connex.\n"         \
"\n"         \
"Types supported: byte 2d, int32_t 2d, byte 3d, int32_t 3d\n"         \
"\n"         \
"Category: connect, topogray\n"         \
"  connect, topogray\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__pca__c__ "  pca.c\n"         \
"\n"         \
" principal component analysis\n"         \
"\n"         \
"Usage: pca in.list out.list\n"         \
"\n"         \
"Description:\n"         \
"Computes the main directions (2D or 3D vectors) of a set of points in 2D or 3D space, by the method of Pricipal Component Analysis (PCA).\n"         \
"\n"         \
"The output is a file in the list \"b\" or \"B\" format that contains the following information:\n"         \
" center of mass (point)\n"         \
" first principal direction (vector) \n"         \
" second principal direction (vector) \n"         \
" for 3D images, third principal direction (vector) \n"         \
"\n"         \
"Types supported: list 2D, list 3D\n"         \
"\n"         \
"Category: stats\n"         \
" stats\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__showheader__c__ "  showheader.c\n"         \
"\n"         \
" prints image information\n"         \
"\n"         \
"Usage: showheader in.pgm\n"         \
"\n"         \
"Description:\n"         \
"Prints all informations contained in the image file header: type, header, size and comments.\n"         \
"\n"         \
"Types supported: all pgm and ppm types\n"         \
"\n"         \
"Category: convert\n"         \
" convert\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__lfopen_rect__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__skel_ACK3a__c__ "  skel_ACK3a.c\n"         \
"\n"         \
" parallel 3D binary curvilinear, asymetric skeleton based on thin 1D isthmus\n"         \
"\n"         \
"Usage: skel_ACK3a in.pgm nsteps [ndel [inhibit]] out.pgm\n"         \
"\n"         \
"Description: Parallel 3D binary thinning or curvilinear,\n"         \
"asymetric skeleton based on thin 1D isthmus. The parameter  nsteps\n"         \
"gives, if positive, the number of parallel thinning steps to be\n"         \
"processed.  If the value given for  nsteps equals -1, the thinning\n"         \
"is continued until stability.\n"         \
"\n"         \
"During the first  ndel steps (default value 0), detected curve points are marked: they are indicated by a value 127 (instead of 255) in the output image.\n"         \
"\n"         \
"If the parameter  inhibit is given and is a binary image name,\n"         \
"then the points of this image will be left unchanged. \n"         \
"\n"         \
" When  ndel is not set to 0, the output image may contain points with value 127.\n"         \
"\n"         \
" The object must not have any point on the frame of the image.\n"         \
"\n"         \
"Types supported: byte 3d\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__squelval__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__asf__c__ "  asf.c\n"         \
"\n"         \
" alternate sequential filter\n"         \
"\n"         \
"Usage: asf in.pgm [rmin] rmax out.pgm\n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"Alternate sequential filter for grayscale images (for binary images\n"         \
"use preferably asfbin).\n"         \
"\n"         \
"Disc-shaped structuring elements of increasing radius, ranging from 1\n"         \
"(or rmin if this parameter is specified) to rmax, are\n"         \
"generated.\n"         \
"\n"         \
"Let D1...Dn be these structuring elements, sorted by increasing radius.\n"         \
"\n"         \
"Let I0 = in.pgm, the ith intermediate result Ii is obtained by\n"         \
"the closing of the opening of Ii-1 by the structuring element Di.\n"         \
"\n"         \
"The result out.pgm contains the final result In.\n"         \
"\n"         \
"Giving a value 0 for the optional parameter rmin has the effect\n"         \
"of beginning by a closing instead of an opening.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: morpho\n"         \
"  morpho\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"         \
"\n"


#define doc__zoom__c__ "  zoom.c\n"         \
"\n"         \
" zoom (shrink or expand) an image\n"         \
"\n"         \
"Usage: zoom in.pgm {f | x rs | y cs | z ds | fx fy fz} out.pgm\n"         \
"\n"         \
"Description: \n"         \
"There are 3 modes, depending on the number of arguments.\n"         \
"\n"         \
"1 argument: the same scale factor f is applied to both dimensions \n"         \
"  x and y (and z in 3D)\n"         \
"\n"         \
"2 arguments: if the parameter x is used, \n"         \
"  followed by an integer number rs, the zoom factor f \n"         \
"  is computed by dividing rs by the rowsize of in.pgm.\n"         \
"  If the parameter y is used, followed by an integer number cs, \n"         \
"  f is computed by dividing cs by the colsize of in.pgm.\n"         \
"  If the parameter z is used, followed by an integer number ds, \n"         \
"  f is computed by dividing ds by the depth of in.pgm.\n"         \
"\n"         \
"3 arguments: different zoom factors fx, fy, fz\n"         \
"  are given for directions x, y, z.\n"         \
"\n"         \
"Types supported: byte 1d, byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n"         \
"\n"         \
"Category: geo\n"         \
"  geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__maxima__c__ "  maxima.c\n"         \
"\n"         \
" regional maxima \n"         \
"\n"         \
"Usage: in.pgm connex out.pgm\n"         \
"\n"         \
"Description: \n"         \
"Selects the regional maxima of a grayscale image with connexity connex.\n"         \
"\n"         \
"Types supported: byte 2d, int32_t 2d, byte 3d, int32_t 3d\n"         \
"\n"         \
"Category: connect, topogray\n"         \
"  connect, topogray\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__lpetoporeg__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__deletewells__c__ "  deletewells.c\n"         \
"\n"         \
" deletes wells in a grayscale image \n"         \
"\n"         \
"Usage: deletewells filein.pgm [mask] connexmin fileout.pgm\n"         \
"\n"         \
"Description:\n"         \
"Deletes wells in a grayscale image. A point p is a well if T-(p) = 0\n"         \
"(see ref. [BEC97]).\n"         \
"\n"         \
"References: \n"         \
"[BEC97] G. Bertrand, J. C. Everat and M. Couprie: \"Image segmentation through operators based upon topology\",  Journal of Electronic Imaging, Vol.&nbsp;6, No.&nbsp;4, pp.&nbsp;395-405, 1997.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: topogray\n"         \
"  topogray\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__files_to_copy__c__ "  files_to_copy Draft\n"         \
"\n"         \
" html pink_euroscipy_web.jpeg \"EuroSciPy 2011 poster\"\n"         \
" html pink_euroscipy_web.pdf \"EuroSciPy 2011 poster\"\n"         \
"\n"         \
"\n"         \
"\n"


#define doc__pol2car__c__ "  pol2car.c\n"         \
"\n"         \
" converts a 2d image from polar to cartesian coordinates\n"         \
"\n"         \
"Usage: in_pol in_car x_center y_center out\n"         \
"\n"         \
"Description:\n"         \
"Converts a 2d image from polar to cartesian coordinates.\n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: convert\n"         \
"  convert\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__gradient_abs__cpp__ "\n"         \
"    gradient_abs.cpp\n"         \
"\n"         \
"    Computes the absolute value of the gradient of the spline\n"         \
"   approximation of the image.\n"         \
"   \n"         \
"   Usage: gradient_abs( (float_image)image) -> float_image\n"         \
"   \n"         \
"   Description: The an image can be considered as a continuous\n"         \
"   field. This function approximates the image in all directions (x,\n"         \
"   y, z), with a 1D spline. Than it calculates the absolute value of\n"         \
"   the gradient for every point in form\n"         \
"\n"         \
"   g[[x,y,z]] = fabs([ pd_x g, pd_y g, pd_z g ])\n"         \
"\n"         \
"   here 'pd_x' stands for the partial derivative in the 'x' direction.\n"         \
"\n"         \
"   Types supported: float 2d, float 3d\n"         \
"   \n"         \
"   Category: signal, development\n"         \
"    signal, development\n"         \
"\n"         \
"   References:\n"         \
"   [1]  GNU Scientific Library \n"         \
"    Laszlo Marak, 2009-2010\n"         \
"\n"         \
"\n"         \
" \n"         \
"\n"


#define doc__lengthsplines__c__ "  lengthsplines.c\n"         \
"\n"         \
" computes the lengths of splines which are specified by their control points in a text file\n"         \
"\n"         \
"Usage: lengthsplines splines.txt out.list\n"         \
"\n"         \
"Description:\n"         \
"Computes the lengths of splines which are specified by their control points in a text file.\n"         \
"\n"         \
"The file format for  splines.txt is the following for 2D:\n"         \
"\n"         \
"The file  splines.txt contains a list of splines under the format:\n"         \
"d nb_splines\n"         \
"nb_points_spline_1  x11 y11  x12 y12 ...\n"         \
"nb_points_spline_2  x21 y21  x22 y22 ...\n"         \
"nb_points_spline_3  x31 y31  x32 y32 ...\n"         \
"...\n"         \
"or, in 3D:\n"         \
"D nb_splines\n"         \
"nb_points_spline_1  x11 y11 z11  x12 y12 z12 ...\n"         \
"nb_points_spline_2  x21 y21 z21  x22 y22 z22 ...\n"         \
"nb_points_spline_3  x31 y31 z31  x32 y32 z32 ...\n"         \
"...\n"         \
"\n"         \
"Types supported: spline 2D, spline 3D\n"         \
"\n"         \
"Category: geo\n"         \
" geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__3dflowskeleton__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__GA2pgm__c__ "  GA2pgm.c\n"         \
"\n"         \
" Convert a 4-connected edge-weighted graph (a GA) into a pgm image \n"         \
"\n"         \
"Usage: GA2pgm graph.ga param out.pgm \n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"Convert a 4-connected edge-weighted graph (graph.ga) into a pgm\n"         \
"image (out.pgm) where each pixel represents a vertex of the\n"         \
"input graph and the gray level of a pixel is obtained from the values\n"         \
"of its incident edges by means of an operation depending of the\n"         \
"parameter param.\n"         \
"\n"         \
"If param = 1, then max is used, and if param = 2, min is used.\n"         \
"\n"         \
"Types supported: ga 2d, ga 3d\n"         \
"\n"         \
"Category: GA\n"         \
" GA\n"         \
"\n"         \
" Jean Cousty\n"         \
"\n"         \
"\n"


#define doc__pgm2GA4d__c__ "  pgm2GA4d.c\n"         \
"\n"         \
" Create a 4D edge-weighted graph from a series of 3D images\n"         \
"\n"         \
"\n"         \
"Usage: pgm2GA4d prefix_in first last mode GA4d_out.ga\n"         \
"\n"         \
"Description: Create a 4D GA (edge-weighted graph) from the 4D\n"         \
"image obtained by concatenation of the series of 3D images \n"         \
"prefix_inxxxx.pgm | xxxx is a four digit decimal integer of the\n"         \
"interval [ first , last]  . The values of the edges\n"         \
"are computed according to the parameter mode. If mode is\n"         \
"set to 0, then the value of an edge { x,y } is the absolute diference\n"         \
"of intensity between the pixels x and y and if mode is set to 1\n"         \
"the maximum of intensity between x and y is used. The output\n"         \
"GA4d_out.ga is a 4D GA, that is a 4D, 8-connected,\n"         \
"edge-weighted grah, (i.e. the adjacency is the direct adjacency in\n"         \
"dimension 4).\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d, byte 4d\n"         \
"\n"         \
"Category: convert\n"         \
" convert\n"         \
"\n"         \
" Jean Cousty - janvier 2006\n"         \
"\n"         \
"\n"


#define doc__shake__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__delaunaymask__c__ "  delaunaymask.c\n"         \
"\n"         \
" delaunay triangulation\n"         \
"\n"         \
"\n"         \
"\n"         \
"Usage: delaunaymask in.list mask.pgm out.list\n"         \
"\n"         \
"\n"         \
"Description:\n"         \
"Reads a point list in file in.list under the following format:\n"         \
"  \n"         \
"    b &lt;n&gt;         n &lt;n&gt;    \n"         \
"    x1 y1         x1 y1 v1\n"         \
"    x2 y2   ou    x2 y2 v2\n"         \
"    ...           ...\n"         \
"    xn yn         xn yn vn\n"         \
"  \n"         \
"\n"         \
"Computes a Delaunay triangulation and stores the resulting graph \n"         \
"\n"         \
"into file out.graph under the following format:\n"         \
"\n"         \
"    G &lt;n&gt;\n"         \
"    x1 y1 v1 ec1 ns1 s11 s12 ... s1ns1\n"         \
"    x2 y2 v2 ec2 ns2 s21 s22 ... s1ns2\n"         \
"    ...\n"         \
"    xn yn vn ecn nsn sn1 sn2 ... s1nsn\n"         \
"\n"         \
"  \n"         \
"where xi, yi are the coordinates of the ith vertex, vi is the associated \n"         \
"value (if given in the input file), eci is a int32_t which indicates \n"         \
"whether the vertex i belongs to the convex hull, nsi denotes the number\n"         \
"of adjacent vertices, and si1 si2 ... sins1 is the list of the indexes of\n"         \
"the adjacent vertices (counted from 0).\n"         \
"\n"         \
"Types supported: byte 2D\n"         \
"\n"         \
"Category: geo\n"         \
"  geo\n"         \
"\n"         \
" algorithm in O(n^2)\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__skelcurv__c__ "  skelcurv.c\n"         \
"\n"         \
" curvilinear binary skeleton guided by a priority image\n"         \
"\n"         \
"Usage: skelcurv in.pgm prio connex [inhibit] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Curvilinear binary skeleton guided by a priority image.\n"         \
"The lowest values of the priority image correspond to the highest priority.\n"         \
"\n"         \
"The parameter  prio is either an image (byte or int32_t or float or double), or a numerical code\n"         \
"indicating that a distance map will be used as a priority image; \n"         \
"the possible choices are:\n"         \
" 0: approximate euclidean distance\n"         \
" 1: approximate quadratic euclidean distance\n"         \
" 2: chamfer distance\n"         \
" 3: exact quadratic euclidean distance\n"         \
" 4: 4-distance in 2d\n"         \
" 8: 8-distance in 2d\n"         \
" 6: 6-distance in 3d\n"         \
" 18: 18-distance in 3d\n"         \
" 26: 26-distance in 3d\n"         \
" 5: breadth-first and directional strategy\n"         \
"\n"         \
"The parameter  connex indicates the connectivity of the binary object.\n"         \
"Possible choices are 4, 8 in 2d and 6, 26 in 3d.\n"         \
"\n"         \
"If the parameter  inhibit is given and is a binary image name,\n"         \
"then the points of this image will be left unchanged. \n"         \
"\n"         \
"Let X be the set corresponding to the input image  in.pgm.\n"         \
"Let P be the function corresponding to the priority image.\n"         \
"Let I be the set corresponding to the inhibit image, if given, or the empty\n"         \
"set otherwise.\n"         \
"The algorithm is the following:\n"         \
"\n"         \
"\n"         \
"C = null image\n"         \
"Repeat until stability\n"         \
"  choose a point x in X  I, simple for X, such that C[x] == 0 \n"         \
"    and such that P[x] is minimal\n"         \
"  X = X  {x}\n"         \
"  for all y in gamma(x)\n"         \
"    if T(y) > 1 then C[y] = 1\n"         \
"Result: X\n"         \
"\n"         \
"\n"         \
"where T(y) refers to the connectivity number of y, that is, \n"         \
"(informally) the number of connected components of gamma(y) inter (X  {y}).\n"         \
"\n"         \
"References: \n"         \
"[BC07] G. Bertrand and M. Couprie: \"Transformations topologiques discretes\", in G&eacute;om&eacute;trie discr&egrave;te et images num&eacute;riques, D. Coeurjolly and A. Montanvert and J.M. Chassery, pp.&nbsp;187-209, Herm&egrave;s, 2007.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__levialdi__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__lferode_rect__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__skel_CKG__c__ "  skel_CKG.c\n"         \
"\n"         \
" parallel 2D and 3D binary guided thinning\n"         \
"\n"         \
"Usage: skel_CKG in.pgm prio.pgm val out.pgm\n"         \
"\n"         \
"Description: Parallel 2D and 3D binary guided thinning based on\n"         \
"critical kernels. The parameter  in.pgm specifies the set\n"         \
"(object) to be thinned. The parameter  prio.pgm specifies the priority\n"         \
"function. The parameter  val is a threshold: any\n"         \
"pixel having a priority greater than or equal to  val will be preserved\n"         \
"from deletion.  If  val equals -1, then all points will be\n"         \
"considered for deletion.\n"         \
"\n"         \
"Warning: The object must not have any point on the frame of the image.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__skel_ACK3_pers__c__ "  skel_ACK3c.c\n"         \
"\n"         \
" topological persistence of 1D isthmuses, based on parallel 3D asymetric thinning\n"         \
"\n"         \
"Usage: skel_ACK3c in.pgm out.pgm\n"         \
"\n"         \
"Description: Topological persistence of 1D isthmuses, based on \n"         \
"parallel 3D asymetric thinning.\n"         \
"\n"         \
"When a point x is detected as a 1D isthmus, a counter p(x) is\n"         \
"associated to this point and initialized with value 1. This counter is\n"         \
"incremented a each iteration as long as x is still an isthmus. When this point x is\n"         \
"eventually deleted, the value of the counter is freezed.\n"         \
"\n"         \
" The object must not have any point on the frame of the image.\n"         \
"\n"         \
"Types supported: byte 3d\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__recons___notused_c__ "WRITE ME!!!\n"         \
"\n"


#define doc__signal2pgm___notused_c__ "  signal2pgm.c\n"         \
"\n"         \
" converts from signal representation to pgm\n"         \
"\n"         \
"Usage: signal2pgm in.list [scale] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"Reads the file in.list. This file must have one of the following formats:\n"         \
"  \n"         \
"    s &lt;n&gt; \n"         \
"    x1 y1 \n"         \
"    x2 y2 \n"         \
"    ...   \n"         \
"    xn yn \n"         \
"\n"         \
"\n"         \
"The optional parameter  scale allows to scale the y coordinates.\n"         \
"\n"         \
"Types supported: signal 1D\n"         \
"\n"         \
"Category: convert\n"         \
"  convert\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__argmax__c__ "  argmax.c\n"         \
"\n"         \
" return the coordinates of a pixel having the maximal value\n"         \
"\n"         \
"Usage: argmax in.pgm [out.list]\n"         \
"\n"         \
"Description:\n"         \
"This function returns (in the list out.list) \n"         \
"coordinates of a pixel having the maximal value in the image  in.pgm .\n"         \
"\n"         \
"If the parameter  out.list is ommitted, the result is printed on the standard output.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n"         \
"\n"         \
"Category: arith\n"         \
"  arith\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__2dcollapse__c__ "  2dcollapse.c\n"         \
"\n"         \
" ultimate constrained collapse guided by a priority image\n"         \
"\n"         \
"Usage: 2dcollapse in.pgm prio [inhibit] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Ultimate constrained collapse guided by a priority image.\n"         \
"The lowest values of the priority image correspond to the highest priority.\n"         \
"\n"         \
"The parameter  prio is either an image (byte or int32_t), or a numerical code\n"         \
"indicating that a distance map will be used as a priority image; \n"         \
"the possible choices are:\n"         \
" 0: approximate euclidean distance\n"         \
" 1: approximate quadratic euclidean distance\n"         \
" 2: chamfer distance\n"         \
" 3: exact quadratic euclidean distance\n"         \
" 4: 4-distance in 2d\n"         \
" 8: 8-distance in 2d\n"         \
"\n"         \
"If the parameter  inhibit is given and is a binary image name, then\n"         \
"the elements of this image will be left unchanged.  If the parameter\n"         \
" inhibit is given and is a number I, then the elements with priority\n"         \
"greater than or equal to I will be left unchanged.  \n"         \
"\n"         \
" The result makes sense only if the input image is a complex.\n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: orders\n"         \
"  orders\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__dynamique__c__ "  dynamique.c\n"         \
"\n"         \
" dynamics of the maxima\n"         \
"\n"         \
"Usage: dynamique in.pgm connex [order] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Computes the ordered dynamics of the maxima, with connectivity connex.\n"         \
"The definition of the ordered dynamics is the one given in [Ber05]. \n"         \
"The optional argument order is one of the following:\n"         \
" 0: altitude [default] \n"         \
" 1: area\n"         \
" 2: volume\n"         \
"\n"         \
"References: \n"         \
"[Ber05] G. Bertrand: \"A new definition of the dynamics\", Procs. ISMM05, Springer, series Computational Imaging and Vision, Vol.&nbsp;30, pp.&nbsp;197-206, 2005.\n"         \
"\n"         \
"Types supported: byte 2D, byte 3D.\n"         \
"\n"         \
"Category: \n"         \
" connect\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__erosion__c__ "  erosion.c\n"         \
"\n"         \
" morphological erosion by a plane structuring element\n"         \
"\n"         \
"Usage: erosion in.pgm se.pgm out.pgm\n"         \
"\n"         \
"Description:\n"         \
"The (plane) structuring element is given by the non-null values in  se.pgm, \n"         \
"its origin (wrt the point (0,0) of  se.pgm ) is given by a comment line in the file se.pgm. \n"         \
"\n"         \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n"         \
"\n"         \
"Category: morpho\n"         \
"  morpho\n"         \
"\n"         \
" Michel Couprie 2002\n"         \
"\n"         \
"Example:\n"         \
"\n"         \
"erosion cells ~/Pink/masks/carre5.pgm cells_eros\n"         \
"\n"         \
"\n"         \
"  \n"         \
"      \n"         \
"  \n"         \
"  cells\n"         \
"      cells_eros\n"         \
"  \n"         \
"\n"         \
"\n"         \
"\n"         \
"\n"


#define doc__randimage__c__ "  randimage.c\n"         \
"\n"         \
" random image generation\n"         \
"\n"         \
"Usage: randimage [in.pgm | rowsize colsize depth] valmax [seed] out.pgm\n"         \
"\n"         \
"Description: Generates an image with pixel values uniformly randomly\n"         \
"distributed between 0 and  valmax (included). \n"         \
"If given, the parameter  seed is used to \n"         \
"initialize the random generator, otherwise the clock is used instead.\n"         \
"\n"         \
"Types supported: byte 2D, byte 3D, int32_t 2D, int32_t 3D\n"         \
"\n"         \
"Category: signal\n"         \
"  signal\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__border__c__ "  border.c\n"         \
"\n"         \
" border of a binary image\n"         \
"\n"         \
"Usage: border in connex out\n"         \
"\n"         \
"Description:\n"         \
"Let X be the subset of Zn that corresponds to the input image  in, and \n"         \
"let k be the value of the parameter  connex (that is, 4, 8 (2d) or 6, 26 (3d)), which is the connectivity chosen for X.\n"         \
"\n"         \
"The border B(X) of X is defined by:\n"         \
"B(X) = {x in X, (Gamma_nk(x) inter Xbar) not empty}\n"         \
"where nk is the complementary connectivity for k.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__setvoxdim__c__ "  setvoxdim.c\n"         \
"\n"         \
" adds voxel dimensions to file header\n"         \
"\n"         \
"Usage: setvoxdim image xdim ydim zdim\n"         \
"\n"         \
"Description: Adds voxel dimensions ( xdim  ydim  zdim ) \n"         \
"to file header.\n"         \
"\n"         \
"Types supported: byte 3d, int32_t 3d\n"         \
"\n"         \
"Category: convert\n"         \
" convert\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__ptsimple__c__ "  ptsimple.c\n"         \
"\n"         \
" detects simple points in a binary image\n"         \
"\n"         \
"Usage: ptsimple in.pgm connex out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Detects simple points in a binary image.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" Michel Couprie 2003\n"         \
"\n"         \
"\n"


#define doc__curvelength__cxx__ "  curvelength.c\n"         \
"\n"         \
" computes the length of a digital curve\n"         \
"\n"         \
"Usage: curvelength curve.list closed [out.list]\n"         \
"\n"         \
"Description:\n"         \
"Computes the length of a curve which is specified by the ordered list of its points.\n"         \
"\n"         \
"Parameter  closed is a boolean (1 or 0) that indicates whether the curve is closed or not.\n"         \
"\n"         \
"Types supported: curve 2D, curve 3D\n"         \
"\n"         \
"Category: geo\n"         \
" geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__skelpar3d_others__c___ "  skelpar3d_others.c\n"         \
"\n"         \
" parallel 3D binary skeleton\n"         \
"\n"         \
"Usage: skelpar3d in.pgm algorithm nsteps [inhibit] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Parallel 3D binary thinning or skeleton. The parameter  nsteps gives,\n"         \
"if positive, the number of parallel thinning steps to be processed.\n"         \
"If the value given for  nsteps equals -1, the thinning is continued\n"         \
"until stability.\n"         \
"\n"         \
"The parameter  algorithm is a numerical code\n"         \
"indicating which method will be used for the thinning.\n"         \
"The possible choices are:\n"         \
" 0: Palagyi (fully parallel, 2007)\n"         \
"\n"         \
"If the parameter  inhibit is given and is a binary image name,\n"         \
"then the points of this image will be left unchanged. \n"         \
"\n"         \
"Types supported: byte 3d\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__relief__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__deletecomp__c__ "  deletecomp.c\n"         \
"\n"         \
" deletes a connected component\n"         \
"\n"         \
"Usage: deletecomp in.pgm x y z out.pgm\n"         \
"\n"         \
"Description:\n"         \
"The connected component of the binary image in.pgm\n"         \
"which contains the point (x,y,z)\n"         \
"is deleted.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: connect, topobin\n"         \
"  connect, topobin\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__ppm2bmp__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__comptreetime__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__proj__c__ "  proj.c\n"         \
"\n"         \
" projection parallel to one of the main axes\n"         \
"\n"         \
"Usage: proj in.pgm dir mode out.pgm\n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"Projection parallel to one of the main axes.\n"         \
"\n"         \
"dir = x | y | z\n"         \
"\n"         \
"mode = 0 (mean) | 1 (max)\n"         \
"\n"         \
"Types supported: byte 2D, byte 3D\n"         \
"\n"         \
"Category: geo\n"         \
"  geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__hitormiss__c__ "  hitormiss.c\n"         \
"\n"         \
" morphological hit or miss transformation for binay images\n"         \
"\n"         \
"Usage: hitormiss in.pgm e1.pgm e2.pgm out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Let X be the input set, E1, E2 be the two structuring elements, the result Y is given by:\n"         \
"Y = (X - E1) inter (Xb - E2)\n"         \
"where '-' denotes the erosion operator, and 'Xb' denotes the complementary of X.  \n"         \
"\n"         \
"Types supported: byte 2d, byte 3d, int32_t 2d, long3d\n"         \
"\n"         \
"Category: morpho\n"         \
"  morpho\n"         \
"\n"         \
" Michel Couprie 2003\n"         \
"\n"         \
"\n"         \
"\n"


#define doc__areaclosing__c__ "  areaclosing.c\n"         \
"\n"         \
" area closing\n"         \
"\n"         \
"Usage: areaclosing in.pgm connex area out.pgm\n"         \
"\n"         \
"Note:  in C main passes an argument 'area+1', whereas python passes the 'area'\n"         \
"as argument (LuM)\n"         \
"\n"         \
"Description:\n"         \
"Area closing with connexity connex and area area.\n"         \
"Deletes the components of the lower cross-sections \n"         \
"which have an area not higher than area.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: connect\n"         \
" connect\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__rgb2hls__c__ "  rgb2hls.c\n"         \
"\n"         \
" converts a ppm color file to hls representation\n"         \
"\n"         \
"Usage: in.ppm [mode] h.pgm l.pgm s.pgm\n"         \
"\n"         \
"Description:\n"         \
"Constructs 3 pgm files from 1 ppm file:\n"         \
" h.pgm : Hue (int32_t - 0 to 359)\n"         \
" h.pgm : Luminance (byte)\n"         \
" h.pgm : Saturation (byte)\n"         \
"\n"         \
"Different modes are available [default mode is 0]:\n"         \
" mode = 0: classical HLS coding, based on [1].\n"         \
" mode = 1: L1 norm (NYI) (see [2])\n"         \
" mode = 2: L1 norm with gamma correction (NYI) (see [2])\n"         \
"\n"         \
"[1] Foley, Van Damm & al: \"Computer Graphics\", 2nd ed., p. 595\n"         \
"\n"         \
"[2] J. Angulo and J. Serra. \"Traitements des images de couleur \n"         \
"en représentation luminance/saturation/teinte par norme L_1\" (in French). \n"         \
"Traitement du Signal, Vol. 21, No. 6, pp 583-604, December 2004.\n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: convert\n"         \
"  convert\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__mcmbuild__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__lambdamedialaxis2auto__c__ "  lambdamedialaxis2.c\n"         \
"\n"         \
" discrete lambda-medial axis transform\n"         \
"\n"         \
"Usage: lambdamedialaxis2 in.pgm lambda opening.pgm out.pgm\n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"lambda:\n"         \
" -1: produce lambdamedialaxis map\n"         \
"  0: automaticly define lambda parameter\n"         \
"  >0: produce binary image based on defined lambda\n"         \
" \n"         \
"Experimental\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: morpho\n"         \
"  morpho\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__saliencyGA__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__surfels__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__lintophat__c__ "  lintophat.c\n"         \
"\n"         \
" max of morphological black top hats by linear structuring elements\n"         \
"\n"         \
"Usage: lintophat in.pgm length out.pgm\n"         \
"\n"         \
"Description: \n"         \
"Max of morphological black top hats by linear structuring elements, \n"         \
"in all possible discrete directions.\n"         \
"Let F be the original image and E be a structuring element, the black\n"         \
"top hat of F by E is defined by F - closing(F, E).\n"         \
"The closing deletes dark structures that do not match the \n"         \
"structuring element, thus the black top hat detects those dark structures.\n"         \
"For a linear structuring element, the detected structures are those which\n"         \
"are orthogonal to the se.\n"         \
"The length if the linear structuring elements is given by  length.\n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: morpho\n"         \
"  morpho\n"         \
"\n"         \
" Hildegard Koehler 2003\n"         \
"\n"         \
"\n"         \
"\n"


#define doc__pthseparatinggray__c__ "  pthseparatinggray.c\n"         \
"\n"         \
" detects h-separating points in a grayscale image\n"         \
"\n"         \
"Usage: pthseparatinggray in.pgm connex h out.pgm\n"         \
"\n"         \
"Description:\n"         \
"An h-separating point for an image F is a point p, \n"         \
"such that there exists c verifying F(p)-h \n"         \
"#CC(X inter N(p)) > 1, with X = {x | F(x) \n"         \
"\n"         \
"Types supported: byte 2d, byte 2d\n"         \
"\n"         \
"Category: topogray\n"         \
"  topogray\n"         \
"\n"         \
" Michel Couprie 2010\n"         \
"\n"         \
"\n"


#define doc__matchellipse__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__erosball__c__ "  erosball.c\n"         \
"\n"         \
" morphological binary erosion by a ball\n"         \
"\n"         \
"Usage: erosball in.pgm r [dist] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Morphological binary erosion by a metric ball.\n"         \
"This erosion is the dual of the dilatball operator, \n"         \
"i.e. erosball(X) = inverse(dilatball(inverse(X))).\n"         \
"The structuring element for this erosion is a ball (or a disc) of radius  r.\n"         \
"The erosion is computed by thresholding a distance map.\n"         \
"The distance used depends on the optional parameter  dist (default is 0) :\n"         \
" 0: rounded euclidean distance\n"         \
" 2: chamfer distance\n"         \
" 3: exact quadratic euclidean distance\n"         \
" 4: 4-distance in 2d\n"         \
" 8: 8-distance in 2d\n"         \
" 6: 6-distance in 3d\n"         \
" 18: 18-distance in 3d\n"         \
" 26: 26-distance in 3d\n"         \
"\n"         \
" The input image  in.pgm must be a binary image. No test is done.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: morpho\n"         \
"  morpho\n"         \
"\n"         \
" Michel Couprie 2002\n"         \
"\n"         \
"Example:\n"         \
"\n"         \
"erosball cells 3 cells_erosball\n"         \
"\n"         \
"\n"         \
"  \n"         \
"      \n"         \
"  \n"         \
"  cells\n"         \
"      cells_erosball\n"         \
"  \n"         \
"\n"         \
"\n"         \
"\n"         \
"\n"


#define doc__histoelevation__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__3dskelck__c__ "  3dskelck.c\n"         \
"\n"         \
" homotopic thinning based on critical kernels\n"         \
"\n"         \
"Usage: 3dskelck in.pgm mode niter [inhibit] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"Homotopic thinning of a 3d Khalimsky image.\n"         \
"The maximum number of steps is given by niter.\n"         \
"If the value given for  niter equals -1, the thinning is continued\n"         \
"until stability.\n"         \
"\n"         \
"The parameter  mode specifies the algorithm used for thinning.\n"         \
"Possible choices are:\n"         \
" 0: ultimate crucial thinning, symmetrical\n"         \
"\n"         \
"If the parameter  inhibit is given and is a binary image name,\n"         \
"then the points of this image will be left unchanged. \n"         \
"\n"         \
" Input must be a complex with no point on the image border.\n"         \
"\n"         \
"References: \n"         \
"\n"         \
"[Ber07] G. Bertrand: \"On critical kernels\", Comptes-rendus de l'AcadÃ©mie des Sciences, sÃ©rie math., Vol. I, Nb. 345, pp. 363-367, 2007.\n"         \
"\n"         \
"[BC09] G. Bertrand and M. Couprie: \"On parallel thinning algorithms: minimal non-simple sets, P-simple points and critical kernels\", Journal of Mathematical Imaging and Vision, Vol. 35, Nb. 1, pp. 23-35, 2009.\n"         \
"\n"         \
"Types supported: byte 3d\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__matchrect__c__ "  matchrect.c\n"         \
"\n"         \
" matches (rounded) rectangles to the contours of objects\n"         \
"\n"         \
"Usage: matchrect in.pgm connex [rounded|axis] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Matches (rounded) rectangles to the contours of objects.\n"         \
"Objects are connected components according to the connectivity\n"         \
"given by  connex .\n"         \
"Option  rounded is used to match rounded rectangles.\n"         \
"Option  axis is used to output only the big axis of the matched rectangle.\n"         \
"\n"         \
"Types supported: byte 2D\n"         \
"\n"         \
"Category:  geo\n"         \
"  geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__measure__cpp__ "\n"         \
"    measure.cpp\n"         \
"\n"         \
"    Normalized absolute measure calculated from the image.\n"         \
"   \n"         \
"   Usage: measure( (float_image)image) -> float_image\n"         \
"   \n"         \
"   Description: \n"         \
"\n"         \
"   Types supported: float 2d, float 3d\n"         \
"   \n"         \
"   Category: signal, development\n"         \
"    signal, development\n"         \
"\n"         \
"   References:\n"         \
"   [1]  GNU Scientific Library \n"         \
"    Laszlo Marak, 2009-2010\n"         \
"\n"         \
" \n"         \
"\n"


#define doc__extractplane__c__ "  extractplane.c\n"         \
"\n"         \
" extracts a plane normal to one of the directions x,y,z from a 3D image\n"         \
"\n"         \
"Usage: extractplane in.pgm n plane out.pgm\n"         \
"\n"         \
"Description:\n"         \
"The original 3D image is in.pgm. The parameter n is an integer.\n"         \
"The parameter plane is set to xy, yx, \n"         \
"xz, zx, yz, zy.\n"         \
"The order of the letters x, y, z is meaningfull: for example, zy means that \n"         \
"each row of the output image will correspond to a z-line of the original image, \n"         \
"each column of the output image will correspond to a y-line of the original image.\n"         \
"The output image out.pgm contains the nth plane of the given form extracted\n"         \
"from in.pgm.\n"         \
"\n"         \
"Types supported: byte 3d, long 3d, float 3d\n"         \
"\n"         \
"Category: geo\n"         \
"  geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__directionalfilter__c__ "  directionalfilter.c\n"         \
"\n"         \
" directional filter for curvilinear feature extraction\n"         \
"\n"         \
"Usage: directionalfilter.c in.pgm width length ndir out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Let F be the original image from  in.pgm .\n"         \
"This operator computes the supremum of the convolutions of F\n"         \
"by a series of kernels K0, ... Kn where n =  ndir - 1, which are defined\n"         \
"by, for each (x,y) and each i in [0...n]: \n"         \
"\n"         \
"\n"         \
"sigma = 1 / (2*width*width);\n"         \
"lambda = 1 / (2*length*length);\n"         \
"theta = i * PI / n;\n"         \
"xr = cos(theta) * x - sin(theta) * y;\n"         \
"yr = sin(theta) * x + cos(theta) * y;\n"         \
"Ki(x,y) = exp(-lambda*yr*yr) *\n"         \
"          (4*sigma*sigma*xr*xr - 2*sigma) * \n"         \
"          exp(-sigma*xr*xr) \n"         \
"\n"         \
"\n"         \
"Types supported: byte 2d, int32_t 2d, float 2d\n"         \
"\n"         \
"Category: signal\n"         \
"  signal\n"         \
"\n"         \
" Michel Couprie 2003\n"         \
"\n"         \
"\n"


#define doc__skel_ASK3b___notused_c__ "  skel_ASK3b.c\n"         \
"\n"         \
" parallel 3D binary surface, asymetric skeleton based on thin 2D isthmuses\n"         \
"\n"         \
"Usage: skel_ASK3b in.pgm nsteps isthmus_persistence [inhibit] out.pgm\n"         \
"\n"         \
"Description: Parallel 3D binary thinning or surface,\n"         \
"asymetric skeleton based on thin 2D isthmuses. The parameter  nsteps\n"         \
"gives, if positive, the number of parallel thinning steps to be\n"         \
"processed.  If the value given for  nsteps equals -1, the thinning\n"         \
"is continued until stability.\n"         \
"\n"         \
"When a point x is detected as a 2D isthmus, a counter p(x) is\n"         \
"associated to this point and initialized with value 1. This counter is\n"         \
"incremented a each iteration as long as x is still an isthmus. At each\n"         \
"iteration, the isthmuses x such that p(x) >=  isthmus_persistence are\n"         \
"stored as a constraint set (see also  inhibit parameter).\n"         \
"\n"         \
"If the parameter  inhibit is given and is a binary image name,\n"         \
"then the points of this image will be left unchanged. \n"         \
"\n"         \
" The object must not have any point on the frame of the image.\n"         \
"\n"         \
"Types supported: byte 3d\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__dilatfast__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__CMakeLists__txt__ "WRITE ME!!!\n"         \
"\n"


#define doc__jones__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__selrect__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__catpgm__c__ "  catpgm.c\n"         \
"\n"         \
" catenates a sequence of 2d (resp. 1d) images into a 3d (resp. 2d) image\n"         \
"\n"         \
"Usage: catpgm name_prefix index_begin index_end [xdim ydim zdim] name_out\n"         \
"\n"         \
"Description:\n"         \
"Original file names must be of the form: name_prefixnnnn.pgm, \n"         \
"where nnnn is a four digit decimal integer. The integers index_begin\n"         \
"and index_end specify respectively the first and the last index of the series.\n"         \
"\n"         \
"Types supported: sequence of byte 1d or 2d images\n"         \
"\n"         \
"Category:  convert\n"         \
"   convert\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__vskel2graph___notfinished_c__ "WRITE ME!!!\n"         \
"\n"


#define doc__seamcarving__c__ "  seamcarving.c\n"         \
"\n"         \
" applies the seam carving method to shrink an image\n"         \
"\n"         \
"Usage: seamcarving in.ppm energy.pgm w h out.ppm\n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: geo\n"         \
"  geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__mcm2vtk__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__geoeros__c__ "  geoeros.c\n"         \
"\n"         \
" geodesic (grayscale or binary) erosion\n"         \
"\n"         \
"Usage: geoeros g.pgm f.pgm connex niter out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Geodesic erosion of g.pgm over f.pgm.\n"         \
"Let G and F be the two input images. If G is not over F, then \n"         \
"G is replaced initially by max(G,F).\n"         \
"The structuring element is specified by the value of the parameter connex, \n"         \
"which can be one of the following ones: 4, 8 in 2d, or 6, 18, 26 in 3d.\n"         \
"The parameter  niter sets the number of iterations. If  niter = -1,\n"         \
"then the iterations continue until stability.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d.\n"         \
"\n"         \
"Category: connect, morpho\n"         \
"  connect, morpho\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__squelubp3d___notused_c__ "WRITE ME!!!\n"         \
"\n"


#define doc__crop__c__ "  crop.c\n"         \
"\n"         \
" extracts a rectangular (cuboidal in 3d) area from an image\n"         \
"\n"         \
"Usage: crop in.pgm x y [z] w h [d] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"For a 2d image, extracts the rectangle with upper left corner (x,y),\n"         \
"of width w and height h from the image in.pgm .\n"         \
"For a 3d image, extracts the cuboid with upper left corner (x,y, z),\n"         \
"of width w, height h and depth d from the image in.pgm .\n"         \
"\n"         \
"Types supported: byte 2d, int32_t 2d, float 2d, byte 3d, int32_t 3d, float 3d.\n"         \
"\n"         \
"Category: convert, geo\n"         \
"  convert, geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__minimalsimplepair__c__ "  minimalsimplepair.c\n"         \
"\n"         \
" detects minimal simple pairs in a 3D binary image\n"         \
"\n"         \
"Usage: minimalsimplepair in.pgm [all] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Detects minimal simple pairs in a 3D binary image. \n"         \
"If the optional parameter  all is used, then all \n"         \
"minimal simple pairs are searched.\n"         \
"Otherwise, only one pair (if any) is given.\n"         \
"\n"         \
"If the input image contains no simple point, consider using \"simplepair\" (faster)\n"         \
"instead of \"minimalsimplepair\".\n"         \
"\n"         \
"Reference: \n"         \
"[PCB08] Nicolas Passat, Michel Couprie, and Gilles Bertrand:\n"         \
"\"Minimal simple pairs in the 3D cubic grid\", in\n"         \
"Journal of Mathematical Imaging and Vision, 32 (3), pp.&nbsp;239â€“249, November 2008.\n"         \
"\n"         \
"Types supported: byte 3d\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" Michel Couprie 2008\n"         \
"\n"         \
"\n"


#define doc__skelvertex__c__ "  skelvertex.c\n"         \
"\n"         \
" selection of a specified vertex in a curvilinear skeleton\n"         \
"\n"         \
"Usage: skelvertex in.skel vertex_id out.pgm\n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"In the output image  out.pgm, only the pixels/voxels of the skeleton element\n"         \
"corresponding to  vertex_id are present.\n"         \
"\n"         \
"Types supported: skel 2d, skel 3d\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" Michel Couprie 2009\n"         \
"\n"         \
"\n"


#define doc__explode__c__ "  explode.c\n"         \
"\n"         \
" converts single 3D pgm file into a series of 2D pgm files\n"         \
"\n"         \
"Usage: explode in.pgm [begin end step] name_prefix\n"         \
"\n"         \
"Description:\n"         \
"Generated file names are of the form: name_prefixnnnn.pgm, \n"         \
"where nnnn is a four digit decimal integer. \n"         \
"\n"         \
"Types supported: byte 3d\n"         \
"\n"         \
"Category: convert\n"         \
"  convert\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__skelsmoothing__c__ "  skelsmoothing.c\n"         \
"\n"         \
" computes a smmothed version of a curvilinear skeleton\n"         \
"\n"         \
"Usage: skelsmoothing in.skel mode param out.skel\n"         \
"\n"         \
"Description:\n"         \
"Computes a smmothed version of a curvilinear skeleton.\n"         \
"\n"         \
"The parameter  mode specifies the smoothing method:\n"         \
" 0: straight lines segments\n"         \
" 1: splines\n"         \
"\n"         \
"Types supported: 2Dskel, 3Dskel\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" Michel Couprie 2011\n"         \
"\n"         \
"\n"


#define doc__gaussianfilter__c__ "  gaussianfilter.c\n"         \
"\n"         \
" Gaussian filter operator\n"         \
"\n"         \
"Usage: gaussianfilter in.pgm alpha out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Deriche's recursive implementation of the smoothing filter\n"         \
"operator (approximates the convolution with a Gaussian kernel). \n"         \
"The parameter  alpha (double) controls the spatial extension of the\n"         \
"filter: 0 \n"         \
"Intuitively, the \"width\" of the Gaussian kernel corresponds to 1/ alpha.\n"         \
"\n"         \
"Reference: \n"         \
"[Der90] R. Deriche, \"Fast algorithms for low-level vision\",\n"         \
"IEEE Transactions on PAMI, 12(1), 78-87, 1990.\n"         \
"\n"         \
"Types supported: byte 2d, long 2d, float 2d, byte 3d, long 3d, float 3d\n"         \
"\n"         \
"Category: signal\n"         \
"  signal\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__correctbias__c__ "  correctbias.c\n"         \
"\n"         \
" correction of a luminosity bias expressed by a linear function\n"         \
"\n"         \
"Usage: correctbias in.pgm factor type [xc yc] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Let  a be the value of parameter  factor.\n"         \
"The bias  type can be:\n"         \
" 0 radial (center  xc,  yc), linear (add -ax to the value of the element of abcissa x).\n"         \
" 1 radial (center  xc,  yc), parabolic (add -a^2 x to the value of the element of abcissa x).\n"         \
"\n"         \
"Types supported: byte 2D\n"         \
"\n"         \
"Category: arith\n"         \
" arith\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__symse__c__ "  symse.c\n"         \
"\n"         \
" inverse of a structuring element\n"         \
"\n"         \
"Usage: symse in.pgm out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Inverse of a structuring element.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d.\n"         \
"\n"         \
"Category: geo\n"         \
"  geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__pgm2GA__c__ "  pgm2GA.c\n"         \
"\n"         \
" Computes an edge-weighted graph from an image\n"         \
"\n"         \
"Usage: pgm2GA im.pgm param [alpha] out.ga \n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"Computes a GA (an edge-weighted graph) from an image. The values of an\n"         \
"edge linking two pixels {x,y} is computed according to the parameter\n"         \
"param.\n"         \
"\n"         \
"If param = 0, the absolute difference of intensity between x and y is used.\n"         \
"\n"         \
"If param = 1, the maximum between the intensities of x and y is used.\n"         \
"\n"         \
"If param = 2, the minimum between the intensities of x and y is used.\n"         \
"\n"         \
"If param = 3, a Deriche-like gradient is used, the optional parameter\n"         \
"alpha specifies the spatial extention of the filter (by default alpha\n"         \
"is set to  1]),\n"         \
"\n"         \
"If im.pgm is a 2D (resp. 3D) image, then out.ga is a 2D (resp\n"         \
"3D GA), that is a 2D 4-connected edge-weighted graph (resp. a 2D\n"         \
"6-connected edge-weighted graph).\n"         \
"\n"         \
"\n"         \
"Types supported: GA byte 2D, GA byte 3D, GA float 2D\n"         \
"\n"         \
"Category: \n"         \
"  GA\n"         \
"\n"         \
" Jean Cousty\n"         \
"\n"         \
"\n"


#define doc__rotse__c__ "  rotse.c\n"         \
"\n"         \
" rotation of a structuring element\n"         \
"\n"         \
"Usage: rotse in.pgm angle out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Rotation of a structuring element, by an angle of 0, 90, 180 or 270 degrees.\n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: geo\n"         \
"  geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__skel_EK3__c__ "  skel_EK3.c\n"         \
"\n"         \
" parallel 3D binary curvilinear skeleton based on ends\n"         \
"\n"         \
"Usage: skel_EK3 in.pgm nsteps [inhibit] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Parallel 3D binary thinning or curvilinear skeleton based on ends. \n"         \
"The parameter  nsteps gives,\n"         \
"if positive, the number of parallel thinning steps to be processed.\n"         \
"If the value given for  nsteps equals -1, the thinning is continued\n"         \
"until stability.\n"         \
"\n"         \
"If the parameter  inhibit is given and is a binary image name,\n"         \
"then the points of this image will be left unchanged. \n"         \
"\n"         \
"Warning: The object must not have any point on the frame of the image.\n"         \
"\n"         \
"Types supported: byte 3d\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__amont__c__ "  amont.c\n"         \
"\n"         \
" amont of a binary marker in a grayscale image\n"         \
"\n"         \
"Usage: amont in.pgm mark.pgm connex [s] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Let F be the function defined by  in.pgm, \n"         \
"let X be the set defined by  mark.pgm.\n"         \
"The amont of X in F is defined as the set of points x such that there exists\n"         \
"an ascending path from X to x for F with connectivity connex.\n"         \
"If the parameter s is given, then only strictly ascending paths are \n"         \
"considered.\n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: connect\n"         \
" connect\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__gradill__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__double2byte__c__ "  double2byte.c\n"         \
"\n"         \
" converts a \"double\" image to a \"byte\" image\n"         \
"\n"         \
"Usage: double2byte in.pgm [mode] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"Depending on the value given for the (optional) parameter mode:\n"         \
"   mode = 0 (default) : for all x, out[x] = min(255, arrondi(in[x])).\n"         \
"   mode = 1 : for all x, out[x] = arrondi(in[x]) modulo 256.\n"         \
"   mode = 2 : scales values in the range 0-255.\n"         \
"   mode = 4 : truncation of the square root in the range 0-255.\n"         \
"   mode = 5 : truncation of the log in the range 0-255.\n"         \
"\n"         \
"Types supported: double 2d, double 3d\n"         \
"\n"         \
"Category: convert\n"         \
"  convert\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__ccv__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__convexhull__c__ "  convexhull.c\n"         \
"\n"         \
" convex hull of a set of points in the 2D plane\n"         \
"\n"         \
"Usage: convewhull in.pgm mode out\n"         \
"\n"         \
"Description: Computes the convex hull of the set of non-null points\n"         \
"in the image in.pgm, by the Jarvis's algorithm as described in \n"         \
"\"Introduction to algorithms\", T. Cormen, C. Leiserson, R. Rivest, MIT Press.\n"         \
"\n"         \
"The parameter  mode selects the format of the result: \n"         \
" 0: list of the support points of the convex hull\n"         \
" 1: support points of the convex hull, embedded in the image frame\n"         \
" 2: contour of the convex hull, embedded in the image frame\n"         \
" 3: full convex hull, embedded in the image frame\n"         \
"\n"         \
"Types supported: byte 2D\n"         \
"\n"         \
"Category: geo\n"         \
"  geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__skelfindelbows__c__ "  skelfindelbows.c\n"         \
"\n"         \
" detection of \"elbows\" in a curvilinear skeleton\n"         \
"\n"         \
"Usage: skelfindelbows in.skel thickness sharp out.pgm\n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"Find \"elbows\" (points making sharp angles) in the curves of skeleton  in.skel.\n"         \
"Matching points are written as voxels in the returned image  out.pgm.\n"         \
"\n"         \
"\n"         \
"Let  be the points of a cover of the curve C by digital straight line segments (DSSs). \n"         \
"Let j be an index between 1 and n-2, if angle(S[j-1]S[j], S[j]S[j+1]) \n"         \
"\n"         \
"\n"         \
"Parameter  thick (thickness for DSS recognition) is given in pixels, parameter  sharp in degrees.\n"         \
"\n"         \
"Types supported: skel 2d, skel 3d\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" Michel Couprie 2011\n"         \
"\n"         \
"\n"


#define doc__long2short__cpp__ "  long2byte.c\n"         \
"\n"         \
" converts a \"int32_t\" image to a \"ushort\" image\n"         \
"\n"         \
"Usage: long2byte in.pgm [mode] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"Depending on the value given for the (optional) parameter mode:\n"         \
"   mode = 0 (default) : for all x, out[x] = min(USHRT_MAX, in[x]).\n"         \
"   mode = 1 : for all x, out[x] = in[x] modulo USHRT_MAX.\n"         \
"   mode = 2 : scales values in the range 0-USHRT_MAX.\n"         \
"   mode = 4 : truncation of the square root in the range 0-255.\n"         \
"   mode = 5 : truncation of the log in the range 0-255.\n"         \
"\n"         \
"Types supported: short_image (any D)\n"         \
"\n"         \
"Category: convert\n"         \
"  convert\n"         \
"\n"         \
" Laszlo Marak, 2011\n"         \
"\n"         \
"\n"


#define doc__meshwarp___notused_c__ "WRITE ME!!!\n"         \
"\n"


#define doc__pgm2graph__c__ "  pgm2graph.c\n"         \
"\n"         \
" converts a binary image into a simple graph\n"         \
"\n"         \
"Usage: pgm2graph in.pgm connex out.graph\n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"The vertices of the graph are the object pixels of the binary image  in.pgm, and there is an edge between each pair of adjacent pixels (according to  connex).  \n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" Michel Couprie 2010\n"         \
"\n"         \
"\n"


#define doc__lambdakern___notused_c__ "WRITE ME!!!\n"         \
"\n"


#define doc__boxmin__c__ "  boxmin.c\n"         \
"\n"         \
" minimal box including a given set of points\n"         \
"\n"         \
"Usage: boxmin in.pgm out.pgm\n"         \
"\n"         \
"Description:\n"         \
"The result out.pgm has the same size as in.pgm. \n"         \
"It contains the minimal box including the set of points in  in.pgm \n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: geo\n"         \
"  geo\n"         \
"\n"         \
" Michel Couprie 2002\n"         \
"\n"         \
"\n"


#define doc__hthick3d__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__recalagerigide_num__c__ "  recalagerigide_num.c\n"         \
"\n"         \
" rigid registration of two grayscale images\n"         \
"\n"         \
"Usage: recalagerigide_num in1 in2 seuil [init] out\n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"Rigid registration of two grayscale images. \n"         \
"Let X and Y be two images, given respectively by  in1 and  in2.\n"         \
"This procedure identifies the parameters of a rigid deformation R such that\n"         \
"the \"distance\" between R(X) and Y is a local minimum.\n"         \
"The distance D(Z,Y) between two images Z and Y is defined by:\n"         \
"\n"         \
"D(Z,Y) = sum { (Z[i] - Y[i])^2 ; for all i in support(Y) }\n"         \
"\n"         \
"The rigid deformation R is defined as the composition (in this order) \n"         \
"of a zoom, a rotation, and a translation. \n"         \
"\n"         \
"In 2d, the parameters are:\n"         \
" hx : parameter for the scaling in direction x\n"         \
" hy : parameter for the scaling in direction y\n"         \
" theta : angle (in degrees) of the rotation around (0,0)\n"         \
" tx : parameter for the translation in direction x\n"         \
" ty : parameter for the translation in direction y\n"         \
"\n"         \
"The output parameter  out is the name of a text file in which these \n"         \
"values will be written (type \"e\" list format, see doc/formats.txt).\n"         \
"\n"         \
"The parameter  seuil makes it possible to eliminate outliers: \n"         \
"points i such that (Z[i] - Y[i])^2 > seuil^2 are not taken into account.\n"         \
"A value 255 for this parameter means no outlier elimination.\n"         \
"\n"         \
"The optional parameter  init makes it possible to give, \n"         \
"in the same format as the output, an initial deformation which is \n"         \
"\"close\" to the expected one. The default initialisation is the identity\n"         \
"(parameters 0, 1, 1, 0, 0).\n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: geo\n"         \
"  geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__readgif__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__openingfunction__c__ "  openingfunction.c\n"         \
"\n"         \
" opening function\n"         \
"\n"         \
"Usage: openingfunction in.pgm [dist] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"This operator associates, to each point x of the input object X, \n"         \
"the radius of the biggest ball included in X that includes x.\n"         \
"The distance used depends on the optional parameter  dist (default is 0) :\n"         \
" 0: Euclidean distance (truncated)\n"         \
" 1: approximate quadratic Euclidean distance\n"         \
" 2: chamfer distance\n"         \
" 3: exact quadratic Euclidean distance\n"         \
" 4: 4-distance in 2d\n"         \
" 8: 8-distance in 2d\n"         \
" 6: 6-distance in 3d\n"         \
" 18: 18-distance in 3d\n"         \
" 26: 26-distance in 3d\n"         \
"\n"         \
" The input image  in.pgm must be a binary image. No test is done.\n"         \
"\n"         \
"Types supported: byte 2D, byte 3D\n"         \
"\n"         \
"Category: morpho\n"         \
"  morpho\n"         \
"\n"         \
" Michel Couprie 2009\n"         \
"\n"         \
"\n"         \
"\n"


#define doc__volume__c__ "  volume.c\n"         \
"\n"         \
" return the sum of the pixel values of an image\n"         \
"\n"         \
"Usage: volume in.pgm out.list\n"         \
"\n"         \
"Description:\n"         \
"This function returns (in the list out.list) \n"         \
"the sum of the pixel values of the image  in.pgm .\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n"         \
"\n"         \
"Category: arith\n"         \
"  arith\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__seuilhistolocal__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__ptseparatinggray__c__ "  ptseparatinggray.c\n"         \
"\n"         \
" detects separating points in a grayscale image\n"         \
"\n"         \
"Usage: ptseparatinggray in.pgm connex out.pgm\n"         \
"\n"         \
"Description:\n"         \
"A separating point is a point p, which is separating (in the binary sense)\n"         \
"for a section h of F, with h \n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: topogray\n"         \
"  topogray\n"         \
"\n"         \
" Michel Couprie 2002\n"         \
"\n"         \
"\n"


#define doc__hk_tophat_lin_rotall__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__volmaxima__c__ "  volmaxima.c\n"         \
"\n"         \
" volume-based filtering\n"         \
"\n"         \
"Usage: volmaxima in.pgm connex vol out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Volume-based filtering with connexity connex and volume vol.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: connect\n"         \
" connect\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__colorize__c__ "  colorize.c\n"         \
"\n"         \
" generates a color image from a grayscale image and a lookup table\n"         \
"\n"         \
"Usage: colorize in.pgm lut.ppm out.ppm\n"         \
"\n"         \
"Description:\n"         \
"Generates a color image from a grayscale image and a lookup table (see genlut.c).\n"         \
"\n"         \
"Types supported: byte 2D, long 2D\n"         \
"\n"         \
"Category: convert\n"         \
"  convert\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__skel_CKSC3__c__ "  skel_CKSC3.c\n"         \
"\n"         \
" sequential 3D binary curvilinear skeleton\n"         \
"\n"         \
"Usage: skel_CKSC3 in.pgm nsteps [inhibit] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Sequentl 3D binary thinning or curvilinear skeleton. The parameter  nsteps gives,\n"         \
"if positive, the number of parallel thinning steps to be processed.\n"         \
"If the value given for  nsteps equals -1, the thinning is continued\n"         \
"until stability.\n"         \
"\n"         \
"If the parameter  inhibit is given and is a binary image name,\n"         \
"then the points of this image will be left unchanged. \n"         \
"\n"         \
"Warning: The object must not have any point on the frame of the image.\n"         \
"\n"         \
"Types supported: byte 3d\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__dev_conventions__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__temp3dsimple__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__3dthin__c__ "  3dthin.c\n"         \
"\n"         \
" homotopic thinning of a 3d Khalimsky image\n"         \
"\n"         \
"Usage: 3dthin in.pgm n out.pgm\n"         \
"\n"         \
"Description: Homotopic thinning of a 3d Khalimsky image.\n"         \
"The maximum number of steps is given by n.\n"         \
"If the value given for  n equals -1, the thinning is continued\n"         \
"until stability.\n"         \
"\n"         \
"Types supported: byte 3d\n"         \
"\n"         \
"Category: orders\n"         \
"  orders\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__lfdilat3d_line__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__ecarttype__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__drawball__c__ "  drawball.c\n"         \
"\n"         \
" generates a binary euclidean ball\n"         \
"\n"         \
"Usage: drawball in.pgm radius xc yc zc out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Draws a binary euclidean ball in the image  in.pgm.\n"         \
"The parameter radius specifies the radius of the ball. \n"         \
"The parameters xc, yc, zc specify the center of the ball. \n"         \
"\n"         \
"Types supported: byte 3d\n"         \
"\n"         \
"Category: draw\n"         \
"  draw\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__inverse__c__ "  inverse.c\n"         \
"\n"         \
" computes the pixelwise inverse of an image\n"         \
"\n"         \
"Usage: inverse in.pgm [out.pgm]\n"         \
"\n"         \
"Description:\n"         \
"Byte images: for each pixel x, out[x] = 255 - in[x].\n"         \
"Long or float images: for each pixel x, out[x] = VMAX - in[x], where VMAX = max{in[x]}.\n"         \
"\n"         \
"If  out.pgm is not specified, then out.pgm = in.pgm.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d, long 2d, long 3d, float 2d, float 3d\n"         \
"\n"         \
"Category: arith\n"         \
"  arith\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__lantuejoul__c__ "  lantuejoul.c\n"         \
"\n"         \
" Lantuejoul's skeleton\n"         \
"\n"         \
"Usage: lantuejoul in.pgm [dist] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Let X be the set in  in.pgm .\n"         \
"The result is union{Si(X), i in N} where\n"         \
"Si(X) = erosball(X,i)  openball(erosball(X,i),1). \n"         \
"Structuring elements are balls defined after a distance.\n"         \
"The distance used depends on the optional parameter  dist (default is 0) :\n"         \
" 0: approximate euclidean distance (truncated)\n"         \
" 1: approximate quadratic euclidean distance\n"         \
" 2: chamfer distance\n"         \
" 3: exact quadratic euclidean distance\n"         \
" 4: 4-distance in 2d\n"         \
" 8: 8-distance in 2d\n"         \
" 6: 6-distance in 3d\n"         \
" 18: 18-distance in 3d\n"         \
" 26: 26-distance in 3d\n"         \
"\n"         \
" The input image  in.pgm must be a binary image. No test is done.\n"         \
"\n"         \
"Types supported: byte 2D, byte 3D\n"         \
"\n"         \
"Category: morpho\n"         \
"  morpho\n"         \
"\n"         \
" Michel Couprie aoÃ»t 2009\n"         \
"\n"         \
"\n"         \
"\n"


#define doc__regul___notused_c__ "WRITE ME!!!\n"         \
"\n"


#define doc__geodilat__c__ "  geodilat.c\n"         \
"\n"         \
" geodesic (grayscale or binary) dilation\n"         \
"\n"         \
"Usage: geodilat g.pgm f.pgm connex niter out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Geodesic dilation of g.pgm under f.pgm.\n"         \
"Let G and F be the two input images. If G is not under F, then G is replaced initially by min(G,F).\n"         \
"The structuring element is specified by the value of the parameter connex, \n"         \
"which can be one of the following ones: 4, 8 in 2d, or 6, 18, 26 in 3d.\n"         \
"The parameter  niter sets the number of iterations. If  niter = -1,\n"         \
"then the iterations continue until stability.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d, short 2d, short 3d, long 2d, long 3d.\n"         \
"\n"         \
"Category: connect, morpho\n"         \
"  connect, morpho\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__drawtriangulation__c__ "  drawtriangulation.c\n"         \
"\n"         \
" draw a triangulation\n"         \
"\n"         \
"Usage: drawtriangulation in.list in.pgm out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Reads a triangulation in file in.list under the following format:\n"         \
"\n"         \
"    G &lt;n&gt;\n"         \
"    x1 y1 v1 ec1 ns1 s11 s12 ... s1ns1\n"         \
"    x2 y2 v2 ec2 ns2 s21 s22 ... s1ns2\n"         \
"    ...\n"         \
"    xn yn vn ecn nsn sn1 sn2 ... s1nsn\n"         \
"  \n"         \
"where xi, yi are the coordinates of the ith vertex, vi is the associated \n"         \
"value (if given in the input file), eci is a int32_t which indicates \n"         \
"whether the vertex i belongs to the convex hull, nsi denotes the number\n"         \
"of adjacent vertices, and si1 si2 ... sins1 is the list of the indexes of\n"         \
"the adjacent vertices (counted from 0).\n"         \
"\n"         \
"Draws the triangulation in image in.pgm, output image is out.pgm\n"         \
"\n"         \
"Types supported: byte 2D\n"         \
"\n"         \
"See also: delaunay.c\n"         \
"\n"         \
"Category: geo, draw\n"         \
"  geo, draw\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__genplane__c__ "  genplane.c\n"         \
"\n"         \
" generates a plane normal to one of the directions x,y,z\n"         \
"\n"         \
"Usage: genplane rs cs ds dir n out.pgm\n"         \
"\n"         \
"Description:\n"         \
"The size of the result image out.pgm is given by the parameters \n"         \
"rs, cs, ds.\n"         \
"The parameter dir is set to x, y, or z.\n"         \
"The parameter n is an integer.\n"         \
"The output binary image out.pgm contains the plane defined by dir=n.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: geo\n"         \
"  geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__tracevect__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__toposhrinkgray__c__ "  toposhrinkgray.c\n"         \
"\n"         \
" topologically controled grayscale shrinking (one step)\n"         \
"\n"         \
"Usage: toposhrinkgray in.pgm connex t+min t+max t--min t--max {0|a|d} [inhibit] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Topologically controled grayscale shrinking (one step).\n"         \
"\n"         \
"The parameter  indicates the action performed on the selected point p:\n"         \
" 0: it is lowered down to 0;\n"         \
" a: it is lowered down to alpha-(p,F) where F denotes the original image;\n"         \
" d: it is lowered down to delta-(p,F) where F denotes the original image.\n"         \
"\n"         \
"The parameter  connex indicates the connectivity of the binary object.\n"         \
"Possible choices are 4, 8 in 2D and 6, 18, 26 in 3D.\n"         \
"\n"         \
"If the parameter  inhibit is given and is a binary image name,\n"         \
"then the points of this image (set Y) will be left unchanged. \n"         \
"\n"         \
"\n"         \
"  For all points p, not in Y, and such that \n"         \
"      t+min \n"         \
"    lower p according to the chosen strategy\n"         \
"\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: topogray\n"         \
"  topogray\n"         \
"\n"         \
"References:\n"         \
"\n"         \
"[BEC97] G. Bertrand, J. C. Everat and M. Couprie: \"Image segmentation through operators based upon topology\",  Journal of Electronic Imaging, Vol.&nbsp;6, No.&nbsp;4, pp.&nbsp;395-405, 1997.\n"         \
"\n"         \
"[CBB01] M. Couprie, F.N. Bezerra, Gilles Bertrand: \"Topological operators for\n"         \
"grayscale image processing\",  Journal of Electronic Imaging, Vol.&nbsp;10, No.&nbsp;4, pp.&nbsp;1003-1015, 2001.\n"         \
"\n"         \
" Nicolas Combaret 2006\n"         \
"\n"         \
"\n"


#define doc__lferode3d_line__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__profil__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__ptmultiple__c__ "  ptmultiple.c\n"         \
"\n"         \
" detects multiple points in 2d or 3d binary images\n"         \
"\n"         \
"Usage: ptmultiple in.pgm connex out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Detects multiple points in the 2d or 3d binary image  in.pgm .\n"         \
"A multiple point is a white point x such that Tn[x] > 2\n"         \
"(n = 4, 8 in 2D or n = 6, 18, 26 in 3D, as set by the parameter  connex).\n"         \
"\n"         \
"Types supported: byte 2D, byte 3D\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" Michel Couprie 2002\n"         \
"\n"         \
"\n"


#define doc__propgeo__c__ "  propgeo.c\n"         \
"\n"         \
" geodesic propagation of image values\n"         \
"\n"         \
"Usage: propgeo in.pgm mask.pgm connex mode out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Propagates certain values of the original image in.pgm, \n"         \
"geodesicaly wrt the connected components of the binary image mask.pgm.\n"         \
"The connexity is specified by the parameter connex.\n"         \
"The mode is one of the following ones:\n"         \
" 0 | min   : propagates the minimal value of the component\n"         \
" 1 | max   : propagates the maximal value of the component\n"         \
" 2 | moy   : propagates the mean grayscale value of the component\n"         \
" 3 | min1  : selects one point the value of which is equal to the min\n"         \
" 4 | max1  : selects one point the value of which is equal to the max\n"         \
" 5 | moy1  : selects one point the value of which is nearest to the mean\n"         \
" 6 | minb  : propagates the minimal value of the external border of the component\n"         \
" 7 | maxb  : propagates the maximal value of the external border of the component\n"         \
" 8 | moyb  : propagates the mean value of the external border of the component\n"         \
" 9 | randb : fills the component with random values, with a normal distribution centered around the value computed as for moyb\n"         \
"\n"         \
"Only modes min, max and moy are available for int32_t images.\n"         \
"\n"         \
"Types supported: byte 2d, int32_t 2d byte 3d, int32_t 3d\n"         \
"\n"         \
"Category: connect\n"         \
" connect\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__asfr__c__ "  asfr.c\n"         \
"\n"         \
" alternate sequential filter by reconstruction\n"         \
"\n"         \
"Usage: asfr in.pgm [rmin] rmax out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Alternate sequential filter by reconstruction. \n"         \
"Disc-shaped structuring elements of increasing radius,\n"         \
"ranging from 1 (or rmin if this parameter is specified) to rmax,\n"         \
"are generated. Let D1...Dn be these structuring elements, sorted by increasing radius.\n"         \
"Let I0 = in.pgm, the ith intermediate result Ii is obtained by the closure \n"         \
"by reconstruction of Ii-1 by the structuring element Di. \n"         \
"The result out.pgm contains the final result In.\n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: morpho\n"         \
"  morpho\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__histodist__c__ "  histodist.c\n"         \
"\n"         \
" computes the distance between two histograms\n"         \
"\n"         \
"Usage: histodist h1.list h2.list [type] out.list\n"         \
"\n"         \
"Description:\n"         \
"Computes the distance between the two histograms given in  h1.list and  h2.list. The result is written in  out.list.\n"         \
"\n"         \
"The distance is an edit distance, it is defined as the minimal number of elementary operations (moving one unit from a bin to the neighboring one) that is necessary to transform h1 into h2.\n"         \
"\n"         \
"Parameter  type (default 0) indicates whether the histogram must be interpreted as circular (type = 1) of not (type = 0). Circular interpretation implies that first and last bins are considered as neighbors.\n"         \
"\n"         \
"Types supported: list\n"         \
"\n"         \
"Category: histo\n"         \
"  histo\n"         \
"\n"         \
" Michel Couprie, Mohamed Amine Salem\n"         \
"\n"         \
"\n"


#define doc__expandframe__c__ "  expandframe.c\n"         \
"\n"         \
" expands the values of the image in a n-pixel wide band outside the image \n"         \
"\n"         \
"Usage: expandframe in.pgm n out.pgm\n"         \
"\n"         \
"Description: \n"         \
"Expands the values of the image in a n-pixel wide band outside the image .\n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: convert\n"         \
"  convert\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__sceneconvert__c__ "  sceneconvert.c\n"         \
"\n"         \
" converts a 3D scene into another format\n"         \
"\n"         \
"Usage: sceneconvert in.3sc format out\n"         \
"\n"         \
"Description:\n"         \
"The file  scene.3sc contains a scene under the 3Dscene format.\n"         \
"\n"         \
"The parameter  format indicate the format of the output file \n"         \
"(choices are POV, POVB).\n"         \
"The keyword POVB corresponds to a bare Povray mesh: a header and a footer must be\n"         \
"catenated in order to make a full Povray scene. \n"         \
"\n"         \
"Types supported: 3D scene\n"         \
"\n"         \
"Category: convert\n"         \
"  convert\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__pgm2ps__c__ "  pgm2ps.c\n"         \
"\n"         \
" converts from pgm to ps for illustrating small image configurations\n"         \
"\n"         \
"Usage: pgm2ps in.pgm mode {label.pgm|null} {marker.pgm|null} coord maxval out.ps\n"         \
"\n"         \
"Description:\n"         \
"Produces a Postscript file from a binary or grayscale image.\n"         \
"If mode = \n"         \
"     b: binary image\n"         \
"     c: grayscale image (levels not printed)\n"         \
"     n: grayscale image (levels printed as numbers on a colored background)\n"         \
"     m: grayscale image (levels printed as numbers on a colored background)\n"         \
"     d: grayscale image (idem n - levels are square-rooted)\n"         \
"     i: grayscale image (levels printed as numbers on an inverted colored background)\n"         \
"     p: grayscale image (levels printed as numbers)\n"         \
"     a: grayscale image (levels printed as letters: a=1, b=2...)\n"         \
"     g: grayscale image (levels showed as colored items)\n"         \
"     v: vector image\n"         \
"     B: binary khalimsky grid\n"         \
"     N: grayscale khalimsky grid (levels printed as numbers)\n"         \
"     G: grayscale khalimsky grid (levels showed as colored items)\n"         \
"     C: binary khalimsky grid (dual of B)\n"         \
"     M: grayscale khalimsky grid (dual of N)\n"         \
"     H: grayscale khalimsky grid (dual of G)\n"         \
"\n"         \
"If an image  label.pgm is present and the mode is n, the level number will be replaced by a letter for each pixel not null in the label image.\n"         \
"\n"         \
"If an image  label.pgm is present and the mode is m, the printed number will\n"         \
"taken in the image  label.pgm while the color will be taken from  in.pgm .\n"         \
"\n"         \
"If an image  marker.pgm is present, a circle will be drawn on each point not null in the marker image.\n"         \
"\n"         \
"The parameter  coord is a int32_t (0 or 1) which commands the inclusion of axis coordinates in the\n"         \
"figure. \n"         \
"\n"         \
"The parameter  maxval is an integer which indicates the maximum grayscale value of the figure\n"         \
"(not necessarily the maximum value of the input image).\n"         \
"\n"         \
"Types supported: byte 2d, int32_t 2d\n"         \
"\n"         \
"Category: convert\n"         \
"  convert\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__radialopening__c__ "  radialopening.c\n"         \
"\n"         \
" radial opening by line\n"         \
"\n"         \
"Usage: radialopening in.pgm length [angle in degrees] [dilatmask.pgm] out.pgm\n"         \
"\n"         \
"Description: \n"         \
"Radial opening by line (or dilated line). Max between the openings of\n"         \
"in.pgm by a linear structuring element rotated by angle until 360 degrees\n"         \
"and dilated by dilatmask.pgm. The angle is normalized to an exact division\n"         \
"of 360 by an integer.\n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: morpho\n"         \
"  morpho\n"         \
"\n"         \
" André Vital Saude jan 2005\n"         \
"\n"         \
"\n"         \
"\n"


#define doc__herosball__c__ "  herosball.c\n"         \
"\n"         \
" topologically controlled erosion\n"         \
"\n"         \
"Usage: herosball in.pgm radius dist connex out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Performs a topologically controlled erosion, that is, a homotopic thinning \n"         \
"constrained by the erosion of the input object.\n"         \
"\n"         \
"The parameter  radius gives the radius of the ball which is \n"         \
"used as structuring element for the erosion.\n"         \
"\n"         \
"The parameter  dist is a numerical code\n"         \
"indicating the distance which is used to compute the erosion.\n"         \
"The possible choices are:\n"         \
" 0: approximate euclidean distance\n"         \
" 1: approximate quadratic euclidean distance\n"         \
" 2: chamfer distance\n"         \
" 3: exact quadratic euclidean distance\n"         \
" 4: 4-distance in 2d\n"         \
" 8: 8-distance in 2d\n"         \
" 6: 6-distance in 3d\n"         \
" 18: 18-distance in 3d\n"         \
" 26: 26-distance in 3d\n"         \
"\n"         \
"The parameter  connex indicates the connectivity of the binary object.\n"         \
"Possible choices are 4, 8 in 2d and 6, 26 in 3d.\n"         \
"\n"         \
"Let X be the set corresponding to the input image  in.pgm, and let\n"         \
"Xbar denote its complementary set. The algorithm is the following:\n"         \
"\n"         \
"\n"         \
"I = erosball(X, dist, r)\n"         \
"Repeat:\n"         \
"    Select a point x in X  I such that dist(x,Xbar) is minimal\n"         \
"    If x is simple for X then\n"         \
"        X = X  {x}\n"         \
"	I = I union {x}\n"         \
"Until X == I\n"         \
"Result: X\n"         \
"\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"         \
"\n"


#define doc__any2complex__c__ "  any2complex.c\n"         \
"\n"         \
" converts a \"byte\", \"long\" of \"float\" image (or couple of images) to a \"fcomplex\" image\n"         \
"\n"         \
"Usage: any2complex re.pgm [im.pgm] out.pgm\n"         \
"\n"         \
"Description: \n"         \
"\n"         \
"Types supported: byte 2d, byte 3d, long 2d, long 3d, float 2d, float 3d.\n"         \
"\n"         \
"Category: convert\n"         \
" convert\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__compiling_cmake__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__complex_real__c__ "  complex_real.c\n"         \
"\n"         \
" pixelwise real part of complex\n"         \
"\n"         \
"Usage: complex_real in.pgm out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Applies to complex images only.\n"         \
"For each pixel p, out[p] = real(in[p]), where \n"         \
"real(x+iy) = x\n"         \
"\n"         \
"Types supported: complex 2d, complex 3d\n"         \
"\n"         \
"Category: arith\n"         \
"  arith\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__longestplateau__c__ "  longestplateau.c\n"         \
"\n"         \
" finds the location of the longest plateau in 1D sequence\n"         \
"\n"         \
"Usage: longestplateau in.list [tolerance] out.list\n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"Reads the sequence S from the file in.list. \n"         \
"This file must have the following format:\n"         \
"  \n"         \
"  e &lt;n&gt;\n"         \
"  x1\n"         \
"  x2\n"         \
"  ...\n"         \
"  xn\n"         \
"\n"         \
"The tolerance t (float) is given by the parameter tolerance\n"         \
"(default value is 0).\n"         \
"A plateau is a subsequence P of S formed by consecutive elements of P, \n"         \
"between indices i and j, and such that max{P[k];i\n"         \
"The program returns the base index and length of the first occurence of a plateau with \n"         \
"maximal length in S.\n"         \
"\n"         \
"Types supported: list 1D\n"         \
"\n"         \
"Category: signal\n"         \
" signal\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__python_export__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__labelfgd__c__ "  labelfgd.c\n"         \
"\n"         \
" labeling of the foreground components of a binary image\n"         \
"\n"         \
"Usage: labelfgd in.pgm connex out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Each connected component of  in.pgm is labeled with a unique integer, starting from 1. The background points are labeled by 0.\n"         \
"The argument  connex selects the connectivity (4, 8 in 2D; 6, 18, 26 in 3D).\n"         \
"The output image  out.pgm has the type \"int32_t\".\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d\n"         \
"\n"         \
"Category: connect\n"         \
"  connect\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__click3d__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__contours__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__pgm2vskel__c__ "  pgm2vskel.c\n"         \
"\n"         \
" decomposition of a valuated curvilinear skeleton into isolated points, end points, curves and junctions\n"         \
"\n"         \
"Usage: pgm2vskel in.pgm val.pgm connex out.skel\n"         \
"\n"         \
"Description:\n"         \
"The skeleton found in  in.pgm is decomposed into isolated points, \n"         \
"end points, curves and junctions ;\n"         \
"and its description is stored in  out.skel . \n"         \
"Each point of the skeleton is valuated by the corresponding value\n"         \
"found in  val.pgm .\n"         \
"The parameter  connex sets the adjacency relation used for the object\n"         \
"(4, 8 (2d) or 6, 18, 26 (3d)).\n"         \
"\n"         \
" No verification is done to check that the input image  in.pgm \n"         \
"contains indeed a curvilinear skeleton.\n"         \
"In the contrary case, the result would be meaningless.\n"         \
"\n"         \
" IMPORTANT LIMITATION: different junctions in the original image \n"         \
"must not be in direct contact with each other\n"         \
"(i.e., connected) otherwise they will be considered as a single junction. \n"         \
"To prevent this to occur, one can increase image resolution. \n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" Michel Couprie 2004\n"         \
"\n"         \
"\n"


#define doc__concat3d__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__crestrestoration__c__ "  crestrestoration.c\n"         \
"\n"         \
" crest restoration algorithm \n"         \
"\n"         \
"Usage: crestrestoration im.pgm {imcond.pgm|null} connex niter fileout.pgm [condout.pgm]\n"         \
"\n"         \
"Description:\n"         \
"Crest restoration algorithm, as described in ref. CBB01.\n"         \
"The input image  im.pgm must be a \"thin\" grayscale image, as\n"         \
"the output of the operator  hthin.\n"         \
"The parameter  connex indicates the connectivity used for the minimal \n"         \
"regions.\n"         \
"The parameter  niter gives the number of iterations.\n"         \
"The optional parameter  imcond.pgm is a binary image (a set C) which indicates the points\n"         \
"in the neighborhood of which the extensible points will be searched.\n"         \
"The points which are modified by the algorithm will be dynamically added to C.\n"         \
"The optional parameter  condout.pgm is an output file containing the final state of the \n"         \
"set C.\n"         \
"\n"         \
"Reference: \n"         \
"[CBB01] M. Couprie, F.N. Bezerra, Gilles Bertrand: \"Topological operators for\n"         \
"grayscale image processing\",  Journal of Electronic Imaging, Vol.&nbsp;10, No.&nbsp;4, pp.&nbsp;1003-1015, 2001.\n"         \
"\n"         \
"Types supported: byte 2D\n"         \
"\n"         \
"Category: topogray\n"         \
"  topogray\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__desfilssombres3d__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__comphisto__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__lvkern__c__ "  lvkern.c\n"         \
"\n"         \
" grayscale ultimate leveling thinning\n"         \
"\n"         \
"Usage: lvkern in.pgm {imcond.pgm|null} connex out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Grayscale ultimate leveling thinning (refs. [BEC97, CBB01]).\n"         \
"The parameter  connex gives the connectivity used for the minima;\n"         \
"possible choices are 4, 8 in 2D and 6, 26 in 3D.\n"         \
"Let F be the function corresponding to the input image  in.pgm.\n"         \
"Let G be the function corresponding to the input image \n"         \
" imcond.pgm, or the null function if the keyword  null is used.\n"         \
"The algorithm is the following:\n"         \
"\n"         \
"\n"         \
"Repeat until stability:\n"         \
"    select p such that T--(p,F) = 1 and such that F(p) is minimal\n"         \
"    F(p) = max{ delta-(p,F), G(p) }    \n"         \
"Result: F\n"         \
"\n"         \
"\n"         \
"References: \n"         \
"[BEC97] G. Bertrand, J. C. Everat and M. Couprie: \"Image segmentation through operators based upon topology\",  Journal of Electronic Imaging, Vol.&nbsp;6, No.&nbsp;4, pp.&nbsp;395-405, 1997.\n"         \
"[CBB01] M. Couprie, F.N. Bezerra, Gilles Bertrand: \"Topological operators for\n"         \
"grayscale image processing\",  Journal of Electronic Imaging, Vol.&nbsp;10, No.&nbsp;4, pp.&nbsp;1003-1015, 2001.\n"         \
"\n"         \
"Types supported: byte 2D, byte 3D.\n"         \
"\n"         \
"Category: topogray\n"         \
"  topogray\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"Example:\n"         \
"\n"         \
"lvkern ur1 null 4 ur1_lvkern\n"         \
"minima ur1_lvkern 4 ur1_lvkern_m\n"         \
"\n"         \
"\n"         \
"  \n"         \
"      \n"         \
"      \n"         \
"  \n"         \
"  ur1\n"         \
"      ur1_lvkern\n"         \
"      ur1_lvkern_m\n"         \
"  \n"         \
"\n"         \
"\n"         \
"\n"


#define doc__pow__c__ "  pow.c\n"         \
"\n"         \
" raises the value of each pixel to the pth power\n"         \
"\n"         \
"Usage: pow in.pgm p out.pgm\n"         \
"\n"         \
"Description:\n"         \
"For each pixel x, out[x] = in[x]^p. If the image is a byte image,\n"         \
"and if out[x] exceeds 255, then out[x] is set to 255.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n"         \
"\n"         \
"Category: arith\n"         \
"  arith\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__text___notused_c__ "WRITE ME!!!\n"         \
"\n"


#define doc__translate__c__ "  translate.c\n"         \
"\n"         \
" image translation \n"         \
"\n"         \
"Usage: in.pgm ox oy oz [mode] out.ppm\n"         \
"\n"         \
"Description:\n"         \
"Translates the image by the offsets  ox,  oy,  oz in the directions\n"         \
"x, y, z respectively. Offsets may be negative or positive integers.\n"         \
"If  mode is 0, the points that get out of the image frame are lost. \n"         \
"If  mode is 1, a torical space is simulated in all dimensions. \n"         \
"Default mode is 0.\n"         \
"\n"         \
"Types supported: byte 2d, byte3d\n"         \
"\n"         \
"Category: geo\n"         \
"  geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__lfclose3d_rect__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__bruite__c__ "  bruite.c\n"         \
"\n"         \
" adds noise to an image\n"         \
"\n"         \
"Usage: bruite in.pgm n alpha p out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Adds a uniform or gaussian noise, of mean 0 and standard deviation alpha, \n"         \
"to a proportion p of the pixels of the image in.pgm .\n"         \
"\n"         \
"  OPTIONS\n"         \
"    n = 0  uniform noise\n"         \
"    n = 1  gaussian noise\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: signal\n"         \
"  signal\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__reformat__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__houghcercles__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__fmm__c__ "  fmm.c\n"         \
"\n"         \
" fast marching method\n"         \
"\n"         \
"Usage: fmm speed.pgm seeds.pgm stop threshold seedout.pgm distanceout.pgm\n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"The fast marching method of J.A. Sethian is a geodesic distance transform.\n"         \
"It integrates the constant arrival hyperbolic PDE. A rough physical analogy\n"         \
"is the following: Assume an anisotropic medium with varying propagation\n"         \
"celerities, and waves starting from various seeds travelling through that\n"         \
"medium. This function computes the successive arrival times of these\n"         \
"waves, as well as the propagation of the initial labels. The successive\n"         \
"arrival times are equivalent to a distance transform. The propagation\n"         \
"of the label yield a partition similar to a Voronoi.\n"         \
"\n"         \
"If the speed function is constant and equal to 1, the arrival times would indeed\n"         \
"be the Euclidean distance function and the partition the Euclidean Voronoi.\n"         \
"Due to discretisation issue, the result is only approximately Euclidiean\n"         \
"(to second order).\n"         \
"\n"         \
"Stopping criteria:\n"         \
"stop is the stopping criteria :\n"         \
"\n"         \
" stop = 0 => no stop\n"         \
" stop = 1 => stop on metric (if speed function >= threshold)\n"         \
" stop = 2 => stop on distance (if distance > threshold)\n"         \
"\n"         \
"the threshold is given after.\n"         \
"\n"         \
"Types supported: integer, float Nd (N >= 2)\n"         \
"speed must be float, seeds must be integer.\n"         \
"\n"         \
"Category: morpho\n"         \
" morpho\n"         \
"\n"         \
" Hugues Talbot and Ben Appleton\n"         \
"\n"         \
"\n"


#define doc__rawthreshold__c__ "  rawthreshold.c\n"         \
"\n"         \
" converts from raw format into pgm format and applies a double threshold\n"         \
"\n"         \
"Usage: rawthreshold in.raw rs cs ds headersize datatype littleendian thresh1 thresh2 [xdim ydim zdim] out.pgm\n"         \
"\n"         \
"Description: Converts from raw format into pgm byte format, and selects only pixels which have values V such that thresh1 \n"         \
"\n"         \
"Parameters:\n"         \
"  in.pgm : source file in raw format\n"         \
"  rs (int32_t): row size (number of voxels in a row)\n"         \
"  cs (int32_t): column size (number of voxels in a column)\n"         \
"  ds (int32_t): number of planes\n"         \
"  headersize (int32_t): size of the header in bytes (information in the header will be ignored)\n"         \
"  datatype (int32_t): (1 for byte, 2 for short int, 4 for long int, 5 for float)\n"         \
"  littleendian (int32_t) 1: littleendian, 0: bigendian. Usual choice is 0.\n"         \
"  thresh1, thresh2 (float): threshold values\n"         \
"  xdim (float, optional) : gap (in the real world) between two adjacent voxels in a row.  \n"         \
"  ydim (float, optional) : gap (in the real world) between two adjacent voxels in a column.  \n"         \
"  zdim (float, optional) : gap (in the real world) between two adjacent planes.  \n"         \
"\n"         \
"This operator does not load the complete image into memory. \n"         \
"\n"         \
"Types supported: float\n"         \
"\n"         \
"Category: convert\n"         \
" convert\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__skeletonlab__c__ "  skeletonlab.c\n"         \
"\n"         \
" ultimate binary skeleton on label image guided by a priority image (see [BC07])\n"         \
"\n"         \
"Usage: skeletonlab in.pgm prio connex [inhibit] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Ultimate binary skeleton on label image guided by a priority image.\n"         \
"For simple point testing, each label is treated as if all other labels are turned to 0 (background).\n"         \
"For computing distance maps, all labels are turned to 1 (foregroung).\n"         \
"The lowest values of the priority image correspond to the highest priority.\n"         \
"\n"         \
"The parameter  prio is either an image (byte, int32_t, float or double), or a numerical code\n"         \
"indicating that a distance map will be used as a priority image; \n"         \
"the possible choices are:\n"         \
" 0: approximate euclidean distance\n"         \
" 1: approximate quadratic euclidean distance\n"         \
" 2: chamfer distance\n"         \
" 3: exact quadratic euclidean distance\n"         \
" 4: 4-distance in 2d\n"         \
" 8: 8-distance in 2d\n"         \
" 6: 6-distance in 3d\n"         \
" 18: 18-distance in 3d\n"         \
" 26: 26-distance in 3d\n"         \
"\n"         \
"The parameter  connex indicates the connectivity of the binary object.\n"         \
"Possible choices are 4, 8 in 2d and 6, 26 in 3d.\n"         \
"\n"         \
"If the parameter  inhibit is given and is an integer different from -1,\n"         \
"then the points which correspond to this priority value will be left unchanged. \n"         \
"If the parameter  inhibit is given and is a binary image name,\n"         \
"then the points of this image will be left unchanged. \n"         \
"\n"         \
"Let X be the set corresponding to the input image  in.pgm.\n"         \
"Let P be the function corresponding to the priority image.\n"         \
"Let I be the set corresponding to the inhibit image, if given, or the empty\n"         \
"set otherwise.\n"         \
"The algorithm is the following:\n"         \
"\n"         \
"\n"         \
"Repeat until stability\n"         \
"    Select a point x in X  I such that P[x] is minimal\n"         \
"    If x is simple for X then\n"         \
"        X = X  {x}\n"         \
"Result: X\n"         \
"\n"         \
"\n"         \
"Reference: \n"         \
"[BC07] G. Bertrand and M. Couprie: \"Transformations topologiques discretes\", in G&eacute;om&eacute;trie discr&egrave;te et images num&eacute;riques, D. Coeurjolly and A. Montanvert and J.M. Chassery, pp.&nbsp;187-209, Herm&egrave;s, 2007.\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__seuilauto__c__ "  seuilauto.c\n"         \
"\n"         \
" automatic threshold\n"         \
"\n"         \
"Usage: seuilauto in.pgm L {min|max} [n] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Determine automatically a threshold thanks to the smoothed histogram\n"         \
"of the picture ( L : smoothing coefficient, 0\n"         \
"If n is given, it indicates the number of peaks on the histogram to be kept.\n"         \
"If n isn't given, the threshold separates the picture in two approximatively equivalent groups of points.\n"         \
"The argument  selects the part of the picture\n"         \
"to be kept:\n"         \
"\n"         \
"  min: Every pixel   =  smax gets value  vmax \n"         \
"  max: Every pixel >  smin gets value  vmin and every pixel   \n"         \
"\n"         \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d\n"         \
"\n"         \
"Category: arith\n"         \
"  arith\n"         \
"\n"         \
" Cedric Allene 2003\n"         \
"\n"         \
"\n"


#define doc__ptisolated__c__ "  ptisolated.c\n"         \
"\n"         \
" detects isolated points in a binary image\n"         \
"\n"         \
"Usage: ptisolated in.pgm connex out.pgm\n"         \
"\n"         \
"Description:\n"         \
"An isolated point is a white point, all the n-neighbours of which are black\n"         \
"(n = 4, 8 (2d) or 6, 18, 26 (3d), as set by the parameter  connex)\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" Michel Couprie 2002\n"         \
"\n"         \
"\n"


#define doc__frame__c__ "  frame.c\n"         \
"\n"         \
" generates an image with a white border and a black interior\n"         \
"\n"         \
"Usage: frame in.pgm [width] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"The result out.pgm has the same size as in.pgm. Its border is set to 255, \n"         \
"all other pixels are set to 0.\n"         \
"\n"         \
"If the optional parameter  width is given, then the border has thickness 'width'.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: geo\n"         \
"  geo\n"         \
"\n"         \
" Michel Couprie 2000\n"         \
"\n"         \
"\n"


#define doc__erosnum__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__lvkernu__c__ "  lvkernu.c\n"         \
"\n"         \
" grayscale ultimate leveling thickening\n"         \
"\n"         \
"Usage: lvkernu in.pgm {imcond.pgm|null} connex out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Grayscale ultimate leveling thickening (refs. [BEC97, CBB01]).\n"         \
"The parameter  connex gives the connectivity used for the minima;\n"         \
"possible choices are 4, 8 in 2D and 6, 26 in 3D.\n"         \
"Let F be the function corresponding to the input image  in.pgm.\n"         \
"Let G be the function corresponding to the input image \n"         \
" imcond.pgm, or the blanck function (constant 255) if the keyword  null is used.\n"         \
"The algorithm is the following:\n"         \
"\n"         \
"\n"         \
"Repeat until stability:\n"         \
"    select p such that T++(p,F) = 1 and such that F(p) is maximal\n"         \
"    F(p) = min{ delta+(p,F), G(p) }    \n"         \
"Result: F\n"         \
"\n"         \
"\n"         \
"References: \n"         \
"[BEC97] G. Bertrand, J. C. Everat and M. Couprie: \"Image segmentation through operators based upon topology\",  Journal of Electronic Imaging, Vol.&nbsp;6, No.&nbsp;4, pp.&nbsp;395-405, 1997.\n"         \
"[CBB01] M. Couprie, F.N. Bezerra, Gilles Bertrand: \"Topological operators for\n"         \
"grayscale image processing\",  Journal of Electronic Imaging, Vol.&nbsp;10, No.&nbsp;4, pp.&nbsp;1003-1015, 2001.\n"         \
"\n"         \
"Types supported: byte 2D, byte 3D.\n"         \
"\n"         \
"Category: topogray\n"         \
"  topogray\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__eden__cxx__ "  eden.cxx\n"         \
"\n"         \
" growth and/or shrinking of an 2D binary image - Eden process\n"         \
"\n"         \
"Usage: eden in.pgm niter grow shrink topo out.pgm\n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"Algorithm is as follows.\n"         \
"\n"         \
"For growth:\n"         \
"   start from known set\n"         \
"   take pixels from the border into a queue\n"         \
"   choose randomly one such pixel\n"         \
"   set it to 1\n"         \
"   update queues\n"         \
"   repeat\n"         \
"\n"         \
"For shrinking:\n"         \
"   same thing but set to 0\n"         \
"\n"         \
"For adding boundary noise\n"         \
"   alternate growth and shrinking\n"         \
"\n"         \
"With topological constraints\n"         \
"   make sure the point to add or remove is simple\n"         \
"\n"         \
"Parameters:\n"         \
"   niter (positive integer): number of iterations\n"         \
"   grow (1/0): perform growing or not\n"         \
"   shrink (1/0): perform shrinking or not\n"         \
"   topo (26/6/8/4/0): connectivity for the object - 0: no topological constraint\n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: geo\n"         \
" geo\n"         \
"\n"         \
" Hugues Talbot\n"         \
"\n"         \
"\n"


#define doc__skel2graph__c__ "  skel2graph.c\n"         \
"\n"         \
" generation of a graph from a curvilinear skeleton\n"         \
"\n"         \
"Usage: skel2graph in.skel [mode [param]] out.graph\n"         \
"\n"         \
"Description:\n"         \
"Generation of a graph from a curvilinear skeleton.\n"         \
"\n"         \
"The parameter  mode has the following meaning (default is 0):\n"         \
"\n"         \
" 0: vertices of the graphs are elements (ends, curves, junctions, isolated points) of the skeleton\n"         \
"\n"         \
" 1: vertices of the graph are only ends, isolated points and junctions of the skeleton\n"         \
"\n"         \
" 2: each curve of the skeleton (not including its ends) is represented by N edges and N-1 vertices of the graph, where N is set to  param\n"         \
"\n"         \
" 3: each curve of the skeleton (not including its end) is represented by N edges and N-1 vertices of the graph, where N is set to max(1, trunc(len /  param)), where len is the number of points of the curve.\n"         \
"\n"         \
"Types supported: 2Dskel, 3Dskel\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" Michel Couprie 2009\n"         \
"\n"         \
"\n"


#define doc__variance1__c__ "  variance1.c\n"         \
"\n"         \
" return the variance of the pixel values of an image\n"         \
"\n"         \
"Usage: variance1 in.pgm [mask.pgm] out.list\n"         \
"\n"         \
"Description:\n"         \
"This function returns (in the list out.list) \n"         \
"the variance of the pixel values of the image  in.pgm .\n"         \
"If the optional parameter  mask.pgm is given, then only the \n"         \
"values which correspond to non-null points of mask are considered.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n"         \
"\n"         \
"Category: signal stats\n"         \
" signal stats\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__medianfilter__c__ "  medianfilter.c\n"         \
"\n"         \
" median filter \n"         \
"\n"         \
"Usage: medianfilter in.pgm el.pgm out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Let F be the input image, G be the output image, and E the structuring\n"         \
"element. \n"         \
"then for each pixel p, G[p] is the median element of the sorted list (by \n"         \
"increasing order) of the pixel values in the set { F[q], q in E[p] }.\n"         \
"\n"         \
"The median filter is a particular case of rank filter (see rankfilter.c), \n"         \
"with rank = 0.5 .\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: morpho\n"         \
" morpho\n"         \
"\n"         \
" Michel Couprie 1997\n"         \
"\n"         \
"\n"


#define doc__3dlabel__c__ "  3dlabel.c\n"         \
"\n"         \
" labels the theta-connected component of a 3D Khalimsky order\n"         \
"\n"         \
"Usage: 3dlabel in.pgm out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Labels the theta-connected component of a 3D Khalimsky order.\n"         \
"\n"         \
"Types supported: byte 3d\n"         \
"\n"         \
"Category: orders\n"         \
"  orders\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__2dborder__c__ "  2dborder.c\n"         \
"\n"         \
" extracts the border of an object in H2\n"         \
"\n"         \
"Usage: 2dborder in.pgm out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Extracts the border of an object in H2. The (internal) border is defined as the set \n"         \
"of the object points which have at least one neighboring background point. \n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: topo \n"         \
"  topo \n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__max1__c__ "  max1.c\n"         \
"\n"         \
" return the maximum of the pixel values of an image\n"         \
"\n"         \
"Usage: max1 in.pgm [out.list]\n"         \
"\n"         \
"Description:\n"         \
"This function returns (in the list out.list) \n"         \
"the maximum of the pixel values of the image  in.pgm .\n"         \
"\n"         \
"If the parameter  out.list is ommitted, the result is printed on the standard output.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n"         \
"\n"         \
"Category: arith\n"         \
"  arith\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__dilat__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__cropondisk__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__htkern__c__ "  htkern.c\n"         \
"\n"         \
" grayscale ultimate homotopic thinning\n"         \
"\n"         \
"Usage: htkern in.pgm {imcond.pgm|null} connex out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Grayscale ultimate homotopic thinning (refs. [BEC97, CBB01]).\n"         \
"The parameter  connex gives the connectivity used for the minima;\n"         \
"possible choices are 4, 8 in 2D and 6, 26 in 3D.\n"         \
"Let F be the function corresponding to the input image  in.pgm.\n"         \
"Let G be the function corresponding to the input image \n"         \
" imcond.pgm, or the null function if the keyword  null is used.\n"         \
"The algorithm is the following:\n"         \
"\n"         \
"\n"         \
"Repeat until stability:\n"         \
"    select p destructible for F such that F(p) is minimal\n"         \
"    F(p) = max{ delta-(p,F), G(p) }    \n"         \
"Result: F\n"         \
"\n"         \
"\n"         \
"References: \n"         \
"[BEC97] G. Bertrand, J. C. Everat and M. Couprie: \"Image segmentation through operators based upon topology\",  Journal of Electronic Imaging, Vol.&nbsp;6, No.&nbsp;4, pp.&nbsp;395-405, 1997.\n"         \
"[CBB01] M. Couprie, F.N. Bezerra, Gilles Bertrand: \"Topological operators for\n"         \
"grayscale image processing\",  Journal of Electronic Imaging, Vol.&nbsp;10, No.&nbsp;4, pp.&nbsp;1003-1015, 2001.\n"         \
"\n"         \
"Types supported: byte 2D, byte 3D.\n"         \
"\n"         \
"Category: topogray\n"         \
"  topogray\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"Example:\n"         \
"\n"         \
"htkern ur1 null 4 ur1_htkern\n"         \
"minima ur1_htkern 4 ur1_htkern_m\n"         \
"\n"         \
"\n"         \
"  \n"         \
"      \n"         \
"      \n"         \
"  \n"         \
"  ur1\n"         \
"      ur1_htkern\n"         \
"      ur1_htkern_m\n"         \
"  \n"         \
"\n"         \
"\n"         \
"\n"


#define doc__closing__c__ "  closing.c\n"         \
"\n"         \
" morphological closing by a plane structuring element\n"         \
"\n"         \
"Usage: closing in.pgm se.pgm out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Morphological closing by a plane structuring element.\n"         \
"The (plane) structuring element is given by the non-null values in  se.pgm, \n"         \
"its origin (wrt the point (0,0) of  se.pgm ) \n"         \
"is given by a comment line in the file se.pgm. \n"         \
"\n"         \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n"         \
"\n"         \
"Category: morpho\n"         \
"  morpho\n"         \
"\n"         \
" Michel Couprie 2002\n"         \
"\n"         \
"\n"         \
"\n"


#define doc__pgm2closedcurve__c__ "  pgm2closedcurve.c\n"         \
"\n"         \
" extracts a curve from a binary image\n"         \
"\n"         \
"Usage: pgm2closedcurve image.pgm connex [x y [z]] out.curve\n"         \
"\n"         \
"Description:\n"         \
"Extracts a closed curve from a binary image, that is, a curve with the\n"         \
"same starting and ending point.\n"         \
"The parameter  connex is the connexity of the curve.\n"         \
"It may be equal to 4 or 8 in 2D, and to 6, 18 or 26 in 3D.\n"         \
"If given, the point (x, y, z) (2D) or (x, y, z) (3D) \n"         \
"is taken as the starting point of the curve, and must be a curve point. \n"         \
"The output is the text file  out.curve, with the following format:\n"         \
"b nbpoints\n"         \
"x1 y1\n"         \
"x2 y2\n"         \
"...\n"         \
"or (3D): \n"         \
"B nbpoints\n"         \
"x1 y1 z1\n"         \
"x2 y2 z2\n"         \
"...\n"         \
"\n"         \
"The points of the curve may also be valued. This is must be indicated by \n"         \
"a value of 40, 80, 60, 180 or 260 for the parameter  connex, instead\n"         \
"of 4, 8, 6, 18 or 26 respectively. In this case,\n"         \
"the output is the text file  out.curve, with the following format:\n"         \
"n nbpoints\n"         \
"x1 y1 v1\n"         \
"x2 y2 v2\n"         \
"...\n"         \
"or (3D): \n"         \
"N nbpoints\n"         \
"x1 y1 z1 v1\n"         \
"x2 y2 z2 v2\n"         \
"...\n"         \
"\n"         \
"Types supported: byte 2D, byte 3D\n"         \
"\n"         \
"Category: convert geo\n"         \
"  convert geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__watershedMeyer3__c__ "  watershedMeyer3.c\n"         \
"\n"         \
" watershed transformation (Meyer's algorithm) with labelled markers and without separation lines\n"         \
"\n"         \
"Usage: watershedMeyer3 in mark {roi|null} connex out\n"         \
"\n"         \
"Description:\n"         \
"Performs the watershed transformation without separation lines, on the image in, taking the\n"         \
"set of markers in mark, in the form of a label image where all the pixels\n"         \
"sharing the same label (even if not connected) belong to the same marker.\n"         \
"If this parameter is present, roi\n"         \
"indicates the region of interest on which the operation is performed.\n"         \
"The parameter connex gives the adjacency relation (4,8 in 2D; 6,18,26 in 3D) \n"         \
"for the makers.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: connect\n"         \
" connect\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__drawsplines__c__ "  drawsplines.c\n"         \
"\n"         \
" draw spline segments which are specified by a text file\n"         \
"\n"         \
"Usage: drawsplines in.pgm splines.txt [len] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Draws splines which are specified by control points in a text file.\n"         \
"The parameter  in.pgm gives an image into which the splines are to be drawn.\n"         \
"The file format for  splines.txt is the following for 2D:\n"         \
"\n"         \
"The file  splines.txt contains a list of splines under the format:\n"         \
"d nb_splines\n"         \
"nb_points_spline_1  x11 y11  x12 y12 ...\n"         \
"nb_points_spline_2  x21 y21  x22 y22 ...\n"         \
"nb_points_spline_3  x31 y31  x32 y32 ...\n"         \
"...\n"         \
"or, in 3D:\n"         \
"D nb_splines\n"         \
"nb_points_spline_1  x11 y11 z11  x12 y12 z12 ...\n"         \
"nb_points_spline_2  x21 y21 z21  x22 y22 z22 ...\n"         \
"nb_points_spline_3  x31 y31 z31  x32 y32 z32 ...\n"         \
"...\n"         \
"\n"         \
"If parameter  len is given and non-zero, the splines are extended on both sides by straight line segments of length  len. \n"         \
"\n"         \
"Types supported: byte 2D, byte 3D\n"         \
"\n"         \
"Category: draw geo\n"         \
"  draw geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__ptseparating__c__ "  ptseparating.c\n"         \
"\n"         \
" detects separating points in a 2D or 3D binary image\n"         \
"\n"         \
"Usage: ptseparating in.pgm connex out.pgm\n"         \
"\n"         \
"Description:\n"         \
"A separating point is a white point x such that Tb_n[x] >= 2\n"         \
"(n = 4,8 in 2D or n = 6,18,26 in 3D, as set by the parameter  connex)\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d \n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" Michel Couprie 2002\n"         \
"\n"         \
"\n"


#define doc__bisector__c__ "  bisector.c\n"         \
"\n"         \
" compute the bisector function of a binary image\n"         \
"\n"         \
"Usage: bisector dist.pgm mask.pgm out.pgm\n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"The bisector function is computed for the points which are non-null in the binary image\n"         \
" mask.pgm , from the exact quadratic euclidean distance map found in  dist.pgm .\n"         \
"\n"         \
" If the image  dist.pgm is not an exact quadratic euclidean distance map, \n"         \
"then the result of this operator will make no sense. \n"         \
"\n"         \
"References: \n"         \
"[CCZ07] M. Couprie, D. Coeurjolly and R. Zrour: \"Discrete bisector function and Euclidean skeleton in 2D and 3D\", Image and Vision Computing, Vol.&nbsp;25, No.&nbsp;10, pp.&nbsp;1543-1556, 2007.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d \n"         \
"\n"         \
"Category: morpho\n"         \
"  morpho\n"         \
"\n"         \
"\n"         \
" Rita Zrour, Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__segbi___notused_c__ "WRITE ME!!!\n"         \
"\n"


#define doc__squelbin__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__GA2tikz__c__ "  GA2tikz.c\n"         \
"\n"         \
" converts from GA to latex for illustrating small image configurations\n"         \
"\n"         \
"Usage: GA2tikz in.ga [b] out.tikz\n"         \
"\n"         \
"Description:\n"         \
"Produces a LaTeX file from a binary or grayscale GA image.\n"         \
"If b is present, the result is binary otherwise the edges are greyscale colored.\n"         \
"\n"         \
"The result should be compiled with pdflatex to produce a pdf file.\n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: convert\n"         \
"  convert\n"         \
"\n"         \
" Laurent Najman\n"         \
"\n"         \
"\n"


#define doc__histopgm__c__ "  histopgm.c\n"         \
"\n"         \
" histogram of the gray values\n"         \
"\n"         \
"Usage: histopgm in.pgm [mask.pgm] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Generates a binary image which contains the graphical representation\n"         \
"of the histogram of  in.pgm (masked by the binary image\n"         \
" mask.pgm, if given).\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n"         \
"\n"         \
"Category: histo\n"         \
"  histo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__scale__c__ "  scale.c\n"         \
"\n"         \
" product of an image by a scalar\n"         \
"\n"         \
"Usage: scale in.pgm const out.pgm\n"         \
"\n"         \
"Description:\n"         \
"For each pixel x, out[x] = in[x] * const. If the image is a byte image,\n"         \
"and if out[x] exceeds 255, then out[x] is set to 255.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n"         \
"\n"         \
"Category: arith\n"         \
"  arith\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__stretch__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__MSF__c__ "  MSF.c\n"         \
"\n"         \
" \n"         \
"Compute a minimum spanning forest of a GA from a set of markers\n"         \
"\n"         \
"Usage: MSF GAin.ga marqueurs.pgm typeResul Result.?? \n"         \
"\n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"Compute a minimum spanning forest of GAin.ga (a 4-connected 2D\n"         \
"or 6-connected 3D edge-weighted graph) relative to the connected\n"         \
"components of the non-zero pixels of marqueurs.pgm. If\n"         \
"TypeResult = 0, then Result.?? is a GA whose non-zero edges form the\n"         \
"induced MSF cut, otherwise Result.?? is a label image that represents\n"         \
"the vertex partition induced by the MSF (the connected components of\n"         \
"the MSF are marked with distinct labels).\n"         \
"\n"         \
"See [COUSTYetAl-PAMI2009] and [COUSTYetAl-PAMI2010] for more details.\n"         \
"\n"         \
"Types supported: GA byte 2D, GA byte 3D\n"         \
"\n"         \
"Category: \n"         \
"  GA\n"         \
"\n"         \
" Jean Cousty\n"         \
"\n"         \
"\n"


#define doc__area__c__ "  area.c\n"         \
"\n"         \
" return the number of non-null pixels of an image\n"         \
"\n"         \
"Usage: area in.pgm [out.list]\n"         \
"\n"         \
"Description:\n"         \
"This function returns (in the list out.list) \n"         \
"the number of non-null pixels of the image  in.pgm .\n"         \
"\n"         \
"If the parameter  out.list is ommitted, the result is printed on the standard output.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n"         \
"\n"         \
"Category: arith\n"         \
"  arith\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__drawgraph__c__ "  drawline.c\n"         \
"\n"         \
" draw a graph in a 2D image\n"         \
"\n"         \
"Usage: drawline in.graph in.pgm out.pgm\n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"Draws the graph described in file in.graph into the image in.pgm\n"         \
"\n"         \
"Types supported: byte 2D\n"         \
"\n"         \
"Category: draw\n"         \
"  draw\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__crestvol__c__ "  crestvol.c\n"         \
"\n"         \
" enhances linear structures in a grayscale image\n"         \
"\n"         \
"Usage: crestvol in.pgm connex radius out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Enhances linear structures in a grayscale image.\n"         \
"The algorithm is the following:\n"         \
"\n"         \
"\n"         \
"Let F be the original image (\"in.pgm\")\n"         \
"Let O be the final result (\"out.pgm\")\n"         \
"Let r be the value of parameter \"radius\"\n"         \
"Compute k iterations of homotopic thinning, result S\n"         \
"Lower all separating points p in S down to alpha8m(S, p), result Sp \n"         \
"For each point x of S which is separating\n"         \
"  Let Dx be the image with D(x) = F(x), and D(y) = 0 for all y != x\n"         \
"  Let Fx be the geodesic reconstruction of Dx under F \n"         \
"  O(x) = volume((Fx-Sp).Br(x)) \n"         \
"  where Br(x) is the disc centered on x with radius r\n"         \
"EndFor\n"         \
"\n"         \
"\n"         \
"References: \n"         \
"\"Topological operators for the detection of\n"         \
"curvilinar structures in grayscale images\", in preparation.\n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: topogray\n"         \
"  topogray\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__t8pp__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__offset__c__ "  offset.c\n"         \
"\n"         \
" image translation \n"         \
"\n"         \
"Usage: in.pgm ox oy oz [mode] out.ppm\n"         \
"\n"         \
"Description:\n"         \
"Translates the image by the offsets  ox,  oy,  oz in the directions\n"         \
"x, y, z respectively. Offsets may be negative or positive integers.\n"         \
"If  mode is 0, the points that get out of the image frame are lost. \n"         \
"If  mode is 1, a torical space is simulated in all dimensions. \n"         \
"Default mode is 0.\n"         \
"\n"         \
"Types supported: byte 2d, byte3d\n"         \
"\n"         \
"Category: geo\n"         \
"  geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__nbtopo__c__ "  nbtopo.c\n"         \
"\n"         \
" connectivity numbers\n"         \
"\n"         \
"Usage: nbtopo filein.pgm connex {PP|P|M|MM} fileout.pgm\n"         \
"\n"         \
"Description:\n"         \
"For each point p of the input grayscale image, compute the connectivity number T++,\n"         \
"T+, T- or T-- according to the given option (resp. PP, P, M, MM).\n"         \
"Refs: [BEC97, CBB01].\n"         \
"\n"         \
"Types supported: byte 2D, byte 3D\n"         \
"\n"         \
"Category: topogray\n"         \
"  topogray\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__filtreordre__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__lfdilat_rect__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__watershedwithoutlinelab__c__ "  watershedwithoutlinelab.c\n"         \
"\n"         \
" watershed transformation without line of separation from labelled marker\n"         \
"\n"         \
"Usage: watershedwithoutlinelab in mark {roi|null} connex out\n"         \
"\n"         \
"Description:\n"         \
"Performs the watershed transformation on the image in, taking the\n"         \
"labelled marker in mark. \n"         \
"If this parameter is present, roi\n"         \
"indicates the region of interest on which the operation is performed.\n"         \
"The parameter connex gives the adjacency relation (4,8 in 2D; 6,18,26 in 3D) \n"         \
"for the makers.\n"         \
"\n"         \
"The image mark is a label image (int32_t)\n"         \
"\n"         \
"The image out is a label image (int32_t)\n"         \
"\n"         \
"This version does not create any line to separate the catchment basins. \n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: connect\n"         \
" connect\n"         \
"\n"         \
" Michel Couprie and Laurent Najman\n"         \
"\n"         \
"\n"


#define doc__list2pgm__c__ "  list2pgm.c\n"         \
"\n"         \
" converts from point list representation to pgm\n"         \
"\n"         \
"Usage: list2pgm in.list {in.pgm|r\n"         \
"\n"         \
"\n"         \
"s cs ds} [scale] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"Reads the file in.list. This file must have one of the following formats:\n"         \
"  \n"         \
"  e &lt;n&gt;       s &lt;n&gt;         b &lt;n&gt;         n &lt;n&gt;            B &lt;n&gt;            N &lt;n&gt;    \n"         \
"  x1          x1 v1         x1 y1         x1 y1 v1         x1 y1 z1         x1 y1 z1 v1\n"         \
"  x2    or    x2 v2   or    x2 y2   or    x2 y2 v2   or    x2 y2 z2   or    z2 x2 y2 v2\n"         \
"  ...         ...           ...           ...              ...              ...\n"         \
"  xn          xn vn         xn yn         xn yn vn         xn yn zn         zn xn yn vn\n"         \
"\n"         \
"The formats e, s, b, n, B, and N, correspond respectively\n"         \
"to binary 1D, graylevel 1D, binary 2D, graylevel 2D, binary 3D and graylevel 3D images.\n"         \
"If a file name in.pgm is given, then the points read in in.list are \n"         \
"inserted (if possible) into the image read in in.pgm. Else, they are inserted in\n"         \
"a new image, the dimensions of which are rs, cs, and d.\n"         \
"\n"         \
"The optional parameter  scale allows to scale the coordinates.\n"         \
"\n"         \
"Types supported: list 1D, 2D, 3D\n"         \
"\n"         \
"Category: convert\n"         \
"  convert\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__points2spline__c__ "  points2spline.c\n"         \
"\n"         \
" converts a sequence of control points into a spline\n"         \
"\n"         \
"Usage: points2spline points.txt spline.txt\n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"Let S be the sequence of control points in points.txt. This program computes\n"         \
"a parametric curve P defined by two (in 2D) or three (in 3D) splines \n"         \
"that interpolates the points of the sequence S.\n"         \
"\n"         \
"If the last point equals the first point, then a closed spline is generated.\n"         \
"\n"         \
"The result is given in the form of the list of control points, followed by\n"         \
"the set of coefficients for each spline segment. \n"         \
"\n"         \
"The input file  points.txt contains a list of points under the format:\n"         \
"\n"         \
"b nbpoints  \n"         \
"x1 y1\n"         \
"x2 y2\n"         \
"x3 y3\n"         \
"...\n"         \
"\n"         \
"or:\n"         \
"\n"         \
"B nbpoints  \n"         \
"x1 y1 z1\n"         \
"x2 y2 z2\n"         \
"x3 y3 z3\n"         \
"...\n"         \
"\n"         \
"The output file format is the following for 2D:\n"         \
"\n"         \
"c n+1 (where n+1 denotes the number of control points)\n"         \
"x1 y1\n"         \
"...\n"         \
"xn+1 yn+1\n"         \
"C0X1 C0Y1 C1X1 C1Y1 C2X1 C2Y1 C3X1 C3Y1\n"         \
"...\n"         \
"C0Xn C0Yn C1Xn C1Yn C2Xn C2Yn C3Xn C3Yn\n"         \
"\n"         \
"and in the 3D case:\n"         \
"\n"         \
"C n+1 (where n+1 denotes the number of control points)\n"         \
"x1 y1 z1\n"         \
"...\n"         \
"xn+1 yn+1 zn+1\n"         \
"C0X1 C0Y1 C0Z1 C1X1 C1Y1 C1Z1 C2X1 C2Y1 C2Z1 C3X1 C3Y1 C3Z1\n"         \
"...\n"         \
"C0Xn C0Yn C0Zn C1Xn C1Yn C1Zn C2Xn C2Yn C2Zn C3Xn C3Yn C3Zn\n"         \
"\n"         \
"The ith segment (starting from i=0) of the parametric curve P is then defined by:\n"         \
"\n"         \
"x(t) = C3Xi * t^3 + C2Xi * t^2 + C1Xi * t + C0Xi\n"         \
"y(t) = C3Yi * t^3 + C2Yi * t^2 + C1Yi * t + C0Yi\n"         \
"z(t) = C3Zi * t^3 + C2Zi * t^2 + C1Zi * t + C0Zi\n"         \
"with t in [i,i+1]\n"         \
"\n"         \
"Types supported: list 2D, list 3D\n"         \
"\n"         \
"Category: draw geo\n"         \
"  draw geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__meanfilter__c__ "  meanfilter.c\n"         \
"\n"         \
" Mean filter operator\n"         \
"\n"         \
"Usage: meanfilter in.pgm roi.pgm niter [inhibit.pgm] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Let F be the function stored in image  in.pgm.\n"         \
"Let R be the function stored in image  roi.pgm (region of interest).\n"         \
"Let I be the function stored in image  inhibit.pgm (if any).\n"         \
"The algorithm is the following:\n"         \
"\n"         \
"\n"         \
"Repeat niter times\n"         \
"  G = F\n"         \
"  For each image point x\n"         \
"    If R[x] and not I[x] then\n"         \
"      S = G[x]; C = 1;\n"         \
"      For each neighbour y of x\n"         \
"        If R[y] then S = S + G[y]; C = C + 1;\n"         \
"      F[x] = S / C;\n"         \
"Result: F\n"         \
"\n"         \
"\n"         \
"Types supported: byte 2d, int32_t 2d, float 2d, byte 3d, int32_t 3d, float 3d\n"         \
"\n"         \
"Category: signal\n"         \
"  signal\n"         \
"\n"         \
" Marcin Janaszewski, Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__areaselnb__c__ "  areaselnb.c\n"         \
"\n"         \
" area based segmentation by selection of a given number or components\n"         \
"\n"         \
"Usage: areaselnb in.pgm connex nb out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Segmentation by selection of a given number or components, based on an area criterion. \n"         \
"Connexity = connex ; desired number of components = nb.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: connect\n"         \
" connect\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__holeclosing__c__ "  holeclosing.c\n"         \
"\n"         \
" 3d hole closing\n"         \
"\n"         \
"Usage: holeclosing in connex holesize [guide] out\n"         \
"\n"         \
"Description: \n"         \
"Hole closing in 3d binary images. \n"         \
"The parameter  connex gives the connectivity used for the object;\n"         \
"possible choices are 6 and 26.\n"         \
"Holes which have a \"size\" greater (strictly) than  holesize are let open \n"         \
"(where -1 is used as a symbol for infinity).\n"         \
"\n"         \
"Let X be the set of points of the binary image  in, let Y be a full enclosing box. \n"         \
"The algorithm is the following:\n"         \
"\n"         \
"\n"         \
"Repeat until stability:\n"         \
"    Select a point p of Y  X such that Tb(p,Y) = 1\n"         \
"        or such that Tb(p,Y) = 2 and d(p,X) > holesize\n"         \
"        which is at the greatest distance from X\n"         \
"    Y := Y  {p}\n"         \
"Result: Y\n"         \
"\n"         \
"\n"         \
"If the optional parameter  guide is given, then replace 'greatest distance from X' by \n"         \
"'greatest distance from Y' in the algorithm, Y being the set of non-null points of image  guide.\n"         \
"\n"         \
" There must be no object point on the border of the image (a test is done).\n"         \
"\n"         \
"Reference: \n"         \
"Z. Aktouf, G. Bertrand, L.Perroton: \n"         \
"\"A three-dimensional holes closing algorithm\",\n"         \
"Pattern Recognition Letters, No.23, pp.523-531, 2002.\n"         \
"\n"         \
"Types supported: byte 3d\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__2dcolor__c__ "  2dcolor.c\n"         \
"\n"         \
" gives a specific color to points in a 2D khalimsky grid, \n"         \
"according to their rank\n"         \
"\n"         \
"Usage: 2dcolor in.pgm out.pgm\n"         \
"\n"         \
"Description:\n"         \
"gives a specific color to points in a 2D khalimsky grid, \n"         \
"according to their rank\n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: orders\n"         \
"  orders\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__temp3d___notused_c__ "WRITE ME!!!\n"         \
"\n"


#define doc__click2d__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__bisector_talbot__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__skel_SK3__c__ "  skel_SK3.c\n"         \
"\n"         \
" parallel 3D binary surface skeleton based on 2D isthmuses\n"         \
"\n"         \
"Usage: skel_SK3 in.pgm nsteps [inhibit] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Parallel 3D binary thinning or surface skeleton based on 2D isthmuses. \n"         \
"The parameter  nsteps gives,\n"         \
"if positive, the number of parallel thinning steps to be processed.\n"         \
"If the value given for  nsteps equals -1, the thinning is continued\n"         \
"until stability.\n"         \
"\n"         \
"If the parameter  inhibit is given and is a binary image name,\n"         \
"then the points of this image will be left unchanged. \n"         \
"\n"         \
"Warning: The object must not have any point on the frame of the image.\n"         \
"\n"         \
"Types supported: byte 3d\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__setorigin__c__ "  setorigin.c\n"         \
"\n"         \
" adds origin to file header\n"         \
"\n"         \
"Usage: setorigin in.pgm [source_se | xorig yorig zorig] \n"         \
"\n"         \
"Description: Adds origin (the one of  se or  xorig  yorig  zorig ) \n"         \
"to file header of  in.pgm.\n"         \
"\n"         \
"Types supported: byte 3d, int32_t 3d\n"         \
"\n"         \
"Category: convert\n"         \
" convert\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__zoomrgb__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__cadre__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__proj3d__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__genball__c__ "  genball.c\n"         \
"\n"         \
" generates a binary euclidean ball\n"         \
"\n"         \
"Usage: genball radius [dim] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Draws a binary euclidean ball in the new image  out.pgm.\n"         \
"The parameter radius specifies the radius of the ball. \n"         \
"The parameter dim specifies the dimension: 2 (default) or 3.\n"         \
"The center of the ball is designated as the origin (for use as structuring element).\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: draw\n"         \
"  draw\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__circleincl__c__ "  circleincl.c\n"         \
"\n"         \
" circle identification and drawing from spare points\n"         \
"\n"         \
"Usage: circleincl in.pgm pts.list filled out.pgm\n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: geo\n"         \
"  geo\n"         \
"\n"         \
" John Chaussard\n"         \
"\n"         \
"\n"


#define doc__lambdalthin__c__ "  lambdalthin.c\n"         \
"\n"         \
" grayscale filtered leveling\n"         \
"\n"         \
"Usage: lambdalthin in.pgm {imcond.pgm|null} connex niter h out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Filtered leveling for 2D grayscale images. \n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: topogray\n"         \
"  topogray\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__sym__c__ "  sym.c\n"         \
"\n"         \
" symmetry (vertical, horizontal, central in 2D; x, y, z in 3D) \n"         \
"\n"         \
"Usage: sym in.pgm type out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Symmetry (vertical, horizontal, central in 2D; x, y, z in 3D).\n"         \
"The parameter type is set to v or y (vertical), \n"         \
"h or x (horizontal), or c (central) for 2D images, \n"         \
"or to x, y or z for 3D images.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d.\n"         \
"\n"         \
"Category: geo\n"         \
"  geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__rotatebin__c__ "  rotatebin.c\n"         \
"\n"         \
" rotation of an image\n"         \
"\n"         \
"Usage: rotatebin in.pgm theta [x y] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"rotates the image  in.pgm of the angle  theta (in degrees).\n"         \
"Method: truncated real rotation.\n"         \
"If  x and  y are given, then the center of the rotation is the point \n"         \
"(x,y) and the image size is left unchanged (hence parts of object \n"         \
"may be lost). \n"         \
"Otherwise, the center of the rotation is the point (0,0) and the resulting \n"         \
"image size is computed such that no loss of information occur.\n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: geo\n"         \
"  geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__showpoint__c__ "  showpoint.c\n"         \
"\n"         \
" prints the value of a given point \n"         \
"\n"         \
"Usage: showpoint in.pgm x y z\n"         \
"\n"         \
"Description:\n"         \
"The value of the point (x,y,z)\n"         \
"is printed in the standard output stream.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n"         \
"\n"         \
"Category: convert\n"         \
"  convert\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__ppmascmode__c__ "  ppmascmode.c\n"         \
"\n"         \
" converts a ppm file to ascii mode\n"         \
"\n"         \
"Usage: ppmascmode filename [outfilename]\n"         \
"\n"         \
"Description: Converts a ppm file to ascii mode.\n"         \
"If only parameter  filename is given, then the result\n"         \
"is also named  filename .\n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: convert\n"         \
"  convert\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__laplacian__c__ "  laplacian.c\n"         \
"\n"         \
" Laplacian operator\n"         \
"\n"         \
"Usage: laplacian in.pgm alpha out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Deriche's recursive implementation of the Laplacian\n"         \
"operator. The parameter \n"         \
" alpha (double) controls the spatial extension of the\n"         \
"filter: 0 \n"         \
"\n"         \
"Types supported: byte 2D\n"         \
"\n"         \
"Category: signal\n"         \
"  signal\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__segment__c__sav__ "WRITE ME!!!\n"         \
"\n"


#define doc__standarddeviation1__c__ "  standarddeviation1.c\n"         \
"\n"         \
" return the standard deviation of the pixel values of an image\n"         \
"\n"         \
"Usage: standarddeviation1 in.pgm [mask.pgm] out.list\n"         \
"\n"         \
"Description:\n"         \
"This function returns (in the list out.list) \n"         \
"the standard deviation of the pixel values of the image  in.pgm .\n"         \
"If the optional parameter  mask.pgm is given, then only the \n"         \
"values which correspond to non-null points of mask are considered.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n"         \
"\n"         \
"Category: arith\n"         \
"  arith\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__ptjunction__c__ "  ptjunction.c\n"         \
"\n"         \
" detects junction points in 2d or 3d binary images\n"         \
"\n"         \
"Usage: ptjunction in.pgm connex out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Detects junction points in the 2d or 3d binary image  in.pgm, which is supposed to contain a skeleton.\n"         \
"A junction point is a white point x such that #(Nn[x] inter X) > 2, where Nn[x] stands for the n-neighborhood of x (excluding x), and n = 4, 8 in 2D or n = 6, 18, 26 in 3D, as set by the parameter  connex.\n"         \
"\n"         \
"When the type of  in.pgm is 4_BYTE, the image is treated as a label image, where each label is processed as a separate binary image (all other labels are considered as background).\n"         \
"\n"         \
"Types supported: byte 2D, byte 3D, long 3D\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" Michel Couprie 2010\n"         \
"\n"         \
"\n"


#define doc__graph2pgm__c__ "  graph2pgm.c\n"         \
"\n"         \
" converts from graph representation to pgm\n"         \
"\n"         \
"Usage: graph2pgm in.graph {in.pgm|rs cs ds} out.pgm\n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"Reads the file in.graph. Each vertex of this graph must have integer coordinates, and \n"         \
"represents a pixel/voxel of a 2D/3D image.\n"         \
"If a file name in.pgm is given, then the points read in in.graph are \n"         \
"inserted (if possible) into the image read in in.pgm. Else, they are inserted in\n"         \
"a new image, the dimensions of which are rs, cs, and ds.\n"         \
"\n"         \
"Types supported: graph\n"         \
"\n"         \
"Category: convert\n"         \
"  convert\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__histo__c__ "  histo.c\n"         \
"\n"         \
" computes the histogram of an image or a region\n"         \
"\n"         \
"Usage: histo in.pgm [mask.pgm] out.list\n"         \
"\n"         \
"Description:\n"         \
"Calculates the histogram of  im.pgm (masked by the binary image\n"         \
" mask.pgm, if given) and saves it in file  out.list .\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n"         \
"\n"         \
"Category: histo\n"         \
"  histo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__proj3d__c__new__ "WRITE ME!!!\n"         \
"\n"


#define doc__recalagerigide_translateplane__c__ "  recalagerigide.c\n"         \
"\n"         \
" rigid registration of two closed contours\n"         \
"\n"         \
"Usage: recalagerigide in1 in2 out\n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"Rigid registration of two closed contours. \n"         \
"Let X and Y be two point sets, given respectively by  in1 and  in2.\n"         \
"This procedure identifies the parameters of a rigid deformation R such that\n"         \
"the \"distance\" between R(X) and Y is a local minimum.\n"         \
"The distance D(Z,Y) between two sets Z and Y is defined by:\n"         \
"\n"         \
"D(Z,Y) = sum { d2(z,Y) ; for all z in Z }\n"         \
"\n"         \
"d2(z,Y) = min { d2(z,y) ; for all y in Y }\n"         \
"\n"         \
"d2(z,y) = (z - y)^2 ; i.e., the square or the Euclidean distance between z and y.\n"         \
"\n"         \
"The rigid deformation R is defined as the composition (in this order) \n"         \
"of scalings, rotations and translations. \n"         \
"\n"         \
"In 2d, the parameters are:\n"         \
" sx : parameter for the scaling in direction x\n"         \
" sy : parameter for the scaling in direction y\n"         \
" theta : angle (in radians) of the rotation around the barycenter of X\n"         \
" tx : parameter for the translation in direction x\n"         \
" ty : parameter for the translation in direction y\n"         \
"\n"         \
"In 3d, the parameters are:\n"         \
" sx : parameter for the scaling in direction x\n"         \
" sy : parameter for the scaling in direction y\n"         \
" sz : parameter for the scaling in direction z\n"         \
" theta : angle (in radians) of the rotation around the parallel to the the z axis passing by the barycenter of X\n"         \
" phi : angle (in radians) of the rotation around the parallel to the the y axis passing by the barycenter of X\n"         \
" tx : parameter for the translation in direction x\n"         \
" ty : parameter for the translation in direction y\n"         \
" tz : parameter for the translation in direction z\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: geo\n"         \
"  geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__lpemeyer3d___notused_c__ "WRITE ME!!!\n"         \
"\n"


#define doc__despuits__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__2dthin__c__ "  2dthin.c\n"         \
"\n"         \
" homotopic thinning of a 2d Khalimsky image\n"         \
"\n"         \
"Usage: 2dthin in.pgm n out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Homotopic thinning of a 2d Khalimsky image.\n"         \
"The maximum number of steps is given by n.\n"         \
"If the value given for  n equals -1, the thinning is continued\n"         \
"until stability.\n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: orders\n"         \
"  orders\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__selndg__c__ "  selndg.c\n"         \
"\n"         \
" selects pixels with values between two thresholds \n"         \
"\n"         \
"Usage: selndg in.pgm inf sup out.ppm\n"         \
"\n"         \
"Description: \n"         \
"Selects pixels x such that  inf  \n"         \
"If the input image is of type byte, the output is a binary image.\n"         \
"If the input image is of type long, the output is also a long image\n"         \
"where all pixels whose values are not in the interval, are put to 0.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d, long 2d, long 3d\n"         \
"\n"         \
"Category: arith\n"         \
"  arith\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__nbvois__c__ "  nbvois.c\n"         \
"\n"         \
" number of neighbors for each object point\n"         \
"\n"         \
"Usage: nbvois in connex out\n"         \
"\n"         \
"Description:\n"         \
"Let X be the set of object points of the binary image  in .\n"         \
"For each point x of X,\n"         \
" out (x) = card( gamma(x) inter X ). \n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__watershedwithoutline__c__ "  watershedwithoutline.c\n"         \
"\n"         \
" watershed transformation without line of separation\n"         \
"\n"         \
"Usage: watershedwithoutline in mark {bgmark|null} {roi|null} connex out\n"         \
"\n"         \
"Description:\n"         \
"Performs the watershed transformation on the image in, taking the\n"         \
"set of markers in mark. \n"         \
"If this parameter is present, bgmark\n"         \
"is used as a set of markers for the background.\n"         \
"If this parameter is present, roi\n"         \
"indicates the region of interest on which the operation is performed.\n"         \
"The parameter connex gives the adjacency relation (4,8 in 2D; 6,18,26 in 3D) \n"         \
"for the makers.\n"         \
"\n"         \
"The image out is a label image (int32_t)\n"         \
"\n"         \
"This version does not create any line to separate the catchment basins. \n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: connect\n"         \
" connect\n"         \
"\n"         \
" Michel Couprie and Laurent Najman\n"         \
"\n"         \
"\n"


#define doc__2dpardircollapse__c__ "  2dpardircollapse.c\n"         \
"\n"         \
" parallel directional constrained collapse\n"         \
"\n"         \
"Usage: 2dpardircollapse in.pgm nsteps [inhibit] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Parallel directional constrained collapse.\n"         \
"\n"         \
"If the parameter  inhibit is given and is a binary image name,\n"         \
"then the points of this image will be left unchanged. \n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: orders\n"         \
"  orders\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__labeltree__c__ "  labeltree.c\n"         \
"\n"         \
" branch distance\n"         \
"\n"         \
"Usage: labeltree in.pgm connex out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Level of the branch section defined by his level in the tree. Returns the level\n"         \
"max of the branches.\n"         \
"\n"         \
"The distance d used depends on the parameter  connex :\n"         \
" 4: 4-distance in 2d\n"         \
" 8: 8-distance in 2d\n"         \
" 6: 6-distance in 3d\n"         \
" 18: 18-distance in 3d\n"         \
" 26: 26-distance in 3d\n"         \
"\n"         \
"Types supported: byte 2D, byte 3D\n"         \
"\n"         \
"Category: topo\n"         \
"   topo\n"         \
"\n"         \
" Cédric Allène\n"         \
"\n"         \
"\n"


#define doc__skelpar3d_others__cxx___ "  skelpar3d_others.cxx\n"         \
"\n"         \
" parallel 3D binary skeleton\n"         \
"DirectionalSkeletonizer\n"         \
"Usage: skelpar3d in.pgm algorithm nsteps [inhibit] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Parallel 3D binary thinning or skeleton. The parameter  nsteps gives,\n"         \
"if positive, the number of parallel thinning steps to be processed.\n"         \
"If the value given for  nsteps equals -1, the thinning is continued\n"         \
"until stability.\n"         \
"\n"         \
"The parameter  algorithm is a numerical code\n"         \
"indicating which method will be used for the thinning.\n"         \
"The possible choices are:\n"         \
" 0: Palagyi (curvilinear, 6-subiterations directional, 1998)\n"         \
" 1: Palagyi (curvilinear, sequential, 2006)\n"         \
" 2: Palagyi (surface, parallel directional, 2002)\n"         \
" 3: Palagyi (surface, fully parallel, 2008)\n"         \
" 4: Raynal  (curvilinear, directional, 2010)\n"         \
" 5: Raynal  (surface, directional, 2010)\n"         \
" 6: Lohou-Bertrand  (curvilinear, symmetric, 2007)\n"         \
" 7: Ma-Wan-Chang (curvilinear, 2 subfields, 2002)\n"         \
" 8: Tsao-Fu (curvilinear, 6-subiterations directional, 1982)\n"         \
" 9: Ma-Sonka (curvilinear, fully parallel, does not preserve topology 1996)\n"         \
" 10: Ma-Wan (curvilinear (18/6) 6 subiterations, CVIU 2000)\n"         \
" 11: Lohou-Bertrand (curvilinear 6 subiterations, DAM 2005)\n"         \
" 12: Lohou-Bertrand (curvilinear 12 subiterations, DAM 2004)\n"         \
" 13: ACK3a\n"         \
" 14: CKSC\n"         \
" 15: Ma-Wan-Lee (curvilinear, 4 subfields, 2002)\n"         \
" 16: Nemeth-Kardos-Palagyi (curvilinear, 2 subfields, 2010, var. 1)\n"         \
" 17: Nemeth-Kardos-Palagyi (curvilinear, 2 subfields, 2010, var. 2)\n"         \
" 18: Nemeth-Kardos-Palagyi (curvilinear, 2 subfields, 2010, var. 3)\n"         \
" 19: Nemeth-Kardos-Palagyi (curvilinear, 4 subfields, 2010, var. 1)\n"         \
" 20: Nemeth-Kardos-Palagyi (curvilinear, 4 subfields, 2010, var. 2)\n"         \
" 21: Nemeth-Kardos-Palagyi (curvilinear, 4 subfields, 2010, var. 3)\n"         \
" 22: Nemeth-Kardos-Palagyi (curvilinear, 8 subfields, 2010, var. 1)\n"         \
" 23: Nemeth-Kardos-Palagyi (curvilinear, 8 subfields, 2010, var. 2)\n"         \
" 24: Nemeth-Kardos-Palagyi (curvilinear, 8 subfields, 2010, var. 3)\n"         \
" 25: She et al. (curvilinear, symmetric, DICTA 2009)\n"         \
" 26: Tsao-Fu (surface, 6-subiterations directional, 1981)\n"         \
" 27: Tsao-Fu (curvilinear, 6-subiterations directional, 1981)\n"         \
" 28: Nemeth-Kardos-Palagyi (curvilinear, 2 subfields, 2010, var. 0)\n"         \
" 29: Nemeth-Kardos-Palagyi (curvilinear, 4 subfields, 2010, var. 0)\n"         \
" 30: Nemeth-Kardos-Palagyi (curvilinear, 8 subfields, 2010, var. 0)\n"         \
" 31: Lohou-Bertrand  (surface, symmetric, 2007)\n"         \
" 32: Manzanera et al. (surface, symmetric, 1999)\n"         \
"\n"         \
"If the parameter  inhibit is given and is a binary image name,\n"         \
"then the points of this image will be left unchanged.\n"         \
"\n"         \
"Types supported: byte 3d\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" Michel Couprie, Benjamin Raynal, John Chaussard\n"         \
"\n"         \
"\n"


#define doc__GA2khalimsky__c__ "  GA2khalimsky.c\n"         \
"\n"         \
" Convert a 4-connected edge-weighted graph (a GA) into its\n"         \
"representation in the Khalimsky grid\n"         \
"\n"         \
"Usage: GA2khalimsky GA.pgm type out.pgm \n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"Convert a 4-connected (6-connected, in 3D) edge-weighted graph\n"         \
"(graph.ga) into its Khalimsky grid representation (depending on\n"         \
"the parameter type), stored as a pgm image\n"         \
"(out.pgm). The vertices of the graph are associated to the\n"         \
"square of the Khalimsky grid, and the edges of the graphs are\n"         \
"associated to the line segments.\n"         \
"\n"         \
"If type = 0, then the closure of the set of weighted-edges is used\n"         \
"(usefull for representing contours).  \n"         \
"\n"         \
"If type = 1, then the dual of the closure of the set of weighted edges\n"         \
"is used (usefull for representing regions).\n"         \
"\n"         \
"\n"         \
"Types supported: ga 2d, ga 3d\n"         \
"\n"         \
"Category: GA\n"         \
" GA\n"         \
"\n"         \
" Jean Cousty\n"         \
"\n"         \
"\n"


#define doc__animscenes__c__ "  animscenes.c\n"         \
"\n"         \
" generates intermediate scenes by interpolation\n"         \
"\n"         \
"Usage: animscenes prefix_in begin end prefix_out nbsamples\n"         \
"\n"         \
"Description:\n"         \
"The input scenes must be stored in files with names of the form  \n"         \
" prefix_inXXXX.3sc, where XXXX is an integer between  begin\n"         \
"and  end. They must all contain the same number of objects, \n"         \
"and object types must match from one scene to each other (order matters).\n"         \
"\n"         \
"Between the scenes of the series, a total number of  nbsamples scenes \n"         \
"are generated by spline interpolation.\n"         \
"\n"         \
"Types supported: scene 3D\n"         \
"\n"         \
"Category: draw geo\n"         \
"  draw geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__enframe__c__ "  enframe.c\n"         \
"\n"         \
" adds a border with a given gray value to an image\n"         \
"\n"         \
"Usage: enframe in.pgm [grayvalue [width [out.pgm]]]\n"         \
"\n"         \
"Description:\n"         \
"Adds a border to the input image, filled with the value  grayvalue (default 0). \n"         \
"The width of the border may be given as parameter  width, \n"         \
"otherwise its value is 1.\n"         \
"\n"         \
"If  out.pgm is not specified, then out.pgm = in.pgm.\n"         \
"\n"         \
"Types supported: byte 2d, int32_t 2d, float 2d, byte 3d, int32_t 3d, float 3d.\n"         \
"\n"         \
"Category: convert, geo\n"         \
"  convert, geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__hcutting__c__ "  hcutting.c\n"         \
"\n"         \
" homotopic cutting (pseudo opening) by a ball\n"         \
"\n"         \
"Usage: hcutting in.pgm r connex out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Homotopic cutting (pseudo opening) by a ball of radius  r.\n"         \
"\n"         \
"Reference: \n"         \
"[CB04] M. Couprie and G. Bertrand:\n"         \
"Topology preserving alternating sequential filter for smoothing \n"         \
"2D and 3D objects,  Journal of Electronic Imaging, Vol.&nbsp;13, No.&nbsp;4, pp.&nbsp;720-730, 2004.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: morpho, topobin\n"         \
"  morpho, topobin\n"         \
"\n"         \
" Michel Couprie 2002\n"         \
"\n"         \
"\n"         \
"\n"


#define doc__mcube__c__ "  mcube.c\n"         \
"\n"         \
" topologically correct \"marching cubes\"-like algorithm\n"         \
"\n"         \
"Usage: mcube in.pgm threshold nregul obj_id format [connex] out\n"         \
"\n"         \
"Description:\n"         \
"Generates a 3d mesh from the binary or grayscale image  in.pgm .\n"         \
"\n"         \
"The original image is first thresholded (parameter  threshold, values 0 and 1\n"         \
"both fit for a binary image). Then the method described in [Lac96] is applied to\n"         \
"generate the 3d mesh. The resulting mesh is then smoothed by  nregul steps of \n"         \
"laplacian smoothing. The parameter  obj_id is used to tag the generated mesh.\n"         \
"\n"         \
"The parameter  format indicate the format of the output file \n"         \
"(choices are POV, POVB, COL, MCM, AC, GL, VTK, RAW).\n"         \
"The keyword POVB corresponds to a bare Povray mesh: \n"         \
"a header and a footer must be catenated in order to make a full Povray scene. \n"         \
"The keyword RAW is the exchange format for the \"mesh\" software: see http://mesh.berlios.de/\n"         \
"\n"         \
"The optional parameter  connex indicates the connexity used for the object.\n"         \
"Possible values are 6 and 26 (default).\n"         \
"\n"         \
"[Lac96] J.-O. Lachaud, \"Topologically defined iso-surfaces\", DGCI'96, LNCS 1176 (245--256), Springer Verlag, 1996.\n"         \
"\n"         \
"Types supported: byte 3d\n"         \
"\n"         \
"Category: mesh3d\n"         \
"  mesh3d\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__complex_imaginary__c__ "  complex_imaginary.c\n"         \
"\n"         \
" pixelwise imaginary part of complex\n"         \
"\n"         \
"Usage: complex_imaginary in.pgm out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Applies to complex images only.\n"         \
"For each pixel p, out[p] = imaginary(in[p]), where \n"         \
"imaginary(x+iy) = y\n"         \
"\n"         \
"Types supported: complex 2d, complex 3d\n"         \
"\n"         \
"Category: arith\n"         \
"  arith\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__erosbin__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__rotate__c__ "  rotate.c\n"         \
"\n"         \
" rotation of an image\n"         \
"\n"         \
"Usage: rotate in.pgm theta [x y] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"rotates the image  in.pgm of the angle  theta (in degrees).\n"         \
"Method: interpolation.\n"         \
"If  x and  y are given, then the center of the rotation is the point \n"         \
"(x,y) and the image size is left unchanged (hence parts of object \n"         \
"may be lost). \n"         \
"Otherwise, the center of the rotation is the point (0,0) and the resulting \n"         \
"image size is computed such that no loss of information occur.\n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: geo\n"         \
"  geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__tlf__c__ "  tlf.c\n"         \
"\n"         \
" topological lower filter\n"         \
"\n"         \
"Usage: tlf in.pgm connexmin r out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Topological lower filter. Performs the homotopic thickening controlled by\n"         \
"a radius r, followed by a well deletion, and a homotopic reconstruction over\n"         \
"the original image.\n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: topogray\n"         \
"  topogray\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__delaunay__c__ "  delaunay.c\n"         \
"\n"         \
" delaunay triangulation\n"         \
"\n"         \
"Usage: delaunay in.list [mask.pgm] out.list\n"         \
"\n"         \
"Description:\n"         \
"Reads a point list in file in.list under the following format:\n"         \
"  \n"         \
"    b &lt;n&gt;         n &lt;n&gt;    \n"         \
"    x1 y1         x1 y1 v1\n"         \
"    x2 y2   ou    x2 y2 v2\n"         \
"    ...           ...\n"         \
"    xn yn         xn yn vn\n"         \
"  \n"         \
"\n"         \
"Computes a Delaunay triangulation and stores the resulting graph \n"         \
"into file out.graph under the following format:\n"         \
"\n"         \
"    G &lt;n&gt;\n"         \
"    x1 y1 v1 ec1 ns1 s11 s12 ... s1ns1\n"         \
"    x2 y2 v2 ec2 ns2 s21 s22 ... s1ns2\n"         \
"    ...\n"         \
"    xn yn vn ecn nsn sn1 sn2 ... s1nsn\n"         \
"\n"         \
"  \n"         \
"where xi, yi are the coordinates of the ith vertex, vi is the associated \n"         \
"value (if given in the input file), eci is a int32_t which indicates \n"         \
"whether the vertex i belongs to the convex hull, nsi denotes the number\n"         \
"of adjacent vertices, and si1 si2 ... sins1 is the list of the indexes of\n"         \
"the adjacent vertices (counted from 0).\n"         \
"\n"         \
"If the optional parameter mask.pgm is given, then it must be a \n"         \
"binary byte image. Any edge of the triangulation which intersects the\n"         \
"mask will be discarded.  \n"         \
"\n"         \
"Types supported: byte 2D\n"         \
"\n"         \
"See also: drawtriangulation.c\n"         \
"\n"         \
"Category: geo\n"         \
"  geo\n"         \
"\n"         \
" algorithm in O(n^2)\n"         \
"\n"         \
" Michel Couprie, Laurent Mercier\n"         \
"\n"         \
"\n"


#define doc__segmentnum__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__lfopen3d_rect__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__alpha__c__ "  alpha.c\n"         \
"\n"         \
" computes the connecivity number alpha for each image voxel\n"         \
"\n"         \
"Usage: alpha in out\n"         \
"\n"         \
"Description: \n"         \
"Computes the connecivity number alpha (as defined in ref. BEC97) for each image voxel.\n"         \
"\n"         \
"Types supported: byte 3D\n"         \
"\n"         \
"Category: topogray\n"         \
"  topogray\n"         \
"\n"         \
"References:\n"         \
"\n"         \
"[BEC97] G. Bertrand, J. C. Everat and M. Couprie: \"Image segmentation through operators based upon topology\",  Journal of Electronic Imaging, Vol.&nbsp;6, No.&nbsp;4, pp.&nbsp;395-405, 1997.\n"         \
"\n"         \
"[CBB01] M. Couprie, F.N. Bezerra, Gilles Bertrand: \"Topological operators for\n"         \
"grayscale image processing\",  Journal of Electronic Imaging, Vol.&nbsp;10, No.&nbsp;4, pp.&nbsp;1003-1015, 2001.\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__skeleton__c__ "  skeleton.c\n"         \
"\n"         \
" ultimate binary skeleton guided by a priority image (see [BC07])\n"         \
"\n"         \
"Usage: skeleton in.pgm prio connex [inhibit] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Ultimate binary skeleton guided by a priority image.\n"         \
"The lowest values of the priority image correspond to the highest priority.\n"         \
"\n"         \
"The parameter  prio is either an image (byte, int32_t, float or double), or a numerical code\n"         \
"indicating that a distance map will be used as a priority image; \n"         \
"the possible choices are:\n"         \
" 0: approximate euclidean distance\n"         \
" 1: approximate quadratic euclidean distance\n"         \
" 2: chamfer distance\n"         \
" 3: exact quadratic euclidean distance\n"         \
" 4: 4-distance in 2d\n"         \
" 8: 8-distance in 2d\n"         \
" 6: 6-distance in 3d\n"         \
" 18: 18-distance in 3d\n"         \
" 26: 26-distance in 3d\n"         \
"\n"         \
"The parameter  connex indicates the connectivity of the binary object.\n"         \
"Possible choices are 4, 8 in 2d and 6, 26 in 3d.\n"         \
"\n"         \
"If the parameter  inhibit is given and is an integer different from -1,\n"         \
"then the points which correspond to this priority value will be left unchanged. \n"         \
"If the parameter  inhibit is given and is a binary image name,\n"         \
"then the points of this image will be left unchanged. \n"         \
"\n"         \
"Let X be the set corresponding to the input image  in.pgm.\n"         \
"Let P be the function corresponding to the priority image.\n"         \
"Let I be the set corresponding to the inhibit image, if given, or the empty\n"         \
"set otherwise.\n"         \
"The algorithm is the following:\n"         \
"\n"         \
"\n"         \
"Repeat until stability\n"         \
"    Select a point x in X  I such that P[x] is minimal\n"         \
"    If x is simple for X then\n"         \
"        X = X  {x}\n"         \
"Result: X\n"         \
"\n"         \
"\n"         \
"Reference: \n"         \
"[BC07] G. Bertrand and M. Couprie: \"Transformations topologiques discretes\", in G&eacute;om&eacute;trie discr&egrave;te et images num&eacute;riques, D. Coeurjolly and A. Montanvert and J.M. Chassery, pp.&nbsp;187-209, Herm&egrave;s, 2007.\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"Example:\n"         \
"\n"         \
"skeleton circuit1 8 8 circuit1_skeleton\n"         \
"\n"         \
"\n"         \
"  \n"         \
"      \n"         \
"  \n"         \
"  circuit1\n"         \
"      circuit1_skeleton\n"         \
"  \n"         \
"\n"         \
"\n"         \
"\n"         \
"\n"


#define doc__short2float__c__ "  short2float.c\n"         \
"\n"         \
" converts a \"short\" image to a \"float\" image\n"         \
"\n"         \
"Usage: short2float in [out]\n"         \
"\n"         \
"Description: \n"         \
"\n"         \
"For each pixel x, out[x] = (float)in[x]\n"         \
"\n"         \
"If the last argument  out is omitted, then out = in.\n"         \
"\n"         \
"Types supported: short 2d, short 3d.\n"         \
"\n"         \
"Category: convert\n"         \
" convert\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__deframe__c__ "  deframe.c\n"         \
"\n"         \
" suppress the frame from an image\n"         \
"\n"         \
"Usage: deframe in.pgm out.pgm\n"         \
"\n"         \
"Description:\n"         \
"suppress the frame from an image.\n"         \
"\n"         \
"Types supported: byte 2d, int32_t 2d, float 2d, byte 3d, int32_t 3d, float 3d.\n"         \
"\n"         \
"Category: convert, geo\n"         \
"  convert, geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__gettree__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__warp__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__segmentheight__c__ "  segmentheight.c\n"         \
"\n"         \
" segmentation based on a height criterion\n"         \
"\n"         \
"Usage: segmentheight in.pgm connex height [r] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Height segmentation with connexity connex and depth height, \n"         \
"followed by a reconstruction (2D only) if option r is set (see reference: to appear)\n"         \
"or a maximization if option m is set.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: connect\n"         \
" connect\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__2dflowskeleton__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__wshedtopo__c__ "  wshedtopo.c\n"         \
"\n"         \
" topological grayscale watershed  (Couprie-Bertrand algorithm)\n"         \
"\n"         \
"Usage: wshedtopo in.pgm connex [i] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Topological grayscale watershed as defined in [CB97], preserves the minima - connectivity connex\n"         \
"If the parameter i is given, then the dual operator is applied.\n"         \
"\n"         \
"[CB97] M. Couprie and G. Bertrand:\n"         \
"``Topological Grayscale Watershed Transformation'',\n"         \
"SPIE Vision Geometry V Proceedings, 3168 (136--146), 1997.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: connect\n"         \
" connect\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"Example:\n"         \
"\n"         \
"wshedtopo ur1 4 ur1_wshedtopo\n"         \
"\n"         \
"\n"         \
"  \n"         \
"      \n"         \
"  \n"         \
"  ur1\n"         \
"      ur1_wshedtopo\n"         \
"  \n"         \
"\n"         \
"\n"         \
"\n"


#define doc__hthinpar__c__ "  hthinpar.c\n"         \
"\n"         \
" parallel grayscale homotopic thinning\n"         \
"\n"         \
"Usage: hthinpar in.pgm {imcond.pgm|null} connex niter out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Parallel grayscale homotopic thinning (refs. [BEC97, CBB01]).\n"         \
"The parameter  connex gives the connectivity used for the minima;\n"         \
"possible choices are 4, 8 in 2D and 6, 26 in 3D.\n"         \
"Let F be the function corresponding to the input image  in.pgm.\n"         \
"Let G be the function corresponding to the input image \n"         \
" imcond.pgm, or the null function if the keyword  null is used.\n"         \
"\n"         \
"If  niter is set to -1, then the operator iterates until stability.\n"         \
"\n"         \
"References:\n"         \
"\n"         \
"to appear\n"         \
"\n"         \
"Types supported: byte 2D.\n"         \
"\n"         \
"Category: topogray\n"         \
"  topogray\n"         \
"\n"         \
" Nivando Bezerra\n"         \
"\n"         \
"\n"


#define doc__toposhrink__c__ "  toposhrink.c\n"         \
"\n"         \
" topologically controled binary shrinking guided by a priority image\n"         \
"\n"         \
"Usage: toposhrink in.pgm prio.pgm connex tmin tmax tbmin tbmax [inhibit] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Topologically controled binary shrinking guided by a priority image.\n"         \
"The lowest values of the priority image correspond to the highest priority.\n"         \
"\n"         \
"The parameter  prio is an image (byte or int32_t).\n"         \
"\n"         \
"The parameter  connex indicates the connectivity of the binary object.\n"         \
"Possible choices are 4, 8 in 2D and 6, 18, 26 in 3D.\n"         \
"\n"         \
"If the parameter  inhibit is given and is a binary image name,\n"         \
"then the points of this image (set Y) will be left unchanged. \n"         \
"\n"         \
"Let X be the set of points of the binary image  in.pgm .\n"         \
"\n"         \
"\n"         \
"Repeat until stability: \n"         \
"    select a point p of X  Y such that\n"         \
"        tmin \n"         \
"        and with the lowest possible priority value\n"         \
"    X := X  { p }\n"         \
"\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__watershedMeyer2__c__ "  watershedMeyer2.c\n"         \
"\n"         \
" watershed transformation (Meyer's algorithm) with labelled markers  \n"         \
"\n"         \
"Usage: watershedMeyer2 in mark {roi|null} connex out\n"         \
"\n"         \
"Description:\n"         \
"Performs the watershed transformation on the image in, taking the\n"         \
"set of markers in mark, in the form of a label image where all the pixels\n"         \
"sharing the same label (even if not connected) belong to the same marker.\n"         \
"If this parameter is present, roi\n"         \
"indicates the region of interest on which the operation is performed.\n"         \
"The parameter connex gives the adjacency relation (4,8 in 2D; 6,18,26 in 3D) \n"         \
"for the makers.\n"         \
"\n"         \
"The result is a binary image.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: connect\n"         \
" connect\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__gammacor__c__ "  gammacor.c\n"         \
"\n"         \
" performs gamma correction on image\n"         \
"\n"         \
"Usage: gammacor in.pgm gamma out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Applies the following steps to  in.pgm :\n"         \
" normalization between 0 and 1\n"         \
" raise pixel values to the power  gamma\n"         \
" reverts the normalization\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n"         \
"\n"         \
"Category: arith\n"         \
"  arith\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__htkernu__c__ "  htkernu.c\n"         \
"\n"         \
" grayscale ultimate homotopic thickening\n"         \
"\n"         \
"Usage: htkernu in.pgm {imcond.pgm|null} connex out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Grayscale ultimate homotopic thickening (refs. [BEC97, CBB01]).\n"         \
"The parameter  connex gives the connectivity used for the minima;\n"         \
"possible choices are 4, 8 in 2D and 6, 26 in 3D.\n"         \
"Let F be the function corresponding to the input image  in.pgm.\n"         \
"Let G be the function corresponding to the input image \n"         \
" imcond.pgm, or the null function if the keyword  null is used.\n"         \
"The algorithm is the following:\n"         \
"\n"         \
"\n"         \
"Repeat until stability:\n"         \
"    select p constructible for F such that F(p) is maximal\n"         \
"    F(p) = min{ delta+(p,F), G(p) }    \n"         \
"Result: F\n"         \
"\n"         \
"\n"         \
"References: \n"         \
"[BEC97] G. Bertrand, J. C. Everat and M. Couprie: \"Image segmentation through operators based upon topology\",  Journal of Electronic Imaging, Vol.&nbsp;6, No.&nbsp;4, pp.&nbsp;395-405, 1997.\n"         \
"[CBB01] M. Couprie, F.N. Bezerra, Gilles Bertrand: \"Topological operators for\n"         \
"grayscale image processing\",  Journal of Electronic Imaging, Vol.&nbsp;10, No.&nbsp;4, pp.&nbsp;1003-1015, 2001.\n"         \
"\n"         \
"Types supported: byte 2D, byte 3D.\n"         \
"\n"         \
"Category: topogray\n"         \
"  topogray\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__watershedMeyer__c__ "  watershedMeyer.c\n"         \
"\n"         \
" watershed transformation (Meyer's algorithm)\n"         \
"\n"         \
"Usage: watershedMeyer in mark {bgmark|null} {roi|null} connex out\n"         \
"\n"         \
"Description:\n"         \
"Performs the watershed transformation on the image in, taking the\n"         \
"set of markers in mark. \n"         \
"If this parameter is present, bgmark\n"         \
"is used as a set of markers for the background.\n"         \
"If this parameter is present, roi\n"         \
"indicates the region of interest on which the operation is performed.\n"         \
"The parameter connex gives the adjacency relation (4,8 in 2D; 6,18,26 in 3D) \n"         \
"for the makers.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: connect\n"         \
" connect\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__fusreg___notused_c__ "WRITE ME!!!\n"         \
"\n"


#define doc__rankfilter__c__ "  rankfilter.c\n"         \
"\n"         \
" rank filter \n"         \
"\n"         \
"Usage: rankfilter in.pgm el.pgm r out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Let F be the input image, G be the output image, and E the structuring\n"         \
"element. Let n be the number of elements of E, and R be the product n.r,\n"         \
"then for each pixel p, G[p] is the Rth element of the sorted list (by \n"         \
"increasing order) of the pixel values in the set { F[q], q in E[p] }.\n"         \
"\n"         \
"Particular cases are the median filter (r = 0.5), the erosion (r = 0),\n"         \
"and the dilation (r = 1).\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: morpho\n"         \
" morpho\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__genfield__c__ "  genfield.c\n"         \
"\n"         \
" generates a null vector field of given size\n"         \
"\n"         \
"Usage: genfield {in.pgm|rs cs ds} out.pgm\n"         \
"\n"         \
"Description:\n"         \
"The size of the result field out.pgm is taken from image\n"         \
"in.pgm, or given by the parameters rs, cs, ds. \n"         \
"\n"         \
"Types supported: float 2d, float 3d\n"         \
"\n"         \
"Category: arith, geo\n"         \
"  arith, geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__seuili__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__curvatures__c__ "  curvatures.c\n"         \
"\n"         \
" compute the curvatures of a natural cubic spline\n"         \
"\n"         \
"Usage: curvatures spline.txt n out.txt\n"         \
"\n"         \
"Description:\n"         \
"Samples the spline in  spline.txt and compute the curvature at each sample point.\n"         \
"The file  spline.txt contains a 2D or 3D spline under the format:\n"         \
"s nbpoints  \n"         \
"x1 y1\n"         \
"x2 y2\n"         \
"x3 y3\n"         \
"...\n"         \
"or:\n"         \
"S nbpoints  \n"         \
"x1 y1 z1\n"         \
"x2 y2 z2\n"         \
"x3 y3 z3\n"         \
"...\n"         \
"\n"         \
"Types supported: 2D, 3D splines\n"         \
"\n"         \
"Category: geo\n"         \
"  geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__attribute__c__ "  attribute.c\n"         \
"\n"         \
" labels each connected component with its attribute value \n"         \
"\n"         \
"Usage: attribute in.pgm connex seuil typregion attrib out.pgm\n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"Labels each connected component with its attribute value.\n"         \
"\n"         \
"Argument  connex defines the connectivity.\n"         \
"\n"         \
"Argument  seuil defines a threshold: components with attribute value not higher than  seuil are filtered out.\n"         \
"\n"         \
"Argument  typregion defines the type of regions:\n"         \
" 0: regional maxima\n"         \
" 1: regional minima\n"         \
" 2: plateaus\n"         \
"\n"         \
"Argument  attrib defines the considered attribute:\n"         \
" 0: area\n"         \
" 1: perimeter\n"         \
" 2: circularity\n"         \
" 3: number of holes\n"         \
" 4: excentricity\n"         \
" 5: orientation\n"         \
" 6: horizontal size (of bounding box)\n"         \
" 7: vertical size (of bounding box)\n"         \
" 8: depth size (of bounding box)\n"         \
" 9: max of horizontal, vertical [and depth] sizes (of bounding box)\n"         \
" 10: maximum diameter (Euclidean distance between most distant points): not yet implemented\n"         \
"\n"         \
" In 3D only the attributes 0 and 6-9 are implemented.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: geo\n"         \
"  geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__histstretch__c__ "  histstretch.c\n"         \
"\n"         \
" transforms the gray scale by stretching the histogram\n"         \
"\n"         \
"Usage: histstretch in.pgm vmin vmax p out.pgm\n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"Eliminates the levels of the histogram strictly less than vmin or \n"         \
"strictly greater than vmax, and also the extremal levels\n"         \
"that have strictly less than N*(p/100) pixels \n"         \
"(N being the total number of pixels). If p = 0, only the \n"         \
"extremal levels with 0 pixels are eliminated.\n"         \
"A linear histogram scaling is then performed.\n"         \
"\n"         \
"Types supported: byte 2D\n"         \
"\n"         \
"Category: histo\n"         \
"  histo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__ellipsefit__c__ "  ellipsefit.c\n"         \
"\n"         \
" ellipse identification and drawing from spare points\n"         \
"\n"         \
"Usage: ellipsefit in.pgm pts.list filled out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Identifies the ellipse E which minimizes the sum of the (approximate) distances\n"         \
"between the points in  pts.list and E. \n"         \
"If  filled (int32_t) is set, then a filled ellipse is drawn.\n"         \
"\n"         \
"Ref: \n"         \
"Andrew W. Fitzgibbon, Maurizio Pilu, and Robert B. Fisher\n"         \
"Direct least-squares fitting of ellipses,\n"         \
"IEEE Transactions on Pattern Analysis and Machine Intelligence, 21(5), 476--480, May 1999\n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: geo\n"         \
"  geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__max__c__ "  max.c\n"         \
"\n"         \
" maximum of 2 images\n"         \
"\n"         \
"Usage: max in1.pgm in2.pgm out.pgm\n"         \
"\n"         \
"Description:\n"         \
"For each pixel x, out[x] = max{in1[x], in2[x]}. \n"         \
"Images must be of the same type and same dimensions.\n"         \
"\n"         \
"Types supported: byte 2D, byte 3D\n"         \
"\n"         \
"Category: arith\n"         \
"  arith\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__deriche__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__drawline__c__ "  drawline.c\n"         \
"\n"         \
" draw a straight line segment in a 2D image\n"         \
"\n"         \
"Usage: drawline in.pgm x1 y1 [z1] x2 y2 [z2] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"Draws a straight line segment between (x1,y1[,z1]) and (x2,y2[,z2])\n"         \
"\n"         \
"Types supported: byte 2D, byte 3D\n"         \
"\n"         \
"Category: draw\n"         \
"  draw\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__insert__c__ " insert.c\n"         \
"\n"         \
" inserts an image into another image\n"         \
"\n"         \
"Usage: insert a.pgm b.pgm x y z out.ppm\n"         \
"\n"         \
"Description: Inserts image  a.pgm into image  b.pgm at position  x,y,z\n"         \
"\n"         \
"Types supported: byte 2d, int32_t 2d, float 2d, byte 3d, int32_t 3d, float 3d.\n"         \
"\n"         \
"Category: convert, geo\n"         \
" convert, geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__localextrema__c__ "  localextrema.c\n"         \
"\n"         \
" local extrema \n"         \
"\n"         \
"Usage: in.pgm connex minimum out.pgm\n"         \
"\n"         \
"Description: \n"         \
"Selects the local maxima or minima of a grayscale image with connexity connex.\n"         \
"\n"         \
"Types supported: byte 2d - byte 3d\n"         \
"\n"         \
"Category: connect, topogray\n"         \
"  connect, topogray\n"         \
"\n"         \
" Laurent Najman\n"         \
"\n"         \
"\n"


#define doc__GAwatershedDouble__c__ "  GAwatershedDouble.c\n"         \
"\n"         \
" Compute the watershed of a 4-connected edge-weighted graph (a GA) where the weights are doubles.\n"         \
"\n"         \
"Usage: \n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"Types supported: GA byte 2D \n"         \
"\n"         \
"Category: \n"         \
"  \n"         \
"\n"         \
" Jean Cousty\n"         \
"\n"         \
"\n"


#define doc__skel_AK2__c__ "  skel_AK2.c\n"         \
"\n"         \
" parallel 2D binary curvilinear skeleton\n"         \
"\n"         \
"Usage: skel_AK2 in.pgm nsteps [inhibit] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Parallel 2D binary thinning or curvilinear skeleton. The parameter  nsteps gives,\n"         \
"if positive, the number of parallel thinning steps to be processed.\n"         \
"If the value given for  nsteps equals -1, the thinning is continued\n"         \
"until stability.\n"         \
"\n"         \
"If the parameter  inhibit is given and is a binary image name,\n"         \
"then the points of this image will be left unchanged. \n"         \
"\n"         \
"Warning: The object must not have any point on the frame of the image.\n"         \
"\n"         \
"References:\n"         \
"[BC08] Gilles Bertrand and Michel Couprie, Two-dimensional thinning algorithms based on critical kernels. Journal of Mathematical Imaging and Vision, 31(1). 2008. pp.35â€“56.\n"         \
"http://igm.univ-mlv.fr/LIGM/internal_report/pdf/2006_02.v2.pdf\n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__detectcercles__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__div__c__ "  div.c\n"         \
"\n"         \
" pixelwise division of two images\n"         \
"\n"         \
"Usage: div in1.pgm in2.pgm out.pgm\n"         \
"\n"         \
"Description:\n"         \
"For each pixel x such that in2[x] != 0, out[x] = in1[x] / in2[x]. \n"         \
"For each pixel x such that in2[x] = 0, out[x] = 0. \n"         \
"Images must be of the same type and same dimensions.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n"         \
"\n"         \
"Category: arith\n"         \
"  arith\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__lfclose_line__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__lambdathin__c__ "  lambdathin.c\n"         \
"\n"         \
" grayscale homotopic and filtered thinning\n"         \
"\n"         \
"Usage: lambdathin in.pgm {imcond.pgm|null} connex niter h out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Homotopic thinning for 2D grayscale images. \n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: topogray\n"         \
"  topogray\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__nbcomp__c__ "  nbcomp.c\n"         \
"\n"         \
" returning number of connected components of a grayscale or a binary image\n"         \
"\n"         \
"Usage: nbcomp in.pgm connex {fgd|bgd|min|max|pla} [out.list]\n"         \
"\n"         \
"Description:\n"         \
"The argument  connex selects the connectivity (4, 8 in 2D; 6, 18, 26 in 3D).\n"         \
"The argument  selects the kind of flat zone\n"         \
"to be counted:\n"         \
"  fgd selects foreground components for a binary image\n"         \
"  bgd selects background components for a binary image\n"         \
"  min selects regional minima\n"         \
"  max selects regional maxima\n"         \
"  pla selects all flat zones (plateaux).\n"         \
"\n"         \
"The result is written in the list out.list.\n"         \
"\n"         \
"If the parameter  out.list is ommitted, the result is printed on the standard output.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d\n"         \
"\n"         \
"Category: connect\n"         \
"  connect\n"         \
"\n"         \
" Cedric Allene\n"         \
"\n"         \
"\n"


#define doc__dilatplan__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__htkernu3d___notused_c__ "WRITE ME!!!\n"         \
"\n"


#define doc__identifyline__c__ "  identifyline.c\n"         \
"\n"         \
" identification of a best matching line from a set of 2D points\n"         \
"\n"         \
"Usage: identifyline in.list out.list\n"         \
"\n"         \
"Description:\n"         \
"Identifies the parameters (a,b) of the equation of the 2D line:\n"         \
"ax+b=y that minimizes the least square error between this line \n"         \
"and the given points. Method: basic linear regression.\n"         \
"\n"         \
"Types supported: list 1D, list 2D\n"         \
"\n"         \
"Category: geo\n"         \
"  geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__ptinterior__c__ "  ptinterior.c\n"         \
"\n"         \
" detects interior points in a binary image\n"         \
"\n"         \
"Usage: ptinterior in.pgm connex out.pgm\n"         \
"\n"         \
"Description:\n"         \
"An interior point is a white point, all the n-neighbours of which are white\n"         \
"(n = 4, 8 (2d) or 6, 18, 26 (3d), as set by the parameter  connex)\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" Michel Couprie 2005\n"         \
"\n"         \
"\n"


#define doc__drawtorus__c__ "  drawtorus.c\n"         \
"\n"         \
" generates a binary torus\n"         \
"\n"         \
"Usage: drawtorus {in.pgm|null} r1 r2 xc yc zc out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Draws a binary torus in the image  in.pgm.\n"         \
"The parameters r1, r2 specify respectively the big \n"         \
"and the small radius of the torus. \n"         \
"The parameters xc, yc, zc specify the center of the torus. \n"         \
"If the first parameter is \"null\", then an image with the\n"         \
"appropriate size is created.\n"         \
"\n"         \
"Types supported: byte 3d\n"         \
"\n"         \
"Category: draw\n"         \
"  draw\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__watershed__c__ "  watershed.c\n"         \
"\n"         \
" topological binary watershed (Couprie-Bertrand algorithm)\n"         \
"\n"         \
"Usage: watershed in.pgm mark.pgm connex [i] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Topological watershed as defined in [CB97] - connectivity connex.\n"         \
"A marker image is supplied in mark.pgm. The result is a binary image.\n"         \
"If the parameter i is given, then the dual operator is applied.\n"         \
"\n"         \
"[CB97] M. Couprie and G. Bertrand:\n"         \
"``Topological Grayscale Watershed Transformation'',\n"         \
"SPIE Vision Geometry V Proceedings, 3168 (136--146), 1997.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: connect\n"         \
" connect\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__skel_PSG__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__3dpardircollapse__c__ "  3dpardircollapse.c\n"         \
"\n"         \
" parallel directional constrained collapse\n"         \
"\n"         \
"Usage: 3dpardircollapse in.pgm nsteps [inhibit] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Parallel directional constrained collapse.\n"         \
"\n"         \
"If the parameter  inhibit is given and is a binary image name,\n"         \
"then the points of this image will be left unchanged. \n"         \
"\n"         \
"Types supported: byte 3d\n"         \
"\n"         \
"Category: orders\n"         \
"  orders\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__long2byte__c__ "  long2byte.c\n"         \
"\n"         \
" converts a \"int32_t\" image to a \"byte\" image\n"         \
"\n"         \
"Usage: long2byte in.pgm [mode] [n] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"Depending on the value given for the (optional) parameter mode:\n"         \
"   mode = 0 (default) : for all x, out[x] = min(255, in[x]).\n"         \
"   mode = 1 : for all x, out[x] = in[x] modulo 256.\n"         \
"   mode = 2 : scales values in the range 0-255.\n"         \
"   mode = 3 : sorts the values by decreasing number of occurence in the image.\n"         \
"Replaces the values by their order.\n"         \
"Only the n (default 255) first values are kept.\n"         \
"Useful for label images.\n"         \
"   mode = 4 : truncation of the square root in the range 0-255.\n"         \
"   mode = 5 : truncation of the log in the range 0-255.\n"         \
"\n"         \
"Types supported: int32_t 2d, int32_t 3d\n"         \
"\n"         \
"Category: convert\n"         \
"  convert\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__asftmed__c__ "  asftmed.c\n"         \
"\n"         \
" alternate sequential filter controled by topology\n"         \
"\n"         \
"Usage: asftmed in.pgm connex rmax out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Alternate sequential filter controled by topology (see [CB04]).\n"         \
"Disc- or Ball-shaped structuring elements of increasing radius,\n"         \
"ranging from 1 to rmax,\n"         \
"are generated. Let D1...Dn be these structuring elements, sorted by increasing radius.\n"         \
"Let I(0) = in.pgm, the ith intermediate result I(i) is obtained by the \n"         \
"homotopic pseudo-closing of the homotopic pseudo-opening of I(i-1) by the structuring element Di. \n"         \
"Furthermore, the medial axes (distance 8 in 2D, 26 in 3D) of both the input image and its inverse \n"         \
"are used as constraint sets.\n"         \
"The result out.pgm contains the final result I(n).\n"         \
"\n"         \
"Reference: \n"         \
"[CB04] M. Couprie and G. Bertrand:\n"         \
"Topology preserving alternating sequential filter for smoothing \n"         \
"2D and 3D objects,  Journal of Electronic Imaging, Vol.&nbsp;13, No.&nbsp;4, pp.&nbsp;720-730, 2004.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: morpho, topobin\n"         \
"  morpho, topobin\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__encadre__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__3dcollapse__c__ "  3dcollapse.c\n"         \
"\n"         \
" ultimate constrained collapse guided by a priority image\n"         \
"\n"         \
"Usage: 3dcollapse in.pgm prio [inhibit] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Ultimate constrained collapse guided by a priority image.\n"         \
"The lowest values of the priority image correspond to the highest priority.\n"         \
"\n"         \
"The parameter  prio is either an image (byte or int32_t), or a numerical code\n"         \
"indicating that a distance map will be used as a priority image; \n"         \
"the possible choices are:\n"         \
" 0: approximate euclidean distance\n"         \
" 1: approximate quadratic euclidean distance\n"         \
" 2: chamfer distance\n"         \
" 3: exact quadratic euclidean distance\n"         \
" 6: 6-distance in 3d\n"         \
" 18: 18-distance in 3d\n"         \
" 26: 26-distance in 3d\n"         \
"\n"         \
"If the parameter  inhibit is given and is a binary image name,\n"         \
"then the elements of this image will be left unchanged. \n"         \
"If the parameter  inhibit is given and is a number I,\n"         \
"then the elements with priority greater than or equal to I will be left unchanged. \n"         \
"\n"         \
" The result makes sense only if the input image is a complex.\n"         \
"\n"         \
"Types supported: byte 3d\n"         \
"\n"         \
"Category: orders\n"         \
"  orders\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__segment3d__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__fft__c__ "  fft.c\n"         \
"\n"         \
" fast Fourier transform\n"         \
"\n"         \
"Usage: fft in.pgm [dir] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Computes the 2 dimensional Fast Fourier Transform of an image.\n"         \
"\n"         \
"This program will compute either a forward or inverse FFT, depending on\n"         \
"the direction requested with the  dir option.  A  dir value of 0\n"         \
"will result in a forward FFT, and a  dir value of 1 will result\n"         \
"in an inverse FFT. \n"         \
"\n"         \
"The arguments are described as follows:\n"         \
"\n"         \
" in.pgm specifies the input image, which must be of data type COMPLEX.\n"         \
"\n"         \
" out.pgm output image, which will be an image of data type COMPLEX. \n"         \
"If row size or column size of in.pgm is not an integral power of 2, then input data is padded and dimensions of out.pgm may be different from those of in.pgm . \n"         \
"\n"         \
" dir (optional)\n"         \
"specifies the FFT direction.  A  dir of 0 (default) will result in a forward FFT, \n"         \
"and a  dir of 1 will result in an inverse FFT.\n"         \
"\n"         \
"This particular implementation of the DFT uses the transform pair defined as follows:\n"         \
"\n"         \
"Let there be two complex arrays each with n rows and m columns.\n"         \
"\n"         \
"Index them as \n"         \
"\n"         \
"f(x,y):    0 \n"         \
"\n"         \
"F(u,v):    -m/2 \n"         \
"\n"         \
"Then the forward and inverse transforms are related as follows.\n"         \
"\n"         \
"Forward:\n"         \
"\n"         \
"F(u,v) =   f(x,y)  i (ux/m + vy/n)}\n"         \
"\n"         \
"Inverse:\n"         \
"\n"         \
"f(x,y) = 1/(mn)   F(u,v)  i (ux/m + vy/n)}\n"         \
"\n"         \
"Therefore, the transforms have these properties:\n"         \
"\n"         \
"1.     f(x,y) = F(0,0)\n"         \
"\n"         \
"2.  m n   |f(x,y)|^2 =   |F(u,v)|^2\n"         \
"\n"         \
"Types supported: complex 2d\n"         \
"\n"         \
"Category: signal\n"         \
" signal\n"         \
"\n"         \
" Stefan Gustavson (stegu@itn.liu.se) 2003-10-20\n"         \
"\n"         \
"\n"


#define doc__printhisto__c__ "  printhisto.c\n"         \
"\n"         \
" prints the histogram of an image or a region\n"         \
"\n"         \
"Usage: printhisto in.pgm [mask.pgm]\n"         \
"\n"         \
"Description:\n"         \
"Calculates the histogram of  im.pgm (masked by the binary image\n"         \
" mask.pgm, if given) and prints it on the screen.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d\n"         \
"\n"         \
"Category: histo\n"         \
"  histo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__histscal__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__normalize__c__ "  normalize.c\n"         \
"\n"         \
" normalization of grayscale values\n"         \
"\n"         \
"Usage: normalize in.pgm [[nmin nmax] out.pgm]\n"         \
"\n"         \
"Description:\n"         \
"Grayscale of  in.pgm values are normalized to span the range of [nmin...nmax].\n"         \
"The parameters  nmin and  nmax are optional.\n"         \
"For byte and int32_t images, the default values are  nmin = 0 and  nmax = 255.\n"         \
"For float images, the default values are  nmin = 0 and  nmax = 1.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n"         \
"\n"         \
"Category: arith\n"         \
"  arith\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__rankmaxopening__c__ "  rankmaxopening.c\n"         \
"\n"         \
" rank-max opening \n"         \
"\n"         \
"Usage: rankmaxopening in.pgm el.pgm r out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Let F be the input image, G be the output image, and E the structuring\n"         \
"element.\n"         \
"\n"         \
"The rank-max opening [Ron86]\n"         \
"of structuring element $E$ and parameter  r in [0...1]\n"         \
"may be defined by G = min(F,dilation(rankfilter(F,E,1-r),E))\n"         \
"\n"         \
"[Ron86] C. Ronse:\n"         \
"\"Erosion of narrow image features by combination of local low rank and max filters\",\n"         \
"Proc. 2nd Int. Conf. on Image Processing and int32_t Applications,\n"         \
"pages 77-81, 1986.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: morpho\n"         \
" morpho\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__pgm2raw__c__ "  pgm2raw.c\n"         \
"\n"         \
" suppress the header from a pgm file\n"         \
"\n"         \
"Usage: pgm2raw in.pgm out.raw\n"         \
"\n"         \
"Description: suppress the header from a pgm file\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d, short 2d, short 3d, long 2d, long 3d, float 2d, float 3d, double 2d, double 3d\n"         \
"\n"         \
"Category: convert\n"         \
"  convert\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__drawrect__c__ "  drawrect.c\n"         \
"\n"         \
" Draws a rectangle with sides parallel to the main axes\n"         \
"\n"         \
"Usage: drawrect in.pgm x1 y1 z1 x2 y2 z2 out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Draws a rectangle (a box in 3D) with sides parallel to the main axes. \n"         \
"Two diagonally opposed corners are specified by \n"         \
"x1, y1, z1 and x2, y2, z2.\n"         \
"The rectangle is surimposed to the contents of in.pgm.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: draw\n"         \
"  draw\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__3dalpha__c__ "  3dalpha.c\n"         \
"\n"         \
" alpha-dilation of a 3d khalimsky order \n"         \
"\n"         \
"Usage: 3dalpha in.pgm out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Let X be the set of elements of the order  in.pgm, \n"         \
"the result is the union of alpha(x) for all x in X.\n"         \
"\n"         \
"Types supported: byte 3d\n"         \
"\n"         \
"Category: orders\n"         \
"  orders\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__curve2spline__c__ "  curve2spline.c\n"         \
"\n"         \
" converts a discrete open curve into a spline\n"         \
"\n"         \
"Usage: curve2spline curve.list [tolerance] out.spline\n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"Let C be the discrete open curve described in curve.list. This program finds a approximation of C\n"         \
"in the form of a parametric curve P defined by two (in 2D) or three (in 3D) splines \n"         \
"such that the maximal distance between P and C is beyond the given tolerance.\n"         \
"The default value for tolerance is 2.\n"         \
"\n"         \
"The result is given in the form of the list of control points for the splines, followed by\n"         \
"the set of coefficients for each spline segment. \n"         \
"The file format is the following for 2D:\n"         \
"\n"         \
"c n+1 (where n+1 denotes the number of control points)\n"         \
"x1 y1\n"         \
"...\n"         \
"xn+1 yn+1\n"         \
"C0X1 C0Y1 C1X1 C1Y1 C2X1 C2Y1 C3X1 C3Y1\n"         \
"...\n"         \
"C0Xn C0Yn C1Xn C1Yn C2Xn C2Yn C3Xn C3Yn\n"         \
"\n"         \
"and in the 3D case:\n"         \
"\n"         \
"C n+1 (where n+1 denotes the number of control points)\n"         \
"x1 y1 z1\n"         \
"...\n"         \
"xn+1 yn+1 zn+1\n"         \
"C0X1 C0Y1 C0Z1 C1X1 C1Y1 C1Z1 C2X1 C2Y1 C2Z1 C3X1 C3Y1 C3Z1\n"         \
"...\n"         \
"C0Xn C0Yn C0Zn C1Xn C1Yn C1Zn C2Xn C2Yn C2Zn C3Xn C3Yn C3Zn\n"         \
"\n"         \
"The ith segment (starting with i=0) of the parametric curve P is then defined by:\n"         \
"\n"         \
"x(t) = C3Xi * t^3 + C2Xi * t^2 + C1Xi * t + C0Xi\n"         \
"y(t) = C3Yi * t^3 + C2Yi * t^2 + C1Yi * t + C0Yi\n"         \
"z(t) = C3Zi * t^3 + C2Zi * t^2 + C1Zi * t + C0Zi\n"         \
"with t in [i,i+1]\n"         \
"\n"         \
" Closed curves are not allowed.\n"         \
"\n"         \
"Reference: \n"         \
"[Cou10] M. Couprie: \"Short note on natural cubic splines and discrete curves\", internal report, 2010.\n"         \
"\n"         \
"Types supported: list 2D, list 3D\n"         \
"\n"         \
"Category: draw geo\n"         \
"  draw geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__fits2pgm__o__ "WRITE ME!!!\n"         \
"\n"


#define doc__desfilsclairs3d__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__2dkhalimskize__c__ "  2dkhalimskize.c\n"         \
"\n"         \
" converts a 2D image into a 2D Khalimsky order\n"         \
"\n"         \
"Usage: 2dkhalimskize in.pgm {0|4|8|h|m|M|g|G|a} out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Converts a 2D image into a 2D Khalimsky order, or conversely.\n"         \
"\n"         \
"Modes 0, 4, 8 and h are for binary images. \n"         \
"\n"         \
"Modes m, M, g, G and a are for grayscale images. \n"         \
"\n"         \
"Mode = \n"         \
"    0 : only maximal elements,\n"         \
"    4 : emulates 4-connectivity, idem \"miss\", \n"         \
"    8 : emulates 8-connectivity (\"hit\" transform followed by a kernel transform),   \n"         \
"    h : \"hit\",\n"         \
"    m : min, \n"         \
"    M : max,\n"         \
"    g : gradient 2, \n"         \
"    G : gradient 4,\n"         \
"    a : average,\n"         \
"    R : reverse.\n"         \
"\n"         \
"In reverse mode (R), only the beta-terminal elements (squares) are selected.\n"         \
"\n"         \
"Types supported: byte 2d, long 2d, float 2d\n"         \
"\n"         \
"Category: orders\n"         \
"  orders\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__using_python__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__radialrankmaxopening__c__ "  radialrankmaxopening.c\n"         \
"\n"         \
" radial rank-max opening by line\n"         \
"\n"         \
"Usage: radialrankmaxopening in.pgm length rank [angle in degrees] [dilatmask.pgm] out.pgm\n"         \
"\n"         \
"Description: Radial rank-max opening by line (or dilated\n"         \
"line). Max between the rank-max openings (with parameter  rank) of\n"         \
" in.pgm by a linear structuring element rotated by angle until 360\n"         \
"degrees and dilated by dilatmask.pgm. The angle is normalized to an\n"         \
"exact division of 360 by an integer.\n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: morpho\n"         \
"  morpho\n"         \
"\n"         \
" André Vital Saude, Michel Couprie jan 2005\n"         \
"\n"         \
"\n"         \
"\n"


#define doc__compiling_make__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__redt__c__ "  redt.c\n"         \
"\n"         \
" reverse euclidean distance transform\n"         \
"\n"         \
"Usage: redt in.pgm out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Reverse euclidean distance transform, by the Coeurjolly's algorithm [1].\n"         \
"The input  in.pgm must be a quadratic euclidean distance map, or \n"         \
"an image of type int32_t where each non-null value correspond to the quadratic\n"         \
"radius of a disc or ball. The output is a binary image.\n"         \
"\n"         \
"[1] D. Coeurjolly,\n"         \
"\"d-Dimensional Reverse Euclidean Distance Transformation\n"         \
"and Euclidean Medial Axis Extraction in Optimal Time\", \n"         \
"DGCI 2003, LNCS 2886, pp. 327-337, 2003.\n"         \
"\n"         \
"Types supported: int32_t 2d, int32_t 3d\n"         \
"\n"         \
"Category: morpho\n"         \
"  morpho\n"         \
"\n"         \
" Michel Couprie 2004, Jean Cousty 2005 (3d)\n"         \
"\n"         \
"\n"


#define doc__test_juliette__cxx__ "WRITE ME!!!\n"         \
"\n"


#define doc__3drecons__c__ "  3drecons.c\n"         \
"\n"         \
" geodesic reconstruction in a 3d khalimsky order \n"         \
"\n"         \
"Usage: 3drecons g.list f.pgm out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Geodesic reconstruction (in the sense of the neighborhood relation theta) \n"         \
"of the set of points represented by the list  g.list in the set  f.pgm .\n"         \
"\n"         \
"Types supported: byte 3d\n"         \
"\n"         \
"Category: orders\n"         \
"  orders\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__planarity__c__ "  planarity.c\n"         \
"\n"         \
" computes a planarity measure for each connected component\n"         \
"\n"         \
"Usage: planarity in.pgm connex out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Labels each connected component with its planarity value.\n"         \
"Argument  connex defines the connectivity.\n"         \
"The result is a floating point image.\n"         \
"\n"         \
"Types supported: byte 3d\n"         \
"\n"         \
"Category: geo\n"         \
"  geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__crestheight__c__ "  crestheight.c\n"         \
"\n"         \
" measures the height of crests in a thinned grayscale image\n"         \
"\n"         \
"Usage: crestheight skel.pgm connex out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Let F be the input image (parameter skel.pgm), and\n"         \
"let R be the output image (parameter out.pgm).\n"         \
"For each point p which is separating for F (connexity\n"         \
"given by the parameter connex), the output value R(p) is\n"         \
"equal to calpha8m(F,S,p).\n"         \
"\n"         \
"References: \n"         \
"\"Topological operators for the detection of\n"         \
"curvilinar structures in grayscale images\", in preparation, 2003.\n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: topogray\n"         \
"  topogray\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__short2long__c__ "  short2long.c\n"         \
"\n"         \
" converts a \"short\" image to a \"int32_t\" image\n"         \
"\n"         \
"Usage: short2long in [out]\n"         \
"\n"         \
"Description: \n"         \
"\n"         \
"For each pixel x, out[x] = (int32_t)in[x].\n"         \
"\n"         \
"If the last argument  out is omitted, then out = in.\n"         \
"\n"         \
"Types supported: short 2d, short 3d.\n"         \
"\n"         \
"Category: convert\n"         \
" convert\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__hthin3d___notused_c__ "WRITE ME!!!\n"         \
"\n"


#define doc__moments__c__ "  moments.c\n"         \
"\n"         \
" calcul des moments d'ordre 1 et 2 d'une image binaire (nuage de points) \n"         \
"\n"         \
"Usage: moments in.pgm out.list\n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"\n"         \
"\n"         \
"Types supported: byte 2D\n"         \
"\n"         \
"Category: geo\n"         \
"  geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__ptWsimple__c__ "  ptWsimple.c\n"         \
"\n"         \
" detects W-simple points in a 2D binary image\n"         \
"\n"         \
"Usage: ptWsimple in.pgm connex out.pgm\n"         \
"\n"         \
"Description:\n"         \
"The argument  connex selects the connectivity (4, 8 in 2D).\n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: connect\n"         \
"  connect\n"         \
"\n"         \
" Jean Cousty (2007)\n"         \
"\n"         \
"\n"


#define doc__skelend__c__ "  skelend.c\n"         \
"\n"         \
" homotopic skeleton of a 2d or 3d binary image with dynamic detection of end points\n"         \
"\n"         \
"Usage: skelend in.pgm connex [n] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Homotopic skeletonization by iterative removal of simple,\n"         \
"non-end points. Breadth-first strategy.\n"         \
"During the first  n iterations (default 0), the end points\n"         \
"are removed as well.\n"         \
"If  n = -1, the end points are always removed.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" This operator is very sensitive to small contour irregularities. \n"         \
"See skelcurv for a more robust operator. Useful for reducing quasi-curvilinear objects.\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__seltopo__c__ "  seltopo.c\n"         \
"\n"         \
" selects points according to a local topological criterion \n"         \
"\n"         \
"Usage: seltopo in connex t- t+ tb- tb+ out\n"         \
"\n"         \
"Description:\n"         \
"Let X be the set of points of the binary image  in, let x be a point.\n"         \
"Let t(x) and tb(x) be the two connectivity numbers for x.\n"         \
"The result  out is the set of points:\n"         \
"{ x in X, t- \n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__segmentarea__c__ "  segmentarea.c\n"         \
"\n"         \
" segmentation by filtering components on an area criterion\n"         \
"\n"         \
"Usage: segmentarea in.pgm connex area [m] out.pgm\n"         \
"\n"         \
"Note: in the python front-end 'm' is mandatory, set 0 without maximization\n"         \
"and 1 with maximization.\n"         \
"\n"         \
"Description:\n"         \
"Segmentation by selection of cross-section components, based on an area criterion. \n"         \
"Connexity = connex ; area threshold = area.\n"         \
"\n"         \
"Segmentation is followed by a maximization if option m is set.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category:  connect\n"         \
"  connect\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__3dsphere__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__lthin__c__ "  lthin.c\n"         \
"\n"         \
" grayscale leveling thinning\n"         \
"\n"         \
"Usage: lthin in.pgm {imcond.pgm|null} connex niter out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Grayscale leveling thinning (refs. [BEC97, CBB01]).\n"         \
"The parameter  connex gives the connectivity used for the minima;\n"         \
"possible choices are 4, 8 in 2D and 6, 26 in 3D.\n"         \
"Let F be the function corresponding to the input image  in.pgm.\n"         \
"Let G be the function corresponding to the input image \n"         \
" imcond.pgm, or the null function if the keyword  null is used.\n"         \
"The algorithm is the following:\n"         \
"\n"         \
"\n"         \
"Repeat niter times:\n"         \
"    L = {(p,a) such that T--(p,F) = 1 and a = alpha-(p,F)}\n"         \
"    While L not empty\n"         \
"       extract a couple (p,a) from L\n"         \
"       F(p) = max{ a, alpha-(p,F), G(p) }    \n"         \
"Result: F\n"         \
"\n"         \
"\n"         \
"If  niter is set to -1, then the operator iterates until stability.\n"         \
"\n"         \
"References: \n"         \
"[BEC97] G. Bertrand, J. C. Everat and M. Couprie: \"Image segmentation through operators based upon topology\",  Journal of Electronic Imaging, Vol.&nbsp;6, No.&nbsp;4, pp.&nbsp;395-405, 1997.\n"         \
"[CBB01] M. Couprie, F.N. Bezerra, Gilles Bertrand: \"Topological operators for\n"         \
"grayscale image processing\",  Journal of Electronic Imaging, Vol.&nbsp;10, No.&nbsp;4, pp.&nbsp;1003-1015, 2001.\n"         \
"\n"         \
"Types supported: byte 2D, byte 3D.\n"         \
"\n"         \
"Category: topogray\n"         \
"  topogray\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"Example:\n"         \
"\n"         \
"lthin ur1 null 4 -1 ur1_lthin\n"         \
"minima ur1_lthin 4 ur1_lthin_m\n"         \
"\n"         \
"\n"         \
"  \n"         \
"      \n"         \
"      \n"         \
"  \n"         \
"  ur1\n"         \
"      ur1_lthin\n"         \
"      ur1_lthin_m\n"         \
"  \n"         \
"\n"         \
"\n"         \
"\n"


#define doc__skel2pov__c__ "  skel2pov.c\n"         \
"\n"         \
" generation of a 3d illustration from a curvilinear skeleton\n"         \
"\n"         \
"Usage: skel2pov in.skel out.pov\n"         \
"\n"         \
"Description:\n"         \
"Generation of a 3d illustration from a curvilinear skeleton.\n"         \
"\n"         \
"Types supported: 2Dskel, 3Dskel\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" Michel Couprie 2004\n"         \
"\n"         \
"\n"


#define doc__equal__c__ "  equal.c\n"         \
"\n"         \
" equality test\n"         \
"\n"         \
"Usage: equal in1.pgm in2.pgm\n"         \
"\n"         \
"Description:\n"         \
"This function returns 1 (no error) if for each pixel x, in1[x] == in2[x]; \n"         \
"otherwise it returns 0.\n"         \
"Images must be of the same dimensions.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n"         \
"\n"         \
"Category: arith\n"         \
"  arith\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__drawfield__c__ "  drawfield.c\n"         \
"\n"         \
" draw line segments that represent a vector field\n"         \
"\n"         \
"Usage: drawfield in.pgm [len] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"The file  in.pgm contains a vector field. This operator generates a (binary) image where each vector is represented by a straight line segment of length  len (default 10).\n"         \
"\n"         \
"Types supported: byte 2D, byte 3D\n"         \
"\n"         \
"Category: draw\n"         \
"  draw\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__remspnoise__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__lfclose_rect__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__segment__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__delcomp___notused_c__ "  delcomp.c\n"         \
"\n"         \
" deletes a connected component\n"         \
"\n"         \
"Usage: delcomp in.pgm x y z v out.pgm\n"         \
"\n"         \
"Description:\n"         \
"The connected component of the binary image in.pgm\n"         \
"which contains the point (x,y,z)\n"         \
"is deleted.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: connect, topobin\n"         \
"  connect, topobin\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__traceellipses__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__ptextensible__c__ "  ptextensible.c\n"         \
"\n"         \
" detects extensible points in a grayscale image\n"         \
"\n"         \
"Usage: ptextensible im.pgm {imcond.pgm|null} connex fileout.pgm\n"         \
"\n"         \
"Description:\n"         \
"Extensible point detection.\n"         \
"The input image  im.pgm must be a \"thin\" grayscale image, as\n"         \
"the output of the operator  hthin.\n"         \
"The parameter  connex indicates the cconnectivity used for the minimal \n"         \
"regions.\n"         \
"The optional parameter  imcond.pgm is a binary image which indicates the points\n"         \
"in the neighborhood of which the extensible points will be searched.\n"         \
"\n"         \
"Reference: \n"         \
"M. Couprie, F.N. Bezerra, Gilles Bertrand: \"Topological operators for\n"         \
"grayscale image processing\",  Journal of Electronic Imaging, Vol.&nbsp;10, No.&nbsp;4, pp.&nbsp;1003-1015, 2001.\n"         \
"\n"         \
"Types supported: byte 2D\n"         \
"\n"         \
"Category: topogray\n"         \
"  topogray\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__wshedval__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__skel_ACK3b___notused_c__ "  skel_ACK3b.c\n"         \
"\n"         \
" parallel 3D binary curvilinear, asymetric skeleton based on thin 1D isthmus\n"         \
"\n"         \
"Usage: skel_ACK3b in.pgm nsteps isthmus_persistence [inhibit] out.pgm\n"         \
"\n"         \
"Description: Parallel 3D binary thinning or curvilinear,\n"         \
"asymetric skeleton based on thin 1D isthmus. The parameter  nsteps\n"         \
"gives, if positive, the number of parallel thinning steps to be\n"         \
"processed.  If the value given for  nsteps equals -1, the thinning\n"         \
"is continued until stability.\n"         \
"\n"         \
"When a point x is detected as a 1D isthmus, a counter p(x) is\n"         \
"associated to this point and initialized with value 1. This counter is\n"         \
"incremented a each iteration as long as x is still an isthmus. At each\n"         \
"iteration, the isthmuses x such that p(x) >=  isthmus_persistence are\n"         \
"stored as a constraint set (see also  inhibit parameter).\n"         \
"\n"         \
"If the parameter  inhibit is given and is a binary image name,\n"         \
"then the points of this image will be left unchanged. \n"         \
"\n"         \
" The object must not have any point on the frame of the image.\n"         \
"\n"         \
"Types supported: byte 3d\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__inf__c__ "  inf.c\n"         \
"\n"         \
" pixelwise inf predicate\n"         \
"\n"         \
"Usage: inf in1.pgm in2.pgm out.pgm\n"         \
"\n"         \
"Description:\n"         \
"For each pixel x, if in1[x] \n"         \
"Images must be of the same type and same dimensions.\n"         \
"\n"         \
"Types supported: byte 2D, byte 3D\n"         \
"\n"         \
"Category: arith\n"         \
"  arith\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__fits2pgm__ "WRITE ME!!!\n"         \
"\n"


#define doc__gradsup__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__hfilling__c__ "  hfilling.c\n"         \
"\n"         \
" homotopic filling (pseudo closing) by a ball\n"         \
"\n"         \
"Usage: hfilling in.pgm r connex out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Homotopic filling (pseudo closing) by a ball of radius  r.\n"         \
"\n"         \
"Reference: \n"         \
"[CB04] M. Couprie and G. Bertrand:\n"         \
"Topology preserving alternating sequential filter for smoothing \n"         \
"2D and 3D objects,  Journal of Electronic Imaging, Vol.&nbsp;13, No.&nbsp;4, pp.&nbsp;720-730, 2004.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: morpho, topobin\n"         \
"  morpho, topobin\n"         \
"\n"         \
" Michel Couprie 2002\n"         \
"\n"         \
"\n"         \
"\n"


#define doc__deriche3d__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__skelsurf__c__ "  skelsurf.c\n"         \
"\n"         \
" surfacic binary skeleton guided by a priority image\n"         \
"\n"         \
"Usage: skelsurf in.pgm prio connex [inhibit] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Surfacic binary skeleton guided by a priority image.\n"         \
"The lowest values of the priority image correspond to the highest priority.\n"         \
"\n"         \
"The parameter  prio is either an image (byte or int32_t), or a numerical code\n"         \
"indicating that a distance map will be used as a priority image; \n"         \
"the possible choices are:\n"         \
" 0: approximate euclidean distance\n"         \
" 1: approximate quadratic euclidean distance\n"         \
" 2: chamfer distance\n"         \
" 3: exact quadratic euclidean distance\n"         \
" 6: 6-distance in 3d\n"         \
" 18: 18-distance in 3d\n"         \
" 26: 26-distance in 3d\n"         \
"\n"         \
"The parameter  connex indicates the connectivity of the binary object.\n"         \
"Possible choices are 6, 26.\n"         \
"\n"         \
"If the parameter  inhibit is given and is a binary image name,\n"         \
"then the points of this image will be left unchanged. \n"         \
"\n"         \
"Let X be the set corresponding to the input image  in.pgm.\n"         \
"Let P be the function corresponding to the priority image.\n"         \
"Let I be the set corresponding to the inhibit image, if given, or the empty\n"         \
"set otherwise.\n"         \
"The algorithm is the following:\n"         \
"\n"         \
"\n"         \
"C = {y in F | Tb(y) > 1}\n"         \
"Repeat until stability\n"         \
"  choose a point x in X, simple for X, such that C[x] == 0 \n"         \
"    and such that P[x] is minimal\n"         \
"  X = X  {x}\n"         \
"  for all y in gamma(x)\n"         \
"    if Tb(y) > 1 then C[y] = 1\n"         \
"Result: X\n"         \
"\n"         \
"\n"         \
"where Tb(y) refers to the second connectivity number of y, that is, \n"         \
"(informally) the number of connected components of gamma(y) inter complement(X).\n"         \
"\n"         \
"References: \n"         \
"[BC07] G. Bertrand and M. Couprie: \"Transformations topologiques discretes\", in G&eacute;om&eacute;trie discr&egrave;te et images num&eacute;riques, D. Coeurjolly and A. Montanvert and J.M. Chassery, pp.&nbsp;187-209, Herm&egrave;s, 2007.\n"         \
"\n"         \
"Types supported: byte 3d\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__segmentnumi__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__tempNd__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__mcm2ac__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__genimage__c__ "  genimage.c\n"         \
"\n"         \
" generates a monocolor image of given size\n"         \
"\n"         \
"Usage: genimage {in.pgm|rs cs ds} col out.pgm\n"         \
"\n"         \
"Description:\n"         \
"The size of the result image out.pgm is taken from image\n"         \
"in.pgm, or given by the parameters rs, cs, ds. \n"         \
"For a 2D image, the parameter ds (depth size) is set to 1.\n"         \
"The color of the result image is given by the parameter col.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: arith, geo\n"         \
"  arith, geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__pgm2list__c__ "  pgm2list.c\n"         \
"\n"         \
" converts from pgm format to list format\n"         \
"\n"         \
"Usage: pgm2list image.pgm {e|s|b|n|B|N} out.list\n"         \
"\n"         \
"Description:\n"         \
"Converts from pgm format to list format:\n"         \
" e: set of points\n"         \
" s: 1d digital signal\n"         \
" b: binary 2D image\n"         \
" n: grayscale 2D image\n"         \
" B: binary 3D image\n"         \
" N: grayscale 3D image\n"         \
"\n"         \
"In formats  e,  b and  B, only the points with non-null values are \n"         \
"considered.\n"         \
"\n"         \
"In formats  s,  n and  N, all the points (even those having a \n"         \
"null value) are considered.\n"         \
"\n"         \
"Summary of list formats:\n"         \
"  \n"         \
"  e &lt;n&gt;       s &lt;n&gt;         b &lt;n&gt;         n &lt;n&gt;            B &lt;n&gt;            N &lt;n&gt;    \n"         \
"  x1          x1 v1         x1 y1         x1 y1 v1         x1 y1 z1         x1 y1 z1 v1\n"         \
"  x2    or    x2 v2   or    x2 y2   or    x2 y2 v2   or    x2 y2 z2   or    z2 x2 y2 v2\n"         \
"  ...         ...           ...           ...              ...              ...\n"         \
"  xn          xn vn         xn yn         xn yn vn         xn yn z3         z3 xn yn vn\n"         \
"\n"         \
"Types supported: byte 2D, byte 3D, int32_t 2D, int32_t 3D, float 2D, float 3D\n"         \
"\n"         \
"Category: convert\n"         \
"  convert\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__3dsurfacecollapse__c__ "  3dsurfacecollapse.c\n"         \
"\n"         \
" parallel directional constrained collapse with surface detection and preservation\n"         \
"\n"         \
"Usage: 3dsurfacecollapse in.pgm mode nsteps [inhibit] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Parallel directional constrained collapse with surface detection and preservation.\n"         \
"\n"         \
"The parameter  mode selects the strategy used to detect surface elements.\n"         \
"The possible choices are:\n"         \
" 1: any 2-dimensional facet\n"         \
"\n"         \
"The parameter  nsteps gives the number of \"layers\" to be removes, if\n"         \
"the value is -1 then the interations continue until stability.\n"         \
"\n"         \
"If the parameter  inhibit is given and is a binary image name,\n"         \
"then the points of this image will be left unchanged. \n"         \
"\n"         \
"Types supported: byte 3d\n"         \
"\n"         \
"Category: orders\n"         \
"  orders\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__dynamiquefilter__c__ "  dynamiquefilter.c\n"         \
"\n"         \
" filter components according to the dynamics of the maxima\n"         \
"\n"         \
"Usage: dynamiquefilter in.pgm connex value [order] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Computes the ordered dynamics of the maxima, with connectivity connex, \n"         \
"selects the maxima with a dynamics greater or equal to value,\n"         \
"and find the maximal components which includes these maxima. \n"         \
"The definition of the ordered dynamics is the one given in [Ber05]. \n"         \
"The optional argument order is one of the following:\n"         \
" 0: altitude [default] \n"         \
" 1: area\n"         \
" 2: volume\n"         \
"\n"         \
"References: \n"         \
"[Ber05] G. Bertrand: \"A new definition of the dynamics\", Procs. ISMM05, Springer, series Computational Imaging and Vision, Vol.&nbsp;30, pp.&nbsp;197-206, 2005.\n"         \
"\n"         \
"Types supported: byte 2D, byte 3D.\n"         \
"\n"         \
"Category: \n"         \
" connect\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__pgm2list1d__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__skel_SCK3_pers__c__ "  skel_SCK3_pers.c\n"         \
"\n"         \
" topological persistence of 1D isthmuses, based on parallel 3D asymetric thinning\n"         \
"\n"         \
"Usage: skel_SCK3_pers in.pgm out.pgm\n"         \
"\n"         \
"Description: Topological persistence of 1D isthmuses, based on \n"         \
"parallel 3D asymetric thinning.\n"         \
"\n"         \
"When a point x is detected as a 1D isthmus, a counter p(x) is\n"         \
"associated to this point and initialized with value 1. This counter is\n"         \
"incremented a each iteration as long as x is still an isthmus. When this point x is\n"         \
"eventually deleted, the value of the counter is freezed.\n"         \
"\n"         \
" The object must not have any point on the frame of the image.\n"         \
"\n"         \
"Types supported: byte 3d\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__0000__c__ "  \n"         \
"\n"         \
"Pink is an image processing library developed\n"         \
"at ESIEE Engineering for research\n"         \
"and teaching purposes. It contains implementations of over 200\n"         \
"algorithms for image segmentation and filtering. Most of the operators\n"         \
"come from mathematical morphology, but it contains operators from\n"         \
"different fields. It is a free software licensed under the\n"         \
" CeCILL \n"         \
"license. \n"         \
"\n"         \
"In august 2011 Pink has been presented at the European Python\n"         \
"Scientific Conference as a poster. Below you can see the conference\n"         \
"poster (also in  pdf  and hi-res  jpeg ).\n"         \
"\n"         \
" html pink_euroscipy_web_thumb.jpeg \"EuroSciPy 2011 poster\"\n"         \
"\n"         \
" screenshots Screenshots\n"         \
"\n"         \
"\n"         \
"Screenshots can be found here. \n"         \
"\n"         \
" download Download\n"         \
"\n"         \
"The source code and the binary packages can be accessed from the \n"         \
"download page.\n"         \
"\n"         \
" mailing Mailing list \n"         \
"\n"         \
"If you are interested in the daily life of Pink, please subscribe to\n"         \
"the \n"         \
"Pink Developer mailing list . You can also post questions\n"         \
"concerning the usage or raise issues. \n"         \
"\n"         \
" bughunt Bug-hunt!\n"         \
"\n"         \
"If you have discovered a bug, please report it at\n"         \
"the \n"         \
"bug tracker.\n"         \
"\n"         \
" credits Credits\n"         \
"\n"         \
"\n"         \
"   Code licensed under CeCILL license\n"         \
"   \n"         \
"     Michel Couprie \n"         \
"      Laurent Najman : localextrema, saliency\n"         \
"          Hugues Talbot  : fmm \n"         \
"      Jean\n"         \
"    Cousty : redt 3d (reverse euclidean distance transform -\n"         \
"    algo de D. Coeurjolly), watershedthin, opÃ©rateurs sur les graphes\n"         \
"    d'arÃªtes (GA), forÃªts de poids min (MSF), waterfall,\n"         \
"    recalagerigide_translateplane\n"         \
"     Xavier Daragon: dist, distc (distance euclidienne quadratique 3D)\n"         \
"     AndrÃ© Vital Saude: radialopening, divers scripts tcl, hma\n"         \
"     Nicolas Combaret: toposhrinkgray, ptselectgray\n"         \
"     John Chaussard: lballincl, cropondisk, shrinkondisk\n"         \
"     Christophe Doublier: zoomint\n"         \
"     Hildegard Koehler: lintophat\n"         \
"     CÃ©dric AllÃ¨ne: gettree, histolisse, labeltree, nbcomp, pgm2vtk, seuilauto\n"         \
"     Gu Jun: maxdiameter\n"         \
"     SÃ©bastien Couprie: mcsplines.c\n"         \
"     Rita Zrour: medialaxis (axe mÃ©dian euclidien exact - algo de RÃ©my-Thiel),\n"         \
"      dist, distc (distance euclidienne quadratique exacte - algo de Saito-Toriwaki)\n"         \
"     Laurent Mercier: gestion d'un masque dans delaunay\n"         \
"      Laszlo Marak (ujoimro) : continuous maximum flows,\n"         \
"    Unger flows, Python front-end, native Microsoft Windows port\n"         \
"    Benjamin Raynal: parallel 3D thinning\n"         \
"    Nivando Bezerra: parallel grayscale thinning\n"         \
"   \n"         \
"   Code under different free software licenses\n"         \
"    \n"         \
"       David Coeurjolly: lvoronoilabelling.c\n"         \
"       Dario Bressanini: mcpowell.c\n"         \
"       Andrew W. Fitzgibbon: lbresen.c\n"         \
"       Lilian Buzer: lbdigitalline.cxx\n"         \
"   \n"         \
"\n"         \
" compiling Compiling\n"         \
"\n"         \
"Pink can be compiled with two sets of tools. For the compilation with\n"         \
"dependency checking and Python front-end, visit the page \n"         \
"compiling_cmake. For the classical compilation look at \n"         \
"compiling_make. You can also compile Pink on windows (including the\n"         \
"Python front-end). For details on compiling on windows look at \n"         \
"compiling_windows. \n"         \
"\n"         \
"\n"         \
" development Development\n"         \
"For developing Pink look at the  dev_conventions page. For\n"         \
"exporting functions in Python look at the  python_export page. \n"         \
"\n"         \
" dependencies Dependencies\n"         \
"For optimal use, the following packages should be installed:\n"         \
"\n"         \
"imview\n"         \
"Python\n"         \
"Doxygen\n"         \
"ActiveTcl 8.3\n"         \
"VTK\n"         \
"MPlayer\n"         \
"Gnuplot\n"         \
"\n"         \
"Note, that the detailed dependencies can be found in file\n"         \
"'dependencies'.\n"         \
"\n"         \
"The Python front-end has been financially supported\n"         \
"by  EDF .  The\n"         \
"project management is alpha-hosted\n"         \
"at \n"         \
"\n"         \
"width=\"124\" height=\"32\" border=\"0\" alt=\"BerliOS Logo\" />BerliOs.\n"         \
"\n"         \
"\n"         \
"Michel Couprie  -  Professeur  -  ESIEE Paris\n"         \
"Laboratoire d'Informatique Gaspard-Monge, UniversitÃ© Paris-Est\n"         \
"ESIEE 2, Bd Blaise Pascal - B.P. 99\n"         \
"93162 Noisy-Le-Grand CEDEX\n"         \
"m(dot)couprie(at)esiee(dot)fr \n"         \
"url:  http://www.esiee.fr/~coupriem\n"         \
" \n"         \
"\n"         \
" inter Interactive operators\n"         \
" \n"         \
"\n"         \
" arith Arithmetic operators\n"         \
" \n"         \
"\n"         \
" convert Format and type conversion \n"         \
"\n"         \
"\n"         \
" morpho Mathematical morphology\n"         \
"\n"         \
"\n"         \
" connect Digital connectivity\n"         \
"\n"         \
"\n"         \
" topobin Digital topology (binary)\n"         \
"\n"         \
"\n"         \
" topogray Digital topology (grayscale)\n"         \
"\n"         \
"\n"         \
" orders Orders topology\n"         \
"\n"         \
"\n"         \
" geo Geometrical operators\n"         \
"\n"         \
"\n"         \
" draw Graphic primitives\n"         \
"\n"         \
"\n"         \
" histo Histogram-based operators\n"         \
"\n"         \
"\n"         \
" signal Signal processing\n"         \
"\n"         \
"\n"         \
" stats Statistics\n"         \
"\n"         \
"\n"         \
" mesh3d Three-dimensional meshing\n"         \
"\n"         \
"\n"         \
" development Development\n"         \
"\n"         \
"\n"


#define doc__projsphere__c__ "  projsphere.c\n"         \
"\n"         \
" projection on a sphere\n"         \
"\n"         \
"Usage: projsphere in.pgm out.pgm\n"         \
"\n"         \
"Description: Projection on a sphere.\n"         \
"\n"         \
"Types supported: byte 2D, byte 3D\n"         \
"\n"         \
"Category: geo\n"         \
"  geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__float2long__c__ "  float2long.c\n"         \
"\n"         \
" converts a \"float\" image to a \"int32_t\" image\n"         \
"\n"         \
"Usage: float2long in.pgm offset factor out.pgm\n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"Applies the following tranformation to each value x in  in.pgm :\n"         \
"\n"         \
"X = round(offset + factor * x)\n"         \
"\n"         \
"Types supported: float 2d, float 3d\n"         \
"\n"         \
"Category: convert\n"         \
"  convert\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__skel2pgm__c__ "  skel2pgm.c\n"         \
"\n"         \
" generation of a labelled image from a curvilinear skeleton\n"         \
"\n"         \
"Usage: skel2pgm in.skel [id] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Generation of a labelled image from a curvilinear skeleton.\n"         \
"\n"         \
"If the optional argument  id is given and positive, then only the skeleton element having this index is considered.\n"         \
"\n"         \
"If the optional argument  id is given and is -1, then points of the skeleton are labelled with their index in the skeleton structure (warning: no more than 255 elements)\n"         \
"\n"         \
"Otherwise, points of the skeleton are labelled with:\n"         \
" 1: isolated\n"         \
" 2: end\n"         \
" 3: curve\n"         \
" 4: junction\n"         \
"\n"         \
"Types supported: 2Dskel, 3Dskel\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" Michel Couprie 2004\n"         \
"\n"         \
"\n"


#define doc__lfopen3d_line__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__min1__c__ "  min1.c\n"         \
"\n"         \
" return the minimum of the pixel values of an image\n"         \
"\n"         \
"Usage: min1 in.pgm [out.list]\n"         \
"\n"         \
"Description:\n"         \
"This function returns (in the list out.list) \n"         \
"the minimum of the pixel values of the image  in.pgm .\n"         \
"\n"         \
"If the parameter  out.list is ommitted, the result is printed on the standard output.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n"         \
"\n"         \
"Category: arith\n"         \
"  arith\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__3dskelsurf__c__ "  3dskelsurf.c\n"         \
"\n"         \
" surfacic skeleton\n"         \
"\n"         \
"Usage: in.pgm n out.pgm\n"         \
"\n"         \
"Description: Surfacic skeleton in the sense defined in ref. [LB99]\n"         \
"The parameter  n gives the number of parallel steps of simple,\n"         \
"non surface end point deletions.\n"         \
"\n"         \
"Reference: \n"         \
"[LB99] C. Lohou, Gilles Bertrand: \"Poset approach to 3D parallel thinning\", SPIE Vision Geometry VIII, Vol.&nbsp;3811, pp.&nbsp;45-56, 1999.\n"         \
"\n"         \
"Types supported: byte 3d\n"         \
"\n"         \
"Category: orders\n"         \
"  orders\n"         \
"\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__2latex__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__dilatnum__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__skel_ACK3__c__ "  skel_ACK3.c\n"         \
"\n"         \
" parallel 3D binary curvilinear, asymetric skeleton based on 3D and 2D residuals\n"         \
"\n"         \
"Usage: skel_ACK3 in.pgm nsteps [inhibit] out.pgm\n"         \
"\n"         \
"Description: Parallel 3D binary thinning or curvilinear,\n"         \
"asymetric skeleton based on 3D and 2D residuals. The parameter  nsteps\n"         \
"gives, if positive, the number of parallel thinning steps to be\n"         \
"processed.  If the value given for  nsteps equals -1, the thinning\n"         \
"is continued until stability.\n"         \
"\n"         \
"If the parameter  inhibit is given and is a binary image name,\n"         \
"then the points of this image will be left unchanged. \n"         \
"\n"         \
"Warning: The object must not have any point on the frame of the image.\n"         \
"\n"         \
"Types supported: byte 3d\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__projy___notused_c__ "WRITE ME!!!\n"         \
"\n"


#define doc__drawellipse__c__ "  drawellipse.c\n"         \
"\n"         \
" Draws an ellipse\n"         \
"\n"         \
"Usage: drawellipse in.pgm x1 y1 x2 y2 X1 Y1 X2 Y2 [filled] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Draws an  ellipse specified by four points, e.g. the extremities of the small axis \n"         \
"(x1, y1, x2, y2) and the extremities of the big axis \n"         \
"(X1, Y1, X2, Y2).\n"         \
"The ellipse is surimposed to the contents of in.pgm.\n"         \
"If the (optional) int32_t parameter  filled is set (1), then the ellipse is filled.\n"         \
"Otherwise it is empty (default).\n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: draw geo\n"         \
"  draw geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__watershedMeyer4D__c__ "  watershedMeyer4D.c\n"         \
"\n"         \
" watershed transformation (Meyer's algorithm)\n"         \
"\n"         \
"Usage: watershedMeyer4D prefix_in first last prefix_mark {prefix_bgmark|null} {prefix_roi|null} connex prefix_out\n"         \
"\n"         \
"Description:\n"         \
"Performs the watershed transformation on the 4D image made of the concatenation of the 3D images prefix_inxxxx.pgm (where nnnn is a four digit decimal integer and where first and last specify respectively the first and the last volume).The watershed is computed taking the\n"         \
"set of markers in prefix_markxxxx.pgm. \n"         \
"If this parameter is present, prefix_bgmarkxxxx.pgm\n"         \
"is used as a set of markers for the background.\n"         \
"If this parameter is present, prefix_roixxxx.pgm\n"         \
"indicates the region of interest on which the operation is performed.\n"         \
"The parameter connex gives the adjacency relation (8 in 4D) \n"         \
"for the makers.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: connect\n"         \
" connect\n"         \
"\n"         \
" Jean Cousty - fevrier 2005\n"         \
"\n"         \
"\n"


#define doc__erosplan__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__3dborder__c__ "  3dborder.c\n"         \
"\n"         \
" extracts the border of an object in H3\n"         \
"\n"         \
"Usage: 3dborder in.pgm out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Extracts the border of an object in H3. The (internal) border is defined as the set \n"         \
"of the object points which have at least one neighboring background point. \n"         \
"\n"         \
"Types supported: byte 3d\n"         \
"\n"         \
"Category: topo \n"         \
"  topo \n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__hk_tophat_lin_rotall___notused_c__ "WRITE ME!!!\n"         \
"\n"


#define doc__meshaddnoise__c__ "  meshaddnoise.c\n"         \
"\n"         \
" adds gaussian noise to the positions of the vertices of a mesh\n"         \
"\n"         \
"Usage: meshaddnoise in alpha out\n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"Adds gaussian noise to the positions of the vertices of a mesh.\n"         \
"The mean of the gaussian is 0.\n"         \
"The standard deviation of the gaussian is given by alpha. \n"         \
"Available input formats: mcm, ifs, vtk, ac.\n"         \
"Available output formats: mcm, vtk.\n"         \
"\n"         \
"Types supported: mesh 3d\n"         \
"\n"         \
"Category: mesh3d\n"         \
"  mesh3d\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__wshedkhalimsky__c__ "  wshedkhalimsky.c\n"         \
"\n"         \
" watershed transformation in Khalimsky space (inter pixel watershed)\n"         \
"\n"         \
"Usage: watershed in mark {bgmark|null} {roi|null} out\n"         \
"\n"         \
"Description:\n"         \
"Performs the watershed transformation on the image in.pgm, taking the\n"         \
"set of markers in mark.pgm. \n"         \
"If this parameter is present, bgmark.pgm\n"         \
"is used as a set of markers for the background.\n"         \
"If this parameter is present, roi\n"         \
"indicates the region of interest on which the operation is performed.\n"         \
"All images must be previously transformed in the khalimsky space with a max strategy.\n"         \
"The output image is in khalimsky space too.\n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: connect orders\n"         \
" connect orders\n"         \
"\n"         \
" Michel Couprie & Christophe Doublier\n"         \
"\n"         \
"\n"


#define doc__linapprox__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__dilatplan__c_notused_c__ "WRITE ME!!!\n"         \
"\n"


#define doc__MSF4d__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__lfdilat_line__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__printstats__c__ "  printstats.c\n"         \
"\n"         \
" prints some stats of an image or a region\n"         \
"\n"         \
"Usage: printstats in.pgm [mask.pgm]\n"         \
"\n"         \
"Description:\n"         \
"Calculates the histogram of  im.pgm (masked by the binary image\n"         \
" mask.pgm, if given) and prints it on the screen.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d\n"         \
"\n"         \
"Category: histo\n"         \
"  histo\n"         \
"\n"         \
" Laurent Najman\n"         \
"\n"         \
"\n"


#define doc__grayskel__c__ "  grayskel.c\n"         \
"\n"         \
" grayscale homotopic skeleton\n"         \
"\n"         \
"Usage: grayskel in.pgm {imcond.pgm|null} connex lambda out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Skeleton, either homotopic (lambda = 0) or non-homotopic, for 2D grayscale images\n"         \
"The skeleton is obtained by iterative lowering of lambda-destructible points\n"         \
"(see [CBB01]).\n"         \
"\n"         \
"Reference: \n"         \
"[CBB01] M. Couprie, F.N. Bezerra, Gilles Bertrand: \"Topological operators for\n"         \
"grayscale image processing\",  Journal of Electronic Imaging, Vol.&nbsp;10, No.&nbsp;4, pp.&nbsp;1003-1015, 2001.\n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: topogray\n"         \
"  topogray\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__line__c__ "  line.c\n"         \
"\n"         \
" Draws a straight line between the two first points found in image.\n"         \
"\n"         \
"Usage: line in.pgm out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Draws a straight line between the two first points found in image in.pgm.\n"         \
"Uses the Bresenham's algorithm.\n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: draw\n"         \
"  draw\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__skel_CK3p__c__ "  skel_CK3p.c\n"         \
"\n"         \
" parallel 3D binary curvilinear skeleton based on 1D isthmuses\n"         \
"\n"         \
"Usage: skel_CK3p in.pgm pers [inhibit] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Parallel 3D binary thinning or curvilinear skeleton based on 1D isthmuses, \n"         \
"with a persistence parameter:  pers.\n"         \
"\n"         \
"If the parameter  inhibit is given and is a binary image name,\n"         \
"then the points of this image will be left unchanged. \n"         \
"\n"         \
"Warning: The object must not have any point on the frame of the image.\n"         \
"\n"         \
"Types supported: byte 3d\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__pgm2skel2__c__ "  pgm2skel2.c\n"         \
"\n"         \
" decomposition of a curvilinear skeleton into isolated points,\n"         \
"end points, curves and junctions\n"         \
"\n"         \
"Usage: pgm2skel in.pgm junc.pgm connex out.skel\n"         \
"\n"         \
"Description: \n"         \
"The skeleton found in  in.pgm is decomposed into isolated points,\n"         \
"end points, curves and junctions ; and its description is stored in \n"         \
" out.skel .\n"         \
"\n"         \
"The parameter  connex sets the adjacency relation used for the object\n"         \
"(4, 8 (2d) or 6, 18, 26 (3d)).\n"         \
"\n"         \
"The image given as parameter junc.pgm contains curve points that\n"         \
"will artificially considered as junction points.\n"         \
"\n"         \
" Points at the border of the image will be ignored.\n"         \
"\n"         \
" IMPORTANT LIMITATION: \n"         \
"different junctions in the original image must not be in direct\n"         \
"contact with each other (i.e., connected) otherwise they will be\n"         \
"considered as a single junction. To prevent this to occur, one can\n"         \
"increase image resolution.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" Michel Couprie 2009\n"         \
"\n"         \
"\n"


#define doc__extractline__c__ "  extractline.c\n"         \
"\n"         \
" extracts a line between two given points from a 3D image\n"         \
"\n"         \
"Usage: extractline in.pgm x1 y1 x2 y2 out.pgm\n"         \
"\n"         \
"Description:\n"         \
"The original 2D image is in.pgm. \n"         \
"The output 1D image out.pgm contains the line between \n"         \
"points (x1,y1) and (x2,y2) extracted from in.pgm.\n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: geo\n"         \
"  geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__2dtopoflow__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__drawspline__c__ "  drawspline.c\n"         \
"\n"         \
" draw a spline which is specified by its control points\n"         \
"\n"         \
"Usage: drawspline in.pgm spline.txt [len] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Draws a spline which is specified by its control points.\n"         \
"The control points are read in the text file  spline.txt .\n"         \
"The parameter  in.pgm gives an image into which the spline is to be drawn.\n"         \
"The file format for  spline.txt is the following for 2D:\n"         \
"\n"         \
"c n+1 (where n+1 denotes the number of control points)\n"         \
"x1 y1\n"         \
"...\n"         \
"xn+1 yn+1\n"         \
"C0X1 C0Y1 C1X1 C1Y1 C2X1 C2Y1 C3X1 C3Y1\n"         \
"...\n"         \
"C0Xn C0Yn C1Xn C1Yn C2Xn C2Yn C3Xn C3Yn\n"         \
"\n"         \
"and in the 3D case:\n"         \
"\n"         \
"C n+1 (where n+1 denotes the number of control points)\n"         \
"x1 y1 z1\n"         \
"...\n"         \
"xn+1 yn+1 zn+1\n"         \
"C0X1 C0Y1 C0Z1 C1X1 C1Y1 C1Z1 C2X1 C2Y1 C2Z1 C3X1 C3Y1 C3Z1\n"         \
"...\n"         \
"C0Xn C0Yn C0Zn C1Xn C1Yn C1Zn C2Xn C2Yn C2Zn C3Xn C3Yn C3Zn\n"         \
"\n"         \
"If parameter  len is given and non-zero, the spline is extended on both sides by straight line segments of length  len. \n"         \
"\n"         \
"Types supported: byte 2D, byte 3D\n"         \
"\n"         \
"Category: draw geo\n"         \
"  draw geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__byte2long__c__ "  byte2long.c\n"         \
"\n"         \
" converts a \"byte\" image to a \"int32_t\" image\n"         \
"\n"         \
"Usage: byte2long in [out]\n"         \
"\n"         \
"Description: \n"         \
"\n"         \
"For each pixel x, out[x] = (int32_t)in[x].\n"         \
"\n"         \
"If the last argument  out is omitted, then out = in.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d.\n"         \
"\n"         \
"Category: convert\n"         \
" convert\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__volselnb__c__ "  volselnb.c\n"         \
"\n"         \
" volume based segmentation by selection of a given number or components\n"         \
"\n"         \
"Usage: volselnb in.pgm connex nb out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Segmentation by selection of a given number or components, based on a \n"         \
"volume criterion. \n"         \
"Connexity = connex ; desired number of components = nb.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: connect\n"         \
" connect\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__label__c__ "  label.c\n"         \
"\n"         \
" labeling of some flat zones of a grayscale or a binary image\n"         \
"\n"         \
"Usage: label in.pgm connex {fgd|bgd|min|max|pla} out.pgm\n"         \
"\n"         \
"Description:\n"         \
"The argument  connex selects the connectivity (4, 8 in 2D; 6, 18, 26 in 3D).\n"         \
"The argument  selects the kind of flat zone\n"         \
"to be labeled:\n"         \
"  fgd selects foreground components for a binary image\n"         \
"  bgd selects background components for a binary image\n"         \
"  min selects regional minima\n"         \
"  max selects regional maxima\n"         \
"  pla selects all flat zones (plateaux).\n"         \
"The output image  out.pgm has the type \"int32_t\".\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d\n"         \
"\n"         \
"Category: connect\n"         \
"  connect\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__despics3d__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__meshoffset__c__ "  meshoffset.c\n"         \
"\n"         \
" applies an offset to all points of a mesh (translation)\n"         \
"\n"         \
"Usage: meshoffset in [ox oy oz] out\n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"Applies an offset to all points of a mesh (translation). \n"         \
"Available input formats: vtk.\n"         \
"Available output formats: vtk.\n"         \
"\n"         \
"Types supported: mesh 3d\n"         \
"\n"         \
"Category: mesh3d\n"         \
"  mesh3d\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__pgm2pov__c__ "  pgm2pov.c\n"         \
"\n"         \
" generates a povray file from a 3D image\n"         \
"\n"         \
"Usage: pgm2pov in.pgm mode out.pov\n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"mode = \n"         \
"   0 (none)\n"         \
"   1 (between points)\n"         \
"   2 (everywhere)\n"         \
"   3 (6-links)\n"         \
"   4 (26-links)\n"         \
"   5 (cubes)\n"         \
"   6 (triangles)\n"         \
"   10-14: idem 0-4, khalimsky grid\n"         \
"   15: colored khalimsky \n"         \
"\n"         \
"Types supported: byte 3d\n"         \
"\n"         \
"Category: convert\n"         \
"  convert\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__skel_NK2__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__2dlabel__c__ "  2dlabel.c\n"         \
"\n"         \
" labels the theta-connected component of a 2D Khalimsky order\n"         \
"\n"         \
"Usage: 2dlabel in.pgm out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Labels the theta-connected component of a 2D Khalimsky order.\n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: orders\n"         \
"  orders\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__dist__c__ "  dist.c\n"         \
"\n"         \
" distance transform (external)\n"         \
"\n"         \
"Usage: dist in.pgm mode out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Distance to the object X defined by the binary image  in.pgm .\n"         \
"The result function DX(x) is defined by: DX(x) = min {d(x,y), y in X}.\n"         \
"\n"         \
"The distance d used depends on the parameter  mode:\n"         \
" 0: euclidean distance (rounded to the nearest int32)\n"         \
" 1: approximate quadratic euclidean distance (Danielsson)\n"         \
" 2: chamfer distance ([5,7] in 2D; [4,5,6] in 3D)\n"         \
" 3: exact quadratic euclidean distance (int32)\n"         \
" 4: 4-distance in 2d\n"         \
" 5: exact euclidean distance (float)\n"         \
" 8: 8-distance in 2d\n"         \
" 6: 6-distance in 3d\n"         \
" 18: 18-distance in 3d\n"         \
" 26: 26-distance in 3d\n"         \
" 40: 4-distance in 2d (byte coded ouput)\n"         \
" 80: 8-distance in 2d (byte coded ouput)\n"         \
" 60: 6-distance in 3d (byte coded ouput)\n"         \
" 180: 18-distance in 3d (byte coded ouput)\n"         \
" 260: 26-distance in 3d (byte coded ouput)\n"         \
"\n"         \
"The output  out.pgm is of type int32_t for modes \n"         \
"\n"         \
"Types supported: byte 2d,  byte 3d\n"         \
"\n"         \
"Category: morpho\n"         \
"  morpho\n"         \
"\n"         \
" Michel Couprie, Xavier Daragon\n"         \
"\n"         \
"\n"


#define doc__3disthmus__c__ "  3disthmus.c\n"         \
"\n"         \
" destroys 1D isthmuses in a 3D binary image\n"         \
"\n"         \
"Usage: 3disthmus in.pgm out.pgm\n"         \
"\n"         \
"Description: destroys 1D isthmuses in a 3D binary image\n"         \
"\n"         \
"Types supported: byte 3d\n"         \
"\n"         \
"Category: orders\n"         \
"  orders\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__reconsdilat__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__argmin__c__ "  argmin.c\n"         \
"\n"         \
" return the coordinates of a pixel having the minimal value\n"         \
"\n"         \
"Usage: argmin in.pgm [out.list]\n"         \
"\n"         \
"Description:\n"         \
"This function returns (in the list out.list) \n"         \
"coordinates of a pixel having the minimal value in the image  in.pgm .\n"         \
"\n"         \
"If the parameter  out.list is ommitted, the result is printed on the standard output.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n"         \
"\n"         \
"Category: arith\n"         \
"  arith\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__lambdapmedialaxis__c__ "  lambdapmedialaxis.c\n"         \
"\n"         \
" discrete lambda-medial axis transform (cheaper variant)\n"         \
"\n"         \
"Usage: lambdapmedialaxis in.pgm [lambda] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Discrete lambda-medial axis of the binary image  X contained in  in.pgm. \n"         \
"If the parameter  lambda is given, the output is a binary image. Otherwise, \n"         \
"it is a float image representing the function x->lambda(x).\n"         \
"\n"         \
" Experimental.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: morpho\n"         \
"  morpho\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__meshflatten__c__ "  meshflatten.c\n"         \
"\n"         \
"   mesh flattening\n"         \
"\n"         \
"Usage: meshflatten in out\n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"Types supported: mesh 3d\n"         \
"\n"         \
"Category: mesh3d\n"         \
"  mesh3d\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__ppm2pgm__c__ "  ppm2pgm.c\n"         \
"\n"         \
" converts a color ppm image into 3 grayscale pgm images\n"         \
"\n"         \
"Usage: ppm2pgm in.ppm r.pgm g.pgm b.pgm\n"         \
"\n"         \
"Description:\n"         \
"Converts a color ppm image  in.ppm into 3 grayscale pgm images\n"         \
"r.pgm, g.pgm and b.pgm which are the red, \n"         \
"green and blue color planes of the original color image.\n"         \
"\n"         \
"Types supported: byte color 2d\n"         \
"\n"         \
"Category: convert\n"         \
"  convert\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__pgm2ppm__c__ "  pgm2ppm.c\n"         \
"\n"         \
" constructs a ppm color image from 3 pgm grayscale image or from a pgm image and a lookup table\n"         \
"\n"         \
"Usage: pgm2ppm {r.pgm g.pgm b.pgm|in.pgm in.lut} out.ppm\n"         \
"\n"         \
"Description:\n"         \
"1st mode (3 arguments): combines three grayscale images: r.pgm, g.pgm and\n"         \
"b.pgm as the red, green and blue color planes of the color image out.ppm\n"         \
"\n"         \
"2nd mode (2 arguments): generates a color ppm image from the image in.pgm and the \n"         \
"lookup table in.lut\n"         \
"\n"         \
"A lookup table is represented by a special ppm image with 1 row. \n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: convert\n"         \
"  convert\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__squel__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__areaopening__c__ "  areaopening.c\n"         \
"\n"         \
" area opening\n"         \
"\n"         \
"Usage: areaopening in.pgm connex area out.pgm\n"         \
"\n"         \
"Note:  in C main passes an argument 'area+1', whereas python passes the 'area'\n"         \
"as argument (LuM)\n"         \
"\n"         \
"Description:\n"         \
"Area opening with connexity connex and area area.\n"         \
"Deletes the components of the upper cross-sections \n"         \
"which have an area not higher than area.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: connect\n"         \
" connect\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__ellipseparams__c__ "  ellipseparams.c\n"         \
"\n"         \
" ellipse identification and main parameter computation\n"         \
"\n"         \
"Usage: ellipseparams pts.list out.list\n"         \
"\n"         \
"Description:\n"         \
"Identifies the ellipse E which minimizes the sum of the (approximate) distances\n"         \
"between the points in  pts.list and E. \n"         \
"The output is a list containing 6 numbers: a1, a2, r1, r2, r3, r4\n"         \
"where a1 is the half of the big axis length, a2 is the\n"         \
"the half of the little axis length, and each ri is the mean \"distance\",\n"         \
"for quadrant i, between intput points and the computed ellipse \n"         \
"(the expected value for points forming a perfect ellipse is 0)\n"         \
"\n"         \
"Ref: \n"         \
"Andrew W. Fitzgibbon, Maurizio Pilu, and Robert B. Fisher\n"         \
"Direct least-squares fitting of ellipses,\n"         \
"IEEE Transactions on Pattern Analysis and Machine Intelligence, 21(5), 476--480, May 1999\n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: geo\n"         \
"  geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__compiling_windows__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__genkernel__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__3dmakecomplex__c__ "  3dmakecomplex.c\n"         \
"\n"         \
" makes a cellular complex from the given set\n"         \
"\n"         \
"Usage: 3dmakecomplex in.pgm out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Makes a cellular complex from the given set, \n"         \
"by performing the topological closure.\n"         \
"\n"         \
"Types supported: byte 3d\n"         \
"\n"         \
"Category: orders\n"         \
"  orders\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__medialaxis__c__ "  medialaxis.c\n"         \
"\n"         \
" medial axis transform\n"         \
"\n"         \
"Usage: medialaxis in.pgm distance out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Medial axis of the binary image  X. If x is the center of a maximal ball\n"         \
"included in X, then the result R(x) is equal to the smallest distance between x \n"         \
"and a point outside X, otherwise it is equal to 0 .\n"         \
"The distance is indicated by the parameter distance :\n"         \
" 0: approximate euclidean distance (Meyer's algorithm)\n"         \
" 1: exact quadratic euclidean distance (Saito-Toriwaki's skeleton)\n"         \
" 2: exact quadratic euclidean distance (Coeurjolly's reduced axis)\n"         \
" 3: exact quadratic euclidean distance (Rémy-Thiel)\n"         \
" 4: 4-distance in 2d\n"         \
" 6: 6-distance in 3d\n"         \
" 8: 8-distance in 2d\n"         \
" 26: 26-distance in 3d\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: morpho\n"         \
"  morpho\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__skelpar3d_others__cxx__ "  skelpar3d_others.cxx\n"         \
"\n"         \
" parallel 3D binary skeleton\n"         \
"DirectionalSkeletonizer\n"         \
"Usage: skelpar3d in.pgm algorithm nsteps [inhibit] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Parallel 3D binary thinning or skeleton. The parameter  nsteps gives,\n"         \
"if positive, the number of parallel thinning steps to be processed.\n"         \
"If the value given for  nsteps equals -1, the thinning is continued\n"         \
"until stability.\n"         \
"\n"         \
"The parameter  algorithm is a numerical code\n"         \
"indicating which method will be used for the thinning.\n"         \
"The possible choices are:\n"         \
" 0: Palagyi (curvilinear, 6-subiterations directional, 1998)\n"         \
" 1: Palagyi (curvilinear, sequential, 2006)\n"         \
" 2: Palagyi (surface, parallel directional, 2002)\n"         \
" 3: Palagyi (surface, fully parallel, 2008)\n"         \
" 4: Raynal  (curvilinear, directional, 2010)\n"         \
" 5: Raynal  (surface, directional, 2010)\n"         \
" 6: Lohou-Bertrand  (curvilinear, symmetric, 2007)\n"         \
" 7: Ma-Wan-Chang (curvilinear, 2 subfields, 2002)\n"         \
" 8: Tsao-Fu (curvilinear, 6-subiterations directional, 1982)\n"         \
" 9: Ma-Sonka (curvilinear, fully parallel, does not preserve topology 1996)\n"         \
" 10: Ma-Wan (curvilinear (18/6) 6 subiterations, CVIU 2000)\n"         \
" 11: Lohou-Bertrand (curvilinear 6 subiterations, DAM 2005)\n"         \
" 12: Lohou-Bertrand (curvilinear 12 subiterations, DAM 2004)\n"         \
" 13: ACK3a - see lskelpar3d.c\n"         \
" 14: CKSC - see lskeletons.c\n"         \
" 15: Ma-Wan-Lee (curvilinear, 4 subfields, 2002)\n"         \
" 16: Nemeth-Kardos-Palagyi (curvilinear, 2 subfields, 2010, var. 1)\n"         \
" 17: Nemeth-Kardos-Palagyi (curvilinear, 2 subfields, 2010, var. 2)\n"         \
" 18: Nemeth-Kardos-Palagyi (curvilinear, 2 subfields, 2010, var. 3)\n"         \
" 19: Nemeth-Kardos-Palagyi (curvilinear, 4 subfields, 2010, var. 1)\n"         \
" 20: Nemeth-Kardos-Palagyi (curvilinear, 4 subfields, 2010, var. 2)\n"         \
" 21: Nemeth-Kardos-Palagyi (curvilinear, 4 subfields, 2010, var. 3)\n"         \
" 22: Nemeth-Kardos-Palagyi (curvilinear, 8 subfields, 2010, var. 1)\n"         \
" 23: Nemeth-Kardos-Palagyi (curvilinear, 8 subfields, 2010, var. 2)\n"         \
" 24: Nemeth-Kardos-Palagyi (curvilinear, 8 subfields, 2010, var. 3)\n"         \
" 25: She et al. (curvilinear, symmetric, DICTA 2009)\n"         \
" 26: Tsao-Fu (surface, 6-subiterations directional, 1981)\n"         \
" 27: Tsao-Fu (curvilinear, 6-subiterations directional, 1981)\n"         \
" 28: Nemeth-Kardos-Palagyi (curvilinear, 2 subfields, 2010, var. 0)\n"         \
" 29: Nemeth-Kardos-Palagyi (curvilinear, 4 subfields, 2010, var. 0)\n"         \
" 30: Nemeth-Kardos-Palagyi (curvilinear, 8 subfields, 2010, var. 0)\n"         \
" 31: Lohou-Bertrand  (surface, symmetric, 2007)\n"         \
" 32: Manzanera et al. (surface, symmetric, 1999)\n"         \
"\n"         \
"If the parameter  inhibit is given and is a binary image name,\n"         \
"then the points of this image will be left unchanged.\n"         \
"\n"         \
"Types supported: byte 3d\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" Michel Couprie, Benjamin Raynal, John Chaussard\n"         \
"\n"         \
"\n"


#define doc__dup__c__ "  dup.c\n"         \
"\n"         \
" duplicates an image\n"         \
"\n"         \
"Usage: dup in.pgm out.pgm\n"         \
"\n"         \
"Description:\n"         \
"For each pixel x, out[x] = in[x]\n"         \
"\n"         \
"Types supported: all types\n"         \
"\n"         \
"Category: arith\n"         \
"  arith\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__sub__c__ "  sub.c\n"         \
"\n"         \
" substracts an image from another one\n"         \
"\n"         \
"Usage: sub in1.pgm in2.pgm out.pgm\n"         \
"\n"         \
"Description:\n"         \
"For each pixel x, out[x] = in1[x] - in2[x]. \n"         \
"For byte and int32_t image types, if out[x] \n"         \
"Images must be of the same type and same dimensions.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n"         \
"\n"         \
"Category: arith\n"         \
"  arith\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__isnull__c__ "  isnull.c\n"         \
"\n"         \
" nullity test\n"         \
"\n"         \
"Usage: isnull in.pgm\n"         \
"\n"         \
"Description:\n"         \
"This function returns 1 (no error) if for each pixel x, in[x] == 0; \n"         \
"otherwise it returns 0.\n"         \
"Images must be of the same dimensions.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n"         \
"\n"         \
"Category: arith\n"         \
"  arith\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__lpetopo__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__lfclose3d_line__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__entropy__c__ "  entropy.c\n"         \
"\n"         \
" computes the Shannon entropy of an image or a region\n"         \
"\n"         \
"Usage: entropy in.pgm [mask.pgm] out.list\n"         \
"\n"         \
"Description:\n"         \
"Calculates the Shannon entropy of  im.pgm (masked by the binary image\n"         \
" mask.pgm, if given) and saves it in file  out.list .\n"         \
"\n"         \
"Let H(i), i = 0...B-1, denote the histogram of  im.pgm, where B is the number of bins.\n"         \
"Let P(i) be the frequency of the level i in the image, that is, P(i) = H(i)/N, where N is the number of pixels of  im.pgm .\n"         \
"Then, the Shannon entropy of  im.pgm is defined by: E = - SUM P(i) log_2 P(i), for all i in {0...B-1}.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n"         \
"\n"         \
"Category: signal\n"         \
"  signal\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__erosballnum__c__ "  erosballnum.c\n"         \
"\n"         \
" morphological grayscale erosion by an Euclidean ball\n"         \
"\n"         \
"Usage: erosballnum in.pgm r out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Morphological grayscale erosion by an Euclidean ball.\n"         \
"The parameter  r must a positive number (int or float), \n"         \
"the structuring element for this erosion is a ball (or a disc) of radius  r.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: morpho\n"         \
"  morpho\n"         \
"\n"         \
" Michel Couprie 2006\n"         \
"\n"         \
"\n"         \
"\n"


#define doc__labeldil__c__ "  labeldil.c\n"         \
"\n"         \
" labeling of the connected components of a binary image\n"         \
"\n"         \
"Usage: labeldil in.pgm se.pgm out.pgm\n"         \
"\n"         \
"Description:\n"         \
"The argument  se.pgm is a structuring element which must be reflexive and symmetric. \n"         \
"Connected components are defined by this structuring element.\n"         \
"The output image  out.pgm has the type \"int32_t\".\n"         \
"\n"         \
" No test is done to check properties of the structuring element.\n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: connect\n"         \
"  connect\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__ptend__c__ "  ptend.c\n"         \
"\n"         \
" detects end points in a binary image\n"         \
"\n"         \
"Usage: ptend in.pgm connex out.pgm\n"         \
"\n"         \
"Description:\n"         \
"An end point is a white point, which has exactly 1 white n-neighbour (n = 4, 8 (2d) or 6, 18, 26 (3d), as set by the parameter  connex)\n"         \
"\n"         \
"When the type of  in.pgm is 4_BYTE, the image is treated as a label image, where each label is processed as a separate binary image (all other labels are considered as background).\n"         \
"\n"         \
"Types supported: byte 2D, byte 3D, long 3D\n"         \
"\n"         \
"Category: topobin\n"         \
"  topobin\n"         \
"\n"         \
" Michel Couprie 2002\n"         \
"\n"         \
"\n"


#define doc__segmentvol__c__ "  segmentvol.c\n"         \
"\n"         \
" segmentation by filtering components on a volume criterion\n"         \
"\n"         \
"Note: in the python front-end 'm' is mandatory, set 0 without maximization\n"         \
"and 1 with maximization.\n"         \
"\n"         \
"Usage: segmentvol in.pgm connex vol out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Segmentation by selection of cross-section components, based on a \n"         \
"volume criterion. \n"         \
"Connexity = connex ; volume threshold = vol.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: connect\n"         \
"  connect\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__barycentrelab__c__ "  barycentrelab.c\n"         \
"\n"         \
" compute the center of gravity of each labeled region\n"         \
"\n"         \
"Usage: barycentrelab in.pgm out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Compute the center of gravity of each labeled region.\n"         \
"Labels should be consecutive integers. \n"         \
"Only strictly positive labels are considered. \n"         \
"The result is an image that contains labelled points. \n"         \
"More precisely, each barycentre is represented by the nearest grid\n"         \
"point that is given the same label as the region.\n"         \
"\n"         \
"Types supported: long 2d, long 3d\n"         \
"\n"         \
"Category: geo\n"         \
"  geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__enframe3d__c__ "  enframe3d.c\n"         \
"\n"         \
" adds a border with a given gray value to an image\n"         \
"\n"         \
"Usage: enframe3d in.pgm [grayvalue [width [out.pgm]]]\n"         \
"\n"         \
"Description:\n"         \
"Adds a border to the input image, filled with the value  grayvalue (default 0). \n"         \
"The width of the border may be given as parameter  width, \n"         \
"otherwise its value is 1.\n"         \
"\n"         \
"If  out.pgm is not specified, then out.pgm = in.pgm.\n"         \
"\n"         \
"Types supported: byte 2d, int32_t 2d, float 2d, byte 3d, int32_t 3d, float 3d.\n"         \
"\n"         \
"Category: convert, geo\n"         \
"  convert, geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__maxflow__cpp__ "\n"         \
"    maxflow.cpp\n"         \
"\n"         \
"    Calculates the optimal surface segmentation with the maximum\n"         \
"   flow algorithm [1].\n"         \
"   \n"         \
"   Usage: maxflow( (char_image)source and sink, (float_image)constraint image, (int)iterations, (float)tau [, (int)number of threads=0]) -> float_image\n"         \
"   \n"         \
"   Description: In the input, you specify the constraint image\n"         \
"   'g' and two subsets of the image 'S'==1 and 'P'==-1. The operator\n"         \
"   will return a binary flow 'result', which is 0 or 1 almost\n"         \
"   everywhere. The surface around the volume with value 1 will be the\n"         \
"   optimum in the sense, that it will contain 'S', not contain 'P' and\n"         \
"   and it's integral will be the smallest possible.\n"         \
"\n"         \
"   Generally, we choose the inverse of the gradient for 'g'. Sometimes\n"         \
"   the inverse is weighted circularly, so gaps in the border are\n"         \
"   interpolated with circles. \n"         \
"\n"         \
"   Usually the values, that are known to be in the object (which you\n"         \
"   want to segment) are set to sink (value 1). You can set sink (value\n"         \
"   -1) the points, which you know to be outside the object, but\n"         \
"   generally the border is sufficient.\n"         \
"   \n"         \
"   Types supported: float 2d, float 3d, float 4d, ..., float xd\n"         \
"   \n"         \
"   Category: signal, development\n"         \
"     signal, development\n"         \
"   \n"         \
"   References: [1] Appleton, B. and Talbot, H. (2006). Globally minimal\n"         \
"   surfaces by continuous maximal ï¬‚ows. IEEE Transactions on Pattern\n"         \
"   Analysis and Machine Intelligence, 28(1):106â€“118.\n"         \
"\n"         \
"    Laszlo Marak, 2009-2010\n"         \
"\n"         \
"\n"         \
" \n"         \
"\n"


#define doc__surfels_triang___notused_c__ "WRITE ME!!!\n"         \
"\n"


#define doc__dror__cxx__ "  im2line.c\n"         \
"\n"         \
" find line into an image using bresenham definition\n"         \
"\n"         \
"Usage:test4.pgm  testfind.pgm\n"         \
"test4.pgm  testfind.pgm\n"         \
"test4.pgm  testfind.pgm\n"         \
" im2line in.fits {w}  out.pgmtest4.pgm  testfind.pgm\n"         \
"\n"         \
"\n"         \
"Description:\n"         \
"On commence pas netoyer l'image afin de retirer le bruit de fond et d'ameliorer le contraste.\n"         \
"\n"         \
"On determine ensuite les points sur le bord de l'image qui correspondent potentiellement Ã  une droite.\n"         \
"\n"         \
"Parralellement, on crÃ©e un arbre...\n"         \
"\n"         \
"On trace les droites de Bresenham entre les differents points des bords. Pour chacunes des droites on calcule\n"         \
"le nombre de points non nul present dans le plan sous la droite via l'arbre, puis le nombre de points non nuls pour une droite parralelle.\n"         \
"On soustrait ces 2 nombres, on obtient ainsi le nombre de pixel non nul contenu dans l'espace entre les 2 plans.\n"         \
"La droites contenant les plus de points non nul doit correspondre Ã  la trace du satellite.\n"         \
"\n"         \
"L'image finale est une image noire, avec une droite blanche correspondant Ã  la trace du sattelite.\n"         \
"\n"         \
"Parameters:\n"         \
"  in.fits : source file in fits format\n"         \
"\n"         \
"\n"         \
" Juliette Charpentier\n"         \
"\n"         \
"\n"


#define doc__mask__c__ "  mask.c\n"         \
"\n"         \
" applies a binary mask to an image\n"         \
"\n"         \
"Usage: mask in.pgm mask.pgm out.pgm\n"         \
"\n"         \
"Description:\n"         \
"For each pixel x, if mask[x] == 0 then out[x] = 0 else out[x] = in[x]\n"         \
"Images must be of the same dimensions.\n"         \
"\n"         \
"Types supported: byte 2D-3D, long 2D-3D, float 2D-3D, complex 2D-3D\n"         \
"\n"         \
"Category: arith\n"         \
"  arith\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__hthickbin__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__skew__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__meshregul__c__ "  meshregul.c\n"         \
"\n"         \
" mesh smoothing\n"         \
"\n"         \
"Usage: meshregul in mode [param1 [param2]] out\n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"Mesh smoothing. \n"         \
"The possible choices for parameter mode are:\n"         \
" 0: Laplacian smoothing (param1 = number of iterations, default value 5)\n"         \
" 1: Hamam method [HC07] (SOWA, param1 = theta, default value 1.0)\n"         \
" 2: Vollmer, Mencl and Mueller method [VMM] (param1 = alpha, param2 = beta, default values 0.1 and 0.6 resp.)\n"         \
" 3: Hamam method [HC07], variant: matrix A used instead of AA (FOWA, param1 = theta, default value 1.0)\n"         \
" 4: Hamam method [HC07], variant: theta = infty (SOWA, param1 = number of iterations, default value 5)\n"         \
" 5: Hamam method [HC07], conjugate gradient algorithm (SOWA, param1 = theta, default value 1.0)\n"         \
" 6: Taubin method [Tau95] (param1 = lambda, param2 = mu, param3 = N, default values 0.33 and -0.34 and 60)\n"         \
" 7: Laplacian smoothing for a 2D polygon, with border edges preservation (param1 = number of iterations, default value 10)\n"         \
"\n"         \
"[HC07] Y. Hamam and M. Couprie, \"An optimisation-based approach to mesh smoothing: reformulation and extensions\", to appear, 2007.\n"         \
"\n"         \
"[VMM99] J. Vollmer and R. Mencl and H. Muller,\n"         \
"\"Improved Laplacian Smoothing of Noisy Surface Meshes\",\n"         \
"Computer Graphics Forum, Vol. 18, N. 3, pp. 131-138, 1999.\n"         \
"\n"         \
"[Tau95] G. Taubin, \n"         \
"\"Curve and surface smoothing without shrinkage\"\n"         \
"Proceedings of the Fifth International Conference on Computer Vision,\n"         \
"pp. 852-857, 1995.\n"         \
"\n"         \
"Types supported: mesh 3d\n"         \
"\n"         \
"Category: mesh3d\n"         \
"  mesh3d\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__temp3dthin___notused_c__ "WRITE ME!!!\n"         \
"\n"


#define doc__asflin__c__ "  asflin.c\n"         \
"\n"         \
" alternate sequential filter by linear structuring elements\n"         \
"\n"         \
"Usage: asflin in.pgm {x|y|z} [rmin] rmax out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Alternate sequential filter by linear structuring elements for grayscale images.\n"         \
"Linear structuring elements in direction x, y or z of increasing radius,\n"         \
"ranging from 1 (or rmin if this parameter is specified) to rmax,\n"         \
"are generated. Let D1...Dn be these structuring elements, sorted by increasing radius.\n"         \
"Let I0 = in.pgm, the ith intermediate result Ii is obtained by the closing of\n"         \
"the opening of Ii-1 by the structuring element Di. \n"         \
"The result out.pgm contains the final result In.\n"         \
"Giving a value 0 for the optional parameter rmin has the effect of beginning \n"         \
"by a closing instead of an opening.\n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: morpho\n"         \
"  morpho\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"         \
"\n"


#define doc__integermedialaxis__c__ "  integermedialaxis.c\n"         \
"\n"         \
" integer medial axis transform\n"         \
"\n"         \
"Usage: integermedialaxis in.pgm [gamma] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Integer medial axis, as defined in [HR05],\n"         \
"of the binary image  X contained in  in.pgm.\n"         \
"\n"         \
"References:\n"         \
"[HR05] \"Euclidean Skeletons of 3D Data Sets in Linear Time\n"         \
"by the Integer Medial Axis Transform\",\n"         \
"W.H. Hesselink and B.T.M. Roerdink,\n"         \
"Computational Imaging and Vision, Vol. 30,\n"         \
"Mathematical Morphology: 40 Years On, Springer, 2005, pp. 259-268\n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: morpho\n"         \
"  morpho\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__diZenzo__c__ "  diZenzo.c\n"         \
"\n"         \
" diZenzo gradient pour les images couleurs\n"         \
"\n"         \
"Usage: diZenzo imageRVB.ppm alpha [mode] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Le gradient de diZenzo est défini par la donnée de p, q, et t:\n"         \
"\n"         \
"p = Rx*Rx + Vx*Vx + Bx*Bx\n"         \
"\n"         \
"q = Ry*Ry + Vy*Vy + By*By\n"         \
"\n"         \
"t = Rx*Ry + Vx*Vy + Bx*By\n"         \
"\n"         \
"(ou Rx = dérivée en x de la bande rouge, Ry est la dérivée en y de la bande rouge, etc.)\n"         \
"\n"         \
"et le module est donnée par\n"         \
"\n"         \
"G = sqrt(1/2*(p+q+sqrt((p+q)*(p+q) -4(pq-t*t))))\n"         \
"\n"         \
"La direction est donnée par 1/2*atan(2*t/(p-q))\n"         \
"\n"         \
"Si le mode est égale à 0 (valeur défaut) alors l'image de sortie est le gradient, \n"         \
"sinon l'image de sortie est une int32_t entre 0 et 360.\n"         \
"\n"         \
"Les gradients sont calculés par les filtres de Deriche, de paramètre alpha\n"         \
"\n"         \
"Types supported: byte 2D\n"         \
"\n"         \
"Category: signal\n"         \
"  signal\n"         \
"\n"         \
" Laurent Najman\n"         \
"\n"         \
"\n"


#define doc__qdistance__cpp__ "WRITE ME!!!\n"         \
"\n"


#define doc__genlut__c__ "  genlut.c\n"         \
"\n"         \
" generates a lookup table\n"         \
"\n"         \
"Usage: genlut ncol minhue maxhue minlum maxlum minsat maxsat  out.lut\n"         \
"\n"         \
"Description: Generates a lookup table with a continuous color gradient.\n"         \
"\n"         \
"A lookup table is represented by a special ppm image with 1 row. \n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: convert\n"         \
"  convert\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__meshconvert__c__ "  meshconvert.c\n"         \
"\n"         \
" mesh format conversion\n"         \
"\n"         \
"Usage: meshconvert in [resolution] out\n"         \
"\n"         \
"Description:\n"         \
"\n"         \
"Mesh format conversion. \n"         \
"Available input formats: mcm, ifs, vtk.\n"         \
"Available output formats: mcm, vtk, pgm (points only).\n"         \
"If the output format is pgm, then the optional argument resolution \n"         \
"gives the resolution of the grid (homogeneous in x, y and z dimensions). The \n"         \
"default value is 1.0.\n"         \
"\n"         \
"Types supported: mesh 3d\n"         \
"\n"         \
"Category: mesh3d, convert\n"         \
"  mesh3d, convert\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__explodecomp__c__ "  explodecomp.c\n"         \
"\n"         \
" converts single 2D pgm file into a series of 2D pgm files,\n"         \
"  where each file of the series contains one component of the original image\n"         \
"\n"         \
"Usage: explodecomp in.pgm connex {min|max|pla} name_prefix\n"         \
"\n"         \
"Description:\n"         \
"Generated file names are of the form: name_prefixnnnn.pgm, \n"         \
"where nnnn is a four digit decimal integer. \n"         \
"The argument  connex selects the connectivity (4, 8 in 2D; 6, 18, 26 in 3D).\n"         \
"The argument  selects the kind of flat zone\n"         \
"to be labeled:\n"         \
"  min selects regional minima (or background components for a binary image)\n"         \
"  max selects regional maxima (or foreground components for a binary image)\n"         \
"  pla selects all flat zones (plateaux).\n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: convert\n"         \
"  convert\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__ombre__c__ "  ombre.c\n"         \
"\n"         \
" binary shadow of a grayscale function\n"         \
"\n"         \
"Usage: ombre in.pgm out.pgm\n"         \
"\n"         \
"Description:\n"         \
"The shadow of a 1D grayscale image is a 2D binary image.\n"         \
"The shadow of a 2D grayscale image is a 3D binary image.\n"         \
"\n"         \
"Types supported: byte 1d, byte 2d\n"         \
"\n"         \
"Category: geo\n"         \
"  geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__gradientcd__c__ "  gradientcd.c\n"         \
"\n"         \
" Canny-Deriche gradient operator\n"         \
"\n"         \
"Usage: gradientcd in.pgm alpha out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Deriche's recursive implementation of the\n"         \
"Canny's gradient operator. The parameter \n"         \
" alpha (double) controls the spatial extension of the\n"         \
"filter: 0 \n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: signal\n"         \
"  signal\n"         \
"\n"         \
"Reference:\n"         \
"\n"         \
"[Der90] R. Deriche: \"Fast algorithms for low-level vision\",\n"         \
"IEEE Transactions on PAMI, Vol.&nbsp;12, No.&nbsp;1, pp.&nbsp;78-87, 1990.\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__surfacerestoration__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__asfbin__c__ "  asfbin.c\n"         \
"\n"         \
" alternate sequential filter for binary images\n"         \
"\n"         \
"Usage: asfbin in.pgm [rmin] rmax out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Alternate sequential filter for binary images. \n"         \
"\n"         \
"Disc-shaped structuring elements of increasing radius,\n"         \
"ranging from 1 (or rmin if this parameter is specified) to rmax,\n"         \
"are generated. \n"         \
"\n"         \
"Let D1...Dn be these structuring elements, sorted by increasing radius.\n"         \
"Let F0 = in.pgm, the ith intermediate result Fi is obtained by the closing of\n"         \
"the opening of Fi-1 by the structuring element Di. \n"         \
"The result out.pgm contains the final result Fn.\n"         \
"Giving a value 0 for the optional parameter rmin has the effect of beginning \n"         \
"by a closing instead of an opening.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: morpho\n"         \
"  morpho\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"         \
"\n"


#define doc__asft__c__ "  asft.c\n"         \
"\n"         \
" alternate sequential filter controled by topology\n"         \
"\n"         \
"Usage: asft in.pgm {c.pgm cc.pgm|null null} connex rmax out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Alternate sequential filter controled by topology (see [CB04]).\n"         \
"Disc- or Ball-shaped structuring elements of increasing radius,\n"         \
"ranging from 1 to rmax,\n"         \
"are generated. Let D1...Dn be these structuring elements, sorted by increasing radius.\n"         \
"Let I(0) = in.pgm, the ith intermediate result I(i) is obtained by the \n"         \
"homotopic pseudo-closing of the homotopic pseudo-opening of I(i-1) \n"         \
"by the structuring element Di. \n"         \
"Two images  c.pgm and  cc.pgm can be given to serve as constraints for, respectively,\n"         \
"the object and its complementary.\n"         \
"The result out.pgm contains the final result I(n).\n"         \
"\n"         \
"Reference: \n"         \
"[CB04] M. Couprie and G. Bertrand:\n"         \
"Topology preserving alternating sequential filter for smoothing \n"         \
"2D and 3D objects,  Journal of Electronic Imaging, Vol.&nbsp;13, No.&nbsp;4, pp.&nbsp;720-730, 2004.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: morpho, topobin\n"         \
"  morpho, topobin\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__seuilhisto__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__t26pp__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__ultimateerosion__c__ "  ultimateerosion.c\n"         \
"\n"         \
" ultimate erosion\n"         \
"\n"         \
"Usage: ultimateerosion in.pgm [dist] out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Let X be the set in  in.pgm .\n"         \
"The result is union{Ui(X), i in N} where\n"         \
"Ui(X) = erosball(X,i)  reconsgeo(erosball(X,i+1), erosball(X,i)). \n"         \
"Structuring elements are balls defined after a distance.\n"         \
"The distance used depends on the optional parameter  dist (default is 0) :\n"         \
" 0: approximate euclidean distance (truncated)\n"         \
" 1: approximate quadratic euclidean distance\n"         \
" 2: chamfer distance\n"         \
" 3: exact quadratic euclidean distance\n"         \
" 4: 4-distance in 2d\n"         \
" 8: 8-distance in 2d\n"         \
" 6: 6-distance in 3d\n"         \
" 18: 18-distance in 3d\n"         \
" 26: 26-distance in 3d\n"         \
"\n"         \
" The input image  in.pgm must be a binary image. No test is done.\n"         \
"\n"         \
"Types supported: byte 2D, byte 3D\n"         \
"\n"         \
"Category: morpho\n"         \
"  morpho\n"         \
"\n"         \
" Michel Couprie aoÃ»t 2009\n"         \
"\n"         \
"\n"         \
"\n"


#define doc__download__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__watershedMeyer2lab__c__ "  watershedMeyer2lab.c\n"         \
"\n"         \
" watershed transformation (Meyer's algorithm) with labelled markers  \n"         \
"\n"         \
"Usage: watershedMeyer2lab marker image {roi|null} connex [mode] out\n"         \
"\n"         \
"Description:\n"         \
"Performs the watershed transformation on the image image, taking the\n"         \
"set of markers in marker, in the form of a label image where all the pixels\n"         \
"sharing the same label (even if not connected) belong to the same marker.\n"         \
"If this parameter is present, roi\n"         \
"indicates the region of interest on which the operation is performed.\n"         \
"The parameter connex gives the adjacency relation (4,8 in 2D; 6,18,26 in 3D) \n"         \
"for the makers.\n"         \
"\n"         \
"The result is a label image. If the original markers are labelled by\n"         \
"numbers 1,...,n then the regions (catchment basins) of the result will\n"         \
"be labelled with the same numbers.\n"         \
"\n"         \
"If the optional parameter mode is 0 (default value), then the\n"         \
"separation (watershed) will be labelled with n+1. Otherwise, a\n"         \
"separating point that is neighbour of exactly two regions i and j will be\n"         \
"labelled by j*(n+1)+i, with i\n"         \
"of more than two regions be labelled by n+1.\n"         \
"Thus, from any label L > n+1, one can recover the labels of the two\n"         \
"regions i,j in contact by doing: i = L%(n+1); j = L/(n+1).\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: connect\n"         \
" connect\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__despics__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__dilatbin__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__byte2float__c__ "  byte2float.c\n"         \
"\n"         \
" converts a \"byte\" image to a \"float\" image\n"         \
"\n"         \
"Usage: byte2float in [out]\n"         \
"\n"         \
"Description: \n"         \
"\n"         \
"For each pixel x, out[x] = (float)in[x]\n"         \
"\n"         \
"If the last argument  out is omitted, then out = in.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d.\n"         \
"\n"         \
"Category: convert\n"         \
" convert\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__pgm2curve__c__ "  pgm2curve.c\n"         \
"\n"         \
" extracts a curve from a binary image\n"         \
"\n"         \
"Usage: pgm2curve image.pgm connex [x y [z]] out.curve\n"         \
"\n"         \
"Description:\n"         \
"Extracts a curve from a binary image. \n"         \
"The parameter  connex is the connexity of the curve.\n"         \
"It may be equal to 4 or 8 in 2D, and to 6, 18 or 26 in 3D.\n"         \
"If given, the point (x, y, z) (2D) or (x, y, z) (3D) \n"         \
"is the beginning of the curve, and must be an end point. \n"         \
"The output is the text file  out.curve, with the following format:\n"         \
"b nbpoints\n"         \
"x1 y1\n"         \
"x2 y2\n"         \
"...\n"         \
"or (3D): \n"         \
"B nbpoints\n"         \
"x1 y1 z1\n"         \
"x2 y2 z2\n"         \
"...\n"         \
"\n"         \
"The points of the curve may also be valued. This is must be indicated by \n"         \
"a value of 40, 80, 60, 180 or 260 for the parameter  connex, instead\n"         \
"of 4, 8, 6, 18 or 26 respectively. In this case,\n"         \
"the output is the text file  out.curve, with the following format:\n"         \
"n nbpoints\n"         \
"x1 y1 v1\n"         \
"x2 y2 v2\n"         \
"...\n"         \
"or (3D): \n"         \
"N nbpoints\n"         \
"x1 y1 z1 v1\n"         \
"x2 y2 z2 v2\n"         \
"...\n"         \
"\n"         \
"Types supported: byte 2D, byte 3D\n"         \
"\n"         \
"Category: convert geo\n"         \
"  convert geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__addconst__c__ "  addconst.c\n"         \
"\n"         \
" adds a constant value to an image\n"         \
"\n"         \
"Usage: addconst in.pgm const [out.pgm]\n"         \
"\n"         \
"Description:\n"         \
"For each pixel x, out[x] = in[x] + const. If out[x] \n"         \
"exceeds 255, then out[x] is set to 255.\n"         \
"\n"         \
"If  out.pgm is not specified, then out.pgm = in.pgm.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d\n"         \
"\n"         \
"Category: arith\n"         \
"  arith\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__3dcolor__c__ "  3dcolor.c\n"         \
"\n"         \
" Gives a specific color to points in a 3D khalimsky grid,\n"         \
"according to their rank\n"         \
"\n"         \
"Usage: 3dcolor in.pgm out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Gives a specific color to points in a 3D khalimsky grid,\n"         \
"according to their rank\n"         \
"\n"         \
"Types supported: byte 3d\n"         \
"\n"         \
"Category: orders\n"         \
"  orders\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__eros__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__lengthspline__c__ "  lengthspline.c\n"         \
"\n"         \
" computes the length of a spline which is specified by its control points in a text file\n"         \
"\n"         \
"Usage: lengthspline spline.txt [out.list]\n"         \
"\n"         \
"Description:\n"         \
"Computes the length of a spline which is specified by its control points in a text file.\n"         \
"The file format for  spline.txt is the following for 2D:\n"         \
"\n"         \
"c n+1 (where n+1 denotes the number of control points)\n"         \
"x1 y1\n"         \
"...\n"         \
"xn+1 yn+1\n"         \
"C0X1 C0Y1 C1X1 C1Y1 C2X1 C2Y1 C3X1 C3Y1\n"         \
"...\n"         \
"C0Xn C0Yn C1Xn C1Yn C2Xn C2Yn C3Xn C3Yn\n"         \
"\n"         \
"and in the 3D case:\n"         \
"\n"         \
"C n+1 (where n+1 denotes the number of control points)\n"         \
"x1 y1 z1\n"         \
"...\n"         \
"xn+1 yn+1 zn+1\n"         \
"C0X1 C0Y1 C0Z1 C1X1 C1Y1 C1Z1 C2X1 C2Y1 C2Z1 C3X1 C3Y1 C3Z1\n"         \
"...\n"         \
"C0Xn C0Yn C0Zn C1Xn C1Yn C1Zn C2Xn C2Yn C2Zn C3Xn C3Yn C3Zn\n"         \
"\n"         \
"Types supported: spline 2D, spline 3D\n"         \
"\n"         \
"Category: geo\n"         \
" geo\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__complex_modulus__c__ "  complex_modulus.c\n"         \
"\n"         \
" pixelwise modulus of complex\n"         \
"\n"         \
"Usage: complex_modulus in.pgm out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Applies to complex images only.\n"         \
"For each pixel p, out[p] = modulus(in[p]), where \n"         \
"modulus(x+iy) = sqrt(x^2 + y^2).\n"         \
"\n"         \
"Types supported: complex 2d, complex 3d\n"         \
"\n"         \
"Category: arith\n"         \
"  arith\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__volumelignes__c__ "  volumelignes.c\n"         \
"\n"         \
" enhances linear structures in a grayscale image\n"         \
"\n"         \
"Usage: volumelignes skel.pgm orig.pgm connex radius out.pgm\n"         \
"\n"         \
"Description:\n"         \
"Enhances linear structures in a grayscale image.\n"         \
"The algorithm is the following:\n"         \
"\n"         \
"\n"         \
"Let F be the original image (\"orig.pgm\")\n"         \
"Let O be the final result (\"out.pgm\")\n"         \
"Let r be the value of parameter \"radius\"\n"         \
"Compute k iterations of homotopic thinning, result S (\"skel.pgm\")\n"         \
"For each point x of S which is separating\n"         \
"  O(x) = volume((F-S).Br(x)) \n"         \
"  where Br(x) is the disc centered on x with radius r\n"         \
"EndFor\n"         \
"\n"         \
"\n"         \
"References: \n"         \
"\"Topological operators for the detection of\n"         \
"curvilinar structures in grayscale images\", in preparation, 2003.\n"         \
"\n"         \
"Types supported: byte 2d\n"         \
"\n"         \
"Category: topogray\n"         \
"  topogray\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__topotypes__c__ "WRITE ME!!!\n"         \
"\n"


#define doc__shortestpath__c__ "  shortestpath.c\n"         \
"\n"         \
" finds a shortest path from x to y in image \n"         \
"\n"         \
"Usage: shortestpath in.pgm connex mode S1 S2 [S3] D1 D2 [D3] out.lst\n"         \
"\n"         \
"Description:\n"         \
"A weighted graph G = (V,E) is defined with V = set of pixels of  in.pgm \n"         \
"and E = {(P,Q) in VxV ; P and Q are  connex- adjacent}. \n"         \
"Let F(P) be the value of pixel P in the image  in.pgm. \n"         \
"A weight W(P,Q) is assigned to each edge, according to the value of  mode:\n"         \
"\n"         \
" max : W(P,Q) = max{F(P),F(Q)} \n"         \
" min : W(P,Q) = min{F(P),F(Q)} \n"         \
" avg : W(P,Q) = (F(P) + F(Q)) / 2 \n"         \
"\n"         \
"This operator finds a shortest path from ( S1,  S2) to ( D1,  D2)\n"         \
"in this graph. The result is given as a list of vertices (pixels) \n"         \
"in  out.lst.\n"         \
"\n"         \
"Types supported: byte 2D, byte 3D\n"         \
"\n"         \
"Category: connect\n"         \
" connect\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__watershedMeyer_time___notused_c__ "  watershedMeyer.c\n"         \
"\n"         \
" watershed transformation (Meyer's algorithm)\n"         \
"\n"         \
"Usage: watershedMeyer in mark   connex out\n"         \
"\n"         \
"Description:\n"         \
"Performs the watershed transformation on the image in, taking the\n"         \
"set of markers in mark. \n"         \
"If this parameter is present, bgmark\n"         \
"is used as a set of markers for the background.\n"         \
"If this parameter is present, roi\n"         \
"indicates the region of interest on which the operation is performed.\n"         \
"The parameter connex gives the adjacency relation (4,8 in 2D; 6,18,26 in 3D) \n"         \
"for the makers.\n"         \
"\n"         \
"Types supported: byte 2d, byte 3d\n"         \
"\n"         \
"Category: connect\n"         \
" connect\n"         \
"\n"         \
" Michel Couprie\n"         \
"\n"         \
"\n"


#define doc__htkern3d___notused_c__ "WRITE ME!!!\n"         \
"\n"


#endif /* PINK__PYTHON__DOCUMENTER__HPP__ */
// LuM end of file
