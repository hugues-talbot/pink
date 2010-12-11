// This file is generated automatically by patching Doxygen.
// The generator is not yet part of PInK.
#define doc__0000__c__ \
"WRITE ME!!!"



#define doc__2dborder__c__ \
"Usage: 2dborder in.pgm out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Extracts the border of an object in H2. The (internal) border is defined as the set \n"    \
"of the object points which have at least one neighboring background point. \n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: topo \n"    \
" \n"    \
"\n"    \
" Michel Couprie"



#define doc__2dcollapse__c__ \
"Usage: 2dcollapse in.pgm prio [inhibit] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Ultimate constrained collapse guided by a priority image.\n"    \
"The lowest values of the priority image correspond to the highest priority.\n"    \
"\n"    \
"The parameter  prio is either an image (byte or int32_t), or a numerical code\n"    \
"indicating that a distance map will be used as a priority image; \n"    \
"the possible choices are:\n"    \
" 0: approximate euclidean distance\n"    \
" 1: approximate quadratic euclidean distance\n"    \
" 2: chamfer distance\n"    \
" 3: exact quadratic euclidean distance\n"    \
" 4: 4-distance in 2d\n"    \
" 8: 8-distance in 2d\n"    \
"\n"    \
"If the parameter  inhibit is given and is a binary image name, then\n"    \
"the elements of this image will be left unchanged.  If the parameter\n"    \
" inhibit is given and is a number I, then the elements with priority\n"    \
"greater than or equal to I will be left unchanged.  \n"    \
"\n"    \
" The result makes sense only if the input image is a complex.\n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: orders\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__2dcolor__c__ \
"Usage: 2dcolor in.pgm out.pgm\n"    \
"\n"    \
"Description:\n"    \
"gives a specific color to points in a 2D khalimsky grid, \n"    \
"according to their rank\n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: orders\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__2dflowskeleton__c__ \
"WRITE ME!!!"



#define doc__2dinvariants__c__ \
"Usage: 2dinvariants in.pgm\n"    \
"\n"    \
"Description:\n"    \
"Computes the numbers of connected components\n"    \
"and holes of a 2D Khalimsky order\n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: orders\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__2dkhalimskize__c__ \
"Usage: 2dkhalimskize in.pgm  out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Converts a 2D image into a 2D Khalimsky order, or conversely.\n"    \
"\n"    \
"Modes 0, 4, 8 and h are for binary images. \n"    \
"\n"    \
"Modes m, M, g, G and a are for grayscale images. \n"    \
"\n"    \
"Mode = \n"    \
"    0 : only maximal elements,\n"    \
"    4 : emulates 4-connectivity, idem \"miss\", \n"    \
"    8 : emulates 8-connectivity (\"hit\" transform followed by a kernel transform),   \n"    \
"    h : \"hit\",\n"    \
"    m : min, \n"    \
"    M : max,\n"    \
"    g : gradient 2, \n"    \
"    G : gradient 4,\n"    \
"    a : average,\n"    \
"    R : reverse.\n"    \
"\n"    \
"In reverse mode (R), only the beta-terminal elements (squares) are selected.\n"    \
"\n"    \
"Types supported: byte 2d, long 2d, float 2d\n"    \
"\n"    \
"Category: orders\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__2dlabel__c__ \
"Usage: 2dlabel in.pgm out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Labels the theta-connected component of a 2D Khalimsky order.\n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: orders\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__2dmakecomplex__c__ \
"Usage: 2dmakecomplex in.pgm out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Makes a cellular complex from the given set, \n"    \
"by performing the topological closure.\n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: orders\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__2dpardircollapse__c__ \
"Usage: 2dpardircollapse in.pgm nsteps [inhibit] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Parallel directional constrained collapse.\n"    \
"\n"    \
"If the parameter  inhibit is given and is a binary image name,\n"    \
"then the points of this image will be left unchanged. \n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: orders\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__2dseltopo__c__ \
"Usage: 2dseltopo in d- d+ a- a+ b- b+ out\n"    \
"\n"    \
"Description:\n"    \
"  Selects the elements x of the Khalimsky object K\n"    \
"  which satisfy the following inequalities : \n"    \
"\n"    \
"   d1      Types supported: byte 2d\n"    \
"\n"    \
"Category: orders\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__2dthin__c__ \
"Usage: 2dthin in.pgm n out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Homotopic thinning of a 2d Khalimsky image.\n"    \
"The maximum number of steps is given by n.\n"    \
"If the value given for  n equals -1, the thinning is continued\n"    \
"until stability.\n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: orders\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__2dtopoflow__c__ \
"WRITE ME!!!"



#define doc__2elisp__c__ \
"WRITE ME!!!"



#define doc__2latex__c__ \
"WRITE ME!!!"



#define doc__3dalpha__c__ \
"Usage: 3dalpha in.pgm out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Let X be the set of elements of the order  in.pgm, \n"    \
"the result is the union of alpha(x) for all x in X.\n"    \
"\n"    \
"Types supported: byte 3d\n"    \
"\n"    \
"Category: orders\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__3dbeta__c__ \
"Usage: 3dbeta in.pgm out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Let X be the set of elements of the order  in.pgm, \n"    \
"the result is the union of beta(x) for all x in X.\n"    \
"\n"    \
"Types supported: byte 3d\n"    \
"\n"    \
"Category: orders\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__3dborder__c__ \
"Usage: 3dborder in.pgm out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Extracts the border of an object in H3. The (internal) border is defined as the set \n"    \
"of the object points which have at least one neighboring background point. \n"    \
"\n"    \
"Types supported: byte 3d\n"    \
"\n"    \
"Category: topo \n"    \
" \n"    \
"\n"    \
" Michel Couprie"



#define doc__3dcollapse__c__ \
"Usage: 3dcollapse in.pgm prio [inhibit] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Ultimate constrained collapse guided by a priority image.\n"    \
"The lowest values of the priority image correspond to the highest priority.\n"    \
"\n"    \
"The parameter  prio is either an image (byte or int32_t), or a numerical code\n"    \
"indicating that a distance map will be used as a priority image; \n"    \
"the possible choices are:\n"    \
" 0: approximate euclidean distance\n"    \
" 1: approximate quadratic euclidean distance\n"    \
" 2: chamfer distance\n"    \
" 3: exact quadratic euclidean distance\n"    \
" 6: 6-distance in 3d\n"    \
" 18: 18-distance in 3d\n"    \
" 26: 26-distance in 3d\n"    \
"\n"    \
"If the parameter  inhibit is given and is a binary image name,\n"    \
"then the elements of this image will be left unchanged. \n"    \
"If the parameter  inhibit is given and is a number I,\n"    \
"then the elements with priority greater than or equal to I will be left unchanged. \n"    \
"\n"    \
" The result makes sense only if the input image is a complex.\n"    \
"\n"    \
"Types supported: byte 3d\n"    \
"\n"    \
"Category: orders\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__3dcolor__c__ \
"Usage: 3dcolor in.pgm out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Gives a specific color to points in a 3D khalimsky grid,\n"    \
"according to their rank\n"    \
"\n"    \
"Types supported: byte 3d\n"    \
"\n"    \
"Category: orders\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__3dflowskeleton__c__ \
"WRITE ME!!!"



#define doc__3dinvariants__c__ \
"Usage: 3dinvariants in.pgm\n"    \
"\n"    \
"Description:\n"    \
"Computes the numbers of connected components,\n"    \
"cavities and tunnels of a 3D Khalimsky order\n"    \
"\n"    \
"Types supported: byte 3d\n"    \
"\n"    \
"Category: orders\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__3disthmus__c__ \
"Usage: 3disthmus in.pgm out.pgm\n"    \
"\n"    \
"Description: destroys 1D isthmuses in a 3D binary image\n"    \
"\n"    \
"Types supported: byte 3d\n"    \
"\n"    \
"Category: orders\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__3dkhalimskize__c__ \
"Usage: 3dkhalimskize in.pgm  out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Converts a 3D image into a 3D Khalimsky order.\n"    \
"\n"    \
"Modes 6, 26 and h are for binary images. \n"    \
"\n"    \
"Modes m, M and a are for grayscale images. \n"    \
"\n"    \
"Mode = \n"    \
"    6 : emulates 6-connectivity, idem \"miss\", \n"    \
"    26 : emulates 26-connectivity (\"hit\" transform followed by a kernel transform),   \n"    \
"    h : \"hit\",\n"    \
"    m : min, \n"    \
"    M : max,\n"    \
"    a : average.\n"    \
"    R : reverse.\n"    \
"\n"    \
"Types supported: byte 3d, long 3d, float 3d\n"    \
"\n"    \
"Category: orders\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__3dlabel__c__ \
"Usage: 3dlabel in.pgm out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Labels the theta-connected component of a 3D Khalimsky order.\n"    \
"\n"    \
"Types supported: byte 3d\n"    \
"\n"    \
"Category: orders\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__3dmakecomplex__c__ \
"Usage: 3dmakecomplex in.pgm out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Makes a cellular complex from the given set, \n"    \
"by performing the topological closure.\n"    \
"\n"    \
"Types supported: byte 3d\n"    \
"\n"    \
"Category: orders\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__3dmakepolygons__c__ \
"Usage: 3dmakepolygons lab.pgm border.pgm psubdiv pmerge out.vtk\n"    \
"\n"    \
"Description:\n"    \
"\n"    \
"The input  lab.pgm contains a labelled pure 2D complex, where each label \n"    \
"marks a set of 2-facets which is a manifold with boundary\n"    \
"\n"    \
"psubdiv: this parameter governs the subdivision of polygon edges \n"    \
"pmerge: TODO\n"    \
"\n"    \
"Types supported: byte 3d\n"    \
"\n"    \
"Category: orders\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__3dpardircollapse__c__ \
"Usage: 3dpardircollapse in.pgm nsteps [inhibit] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Parallel directional constrained collapse.\n"    \
"\n"    \
"If the parameter  inhibit is given and is a binary image name,\n"    \
"then the points of this image will be left unchanged. \n"    \
"\n"    \
"Types supported: byte 3d\n"    \
"\n"    \
"Category: orders\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__3dplane__c__ \
"Usage: 3dplane in.pgm a b c d out.pgm\n"    \
"\n"    \
"Description:\n"    \
"  Draws into the Khalimsky volume  k, the discretization of the plane\n"    \
"  defined by the equation:  a x +  b y +  c z +  d = 0\n"    \
"\n"    \
"Types supported: byte 3d\n"    \
"\n"    \
"Category: orders\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__3drecons__c__ \
"Usage: 3drecons g.list f.pgm out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Geodesic reconstruction (in the sense of the neighborhood relation theta) \n"    \
"of the set of points represented by the list  g.list in the set  f.pgm .\n"    \
"\n"    \
"Types supported: byte 3d\n"    \
"\n"    \
"Category: orders\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__3dseltopo__c__ \
"Usage: 3dseltopo in d- d+ a- a+ b- b+ out\n"    \
"\n"    \
"Description:\n"    \
"  Selects the elements x of the Khalimsky volume K\n"    \
"  which satisfy the following inequalities : \n"    \
"\n"    \
"   d1      Types supported: byte 3d\n"    \
"\n"    \
"Category: orders\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__3dskelsurf__c__ \
"Usage: in.pgm n out.pgm\n"    \
"\n"    \
"Description: Surfacic skeleton in the sense defined in ref. [LB99]\n"    \
"The parameter  n gives the number of parallel steps of simple,\n"    \
"non surface end point deletions.\n"    \
"\n"    \
"Reference: \n"    \
"[LB99] C. Lohou, Gilles Bertrand: \"Poset approach to 3D parallel thinning\", SPIE Vision Geometry VIII, Vol.&nbsp;3811, pp.&nbsp;45-56, 1999.\n"    \
"\n"    \
"Types supported: byte 3d\n"    \
"\n"    \
"Category: orders\n"    \
"\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__3dsphere__c__ \
"WRITE ME!!!"



#define doc__3dsurfacecollapse__c__ \
"Usage: 3dsurfacecollapse in.pgm mode nsteps [inhibit] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Parallel directional constrained collapse with surface detection and preservation.\n"    \
"\n"    \
"The parameter  mode selects the strategy used to detect surface elements.\n"    \
"The possible choices are:\n"    \
" 1: any 2-dimensional facet\n"    \
"\n"    \
"The parameter  nsteps gives the number of \"layers\" to be removes, if\n"    \
"the value is -1 then the interations continue until stability.\n"    \
"\n"    \
"If the parameter  inhibit is given and is a binary image name,\n"    \
"then the points of this image will be left unchanged. \n"    \
"\n"    \
"Types supported: byte 3d\n"    \
"\n"    \
"Category: orders\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__3dthin__c__ \
"Usage: 3dthin in.pgm n out.pgm\n"    \
"\n"    \
"Description: Homotopic thinning of a 3d Khalimsky image.\n"    \
"The maximum number of steps is given by n.\n"    \
"If the value given for  n equals -1, the thinning is continued\n"    \
"until stability.\n"    \
"\n"    \
"Types supported: byte 3d\n"    \
"\n"    \
"Category: orders\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__ac2pink__c__ \
"Usage: ac2pink in.ac out.3sc\n"    \
"\n"    \
"Description:\n"    \
"\n"    \
"Reads the file in.ac. This file must be in the AC3D format.\n"    \
"Generates the output file out.3sc in the 3Dscene format : \n"    \
"\n"    \
"FILE ::= '3Dscene' &lt;N&gt; &lt;OBJ&gt;*\n"    \
"OBJ ::= &lt;LINE&gt; | &lt;CLOSEDLINE&gt; | &lt;SPLINE&gt; | &lt;CLOSEDSPLINE&gt; | &lt;POLYGON&gt;\n"    \
"LINE ::= 'line' &lt;N&gt; &lt;POINT&gt;*\n"    \
"CLOSEDLINE ::= 'closedline' &lt;N&gt; &lt;POINT&gt;*\n"    \
"SPLINE ::= 'spline' &lt;N&gt; &lt;POINT&gt;*\n"    \
"CLOSEDSPLINE ::= 'closedspline' &lt;N&gt; &lt;POINT&gt;*\n"    \
"POLYGON ::= 'polygon' &lt;N&gt; &lt;POINT&gt;*\n"    \
"N ::= ascii_coded_integer\n"    \
"POINT ::= ascii_coded_float ascii_coded_float ascii_coded_float\n"    \
"\n"    \
"Types supported: AC3D\n"    \
"\n"    \
"Category: convert\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__add__c__ \
"Usage: add in1.pgm in2.pgm out.pgm\n"    \
"\n"    \
"Description:\n"    \
"For each pixel x, out[x] = in1[x] + in2[x]. If both images are byte images,\n"    \
"and if out[x] exceeds 255, then out[x] is set to 255.\n"    \
"Images must be of the same dimensions.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n"    \
"\n"    \
"Category: arith\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__addconst__c__ \
"Usage: addconst in.pgm const out.pgm\n"    \
"\n"    \
"Description:\n"    \
"For each pixel x, out[x] = in[x] + const. If out[x] \n"    \
"exceeds 255, then out[x] is set to 255.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d\n"    \
"\n"    \
"Category: arith\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__affine__c__ \
"Usage: affine in.pgm  out.pgm\n"    \
"\n"    \
"Description:\n"    \
"\n"    \
"Applies to in.pgm an affine tranformation composed of a zoom \n"    \
"(factors  hx,  hy), a rotation (angle  theta in degrees, \n"    \
"center 0,0), and a translation (vector  tx,  ty). \n"    \
"Alternatively, the transformation parameters may be given \n"    \
"in a type \"e\" list file (see doc/formats.txt).\n"    \
"\n"    \
"Method: interpolation.\n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: geo\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__alpha__c__ \
"Usage: alpha in out\n"    \
"\n"    \
"Description: \n"    \
"Computes the connecivity number alpha (as defined in ref. BEC97) for each image voxel.\n"    \
"\n"    \
"Types supported: byte 3D\n"    \
"\n"    \
"Category: topogray\n"    \
"\n"    \
"\n"    \
"References:\n"    \
"\n"    \
"[BEC97] G. Bertrand, J. C. Everat and M. Couprie: \"Image segmentation through operators based upon topology\",  Journal of Electronic Imaging, Vol.&nbsp;6, No.&nbsp;4, pp.&nbsp;395-405, 1997.\n"    \
"\n"    \
"[CBB01] M. Couprie, F.N. Bezerra, Gilles Bertrand: \"Topological operators for\n"    \
"grayscale image processing\",  Journal of Electronic Imaging, Vol.&nbsp;10, No.&nbsp;4, pp.&nbsp;1003-1015, 2001.\n"    \
"\n"    \
" Michel Couprie"



#define doc__amont__c__ \
"Usage: amont in.pgm mark.pgm connex [s] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Let F be the function defined by  in.pgm, \n"    \
"let X be the set defined by  mark.pgm.\n"    \
"The amont of X in F is defined as the set of points x such that there exists\n"    \
"an ascending path from X to x for F with connectivity connex.\n"    \
"If the parameter s is given, then only strictly ascending paths are \n"    \
"considered.\n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: connect\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__animscenes__c__ \
"Usage: animscenes prefix_in begin end prefix_out nbsamples\n"    \
"\n"    \
"Description:\n"    \
"The input scenes must be stored in files with names of the form  \n"    \
" prefix_inXXXX.3sc, where XXXX is an integer between  begin\n"    \
"and  end. They must all contain the same number of objects, \n"    \
"and object types must match from one scene to each other (order matters).\n"    \
"\n"    \
"Between the scenes of the series, a total number of  nbsamples scenes \n"    \
"are generated by spline interpolation.\n"    \
"\n"    \
"Types supported: scene 3D\n"    \
"\n"    \
"Category: draw geo\n"    \
" \n"    \
"\n"    \
" Michel Couprie"



#define doc__area__c__ \
"Usage: area in.pgm [out.list]\n"    \
"\n"    \
"Description:\n"    \
"This function returns (in the list out.list) \n"    \
"the number of non-null pixels of the image  in.pgm .\n"    \
"\n"    \
"If the parameter  out.list is ommitted, the result is printed on the standard output.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n"    \
"\n"    \
"Category: arith\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__areaclosing__c__ \
"Usage: areaclosing in.pgm connex area out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Area closing with connexity connex and area area.\n"    \
"Deletes the components of the lower cross-sections \n"    \
"which have an area not higher than area.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: connect\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__areaopening__c__ \
"Usage: areaopening in.pgm connex area out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Area opening with connexity connex and area area.\n"    \
"Deletes the components of the upper cross-sections \n"    \
"which have an area not higher than area.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: connect\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__areaselnb__c__ \
"Usage: areaselnb in.pgm connex nb out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Segmentation by selection of a given number or components, based on an area criterion. \n"    \
"Connexity = connex ; desired number of components = nb.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: connect\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__asf__c__ \
"Usage: asf in.pgm [rmin] rmax out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Alternate sequential filter for grayscale images (for binary images use preferably asfbin). \n"    \
"Disc-shaped structuring elements of increasing radius,\n"    \
"ranging from 1 (or rmin if this parameter is specified) to rmax,\n"    \
"are generated. Let D1...Dn be these structuring elements, sorted by increasing radius.\n"    \
"Let I0 = in.pgm, the ith intermediate result Ii is obtained by the closing of\n"    \
"the opening of Ii-1 by the structuring element Di. \n"    \
"The result out.pgm contains the final result In.\n"    \
"Giving a value 0 for the optional parameter rmin has the effect of beginning \n"    \
"by a closing instead of an opening.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: morpho\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__asfbin__c__ \
"Usage: asfbin in.pgm [rmin] rmax out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Alternate sequential filter for binary images. \n"    \
"\n"    \
"Disc-shaped structuring elements of increasing radius,\n"    \
"ranging from 1 (or rmin if this parameter is specified) to rmax,\n"    \
"are generated. \n"    \
"\n"    \
"Let D1...Dn be these structuring elements, sorted by increasing radius.\n"    \
"Let F0 = in.pgm, the ith intermediate result Fi is obtained by the closing of\n"    \
"the opening of Fi-1 by the structuring element Di. \n"    \
"The result out.pgm contains the final result Fn.\n"    \
"Giving a value 0 for the optional parameter rmin has the effect of beginning \n"    \
"by a closing instead of an opening.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: morpho\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__asflin__c__ \
"Usage: asflin in.pgm  [rmin] rmax out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Alternate sequential filter by linear structuring elements for grayscale images.\n"    \
"Linear structuring elements in direction x, y or z of increasing radius,\n"    \
"ranging from 1 (or rmin if this parameter is specified) to rmax,\n"    \
"are generated. Let D1...Dn be these structuring elements, sorted by increasing radius.\n"    \
"Let I0 = in.pgm, the ith intermediate result Ii is obtained by the closing of\n"    \
"the opening of Ii-1 by the structuring element Di. \n"    \
"The result out.pgm contains the final result In.\n"    \
"Giving a value 0 for the optional parameter rmin has the effect of beginning \n"    \
"by a closing instead of an opening.\n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: morpho\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__asfr__c__ \
"Usage: asfr in.pgm [rmin] rmax out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Alternate sequential filter by reconstruction. \n"    \
"Disc-shaped structuring elements of increasing radius,\n"    \
"ranging from 1 (or rmin if this parameter is specified) to rmax,\n"    \
"are generated. Let D1...Dn be these structuring elements, sorted by increasing radius.\n"    \
"Let I0 = in.pgm, the ith intermediate result Ii is obtained by the closure \n"    \
"by reconstruction of Ii-1 by the structuring element Di. \n"    \
"The result out.pgm contains the final result In.\n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: morpho\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__asft__c__ \
"Usage: asft in.pgm  connex rmax out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Alternate sequential filter controled by topology (see [CB04]).\n"    \
"Disc- or Ball-shaped structuring elements of increasing radius,\n"    \
"ranging from 1 to rmax,\n"    \
"are generated. Let D1...Dn be these structuring elements, sorted by increasing radius.\n"    \
"Let I(0) = in.pgm, the ith intermediate result I(i) is obtained by the \n"    \
"homotopic pseudo-closing of the homotopic pseudo-opening of I(i-1) \n"    \
"by the structuring element Di. \n"    \
"Two images  c.pgm and  cc.pgm can be given to serve as constraints for, respectively,\n"    \
"the object and its complementary.\n"    \
"The result out.pgm contains the final result I(n).\n"    \
"\n"    \
"Reference: \n"    \
"[CB04] M. Couprie and G. Bertrand:\n"    \
"Topology preserving alternating sequential filter for smoothing \n"    \
"2D and 3D objects,  Journal of Electronic Imaging, Vol.&nbsp;13, No.&nbsp;4, pp.&nbsp;720-730, 2004.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: morpho, topobin\n"    \
", \n"    \
"\n"    \
" Michel Couprie"



#define doc__asftmed__c__ \
"Usage: asftmed in.pgm connex rmax out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Alternate sequential filter controled by topology (see [CB04]).\n"    \
"Disc- or Ball-shaped structuring elements of increasing radius,\n"    \
"ranging from 1 to rmax,\n"    \
"are generated. Let D1...Dn be these structuring elements, sorted by increasing radius.\n"    \
"Let I(0) = in.pgm, the ith intermediate result I(i) is obtained by the \n"    \
"homotopic pseudo-closing of the homotopic pseudo-opening of I(i-1) by the structuring element Di. \n"    \
"Furthermore, the medial axes (distance 8 in 2D, 26 in 3D) of both the input image and its inverse \n"    \
"are used as constraint sets.\n"    \
"The result out.pgm contains the final result I(n).\n"    \
"\n"    \
"Reference: \n"    \
"[CB04] M. Couprie and G. Bertrand:\n"    \
"Topology preserving alternating sequential filter for smoothing \n"    \
"2D and 3D objects,  Journal of Electronic Imaging, Vol.&nbsp;13, No.&nbsp;4, pp.&nbsp;720-730, 2004.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: morpho, topobin\n"    \
", \n"    \
"\n"    \
" Michel Couprie"



#define doc__asftndg__c__ \
"Usage: asftndg in.pgm  connex rmax out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Alternate sequential filter controled by topology for multilevel images.\n"    \
"Disc- or Ball-shaped structuring elements of increasing radius,\n"    \
"ranging from 1 to rmax,\n"    \
"are generated. Let D1...Dn be these structuring elements, sorted by increasing radius.\n"    \
"Let I(0) = in.pgm, the ith intermediate result I(i) is obtained by the \n"    \
"homotopic pseudo-closing of the homotopic pseudo-opening of I(i-1) by the structuring element Di. \n"    \
"Two images  c.pgm and  cc.pgm can be given to serve as constraints. \n"    \
"The image  c.pgm must be 0 except for the points x that are to be preserved,\n"    \
"which can be set to 255 or to I[x].\n"    \
"The image  cc.pgm must be 255 except for the points x that are to be preserved,\n"    \
"which can be set to 0 or to I[x].\n"    \
"The result out.pgm contains the final result I(n).\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: morpho, topogray\n"    \
", \n"    \
"\n"    \
" Michel Couprie"



#define doc__attribute__c__ \
"Usage: attribute in.pgm connex seuil   out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Labels each connected component with its attribute value.\n"    \
"Argument  connex defines the connectivity.\n"    \
"Argument  seuil defines a threshold: components with attribute value not higher than  seuil are filtered out.\n"    \
"\n"    \
"Warning: in 3D only the attribute \"area\" is implemented.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: geo\n"    \
"\n"    \
"\n"    \
" This operator is in experimental state. Do not use in applications. \n"    \
"\n"    \
" Michel Couprie"



#define doc__autocrop__c__ \
"Usage: autocrop in.pgm [threshold] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Selects the minimum rectangle (cuboid in 3d) that contain all non-null information \n"    \
"of the image  in.pgm .\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: geo\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__average__c__ \
"Usage: average in1.pgm in2.pgm [alpha] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"The default value for the optional parameter  alpha is 0.5 . \n"    \
"For each pixel x, out[x] = alpha * in1[x] + (1 - alpha) * in2[x]. \n"    \
"If both images are byte images,\n"    \
"and if out[x] exceeds 255, then out[x] is set to 255.\n"    \
"Images must be of the same dimensions.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n"    \
"\n"    \
"Category: arith\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__average1__c__ \
"Usage: average1 in.pgm [mask.pgm] out.list\n"    \
"\n"    \
"Description:\n"    \
"This function returns (in the list out.list) \n"    \
"the average of the pixel values of the image  in.pgm .\n"    \
"If the optional parameter  mask.pgm is given, then only the \n"    \
"values which correspond to non-null points of mask are averaged.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n"    \
"\n"    \
"Category: signal stats\n"    \
" \n"    \
"\n"    \
" Michel Couprie"



#define doc__barycentre__c__ \
"Usage: barycentre in.pgm connex out.pgm\n"    \
"\n"    \
"Description: \n"    \
"Compute the center of gravity of each connected component.\n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: geo\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__barycentrelab__c__ \
"Usage: barycentrelab in.pgm out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Compute the center of gravity of each labeled region.\n"    \
"Labels should be consecutive integers. \n"    \
"Only strictly positive labels are considered. \n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: geo\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__bisector__c__ \
"Usage: bisector dist.pgm mask.pgm out.pgm\n"    \
"\n"    \
"Description:\n"    \
"\n"    \
"The bisector function is computed for the points which are non-null in the binary image\n"    \
" mask.pgm , from the exact quadratic euclidean distance map found in  dist.pgm .\n"    \
"\n"    \
" If the image  dist.pgm is not an exact quadratic euclidean distance map, \n"    \
"then the result of this operator will make no sense. \n"    \
"\n"    \
"References: \n"    \
"[CCZ07] M. Couprie, D. Coeurjolly and R. Zrour: \"Discrete bisector function and Euclidean skeleton in 2D and 3D\", Image and Vision Computing, Vol.&nbsp;25, No.&nbsp;10, pp.&nbsp;1543-1556, 2007.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d \n"    \
"\n"    \
"Category: morpho\n"    \
"\n"    \
"\n"    \
"\n"    \
" Rita Zrour, Michel Couprie"



#define doc__bisector_talbot__c__ \
"WRITE ME!!!"



#define doc__blob__c__ \
"WRITE ME!!!"



#define doc__bmp2pgm__c__ \
"Description:\n"    \
"\n"    \
"Types supported: byte 2D\n"    \
"\n"    \
"Category: convert\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__bmp2ppm__c__ \
"Description:\n"    \
"\n"    \
"Types supported: byte 2D\n"    \
"\n"    \
"Category: \n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__border__c__ \
"Usage: border in connex out\n"    \
"\n"    \
"Description:\n"    \
"Let X be the subset of Zn that corresponds to the input image  in, and \n"    \
"let k be the value of the parameter  connex (that is, 4, 8 (2d) or 6, 26 (3d)), which is the connectivity chosen for X.\n"    \
"\n"    \
"The border B(X) of X is defined by:\n"    \
"B(X) = {x in X, (Gamma_nk(x) inter Xbar) not empty}\n"    \
"where nk is the complementary connectivity for k.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: topobin\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__boxmin__c__ \
"Usage: boxmin in.pgm out.pgm\n"    \
"\n"    \
"Description:\n"    \
"The result out.pgm has the same size as in.pgm. \n"    \
"It contains the minimal box including the set of points in  in.pgm \n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: geo\n"    \
"\n"    \
"\n"    \
" Michel Couprie 2002"



#define doc__bruite__c__ \
"Usage: bruite in.pgm n alpha p out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Adds a uniform or gaussian noise, of mean 0 and standard deviation alpha, \n"    \
"to a proportion p of the pixels of the image in.pgm .\n"    \
"\n"    \
"  OPTIONS\n"    \
"    n = 0  uniform noise\n"    \
"    n = 1  gaussian noise\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: signal\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__byte2float__c__ \
"Usage: byte2float in out\n"    \
"\n"    \
"Description: For each pixel x, out[x] = (float)in[x]\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d.\n"    \
"\n"    \
"Category: convert\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__byte2long__c__ \
"Usage: byte2long in out\n"    \
"\n"    \
"Description: For each pixel x, out[x] = (int32_t)in[x]\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d.\n"    \
"\n"    \
"Category: convert\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__cadre__c__ \
"WRITE ME!!!"



#define doc__car2pol__c__ \
"Usage: in x_center y_center out \n"    \
"\n"    \
"Description:\n"    \
"Converts a 2d image from cartesian to polar coordinates.\n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: convert\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__catgif__c__ \
"WRITE ME!!!"



#define doc__catpgm__c__ \
"Usage: catpgm name_prefix index_begin index_end [xdim ydim zdim] name_out\n"    \
"\n"    \
"Description:\n"    \
"Original file names must be of the form: name_prefixnnnn.pgm, \n"    \
"where nnnn is a four digit decimal integer. The integers index_begin\n"    \
"and index_end specify respectively the first and the last index of the series.\n"    \
"\n"    \
"Types supported: sequence of byte 1d or 2d images\n"    \
"\n"    \
"Category:  convert\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__ccv__c__ \
"WRITE ME!!!"



#define doc__changename__c__ \
"WRITE ME!!!"



#define doc__circleincl__c__ \
"Usage: circleincl in.pgm pts.list filled out.pgm\n"    \
"\n"    \
"Description:\n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: geo\n"    \
"\n"    \
"\n"    \
" John Chaussard"



#define doc__circular_measure__cpp__ \
"WRITE ME!!!"



#define doc__click2d__c__ \
"WRITE ME!!!"



#define doc__click3d__c__ \
"WRITE ME!!!"



#define doc__closeball__c__ \
"Usage: closeball in.pgm r [dist] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"The closing consists in a dilation (dilatball) followed by an erosion (erosball).\n"    \
"The structuring element for this closing is a ball (or a disc) of radius  r.\n"    \
"The erosion and dilation are computed by thresholding a distance map.\n"    \
"The distance used depends on the optional parameter  dist (default is 0) :\n"    \
" 0: approximate euclidean distance\n"    \
" 2: chamfer distance\n"    \
" 4: 4-distance in 2d\n"    \
" 8: 8-distance in 2d\n"    \
" 6: 6-distance in 3d\n"    \
" 18: 18-distance in 3d\n"    \
" 26: 26-distance in 3d\n"    \
"\n"    \
" The input image  in.pgm must be a binary image. No test is done.\n"    \
"\n"    \
"Types supported: byte 2D, byte 3D\n"    \
"\n"    \
"Category: morpho\n"    \
"\n"    \
"\n"    \
" Michel Couprie 2002\n"    \
"\n"    \
"Example:\n"    \
"\n"    \
"closeball cells 3 cells_closeball\n"    \
"\n"    \
"\n"    \
"  \n"    \
"      \n"    \
"  \n"    \
"  cells\n"    \
"      cells_closeball\n"    \
"  \n"    \
""



#define doc__closing__c__ \
"Usage: closing in.pgm se.pgm out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Morphological closing by a plane structuring element.\n"    \
"The (plane) structuring element is given by the non-null values in  se.pgm, \n"    \
"its origin (wrt the point (0,0) of  se.pgm ) \n"    \
"is given by a comment line in the file se.pgm. \n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: morpho\n"    \
"\n"    \
"\n"    \
" Michel Couprie 2002"



#define doc__colorize__c__ \
"Usage: colorize in.pgm lut.ppm out.ppm\n"    \
"\n"    \
"Description:\n"    \
"Generates a color image from a grayscale image and a lookup table (see genlut.c).\n"    \
"\n"    \
"Types supported: byte 2D\n"    \
"\n"    \
"Category: convert\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__comphisto__c__ \
"WRITE ME!!!"



#define doc__comptreetime__c__ \
"WRITE ME!!!"



#define doc__concat3d__c__ \
"WRITE ME!!!"



#define doc__contours__c__ \
"WRITE ME!!!"



#define doc__convexhull__c__ \
"Usage: convewhull in.pgm mode out\n"    \
"\n"    \
"Description: Computes the convex hull of the set of non-null points\n"    \
"in the image in.pgm, by the Jarvis's algorithm as described in \n"    \
"\"Introduction to algorithms\", T. Cormen, C. Leiserson, R. Rivest, MIT Press.\n"    \
"\n"    \
"The parameter  mode selects the format of the result: \n"    \
" 0: list of the support points of the convex hull\n"    \
" 1: support points of the convex hull, embedded in the image frame\n"    \
" 2: contour of the convex hull, embedded in the image frame\n"    \
" 3: full convex hull, embedded in the image frame\n"    \
"\n"    \
"Types supported: byte 2D\n"    \
"\n"    \
"Category: geo\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__convol__c__ \
"Usage: convol in.pgm kernel.pgm [mode] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Convolution of  in.pgm by kernel.pgm.\n"    \
"The result is a float image.\n"    \
"Depending on the value given for the (optional) parameter mode:\n"    \
"   mode = 0 (default) : naive algorithm. \n"    \
"      The image  in.pgm is considered as null out of its support. \n"    \
"   mode = 1 : naive algorithm. \n"    \
"      The boundary of image  in.pgm is extended outside its support.\n"    \
"   mode = 2 : convolution using the FFT.\n"    \
"      The image  in.pgm is considered as null out of its support. \n"    \
"   mode = 3 : convolution using the FFT.\n"    \
"      The boundary of image  in.pgm is extended outside its support.\n"    \
"\n"    \
"Types supported: byte 2d, int32_t 2d, float 2d\n"    \
"\n"    \
"Category: signal\n"    \
"\n"    \
"\n"    \
" convolution algorithm is in O(|in|*|kernel|). \n"    \
"For large kernels, use FFT version which is in \n"    \
"O(n log n) where n = max(|in|,|kernel|)\n"    \
"\n"    \
" Michel Couprie"



#define doc__convol3__c__ \
"WRITE ME!!!"



#define doc__correctbias__c__ \
"Usage: correctbias in.pgm factor type [xc yc] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Let  a be the value of parameter  factor.\n"    \
"The bias  type can be:\n"    \
" 0 radial (center  xc,  yc), linear (add -ax to the value of the element of abcissa x).\n"    \
" 1 radial (center  xc,  yc), parabolic (add -a^2 x to the value of the element of abcissa x).\n"    \
"\n"    \
"Types supported: byte 2D\n"    \
"\n"    \
"Category: arith\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__crestheight__c__ \
"Usage: crestheight skel.pgm connex out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Let F be the input image (parameter skel.pgm), and\n"    \
"let R be the output image (parameter out.pgm).\n"    \
"For each point p which is separating for F (connexity\n"    \
"given by the parameter connex), the output value R(p) is\n"    \
"equal to calpha8m(F,S,p).\n"    \
"\n"    \
"References: \n"    \
"\"Topological operators for the detection of\n"    \
"curvilinar structures in grayscale images\", in preparation, 2003.\n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: topogray\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__crestrestoration__c__ \
"Usage: crestrestoration im.pgm  connex niter fileout.pgm [condout.pgm]\n"    \
"\n"    \
"Description:\n"    \
"Crest restoration algorithm, as described in ref. CBB01.\n"    \
"The input image  im.pgm must be a \"thin\" grayscale image, as\n"    \
"the output of the operator  hthin.\n"    \
"The parameter  connex indicates the connectivity used for the minimal \n"    \
"regions.\n"    \
"The parameter  niter gives the number of iterations.\n"    \
"The optional parameter  imcond.pgm is a binary image (a set C) which indicates the points\n"    \
"in the neighborhood of which the extensible points will be searched.\n"    \
"The points which are modified by the algorithm will be dynamically added to C.\n"    \
"The optional parameter  condout.pgm is an output file containing the final state of the \n"    \
"set C.\n"    \
"\n"    \
"Reference: \n"    \
"[CBB01] M. Couprie, F.N. Bezerra, Gilles Bertrand: \"Topological operators for\n"    \
"grayscale image processing\",  Journal of Electronic Imaging, Vol.&nbsp;10, No.&nbsp;4, pp.&nbsp;1003-1015, 2001.\n"    \
"\n"    \
"Types supported: byte 2D\n"    \
"\n"    \
"Category: topogray\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__crestsegment__c__ \
"Usage: crestsegment skel.pgm connex niter base incr out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Let F be the input image (parameter skel.pgm), and\n"    \
"let R be the output image (parameter out.pgm).\n"    \
"\n"    \
"TO BE WRITTEN\n"    \
"\n"    \
"Reference: \n"    \
"\"Topological operators for the detection of\n"    \
"curvilinar structures in grayscale images\", in preparation, 2003.\n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: topogray\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__crestvol__c__ \
"Usage: crestvol in.pgm connex radius out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Enhances linear structures in a grayscale image.\n"    \
"The algorithm is the following:\n"    \
"\n"    \
" @verbatim \n"    \
"Let F be the original image (\"in.pgm\")\n"    \
"Let O be the final result (\"out.pgm\")\n"    \
"Let r be the value of parameter \"radius\"\n"    \
"Compute k iterations of homotopic thinning, result S\n"    \
"Lower all separating points p in S down to alpha8m(S, p), result Sp \n"    \
"For each point x of S which is separating\n"    \
"  Let Dx be the image with D(x) = F(x), and D(y) = 0 for all y != x\n"    \
"  Let Fx be the geodesic reconstruction of Dx under F \n"    \
"  O(x) = volume((Fx-Sp).Br(x)) \n"    \
"  where Br(x) is the disc centered on x with radius r\n"    \
"EndFor\n"    \
" \n"    \
"\"Topological operators for the detection of\n"    \
"curvilinar structures in grayscale images\", in preparation.\n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: topogray\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__crop__c__ \
"Usage: crop in.pgm x y [z] w h [d] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"For a 2d image, extracts the rectangle with upper left corner (x,y),\n"    \
"of width w and height h from the image in.pgm .\n"    \
"For a 3d image, extracts the cuboid with upper left corner (x,y, z),\n"    \
"of width w, height h and depth d from the image in.pgm .\n"    \
"\n"    \
"Types supported: byte 2d, int32_t 2d, float 2d, byte 3d, int32_t 3d, float 3d.\n"    \
"\n"    \
"Category: convert, geo\n"    \
", \n"    \
"\n"    \
" Michel Couprie"



#define doc__cropondisk__c__ \
"WRITE ME!!!"



#define doc__curvatures__c__ \
"Usage: curvatures spline.txt n out.txt\n"    \
"\n"    \
"Description:\n"    \
"Samples the spline in  spline.txt and compute the curvature at each sample point.\n"    \
"The file  spline.txt contains a 2D or 3D spline under the format:\n"    \
"s nbpoints  \n"    \
"x1 y1\n"    \
"x2 y2\n"    \
"x3 y3\n"    \
"...\n"    \
"or:\n"    \
"S nbpoints  \n"    \
"x1 y1 z1\n"    \
"x2 y2 z2\n"    \
"x3 y3 z3\n"    \
"...\n"    \
"\n"    \
"Types supported: 2D, 3D splines\n"    \
"\n"    \
"Category: geo\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__curve2spline__c__ \
"Usage: curve2spline curve.list tolerance out.spline\n"    \
"\n"    \
"Description:\n"    \
"\n"    \
"Let C be the discrete open curve described in curve.list. This program finds a approximation of C\n"    \
"in the form of a parametric curve P defined by two (in 2D) or three (in 3D) splines \n"    \
"such that the maximal distance between P and C is beyond the given tolerance.\n"    \
"\n"    \
"The result is given in the form of the list of control points for the splines, followed by\n"    \
"the set of coefficients for each spline segment. \n"    \
"The file format is the following for 2D:\n"    \
"\n"    \
"c n+1 (where n+1 denotes the number of control points)\n"    \
"x1 y1\n"    \
"...\n"    \
"xn+1 yn+1\n"    \
"C0X1 C0Y1 C1X1 C1Y1 C2X1 C2Y1 C3X1 C3Y1\n"    \
"...\n"    \
"C0Xn C0Yn C1Xn C1Yn C2Xn C2Yn C3Xn C3Yn\n"    \
"\n"    \
"and in the 3D case:\n"    \
"\n"    \
"C n+1 (where n+1 denotes the number of control points)\n"    \
"x1 y1 z1\n"    \
"...\n"    \
"xn+1 yn+1 zn+1\n"    \
"C0X1 C0Y1 C0Z1 C1X1 C1Y1 C1Z1 C2X1 C2Y1 C2Z1 C3X1 C3Y1 C3Z1\n"    \
"...\n"    \
"C0Xn C0Yn C0Zn C1Xn C1Yn C1Zn C2Xn C2Yn C2Zn C3Xn C3Yn C3Zn\n"    \
"\n"    \
"The ith segment of the parametric curve P is then defined by:\n"    \
"\n"    \
"x(t) = C3Xi * t^3 + C2Xi * t^2 + C1Xi * t + C0Xi\n"    \
"y(t) = C3Yi * t^3 + C2Yi * t^2 + C1Yi * t + C0Yi\n"    \
"z(t) = C3Zi * t^3 + C2Zi * t^2 + C1Zi * t + C0Zi\n"    \
"with t in [0,1]\n"    \
"\n"    \
" Closed curves are not allowed.\n"    \
"\n"    \
"Types supported: list 2D, list 3D\n"    \
"\n"    \
"Category: draw geo\n"    \
" \n"    \
"\n"    \
" Michel Couprie"



#define doc__curvelength__cxx__ \
"WRITE ME!!!"



#define doc__curvetangents__cxx__ \
"WRITE ME!!!"



#define doc__deframe__c__ \
"Usage: deframe in.pgm out.pgm\n"    \
"\n"    \
"Description:\n"    \
"suppress the frame from an image.\n"    \
"\n"    \
"Types supported: byte 2d, int32_t 2d, float 2d, byte 3d, int32_t 3d, float 3d.\n"    \
"\n"    \
"Category: convert, geo\n"    \
", \n"    \
"\n"    \
" Michel Couprie"



#define doc__delaunay__c__ \
"Usage: delaunay in.list [mask.pgm] out.list\n"    \
"\n"    \
"Description:\n"    \
"Reads a point list in file in.list under the following format:\n"    \
"  \n"    \
"    b &lt;n&gt;         n &lt;n&gt;    \n"    \
"    x1 y1         x1 y1 v1\n"    \
"    x2 y2   ou    x2 y2 v2\n"    \
"    ...           ...\n"    \
"    xn yn         xn yn vn\n"    \
"  \n"    \
"\n"    \
"Computes a Delaunay triangulation and stores the resulting graph \n"    \
"into file out.graph under the following format:\n"    \
"\n"    \
"    G &lt;n&gt;\n"    \
"    x1 y1 v1 ec1 ns1 s11 s12 ... s1ns1\n"    \
"    x2 y2 v2 ec2 ns2 s21 s22 ... s1ns2\n"    \
"    ...\n"    \
"    xn yn vn ecn nsn sn1 sn2 ... s1nsn\n"    \
"\n"    \
"  \n"    \
"where xi, yi are the coordinates of the ith vertex, vi is the associated \n"    \
"value (if given in the input file), eci is a int32_t which indicates \n"    \
"whether the vertex i belongs to the convex hull, nsi denotes the number\n"    \
"of adjacent vertices, and si1 si2 ... sins1 is the list of the indexes of\n"    \
"the adjacent vertices (counted from 0).\n"    \
"\n"    \
"If the optional parameter mask.pgm is given, then it must be a \n"    \
"binary byte image. Any edge of the triangulation which intersects the\n"    \
"mask will be discarded.  \n"    \
"\n"    \
"Types supported: byte 2D\n"    \
"\n"    \
"See also: drawtriangulation.c\n"    \
"\n"    \
"Category: geo\n"    \
"\n"    \
"\n"    \
" algorithm in O(n^2)\n"    \
"\n"    \
" Michel Couprie, Laurent Mercier"



#define doc__delaunaymask__c__ \
"Usage: delaunaymask in.list mask.pgm out.list\n"    \
"\n"    \
"Description:\n"    \
"Reads a point list in file in.list under the following format:\n"    \
"  \n"    \
"    b &lt;n&gt;         n &lt;n&gt;    \n"    \
"    x1 y1         x1 y1 v1\n"    \
"    x2 y2   ou    x2 y2 v2\n"    \
"    ...           ...\n"    \
"    xn yn         xn yn vn\n"    \
"  \n"    \
"\n"    \
"Computes a Delaunay triangulation and stores the resulting graph \n"    \
"\n"    \
"into file out.graph under the following format:\n"    \
"\n"    \
"    G &lt;n&gt;\n"    \
"    x1 y1 v1 ec1 ns1 s11 s12 ... s1ns1\n"    \
"    x2 y2 v2 ec2 ns2 s21 s22 ... s1ns2\n"    \
"    ...\n"    \
"    xn yn vn ecn nsn sn1 sn2 ... s1nsn\n"    \
"\n"    \
"  \n"    \
"where xi, yi are the coordinates of the ith vertex, vi is the associated \n"    \
"value (if given in the input file), eci is a int32_t which indicates \n"    \
"whether the vertex i belongs to the convex hull, nsi denotes the number\n"    \
"of adjacent vertices, and si1 si2 ... sins1 is the list of the indexes of\n"    \
"the adjacent vertices (counted from 0).\n"    \
"\n"    \
"Types supported: byte 2D\n"    \
"\n"    \
"Category: geo\n"    \
"\n"    \
"\n"    \
" algorithm in O(n^2)\n"    \
"\n"    \
" Michel Couprie"



#define doc__deletecomp__c__ \
"Usage: deletecomp in.pgm x y z out.pgm\n"    \
"\n"    \
"Description:\n"    \
"The connected component of the binary image in.pgm\n"    \
"which contains the point (x,y,z)\n"    \
"is deleted.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: connect, topobin\n"    \
", \n"    \
"\n"    \
" Michel Couprie"



#define doc__deletepeaks__c__ \
"Usage: deletepeaks filein.pgm [mask] connexmin fileout.pgm\n"    \
"\n"    \
"Description:\n"    \
"Deletes peaks in a grayscale image. A point p is a peak if T+(p) = 0\n"    \
"(see ref. [BEC97]).\n"    \
"\n"    \
"References: \n"    \
"[BEC97] G. Bertrand, J. C. Everat and M. Couprie: \"Image segmentation through operators based upon topology\",  Journal of Electronic Imaging, Vol.&nbsp;6, No.&nbsp;4, pp.&nbsp;395-405, 1997.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: topogray\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__deletewells__c__ \
"Usage: deletewells filein.pgm [mask] connexmin fileout.pgm\n"    \
"\n"    \
"Description:\n"    \
"Deletes wells in a grayscale image. A point p is a well if T-(p) = 0\n"    \
"(see ref. [BEC97]).\n"    \
"\n"    \
"References: \n"    \
"[BEC97] G. Bertrand, J. C. Everat and M. Couprie: \"Image segmentation through operators based upon topology\",  Journal of Electronic Imaging, Vol.&nbsp;6, No.&nbsp;4, pp.&nbsp;395-405, 1997.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: topogray\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__deriche__c__ \
"WRITE ME!!!"



#define doc__deriche3d__c__ \
"WRITE ME!!!"



#define doc__desfilsclairs3d__c__ \
"WRITE ME!!!"



#define doc__desfilssombres3d__c__ \
"WRITE ME!!!"



#define doc__despics__c__ \
"WRITE ME!!!"



#define doc__despics3d__c__ \
"WRITE ME!!!"



#define doc__despuits__c__ \
"WRITE ME!!!"



#define doc__despuits3d__c__ \
"WRITE ME!!!"



#define doc__detectcercles__c__ \
"WRITE ME!!!"



#define doc__diffimages__c__ \
"Usage: diffimages in1.pgm in2.pgm out.pgm\n"    \
"\n"    \
"Description:\n"    \
"For each pixel x, out[x] = abs (in1[x] - in2[x]). \n"    \
"Images must be of the same type and same dimensions.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n"    \
"\n"    \
"Category: arith\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__dilat__c__ \
"WRITE ME!!!"



#define doc__dilatball__c__ \
"Usage: dilatball in.pgm r [dist] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Morphological binary dilation by a metric ball.\n"    \
"If the parameter  r is a positive integer, then\n"    \
"the structuring element for this dilation is a ball (or a disc) of radius  r.\n"    \
"In this case the result of the dilation is defined by Y = {x; d(x,X)                Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: morpho\n"    \
"\n"    \
"\n"    \
" Michel Couprie 2002\n"    \
"\n"    \
"Example:\n"    \
"\n"    \
"dilatball cells 4 cells_dilatball\n"    \
"\n"    \
"\n"    \
"  \n"    \
"      \n"    \
"  \n"    \
"  cells\n"    \
"      cells_dilatball\n"    \
"  \n"    \
""



#define doc__dilatballnum__c__ \
"Usage: dilatballnum in.pgm r out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Morphological grayscale dilation by an Euclidean ball.\n"    \
"The parameter  r must a positive number (int or float), \n"    \
"the structuring element for this dilation is a ball (or a disc) of radius  r.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: morpho\n"    \
"\n"    \
"\n"    \
" Michel Couprie 2006"



#define doc__dilatbin__c__ \
"WRITE ME!!!"



#define doc__dilatbin3d__c__ \
"WRITE ME!!!"



#define doc__dilatfast__c__ \
"WRITE ME!!!"



#define doc__dilation__c__ \
"Usage: dilation in.pgm se.pgm out.pgm\n"    \
"\n"    \
"Description:\n"    \
"The (plane) structuring element is given by the non-null values in  se.pgm, \n"    \
"its origin (wrt the point (0,0) of  se.pgm ) is given by a comment line in the file se.pgm. \n"    \
"\n"    \
"Types supported: byte 2d, byte 3d, int32_t 2d, long3d\n"    \
"\n"    \
"Category: morpho\n"    \
"\n"    \
"\n"    \
" Michel Couprie 2002\n"    \
"\n"    \
"Example:\n"    \
"\n"    \
"dilation cells ~/Pink/masks/carre5.pgm cells_dilat\n"    \
"\n"    \
"\n"    \
"  \n"    \
"      \n"    \
"  \n"    \
"  cells\n"    \
"      cells_dilat\n"    \
"  \n"    \
""



#define doc__dilatnum__c__ \
"WRITE ME!!!"



#define doc__dir__c__ \
"WRITE ME!!!"



#define doc__directionalfilter__c__ \
"Usage: directionalfilter.c in.pgm width length ndir out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Let F be the original image from  in.pgm .\n"    \
"This operator computes the supremum of the convolutions of F\n"    \
"by a series of kernels K0, ... Kn where n =  ndir - 1, which are defined\n"    \
"by, for each (x,y) and each i in [0...n]: \n"    \
"\n"    \
" @verbatim \n"    \
"sigma = 1 / (2*width*width);\n"    \
"lambda = 1 / (2*length*length);\n"    \
"theta = i * PI / n;\n"    \
"xr = cos(theta) * x - sin(theta) * y;\n"    \
"yr = sin(theta) * x + cos(theta) * y;\n"    \
"Ki(x,y) = exp(-lambda*yr*yr) *\n"    \
"          (4*sigma*sigma*xr*xr - 2*sigma) * \n"    \
"          exp(-sigma*xr*xr) \n"    \
" supported: byte 2d, int32_t 2d, float 2d\n"    \
"\n"    \
"Category: signal\n"    \
"\n"    \
"\n"    \
" Michel Couprie 2003"



#define doc__directions__c__ \
"WRITE ME!!!"



#define doc__dist__c__ \
"Usage: dist in.pgm mode out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Distance to the object X defined by the binary image  in.pgm .\n"    \
"The result function DX(x) is defined by: DX(x) = min {d(x,y), y in X}.\n"    \
"\n"    \
"The distance d used depends on the parameter  mode :\n"    \
" 0: approximate euclidean distance (truncated)\n"    \
" 1: approximate quadratic euclidean distance\n"    \
" 2: chamfer distance ([5,7] in 2D; [4,5,6] in 3D)\n"    \
" 3: exact quadratic euclidean distance\n"    \
" 4: 4-distance in 2d\n"    \
" 5: exact euclidean distance (float)\n"    \
" 8: 8-distance in 2d\n"    \
" 6: 6-distance in 3d\n"    \
" 18: 18-distance in 3d\n"    \
" 26: 26-distance in 3d\n"    \
" 40: 4-distance in 2d (byte coded ouput)\n"    \
" 80: 8-distance in 2d (byte coded ouput)\n"    \
" 60: 6-distance in 3d (byte coded ouput)\n"    \
" 180: 18-distance in 3d (byte coded ouput)\n"    \
" 260: 26-distance in 3d (byte coded ouput)\n"    \
"\n"    \
"The output  out.pgm is of type int32_t for modes Types supported: byte 2d,  byte 3d\n"    \
"\n"    \
"Category: morpho\n"    \
"\n"    \
"\n"    \
" Michel Couprie, Xavier Daragon"



#define doc__distc__c__ \
"Usage: distc in.pgm mode out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Distance to the complementary of the object X defined by the binary \n"    \
"image  in.pgm .\n"    \
"The result function DX(x) is defined by: DX(x) = min {d(x,y), y not in X}.\n"    \
"\n"    \
"The distance d used depends on the parameter  mode :\n"    \
" 0: approximate euclidean distance (truncated)\n"    \
" 1: approximate quadratic euclidean distance\n"    \
" 2: chamfer distance\n"    \
" 3: exact quadratic euclidean distance\n"    \
" 4: 4-distance in 2d\n"    \
" 5: exact euclidean distance (float)\n"    \
" 8: 8-distance in 2d\n"    \
" 6: 6-distance in 3d\n"    \
" 18: 18-distance in 3d\n"    \
" 26: 26-distance in 3d\n"    \
" 40: 4-distance in 2d (byte coded ouput)\n"    \
" 80: 8-distance in 2d (byte coded ouput)\n"    \
" 60: 6-distance in 3d (byte coded ouput)\n"    \
" 180: 18-distance in 3d (byte coded ouput)\n"    \
" 260: 26-distance in 3d (byte coded ouput)\n"    \
"\n"    \
"The output  out.pgm is of type int32_t for modes Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: morpho\n"    \
"\n"    \
"\n"    \
" Michel Couprie, Xavier Daragon"



#define doc__distgeo__c__ \
"Usage: distgeo in.pgm mask.pgm mode out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Geodesic distance to the object X defined by the binary image  in.pgm ,\n"    \
"in the object Y defined by the binary image  mask.pgm .\n"    \
"\n"    \
"The distance d used depends on the parameter  mode :\n"    \
" 4: 4-distance in 2d\n"    \
" 8: 8-distance in 2d\n"    \
" 6: 6-distance in 3d\n"    \
" 18: 18-distance in 3d\n"    \
" 26: 26-distance in 3d\n"    \
"\n"    \
"Types supported: byte 2D, byte 3D\n"    \
"\n"    \
"Category: morpho\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__distsets__c__ \
"Usage: distsets in1.pgm in1.pgm mode [cut] out.list\n"    \
"\n"    \
"Description:\n"    \
"\n"    \
"Computes the distance between the object X defined by the binary image\n"    \
" in1.pgm and the object Y defined by the binary image  in2.pgm .\n"    \
"\n"    \
"Stores the result (a number) in the file  out.list .\n"    \
"\n"    \
"The used pointwise distance is the exact Euclidean distance (float).\n"    \
"\n"    \
"The definition of the set distance used depends on the parameter  mode :\n"    \
" 0: Hausdorff\n"    \
" 1: Baddeley, order 1\n"    \
" 2: Baddeley, order 2\n"    \
" 3: Dubuisson-Jain\n"    \
"\n"    \
"The optional parameter  cut is required only for Baddeley distances. \n"    \
"\n"    \
" The input images  in1.pgm and  in2.pgm must be binary images. No test is done.\n"    \
"\n"    \
"Types supported: byte 2d,  byte 3d\n"    \
"\n"    \
"Category: morpho\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__div__c__ \
"Usage: div in1.pgm in2.pgm out.pgm\n"    \
"\n"    \
"Description:\n"    \
"For each pixel x such that in2[x] != 0, out[x] = in1[x] / in2[x]. \n"    \
"For each pixel x such that in2[x] = 0, out[x] = 0. \n"    \
"Images must be of the same type and same dimensions.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n"    \
"\n"    \
"Category: arith\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__diZenzo__c__ \
"Usage: diZenzo imageRVB.ppm alpha [mode] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Le gradient de diZenzo est dÈfini par la donnÈe de p, q, et t:\n"    \
"\n"    \
"p = Rx*Rx + Vx*Vx + Bx*Bx\n"    \
"\n"    \
"q = Ry*Ry + Vy*Vy + By*By\n"    \
"\n"    \
"t = Rx*Ry + Vx*Vy + Bx*By\n"    \
"\n"    \
"(ou Rx = dÈrivÈe en x de la bande rouge, Ry est la dÈrivÈe en y de la bande rouge, etc.)\n"    \
"\n"    \
"et le module est donnÈe par\n"    \
"\n"    \
"G = sqrt(1/2*(p+q+sqrt((p+q)*(p+q) -4(pq-t*t))))\n"    \
"\n"    \
"La direction est donnÈe par 1/2*atan(2*t/(p-q))\n"    \
"\n"    \
"Si le mode est Ègale ‡ 0 (valeur dÈfaut) alors l'image de sortie est le gradient, \n"    \
"sinon l'image de sortie est une int32_t entre 0 et 360.\n"    \
"\n"    \
"Les gradients sont calculÈs par les filtres de Deriche, de paramËtre alpha\n"    \
"\n"    \
"Types supported: byte 2D\n"    \
"\n"    \
"Category: signal\n"    \
"\n"    \
"\n"    \
" Laurent Najman"



#define doc__double2byte__c__ \
"Usage: double2byte in.pgm [mode] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"\n"    \
"Depending on the value given for the (optional) parameter mode:\n"    \
"   mode = 0 (default) : for all x, out[x] = min(255, arrondi(in[x])).\n"    \
"   mode = 1 : for all x, out[x] = arrondi(in[x]) modulo 256.\n"    \
"   mode = 2 : scales values in the range 0-255.\n"    \
"   mode = 4 : truncation of the square root in the range 0-255.\n"    \
"   mode = 5 : truncation of the log in the range 0-255.\n"    \
"\n"    \
"Types supported: double 2d, double 3d\n"    \
"\n"    \
"Category: convert\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__drawball__c__ \
"Usage: drawball in.pgm radius xc yc zc out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Draws a binary euclidean ball in the image  in.pgm.\n"    \
"The parameter radius specifies the radius of the ball. \n"    \
"The parameters xc, yc, zc specify the center of the ball. \n"    \
"\n"    \
"Types supported: byte 3d\n"    \
"\n"    \
"Category: draw\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__drawcurve__c__ \
"Usage: drawline in.pgm curve.txt out.pgm\n"    \
"\n"    \
"Description:\n"    \
"The file  curve.txt contains a list of points under the format:\n"    \
"c nb_points\n"    \
"x1 y1\n"    \
"x2 y2\n"    \
"x3 y3\n"    \
"...\n"    \
"\n"    \
"Types supported: byte 2D\n"    \
"\n"    \
"Category: draw\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__drawellipse__c__ \
"Usage: drawellipse in.pgm x1 y1 x2 y2 X1 Y1 X2 Y2 [filled] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Draws an  ellipse specified by four points, e.g. the extremities of the small axis \n"    \
"(x1, y1, x2, y2) and the extremities of the big axis \n"    \
"(X1, Y1, X2, Y2).\n"    \
"The ellipse is surimposed to the contents of in.pgm.\n"    \
"If the (optional) int32_t parameter  filled is set (1), then the ellipse is filled.\n"    \
"Otherwise it is empty (default).\n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: draw geo\n"    \
" \n"    \
"\n"    \
" Michel Couprie"



#define doc__drawfield__c__ \
"Usage: drawfield in.pgm [len] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"The file  in.pgm contains a vector field. This operator generates a (binary) image where each vector is represented by a straight line segment of length  len (default 10).\n"    \
"\n"    \
"Types supported: byte 2D, byte 3D\n"    \
"\n"    \
"Category: draw\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__drawfieldlists__c__ \
"Usage: drawfieldlists in1.list in2.list in.pgm [len] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"The files  in1.pgm,  in2.pgm represent a vector field, with the origins of vectors in  in1.pgm and their directions in  in2.pgm. This operator draws in the image  in.pgm a straight line segment of length  len (default 10) for each vector.\n"    \
"\n"    \
"Types supported: byte 2D, byte 3D\n"    \
"\n"    \
"Category: draw\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__drawline__c__ \
"Usage: drawline in.pgm x1 y1 [z1] x2 y2 [z2] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"\n"    \
"Draws a straight line segment between (x1,y1[,z1]) and (x2,y2[,z2])\n"    \
"\n"    \
"Types supported: byte 2D, byte 3D\n"    \
"\n"    \
"Category: draw\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__drawlines__c__ \
"Usage: drawlines in.pgm vect.txt out.pgm\n"    \
"\n"    \
"Description:\n"    \
"The file  vect.txt contains a list of line segments under the format:\n"    \
"l nb_segments\n"    \
"x11 y11 x12 y12\n"    \
"x21 y21 x22 y22\n"    \
"x31 y31 x32 y32\n"    \
"...\n"    \
"\n"    \
"Types supported: byte 2D\n"    \
"\n"    \
"Category: draw\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__drawrect__c__ \
"Usage: drawrect in.pgm x1 y1 z1 x2 y2 z2 out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Draws a rectangle (a box in 3D) with sides parallel to the main axes. \n"    \
"Two diagonally opposed corners are specified by \n"    \
"x1, y1, z1 and x2, y2, z2.\n"    \
"The rectangle is surimposed to the contents of in.pgm.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: draw\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__drawscene__c__ \
"Usage: drawscene in.pgm scene.3sc out.pgm\n"    \
"\n"    \
"Description:\n"    \
"The file  scene.3sc contains a scene under the 3Dscene format.\n"    \
"\n"    \
"Types supported: byte 3D\n"    \
"\n"    \
"Category: draw geo\n"    \
" \n"    \
"\n"    \
" Michel Couprie"



#define doc__drawspline__c__ \
"Usage: drawspline in.pgm spline.txt [len] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Draws a spline which is specified by its control points in a text file.\n"    \
"The parameter  in.pgm gives an image into which the spline is to be drawn.\n"    \
"The file format for  spline.txt is the following for 2D:\n"    \
"\n"    \
"c n+1 (where n+1 denotes the number of control points)\n"    \
"x1 y1\n"    \
"...\n"    \
"xn+1 yn+1\n"    \
"C0X1 C0Y1 C1X1 C1Y1 C2X1 C2Y1 C3X1 C3Y1\n"    \
"...\n"    \
"C0Xn C0Yn C1Xn C1Yn C2Xn C2Yn C3Xn C3Yn\n"    \
"\n"    \
"and in the 3D case:\n"    \
"\n"    \
"C n+1 (where n+1 denotes the number of control points)\n"    \
"x1 y1 z1\n"    \
"...\n"    \
"xn+1 yn+1 zn+1\n"    \
"C0X1 C0Y1 C0Z1 C1X1 C1Y1 C1Z1 C2X1 C2Y1 C2Z1 C3X1 C3Y1 C3Z1\n"    \
"...\n"    \
"C0Xn C0Yn C0Zn C1Xn C1Yn C1Zn C2Xn C2Yn C2Zn C3Xn C3Yn C3Zn\n"    \
"\n"    \
"If parameter  len is given and non-zero, the spline is extended on both sides by straight line segments of length  len. \n"    \
"\n"    \
"Types supported: byte 2D, byte 3D\n"    \
"\n"    \
"Category: draw geo\n"    \
" \n"    \
"\n"    \
" Michel Couprie"



#define doc__drawsplineorient__c__ \
"Usage: drawsplineorient in.pgm in.spline out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Draws spline orientations in a vector field. \n"    \
"The spline is specified by its control points in a text file.\n"    \
"The parameter  in.pgm gives a vector field into which the spline is to be drawn.\n"    \
"The file format for  in.spline is the following for 2D:\n"    \
"\n"    \
"c n+1 (where n+1 denotes the number of control points)\n"    \
"x1 y1\n"    \
"...\n"    \
"xn+1 yn+1\n"    \
"C0X1 C0Y1 C1X1 C1Y1 C2X1 C2Y1 C3X1 C3Y1\n"    \
"...\n"    \
"C0Xn C0Yn C1Xn C1Yn C2Xn C2Yn C3Xn C3Yn\n"    \
"\n"    \
"and in the 3D case:\n"    \
"\n"    \
"C n+1 (where n+1 denotes the number of control points)\n"    \
"x1 y1 z1\n"    \
"...\n"    \
"xn+1 yn+1 zn+1\n"    \
"C0X1 C0Y1 C0Z1 C1X1 C1Y1 C1Z1 C2X1 C2Y1 C2Z1 C3X1 C3Y1 C3Z1\n"    \
"...\n"    \
"C0Xn C0Yn C0Zn C1Xn C1Yn C1Zn C2Xn C2Yn C2Zn C3Xn C3Yn C3Zn\n"    \
"\n"    \
"Types supported: spline 2D, spline 3D\n"    \
"\n"    \
"Category: draw geo\n"    \
" \n"    \
"\n"    \
" Michel Couprie"



#define doc__drawsplines__c__ \
"Usage: drawsplines in.pgm splines.txt [len] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Draws splines which are specified by control points in a text file.\n"    \
"The parameter  in.pgm gives an image into which the splines are to be drawn.\n"    \
"The file format for  splines.txt is the following for 2D:\n"    \
"\n"    \
"The file  splines.txt contains a list of splines under the format:\n"    \
"d nb_splines\n"    \
"nb_points_spline_1  x11 y11  x12 y12 ...\n"    \
"nb_points_spline_2  x21 y21  x22 y22 ...\n"    \
"nb_points_spline_3  x31 y31  x32 y32 ...\n"    \
"...\n"    \
"or, in 3D:\n"    \
"D nb_splines\n"    \
"nb_points_spline_1  x11 y11 z11  x12 y12 z12 ...\n"    \
"nb_points_spline_2  x21 y21 z21  x22 y22 z22 ...\n"    \
"nb_points_spline_3  x31 y31 z31  x32 y32 z32 ...\n"    \
"...\n"    \
"\n"    \
"If parameter  len is given and non-zero, the splines are extended on both sides by straight line segments of length  len. \n"    \
"\n"    \
"Types supported: byte 2D, byte 3D\n"    \
"\n"    \
"Category: draw geo\n"    \
" \n"    \
"\n"    \
" Michel Couprie"



#define doc__drawsplinesorient__c__ \
"Usage: drawsplinesorient in.pgm in.splines out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Draws spline orientations in a vector field. \n"    \
"The spline is specified by its control points in a text file.\n"    \
"The parameter  in.pgm gives a vector field into which the spline is to be drawn.\n"    \
"\n"    \
"The file  splines.txt contains a list of splines under the format:\n"    \
"d nb_splines\n"    \
"nb_points_spline_1  x11 y11  x12 y12 ...\n"    \
"nb_points_spline_2  x21 y21  x22 y22 ...\n"    \
"nb_points_spline_3  x31 y31  x32 y32 ...\n"    \
"...\n"    \
"or, in 3D:\n"    \
"D nb_splines\n"    \
"nb_points_spline_1  x11 y11 z11  x12 y12 z12 ...\n"    \
"nb_points_spline_2  x21 y21 z21  x22 y22 z22 ...\n"    \
"nb_points_spline_3  x31 y31 z31  x32 y32 z32 ...\n"    \
"...\n"    \
"\n"    \
"Types supported: spline 2D, spline 3D\n"    \
"\n"    \
"Category: draw geo\n"    \
" \n"    \
"\n"    \
" Michel Couprie"



#define doc__drawtorus__c__ \
"Usage: drawtorus  r1 r2 xc yc zc out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Draws a binary torus in the image  in.pgm.\n"    \
"The parameters r1, r2 specify respectively the big \n"    \
"and the small radius of the torus. \n"    \
"The parameters xc, yc, zc specify the center of the torus. \n"    \
"If the first parameter is \"null\", then an image with the\n"    \
"appropriate size is created.\n"    \
"\n"    \
"Types supported: byte 3d\n"    \
"\n"    \
"Category: draw\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__drawtriangulation__c__ \
"Usage: drawtriangulation in.list in.pgm out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Reads a triangulation in file in.list under the following format:\n"    \
"\n"    \
"    G &lt;n&gt;\n"    \
"    x1 y1 v1 ec1 ns1 s11 s12 ... s1ns1\n"    \
"    x2 y2 v2 ec2 ns2 s21 s22 ... s1ns2\n"    \
"    ...\n"    \
"    xn yn vn ecn nsn sn1 sn2 ... s1nsn\n"    \
"  \n"    \
"where xi, yi are the coordinates of the ith vertex, vi is the associated \n"    \
"value (if given in the input file), eci is a int32_t which indicates \n"    \
"whether the vertex i belongs to the convex hull, nsi denotes the number\n"    \
"of adjacent vertices, and si1 si2 ... sins1 is the list of the indexes of\n"    \
"the adjacent vertices (counted from 0).\n"    \
"\n"    \
"Draws the triangulation in image in.pgm, output image is out.pgm\n"    \
"\n"    \
"Types supported: byte 2D\n"    \
"\n"    \
"See also: delaunay.c\n"    \
"\n"    \
"Category: geo, draw\n"    \
", \n"    \
"\n"    \
" Michel Couprie"



#define doc__dup__c__ \
"Usage: dup in.pgm out.pgm\n"    \
"\n"    \
"Description:\n"    \
"For each pixel x, out[x] = in[x]\n"    \
"\n"    \
"Types supported: all types\n"    \
"\n"    \
"Category: arith\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__dynamique__c__ \
"Usage: dynamique in.pgm connex [order] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Computes the ordered dynamics of the maxima, with connectivity connex.\n"    \
"The definition of the ordered dynamics is the one given in [Ber05]. \n"    \
"The optional argument order is one of the following:\n"    \
" 0: altitude [default] \n"    \
" 1: area\n"    \
" 2: volume\n"    \
"\n"    \
"References: \n"    \
"[Ber05] G. Bertrand: \"A new definition of the dynamics\", Procs. ISMM05, Springer, series Computational Imaging and Vision, Vol.&nbsp;30, pp.&nbsp;197-206, 2005.\n"    \
"\n"    \
"Types supported: byte 2D, byte 3D.\n"    \
"\n"    \
"Category: \n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__dynamiquefilter__c__ \
"Usage: dynamiquefilter in.pgm connex value [order] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Computes the ordered dynamics of the maxima, with connectivity connex, \n"    \
"selects the maxima with a dynamics greater or equal to value,\n"    \
"and find the maximal components which includes these maxima. \n"    \
"The definition of the ordered dynamics is the one given in [Ber05]. \n"    \
"The optional argument order is one of the following:\n"    \
" 0: altitude [default] \n"    \
" 1: area\n"    \
" 2: volume\n"    \
"\n"    \
"References: \n"    \
"[Ber05] G. Bertrand: \"A new definition of the dynamics\", Procs. ISMM05, Springer, series Computational Imaging and Vision, Vol.&nbsp;30, pp.&nbsp;197-206, 2005.\n"    \
"\n"    \
"Types supported: byte 2D, byte 3D.\n"    \
"\n"    \
"Category: \n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__dynrecons__c__ \
"WRITE ME!!!"



#define doc__ecarttype__c__ \
"WRITE ME!!!"



#define doc__eden__cxx__ \
"Usage: eden in.pgm niter grow shrink topo out.pgm\n"    \
"\n"    \
"Description:\n"    \
"\n"    \
"Algorithm is as follows.\n"    \
"\n"    \
"For growth:\n"    \
"   start from known set\n"    \
"   take pixels from the border into a queue\n"    \
"   choose randomly one such pixel\n"    \
"   set it to 1\n"    \
"   update queues\n"    \
"   repeat\n"    \
"\n"    \
"For shrinking:\n"    \
"   same thing but set to 0\n"    \
"\n"    \
"For adding boundary noise\n"    \
"   alternate growth and shrinking\n"    \
"\n"    \
"With topological constraints\n"    \
"   make sure the point to add or remove is simple\n"    \
"\n"    \
"Parameters:\n"    \
"   niter (positive integer): number of iterations\n"    \
"   grow (1/0): perform growing or not\n"    \
"   shrink (1/0): perform shrinking or not\n"    \
"   topo (26/6/8/4/0): connectivity for the object - 0: no topological constraint\n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: geo\n"    \
"\n"    \
"\n"    \
" Hugues Talbot"



#define doc__egalise__c__ \
"WRITE ME!!!"



#define doc__ellipsefit__c__ \
"Usage: ellipsefit in.pgm pts.list filled out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Identifies the ellipse E which minimizes the sum of the (approximate) distances\n"    \
"between the points in  pts.list and E. \n"    \
"If  filled (int32_t) is set, then a filled ellipse is drawn.\n"    \
"\n"    \
"Ref: \n"    \
"Andrew W. Fitzgibbon, Maurizio Pilu, and Robert B. Fisher\n"    \
"Direct least-squares fitting of ellipses,\n"    \
"IEEE Transactions on Pattern Analysis and Machine Intelligence, 21(5), 476--480, May 1999\n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: geo\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__ellipseincl__c__ \
"Usage: ellipseincl in.pgm pts.list filled out.pgm\n"    \
"\n"    \
"Description:\n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: geo\n"    \
"\n"    \
"\n"    \
" Michel Couprie and Yskandar Hamam"



#define doc__ellipseparams__c__ \
"Usage: ellipseparams pts.list out.list\n"    \
"\n"    \
"Description:\n"    \
"Identifies the ellipse E which minimizes the sum of the (approximate) distances\n"    \
"between the points in  pts.list and E. \n"    \
"The output is a list containing 6 numbers: a1, a2, r1, r2, r3, r4\n"    \
"where a1 is the half of the big axis length, a2 is the\n"    \
"the half of the little axis length, and each ri is the mean \"distance\",\n"    \
"for quadrant i, between intput points and the computed ellipse \n"    \
"(the expected value for points forming a perfect ellipse is 0)\n"    \
"\n"    \
"Ref: \n"    \
"Andrew W. Fitzgibbon, Maurizio Pilu, and Robert B. Fisher\n"    \
"Direct least-squares fitting of ellipses,\n"    \
"IEEE Transactions on Pattern Analysis and Machine Intelligence, 21(5), 476--480, May 1999\n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: geo\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__ellipticite__c__ \
"WRITE ME!!!"



#define doc__encadre__c__ \
"WRITE ME!!!"



#define doc__enframe__c__ \
"Usage: enframe in.pgm grayvalue [width] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Adds a border to the input image, filled with the value\n"    \
" grayvalue. The width of the border may be given as parameter  width, \n"    \
"otherwise its value is 1.\n"    \
"\n"    \
"Types supported: byte 2d, int32_t 2d, float 2d, byte 3d, int32_t 3d, float 3d.\n"    \
"\n"    \
"Category: convert, geo\n"    \
", \n"    \
"\n"    \
" Michel Couprie"



#define doc__equal__c__ \
"Usage: equal in1.pgm in2.pgm\n"    \
"\n"    \
"Description:\n"    \
"This function returns 1 (no error) if for each pixel x, in1[x] == in2[x]; \n"    \
"otherwise it returns 0.\n"    \
"Images must be of the same dimensions.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n"    \
"\n"    \
"Category: arith\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__eros__c__ \
"WRITE ME!!!"



#define doc__erosball__c__ \
"Usage: erosball in.pgm r [dist] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Morphological binary erosion by a metric ball.\n"    \
"This erosion is the dual of the dilatball operator, \n"    \
"i.e. erosball(X) = inverse(dilatball(inverse(X))).\n"    \
"The structuring element for this erosion is a ball (or a disc) of radius  r.\n"    \
"The erosion is computed by thresholding a distance map.\n"    \
"The distance used depends on the optional parameter  dist (default is 0) :\n"    \
" 0: approximate euclidean distance\n"    \
" 2: chamfer distance\n"    \
" 4: 4-distance in 2d\n"    \
" 8: 8-distance in 2d\n"    \
" 6: 6-distance in 3d\n"    \
" 18: 18-distance in 3d\n"    \
" 26: 26-distance in 3d\n"    \
"\n"    \
" The input image  in.pgm must be a binary image. No test is done.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: morpho\n"    \
"\n"    \
"\n"    \
" Michel Couprie 2002\n"    \
"\n"    \
"Example:\n"    \
"\n"    \
"erosball cells 3 cells_erosball\n"    \
"\n"    \
"\n"    \
"  \n"    \
"      \n"    \
"  \n"    \
"  cells\n"    \
"      cells_erosball\n"    \
"  \n"    \
""



#define doc__erosballnum__c__ \
"Usage: erosballnum in.pgm r out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Morphological grayscale erosion by an Euclidean ball.\n"    \
"The parameter  r must a positive number (int or float), \n"    \
"the structuring element for this erosion is a ball (or a disc) of radius  r.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: morpho\n"    \
"\n"    \
"\n"    \
" Michel Couprie 2006"



#define doc__erosbin__c__ \
"WRITE ME!!!"



#define doc__erosbin3d__c__ \
"WRITE ME!!!"



#define doc__erosfast__c__ \
"WRITE ME!!!"



#define doc__erosfast3d__c__ \
"WRITE ME!!!"



#define doc__erosion__c__ \
"Usage: erosion in.pgm se.pgm out.pgm\n"    \
"\n"    \
"Description:\n"    \
"The (plane) structuring element is given by the non-null values in  se.pgm, \n"    \
"its origin (wrt the point (0,0) of  se.pgm ) is given by a comment line in the file se.pgm. \n"    \
"\n"    \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d\n"    \
"\n"    \
"Category: morpho\n"    \
"\n"    \
"\n"    \
" Michel Couprie 2002\n"    \
"\n"    \
"Example:\n"    \
"\n"    \
"erosion cells ~/Pink/masks/carre5.pgm cells_eros\n"    \
"\n"    \
"\n"    \
"  \n"    \
"      \n"    \
"  \n"    \
"  cells\n"    \
"      cells_eros\n"    \
"  \n"    \
""



#define doc__erosnum__c__ \
"WRITE ME!!!"



#define doc__erosplan__c__ \
"WRITE ME!!!"



#define doc__exp__c__ \
"Usage: exp in.pgm out.pgm\n"    \
"\n"    \
"Description:\n"    \
"For each pixel x, out[x] = exp(in[x]).\n"    \
"\n"    \
"Types supported: float 2d, float 3d\n"    \
"\n"    \
"Category: arith\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__expandframe__c__ \
"Usage: expandframe in.pgm n out.pgm\n"    \
"\n"    \
"Description: \n"    \
"Expands the values of the image in a n-pixel wide band outside the image .\n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: convert\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__explode__c__ \
"Usage: explode in.pgm [begin end step] name_prefix\n"    \
"\n"    \
"Description:\n"    \
"Generated file names are of the form: name_prefixnnnn.pgm, \n"    \
"where nnnn is a four digit decimal integer. \n"    \
"\n"    \
"Types supported: byte 3d\n"    \
"\n"    \
"Category: convert\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__explodecomp__c__ \
"Usage: explodecomp in.pgm connex  name_prefix\n"    \
"\n"    \
"Description:\n"    \
"Generated file names are of the form: name_prefixnnnn.pgm, \n"    \
"where nnnn is a four digit decimal integer. \n"    \
"The argument  connex selects the connectivity (4, 8 in 2D; 6, 18, 26 in 3D).\n"    \
"The argument  selects the kind of flat zone\n"    \
"to be labeled:\n"    \
"  min selects regional minima (or background components for a binary image)\n"    \
"  max selects regional maxima (or foreground components for a binary image)\n"    \
"  pla selects all flat zones (plateaux).\n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: convert\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__extractline__c__ \
"Usage: extractline in.pgm x1 y1 x2 y2 out.pgm\n"    \
"\n"    \
"Description:\n"    \
"The original 2D image is in.pgm. \n"    \
"The output 1D image out.pgm contains the line between \n"    \
"points (x1,y1) and (x2,y2) extracted from in.pgm.\n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: geo\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__extractplane__c__ \
"Usage: extractplane in.pgm n plane out.pgm\n"    \
"\n"    \
"Description:\n"    \
"The original 3D image is in.pgm. The parameter n is an integer.\n"    \
"The parameter plane is set to xy, yx, \n"    \
"xz, zx, yz, zy.\n"    \
"The order of the letters x, y, z is meaningfull: for example, zy means that \n"    \
"each row of the output image will correspond to a z-line of the original image, \n"    \
"each column of the output image will correspond to a y-line of the original image.\n"    \
"The output image out.pgm contains the nth plane of the given form extracted\n"    \
"from in.pgm.\n"    \
"\n"    \
"Types supported: byte 3d\n"    \
"\n"    \
"Category: geo\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__extrude__c__ \
"Usage: extrude in.pgm nbrep [xdim ydim zdim] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Catenates a sequence of 2d (resp. 1d) images, made of the repetition of the same image, into a 3d (resp. 2d) image.\n"    \
"\n"    \
"Types supported: byte 1d, 2d\n"    \
"\n"    \
"Category:  convert\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__fermetrous3d__c__ \
"WRITE ME!!!"



#define doc__fft__c__ \
"Usage: fft in1 in2 dir out1 out2\n"    \
"\n"    \
"Description:\n"    \
"computes the 2 dimensional Fast Fourier Transform of an image.\n"    \
"The input image must be square and the number of rows (or columns)\n"    \
"MUST be a power of two.\n"    \
"\n"    \
"This program will compute either a forward or inverse FFT, depending on\n"    \
"the direction requested with the  dir option.  A  dir value of 0\n"    \
"will result in a forward FFT, and a  dir value of 1 will result\n"    \
"in an inverse FFT. \n"    \
"\n"    \
"The input arguments are described as follows:\n"    \
"\n"    \
" in1\n"    \
"specifies the input image, which can be of data type BYTE, LONG or FLOAT.\n"    \
"If the input image is of data type BYTE, then the data will first be\n"    \
"converted to FLOAT before proceeding.\n"    \
"If the first input image is of type BYTE or FLOAT, then it is assumed that \n"    \
"it is the real component of the complex pair, and the imaginary component \n"    \
"may be input using the optional in2 input argument.  \n"    \
"If no imaginary component is specified when\n"    \
"using a real input, then the imaginary component is assumed to be zero.  \n"    \
"\n"    \
" in2\n"    \
"specifies the optional input image, which can be of data type BYTE, LONG\n"    \
"or FLOAT. The keyword \"null\" may be used.\n"    \
"It is assumed that this image represents the imaginary component of the\n"    \
"complex pair.  \n"    \
"\n"    \
" out1\n"    \
"specifies the real output image, which will be a single band image of data\n"    \
"type FLOAT.  This image contains only the real component of the complex pair.\n"    \
"\n"    \
" out2\n"    \
"specifies the imaginary output image, which will be a single band image of data\n"    \
"type FLOAT.  This image contains only the imaginary component of the complex\n"    \
"pair.\n"    \
"\n"    \
" dir\n"    \
"specifies the FFT direction.  A  dir of 0 will result in a forward FFT, \n"    \
"and a  dir of 1 will result in an inverse FFT.\n"    \
"\n"    \
"Note that it is the users responsibility to ensure that the correct components\n"    \
"of the image are used when requesting an FFT or Inverse FFT.  Unexpected \n"    \
"results may occur if the user requests an inverse FFT and only inputs the real \n"    \
"component of an image.  If an FFT of an image is taken, then both the real and \n"    \
"imaginary components must be used to obtain a correct inverse FFT.\n"    \
"\n"    \
"For a forward FFT, the input data is multiplied by (-1)**(x+y) where (x,y) is\n"    \
"the pixel coordinate. This has the effect of shifting the frequency domain\n"    \
"result so that the DC component is at (N/2,N/2) rather than (0,0). For the\n"    \
"inverse FFT case, the data is multiplied by (-1)**(x+y) AFTER the\n"    \
"FFT processing, accounting for the fact that the input frequency domain data\n"    \
"was center-shifted by the forward FFT. The center-shifted frequency domain\n"    \
"representation is much easier to visualize and filter than it would be\n"    \
"if not shifted. For more information on the shifting teqchnique, see\n"    \
"R.C. Gonsalez and P. Wintz, \"Digital Image Processing, 2nd ed, sec 3.2.2,\n"    \
"p. 77. (1987). The center-shifting should really be an option.\n"    \
"\n"    \
"For the forward FFT, there is no scaling on the data. For the inverse FFT,\n"    \
"the data is scaled by 1/(N*N). Thus, to generate a sinewave of amplitude\n"    \
"1.0 for a 64x64 complex image to be handed to the inverse FFT, there should\n"    \
"be two impulses at conjugate locations (symmetric about the center of the image)\n"    \
"each with amplitude 0.5/(64*64). Why 0.5? It's because each impulse carries\n"    \
"half of the power! The scaling should really be an option, but it currently\n"    \
"is not (perhaps in a future patch).\n"    \
"\n"    \
"Types supported: byte 2d, int32_t 2d, float 2d\n"    \
"\n"    \
"Category: signal\n"    \
"\n"    \
"\n"    \
" Scott Wilson, Rick Bogart, Lyle Bacon"



#define doc__filtreordre__c__ \
"WRITE ME!!!"



#define doc__fitcircle__c__ \
"Usage: fitcircle in.pgm x y r [translate] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Identifies the circle C which locally minimizes the mean gray level \n"    \
"of the image  in.pgm in the vincinity of the circle\n"    \
"of parameters  x ,  y and  r.\n"    \
"If the parameter  translate is added, then the minimization is\n"    \
"done only for dimensions x and y.\n"    \
"Draws this circle in  out.pgm .\n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: geo\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__float2byte__c__ \
"Usage: float2byte in.pgm [mode] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"\n"    \
"Depending on the value given for the (optional) parameter mode:\n"    \
"   mode = 0 (default) : for all x, out[x] = min(255, arrondi(in[x])).\n"    \
"   mode = 1 : for all x, out[x] = arrondi(in[x]) modulo 256.\n"    \
"   mode = 2 : scales values in the range 0-255.\n"    \
"   mode = 4 : truncation of the square root in the range 0-255.\n"    \
"   mode = 5 : truncation of the log in the range 0-255.\n"    \
"\n"    \
"Types supported: float 2d, float 3d\n"    \
"\n"    \
"Category: convert\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__float2long__c__ \
"Usage: float2long in.pgm offset factor out.pgm\n"    \
"\n"    \
"Description:\n"    \
"\n"    \
"Applies the following tranformation to each value x in  in.pgm :\n"    \
"\n"    \
"X = round(offset + factor * x)\n"    \
"\n"    \
"Types supported: float 2d, float 3d\n"    \
"\n"    \
"Category: convert\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__fmm__c__ \
"Usage: fmm speed.pgm seeds.pgm stop threshold seedout.pgm distanceout.pgm\n"    \
"\n"    \
"Description:\n"    \
"\n"    \
"The fast marching method of J.A. Sethian is a geodesic distance transform.\n"    \
"It integrates the constant arrival hyperbolic PDE. A rough physical analogy\n"    \
"is the following: Assume an anisotropic medium with varying propagation\n"    \
"celerities, and waves starting from various seeds travelling through that\n"    \
"medium. This function computes the successive arrival times of these\n"    \
"waves, as well as the propagation of the initial labels. The successive\n"    \
"arrival times are equivalent to a distance transform. The propagation\n"    \
"of the label yield a partition similar to a Voronoi.\n"    \
"\n"    \
"If the speed function is constant and equal to 1, the arrival times would indeed\n"    \
"be the Euclidean distance function and the partition the Euclidean Voronoi.\n"    \
"Due to discretisation issue, the result is only approximately Euclidiean\n"    \
"(to second order).\n"    \
"\n"    \
"Stopping criteria:\n"    \
"stop is the stopping criteria :\n"    \
"\n"    \
" stop = 0 => no stop\n"    \
" stop = 1 => stop on metric (if speed function >= threshold)\n"    \
" stop = 2 => stop on distance (if distance > threshold)\n"    \
"\n"    \
"the threshold is given after.\n"    \
"\n"    \
"Types supported: integer, float Nd (N >= 2)\n"    \
"speed must be float, seeds must be integer.\n"    \
"\n"    \
"Category: morpho\n"    \
"\n"    \
"\n"    \
" Hugues Talbot and Ben Appleton"



#define doc__frame__c__ \
"Usage: frame in.pgm [width] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"The result out.pgm has the same size as in.pgm. Its border is set to 255, \n"    \
"all other pixels are set to 0.\n"    \
"\n"    \
"If the optional parameter  width is given, then the border has thickness 'width'.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: geo\n"    \
"\n"    \
"\n"    \
" Michel Couprie 2000"



#define doc__GA2khalimsky__c__ \
"WRITE ME!!!"



#define doc__GA2tikz__c__ \
"Usage: GA2tikz in.ga [b] out.tikz\n"    \
"\n"    \
"Description:\n"    \
"Produces a LaTeX file from a binary or grayscale GA image.\n"    \
"If b is present, the result is binary otherwise the edges are greyscale colored.\n"    \
"\n"    \
"The result should be compiled with pdflatex to produce a pdf file.\n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: convert\n"    \
"\n"    \
"\n"    \
" Laurent Najman"



#define doc__gaussianfilter__c__ \
"Usage: gaussianfilter in.pgm alpha out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Deriche's recursive implementation of the smoothing filter\n"    \
"operator (approximates the convolution with a Gaussian kernel). \n"    \
"The parameter  alpha (double) controls the spatial extension of the\n"    \
"filter: 0   \n"    \
"[Der90] R. Deriche, \"Fast algorithms for low-level vision\",\n"    \
"IEEE Transactions on PAMI, 12(1), 78-87, 1990.\n"    \
"\n"    \
"Types supported: byte 2d, long 2d, float 2d, byte 3d, long 3d, float 3d\n"    \
"\n"    \
"Category: signal\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__GAwatershed__c__ \
"WRITE ME!!!"



#define doc__GAwatershedDouble__c__ \
"WRITE ME!!!"



#define doc__genbicol__c__ \
"WRITE ME!!!"



#define doc__genbini__c__ \
"WRITE ME!!!"



#define doc__genfield__c__ \
"Usage: genfield  out.pgm\n"    \
"\n"    \
"Description:\n"    \
"The size of the result field out.pgm is taken from image\n"    \
"in.pgm, or given by the parameters rs, cs, ds. \n"    \
"\n"    \
"Types supported: float 2d, float 3d\n"    \
"\n"    \
"Category: arith, geo\n"    \
", \n"    \
"\n"    \
" Michel Couprie"



#define doc__genimage__c__ \
"Usage: genimage  col out.pgm\n"    \
"\n"    \
"Description:\n"    \
"The size of the result image out.pgm is taken from image\n"    \
"in.pgm, or given by the parameters rs, cs, ds. \n"    \
"For a 2D image, the parameter ds (depth size) is set to 1.\n"    \
"The color of the result image is given by the parameter col.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: arith, geo\n"    \
", \n"    \
"\n"    \
" Michel Couprie"



#define doc__genkernel__c__ \
"WRITE ME!!!"



#define doc__genlut__c__ \
"Usage: genlut ncol minhue maxhue minlum maxlum minsat maxsat  out.lut\n"    \
"\n"    \
"Description: Generates a lookup table with a continuous color gradient.\n"    \
"\n"    \
"A lookup table is represented by a special ppm image with 1 row. \n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: convert\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__genplane__c__ \
"Usage: genplane rs cs ds dir n out.pgm\n"    \
"\n"    \
"Description:\n"    \
"The size of the result image out.pgm is given by the parameters \n"    \
"rs, cs, ds.\n"    \
"The parameter dir is set to x, y, or z.\n"    \
"The parameter n is an integer.\n"    \
"The output binary image out.pgm contains the plane defined by dir=n.\n"    \
"\n"    \
"Types supported: byte 3d\n"    \
"\n"    \
"Category: geo\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__geodilat__c__ \
"Usage: geodilat g.pgm f.pgm connex niter out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Geodesic dilation of g.pgm under f.pgm.\n"    \
"Let G and F be the two input images. If G is not under F, then \n"    \
"G is replaced initially by min(G,F).\n"    \
"The structuring element is specified by the value of the parameter connex, \n"    \
"which can be one of the following ones: 4, 8 in 2d, or 6, 18, 26 in 3d.\n"    \
"The parameter  niter sets the number of iterations. If  niter = -1,\n"    \
"then the iterations continue until stability.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d.\n"    \
"\n"    \
"Category: connect, morpho\n"    \
", \n"    \
"\n"    \
" Michel Couprie"



#define doc__geoeros__c__ \
"Usage: geoeros g.pgm f.pgm connex niter out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Geodesic erosion of g.pgm over f.pgm.\n"    \
"Let G and F be the two input images. If G is not over F, then \n"    \
"G is replaced initially by max(G,F).\n"    \
"The structuring element is specified by the value of the parameter connex, \n"    \
"which can be one of the following ones: 4, 8 in 2d, or 6, 18, 26 in 3d.\n"    \
"The parameter  niter sets the number of iterations. If  niter = -1,\n"    \
"then the iterations continue until stability.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d.\n"    \
"\n"    \
"Category: connect, morpho\n"    \
", \n"    \
"\n"    \
" Michel Couprie"



#define doc__gettree__c__ \
"WRITE ME!!!"



#define doc__gradientcd__c__ \
"Usage: gradientcd in.pgm alpha out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Deriche's recursive implementation of the\n"    \
"Canny's gradient operator. The parameter \n"    \
" alpha (double) controls the spatial extension of the\n"    \
"filter: 0 Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: signal\n"    \
"\n"    \
"\n"    \
"Reference:\n"    \
"\n"    \
"[Der90] R. Deriche: \"Fast algorithms for low-level vision\",\n"    \
"IEEE Transactions on PAMI, Vol.&nbsp;12, No.&nbsp;1, pp.&nbsp;78-87, 1990.\n"    \
"\n"    \
" Michel Couprie"



#define doc__gradill__c__ \
"WRITE ME!!!"



#define doc__gradinf__c__ \
"WRITE ME!!!"



#define doc__gradinf3d__c__ \
"WRITE ME!!!"



#define doc__gradsup__c__ \
"WRITE ME!!!"



#define doc__granulometry__c__ \
"Usage: granulometry in.pgm [rmin] rmax out.list\n"    \
"\n"    \
"Description:\n"    \
"Let X be the set of non-null points in in.pgm.\n"    \
"This operator computes the area (volume in 3d) of the opening of X by \n"    \
"euclidean balls of increasing radius, ranging from 1 \n"    \
"(or rmin if this parameter is specified) to rmax. \n"    \
"The result out.list contains a list of couples r a where \n"    \
"r is a radius and a is the corresponding area (or volume).\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: morpho\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__graph2pgm__c__ \
"Usage: graph2pgm in.graph  out.pgm\n"    \
"\n"    \
"Description:\n"    \
"\n"    \
"Reads the file in.graph. Each vertex of this graph must have integer coordinates, and \n"    \
"represents a pixel/voxel of a 2D/3D image.\n"    \
"If a file name in.pgm is given, then the points read in in.graph are \n"    \
"inserted (if possible) into the image read in in.pgm. Else, they are inserted in\n"    \
"a new image, the dimensions of which are rs, cs, and ds.\n"    \
"\n"    \
"Types supported: graph\n"    \
"\n"    \
"Category: convert\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__graph2ps__c__ \
"Usage: graph2ps in.graph out.ps\n"    \
"\n"    \
"Description:\n"    \
"Draws a planar graph into a ps file.\n"    \
"\n"    \
"Types supported: graph\n"    \
"\n"    \
"Category: convert\n"    \
"\n"    \
"\n"    \
" Michel Couprie 2009"



#define doc__grayskel__c__ \
"Usage: grayskel in.pgm  connex lambda out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Skeleton, either homotopic (lambda = 0) or non-homotopic, for 2D grayscale images\n"    \
"The skeleton is obtained by iterative lowering of lambda-destructible points\n"    \
"(see [CBB01]).\n"    \
"\n"    \
"Reference: \n"    \
"[CBB01] M. Couprie, F.N. Bezerra, Gilles Bertrand: \"Topological operators for\n"    \
"grayscale image processing\",  Journal of Electronic Imaging, Vol.&nbsp;10, No.&nbsp;4, pp.&nbsp;1003-1015, 2001.\n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: topogray\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__grid__c__ \
"WRITE ME!!!"



#define doc__hcutting__c__ \
"Usage: hcutting in.pgm r connex out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Homotopic cutting (pseudo opening) by a ball of radius  r.\n"    \
"\n"    \
"Reference: \n"    \
"[CB04] M. Couprie and G. Bertrand:\n"    \
"Topology preserving alternating sequential filter for smoothing \n"    \
"2D and 3D objects,  Journal of Electronic Imaging, Vol.&nbsp;13, No.&nbsp;4, pp.&nbsp;720-730, 2004.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: morpho, topobin\n"    \
", \n"    \
"\n"    \
" Michel Couprie 2002"



#define doc__hdilatball__c__ \
"Usage: hdilatball in.pgm radius dist connex out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Performs a topologically controlled dilation, that is, a homotopic thickening \n"    \
"constrained by the dilation of the input object.\n"    \
"\n"    \
"The parameter  radius gives the radius of the ball which is \n"    \
"used as structuring element for the dilation.\n"    \
"\n"    \
"The parameter  dist is a numerical code\n"    \
"indicating the distance which is used to compute the dilation.\n"    \
"The possible choices are:\n"    \
" 0: approximate euclidean distance\n"    \
" 1: approximate quadratic euclidean distance\n"    \
" 2: chamfer distance\n"    \
" 3: exact quadratic euclidean distance\n"    \
" 4: 4-distance in 2d\n"    \
" 8: 8-distance in 2d\n"    \
" 6: 6-distance in 3d\n"    \
" 18: 18-distance in 3d\n"    \
" 26: 26-distance in 3d\n"    \
"\n"    \
"The parameter  connex indicates the connectivity of the binary object.\n"    \
"Possible choices are 4, 8 in 2d and 6, 26 in 3d.\n"    \
"\n"    \
"Let X be the set corresponding to the input image  in.pgm, and let\n"    \
"Xbar denote its complementary set. The algorithm is the following:\n"    \
"\n"    \
" @verbatim \n"    \
"I = dilatball(X, dist, r)\n"    \
"Repeat:\n"    \
"    Select a point x in [Xbar inter I] such that dist(x,X) is minimal\n"    \
"    If x is simple for X then\n"    \
"        X = X union {x}\n"    \
"	I = I  {x}\n"    \
"Until [Xbar inter I] = emptyset\n"    \
"Result: X\n"    \
" supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: topobin\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__heightmaxima__c__ \
"Usage: heightmaxima in.pgm connex height out.pgm\n"    \
"\n"    \
"Description:\n"    \
"h-maxima with connexity connex and depth height.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: connect\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__heightminima__c__ \
"Usage: heightminima in.pgm connex height out.pgm\n"    \
"\n"    \
"Description:\n"    \
"h-minima with connexity connex and depth height.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: connect\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__heightselnb__c__ \
"Usage: heightselnb in.pgm connex nb out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Segmentation by selection of a given number or components, based on a \n"    \
"height (contrast) criterion. \n"    \
"Connexity = connex ; desired number of components = nb.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: connect\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__hello__cpp__ \
"WRITE ME!!!"



#define doc__herosball__c__ \
"Usage: herosball in.pgm radius dist connex out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Performs a topologically controlled erosion, that is, a homotopic thinning \n"    \
"constrained by the erosion of the input object.\n"    \
"\n"    \
"The parameter  radius gives the radius of the ball which is \n"    \
"used as structuring element for the erosion.\n"    \
"\n"    \
"The parameter  dist is a numerical code\n"    \
"indicating the distance which is used to compute the erosion.\n"    \
"The possible choices are:\n"    \
" 0: approximate euclidean distance\n"    \
" 1: approximate quadratic euclidean distance\n"    \
" 2: chamfer distance\n"    \
" 3: exact quadratic euclidean distance\n"    \
" 4: 4-distance in 2d\n"    \
" 8: 8-distance in 2d\n"    \
" 6: 6-distance in 3d\n"    \
" 18: 18-distance in 3d\n"    \
" 26: 26-distance in 3d\n"    \
"\n"    \
"The parameter  connex indicates the connectivity of the binary object.\n"    \
"Possible choices are 4, 8 in 2d and 6, 26 in 3d.\n"    \
"\n"    \
"Let X be the set corresponding to the input image  in.pgm, and let\n"    \
"Xbar denote its complementary set. The algorithm is the following:\n"    \
"\n"    \
" @verbatim \n"    \
"I = erosball(X, dist, r)\n"    \
"Repeat:\n"    \
"    Select a point x in X  I such that dist(x,Xbar) is minimal\n"    \
"    If x is simple for X then\n"    \
"        X = X  {x}\n"    \
"	I = I union {x}\n"    \
"Until X == I\n"    \
"Result: X\n"    \
" supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: topobin\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__hfilling__c__ \
"Usage: hfilling in.pgm r connex out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Homotopic filling (pseudo closing) by a ball of radius  r.\n"    \
"\n"    \
"Reference: \n"    \
"[CB04] M. Couprie and G. Bertrand:\n"    \
"Topology preserving alternating sequential filter for smoothing \n"    \
"2D and 3D objects,  Journal of Electronic Imaging, Vol.&nbsp;13, No.&nbsp;4, pp.&nbsp;720-730, 2004.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: morpho, topobin\n"    \
", \n"    \
"\n"    \
" Michel Couprie 2002"



#define doc__histo__c__ \
"Usage: histo in.pgm [mask.pgm] out.list\n"    \
"\n"    \
"Description:\n"    \
"Calculates the histogram of  im.pgm (masked by the binary image\n"    \
" mask.pgm, if given) and saves it in file  out.list .\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d\n"    \
"\n"    \
"Category: histo\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__histo2__c__ \
"WRITE ME!!!"



#define doc__histoazimuth__c__ \
"WRITE ME!!!"



#define doc__histodist__c__ \
"Usage: histodist h1.list h2.list [type] out.list\n"    \
"\n"    \
"Description:\n"    \
"Computes the distance between the two histograms given in  h1.list and  h2.list. The result is written in  out.list.\n"    \
"\n"    \
"The distance is an edit distance, it is defined as the minimal number of elementary operations (moving one unit from a bin to the neighboring one) that is necessary to transform h1 into h2.\n"    \
"\n"    \
"Parameter  type (default 0) indicates whether the histogram must be interpreted as circular (type = 1) of not (type = 0). Circular interpretation implies that first and last bins are considered as neighbors.\n"    \
"\n"    \
"Types supported: list\n"    \
"\n"    \
"Category: histo\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__histoelevation__c__ \
"WRITE ME!!!"



#define doc__histolisse__c__ \
"Usage: histolisse in.pgm L [mask.pgm] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Generates a binary image which contains the graphical representation\n"    \
"of the smoothed histogram of  in.pgm (masked by the binary image\n"    \
" mask.pgm, if given).  L : smoothing coefficient, 0Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: histo\n"    \
"\n"    \
"\n"    \
" Cedric Allene 2003"



#define doc__histopgm__c__ \
"Usage: histopgm in.pgm [mask.pgm] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Generates a binary image which contains the graphical representation\n"    \
"of the histogram of  in.pgm (masked by the binary image\n"    \
" mask.pgm, if given).\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n"    \
"\n"    \
"Category: histo\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__histosieve__c__ \
"Usage: histosieve in.pgm val out.pgm\n"    \
"\n"    \
"Description:\n"    \
"\n"    \
"The points which value appear strictly less than  val times in the image are eliminated.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d\n"    \
"\n"    \
"Category: histo\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__histscal__c__ \
"WRITE ME!!!"



#define doc__histstretch__c__ \
"Usage: histstretch in.pgm vmin vmax p out.pgm\n"    \
"\n"    \
"Description:\n"    \
"\n"    \
"Eliminates the levels of the histogram strictly less than vmin or \n"    \
"strictly greater than vmax, and also the extremal levels\n"    \
"that have strictly less than N*(p/100) pixels \n"    \
"(N being the total number of pixels). If p = 0, only the \n"    \
"extremal levels with 0 pixels are eliminated.\n"    \
"A linear histogram scaling is then performed.\n"    \
"\n"    \
"Types supported: byte 2D\n"    \
"\n"    \
"Category: histo\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__hitormiss__c__ \
"Usage: hitormiss in.pgm e1.pgm e2.pgm out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Let X be the input set, E1, E2 be the two structuring elements, the result Y is given by:\n"    \
"Y = (X - E1) inter (Xb - E2)\n"    \
"where '-' denotes the erosion operator, and 'Xb' denotes the complementary of X.  \n"    \
"\n"    \
"Types supported: byte 2d, byte 3d, int32_t 2d, long3d\n"    \
"\n"    \
"Category: morpho\n"    \
"\n"    \
"\n"    \
" Michel Couprie 2003"



#define doc__hls2rgb__c__ \
"Usage: hls2rgb H.pgm L.pgm S.pgm out.ppm\n"    \
"\n"    \
"Description:\n"    \
"\n"    \
"Constructs 1 ppm from 3 pgm files:\n"    \
"   H.pgm : Hue (int32_t - 0 to 359)\n"    \
"   L.pgm : Luminance (byte)\n"    \
"   S.pgm : Saturation (byte)\n"    \
"Some particular values of HUE:\n"    \
"     0  red\n"    \
"    60  yellow\n"    \
"   120  green\n"    \
"   180  cyan\n"    \
"   240  blue\n"    \
"   300  magenta\n"    \
"Based on Foley, Van Damm & al: \"Computer Graphics\", 2nd ed., p. 595\n"    \
"\n"    \
"Types supported: int32_t 2D (hue), byte 2D (others)\n"    \
"\n"    \
"Types supported: byte 2D\n"    \
"\n"    \
"Category: convert\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__hma__c__ \
"WRITE ME!!!"



#define doc__holeclosing__c__ \
"Usage: holeclosing in connex holesize out\n"    \
"\n"    \
"Description: \n"    \
"Hole closing in 3d binary images. \n"    \
"The parameter  connex gives the connectivity used for the object;\n"    \
"possible choices are 6 and 26.\n"    \
"Holes which have a \"size\" greater than  holesize are let open \n"    \
"(where -1 is used as a symbol for infinity).\n"    \
"\n"    \
"Let X be the set of points of the binary image  in, \n"    \
"let Y be a full enclosing box. \n"    \
"The algorithm is the following:\n"    \
"\n"    \
" @verbatim \n"    \
"Repeat until stability:\n"    \
"    Select a point p of Y  X such that Tb(p,Y) = 1\n"    \
"        or such that Tb(p,Y) = 2 and d(p,X) > holesize\n"    \
"        which is at the greatest distance from X\n"    \
"    Y := Y  {p}\n"    \
"Result: Y\n"    \
" \n"    \
"Z. Aktouf, G. Bertrand, L.Perroton: \n"    \
"\"A three-dimensional holes closing algorithm\",\n"    \
"Pattern Recognition Letters, No.23, pp.523-531, 2002.\n"    \
"\n"    \
"Types supported: byte 3d\n"    \
"\n"    \
"Category: topobin\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__houghcercles__c__ \
"WRITE ME!!!"



#define doc__hthick__c__ \
"Usage: hthick in.pgm  connex niter out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Grayscale homotopic thickening (refs. [BEC97, CBB01]).\n"    \
"The parameter  connex gives the connectivity used for the minima;\n"    \
"possible choices are 4, 8 in 2D and 6, 26 in 3D.\n"    \
"Let F be the function corresponding to the input image  in.pgm.\n"    \
"Let G be the function corresponding to the input image \n"    \
" imcond.pgm, or the blanck function (constant 255) if the keyword  null is used.\n"    \
"The algorithm is the following:\n"    \
"\n"    \
" @verbatim \n"    \
"Repeat niter times:\n"    \
"    L = {(p,d) such that p is constructible for F and d = delta+(p,F)}\n"    \
"    While L not empty\n"    \
"       extract a couple (p,d) from L\n"    \
"       F(p) = min{ d, delta+(p,F), G(p) }    \n"    \
"Result: F\n"    \
"  niter is set to -1, then the operator iterates until stability.\n"    \
"\n"    \
"References: \n"    \
"[BEC97] G. Bertrand, J. C. Everat and M. Couprie: \"Image segmentation through operators based upon topology\",  Journal of Electronic Imaging, Vol.&nbsp;6, No.&nbsp;4, pp.&nbsp;395-405, 1997.\n"    \
"[CBB01] M. Couprie, F.N. Bezerra, Gilles Bertrand: \"Topological operators for\n"    \
"grayscale image processing\",  Journal of Electronic Imaging, Vol.&nbsp;10, No.&nbsp;4, pp.&nbsp;1003-1015, 2001.\n"    \
"\n"    \
"Types supported: byte 2D, byte 3D.\n"    \
"\n"    \
"Category: topogray\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__hthick3d__c__ \
"WRITE ME!!!"



#define doc__hthickbin__c__ \
"WRITE ME!!!"



#define doc__hthin__c__ \
"Usage: hthin in.pgm  connex niter out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Grayscale homotopic thinning (refs. [BEC97, CBB01]).\n"    \
"The parameter  connex gives the connectivity used for the minima;\n"    \
"possible choices are 4, 8 in 2D and 6, 26 in 3D.\n"    \
"Let F be the function corresponding to the input image  in.pgm.\n"    \
"Let G be the function corresponding to the input image \n"    \
" imcond.pgm, or the null function if the keyword  null is used.\n"    \
"The algorithm is the following:\n"    \
"\n"    \
" @verbatim \n"    \
"Repeat niter times:\n"    \
"    L = {(p,d) such that p is destructible for F and d = delta-(p,F)}\n"    \
"    While L not empty\n"    \
"       extract a couple (p,d) from L\n"    \
"       F(p) = max{ d, delta-(p,F), G(p) }    \n"    \
"Result: F\n"    \
"  niter is set to -1, then the operator iterates until stability.\n"    \
"\n"    \
"References:\n"    \
"\n"    \
"[BEC97] G. Bertrand, J. C. Everat and M. Couprie: \"Image segmentation through operators based upon topology\",  Journal of Electronic Imaging, Vol.&nbsp;6, No.&nbsp;4, pp.&nbsp;395-405, 1997.\n"    \
"\n"    \
"[CBB01] M. Couprie, F.N. Bezerra, Gilles Bertrand: \"Topological operators for\n"    \
"grayscale image processing\",  Journal of Electronic Imaging, Vol.&nbsp;10, No.&nbsp;4, pp.&nbsp;1003-1015, 2001.\n"    \
"\n"    \
"Types supported: byte 2D, byte 3D.\n"    \
"\n"    \
"Category: topogray\n"    \
"\n"    \
"\n"    \
" Michel Couprie\n"    \
"\n"    \
"Example:\n"    \
"\n"    \
"hthin ur1 null 4 10 ur1_hthin\n"    \
"minima ur1_hthin 4 ur1_hthin_m\n"    \
"\n"    \
"\n"    \
"  \n"    \
"      \n"    \
"      \n"    \
"  \n"    \
"  ur1\n"    \
"      ur1_hthin\n"    \
"      ur1_hthin_m\n"    \
"  \n"    \
""



#define doc__hthinbin__c__ \
"WRITE ME!!!"



#define doc__hthiniso__c__ \
"WRITE ME!!!"



#define doc__htkern__c__ \
"Usage: htkern in.pgm  connex out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Grayscale ultimate homotopic thinning (refs. [BEC97, CBB01]).\n"    \
"The parameter  connex gives the connectivity used for the minima;\n"    \
"possible choices are 4, 8 in 2D and 6, 26 in 3D.\n"    \
"Let F be the function corresponding to the input image  in.pgm.\n"    \
"Let G be the function corresponding to the input image \n"    \
" imcond.pgm, or the null function if the keyword  null is used.\n"    \
"The algorithm is the following:\n"    \
"\n"    \
" @verbatim \n"    \
"Repeat until stability:\n"    \
"    select p destructible for F such that F(p) is minimal\n"    \
"    F(p) = max{ delta-(p,F), G(p) }    \n"    \
"Result: F\n"    \
" \n"    \
"[BEC97] G. Bertrand, J. C. Everat and M. Couprie: \"Image segmentation through operators based upon topology\",  Journal of Electronic Imaging, Vol.&nbsp;6, No.&nbsp;4, pp.&nbsp;395-405, 1997.\n"    \
"[CBB01] M. Couprie, F.N. Bezerra, Gilles Bertrand: \"Topological operators for\n"    \
"grayscale image processing\",  Journal of Electronic Imaging, Vol.&nbsp;10, No.&nbsp;4, pp.&nbsp;1003-1015, 2001.\n"    \
"\n"    \
"Types supported: byte 2D, byte 3D.\n"    \
"\n"    \
"Category: topogray\n"    \
"\n"    \
"\n"    \
" Michel Couprie\n"    \
"\n"    \
"Example:\n"    \
"\n"    \
"htkern ur1 null 4 ur1_htkern\n"    \
"minima ur1_htkern 4 ur1_htkern_m\n"    \
"\n"    \
"\n"    \
"  \n"    \
"      \n"    \
"      \n"    \
"  \n"    \
"  ur1\n"    \
"      ur1_htkern\n"    \
"      ur1_htkern_m\n"    \
"  \n"    \
""



#define doc__htkern3dbin__c__ \
"WRITE ME!!!"



#define doc__htkernu__c__ \
"Usage: htkernu in.pgm  connex out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Grayscale ultimate homotopic thickening (refs. [BEC97, CBB01]).\n"    \
"The parameter  connex gives the connectivity used for the minima;\n"    \
"possible choices are 4, 8 in 2D and 6, 26 in 3D.\n"    \
"Let F be the function corresponding to the input image  in.pgm.\n"    \
"Let G be the function corresponding to the input image \n"    \
" imcond.pgm, or the null function if the keyword  null is used.\n"    \
"The algorithm is the following:\n"    \
"\n"    \
" @verbatim \n"    \
"Repeat until stability:\n"    \
"    select p constructible for F such that F(p) is maximal\n"    \
"    F(p) = min{ delta+(p,F), G(p) }    \n"    \
"Result: F\n"    \
" \n"    \
"[BEC97] G. Bertrand, J. C. Everat and M. Couprie: \"Image segmentation through operators based upon topology\",  Journal of Electronic Imaging, Vol.&nbsp;6, No.&nbsp;4, pp.&nbsp;395-405, 1997.\n"    \
"[CBB01] M. Couprie, F.N. Bezerra, Gilles Bertrand: \"Topological operators for\n"    \
"grayscale image processing\",  Journal of Electronic Imaging, Vol.&nbsp;10, No.&nbsp;4, pp.&nbsp;1003-1015, 2001.\n"    \
"\n"    \
"Types supported: byte 2D, byte 3D.\n"    \
"\n"    \
"Category: topogray\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__identifyline__c__ \
"Usage: identifyline in.list out.list\n"    \
"\n"    \
"Description:\n"    \
"Identifies the parameters (a,b) of the equation of the 2D line:\n"    \
"ax+b=y that minimizes the least square error between this line \n"    \
"and the given points. Method: basic linear regression.\n"    \
"\n"    \
"Types supported: list 1D, list 2D\n"    \
"\n"    \
"Category: geo\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__identifyparabola2__c__ \
"Usage: identifyparabola2 in.list out.list\n"    \
"\n"    \
"Description:\n"    \
"Identifies the parameters (a,b) of the equation of the 2D parabola:\n"    \
"ax^2+b=y that minimizes the least square error between this parabola \n"    \
"and the given points. Method: basic linear regression.\n"    \
"\n"    \
"Types supported: list 1D, list 2D\n"    \
"\n"    \
"Category: geo\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__identifyparabola3__c__ \
"Usage: identifyparabola3 in.list out.list\n"    \
"\n"    \
"Description:\n"    \
"Identifies the parameters (a,b,c) of the equation of the 2D parabola:\n"    \
"ax^2+bx+c=y that minimizes the least square error between this parabola \n"    \
"and the given points. Method: basic linear regression.\n"    \
"\n"    \
"Types supported: list 1D, list 2D\n"    \
"\n"    \
"Category: geo\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__identifyplane__c__ \
"Usage: identifyplane in.list out.list\n"    \
"\n"    \
"Description:\n"    \
"Identifies the parameters (a, b, c, d) of the equation of the 3D plane:\n"    \
"ax+by+cz+d=0 that minimizes the least square error between this plane \n"    \
"and the given points. Method: basic linear regression.\n"    \
"\n"    \
"Types supported: list 3D\n"    \
"\n"    \
"Category: geo\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__inf__c__ \
"Usage: inf in1.pgm in2.pgm out.pgm\n"    \
"\n"    \
"Description:\n"    \
"For each pixel x, if in1[x] Types supported: byte 2D, byte 3D\n"    \
"\n"    \
"Category: arith\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__insert__c__ \
"Usage: insert a.pgm b.pgm x y z out.ppm\n"    \
"\n"    \
"Description: Inserts image  a.pgm into image  b.pgm at position  x,y,z\n"    \
"\n"    \
"Types supported: byte 2d, int32_t 2d, float 2d, byte 3d, int32_t 3d, float 3d.\n"    \
"\n"    \
"Category: convert, geo\n"    \
", \n"    \
"\n"    \
" Michel Couprie"



#define doc__integermedialaxis__c__ \
"Usage: integermedialaxis in.pgm [gamma] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Integer medial axis, as defined in [HR05],\n"    \
"of the binary image  X contained in  in.pgm.\n"    \
"\n"    \
"References:\n"    \
"[HR05] \"Euclidean Skeletons of 3D Data Sets in Linear Time\n"    \
"by the Integer Medial Axis Transform\",\n"    \
"W.H. Hesselink and B.T.M. Roerdink,\n"    \
"Computational Imaging and Vision, Vol. 30,\n"    \
"Mathematical Morphology: 40 Years On, Springer, 2005, pp. 259-268\n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: morpho\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__interpolate__c__ \
"WRITE ME!!!"



#define doc__inverse__c__ \
"Usage: inverse in.pgm out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Byte images: for each pixel x, out[x] = 255 - in[x].\n"    \
"Long or float images: for each pixel x, out[x] = VMAX - in[x], where VMAX = max{in[x]}.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d, long 2d, long 3d, float 2d, float 3d\n"    \
"\n"    \
"Category: arith\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__isnull__c__ \
"Usage: isnull in.pgm\n"    \
"\n"    \
"Description:\n"    \
"This function returns 1 (no error) if for each pixel x, in[x] == 0; \n"    \
"otherwise it returns 0.\n"    \
"Images must be of the same dimensions.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n"    \
"\n"    \
"Category: arith\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__isometry__c__ \
"Usage: isometry in MODE out\n"    \
"\n"    \
"Description:\n"    \
"Basic isometries in 2D and 3D.\n"    \
"Available modes: xzy, yxz, yzx, zxy, zyx, (3D), and\n"    \
"0: identity,\n"    \
"1: rotation 90 degrees clockwise,\n"    \
"2: rotation 180 degrees clockwise,\n"    \
"3: rotation 270 degrees clockwise,\n"    \
"4: symmetry / vertical axis,\n"    \
"5: symmetry / horizontal axis.\n"    \
"\n"    \
"Types supported: byte 2D, byte 3D\n"    \
"\n"    \
"Category: geo\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__jones__c__ \
"WRITE ME!!!"



#define doc__kern__c__ \
"WRITE ME!!!"



#define doc__label__c__ \
"Usage: label in.pgm connex  out.pgm\n"    \
"\n"    \
"Description:\n"    \
"The argument  connex selects the connectivity (4, 8 in 2D; 6, 18, 26 in 3D).\n"    \
"The argument  selects the kind of flat zone\n"    \
"to be labeled:\n"    \
"  fgd selects foreground components for a binary image\n"    \
"  bgd selects background components for a binary image\n"    \
"  min selects regional minima\n"    \
"  max selects regional maxima\n"    \
"  pla selects all flat zones (plateaux).\n"    \
"The output image  out.pgm has the type \"int32_t\".\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d\n"    \
"\n"    \
"Category: connect\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__labeldil__c__ \
"Usage: labeldil in.pgm se.pgm out.pgm\n"    \
"\n"    \
"Description:\n"    \
"The argument  se.pgm is a structuring element which must be reflexive and symmetric. \n"    \
"Connected components are defined by this structuring element.\n"    \
"The output image  out.pgm has the type \"int32_t\".\n"    \
"\n"    \
" No test is done to check properties of the structuring element.\n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: connect\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__labeltree__c__ \
"Usage: labeltree in.pgm connex out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Level of the branch section defined by his level in the tree. Returns the level\n"    \
"max of the branches.\n"    \
"\n"    \
"The distance d used depends on the parameter  connex :\n"    \
" 4: 4-distance in 2d\n"    \
" 8: 8-distance in 2d\n"    \
" 6: 6-distance in 3d\n"    \
" 18: 18-distance in 3d\n"    \
" 26: 26-distance in 3d\n"    \
"\n"    \
"Types supported: byte 2D, byte 3D\n"    \
"\n"    \
"Category: topo\n"    \
"\n"    \
"\n"    \
" CÈdric AllËne"



#define doc__lambdalthin__c__ \
"Usage: lambdalthin in.pgm  connex niter h out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Filtered leveling for 2D grayscale images. \n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: topogray\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__lambdamedialaxis__c__ \
"Usage: lambdamedialaxis in.pgm [lambda] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Discrete lambda-medial axis, as defined in [CCT09], \n"    \
"of the binary image  X contained in  in.pgm.\n"    \
"If the parameter  lambda is given, the output is a binary image. Otherwise, \n"    \
"it is a float image representing the function x->lambda(x).\n"    \
"\n"    \
"References: \n"    \
"[CCT09] John Chaussard, Michel Couprie, and Hugues Talbot. A discrete lambda-medial axis. 15th Discrete Geometry for Computer Imagery (DGCI'09). Lecture Notes in Computer Science.   2009.  pp. 1‚Äì12. To appear.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: morpho\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__lambdapmedialaxis__c__ \
"Usage: lambdapmedialaxis in.pgm [lambda] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Discrete lambda-medial axis of the binary image  X contained in  in.pgm. \n"    \
"If the parameter  lambda is given, the output is a binary image. Otherwise, \n"    \
"it is a float image representing the function x->lambda(x).\n"    \
"\n"    \
" Experimental.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: morpho\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__lambdaskel__c__ \
"Usage: lambdaskel in.pgm  connex lambda out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Filtered topological skeleton for 2D grayscale images. \n"    \
"The parameter  connex gives the connectivity used for the minima;\n"    \
"possible choices are 4 and 8.\n"    \
"The parameter  lambda is a contrast parameter (positive integer).\n"    \
"Let F be the function corresponding to the input image  in.pgm.\n"    \
"The parameter  imcond.pgm is a constraint function G.\n"    \
"The algorithm is the following:\n"    \
"\n"    \
" @verbatim \n"    \
"Repeat until stability:\n"    \
"    Select a point p which is lambda-destructible for F or a peak\n"    \
"        such that F(p) > G(p) and such that F(p) is minimal\n"    \
"    F(p) = alpha-(p,F)\n"    \
"Result: F\n"    \
" \n"    \
"M. Couprie, F.N. Bezerra, Gilles Bertrand: \"Topological operators for\n"    \
"grayscale image processing\",  Journal of Electronic Imaging, Vol.&nbsp;10, No.&nbsp;4, pp.&nbsp;1003-1015, 2001.\n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: topogray\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__lambdathin__c__ \
"Usage: lambdathin in.pgm  connex niter h out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Homotopic thinning for 2D grayscale images. \n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: topogray\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__lantuejoul__c__ \
"Usage: lantuejoul in.pgm [dist] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Let X be the set in  in.pgm .\n"    \
"The result is union{Si(X), i in N} where\n"    \
"Si(X) = erosball(X,i)  openball(erosball(X,i),1). \n"    \
"Structuring elements are balls defined after a distance.\n"    \
"The distance used depends on the optional parameter  dist (default is 0) :\n"    \
" 0: approximate euclidean distance (truncated)\n"    \
" 1: approximate quadratic euclidean distance\n"    \
" 2: chamfer distance\n"    \
" 3: exact quadratic euclidean distance\n"    \
" 4: 4-distance in 2d\n"    \
" 8: 8-distance in 2d\n"    \
" 6: 6-distance in 3d\n"    \
" 18: 18-distance in 3d\n"    \
" 26: 26-distance in 3d\n"    \
"\n"    \
" The input image  in.pgm must be a binary image. No test is done.\n"    \
"\n"    \
"Types supported: byte 2D, byte 3D\n"    \
"\n"    \
"Category: morpho\n"    \
"\n"    \
"\n"    \
" Michel Couprie ao√ªt 2009"



#define doc__laplacian__c__ \
"Usage: laplacian in.pgm alpha out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Deriche's recursive implementation of the Laplacian\n"    \
"operator. The parameter \n"    \
" alpha (double) controls the spatial extension of the\n"    \
"filter: 0 Types supported: byte 2D\n"    \
"\n"    \
"Category: signal\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__lengthspline__c__ \
"Usage: lengthspline spline.txt [out.list]\n"    \
"\n"    \
"Description:\n"    \
"Computes the length of a spline which is specified by its control points in a text file.\n"    \
"The file format for  spline.txt is the following for 2D:\n"    \
"\n"    \
"c n+1 (where n+1 denotes the number of control points)\n"    \
"x1 y1\n"    \
"...\n"    \
"xn+1 yn+1\n"    \
"C0X1 C0Y1 C1X1 C1Y1 C2X1 C2Y1 C3X1 C3Y1\n"    \
"...\n"    \
"C0Xn C0Yn C1Xn C1Yn C2Xn C2Yn C3Xn C3Yn\n"    \
"\n"    \
"and in the 3D case:\n"    \
"\n"    \
"C n+1 (where n+1 denotes the number of control points)\n"    \
"x1 y1 z1\n"    \
"...\n"    \
"xn+1 yn+1 zn+1\n"    \
"C0X1 C0Y1 C0Z1 C1X1 C1Y1 C1Z1 C2X1 C2Y1 C2Z1 C3X1 C3Y1 C3Z1\n"    \
"...\n"    \
"C0Xn C0Yn C0Zn C1Xn C1Yn C1Zn C2Xn C2Yn C2Zn C3Xn C3Yn C3Zn\n"    \
"\n"    \
"Types supported: spline 2D, spline 3D\n"    \
"\n"    \
"Category: geo\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__lengthsplines__c__ \
"Usage: lengthsplines splines.txt out.list\n"    \
"\n"    \
"Description:\n"    \
"Computes the lengths of splines which are specified by their control points in a text file.\n"    \
"\n"    \
"The file format for  splines.txt is the following for 2D:\n"    \
"\n"    \
"The file  splines.txt contains a list of splines under the format:\n"    \
"d nb_splines\n"    \
"nb_points_spline_1  x11 y11  x12 y12 ...\n"    \
"nb_points_spline_2  x21 y21  x22 y22 ...\n"    \
"nb_points_spline_3  x31 y31  x32 y32 ...\n"    \
"...\n"    \
"or, in 3D:\n"    \
"D nb_splines\n"    \
"nb_points_spline_1  x11 y11 z11  x12 y12 z12 ...\n"    \
"nb_points_spline_2  x21 y21 z21  x22 y22 z22 ...\n"    \
"nb_points_spline_3  x31 y31 z31  x32 y32 z32 ...\n"    \
"...\n"    \
"\n"    \
"Types supported: spline 2D, spline 3D\n"    \
"\n"    \
"Category: geo\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__lenoir__c__ \
"WRITE ME!!!"



#define doc__levialdi__c__ \
"WRITE ME!!!"



#define doc__linapprox__c__ \
"WRITE ME!!!"



#define doc__line__c__ \
"Usage: line in.pgm out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Draws a straight line between the two first points found in image in.pgm.\n"    \
"Uses the Bresenham's algorithm.\n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: draw\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__lintophat__c__ \
"Usage: lintophat in.pgm length out.pgm\n"    \
"\n"    \
"Description: \n"    \
"Max of morphological black top hats by linear structuring elements, \n"    \
"in all possible discrete directions.\n"    \
"Let F be the original image and E be a structuring element, the black\n"    \
"top hat of F by E is defined by F - closing(F, E).\n"    \
"The closing deletes dark structures that do not match the \n"    \
"structuring element, thus the black top hat detects those dark structures.\n"    \
"For a linear structuring element, the detected structures are those which\n"    \
"are orthogonal to the se.\n"    \
"The length if the linear structuring elements is given by  length.\n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: morpho\n"    \
"\n"    \
"\n"    \
" Hildegard Koehler 2003"



#define doc__list2pgm__c__ \
"Usage: list2pgm in.list  [scale] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"\n"    \
"Reads the file in.list. This file must have one of the following formats:\n"    \
"  \n"    \
"  e &lt;n&gt;       s &lt;n&gt;         b &lt;n&gt;         n &lt;n&gt;            B &lt;n&gt;            N &lt;n&gt;    \n"    \
"  x1          x1 v1         x1 y1         x1 y1 v1         x1 y1 z1         x1 y1 z1 v1\n"    \
"  x2    or    x2 v2   or    x2 y2   or    x2 y2 v2   or    x2 y2 z2   or    z2 x2 y2 v2\n"    \
"  ...         ...           ...           ...              ...              ...\n"    \
"  xn          xn vn         xn yn         xn yn vn         xn yn zn         zn xn yn vn\n"    \
"\n"    \
"The formats e, s, b, n, B, and N, correspond respectively\n"    \
"to binary 1D, graylevel 1D, binary 2D, graylevel 2D, binary 3D and graylevel 3D images.\n"    \
"If a file name in.pgm is given, then the points read in in.list are \n"    \
"inserted (if possible) into the image read in in.pgm. Else, they are inserted in\n"    \
"a new image, the dimensions of which are rs, cs, and d.\n"    \
"\n"    \
"The optional parameter  scale allows to scale the coordinates.\n"    \
"\n"    \
"Types supported: list 1D, 2D, 3D\n"    \
"\n"    \
"Category: convert\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__localextrema__c__ \
"Usage: in.pgm connex minimum out.pgm\n"    \
"\n"    \
"Description: \n"    \
"Selects the local maxima or minima of a grayscale image with connexity connex.\n"    \
"\n"    \
"Types supported: byte 2d - byte 3d\n"    \
"\n"    \
"Category: connect, topogray\n"    \
", \n"    \
"\n"    \
" Laurent Najman"



#define doc__log__c__ \
"Usage: log in.pgm out.pgm\n"    \
"\n"    \
"Description:\n"    \
"For each pixel x, out[x] = log(in[x]).\n"    \
"\n"    \
"Types supported: float 2d, float 3d\n"    \
"\n"    \
"Category: arith\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__long2byte__c__ \
"Usage: long2byte in.pgm [mode] [n] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"\n"    \
"Depending on the value given for the (optional) parameter mode:\n"    \
"   mode = 0 (default) : for all x, out[x] = min(255, in[x]).\n"    \
"   mode = 1 : for all x, out[x] = in[x] modulo 256.\n"    \
"   mode = 2 : scales values in the range 0-255.\n"    \
"   mode = 3 : sorts the values by decreasing number of occurence in the image.\n"    \
"Replaces the values by their order.\n"    \
"Only the n (default 255) first values are kept.\n"    \
"Useful for label images.\n"    \
"   mode = 4 : truncation of the square root in the range 0-255.\n"    \
"   mode = 5 : truncation of the log in the range 0-255.\n"    \
"\n"    \
"Types supported: int32_t 2d, int32_t 3d\n"    \
"\n"    \
"Category: convert\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__long2float__c__ \
"Usage: long2float in out\n"    \
"\n"    \
"Description: For each pixel x, out[x] = (float)in[x]\n"    \
"\n"    \
"Types supported: long 2d, long 3d.\n"    \
"\n"    \
"Category: convert\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__longestplateau__c__ \
"Usage: longestplateau in.list [tolerance] out.list\n"    \
"\n"    \
"Description:\n"    \
"\n"    \
"Reads the sequence S from the file in.list. \n"    \
"This file must have the following format:\n"    \
"  \n"    \
"  e &lt;n&gt;\n"    \
"  x1\n"    \
"  x2\n"    \
"  ...\n"    \
"  xn\n"    \
"\n"    \
"The tolerance t (float) is given by the parameter tolerance\n"    \
"(default value is 0).\n"    \
"A plateau is a subsequence P of S formed by consecutive elements of P, \n"    \
"between indices i and j, and such that max{P[k];iTypes supported: list 1D\n"    \
"\n"    \
"Category: signal\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__lpetopo__c__ \
"WRITE ME!!!"



#define doc__lpetoporeg__c__ \
"WRITE ME!!!"



#define doc__lthick__c__ \
"Usage: lthick in.pgm  connex niter out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Grayscale leveling thickening (refs. [BEC97, CBB01]).\n"    \
"The parameter  connex gives the connectivity used for the minima;\n"    \
"possible choices are 4, 8 in 2D and 6, 26 in 3D.\n"    \
"Let F be the function corresponding to the input image  in.pgm.\n"    \
"Let G be the function corresponding to the input image \n"    \
" imcond.pgm, or the blanck function (constant 255) if the keyword  null is used.\n"    \
"The algorithm is the following:\n"    \
"\n"    \
" @verbatim \n"    \
"Repeat niter times:\n"    \
"    L = {(p,a) such that T++(p,F) = 1 and a = alpha+(p,F)}\n"    \
"    While L not empty\n"    \
"       extract a couple (p,a) from L\n"    \
"       F(p) = min{ a, alpha+(p,F), G(p) }    \n"    \
"Result: F\n"    \
"  niter is set to -1, then the operator iterates until stability.\n"    \
"\n"    \
"References: \n"    \
"[BEC97] G. Bertrand, J. C. Everat and M. Couprie: \"Image segmentation through operators based upon topology\",  Journal of Electronic Imaging, Vol.&nbsp;6, No.&nbsp;4, pp.&nbsp;395-405, 1997.\n"    \
"[CBB01] M. Couprie, F.N. Bezerra, Gilles Bertrand: \"Topological operators for\n"    \
"grayscale image processing\",  Journal of Electronic Imaging, Vol.&nbsp;10, No.&nbsp;4, pp.&nbsp;1003-1015, 2001.\n"    \
"\n"    \
"Types supported: byte 2D, byte 3D.\n"    \
"\n"    \
"Category: topogray\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__lthin__c__ \
"Usage: lthin in.pgm  connex niter out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Grayscale leveling thinning (refs. [BEC97, CBB01]).\n"    \
"The parameter  connex gives the connectivity used for the minima;\n"    \
"possible choices are 4, 8 in 2D and 6, 26 in 3D.\n"    \
"Let F be the function corresponding to the input image  in.pgm.\n"    \
"Let G be the function corresponding to the input image \n"    \
" imcond.pgm, or the null function if the keyword  null is used.\n"    \
"The algorithm is the following:\n"    \
"\n"    \
" @verbatim \n"    \
"Repeat niter times:\n"    \
"    L = {(p,a) such that T--(p,F) = 1 and a = alpha-(p,F)}\n"    \
"    While L not empty\n"    \
"       extract a couple (p,a) from L\n"    \
"       F(p) = max{ a, alpha-(p,F), G(p) }    \n"    \
"Result: F\n"    \
"  niter is set to -1, then the operator iterates until stability.\n"    \
"\n"    \
"References: \n"    \
"[BEC97] G. Bertrand, J. C. Everat and M. Couprie: \"Image segmentation through operators based upon topology\",  Journal of Electronic Imaging, Vol.&nbsp;6, No.&nbsp;4, pp.&nbsp;395-405, 1997.\n"    \
"[CBB01] M. Couprie, F.N. Bezerra, Gilles Bertrand: \"Topological operators for\n"    \
"grayscale image processing\",  Journal of Electronic Imaging, Vol.&nbsp;10, No.&nbsp;4, pp.&nbsp;1003-1015, 2001.\n"    \
"\n"    \
"Types supported: byte 2D, byte 3D.\n"    \
"\n"    \
"Category: topogray\n"    \
"\n"    \
"\n"    \
" Michel Couprie\n"    \
"\n"    \
"Example:\n"    \
"\n"    \
"lthin ur1 null 4 -1 ur1_lthin\n"    \
"minima ur1_lthin 4 ur1_lthin_m\n"    \
"\n"    \
"\n"    \
"  \n"    \
"      \n"    \
"      \n"    \
"  \n"    \
"  ur1\n"    \
"      ur1_lthin\n"    \
"      ur1_lthin_m\n"    \
"  \n"    \
""



#define doc__lvkern__c__ \
"Usage: lvkern in.pgm  connex out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Grayscale ultimate leveling thinning (refs. [BEC97, CBB01]).\n"    \
"The parameter  connex gives the connectivity used for the minima;\n"    \
"possible choices are 4, 8 in 2D and 6, 26 in 3D.\n"    \
"Let F be the function corresponding to the input image  in.pgm.\n"    \
"Let G be the function corresponding to the input image \n"    \
" imcond.pgm, or the null function if the keyword  null is used.\n"    \
"The algorithm is the following:\n"    \
"\n"    \
" @verbatim \n"    \
"Repeat until stability:\n"    \
"    select p such that T--(p,F) = 1 and such that F(p) is minimal\n"    \
"    F(p) = max{ delta-(p,F), G(p) }    \n"    \
"Result: F\n"    \
" \n"    \
"[BEC97] G. Bertrand, J. C. Everat and M. Couprie: \"Image segmentation through operators based upon topology\",  Journal of Electronic Imaging, Vol.&nbsp;6, No.&nbsp;4, pp.&nbsp;395-405, 1997.\n"    \
"[CBB01] M. Couprie, F.N. Bezerra, Gilles Bertrand: \"Topological operators for\n"    \
"grayscale image processing\",  Journal of Electronic Imaging, Vol.&nbsp;10, No.&nbsp;4, pp.&nbsp;1003-1015, 2001.\n"    \
"\n"    \
"Types supported: byte 2D, byte 3D.\n"    \
"\n"    \
"Category: topogray\n"    \
"\n"    \
"\n"    \
" Michel Couprie\n"    \
"\n"    \
"Example:\n"    \
"\n"    \
"lvkern ur1 null 4 ur1_lvkern\n"    \
"minima ur1_lvkern 4 ur1_lvkern_m\n"    \
"\n"    \
"\n"    \
"  \n"    \
"      \n"    \
"      \n"    \
"  \n"    \
"  ur1\n"    \
"      ur1_lvkern\n"    \
"      ur1_lvkern_m\n"    \
"  \n"    \
""



#define doc__lvkernu__c__ \
"Usage: lvkernu in.pgm  connex out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Grayscale ultimate leveling thickening (refs. [BEC97, CBB01]).\n"    \
"The parameter  connex gives the connectivity used for the minima;\n"    \
"possible choices are 4, 8 in 2D and 6, 26 in 3D.\n"    \
"Let F be the function corresponding to the input image  in.pgm.\n"    \
"Let G be the function corresponding to the input image \n"    \
" imcond.pgm, or the blanck function (constant 255) if the keyword  null is used.\n"    \
"The algorithm is the following:\n"    \
"\n"    \
" @verbatim \n"    \
"Repeat until stability:\n"    \
"    select p such that T++(p,F) = 1 and such that F(p) is maximal\n"    \
"    F(p) = min{ delta+(p,F), G(p) }    \n"    \
"Result: F\n"    \
" \n"    \
"[BEC97] G. Bertrand, J. C. Everat and M. Couprie: \"Image segmentation through operators based upon topology\",  Journal of Electronic Imaging, Vol.&nbsp;6, No.&nbsp;4, pp.&nbsp;395-405, 1997.\n"    \
"[CBB01] M. Couprie, F.N. Bezerra, Gilles Bertrand: \"Topological operators for\n"    \
"grayscale image processing\",  Journal of Electronic Imaging, Vol.&nbsp;10, No.&nbsp;4, pp.&nbsp;1003-1015, 2001.\n"    \
"\n"    \
"Types supported: byte 2D, byte 3D.\n"    \
"\n"    \
"Category: topogray\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__matchellipse__c__ \
"WRITE ME!!!"



#define doc__matchrect__c__ \
"Usage: matchrect in.pgm connex [rounded|axis] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Matches (rounded) rectangles to the contours of objects.\n"    \
"Objects are connected components according to the connectivity\n"    \
"given by  connex .\n"    \
"Option  rounded is used to match rounded rectangles.\n"    \
"Option  axis is used to output only the big axis of the matched rectangle.\n"    \
"\n"    \
"Types supported: byte 2D\n"    \
"\n"    \
"Category:  geo\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__max__c__ \
"Usage: max in1.pgm in2.pgm out.pgm\n"    \
"\n"    \
"Description:\n"    \
"For each pixel x, out[x] = max{in1[x], in2[x]}. \n"    \
"Images must be of the same type and same dimensions.\n"    \
"\n"    \
"Types supported: byte 2D, byte 3D\n"    \
"\n"    \
"Category: arith\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__max1__c__ \
"Usage: max1 in.pgm [out.list]\n"    \
"\n"    \
"Description:\n"    \
"This function returns (in the list out.list) \n"    \
"the maximum of the pixel values of the image  in.pgm .\n"    \
"\n"    \
"If the parameter  out.list is ommitted, the result is printed on the standard output.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n"    \
"\n"    \
"Category: arith\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__maxdiameter__c__ \
"Usage: maxdiameter in connex out\n"    \
"\n"    \
"Description:\n"    \
"\n"    \
"Find two points x, y in a set X (binary image in) such that \n"    \
"d(x,y) is less or equal to d(v,w) for any two points v, w in X.\n"    \
"The distance d is the Euclidean distance.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: geo\n"    \
"\n"    \
"\n"    \
" Gu Jun"



#define doc__maxflow__cpp__ \
"WRITE ME!!!"



#define doc__maxima__c__ \
"Usage: in.pgm connex out.pgm\n"    \
"\n"    \
"Description: \n"    \
"Selects the regional maxima of a grayscale image with connexity connex.\n"    \
"\n"    \
"Types supported: byte 2d, int32_t 2d, byte 3d, int32_t 3d\n"    \
"\n"    \
"Category: connect, topogray\n"    \
", \n"    \
"\n"    \
" Michel Couprie"



#define doc__mcm2ac__c__ \
"WRITE ME!!!"



#define doc__mcm2vtk__c__ \
"WRITE ME!!!"



#define doc__mcmbuild__c__ \
"WRITE ME!!!"



#define doc__mcube__c__ \
"Usage: mcube in.pgm threshold nregul obj_id format [connex] out\n"    \
"\n"    \
"Description:\n"    \
"Generates a 3d mesh from the binary or grayscale image  in.pgm .\n"    \
"\n"    \
"The original image is first thresholded (parameter  threshold, values 0 and 1\n"    \
"both fit for a binary image). Then the method described in [Lac96] is applied to\n"    \
"generate the 3d mesh. The resulting mesh is then smoothed by  nregul steps of \n"    \
"laplacian smoothing. The parameter  obj_id is used to tag the generated mesh.\n"    \
"\n"    \
"The parameter  format indicate the format of the output file \n"    \
"(choices are POV, POVB, COL, MCM, AC, GL, VTK, RAW).\n"    \
"The keyword POVB corresponds to a bare Povray mesh: \n"    \
"a header and a footer must be catenated in order to make a full Povray scene. \n"    \
"The keyword RAW is the exchange format for the \"mesh\" software: see http://mesh.berlios.de/\n"    \
"\n"    \
"The optional parameter  connex indicates the connexity used for the object.\n"    \
"Possible values are 6 and 26 (default).\n"    \
"\n"    \
"[Lac96] J.-O. Lachaud, \"Topologically defined iso-surfaces\", DGCI'96, LNCS 1176 (245--256), Springer Verlag, 1996.\n"    \
"\n"    \
"Types supported: byte 3d\n"    \
"\n"    \
"Category: mesh3d\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__mcube_tmp__c__ \
"WRITE ME!!!"



#define doc__meanfilter__c__ \
"Usage: meanfilter in.pgm roi.pgm niter [inhibit.pgm] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Let F be the function stored in image  in.pgm.\n"    \
"Let R be the function stored in image  roi.pgm (region of interest).\n"    \
"Let I be the function stored in image  inhibit.pgm (if any).\n"    \
"The algorithm is the following:\n"    \
"\n"    \
" @verbatim \n"    \
"Repeat niter times\n"    \
"  G = F\n"    \
"  For each image point x\n"    \
"    If R[x] and not I[x] then\n"    \
"      S = G[x]; C = 1;\n"    \
"      For each neighbour y of x\n"    \
"        If R[y] then S = S + G[y]; C = C + 1;\n"    \
"      F[x] = S / C;\n"    \
"Result: F\n"    \
" supported: byte 2d, int32_t 2d, float 2d, byte 3d, int32_t 3d, float 3d\n"    \
"\n"    \
"Category: signal\n"    \
"\n"    \
"\n"    \
" Marcin Janaszewski, Michel Couprie"



#define doc__measure__cpp__ \
"WRITE ME!!!"



#define doc__medialaxis__c__ \
"Usage: medialaxis in.pgm distance out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Medial axis of the binary image  X. If x is the center of a maximal ball\n"    \
"included in X, then the result R(x) is equal to the smallest distance between x \n"    \
"and a point outside X, otherwise it is equal to 0 .\n"    \
"The distance is indicated by the parameter distance :\n"    \
" 0: approximate euclidean distance\n"    \
" 1: exact quadratic euclidean distance (Saito-Toriwaki's skeleton)\n"    \
" 2: exact quadratic euclidean distance (Coeurjolly's reduced axis)\n"    \
" 3: exact quadratic euclidean distance (RÈmy-Thiel)\n"    \
" 4: 4-distance in 2d\n"    \
" 6: 6-distance in 3d\n"    \
" 8: 8-distance in 2d\n"    \
" 26: 26-distance in 3d\n"    \
"\n"    \
"The usual discrete distances are denoted by 4, 8 (2D) and 6, 26 (3D).\n"    \
"For the euclidean distance, set distance to 0.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: morpho\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__medianfilter__c__ \
"Usage: medianfilter in.pgm el.pgm out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Let F be the input image, G be the output image, and E the structuring\n"    \
"element. \n"    \
"then for each pixel p, G[p] is the median element of the sorted list (by \n"    \
"increasing order) of the pixel values in the set { F[q], q in E[p] }.\n"    \
"\n"    \
"The median filter is a particular case of rank filter (see rankfilter.c), \n"    \
"with rank = 0.5 .\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: morpho\n"    \
"\n"    \
"\n"    \
" Michel Couprie 1997"



#define doc__meshaddnoise__c__ \
"Usage: meshaddnoise in alpha out\n"    \
"\n"    \
"Description:\n"    \
"\n"    \
"Adds gaussian noise to the positions of the vertices of a mesh.\n"    \
"The mean of the gaussian is 0.\n"    \
"The standard deviation of the gaussian is given by alpha. \n"    \
"Available input formats: mcm, ifs, vtk, ac.\n"    \
"Available output formats: mcm, vtk.\n"    \
"\n"    \
"Types supported: mesh 3d\n"    \
"\n"    \
"Category: mesh3d\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__meshconvert__c__ \
"Usage: meshconvert in [resolution] out\n"    \
"\n"    \
"Description:\n"    \
"\n"    \
"Mesh format conversion. \n"    \
"Available input formats: mcm, ifs, vtk.\n"    \
"Available output formats: mcm, vtk, pgm (points only).\n"    \
"If the output format is pgm, then the optional argument resolution \n"    \
"gives the resolution of the grid (homogeneous in x, y and z dimensions). The \n"    \
"default value is 1.0.\n"    \
"\n"    \
"Types supported: mesh 3d\n"    \
"\n"    \
"Category: mesh3d, convert\n"    \
", \n"    \
"\n"    \
" Michel Couprie"



#define doc__meshflatten__c__ \
"Usage: meshflatten in out\n"    \
"\n"    \
"Description:\n"    \
"\n"    \
"Types supported: mesh 3d\n"    \
"\n"    \
"Category: mesh3d\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__meshregul__c__ \
"Usage: meshregul in mode [param1 [param2]] out\n"    \
"\n"    \
"Description:\n"    \
"\n"    \
"Mesh smoothing. \n"    \
"The possible choices for parameter mode are:\n"    \
" 0: Laplacian smoothing (param1 = number of iterations, default value 5)\n"    \
" 1: Hamam method [HC07] (SOWA, param1 = theta, default value 1.0)\n"    \
" 2: Vollmer, Mencl and Mueller method [VMM] (param1 = alpha, param2 = beta, default values 0.1 and 0.6 resp.)\n"    \
" 3: Hamam method [HC07], variant: matrix A used instead of AA (FOWA, param1 = theta, default value 1.0)\n"    \
" 4: Hamam method [HC07], variant: theta = infty (SOWA, param1 = number of iterations, default value 5)\n"    \
" 5: Hamam method [HC07], conjugate gradient algorithm (SOWA, param1 = theta, default value 1.0)\n"    \
" 6: Taubin method [Tau95] (param1 = lambda, param2 = mu, param3 = N, default values 0.33 and -0.34 and 60)\n"    \
" 7: Laplacian smoothing for a 2D polygon, with border edges preservation (param1 = number of iterations, default value 10)\n"    \
"\n"    \
"[HC07] Y. Hamam and M. Couprie, \"An optimisation-based approach to mesh smoothing: reformulation and extensions\", to appear, 2007.\n"    \
"\n"    \
"[VMM99] J. Vollmer and R. Mencl and H. Muller,\n"    \
"\"Improved Laplacian Smoothing of Noisy Surface Meshes\",\n"    \
"Computer Graphics Forum, Vol. 18, N. 3, pp. 131-138, 1999.\n"    \
"\n"    \
"[Tau95] G. Taubin, \n"    \
"\"Curve and surface smoothing without shrinkage\"\n"    \
"Proceedings of the Fifth International Conference on Computer Vision,\n"    \
"pp. 852-857, 1995.\n"    \
"\n"    \
"Types supported: mesh 3d\n"    \
"\n"    \
"Category: mesh3d\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__meshwarp__c__ \
"WRITE ME!!!"



#define doc__min__c__ \
"Usage: min in1.pgm in2.pgm out.pgm\n"    \
"\n"    \
"Description:\n"    \
"For each pixel x, out[x] = min{in1[x], in2[x]}. \n"    \
"Images must be of the same type and same dimensions.\n"    \
"\n"    \
"Types supported: byte 2D, byte 3D\n"    \
"\n"    \
"Category: arith\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__min1__c__ \
"Usage: min1 in.pgm [out.list]\n"    \
"\n"    \
"Description:\n"    \
"This function returns (in the list out.list) \n"    \
"the minimum of the pixel values of the image  in.pgm .\n"    \
"\n"    \
"If the parameter  out.list is ommitted, the result is printed on the standard output.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n"    \
"\n"    \
"Category: arith\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__minima__c__ \
"Usage: in.pgm connex out.pgm\n"    \
"\n"    \
"Description: \n"    \
"Selects the regional minima of a grayscale image with connexity connex.\n"    \
"\n"    \
"Types supported: byte 2d, int32_t 2d, byte 3d, int32_t 3d\n"    \
"\n"    \
"Category: connect, topogray\n"    \
", \n"    \
"\n"    \
" Michel Couprie"



#define doc__minimalsimplepair__c__ \
"Usage: minimalsimplepair in.pgm [all] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Detects minimal simple pairs in a 3D binary image. \n"    \
"If the optional parameter  all is used, then all \n"    \
"minimal simple pairs are searched.\n"    \
"Otherwise, only one pair (if any) is given.\n"    \
"\n"    \
"If the input image contains no simple point, consider using \"simplepair\" (faster)\n"    \
"instead of \"minimalsimplepair\".\n"    \
"\n"    \
"Reference: \n"    \
"[PCB08] Nicolas Passat, Michel Couprie, and Gilles Bertrand:\n"    \
"\"Minimal simple pairs in the 3D cubic grid\", in\n"    \
"Journal of Mathematical Imaging and Vision, 32 (3), pp.&nbsp;239‚Äì249, November 2008.\n"    \
"\n"    \
"Types supported: byte 3d\n"    \
"\n"    \
"Category: topobin\n"    \
"\n"    \
"\n"    \
" Michel Couprie 2008"



#define doc__moments__c__ \
"Usage: moments in.pgm out.list\n"    \
"\n"    \
"Description:\n"    \
"\n"    \
"Types supported: byte 2D\n"    \
"\n"    \
"Category: geo\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__moyenne__c__ \
"WRITE ME!!!"



#define doc__MSF__c__ \
"WRITE ME!!!"



#define doc__MSF4d__c__ \
"WRITE ME!!!"



#define doc__mult__c__ \
"Usage: mult in1.pgm in2.pgm out.pgm\n"    \
"\n"    \
"Description:\n"    \
"For each pixel x, out[x] = in1[x] * in2[x]. If both images are byte images,\n"    \
"and if out[x] exceeds 255, then out[x] is set to 255.\n"    \
"Images must be of the same type and same dimensions.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n"    \
"\n"    \
"Category: arith\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__nbcomp__c__ \
"Usage: nbcomp in.pgm connex  [out.list]\n"    \
"\n"    \
"Description:\n"    \
"The argument  connex selects the connectivity (4, 8 in 2D; 6, 18, 26 in 3D).\n"    \
"The argument  selects the kind of flat zone\n"    \
"to be counted:\n"    \
"  fgd selects foreground components for a binary image\n"    \
"  bgd selects background components for a binary image\n"    \
"  min selects regional minima\n"    \
"  max selects regional maxima\n"    \
"  pla selects all flat zones (plateaux).\n"    \
"\n"    \
"The result is written in the list out.list.\n"    \
"\n"    \
"If the parameter  out.list is ommitted, the result is printed on the standard output.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d\n"    \
"\n"    \
"Category: connect\n"    \
"\n"    \
"\n"    \
" Cedric Allene"



#define doc__nbtopo__c__ \
"Usage: nbtopo filein.pgm connex  fileout.pgm\n"    \
"\n"    \
"Description:\n"    \
"For each point p of the input grayscale image, compute the connectivity number T++,\n"    \
"T+, T- or T-- according to the given option (resp. PP, P, M, MM).\n"    \
"Refs: [BEC97, CBB01].\n"    \
"\n"    \
"Types supported: byte 2D, byte 3D\n"    \
"\n"    \
"Category: topogray\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__nbvois__c__ \
"Usage: nbvois in connex out\n"    \
"\n"    \
"Description:\n"    \
"Let X be the set of object points of the binary image  in .\n"    \
"For each point x of X,\n"    \
" out (x) = card( gamma(x) inter X ). \n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: topobin\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__normalize__c__ \
"Usage: normalize in.pgm [nmin nmax] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"For byte and int32_t images, grayscale values are normalized to span the range of \n"    \
"[nmin...nmax].\n"    \
"The parameters  nmin and  nmax are optional.\n"    \
"For byte and int32_t images, the default values are  nmin = 0 and  nmax = 255.\n"    \
"For float images, the default values are  nmin = 0 and  nmax = 1.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n"    \
"\n"    \
"Category: arith\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__offset__c__ \
"Usage: in.pgm ox oy oz [mode] out.ppm\n"    \
"\n"    \
"Description:\n"    \
"Translates the image by the offsets  ox,  oy,  oz in the directions\n"    \
"x, y, z respectively. Offsets may be negative or positive integers.\n"    \
"If  mode is 0, the points that get out of the image frame are lost. \n"    \
"If  mode is 1, a torical space is simulated in all dimensions. \n"    \
"Default mode is 0.\n"    \
"\n"    \
"Types supported: byte 2d, byte3d\n"    \
"\n"    \
"Category: geo\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__ombre__c__ \
"Usage: ombre in.pgm out.pgm\n"    \
"\n"    \
"Description:\n"    \
"The shadow of a 1D grayscale image is a 2D binary image.\n"    \
"The shadow of a 2D grayscale image is a 3D binary image.\n"    \
"\n"    \
"Types supported: byte 1d, byte 2d\n"    \
"\n"    \
"Category: geo\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__openball__c__ \
"Usage: openball in.pgm r [dist] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"The structuring element for this opening is a ball (or a disc) of radius  r.\n"    \
"The opening consists in a erosion (erosball) followed by an dilation (dilatball).\n"    \
"The erosion and dilation are computed by thresholding a distance map.\n"    \
"The distance used depends on the optional parameter  dist (default is 0) :\n"    \
" 0: approximate euclidean distance\n"    \
" 2: chamfer distance\n"    \
" 4: 4-distance in 2d\n"    \
" 8: 8-distance in 2d\n"    \
" 6: 6-distance in 3d\n"    \
" 18: 18-distance in 3d\n"    \
" 26: 26-distance in 3d\n"    \
"\n"    \
" The input image  in.pgm must be a binary image. No test is done.\n"    \
"\n"    \
"Types supported: byte 2D, byte 3D\n"    \
"\n"    \
"Category: morpho\n"    \
"\n"    \
"\n"    \
" Michel Couprie 2002\n"    \
"\n"    \
"Example:\n"    \
"\n"    \
"openball cells 3 cells_openball\n"    \
"\n"    \
"\n"    \
"  \n"    \
"      \n"    \
"  \n"    \
"  cells\n"    \
"      cells_openball\n"    \
"  \n"    \
""



#define doc__opening__c__ \
"Usage: opening in.pgm se.pgm out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Morphological opening by a plane structuring element.\n"    \
"The (plane) structuring element is given by the non-null values in  se.pgm, \n"    \
"its origin (wrt the point (0,0) of  se.pgm ) \n"    \
"is given by a comment line in the file se.pgm. \n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: morpho\n"    \
"\n"    \
"\n"    \
" Michel Couprie 2002"



#define doc__openingfunction__c__ \
"Usage: openingfunction in.pgm [dist] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"This operator associates, to each point x of the input object X, \n"    \
"the radius of the biggest ball included in X that includes x.\n"    \
"The distance used depends on the optional parameter  dist (default is 0) :\n"    \
" 0: approximate euclidean distance (truncated)\n"    \
" 1: approximate quadratic euclidean distance\n"    \
" 2: chamfer distance\n"    \
" 3: exact quadratic euclidean distance\n"    \
" 4: 4-distance in 2d\n"    \
" 8: 8-distance in 2d\n"    \
" 6: 6-distance in 3d\n"    \
" 18: 18-distance in 3d\n"    \
" 26: 26-distance in 3d\n"    \
"\n"    \
" The input image  in.pgm must be a binary image. No test is done.\n"    \
"\n"    \
"Types supported: byte 2D, byte 3D\n"    \
"\n"    \
"Category: morpho\n"    \
"\n"    \
"\n"    \
" Michel Couprie 2009"



#define doc__pca__c__ \
"Usage: pca in.list out.list\n"    \
"\n"    \
"Description:\n"    \
"Computes the main directions (2D or 3D vectors) of a set of points in 2D or 3D space, by the method of Pricipal Component Analysis (PCA).\n"    \
"\n"    \
"The output is a file in the list \"b\" or \"B\" format that contains the following information:\n"    \
" center of mass (point)\n"    \
" first principal direction (vector) \n"    \
" second principal direction (vector) \n"    \
" for 3D images, third principal direction (vector) \n"    \
"\n"    \
"Types supported: list 2D, list 3D\n"    \
"\n"    \
"Category: stats\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__pgm2bmp__c__ \
"WRITE ME!!!"



#define doc__pgm2closedcurve__c__ \
"Usage: pgm2closedcurve image.pgm connex [x y [z]] out.curve\n"    \
"\n"    \
"Description:\n"    \
"Extracts a closed curve from a binary image, that is, a curve with the\n"    \
"same starting and ending point.\n"    \
"The parameter  connex is the connexity of the curve.\n"    \
"It may be equal to 4 or 8 in 2D, and to 6, 18 or 26 in 3D.\n"    \
"If given, the point (x, y, z) (2D) or (x, y, z) (3D) \n"    \
"is taken as the starting point of the curve, and must be a curve point. \n"    \
"The output is the text file  out.curve, with the following format:\n"    \
"b nbpoints\n"    \
"x1 y1\n"    \
"x2 y2\n"    \
"...\n"    \
"or (3D): \n"    \
"B nbpoints\n"    \
"x1 y1 z1\n"    \
"x2 y2 z2\n"    \
"...\n"    \
"\n"    \
"The points of the curve may also be valued. This is must be indicated by \n"    \
"a value of 40, 80, 60, 180 or 260 for the parameter  connex, instead\n"    \
"of 4, 8, 6, 18 or 26 respectively. In this case,\n"    \
"the output is the text file  out.curve, with the following format:\n"    \
"n nbpoints\n"    \
"x1 y1 v1\n"    \
"x2 y2 v2\n"    \
"...\n"    \
"or (3D): \n"    \
"N nbpoints\n"    \
"x1 y1 z1 v1\n"    \
"x2 y2 z2 v2\n"    \
"...\n"    \
"\n"    \
"Types supported: byte 2D, byte 3D\n"    \
"\n"    \
"Category: convert geo\n"    \
" \n"    \
"\n"    \
" Michel Couprie"



#define doc__pgm2curve__c__ \
"Usage: pgm2curve image.pgm connex [x y [z]] out.curve\n"    \
"\n"    \
"Description:\n"    \
"Extracts a curve from a binary image. \n"    \
"The parameter  connex is the connexity of the curve.\n"    \
"It may be equal to 4 or 8 in 2D, and to 6, 18 or 26 in 3D.\n"    \
"If given, the point (x, y, z) (2D) or (x, y, z) (3D) \n"    \
"is the beginning of the curve, and must be an end point. \n"    \
"The output is the text file  out.curve, with the following format:\n"    \
"b nbpoints\n"    \
"x1 y1\n"    \
"x2 y2\n"    \
"...\n"    \
"or (3D): \n"    \
"B nbpoints\n"    \
"x1 y1 z1\n"    \
"x2 y2 z2\n"    \
"...\n"    \
"\n"    \
"The points of the curve may also be valued. This is must be indicated by \n"    \
"a value of 40, 80, 60, 180 or 260 for the parameter  connex, instead\n"    \
"of 4, 8, 6, 18 or 26 respectively. In this case,\n"    \
"the output is the text file  out.curve, with the following format:\n"    \
"n nbpoints\n"    \
"x1 y1 v1\n"    \
"x2 y2 v2\n"    \
"...\n"    \
"or (3D): \n"    \
"N nbpoints\n"    \
"x1 y1 z1 v1\n"    \
"x2 y2 z2 v2\n"    \
"...\n"    \
"\n"    \
"Types supported: byte 2D, byte 3D\n"    \
"\n"    \
"Category: convert geo\n"    \
" \n"    \
"\n"    \
" Michel Couprie"



#define doc__pgm2GA__c__ \
"WRITE ME!!!"



#define doc__pgm2GA4d__c__ \
"Usage: pgm2GA4d prefix_in first last prefix_mark mode GA4d_out.pgm\n"    \
"\n"    \
"Description: \n"    \
"Create a 4D weighted-edges graph from the 4D image made of the\n"    \
"concatenation of the 3D images prefix_inxxxx.pgm (where nnnn is\n"    \
"a four digit decimal integer and where first and last\n"    \
"specify respectively the first and the last volume). The values of\n"    \
"edges are computed according to the parameter mode. If mode is set to\n"    \
"0 then the intensity diference between the two extremities pixels of\n"    \
"the edges is choosen for. Otherwise the maximum value is taken. The\n"    \
"outputed GA is necessarily 4D, 8-connected (i.e. it corresponds to the\n"    \
"direct adjacency in dimension 4).\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d, byte 4d\n"    \
"\n"    \
"Category: convert\n"    \
"\n"    \
"\n"    \
" Jean Cousty - janvier 2006"



#define doc__pgm2list__c__ \
"Usage: pgm2list image.pgm  out.list\n"    \
"\n"    \
"Description:\n"    \
"Converts from pgm format to list format:\n"    \
" e: set of points\n"    \
" s: 1d digital signal\n"    \
" b: binary 2D image\n"    \
" n: grayscale 2D image\n"    \
" B: binary 3D image\n"    \
" N: grayscale 3D image\n"    \
"\n"    \
"In formats  e,  b and  B, only the points with non-null values are \n"    \
"considered.\n"    \
"\n"    \
"In formats  s,  n and  N, all the points (even those having a \n"    \
"null value) are considered.\n"    \
"\n"    \
"Summary of list formats:\n"    \
"  \n"    \
"  e &lt;n&gt;       s &lt;n&gt;         b &lt;n&gt;         n &lt;n&gt;            B &lt;n&gt;            N &lt;n&gt;    \n"    \
"  x1          x1 v1         x1 y1         x1 y1 v1         x1 y1 z1         x1 y1 z1 v1\n"    \
"  x2    or    x2 v2   or    x2 y2   or    x2 y2 v2   or    x2 y2 z2   or    z2 x2 y2 v2\n"    \
"  ...         ...           ...           ...              ...              ...\n"    \
"  xn          xn vn         xn yn         xn yn vn         xn yn z3         z3 xn yn vn\n"    \
"\n"    \
"Types supported: byte 2D, byte 3D, int32_t 2D, int32_t 3D, float 2D, float 3D\n"    \
"\n"    \
"Category: convert\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__pgm2list1d__c__ \
"WRITE ME!!!"



#define doc__pgm2pov__c__ \
"Usage: pgm2pov in.pgm mode out.pov\n"    \
"\n"    \
"Description:\n"    \
"\n"    \
"mode = \n"    \
"   0 (none)\n"    \
"   1 (between points)\n"    \
"   2 (everywhere)\n"    \
"   3 (6-links)\n"    \
"   4 (26-links)\n"    \
"   5 (cubes)\n"    \
"   6 (triangles)\n"    \
"   10-14: idem 0-4, khalimsky grid\n"    \
"   15: colored khalimsky \n"    \
"\n"    \
"Types supported: byte 3d\n"    \
"\n"    \
"Category: convert\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__pgm2ppm__c__ \
"Usage: pgm2ppm  out.ppm\n"    \
"\n"    \
"Description:\n"    \
"1st mode (3 arguments): combines three grayscale images: r.pgm, g.pgm and\n"    \
"b.pgm as the red, green and blue color planes of the color image out.ppm\n"    \
"\n"    \
"2nd mode (2 arguments): generates a color ppm image from the image in.pgm and the \n"    \
"lookup table in.lut\n"    \
"\n"    \
"A lookup table is represented by a special ppm image with 1 row. \n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: convert\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__pgm2ps__c__ \
"Usage: pgm2ps in.pgm mode   coord maxval out.ps\n"    \
"\n"    \
"Description:\n"    \
"Produces a Postscript file from a binary or grayscale image.\n"    \
"If mode = \n"    \
"     b: binary image\n"    \
"     c: grayscale image (levels not printed)\n"    \
"     n: grayscale image (levels printed as numbers on a colored background)\n"    \
"     m: grayscale image (levels printed as numbers on a colored background)\n"    \
"     d: grayscale image (idem n - levels are square-rooted)\n"    \
"     i: grayscale image (levels printed as numbers on an inverted colored background)\n"    \
"     p: grayscale image (levels printed as numbers)\n"    \
"     a: grayscale image (levels printed as letters: a=1, b=2...)\n"    \
"     g: grayscale image (levels showed as colored items)\n"    \
"     v: vector image\n"    \
"     B: binary khalimsky grid\n"    \
"     N: grayscale khalimsky grid (levels printed as numbers)\n"    \
"     G: grayscale khalimsky grid (levels showed as colored items)\n"    \
"     C: binary khalimsky grid (dual of B)\n"    \
"     M: grayscale khalimsky grid (dual of N)\n"    \
"     H: grayscale khalimsky grid (dual of G)\n"    \
"\n"    \
"If an image  label.pgm is present and the mode is n, the level number will be replaced by a letter for each pixel not null in the label image.\n"    \
"\n"    \
"If an image  label.pgm is present and the mode is m, the printed number will\n"    \
"taken in the image  label.pgm while the color will be taken from  in.pgm .\n"    \
"\n"    \
"If an image  marker.pgm is present, a circle will be drawn on each point not null in the marker image.\n"    \
"\n"    \
"The parameter  coord is a int32_t (0 or 1) which commands the inclusion of axis coordinates in the\n"    \
"figure. \n"    \
"\n"    \
"The parameter  maxval is an integer which indicates the maximum grayscale value of the figure\n"    \
"(not necessarily the maximum value of the input image).\n"    \
"\n"    \
"Types supported: byte 2d, int32_t 2d\n"    \
"\n"    \
"Category: convert\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__pgm2raw__c__ \
"Usage: pgm2raw in.pgm out.raw\n"    \
"\n"    \
"Description: suppress the header from a pgm file\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: convert\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__pgm2skel__c__ \
"Usage: pgm2skel in.pgm connex [len] out.skel\n"    \
"\n"    \
"Description:\n"    \
"The skeleton found in  in.pgm is decomposed into isolated points, end points, curves and junctions ;\n"    \
"and its description is stored in  out.skel .\n"    \
"The parameter  connex sets the adjacency relation used for the object\n"    \
"(4, 8 (2d) or 6, 18, 26 (3d)).\n"    \
"\n"    \
"The optional parameter  indicates the minimum length \n"    \
"(in pixels/voxels) of a curve. If a set of curve points with less than  points, then: a) if it contains at least one end point it will be eliminated (together with its end point(s)), b) otherwise it will be considered as part of a junction.\n"    \
"If this parameter is given, then isolated points will be eliminated. \n"    \
"\n"    \
" Points at the border of the image will be ignored.\n"    \
"\n"    \
" IMPORTANT LIMITATION: \n"    \
"different junctions in the original image must not be in direct\n"    \
"contact with each other (i.e., connected) otherwise they will be\n"    \
"considered as a single junction. To prevent this to occur, one can\n"    \
"increase image resolution.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: topobin\n"    \
"\n"    \
"\n"    \
" Michel Couprie 2004, 2009"



#define doc__pgm2skel2__c__ \
"Usage: pgm2skel in.pgm junc.pgm connex out.skel\n"    \
"\n"    \
"Description:\n"    \
"The skeleton found in  in.pgm is decomposed into isolated points, end points, curves and junctions ;\n"    \
"and its description is stored in  out.skel .\n"    \
"The parameter  connex sets the adjacency relation used for the object\n"    \
"(4, 8 (2d) or 6, 18, 26 (3d)).\n"    \
"\n"    \
"The image given as parameter junc.pgm contains curve points that will artificially considered as junction points. \n"    \
"\n"    \
" Points at the border of the image will be ignored.\n"    \
"\n"    \
" IMPORTANT LIMITATION: \n"    \
"different junctions in the original image must not be in direct\n"    \
"contact with each other (i.e., connected) otherwise they will be\n"    \
"considered as a single junction. To prevent this to occur, one can\n"    \
"increase image resolution.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: topobin\n"    \
"\n"    \
"\n"    \
" Michel Couprie 2009"



#define doc__pgm2slc__c__ \
"WRITE ME!!!"



#define doc__pgm2vskel__c__ \
"Usage: pgm2vskel in.pgm val.pgm connex out.skel\n"    \
"\n"    \
"Description:\n"    \
"The skeleton found in  in.pgm is decomposed into isolated points, \n"    \
"end points, curves and junctions ;\n"    \
"and its description is stored in  out.skel . \n"    \
"Each point of the skeleton is valuated by the corresponding value\n"    \
"found in  val.pgm .\n"    \
"The parameter  connex sets the adjacency relation used for the object\n"    \
"(4, 8 (2d) or 6, 18, 26 (3d)).\n"    \
"\n"    \
" No verification is done to check that the input image  in.pgm \n"    \
"contains indeed a curvilinear skeleton.\n"    \
"In the contrary case, the result would be meaningless.\n"    \
"\n"    \
" IMPORTANT LIMITATION: different junctions in the original image \n"    \
"must not be in direct contact with each other\n"    \
"(i.e., connected) otherwise they will be considered as a single junction. \n"    \
"To prevent this to occur, one can increase image resolution. \n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: topobin\n"    \
"\n"    \
"\n"    \
" Michel Couprie 2004"



#define doc__pgm2vtk__c__ \
"Usage: pgm2vtk in.pgm mode out.vtk\n"    \
"\n"    \
"Description:\n"    \
"Create a vtk image from a 3D pgm image.\n"    \
"If mode = \n"    \
"     1: surfels (2-faces) from a binary khalimsky grid\n"    \
"     2: triangulated surfels (2-faces) from a binary khalimsky grid\n"    \
"     3: voxels (cubes) from a binary image in Z3\n"    \
"     4: minimal grid from a binary image in Z3\n"    \
"     5: voxels (octaedrons) from a binary image in Z3\n"    \
"     6: linels (1-faces) from a binary khalimsky grid\n"    \
"\n"    \
"Types supported: byte 3d\n"    \
"\n"    \
"Category: convert\n"    \
"\n"    \
"\n"    \
" Michel Couprie (2005)"



#define doc__pgmascmode__c__ \
"Usage: pgmascmode filename [outfilename | \"wh\" outfilename]\n"    \
"\n"    \
"Description: Converts a pgm file to ascii mode.\n"    \
"If only parameter  filename is given, then the result\n"    \
"is also named  filename .\n"    \
"If the keyword \"wh\" is used as second parameter (stands for \"without header\"), then \n"    \
"an ascii file without header is produced.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n"    \
"\n"    \
"Category: convert\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__pgmrawmode__c__ \
"Usage: pgmrawmode filename [outfilename]\n"    \
"\n"    \
"Description: Converts a pgm file to raw mode.\n"    \
"If only parameter  filename is given, then the result\n"    \
"is also named  filename .\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: convert\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__point__c__ \
"Usage: point in.pgm x y z v out.pgm\n"    \
"\n"    \
"Description:\n"    \
"The value of the point (x,y,z)\n"    \
"is changed to v.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d\n"    \
"\n"    \
"Category: geo\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__points2spline__c__ \
"Usage: points2spline points.txt spline.txt\n"    \
"\n"    \
"Description:\n"    \
"\n"    \
"Let S be the sequence of control points in points.txt. This program computes\n"    \
"a parametric curve P defined by two (in 2D) or three (in 3D) splines \n"    \
"that interpolates the points of the sequence S.\n"    \
"\n"    \
"If the last point equals the first point, then a closed spline is generated.\n"    \
"\n"    \
"The result is given in the form of the list of control points, followed by\n"    \
"the set of coefficients for each spline segment. \n"    \
"\n"    \
"The input file  points.txt contains a list of points under the format:\n"    \
"\n"    \
"b nbpoints  \n"    \
"x1 y1\n"    \
"x2 y2\n"    \
"x3 y3\n"    \
"...\n"    \
"\n"    \
"or:\n"    \
"\n"    \
"B nbpoints  \n"    \
"x1 y1 z1\n"    \
"x2 y2 z2\n"    \
"x3 y3 z3\n"    \
"...\n"    \
"\n"    \
"The output file format is the following for 2D:\n"    \
"\n"    \
"c n+1 (where n+1 denotes the number of control points)\n"    \
"x1 y1\n"    \
"...\n"    \
"xn+1 yn+1\n"    \
"C0X1 C0Y1 C1X1 C1Y1 C2X1 C2Y1 C3X1 C3Y1\n"    \
"...\n"    \
"C0Xn C0Yn C1Xn C1Yn C2Xn C2Yn C3Xn C3Yn\n"    \
"\n"    \
"and in the 3D case:\n"    \
"\n"    \
"C n+1 (where n+1 denotes the number of control points)\n"    \
"x1 y1 z1\n"    \
"...\n"    \
"xn+1 yn+1 zn+1\n"    \
"C0X1 C0Y1 C0Z1 C1X1 C1Y1 C1Z1 C2X1 C2Y1 C2Z1 C3X1 C3Y1 C3Z1\n"    \
"...\n"    \
"C0Xn C0Yn C0Zn C1Xn C1Yn C1Zn C2Xn C2Yn C2Zn C3Xn C3Yn C3Zn\n"    \
"\n"    \
"The ith segment of the parametric curve P is then defined by:\n"    \
"\n"    \
"x(t) = C3Xi * t^3 + C2Xi * t^2 + C1Xi * t + C0Xi\n"    \
"y(t) = C3Yi * t^3 + C2Yi * t^2 + C1Yi * t + C0Yi\n"    \
"z(t) = C3Zi * t^3 + C2Zi * t^2 + C1Zi * t + C0Zi\n"    \
"with t in [0,1]\n"    \
"\n"    \
"Types supported: list 2D, list 3D\n"    \
"\n"    \
"Category: draw geo\n"    \
" \n"    \
"\n"    \
" Michel Couprie"



#define doc__pol2car__c__ \
"Usage: in_pol in_car x_center y_center out\n"    \
"\n"    \
"Description:\n"    \
"Converts a 2d image from polar to cartesian coordinates.\n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: convert\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__pow__c__ \
"Usage: pow in.pgm p out.pgm\n"    \
"\n"    \
"Description:\n"    \
"For each pixel x, out[x] = in[x]^p. If the image is a byte image,\n"    \
"and if out[x] exceeds 255, then out[x] is set to 255.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n"    \
"\n"    \
"Category: arith\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__ppm2bmp__c__ \
"WRITE ME!!!"



#define doc__ppm2GA__c__ \
"WRITE ME!!!"



#define doc__ppm2pgm__c__ \
"Usage: ppm2pgm in.ppm r.pgm g.pgm b.pgm\n"    \
"\n"    \
"Description:\n"    \
"Converts a color ppm image  in.ppm into 3 grayscale pgm images\n"    \
"r.pgm, g.pgm and b.pgm which are the red, \n"    \
"green and blue color planes of the original color image.\n"    \
"\n"    \
"Types supported: byte color 2d\n"    \
"\n"    \
"Category: convert\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__ppmascmode__c__ \
"Usage: ppmascmode filename [outfilename]\n"    \
"\n"    \
"Description: Converts a ppm file to ascii mode.\n"    \
"If only parameter  filename is given, then the result\n"    \
"is also named  filename .\n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: convert\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__printhisto__c__ \
"Usage: printhisto in.pgm [mask.pgm]\n"    \
"\n"    \
"Description:\n"    \
"Calculates the histogram of  im.pgm (masked by the binary image\n"    \
" mask.pgm, if given) and prints it on the screen.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d\n"    \
"\n"    \
"Category: histo\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__printstats__c__ \
"Usage: printstats in.pgm [mask.pgm]\n"    \
"\n"    \
"Description:\n"    \
"Calculates the histogram of  im.pgm (masked by the binary image\n"    \
" mask.pgm, if given) and prints it on the screen.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d\n"    \
"\n"    \
"Category: histo\n"    \
"\n"    \
"\n"    \
" Laurent Najman"



#define doc__profil__c__ \
"WRITE ME!!!"



#define doc__proj__c__ \
"Usage: proj in.pgm dir mode out.pgm\n"    \
"\n"    \
"Description:\n"    \
"\n"    \
"Projection parallel to one of the main axes.\n"    \
"\n"    \
"dir = x | y | z\n"    \
"\n"    \
"mode = 0 (mean) | 1 (max)\n"    \
"\n"    \
"Types supported: byte 2D, byte 3D\n"    \
"\n"    \
"Category: geo\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__proj3d__c__ \
"WRITE ME!!!"



#define doc__projsphere__c__ \
"Usage: projsphere in.pgm out.pgm\n"    \
"\n"    \
"Description: Projection on a sphere.\n"    \
"\n"    \
"Types supported: byte 2D, byte 3D\n"    \
"\n"    \
"Category: geo\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__propgeo__c__ \
"Usage: propgeo in.pgm mask.pgm connex mode out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Propagates certain values of the original image in.pgm, \n"    \
"geodesicaly wrt the connected components of the binary image mask.pgm.\n"    \
"The connexity is specified by the parameter connex.\n"    \
"The mode is one of the following ones:\n"    \
"    min   : propagates the minimal value of the component\n"    \
"    min1  : selects one point the value of which is equal to the min\n"    \
"    max   : propagates the maximal value of the component\n"    \
"    max1  : selects one point the value of which is equal to the max\n"    \
"    moy   : propagates the mean grayscale value of the component\n"    \
"    moy1  : selects one point the value of which is nearest to the mean\n"    \
"    minb  : propagates the minimal value of the external border of the component\n"    \
"    maxb  : propagates the maximal value of the external border of the component\n"    \
"    moyb  : propagates the mean value of the external border of the component\n"    \
"    randb : fills the component with random values, with a normal distribution\n"    \
"            centered around the value computed as for moyb\n"    \
"\n"    \
"Only modes min, max and moy are available for int32_t images.\n"    \
"\n"    \
"Types supported: byte 2d, int32_t 2d\n"    \
"\n"    \
"Category: connect\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__ptcurve__c__ \
"Usage: ptcurve in.pgm connex out.pgm\n"    \
"\n"    \
"Description:\n"    \
"An curve point is a white point, which has exactly 2 white n-neighbours \n"    \
"that are not n-neighbours to each other. In other words, Tn = 2 and \n"    \
"each neighboring n-connected component is made of exactly one point\n"    \
"(n = 4, 8 (2d) or 6, 18, 26 (3d), as set by the parameter  connex)\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: topobin\n"    \
"\n"    \
"\n"    \
" Michel Couprie 2002"



#define doc__ptend__c__ \
"Usage: ptend in.pgm connex out.pgm\n"    \
"\n"    \
"Description:\n"    \
"An end point is a white point, which has exactly 1 white n-neighbour\n"    \
"(n = 4, 8 (2d) or 6, 18, 26 (3d), as set by the parameter  connex)\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: topobin\n"    \
"\n"    \
"\n"    \
" Michel Couprie 2002"



#define doc__ptextensible__c__ \
"Usage: ptextensible im.pgm  connex fileout.pgm\n"    \
"\n"    \
"Description:\n"    \
"Extensible point detection.\n"    \
"The input image  im.pgm must be a \"thin\" grayscale image, as\n"    \
"the output of the operator  hthin.\n"    \
"The parameter  connex indicates the cconnectivity used for the minimal \n"    \
"regions.\n"    \
"The optional parameter  imcond.pgm is a binary image which indicates the points\n"    \
"in the neighborhood of which the extensible points will be searched.\n"    \
"\n"    \
"Reference: \n"    \
"M. Couprie, F.N. Bezerra, Gilles Bertrand: \"Topological operators for\n"    \
"grayscale image processing\",  Journal of Electronic Imaging, Vol.&nbsp;10, No.&nbsp;4, pp.&nbsp;1003-1015, 2001.\n"    \
"\n"    \
"Types supported: byte 2D\n"    \
"\n"    \
"Category: topogray\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__pthseparatinggray__c__ \
"Usage: pthseparatinggray in.pgm connex h out.pgm\n"    \
"\n"    \
"Description:\n"    \
"An h-separating point for an image F is a point p, \n"    \
"such that there exists c verifying F(p)-h  1, with X = {x | F(x) Types supported: byte 2d, byte 2d\n"    \
"\n"    \
"Category: topogray\n"    \
"\n"    \
"\n"    \
" Michel Couprie 2010"



#define doc__ptinterior__c__ \
"Usage: ptinterior in.pgm connex out.pgm\n"    \
"\n"    \
"Description:\n"    \
"An interior point is a white point, all the n-neighbours of which are white\n"    \
"(n = 4, 8 (2d) or 6, 18, 26 (3d), as set by the parameter  connex)\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: topobin\n"    \
"\n"    \
"\n"    \
" Michel Couprie 2005"



#define doc__ptisolated__c__ \
"Usage: ptisolated in.pgm connex out.pgm\n"    \
"\n"    \
"Description:\n"    \
"An isolated point is a white point, all the n-neighbours of which are black\n"    \
"(n = 4, 8 (2d) or 6, 18, 26 (3d), as set by the parameter  connex)\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: topobin\n"    \
"\n"    \
"\n"    \
" Michel Couprie 2002"



#define doc__ptjunction__c__ \
"Usage: ptjunction in.pgm connex out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Detects junction points in the 2d or 3d binary image  in.pgm, which is supposed to contain a skeleton.\n"    \
"A junction point is a white point x such that #(Nn[x] inter X) > 2,\n"    \
"where Nn[x] stands for the n-neighborhood of x (excluding x), and\n"    \
"n = 4, 8 in 2D or n = 6, 18, 26 in 3D, as set by the parameter  connex.\n"    \
"\n"    \
"Types supported: byte 2D, byte 3D\n"    \
"\n"    \
"Category: topobin\n"    \
"\n"    \
"\n"    \
" Michel Couprie 2010"



#define doc__ptmultiple__c__ \
"Usage: ptmultiple in.pgm connex out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Detects multiple points in the 2d or 3d binary image  in.pgm .\n"    \
"A multiple point is a white point x such that Tn[x] > 2\n"    \
"(n = 4, 8 in 2D or n = 6, 18, 26 in 3D, as set by the parameter  connex).\n"    \
"\n"    \
"Types supported: byte 2D, byte 3D\n"    \
"\n"    \
"Category: topobin\n"    \
"\n"    \
"\n"    \
" Michel Couprie 2002"



#define doc__ptselectgray__c__ \
"Usage: ptselectgray in.pgm connex mint+ maxt+ mint-- maxt-- out.pgm\n"    \
"\n"    \
"Description:\n"    \
"\n"    \
"Select all points p such that the condition:\n"    \
"mint+ \n"    \
"is satisfied.\n"    \
"\n"    \
"The parameter  connex gives the connectivity used for the minima;\n"    \
"possible choices are 4, 8 in 2D and 6, 26 in 3D.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d \n"    \
"\n"    \
"Category: topogray\n"    \
"\n"    \
"\n"    \
" Nicolas Combaret 2006"



#define doc__ptseparating__c__ \
"Usage: ptseparating in.pgm connex out.pgm\n"    \
"\n"    \
"Description:\n"    \
"A separating point is a white point x such that Tb_n[x] >= 2\n"    \
"(n = 4,8 in 2D or n = 6,18,26 in 3D, as set by the parameter  connex)\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d \n"    \
"\n"    \
"Category: topobin\n"    \
"\n"    \
"\n"    \
" Michel Couprie 2002"



#define doc__ptseparatinggray__c__ \
"Usage: ptseparatinggray in.pgm connex out.pgm\n"    \
"\n"    \
"Description:\n"    \
"A separating point is a point p, which is separating (in the binary sense)\n"    \
"for a section h of F, with h Types supported: byte 2d\n"    \
"\n"    \
"Category: topogray\n"    \
"\n"    \
"\n"    \
" Michel Couprie 2002"



#define doc__ptsimple__c__ \
"Usage: ptsimple in.pgm connex out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Detects simple points in a binary image.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: topobin\n"    \
"\n"    \
"\n"    \
" Michel Couprie 2003"



#define doc__ptWsimple__c__ \
"Usage: ptWsimple in.pgm connex out.pgm\n"    \
"\n"    \
"Description:\n"    \
"The argument  connex selects the connectivity (4, 8 in 2D).\n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: connect\n"    \
"\n"    \
"\n"    \
" Jean Cousty (2007)"



#define doc__qdistance__cpp__ \
"WRITE ME!!!"



#define doc__quasishear__c__ \
"Usage: quasishear in.pgm theta [x y] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"rotates the image of the angle  theta (in degrees).\n"    \
"If a point (x,y) is given, then the result is croped to the original\n"    \
"image size, otherwise a new image with sufficient size is created.\n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: geo\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__radialopening__c__ \
"Usage: radialopening in.pgm length [angle in degrees] [dilatmask.pgm] out.pgm\n"    \
"\n"    \
"Description: \n"    \
"Radial opening by line (or dilated line). Max between the openings of\n"    \
"in.pgm by a linear structuring element rotated by angle until 360 degrees\n"    \
"and dilated by dilatmask.pgm. The angle is normalized to an exact division\n"    \
"of 360 by an integer.\n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: morpho\n"    \
"\n"    \
"\n"    \
" AndrÈ Vital Saude jan 2005"



#define doc__radialrankmaxopening__c__ \
"Usage: radialrankmaxopening in.pgm length rank [angle in degrees] [dilatmask.pgm] out.pgm\n"    \
"\n"    \
"Description: Radial rank-max opening by line (or dilated\n"    \
"line). Max between the rank-max openings (with parameter  rank) of\n"    \
" in.pgm by a linear structuring element rotated by angle until 360\n"    \
"degrees and dilated by dilatmask.pgm. The angle is normalized to an\n"    \
"exact division of 360 by an integer.\n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: morpho\n"    \
"\n"    \
"\n"    \
" AndrÈ Vital Saude, Michel Couprie jan 2005"



#define doc__randimage__c__ \
"Usage: randimage [in.pgm | rowsize colsize depth] valmax [seed] out.pgm\n"    \
"\n"    \
"Description: Generates an image with pixel values uniformly randomly\n"    \
"distributed between 0 and  valmax (included). \n"    \
"If given, the parameter  seed is used to \n"    \
"initialize the random generator, otherwise the clock is used instead.\n"    \
"\n"    \
"Types supported: byte 2D, byte 3D, int32_t 2D, int32_t 3D\n"    \
"\n"    \
"Category: signal\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__randpoints__c__ \
"WRITE ME!!!"



#define doc__randrgb__c__ \
"Usage: randrgb in.pgm out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Generates a color image where each gray level is replaced by a randomly\n"    \
"chosen color\n"    \
"\n"    \
"Types supported: byte 2d, int32_t 2d\n"    \
"\n"    \
"Category: convert\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__rankfilter__c__ \
"Usage: rankfilter in.pgm el.pgm r out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Let F be the input image, G be the output image, and E the structuring\n"    \
"element. Let n be the number of elements of E, and R be the product n.r,\n"    \
"then for each pixel p, G[p] is the Rth element of the sorted list (by \n"    \
"increasing order) of the pixel values in the set { F[q], q in E[p] }.\n"    \
"\n"    \
"Particular cases are the median filter (r = 0.5), the erosion (r = 0),\n"    \
"and the dilation (r = 1).\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: morpho\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__rankmaxopening__c__ \
"Usage: rankmaxopening in.pgm el.pgm r out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Let F be the input image, G be the output image, and E the structuring\n"    \
"element.\n"    \
"\n"    \
"The rank-max opening [Ron86]\n"    \
"of structuring element $E$ and parameter  r in [0...1]\n"    \
"may be defined by G = min(F,dilation(rankfilter(F,E,1-r),E))\n"    \
"\n"    \
"[Ron86] C. Ronse:\n"    \
"\"Erosion of narrow image features by combination of local low rank and max filters\",\n"    \
"Proc. 2nd Int. Conf. on Image Processing and int32_t Applications,\n"    \
"pages 77-81, 1986.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: morpho\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__raw2pgm__c__ \
"Usage: in.raw rs cs ds headersize datatype littleendian [xdim ydim zdim] out.pgm\n"    \
"\n"    \
"Description: Converts from raw format into pgm format. \n"    \
"\n"    \
"Parameters:\n"    \
"  in.pgm : source file in raw format\n"    \
"  rs (int32_t): row size (number of voxels in a row)\n"    \
"  cs (int32_t): column size (number of voxels in a column)\n"    \
"  ds (int32_t): number of planes\n"    \
"  headersize (int32_t): size of the header in bytes (information in the header will be ignored)\n"    \
"  datatype (int32_t): (1 for byte, 2 for short int, 4 for long int, 5 for float)\n"    \
"  littleendian (int32_t) 1: littleendian, 0: bigendian. Usual choice is 0.\n"    \
"  xdim (float, optional) : gap (in the real world) between two adjacent voxels in a row.  \n"    \
"  ydim (float, optional) : gap (in the real world) between two adjacent voxels in a column.  \n"    \
"  zdim (float, optional) : gap (in the real world) between two adjacent planes.  \n"    \
"\n"    \
"Types supported: byte 3D, int16_t 3D, int32_t 3D\n"    \
"\n"    \
" Signed integers and floating numbers are not supported.\n"    \
"\n"    \
"Category: convert\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__readgif__c__ \
"WRITE ME!!!"



#define doc__recalagerigide__c__ \
"Usage: recalagerigide in1 in2 out\n"    \
"\n"    \
"Description:\n"    \
"\n"    \
"Rigid registration of two closed contours. \n"    \
"Let X and Y be two point sets, given respectively by  in1 and  in2.\n"    \
"This procedure identifies the parameters of a rigid deformation R such that\n"    \
"the \"distance\" between R(X) and Y is a local minimum.\n"    \
"The distance D(Z,Y) between two sets Z and Y is defined by:\n"    \
"\n"    \
"D(Z,Y) = sum { d2(z,Y) ; for all z in Z }\n"    \
"\n"    \
"d2(z,Y) = min { d2(z,y) ; for all y in Y }\n"    \
"\n"    \
"d2(z,y) = (z - y)^2 ; i.e., the square or the Euclidean distance between z and y.\n"    \
"\n"    \
"The rigid deformation R is defined as the composition (in this order) \n"    \
"of scalings, rotations and translations. \n"    \
"\n"    \
"In 2d, the parameters are:\n"    \
" sx : parameter for the scaling in direction x\n"    \
" sy : parameter for the scaling in direction y\n"    \
" theta : angle (in radians) of the rotation around the barycenter of X\n"    \
" tx : parameter for the translation in direction x\n"    \
" ty : parameter for the translation in direction y\n"    \
"\n"    \
"In 3d, the parameters are:\n"    \
" sx : parameter for the scaling in direction x\n"    \
" sy : parameter for the scaling in direction y\n"    \
" sz : parameter for the scaling in direction z\n"    \
" theta : angle (in radians) of the rotation around the parallel to the the z axis passing by the barycenter of X\n"    \
" phi : angle (in radians) of the rotation around the parallel to the the y axis passing by the barycenter of X\n"    \
" tx : parameter for the translation in direction x\n"    \
" ty : parameter for the translation in direction y\n"    \
" tz : parameter for the translation in direction z\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: geo\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__recalagerigide_num__c__ \
"Usage: recalagerigide_num in1 in2 xmin ymin xmax ymax seuil [init] out\n"    \
"\n"    \
"Description:\n"    \
"\n"    \
"Rigid registration of two grayscale images. \n"    \
"Let X and Y be two images, given respectively by  in1 and  in2.\n"    \
"This procedure identifies the parameters of a rigid deformation R such that\n"    \
"the \"distance\" between R(X) and Y is a local minimum.\n"    \
"The distance D(Z,Y) between two images Z and Y is defined by:\n"    \
"\n"    \
"D(Z,Y) = sum { (Z[i] - Y[i])^2 ; for all i in support(Y) }\n"    \
"\n"    \
"where support(Y) is a rectangular zone specified by arguments \n"    \
"xmin ymin xmax ymax.\n"    \
"\n"    \
"The rigid deformation R is defined as the composition (in this order) \n"    \
"of a zoom, a rotation, and a translation. \n"    \
"\n"    \
"In 2d, the parameters are:\n"    \
" hx : parameter for the scaling in direction x\n"    \
" hy : parameter for the scaling in direction y\n"    \
" theta : angle (in degrees) of the rotation around (0,0)\n"    \
" tx : parameter for the translation in direction x\n"    \
" ty : parameter for the translation in direction y\n"    \
"\n"    \
"The output parameter  out is the name of a text file in which these \n"    \
"values will be written (type \"e\" list format, see doc/formats.txt).\n"    \
"\n"    \
"The parameter  seuil makes it possible to eliminate outliers: \n"    \
"points i such that (Z[i] - Y[i])^2 > seuil^2 are not taken into account.\n"    \
"A value 255 for this parameter means no outlier elimination.\n"    \
"\n"    \
"The optional parameter  init makes it possible to give, \n"    \
"in the same format as the output, an initial deformation which is \n"    \
"\"close\" to the expected one. The default initialisation is the identity\n"    \
"(parameters 0, 1, 1, 0, 0).\n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: geo\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__recalagerigide_translateplane__c__ \
"WRITE ME!!!"



#define doc__reconsplateaux__c__ \
"WRITE ME!!!"



#define doc__redt__c__ \
"Usage: redt in.pgm out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Reverse euclidean distance transform, by the Coeurjolly's algorithm [1].\n"    \
"The input  in.pgm must be a quadratic euclidean distance map, or \n"    \
"an image of type int32_t where each non-null value correspond to the quadratic\n"    \
"radius of a disc or ball. The output is a binary image.\n"    \
"\n"    \
"[1] D. Coeurjolly,\n"    \
"\"d-Dimensional Reverse Euclidean Distance Transformation\n"    \
"and Euclidean Medial Axis Extraction in Optimal Time\", \n"    \
"DGCI 2003, LNCS 2886, pp. 327-337, 2003.\n"    \
"\n"    \
"Types supported: int32_t 2d, int32_t 3d\n"    \
"\n"    \
"Category: morpho\n"    \
"\n"    \
"\n"    \
" Michel Couprie 2004, Jean Cousty 2005 (3d)"



#define doc__reformat__c__ \
"WRITE ME!!!"



#define doc__relief__c__ \
"WRITE ME!!!"



#define doc__remspnoise__c__ \
"WRITE ME!!!"



#define doc__rgb2hls__c__ \
"Usage: in.ppm [mode] h.pgm l.pgm s.pgm\n"    \
"\n"    \
"Description:\n"    \
"Constructs 3 pgm files from 1 ppm file:\n"    \
" h.pgm : Hue (int32_t - 0 to 359)\n"    \
" h.pgm : Luminance (byte)\n"    \
" h.pgm : Saturation (byte)\n"    \
"\n"    \
"Different modes are available [default mode is 0]:\n"    \
" mode = 0: classical HLS coding, based on [1].\n"    \
" mode = 1: L1 norm (NYI) (see [2])\n"    \
" mode = 2: L1 norm with gamma correction (NYI) (see [2])\n"    \
"\n"    \
"[1] Foley, Van Damm & al: \"Computer Graphics\", 2nd ed., p. 595\n"    \
"\n"    \
"[2] J. Angulo and J. Serra. \"Traitements des images de couleur \n"    \
"en reprÈsentation luminance/saturation/teinte par norme L_1\" (in French). \n"    \
"Traitement du Signal, Vol. 21, No. 6, pp 583-604, December 2004.\n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: convert\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__rgb2ppm__c__ \
"WRITE ME!!!"



#define doc__rotate__c__ \
"Usage: rotate in.pgm theta [x y] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"rotates the image  in.pgm of the angle  theta (in degrees).\n"    \
"Method: interpolation.\n"    \
"If  x and  y are given, then the center of the rotation is the point \n"    \
"(x,y) and the image size is left unchanged (hence parts of object \n"    \
"may be lost). \n"    \
"Otherwise, the center of the rotation is the point (0,0) and the resulting \n"    \
"image size is computed such that no loss of information occur.\n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: geo\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__rotate3dbin__c__ \
"Usage: rotate3dbin in.pgm theta axis [c1 c2] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Method: truncated real rotation.\n"    \
"Rotates the image  in.pgm of the angle  theta (in degrees), \n"    \
"around the straight line defined by parameters  axis,  c1 and  c2, parallel to one of the main axes x, y, z.\n"    \
"Parameter  axis may be either x, y or z.\n"    \
"If  axis = x, then the rotation axis is defined by y =  c1, z =  c2.\n"    \
"If  axis = y, then the rotation axis is defined by x =  c1, z =  c2.\n"    \
"If  axis = z, then the rotation axis is defined by x =  c1, y =  c2.\n"    \
"\n"    \
"If  in.pgm is not a structuring element, and if parameters  c1 and  c2 are omitted then the default values 0, 0 are assumed and the resulting image size is computed such that no loss of information occur. Otherwise, no resize is made.\n"    \
"\n"    \
"If  in.pgm is a structuring element, then parameters  c1 and  c2 are ignored and the coordinates of the rotation axis are taken among the ones of the origin of the structuring element.\n"    \
"\n"    \
"Types supported: byte 3d\n"    \
"\n"    \
"Category: geo\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__rotatebin__c__ \
"Usage: rotatebin in.pgm theta [x y] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"rotates the image  in.pgm of the angle  theta (in degrees).\n"    \
"Method: truncated real rotation.\n"    \
"If  x and  y are given, then the center of the rotation is the point \n"    \
"(x,y) and the image size is left unchanged (hence parts of object \n"    \
"may be lost). \n"    \
"Otherwise, the center of the rotation is the point (0,0) and the resulting \n"    \
"image size is computed such that no loss of information occur.\n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: geo\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__rotse__c__ \
"Usage: rotse in.pgm angle out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Rotation of a structuring element, by an angle of 0, 90, 180 or 270 degrees.\n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: geo\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__saliency__c__ \
"Usage: saliency in connex out\n"    \
"\n"    \
"Description:\n"    \
"Performs the salient watershed transformation on the image in, \n"    \
"The parameter connex gives the adjacency relation (4 or 6b in 2D).\n"    \
"\n"    \
"The image out is a int32_t image (int32_t)\n"    \
"\n"    \
"In 4 connectivity, the output image is a line graph in the khalimsky\n"    \
"grid. In 6b connectivity, the result is on the pixels themselves, with\n"    \
"the gammab neighborhood.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: connect\n"    \
"\n"    \
"\n"    \
" Laurent Najman"



#define doc__saliencyGA__c__ \
"WRITE ME!!!"



#define doc__samplespline__c__ \
"Usage: samplespline in.txt nsamples out.txt\n"    \
"\n"    \
"Description:\n"    \
"\n"    \
"This program computes a regular sampling of the spline given in in.txt.\n"    \
"The result is a list of nsamples points, which are stored in out.txt.\n"    \
"\n"    \
"Types supported: list 2D, list 3D\n"    \
"\n"    \
"Category: draw geo\n"    \
" \n"    \
"\n"    \
" Michel Couprie"



#define doc__scale__c__ \
"Usage: scale in.pgm const out.pgm\n"    \
"\n"    \
"Description:\n"    \
"For each pixel x, out[x] = in[x] * const. If the image is a byte image,\n"    \
"and if out[x] exceeds 255, then out[x] is set to 255.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n"    \
"\n"    \
"Category: arith\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__sceneconvert__c__ \
"Usage: sceneconvert in.3sc format out\n"    \
"\n"    \
"Description:\n"    \
"The file  scene.3sc contains a scene under the 3Dscene format.\n"    \
"\n"    \
"The parameter  format indicate the format of the output file \n"    \
"(choices are POV, POVB).\n"    \
"The keyword POVB corresponds to a bare Povray mesh: a header and a footer must be\n"    \
"catenated in order to make a full Povray scene. \n"    \
"\n"    \
"Types supported: 3D scene\n"    \
"\n"    \
"Category: convert\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__section__c__ \
"Usage: section in.pgm x0 y0 z0 x1 y1 z1 x2 y2 z2 out.pgm\n"    \
"\n"    \
"Description: \n"    \
"The points P0 (  x0,  y0,  z0 ), P1 (  x1,  y1,  z1 ) and\n"    \
"P2 (  x2,  y2,  z2 ) form a basis for a plane in the 3d space. \n"    \
"The intersection of this plane with the domain of the 3d image  in.pgm\n"    \
"form the domain of the 2d image  out.pgm .\n"    \
"The grayscale values of  out.pgm are obtained by trilinear interpolation from\n"    \
"the grayscale values of  in.pgm .\n"    \
"The coordinates of the basis for the plane are expressed in \"real world\" dimensions.\n"    \
"\n"    \
"Types supported: byte 3d, int32_t 3d\n"    \
"\n"    \
"Category: geo\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__segment__c__ \
"WRITE ME!!!"



#define doc__segment3d__c__ \
"WRITE ME!!!"



#define doc__segment3di__c__ \
"WRITE ME!!!"



#define doc__segmentarea__c__ \
"Usage: segmentarea in.pgm connex area out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Segmentation by selection of cross-section components, based on an area criterion. \n"    \
"Connexity = connex ; area threshold = area.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category:  connect\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__segmentheight__c__ \
"Usage: segmentheight in.pgm connex height [r] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Height segmentation with connexity connex and depth height, \n"    \
"followed by a reconstruction (2D only) if option r is set (see reference: to appear)\n"    \
"or a maximization if option m is set.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: connect\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__segmenti__c__ \
"WRITE ME!!!"



#define doc__segmentlignes__c__ \
"WRITE ME!!!"



#define doc__segmentnum__c__ \
"WRITE ME!!!"



#define doc__segmentnumi__c__ \
"WRITE ME!!!"



#define doc__segmentvol__c__ \
"Usage: segmentvol in.pgm connex vol out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Segmentation by selection of cross-section components, based on a \n"    \
"volume criterion. \n"    \
"Connexity = connex ; volume threshold = vol.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: connect\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__selectcomp__c__ \
"Usage: selectcomp in.pgm connex x y z out.pgm\n"    \
"\n"    \
"Description:\n"    \
"The connected component of the binary image in.pgm\n"    \
"(according to the connectivity connex)\n"    \
"which contains the point (x,y,z)\n"    \
"is preserved. All other points are deleted.\n"    \
"Possible values for  connex are 4, 8 (2D), 6, 18, 26, 60, 260 (3D).\n"    \
"Values 60 and 260 correspond to restrictions of 6 and 26 connectivities to the \n"    \
"current xy plane.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: connect\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__selndg__c__ \
"Usage: selndg in.pgm inf sup out.ppm\n"    \
"\n"    \
"Description: \n"    \
"Selects pixels x such that  inf  Types supported: byte 2d, byte 3d, long 2d, long 3d\n"    \
"\n"    \
"Category: arith\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__selrect__c__ \
"WRITE ME!!!"



#define doc__seltopo__c__ \
"Usage: seltopo in connex t- t+ tb- tb+ out\n"    \
"\n"    \
"Description:\n"    \
"Let X be the set of points of the binary image  in, let x be a point.\n"    \
"Let t(x) and tb(x) be the two connectivity numbers for x.\n"    \
"The result  out is the set of points:\n"    \
"{ x in X, t- Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: topobin\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__setorigin__c__ \
"Usage: setorigin in.pgm [source_se | xorig yorig zorig] \n"    \
"\n"    \
"Description: Adds origin (the one of  se or  xorig  yorig  zorig ) \n"    \
"to file header of  in.pgm.\n"    \
"\n"    \
"Types supported: byte 3d, int32_t 3d\n"    \
"\n"    \
"Category: convert\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__setvoxdim__c__ \
"Usage: setvoxdim image xdim ydim zdim\n"    \
"\n"    \
"Description: Adds voxel dimensions ( xdim  ydim  zdim ) \n"    \
"to file header.\n"    \
"\n"    \
"Types supported: byte 3d, int32_t 3d\n"    \
"\n"    \
"Category: convert\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__seuil__c__ \
"Usage: seuil in.pgm n [n2] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"If n2 is not present, for each pixel x, out[x] = if (in[x] Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n"    \
"\n"    \
"Category: arith\n"    \
"\n"    \
"\n"    \
" Michel Couprie 1997"



#define doc__seuil2__c__ \
"Usage: seuil2 in.pgm smin smax vmin vmax out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Every pixel   =  smax gets value  vmax \n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: arith\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__seuilauto__c__ \
"Usage: seuilauto in.pgm L  [n] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Determine automatically a threshold thanks to the smoothed histogram\n"    \
"of the picture ( L : smoothing coefficient, 0 selects the part of the picture\n"    \
"to be kept:\n"    \
"\n"    \
"  min: Every pixel   =  smax gets value  vmax \n"    \
"  max: Every pixel >  smin gets value  vmin and every pixel   Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d\n"    \
"\n"    \
"Category: arith\n"    \
"\n"    \
"\n"    \
" Cedric Allene 2003"



#define doc__seuilhisto__c__ \
"WRITE ME!!!"



#define doc__seuilhistoglobal__c__ \
"WRITE ME!!!"



#define doc__seuilhistolocal__c__ \
"WRITE ME!!!"



#define doc__seuili__c__ \
"WRITE ME!!!"



#define doc__shake__c__ \
"WRITE ME!!!"



#define doc__shencastan__c__ \
"WRITE ME!!!"



#define doc__shortestpath__c__ \
"Usage: shortestpath in.pgm connex mode S1 S2 [S3] D1 D2 [D3] out.lst\n"    \
"\n"    \
"Description:\n"    \
"A weighted graph G = (V,E) is defined with V = set of pixels of  in.pgm \n"    \
"and E = {(P,Q) in VxV ; P and Q are  connex- adjacent}. \n"    \
"Let F(P) be the value of pixel P in the image  in.pgm. \n"    \
"A weight W(P,Q) is assigned to each edge, according to the value of  mode:\n"    \
"\n"    \
" max : W(P,Q) = max{F(P),F(Q)} \n"    \
" min : W(P,Q) = min{F(P),F(Q)} \n"    \
" avg : W(P,Q) = (F(P) + F(Q)) / 2 \n"    \
"\n"    \
"This operator finds a shortest path from ( S1,  S2) to ( D1,  D2)\n"    \
"in this graph. The result is given as a list of vertices (pixels) \n"    \
"in  out.lst.\n"    \
"\n"    \
"Types supported: byte 2D, byte 3D\n"    \
"\n"    \
"Category: connect\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__showheader__c__ \
"Usage: showheader in.pgm\n"    \
"\n"    \
"Description:\n"    \
"Prints all informations contained in the image file header: type, header, size and comments.\n"    \
"\n"    \
"Types supported: all pgm and ppm types\n"    \
"\n"    \
"Category: convert\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__showpoint__c__ \
"Usage: showpoint in.pgm x y z\n"    \
"\n"    \
"Description:\n"    \
"The value of the point (x,y,z)\n"    \
"is printed in the standard output stream.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n"    \
"\n"    \
"Category: convert\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__shrinkondisk__c__ \
"WRITE ME!!!"



#define doc__simplepair__c__ \
"Usage: simplepair in.pgm [all] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Detects simple pairs in a 3D binary image. \n"    \
"If the optional parameter  all is used, then all simple pairs are searched. \n"    \
"Otherwise, only one pair (if any) is given.\n"    \
"\n"    \
"Reference: \n"    \
"[PCB08] Nicolas Passat, Michel Couprie, and Gilles Bertrand:\n"    \
"\"Minimal simple pairs in the 3D cubic grid\", in\n"    \
"Journal of Mathematical Imaging and Vision, 32 (3), pp.&nbsp;239‚Äì249, November 2008.\n"    \
"\n"    \
"Types supported: byte 3d\n"    \
"\n"    \
"Category: topobin\n"    \
"\n"    \
"\n"    \
" Michel Couprie 2008"



#define doc__skel2graph__c__ \
"Usage: skel2graph in.skel out.graph\n"    \
"\n"    \
"Description:\n"    \
"Generation of a graph from a curvilinear skeleton.\n"    \
"\n"    \
"Types supported: 2Dskel, 3Dskel\n"    \
"\n"    \
"Category: topobin\n"    \
"\n"    \
"\n"    \
" Michel Couprie 2009"



#define doc__skel2pgm__c__ \
"Usage: skel2pgm in.skel [id] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Generation of a binary image from a curvilinear skeleton.\n"    \
"\n"    \
"If the optional argument  id is given, then only the skeleton element having this index is considered.\n"    \
"\n"    \
"Types supported: 2Dskel, 3Dskel\n"    \
"\n"    \
"Category: topobin\n"    \
"\n"    \
"\n"    \
" Michel Couprie 2004"



#define doc__skel2pov__c__ \
"Usage: skel2pov in.skel out.pov\n"    \
"\n"    \
"Description:\n"    \
"Generation of a 3d illustration from a curvilinear skeleton.\n"    \
"\n"    \
"Types supported: 2Dskel, 3Dskel\n"    \
"\n"    \
"Category: topobin\n"    \
"\n"    \
"\n"    \
" Michel Couprie 2004"



#define doc__skel_ACK3__c__ \
"Usage: skel_ACK3 in.pgm nsteps [inhibit] out.pgm\n"    \
"\n"    \
"Description: Parallel 3D binary thinning or curvilinear,\n"    \
"asymetric skeleton based on 3D and 2D residuals. The parameter  nsteps\n"    \
"gives, if positive, the number of parallel thinning steps to be\n"    \
"processed.  If the value given for  nsteps equals -1, the thinning\n"    \
"is continued until stability.\n"    \
"\n"    \
"If the parameter  inhibit is given and is a binary image name,\n"    \
"then the points of this image will be left unchanged. \n"    \
"\n"    \
"Warning: The object must not have any point on the frame of the image.\n"    \
"\n"    \
"Types supported: byte 3d\n"    \
"\n"    \
"Category: topobin\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__skel_ACK3a__c__ \
"Usage: skel_ACK3a in.pgm nsteps ndel [inhibit] out.pgm\n"    \
"\n"    \
"Description: Parallel 3D binary thinning or curvilinear,\n"    \
"asymetric skeleton based on thin 1D isthmus. The parameter  nsteps\n"    \
"gives, if positive, the number of parallel thinning steps to be\n"    \
"processed.  If the value given for  nsteps equals -1, the thinning\n"    \
"is continued until stability.\n"    \
"\n"    \
"During the first  ndel steps, detected curve points are marked: \n"    \
"they are indicated by a value 127 (instead of 255) in the output image.\n"    \
"\n"    \
"If the parameter  inhibit is given and is a binary image name,\n"    \
"then the points of this image will be left unchanged. \n"    \
"\n"    \
" When  ndel is not set to 0, the output image may contain points with value 127.\n"    \
"\n"    \
" The object must not have any point on the frame of the image.\n"    \
"\n"    \
"Types supported: byte 3d\n"    \
"\n"    \
"Category: topobin\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__skel_AK2__c__ \
"Usage: skel_AK2 in.pgm nsteps [inhibit] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Parallel 2D binary thinning or curvilinear skeleton. The parameter  nsteps gives,\n"    \
"if positive, the number of parallel thinning steps to be processed.\n"    \
"If the value given for  nsteps equals -1, the thinning is continued\n"    \
"until stability.\n"    \
"\n"    \
"If the parameter  inhibit is given and is a binary image name,\n"    \
"then the points of this image will be left unchanged. \n"    \
"\n"    \
"Warning: The object must not have any point on the frame of the image.\n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: topobin\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__skel_CK3__c__ \
"Usage: skel_CK3 in.pgm nsteps [inhibit] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Parallel 3D binary thinning or curvilinear skeleton. The parameter  nsteps gives,\n"    \
"if positive, the number of parallel thinning steps to be processed.\n"    \
"If the value given for  nsteps equals -1, the thinning is continued\n"    \
"until stability.\n"    \
"\n"    \
"If the parameter  inhibit is given and is a binary image name,\n"    \
"then the points of this image will be left unchanged. \n"    \
"\n"    \
"Warning: The object must not have any point on the frame of the image.\n"    \
"\n"    \
"Types supported: byte 3d\n"    \
"\n"    \
"Category: topobin\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__skel_EK3__c__ \
"Usage: skel_EK3 in.pgm nsteps [inhibit] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Parallel 3D binary thinning or curvilinear skeleton based on ends. \n"    \
"The parameter  nsteps gives,\n"    \
"if positive, the number of parallel thinning steps to be processed.\n"    \
"If the value given for  nsteps equals -1, the thinning is continued\n"    \
"until stability.\n"    \
"\n"    \
"If the parameter  inhibit is given and is a binary image name,\n"    \
"then the points of this image will be left unchanged. \n"    \
"\n"    \
"Warning: The object must not have any point on the frame of the image.\n"    \
"\n"    \
"Types supported: byte 3d\n"    \
"\n"    \
"Category: topobin\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__skel_MK2__c__ \
"Usage: skel_MK2 in.pgm nsteps [inhibit] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Parallel 2D binary thinning or ultimate skeleton. The parameter  nsteps gives,\n"    \
"if positive, the number of parallel thinning steps to be processed.\n"    \
"If the value given for  nsteps equals -1, the thinning is continued\n"    \
"until stability.\n"    \
"\n"    \
"If the parameter  inhibit is given and is a binary image name,\n"    \
"then the points of this image will be left unchanged. \n"    \
"\n"    \
"Warning: The object must not have any point on the frame of the image.\n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: topobin\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__skel_MK3__c__ \
"Usage: skel_MK3 in.pgm nsteps [inhibit] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Parallel 3D binary thinning or ultimate skeleton. The parameter  nsteps gives,\n"    \
"if positive, the number of parallel thinning steps to be processed.\n"    \
"If the value given for  nsteps equals -1, the thinning is continued\n"    \
"until stability.\n"    \
"\n"    \
"If the parameter  inhibit is given and is a binary image name,\n"    \
"then the points of this image will be left unchanged. \n"    \
"\n"    \
"Warning: The object must not have any point on the frame of the image.\n"    \
"\n"    \
"Types supported: byte 3d\n"    \
"\n"    \
"Category: topobin\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__skel_NK2__c__ \
"Usage: skel_NK2 in.pgm nsteps [inhibit] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Parallel 2D binary thinning or curvilinear skeleton. The parameter  nsteps gives,\n"    \
"if positive, the number of parallel thinning steps to be processed.\n"    \
"If the value given for  nsteps equals -1, the thinning is continued\n"    \
"until stability.\n"    \
"\n"    \
"If the parameter  inhibit is given and is a binary image name,\n"    \
"then the points of this image will be left unchanged. \n"    \
"\n"    \
"Warning: The object must not have any point on the frame of the image.\n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: topobin\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__skelcurv__c__ \
"Usage: skelcurv in.pgm prio connex [inhibit] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Curvilinear binary skeleton guided by a priority image.\n"    \
"The lowest values of the priority image correspond to the highest priority.\n"    \
"\n"    \
"The parameter  prio is either an image (byte or int32_t or float or double), or a numerical code\n"    \
"indicating that a distance map will be used as a priority image; \n"    \
"the possible choices are:\n"    \
" 0: approximate euclidean distance\n"    \
" 1: approximate quadratic euclidean distance\n"    \
" 2: chamfer distance\n"    \
" 3: exact quadratic euclidean distance\n"    \
" 4: 4-distance in 2d\n"    \
" 8: 8-distance in 2d\n"    \
" 6: 6-distance in 3d\n"    \
" 18: 18-distance in 3d\n"    \
" 26: 26-distance in 3d\n"    \
"\n"    \
"The parameter  connex indicates the connectivity of the binary object.\n"    \
"Possible choices are 4, 8 in 2d and 6, 26 in 3d.\n"    \
"\n"    \
"If the parameter  inhibit is given and is a binary image name,\n"    \
"then the points of this image will be left unchanged. \n"    \
"\n"    \
"Let X be the set corresponding to the input image  in.pgm.\n"    \
"Let P be the function corresponding to the priority image.\n"    \
"Let I be the set corresponding to the inhibit image, if given, or the empty\n"    \
"set otherwise.\n"    \
"The algorithm is the following:\n"    \
"\n"    \
" @verbatim \n"    \
"C = null image\n"    \
"Repeat until stability\n"    \
"  choose a point x in X  I, simple for X, such that C[x] == 0 \n"    \
"    and such that P[x] is minimal\n"    \
"  X = X  {x}\n"    \
"  for all y in gamma(x)\n"    \
"    if T(y) > 1 then C[y] = 1\n"    \
"Result: X\n"    \
" T(y) refers to the connectivity number of y, that is, \n"    \
"(informally) the number of connected components of gamma(y) inter (X  {y}).\n"    \
"\n"    \
"References: \n"    \
"[BC07] G. Bertrand and M. Couprie: \"Transformations topologiques discretes\", in G&eacute;om&eacute;trie discr&egrave;te et images num&eacute;riques, D. Coeurjolly and A. Montanvert and J.M. Chassery, pp.&nbsp;187-209, Herm&egrave;s, 2007.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: topobin\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__skelend__c__ \
"Usage: skelend in.pgm connex [n] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Homotopic skeletonization by iterative removal of simple,\n"    \
"non-end points. Breadth-first strategy.\n"    \
"During the first  n iterations (default 0), the end points\n"    \
"are removed as well.\n"    \
"If  n = -1, the end points are always removed.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: topobin\n"    \
"\n"    \
"\n"    \
" This operator is very sensitive to small contour irregularities. \n"    \
"See skelcurv for a more robust operator.\n"    \
"\n"    \
" Michel Couprie"



#define doc__skeleton__c__ \
"Usage: skeleton in.pgm prio connex [inhibit] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Ultimate binary skeleton guided by a priority image.\n"    \
"The lowest values of the priority image correspond to the highest priority.\n"    \
"\n"    \
"The parameter  prio is either an image (byte, int32_t, float or double), or a numerical code\n"    \
"indicating that a distance map will be used as a priority image; \n"    \
"the possible choices are:\n"    \
" 0: approximate euclidean distance\n"    \
" 1: approximate quadratic euclidean distance\n"    \
" 2: chamfer distance\n"    \
" 3: exact quadratic euclidean distance\n"    \
" 4: 4-distance in 2d\n"    \
" 8: 8-distance in 2d\n"    \
" 6: 6-distance in 3d\n"    \
" 18: 18-distance in 3d\n"    \
" 26: 26-distance in 3d\n"    \
"\n"    \
"The parameter  connex indicates the connectivity of the binary object.\n"    \
"Possible choices are 4, 8 in 2d and 6, 26 in 3d.\n"    \
"\n"    \
"If the parameter  inhibit is given and is an integer different from -1,\n"    \
"then the points which correspond to this priority value will be left unchanged. \n"    \
"If the parameter  inhibit is given and is a binary image name,\n"    \
"then the points of this image will be left unchanged. \n"    \
"\n"    \
"Let X be the set corresponding to the input image  in.pgm.\n"    \
"Let P be the function corresponding to the priority image.\n"    \
"Let I be the set corresponding to the inhibit image, if given, or the empty\n"    \
"set otherwise.\n"    \
"The algorithm is the following:\n"    \
"\n"    \
" @verbatim \n"    \
"Repeat until stability\n"    \
"    Select a point x in X  I such that P[x] is minimal\n"    \
"    If x is simple for X then\n"    \
"        X = X  {x}\n"    \
"Result: X\n"    \
" \n"    \
"[BC07] G. Bertrand and M. Couprie: \"Transformations topologiques discretes\", in G&eacute;om&eacute;trie discr&egrave;te et images num&eacute;riques, D. Coeurjolly and A. Montanvert and J.M. Chassery, pp.&nbsp;187-209, Herm&egrave;s, 2007.\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: topobin\n"    \
"\n"    \
"\n"    \
" Michel Couprie\n"    \
"\n"    \
"Example:\n"    \
"\n"    \
"skeleton circuit1 8 8 circuit1_skeleton\n"    \
"\n"    \
"\n"    \
"  \n"    \
"      \n"    \
"  \n"    \
"  circuit1\n"    \
"      circuit1_skeleton\n"    \
"  \n"    \
""



#define doc__skeleucl__c__ \
"Usage: skeleucl in.pgm connex [inhibit] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Euclidean binary skeleton.\n"    \
"\n"    \
"The parameter  connex indicates the connectivity of the binary object.\n"    \
"Possible choices are 4, 8 in 2d and 6, 26 in 3d.\n"    \
"\n"    \
"If the parameter  inhibit is given and is a binary image name,\n"    \
"then the points of this image (constraint set) will be left unchanged. \n"    \
"\n"    \
"This operator is usually used with a constraint set (parameter  inhibit) that is a subset of the exact Euclidean medial axis (see operator medialaxis). \n"    \
"\n"    \
"Here is an example using the whole medial axis as constraint set:\n"    \
"\n"    \
" @verbatim \n"    \
"medialaxis test.pgm 3 _1\n"    \
"threshold _1 1 _2\n"    \
"skeleucl test.pgm 8 _2 result.pgm\n"    \
" subsets are obtained by filtering the medial axis, either based on the ball radiuses, or based on the bisector angle (see operator bisector). Below is a script showing how to proceed:\n"    \
"\n"    \
" @verbatim \n"    \
"#!/bin/sh\n"    \
"USAGE=\"Usage: $0 in seuilR (in [1..infnty[) seuilA (in [0.001..pi]) out\"\n"    \
"if [ $# -ne 4 ]\n"    \
"then\n"    \
"	echo $USAGE\n"    \
"        exit\n"    \
"fi\n"    \
"medialaxis $1 3 /tmp/skel2_tmp_m\n"    \
"threshold /tmp/skel2_tmp_m 1 /tmp/skel2_tmp_m1\n"    \
"skeleucl $1 8 /tmp/skel2_tmp_m1 /tmp/skel2_tmp_s\n"    \
"threshold /tmp/skel2_tmp_m $2 /tmp/skel2_tmp_ms\n"    \
"distc $1 3 /tmp/skel2_tmp_d\n"    \
"bisector /tmp/skel2_tmp_d /tmp/skel2_tmp_ms /tmp/skel2_tmp_a\n"    \
"threshold /tmp/skel2_tmp_a $3 /tmp/skel2_tmp_i\n"    \
"skeleton /tmp/skel2_tmp_s /tmp/skel2_tmp_d 8 /tmp/skel2_tmp_i $4\n"    \
"rm -f /tmp/skel2_tmp_*\n"    \
" \n"    \
"[CCZ07] M. Couprie, D. Coeurjolly and R. Zrour: \"Discrete bisector function and Euclidean skeleton in 2D and 3D\", Image and Vision Computing, Vol.&nbsp;25, No.&nbsp;10, pp.&nbsp;1543-1556, 2007.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: topobin\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__skelfindelbows__c__ \
"Usage: skelfindelbows in.skel len sharp out.pgm\n"    \
"\n"    \
"Description:\n"    \
"\n"    \
"Find \"elbows\" (points making sharp angles) in the curves of skeleton  in.skel.\n"    \
"Matching points are written as voxels in the returned image  out.pgm.\n"    \
"\n"    \
" @verbatim \n"    \
"Let  be the points of the curve C. \n"    \
"Let j be an index between len and n-1-len, \n"    \
"let i = j - len, let k = j + len.\n"    \
"If angle(C[j]-C[i], C[j]-C[k])    Types supported: skel 2d, skel 3d\n"    \
"\n"    \
"Category: topobin\n"    \
"\n"    \
"\n"    \
" Michel Couprie 2009"



#define doc__skelpar__c__ \
"Usage: skelpar in.pgm algorithm nsteps [inhibit] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Parallel binary thinning or skeleton. The parameter  nsteps gives,\n"    \
"if positive, the number of parallel thinning steps to be processed.\n"    \
"If the value given for  nsteps equals -1, the thinning is continued\n"    \
"until stability.\n"    \
"\n"    \
"The parameter  algorithm is a numerical code\n"    \
"indicating which method will be used for the thinning.\n"    \
"The possible choices are:\n"    \
" 0: Pavlidis 1981\n"    \
" 1: Eckhardt and Maderlechner 1993\n"    \
" 2: Couprie and Bertrand, ultimate (MK2)\n"    \
" 3: Couprie and Bertrand, curvilinear\n"    \
" 4: Couprie and Bertrand, curvilinear with reconstruction property (AK2)\n"    \
" 5: Rutovitz 1966 [DOES NOT PRESERVE TOPOLOGY]\n"    \
" 6: Zhang and Wang 1988 [DOES NOT PRESERVE TOPOLOGY]\n"    \
" 7: Han, La and Rhee\n"    \
" 8: Guo and Hall 1992 (1)\n"    \
" 9: Guo and Hall 1992 (2)\n"    \
" 10: Guo and Hall 1992 (3)\n"    \
" 11: Chin and Wan and Stover and Iverson 1987\n"    \
" 12: Jang and Chin 1992 [DOES NOT PRESERVE TOPOLOGY]\n"    \
" 13: Jang and Chin (correction mc)\n"    \
" 14: MNS preservation\n"    \
" 15: Jang and Chin 1993 (reconstruction)\n"    \
" 16: Choy, Choy and Siu 1995 [DOES NOT PRESERVE TOPOLOGY]\n"    \
" 17: Bernard and Manzanera 1999\n"    \
" 18: Holt et al. 1987\n"    \
" 19: Hall 1989\n"    \
" 20: Wu and Tsai 1992 [DOES NOT PRESERVE TOPOLOGY]\n"    \
" 21: Manzanera and Bernard (variant by GB)\n"    \
" 22: Couprie and Bertrand, ultimate with medial axis constraint (NK2)\n"    \
" 23: Bertrand curvilinear with reconstruction, asymmetrical\n"    \
" 24: Bertrand curvilinear with reconstruction, symmetrical\n"    \
" 25: Rosenfeld directionnel\n"    \
" 26: Nemeth et Palagyi, 2009 (1)\n"    \
" 27: Nemeth et Palagyi, 2009 (2)\n"    \
" 28: Nemeth et Palagyi, 2009 (3)\n"    \
"\n"    \
"If the parameter  inhibit is given and is a binary image name,\n"    \
"then the points of this image will be left unchanged.\n"    \
"\n"    \
"Warning: The object must not have any point on the frame of the image.\n"    \
"\n"    \
"References:\n"    \
"[COU05] M. Couprie, Note on fifteen 2D parallel thinning algorithms,\n"    \
"IGM2006-01}, Universit de Marne-la-Vall 2006,\n"    \
"http://igm.univ-mlv.fr/LabInfo/rapportsInternes/2006/01.pdf\n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: topobin\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__skelpar3d__c__ \
"Usage: skelpar3d in.pgm algorithm nsteps [inhibit] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Parallel 3D binary thinning or skeleton. The parameter  nsteps gives,\n"    \
"if positive, the number of parallel thinning steps to be processed.\n"    \
"If the value given for  nsteps equals -1, the thinning is continued\n"    \
"until stability.\n"    \
"\n"    \
"The parameter  algorithm is a numerical code\n"    \
"indicating which method will be used for the thinning.\n"    \
"The possible choices are:\n"    \
" 0: ultimate, without constraint (MK3a)\n"    \
" 1: curvilinear, based on 1D isthmus (CK3a)\n"    \
" 2: medial axis preservation (AK3) - parameter inhibit represents the minimal radius of medial axis balls which are considered\n"    \
" 3: ultimate (MK3) - if nsteps = -2, returns the topological distance\n"    \
" 4: curvilinear based on ends (EK3)\n"    \
" 5: curvilinear based on ends, with end reconstruction (CK3b)\n"    \
" 6: topological axis (not homotopic)\n"    \
" 7: curvilinear, based on residual points and 2D isthmus (CK3)\n"    \
" 8: ultimate, asymetric (AMK3)\n"    \
" 9: curvilinear, asymetric, based on thin 1D isthmus (ACK3a)\n"    \
" 10: curvilinear, asymetric, based on 3D and 2D residuals (ACK3)\n"    \
" 11: surfacic, based on residual points (RK3)\n"    \
" 12: surfacic, based on 2D isthmuses (SK3)\n"    \
" 13: ultimate, directional, (DK3)\n"    \
" 14: surfacic, directional, based on residual points (DRK3)\n"    \
" 15: surfacic, directional, based on 2D isthmuses (DSK3)\n"    \
"\n"    \
"In modes other than 2, if the parameter  inhibit is given and is a binary image name,\n"    \
"then the points of this image will be left unchanged. \n"    \
"\n"    \
"The following codes give access to auxiliary functions, for isthmus detection. \n"    \
"Parameter  nsteps will not be used in this case (any value can be given).\n"    \
"\n"    \
" 100: 1D isthmus points\n"    \
"\n"    \
"Warning: The object must not have any point on the frame of the image.\n"    \
"\n"    \
"Types supported: byte 3d\n"    \
"\n"    \
"Category: topobin\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__skelpar3d_others__cxx__ \
"Description:\n"    \
"Parallel 3D binary thinning or skeleton. The parameter  nsteps gives,\n"    \
"if positive, the number of parallel thinning steps to be processed.\n"    \
"If the value given for  nsteps equals -1, the thinning is continued\n"    \
"until stability.\n"    \
"\n"    \
"The parameter  algorithm is a numerical code\n"    \
"indicating which method will be used for the thinning.\n"    \
"The possible choices are:\n"    \
" 0: Palagyi (curvilinear, directional, 1998 - 6-subiterations directional curve-thinning)\n"    \
" 1: Palagyi (curvilinear, sequential, 2006)\n"    \
" 2: Palagyi (surfacic, parallel directional, 2002)\n"    \
" 3: Palagyi (surfacic, fully parallel, 2008)\n"    \
"\n"    \
"If the parameter  inhibit is given and is a binary image name,\n"    \
"then the points of this image will be left unchanged.\n"    \
"\n"    \
"Types supported: byte 3d\n"    \
"\n"    \
"Category: topobin\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__skelsurf__c__ \
"Usage: skelsurf in.pgm prio connex [inhibit] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Surfacic binary skeleton guided by a priority image.\n"    \
"The lowest values of the priority image correspond to the highest priority.\n"    \
"\n"    \
"The parameter  prio is either an image (byte or int32_t), or a numerical code\n"    \
"indicating that a distance map will be used as a priority image; \n"    \
"the possible choices are:\n"    \
" 0: approximate euclidean distance\n"    \
" 1: approximate quadratic euclidean distance\n"    \
" 2: chamfer distance\n"    \
" 3: exact quadratic euclidean distance\n"    \
" 6: 6-distance in 3d\n"    \
" 18: 18-distance in 3d\n"    \
" 26: 26-distance in 3d\n"    \
"\n"    \
"The parameter  connex indicates the connectivity of the binary object.\n"    \
"Possible choices are 6, 26.\n"    \
"\n"    \
"If the parameter  inhibit is given and is a binary image name,\n"    \
"then the points of this image will be left unchanged. \n"    \
"\n"    \
"Let X be the set corresponding to the input image  in.pgm.\n"    \
"Let P be the function corresponding to the priority image.\n"    \
"Let I be the set corresponding to the inhibit image, if given, or the empty\n"    \
"set otherwise.\n"    \
"The algorithm is the following:\n"    \
"\n"    \
" @verbatim \n"    \
"C = {y in F | Tb(y) > 1}\n"    \
"Repeat until stability\n"    \
"  choose a point x in X, simple for X, such that C[x] == 0 \n"    \
"    and such that P[x] is minimal\n"    \
"  X = X  {x}\n"    \
"  for all y in gamma(x)\n"    \
"    if Tb(y) > 1 then C[y] = 1\n"    \
"Result: X\n"    \
" Tb(y) refers to the second connectivity number of y, that is, \n"    \
"(informally) the number of connected components of gamma(y) inter complement(X).\n"    \
"\n"    \
"References: \n"    \
"[BC07] G. Bertrand and M. Couprie: \"Transformations topologiques discretes\", in G&eacute;om&eacute;trie discr&egrave;te et images num&eacute;riques, D. Coeurjolly and A. Montanvert and J.M. Chassery, pp.&nbsp;187-209, Herm&egrave;s, 2007.\n"    \
"\n"    \
"Types supported: byte 3d\n"    \
"\n"    \
"Category: topobin\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__skelvertex__c__ \
"Usage: skelvertex in.skel vertex_id out.pgm\n"    \
"\n"    \
"Description:\n"    \
"\n"    \
"In the output image  out.pgm, only the pixels/voxels of the skeleton element\n"    \
"corresponding to  vertex_id are present.\n"    \
"\n"    \
"Types supported: skel 2d, skel 3d\n"    \
"\n"    \
"Category: topobin\n"    \
"\n"    \
"\n"    \
" Michel Couprie 2009"



#define doc__skew__c__ \
"WRITE ME!!!"



#define doc__souzabanon__c__ \
"WRITE ME!!!"



#define doc__split__c__ \
"WRITE ME!!!"



#define doc__squel__c__ \
"WRITE ME!!!"



#define doc__squelbin__c__ \
"WRITE ME!!!"



#define doc__squelval__c__ \
"WRITE ME!!!"



#define doc__standarddeviation1__c__ \
"Usage: standarddeviation1 in.pgm [mask.pgm] out.list\n"    \
"\n"    \
"Description:\n"    \
"This function returns (in the list out.list) \n"    \
"the standard deviation of the pixel values of the image  in.pgm .\n"    \
"If the optional parameter  mask.pgm is given, then only the \n"    \
"values which correspond to non-null points of mask are considered.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n"    \
"\n"    \
"Category: arith\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__stretch__c__ \
"WRITE ME!!!"



#define doc__sub__c__ \
"Usage: sub in1.pgm in2.pgm out.pgm\n"    \
"\n"    \
"Description:\n"    \
"For each pixel x, out[x] = in1[x] - in2[x]. \n"    \
"For byte and int32_t image types, if out[x] Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n"    \
"\n"    \
"Category: arith\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__sup__c__ \
"Usage: sup in1.pgm in2.pgm out.pgm\n"    \
"\n"    \
"Description:\n"    \
"For each pixel x, if in1[x] Types supported: byte 2D, byte 3D\n"    \
"\n"    \
"Category: arith\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__surfacerestoration__c__ \
"WRITE ME!!!"



#define doc__surfels__c__ \
"WRITE ME!!!"



#define doc__surimp__c__ \
"Usage: surimp i1.ndg i2.bin out.ppm\n"    \
"\n"    \
"Description: Superimposes the binary image i2.bin (in red) to the \n"    \
"grayscale image i1.bin. The result is stored as a color image out.ppm.\n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: convert\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__surrect__c__ \
"WRITE ME!!!"



#define doc__sym__c__ \
"Usage: sym in.pgm type out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Symmetry (vertical, horizontal, central in 2D; x, y, z in 3D).\n"    \
"The parameter type is set to v or y (vertical), \n"    \
"h or x (horizontal), or c (central) for 2D images, \n"    \
"or to x, y or z for 3D images.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d.\n"    \
"\n"    \
"Category: geo\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__symse__c__ \
"Usage: symse in.pgm out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Inverse of a structuring element.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d.\n"    \
"\n"    \
"Category: geo\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__t26pp__c__ \
"WRITE ME!!!"



#define doc__t4pp__c__ \
"WRITE ME!!!"



#define doc__t8pp__c__ \
"WRITE ME!!!"



#define doc__taf__c__ \
"Usage: taf in.pgm connexmin r [lambdap [lambdaw]] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Topological alternating filter. Performs alternatively topological upper filter (tuf)\n"    \
"and topological lower filter (tlf) with increasing radius until radius r.\n"    \
"\n"    \
"If given, parameter lambdap (resp. lambdaw) is an integer which \n"    \
"indicates that peaks (resp. wells) of height greater than this value must be \n"    \
"preserved. \n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: topogray\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__temp3dsimple__c__ \
"WRITE ME!!!"



#define doc__tempNd__c__ \
"WRITE ME!!!"



#define doc__threshold__c__ \
"Usage: threshold in.pgm n [n2] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"If n2 is not present, for each pixel x, out[x] = if (in[x] Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n"    \
"\n"    \
"Category: arith\n"    \
"\n"    \
"\n"    \
" Michel Couprie 2010"



#define doc__tlf__c__ \
"Usage: tlf in.pgm connexmin r out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Topological lower filter. Performs the homotopic thickening controlled by\n"    \
"a radius r, followed by a well deletion, and a homotopic reconstruction over\n"    \
"the original image.\n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: topogray\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__toposhrink__c__ \
"Usage: toposhrink in.pgm prio.pgm connex tmin tmax tbmin tbmax [inhibit] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Topologically controled binary shrinking guided by a priority image.\n"    \
"The lowest values of the priority image correspond to the highest priority.\n"    \
"\n"    \
"The parameter  prio is an image (byte or int32_t).\n"    \
"\n"    \
"The parameter  connex indicates the connectivity of the binary object.\n"    \
"Possible choices are 4, 8 in 2D and 6, 18, 26 in 3D.\n"    \
"\n"    \
"If the parameter  inhibit is given and is a binary image name,\n"    \
"then the points of this image (set Y) will be left unchanged. \n"    \
"\n"    \
"Let X be the set of points of the binary image  in.pgm .\n"    \
"\n"    \
" @verbatim \n"    \
"Repeat until stability: \n"    \
"    select a point p of X  Y such that\n"    \
"        tmin   supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: topobin\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__toposhrinkgray__c__ \
"Usage: toposhrinkgray in.pgm connex t+min t+max t--min t--max  [inhibit] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Topologically controled grayscale shrinking (one step).\n"    \
"\n"    \
"The parameter  indicates the action performed on the selected point p:\n"    \
" 0: it is lowered down to 0;\n"    \
" a: it is lowered down to alpha-(p,F) where F denotes the original image;\n"    \
" d: it is lowered down to delta-(p,F) where F denotes the original image.\n"    \
"\n"    \
"The parameter  connex indicates the connectivity of the binary object.\n"    \
"Possible choices are 4, 8 in 2D and 6, 18, 26 in 3D.\n"    \
"\n"    \
"If the parameter  inhibit is given and is a binary image name,\n"    \
"then the points of this image (set Y) will be left unchanged. \n"    \
"\n"    \
" @verbatim \n"    \
"  For all points p, not in Y, and such that \n"    \
"      t+min  supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: topogray\n"    \
"\n"    \
"\n"    \
"References:\n"    \
"\n"    \
"[BEC97] G. Bertrand, J. C. Everat and M. Couprie: \"Image segmentation through operators based upon topology\",  Journal of Electronic Imaging, Vol.&nbsp;6, No.&nbsp;4, pp.&nbsp;395-405, 1997.\n"    \
"\n"    \
"[CBB01] M. Couprie, F.N. Bezerra, Gilles Bertrand: \"Topological operators for\n"    \
"grayscale image processing\",  Journal of Electronic Imaging, Vol.&nbsp;10, No.&nbsp;4, pp.&nbsp;1003-1015, 2001.\n"    \
"\n"    \
" Nicolas Combaret 2006"



#define doc__topotypes__c__ \
"WRITE ME!!!"



#define doc__traceellipses__c__ \
"WRITE ME!!!"



#define doc__tracevect__c__ \
"WRITE ME!!!"



#define doc__translate__c__ \
"Usage: in.pgm ox oy oz [mode] out.ppm\n"    \
"\n"    \
"Description:\n"    \
"Translates the image by the offsets  ox,  oy,  oz in the directions\n"    \
"x, y, z respectively. Offsets may be negative or positive integers.\n"    \
"If  mode is 0, the points that get out of the image frame are lost. \n"    \
"If  mode is 1, a torical space is simulated in all dimensions. \n"    \
"Default mode is 0.\n"    \
"\n"    \
"Types supported: byte 2d, byte3d\n"    \
"\n"    \
"Category: geo\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__tuf__c__ \
"Usage: tuf in.pgm connexmin r out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Topological upper filter. Performs the homotopic thinning controlled by\n"    \
"a radius r, followed by a peak deletion, and a homotopic reconstruction under\n"    \
"the original image.\n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: topogray\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__ultimateerosion__c__ \
"Usage: ultimateerosion in.pgm [dist] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Let X be the set in  in.pgm .\n"    \
"The result is union{Ui(X), i in N} where\n"    \
"Ui(X) = erosball(X,i)  reconsgeo(erosball(X,i+1), erosball(X,i)). \n"    \
"Structuring elements are balls defined after a distance.\n"    \
"The distance used depends on the optional parameter  dist (default is 0) :\n"    \
" 0: approximate euclidean distance (truncated)\n"    \
" 1: approximate quadratic euclidean distance\n"    \
" 2: chamfer distance\n"    \
" 3: exact quadratic euclidean distance\n"    \
" 4: 4-distance in 2d\n"    \
" 8: 8-distance in 2d\n"    \
" 6: 6-distance in 3d\n"    \
" 18: 18-distance in 3d\n"    \
" 26: 26-distance in 3d\n"    \
"\n"    \
" The input image  in.pgm must be a binary image. No test is done.\n"    \
"\n"    \
"Types supported: byte 2D, byte 3D\n"    \
"\n"    \
"Category: morpho\n"    \
"\n"    \
"\n"    \
" Michel Couprie ao√ªt 2009"



#define doc__ungerflow__cpp__ \
"WRITE ME!!!"



#define doc__variance1__c__ \
"Usage: variance1 in.pgm [mask.pgm] out.list\n"    \
"\n"    \
"Description:\n"    \
"This function returns (in the list out.list) \n"    \
"the variance of the pixel values of the image  in.pgm .\n"    \
"If the optional parameter  mask.pgm is given, then only the \n"    \
"values which correspond to non-null points of mask are considered.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n"    \
"\n"    \
"Category: signal stats\n"    \
" \n"    \
"\n"    \
" Michel Couprie"



#define doc__visu3d__c__ \
"WRITE ME!!!"



#define doc__volmaxima__c__ \
"Usage: volmaxima in.pgm connex vol out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Volume-based filtering with connexity connex and volume vol.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: connect\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__volselnb__c__ \
"Usage: volselnb in.pgm connex nb out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Segmentation by selection of a given number or components, based on a \n"    \
"volume criterion. \n"    \
"Connexity = connex ; desired number of components = nb.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: connect\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__volume__c__ \
"Usage: volume in.pgm out.list\n"    \
"\n"    \
"Description:\n"    \
"This function returns (in the list out.list) \n"    \
"the sum of the pixel values of the image  in.pgm .\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n"    \
"\n"    \
"Category: arith\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__volumelignes__c__ \
"Usage: volumelignes skel.pgm orig.pgm connex radius out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Enhances linear structures in a grayscale image.\n"    \
"The algorithm is the following:\n"    \
"\n"    \
" @verbatim \n"    \
"Let F be the original image (\"orig.pgm\")\n"    \
"Let O be the final result (\"out.pgm\")\n"    \
"Let r be the value of parameter \"radius\"\n"    \
"Compute k iterations of homotopic thinning, result S (\"skel.pgm\")\n"    \
"For each point x of S which is separating\n"    \
"  O(x) = volume((F-S).Br(x)) \n"    \
"  where Br(x) is the disc centered on x with radius r\n"    \
"EndFor\n"    \
" \n"    \
"\"Topological operators for the detection of\n"    \
"curvilinar structures in grayscale images\", in preparation, 2003.\n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: topogray\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__voronoilabelling__c__ \
"Usage: voronoilabelling in.pgm [dist.pgm] vor.pgm\n"    \
"\n"    \
"Description:\n"    \
"\n"    \
"Types supported: byte 2d,  byte 3d\n"    \
"\n"    \
"Category: morpho"



#define doc__warp__c__ \
"WRITE ME!!!"



#define doc__waterfall__c__ \
"WRITE ME!!!"



#define doc__watershed__c__ \
"Usage: watershed in.pgm mark.pgm connex [i] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Topological watershed as defined in [CB97] - connectivity connex.\n"    \
"A marker image is supplied in mark.pgm. The result is a binary image.\n"    \
"If the parameter i is given, then the dual operator is applied.\n"    \
"\n"    \
"[CB97] M. Couprie and G. Bertrand:\n"    \
"``Topological Grayscale Watershed Transformation'',\n"    \
"SPIE Vision Geometry V Proceedings, 3168 (136--146), 1997.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: connect\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__watershedMeyer__c__ \
"Usage: watershedMeyer in mark   connex out\n"    \
"\n"    \
"Description:\n"    \
"Performs the watershed transformation on the image in, taking the\n"    \
"set of markers in mark. \n"    \
"If this parameter is present, bgmark\n"    \
"is used as a set of markers for the background.\n"    \
"If this parameter is present, roi\n"    \
"indicates the region of interest on which the operation is performed.\n"    \
"The parameter connex gives the adjacency relation (4,8 in 2D; 6,18,26 in 3D) \n"    \
"for the makers.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: connect\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__watershedMeyer2__c__ \
"Usage: watershedMeyer2 in mark  connex out\n"    \
"\n"    \
"Description:\n"    \
"Performs the watershed transformation on the image in, taking the\n"    \
"set of markers in mark, in the form of a label image where all the pixels\n"    \
"sharing the same label (even if not connected) belong to the same marker.\n"    \
"If this parameter is present, roi\n"    \
"indicates the region of interest on which the operation is performed.\n"    \
"The parameter connex gives the adjacency relation (4,8 in 2D; 6,18,26 in 3D) \n"    \
"for the makers.\n"    \
"\n"    \
"The result is a label image.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: connect\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__watershedMeyer2lab__c__ \
"Usage: watershedMeyer2lab in mark  connex [mode] out\n"    \
"\n"    \
"Description:\n"    \
"Performs the watershed transformation on the image in, taking the\n"    \
"set of markers in mark, in the form of a label image where all the pixels\n"    \
"sharing the same label (even if not connected) belong to the same marker.\n"    \
"If this parameter is present, roi\n"    \
"indicates the region of interest on which the operation is performed.\n"    \
"The parameter connex gives the adjacency relation (4,8 in 2D; 6,18,26 in 3D) \n"    \
"for the makers.\n"    \
"\n"    \
"The result is a label image. If the original markers are labelled by\n"    \
"numbers 1,...,n then the regions (catchment basins) of the result will\n"    \
"be labelled with the same numbers.\n"    \
"\n"    \
"If the optional parameter  mode is 0 (default value), then the\n"    \
"separation (watershed) will be labelled with n+1. Otherwise, a\n"    \
"separating point that is neighbour of exactly two regions i and j will be\n"    \
"labelled by i*j+n, and a separating point that is neighbour of more\n"    \
"than two regions be labelled by n+1.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: connect\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__watershedMeyer3__c__ \
"Usage: watershedMeyer3 in mark  connex out\n"    \
"\n"    \
"Description:\n"    \
"Performs the watershed transformation without separation lines, on the image in, taking the\n"    \
"set of markers in mark, in the form of a label image where all the pixels\n"    \
"sharing the same label (even if not connected) belong to the same marker.\n"    \
"If this parameter is present, roi\n"    \
"indicates the region of interest on which the operation is performed.\n"    \
"The parameter connex gives the adjacency relation (4,8 in 2D; 6,18,26 in 3D) \n"    \
"for the makers.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: connect\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__watershedMeyer4D__c__ \
"Usage: watershedMeyer4D prefix_in first last prefix_mark   connex prefix_out\n"    \
"\n"    \
"Description:\n"    \
"Performs the watershed transformation on the 4D image made of the concatenation of the 3D images prefix_inxxxx.pgm (where nnnn is a four digit decimal integer and where first and last specify respectively the first and the last volume).The watershed is computed taking the\n"    \
"set of markers in prefix_markxxxx.pgm. \n"    \
"If this parameter is present, prefix_bgmarkxxxx.pgm\n"    \
"is used as a set of markers for the background.\n"    \
"If this parameter is present, prefix_roixxxx.pgm\n"    \
"indicates the region of interest on which the operation is performed.\n"    \
"The parameter connex gives the adjacency relation (8 in 4D) \n"    \
"for the makers.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: connect\n"    \
"\n"    \
"\n"    \
" Jean Cousty - fevrier 2005"



#define doc__watershedMeyer_regular__c__ \
"WRITE ME!!!"



#define doc__watershedthin__c__ \
"Usage: watershedthin in mark   parite out\n"    \
"\n"    \
"Description:\n"    \
"\n"    \
"Performs the watershed transformation on the image in,\n"    \
"considered to be on a perfect fusion grid, taking the set of markers in\n"    \
"mark.  Returns the divide set.\n"    \
"\n"    \
"If this parameter is present, bgmark is used as a\n"    \
"set of markers for the background.  If this parameter is present,\n"    \
"roi indicates the region of interest on which the operation is\n"    \
"performed.  The parameter parite (0 or 1) gives the \n"    \
"the translation of the fusion grid.\n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: connect\n"    \
"\n"    \
"\n"    \
" Jean Cousty"



#define doc__watershedwithoutline__c__ \
"Usage: watershedwithoutline in mark   connex out\n"    \
"\n"    \
"Description:\n"    \
"Performs the watershed transformation on the image in, taking the\n"    \
"set of markers in mark. \n"    \
"If this parameter is present, bgmark\n"    \
"is used as a set of markers for the background.\n"    \
"If this parameter is present, roi\n"    \
"indicates the region of interest on which the operation is performed.\n"    \
"The parameter connex gives the adjacency relation (4,8 in 2D; 6,18,26 in 3D) \n"    \
"for the makers.\n"    \
"\n"    \
"The image out is a label image (int32_t)\n"    \
"\n"    \
"This version does not create any line to separate the catchment basins. \n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: connect\n"    \
"\n"    \
"\n"    \
" Michel Couprie and Laurent Najman"



#define doc__watershedwithoutlinelab__c__ \
"Usage: watershedwithoutlinelab in mark  connex out\n"    \
"\n"    \
"Description:\n"    \
"Performs the watershed transformation on the image in, taking the\n"    \
"labelled marker in mark. \n"    \
"If this parameter is present, roi\n"    \
"indicates the region of interest on which the operation is performed.\n"    \
"The parameter connex gives the adjacency relation (4,8 in 2D; 6,18,26 in 3D) \n"    \
"for the makers.\n"    \
"\n"    \
"The image mark is a label image (int32_t)\n"    \
"\n"    \
"The image out is a label image (int32_t)\n"    \
"\n"    \
"This version does not create any line to separate the catchment basins. \n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: connect\n"    \
"\n"    \
"\n"    \
" Michel Couprie and Laurent Najman"



#define doc__worms__c__ \
"WRITE ME!!!"



#define doc__wshedkhalimsky__c__ \
"Usage: watershed in mark   out\n"    \
"\n"    \
"Description:\n"    \
"Performs the watershed transformation on the image in.pgm, taking the\n"    \
"set of markers in mark.pgm. \n"    \
"If this parameter is present, bgmark.pgm\n"    \
"is used as a set of markers for the background.\n"    \
"If this parameter is present, roi\n"    \
"indicates the region of interest on which the operation is performed.\n"    \
"All images must be previously transformed in the khalimsky space with a max strategy.\n"    \
"The output image is in khalimsky space too.\n"    \
"\n"    \
"Types supported: byte 2d\n"    \
"\n"    \
"Category: connect orders\n"    \
" \n"    \
"\n"    \
" Michel Couprie & Christophe Doublier"



#define doc__wshedtopo__c__ \
"Usage: wshedtopo in.pgm connex [i] out.pgm\n"    \
"\n"    \
"Description:\n"    \
"Topological grayscale watershed as defined in [CB97], preserves the minima - connectivity connex\n"    \
"If the parameter i is given, then the dual operator is applied.\n"    \
"\n"    \
"[CB97] M. Couprie and G. Bertrand:\n"    \
"``Topological Grayscale Watershed Transformation'',\n"    \
"SPIE Vision Geometry V Proceedings, 3168 (136--146), 1997.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: connect\n"    \
"\n"    \
"\n"    \
" Michel Couprie\n"    \
"\n"    \
"Example:\n"    \
"\n"    \
"wshedtopo ur1 4 ur1_wshedtopo\n"    \
"\n"    \
"\n"    \
"  \n"    \
"      \n"    \
"  \n"    \
"  ur1\n"    \
"      ur1_wshedtopo\n"    \
"  \n"    \
""



#define doc__wshedval__c__ \
"WRITE ME!!!"



#define doc__xor__c__ \
"Usage: xor in1.pgm in2.pgm out.pgm\n"    \
"\n"    \
"Description:\n"    \
"For each pixel x, out[x] = in1[x] xor in2[x]. \n"    \
"Images must be of the same type and same dimensions.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: arith\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__yuv2rgb__c__ \
"WRITE ME!!!"



#define doc__zoom__c__ \
"Usage: zoom in.pgm  out.pgm\n"    \
"\n"    \
"Description: \n"    \
"There are 3 modes, depending on the number of arguments.\n"    \
"\n"    \
"1 argument: the same scale factor f is applied to both dimensions \n"    \
"  x and y (and z in 3D)\n"    \
"\n"    \
"2 arguments: if the parameter x is used, \n"    \
"  followed by an integer number rs, the zoom factor f \n"    \
"  is computed by dividing rs by the rowsize of in.pgm.\n"    \
"  If the parameter y is used, followed by an integer number cs, \n"    \
"  f is computed by dividing cs by the colsize of in.pgm.\n"    \
"  If the parameter z is used, followed by an integer number ds, \n"    \
"  f is computed by dividing ds by the depth of in.pgm.\n"    \
"\n"    \
"3 arguments: different zoom factors fx, fy, fz\n"    \
"  are given for directions x, y, z.\n"    \
"\n"    \
"Types supported: byte 1d, byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d\n"    \
"\n"    \
"Category: geo\n"    \
"\n"    \
"\n"    \
" Michel Couprie"



#define doc__zoomint__c__ \
"Usage: zoomint in.pgm   [nofill] out.pgm\n"    \
"\n"    \
"Description: \n"    \
"The zoom factor(s) can be unique ( f ) of there can be one zoom factor\n"    \
"per image dimension ( fx,  fy,  fz ). In the case of a 2d image, set  fz\n"    \
"to 1.\n"    \
"The zoom parameter may be a positive or negative integer.\n"    \
"If it is positive, the image is expanded in the corresponding \n"    \
"direction. The pixels are replicated in order to fill all the space,\n"    \
"except if the parameter nofill is used.\n"    \
"If it is negative, the image is shrinked the corresponding \n"    \
"directions, by undersampling.\n"    \
"\n"    \
"Types supported: byte 2d, byte 3d\n"    \
"\n"    \
"Category: geo\n"    \
"\n"    \
"\n"    \
" Michel Couprie, Christophe Doublier"



#define doc__zoomrgb__c__ \
"WRITE ME!!!"



