// -*- mode: C; coding: utf-8 -*-
/*! \mainpage

  \section notice Notice 

Copyright ESIEE (2009-2011) 

m.couprie@esiee.fr

This software is an image processing library whose purpose is to be
used primarily for research and teaching.

This software is governed by the CeCILL  license under French law and
abiding by the rules of distribution of free software. You can  use, 
modify and/ or redistribute the software under the terms of the CeCILL
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info". 

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's author,  the holder of the
economic rights,  and the successive licensors  have only  limited
liability. 

In this respect, the user's attention is drawn to the risks associated
with loading,  using,  modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate,  and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or 
data to be ensured and,  more generally, to use and operate it in the 
same conditions as regards security. 

The fact that you are presently reading this means that you have had
knowledge of the CeCILL license. You don't have to accept the License,
but note that nothing other than this License grants you permission 
to use, propagate or modify any covered work.

\section credits Credits

<ul>
  <li> Code licensed under CeCILL license
  <ul> 
    <li> <b>Michel Couprie</b>
    <li> <b>Laurent Najman</b>: localextrema, saliency
    <li> <b>Hugues Talbot</b>: fmm 
    <li> <b>Jean Cousty</b>: redt 3d (reverse euclidean distance transform - algo de D. Coeurjolly), watershedthin, opérateurs sur les graphes d'arêtes (GA), forêts de poids min (MSF), waterfall, recalagerigide_translateplane
    <li> <b>Xavier Daragon</b>: dist, distc (distance euclidienne quadratique 3D)
    <li> <b>André Vital Saude</b>: radialopening, divers scripts tcl, hma
    <li> <b>Nicolas Combaret</b>: toposhrinkgray, ptselectgray
    <li> <b>John Chaussard</b>: lballincl, cropondisk, shrinkondisk
    <li> <b>Christophe Doublier</b>: zoomint
    <li> <b>Hildegard Koehler</b>: lintophat
    <li> <b>Cédric Allène</b>: gettree, histolisse, labeltree, nbcomp, pgm2vtk, seuilauto
    <li> <b>Gu Jun</b>: maxdiameter
    <li> <b>Sébastien Couprie</b>: mcsplines.c
    <li> <b>Rita Zrour</b>: medialaxis (axe médian euclidien exact - algo de Rémy-Thiel),
      dist, distc (distance euclidienne quadratique exacte - algo de Saito-Toriwaki)
    <li> <b>Laurent Mercier</b>: gestion d'un masque dans delaunay
    <li> <b>Laszlo Marak (ujoimro)</b>: continuous maximum flows, Unger flows, Python front-end
   </ul>
  <li> Code under different free software licenses
    <ul>
      <li> <b>David Coeurjolly</b>: lvoronoilabelling.c
      <li> <b>Dario Bressanini</b>: mcpowell.c
      <li> <b>Andrew W. Fitzgibbon</b>: lbresen.c
      <li> <b>Lilian Buzer</b>: lbdigitalline.cxx
   </ul>
</ul>
\section compiling Compiling

Pink can be compiled with two sets of tools. For the compilation with
dependency checking and Python front-end, visit the page \ref
compiling_cmake. For the classical compilation look at \ref
compiling_make. 

\section development Development
For developing Pink look at the \ref dev_conventions page. For
exporting functions in Python look at the \ref python_export page. 

\section dependencies Dependencies
For optimal use, the following packages should be installed:
<br><br>
<A HREF="http://www.python.org">Python</A><br>
<A HREF="http://www.doxygen.org">Doxygen</A><br>
<A HREF="http://dev.scriptics.com/">ActiveTcl 8.3</A><br>
<A HREF="http://public.kitware.com/VTK/">VTK</A><br>
<A HREF="http://www.mplayerhq.hu/design7/news.html">MPlayer</A><br>
<A HREF="http://www.gnuplot.info/">Gnuplot</A><br>

Note,
that the detailed dependencies can be found in file 'dependencies'.

<HR>
Michel Couprie  -  Professeur  -  ESIEE Paris
Laboratoire d'Informatique Gaspard-Monge, Université Paris-Est
ESIEE 2, Bd Blaise Pascal - B.P. 99
93162 Noisy-Le-Grand CEDEX
mail: m.couprie@esiee.fr 
url:  http://www.esiee.fr/~coupriem
tel:  01 45 92 66 88          fax:  01 45 92 66 99          

 */

/*!
\defgroup inter Interactive operators
 */

/*!
\defgroup arith Arithmetic operators
 */

/*!
\defgroup convert Format and type conversion 
*/

/*!
\defgroup morpho Mathematical morphology
*/

/*!
\defgroup connect Digital connectivity
*/

/*!
\defgroup topobin Digital topology (binary)
*/

/*!
\defgroup topogray Digital topology (grayscale)
*/

/*!
\defgroup orders Orders topology
*/

/*!
\defgroup geo Geometrical operators
*/

/*!
\defgroup draw Graphic primitives
*/

/*!
\defgroup histo Histogram-based operators
*/

/*!
\defgroup signal Signal processing
*/

/*!
\defgroup stats Statistics
*/

/*!
\defgroup mesh3d Three-dimensional meshing
*/

/*!
\defgroup development Development
*/


// LuM end of file
