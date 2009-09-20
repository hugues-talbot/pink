/*! \mainpage
 
  \section warning Avertissement

Copyright ESIEE (2009) 

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
knowledge of the CeCILL license and that you accept its terms.

  \section contributors Contributeurs

Michel Couprie<br>
Laurent Najman: localextrema, saliency<br>
Hugues Talbot: fmm <br>
Jean Cousty: redt 3d (reverse euclidean distance transform - algo de D. Coeurjolly), watershedthin, opérateurs sur les graphes d'arêtes (GA), forêts de poids min (MSF), waterfall, recalagerigide_translateplane<br>
Xavier Daragon: dist, distc (distance euclidienne quadratique 3D)<br>
André Vital Saude: radialopening, divers scripts tcl, hma<br>
Nicolas Combaret: toposhrinkgray, ptselectgray<br>
John Chaussard: lballincl, cropondisk, shrinkondisk<br>
Christophe Doublier: zoomint<br>
Hildegard Koehler: lintophat<br>
Cédric Allène: gettree, histolisse, labeltree, nbcomp, pgm2vtk, seuilauto<br>
Gu Jun: maxdiameter<br>
Sébastien Couprie: mcsplines.c<br>
Rita Zrour: medialaxis (axe médian euclidien exact - algo de Rémy-Thiel),
dist, distc (distance euclidienne quadratique exacte - algo de Saito-Toriwaki)<br>
Laurent Mercier: gestion d'un masque dans delaunay<br>

CODE UNDER FREE LICENCE INCLUDED

David Coeurjolly: lvoronoilabelling.c<br>
Dario Bressanini: mcpowell.c<br>
Andrew W. Fitzgibbon: lbresen.c<br>

  \section install Installation

\subsection linux Linux

Pour installer la bibliothèque sous Linux, tapez:
\verbatim
tar zxvf pink.tgz
mv Pinktmp Pink
cd Pink
makelin
\endverbatim

Les exécutables se trouvent dans Pink/linux/bin,
les scripts dans Pink/scripts et dans Pink/tcl
Mettez à jour votre variable $path en conséquence.
Par exemple (en csh): 

\verbatim
setenv PINK ~coupriem/Pink
set path=( $path $PINK/linux/bin $PINK/scripts $PINK/tcl )
\endverbatim

\subsection windows Windows

Installez tout d'abord <A HREF="http://www.mingw.org/">MinGW</A> (Minimalist GNU For Windows), 
ainsi que le programme "make" de la distribution MinGW (installation séparée).

Décompressez l'archive pink.tgz (par exemple avec WinZip ou WinRar).

Modifiez selon votre configuration, les fichiers "mgw.make" et "makemgw.bat".

Ouvrez une fenêtre DOS et mettez-vous dans le répertoire principal de Pink.

tapez: makemgw

Les exécutables se trouvent dans Pink/bin,
mettez à jour votre variable PATH en conséquence.
Par exemple :

\verbatim
set PATH=c:\Pink\bin;%PATH%
\endverbatim

Erreurs de compilation possibles et solutions:

message d'erreur contenant "M_PI"
cause: version MinGW obsolete
solution rapide: recopier le contenu du fichier Pink/tools/pinkconst.h
dans le fichier MinGW/include/math.h

message d'erreur contenant "uint8_t", "int8_t", "uint16_t", etc 
cause: version MinGW obsolete
solution rapide: recopier le contenu du fichier Pink/tools/pinktypes.h
dans le fichier MinGW/include/math.h

message d'erreur contenant "chrono"
cause: flag de mise au point oublié
solution rapide: chercher le "#define CHRONO" dans le fichier source et le retirer

\subsection associes Logiciels associés

Pour une utilisation optimale, les logiciels suivants doivent être
installés:<br><br>
<A HREF="http://www.doxygen.org">Doxygen</A><br>
<A HREF="http://dev.scriptics.com/">ActiveTcl 8.3</A><br>
<A HREF="http://public.kitware.com/VTK/">VTK</A><br>
<A HREF="http://www.mplayerhq.hu/design7/news.html">MPlayer</A><br>
<A HREF="http://www.gnuplot.info/">Gnuplot</A><br>


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
\defgroup mesh3d Three-dimensional meshing
*/

