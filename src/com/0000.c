/*! \mainpage
 
  \section warning Avertissement
 
Cette bibliothèque d'opérateurs de traitement d'images a été 
développée exclusivement à des fins d'enseignement, de recherche et d'expérimentation.
Elle ne doit en aucun cas être diffusée sans l'accord de:

\verbatim
Michel Couprie  -  Professeur  -  ESIEE-Paris
Laboratoire D'Informatique Gaspard-Monge, Université Paris-Est
ESIEE 2, Bd Blaise Pascal - B.P. 99
93162 Noisy-Le-Grand CEDEX
mail: m.couprie@esiee.fr 
url:  http://www.esiee.fr/~coupriem
tel:  01 45 92 66 88          fax:  01 45 92 66 99          
\endverbatim

  \section contributors Contributeurs

Michel Couprie<br>
Laurent Najman: localextrema, saliency<br>
Xavier Daragon: dist, distc (distance euclidienne quadratique 3D)<br>
Christophe Doublier: zoomint<br>
Hildegard Koehler: lintophat<br>
Cédric Allène: gettree, histolisse, labeltree, nbcomp, pgm2vtk, seuilauto<br>
Gu Jun: maxdiameter<br>
Sébastien Couprie: mcsplines.c<br>
Rita Zrour: medialaxis (axe médian euclidien exact - algo de Rémy-Thiel),
dist, distc (distance euclidienne quadratique exacte - algo de Saito-Toriwaki)<br>
Jean Cousty: redt 3d (reverse euclidean distance transform - algo de D. Coeurjolly), watershedthin, opérateurs sur les graphes d'arêtes (GA), forêts de poids min (MSF), waterfall, recalagerigide_translateplane<br>
André Vital Saude: radialopening, divers scripts tcl, hma<br>
Laurent Mercier: gestion d'un masque dans delaunay.c<br>
Nicolas Combaret: toposhrinkgray, ptselectgray<br>
John Chaussard: lballincl<br>

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

