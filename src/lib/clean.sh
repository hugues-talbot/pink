#! /bin/bash
# -*- sh -*-

# FILENAMES="ldilateros3d lerosfast3d"
# EXPRS="lerosfast3d"

# for q in $FILENAMES; 
# do
#     for w in $EXPRS;
#     do
#         #echo sed s/$EXPR/$q\_$EXPR/ "< $q.c > $q.c_"
# 	sed s/$w/$q\_$w/ < $q.c > $q.c_
# 	mv $q.c_ $q.c
#     done;
# done;

TARGET=nbvoisc6
#for FILENAME in lcurves lgettree llabeltree lseltopo lskelcurv lskelpar3d ;
#for FILENAME in lfermetrous3d lfiltrestopo lhtkern3d lnbtopo lseltopo lskelcurv lskeletons lskelpar3d lsquelval ltopotypes ;
for FILENAME in lborder  lmaxdiameter ;
do
    sed s/$TARGET/mctopo3d_$TARGET/g < $FILENAME.c > $FILENAME.c_
#    sed s/mctopo3d_mctopo3d/mctopo3d/g < $FILENAME.c > $FILENAME.c_
    mv $FILENAME.c_ $FILENAME.c
done




# LuM end of file
