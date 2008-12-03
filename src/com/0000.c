/*! \mainpage
 
  \section warning Avertissement
 
Cette biblioth�que d'op�rateurs de traitement d'images a �t� 
d�velopp�e exclusivement � des fins d'enseignement, de recherche et d'exp�rimentation.
Elle ne doit en aucun cas �tre diffus�e sans l'accord de:

\verbatim
Michel Couprie  -  Professeur  -  ESIEE-Paris
Laboratoire D'Informatique Gaspard-Monge, Universit� Paris-Est
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
C�dric All�ne: gettree, histolisse, labeltree, nbcomp, pgm2vtk, seuilauto<br>
Gu Jun: maxdiameter<br>
S�bastien Couprie: mcsplines.c<br>
Rita Zrour: medialaxis (axe m�dian euclidien exact - algo de R�my-Thiel),
dist, distc (distance euclidienne quadratique exacte - algo de Saito-Toriwaki)<br>
Jean Cousty: redt 3d (reverse euclidean distance transform - algo de D. Coeurjolly), watershedthin, op�rateurs sur les graphes d'ar�tes (GA), for�ts de poids min (MSF), waterfall, recalagerigide_translateplane<br>
Andr� Vital Saude: radialopening, divers scripts tcl, hma<br>
Laurent Mercier: gestion d'un masque dans delaunay.c<br>
Nicolas Combaret: toposhrinkgray, ptselectgray<br>
John Chaussard: lballincl<br>

  \section install Installation

\subsection linux Linux

Pour installer la biblioth�que sous Linux, tapez:
\verbatim
tar zxvf pink.tgz
mv Pinktmp Pink
cd Pink
makelin
\endverbatim

Les ex�cutables se trouvent dans Pink/linux/bin,
les scripts dans Pink/scripts et dans Pink/tcl
Mettez � jour votre variable $path en cons�quence.
Par exemple (en csh): 

\verbatim
setenv PINK ~coupriem/Pink
set path=( $path $PINK/linux/bin $PINK/scripts $PINK/tcl )
\endverbatim

\subsection windows Windows

Installez tout d'abord <A HREF="http://www.mingw.org/">MinGW</A> (Minimalist GNU For Windows), 
ainsi que le programme "make" de la distribution MinGW (installation s�par�e).

D�compressez l'archive pink.tgz (par exemple avec WinZip ou WinRar).

Modifiez selon votre configuration, les fichiers "mgw.make" et "makemgw.bat".

Ouvrez une fen�tre DOS et mettez-vous dans le r�pertoire principal de Pink.

tapez: makemgw

Les ex�cutables se trouvent dans Pink/bin,
mettez � jour votre variable PATH en cons�quence.
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
cause: flag de mise au point oubli�
solution rapide: chercher le "#define CHRONO" dans le fichier source et le retirer

\subsection associes Logiciels associ�s

Pour une utilisation optimale, les logiciels suivants doivent �tre
install�s:<br><br>
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

