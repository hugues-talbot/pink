# -*- coding: utf-8 -*- 
#  
# This software is licensed under  
# CeCILL FREE SOFTWARE LICENSE AGREEMENT 
 
# This software comes in hope that it will be useful but  
# without any warranty to the extent permitted by applicable law. 
   
# (C) UjoImro <ujoimro@gmail.com>, 2012 
# ProCarPlan s.r.o. 
 
import os 
import pink 
import unittest 
import xmlrunner 
 
try:   
    IMAGES  = os.environ["PINKTEST"] + "/images" 
    RESULTS = os.environ["PINKTEST"] + "/results_prev" 
except KeyError:  
    raise Exception, "PINKTEST environment variable must be defined for the testing module. It must point to the testimages directory" 


class erosion(unittest.TestCase):
    def test_0(self):
        result = pink.cpp.erosion( pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2hebreu.pgm' ), pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2_se_5_7.pgm' ))
        gold   = pink.cpp.readimage( RESULTS + '/erosion_b2hebreu_b2_se_5_7.pgm')
        self.assertTrue( result == gold )


    def test_1(self):
        result = pink.cpp.erosion( pink.cpp.readimage( IMAGES + '/2dbyte/gray/g2fish1.pgm' ), pink.cpp.readimage( IMAGES + '/2dbyte/binary/b2_se_5_7.pgm' ))
        gold   = pink.cpp.readimage( RESULTS + '/erosion_g2fish1_b2_se_5_7.pgm')
        self.assertTrue( result == gold )


    def test_2(self):
        result = pink.cpp.erosion( pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3a.pgm' ), pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3_se_5_5_7.pgm' ))
        gold   = pink.cpp.readimage( RESULTS + '/erosion_b3a_b3_se_5_5_7.pgm')
        self.assertTrue( result == gold )


    def test_3(self):
        result = pink.cpp.erosion( pink.cpp.readimage( IMAGES + '/3dbyte/gray/g3a.pgm' ), pink.cpp.readimage( IMAGES + '/3dbyte/binary/b3_se_5_5_7.pgm' ))
        gold   = pink.cpp.readimage( RESULTS + '/erosion_g3a_b3_se_5_5_7.pgm')
        self.assertTrue( result == gold )


if __name__ == '__main__':
    unittest.main(testRunner=xmlrunner.XMLTestRunner(output='test-reports'))