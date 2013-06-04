#!/usr/bin/python
# -*- coding: utf-8 -*-
# 
# This software is licensed under 
# CeCILL FREE SOFTWARE LICENSE AGREEMENT

# This software comes in hope that it will be useful but 
# without any warranty to the extent permitted by applicable law.
  
# (C) UjoImro <ujoimro@gmail.com>, 2012
# ProCarPlan s.r.o.

# This script generates the unittests from the src/com directory

from sys import argv
from os import listdir
from os.path import join
from time import strftime
from string import replace, find



def parse_file(filename):
    result = []
    parsfile = open( filename, 'r' )
    lines = parsfile.readlines();
    for line in lines:
        test_pos   = line.find('%TEST')
        images_pos = line.find('%IMAGES')
        results_pos = line.find('%RESULTS')

        if test_pos>=0:
            split = map( test_filter, line[:len(line)-1].split(' ') )
            command = '';
            command += '        result = pink.cpp.' + split[1] + '( ' + split[2]
            for q in split[3:len(split)-1]:
                 command += ', ' + q;
                
            command += ')\n'
            command += '        gold   = pink.cpp.readimage( RESULTS + \'' +  split[len(split)-1].replace('%RESULTS', '') + '\')'

            result.append(command)

    return result

def test_filter( test ):
    if test.find('%IMAGES')>=0:
        return test.replace('%IMAGES','pink.cpp.readimage( IMAGES + \'') + '\' )'
    else:
        return test

def generate_unit_test( filename, tests, testname ):
    commands = ''
    commands += '# -*- coding: utf-8 -*- \n'
    commands += '#  \n'
    commands += '# This software is licensed under  \n'
    commands += '# CeCILL FREE SOFTWARE LICENSE AGREEMENT \n'
    commands += ' \n'
    commands += '# This software comes in hope that it will be useful but  \n'
    commands += '# without any warranty to the extent permitted by applicable law. \n'
    commands += '   \n'
    commands += '# (C) UjoImro <ujoimro@gmail.com>, 2012 \n'
    commands += '# ProCarPlan s.r.o. \n'
    commands += ' \n'
    commands += 'import os \n'
    commands += 'import pink \n'
    commands += 'import unittest \n'
    commands += 'import xmlrunner \n'
    commands += ' \n'
    commands += 'try:   \n'
    commands += '    IMAGES  = os.environ["PINKTEST"] + "/images" \n'
    commands += '    RESULTS = os.environ["PINKTEST"] + "/results_prev" \n'
    commands += 'except KeyError:  \n'
    commands += '    raise Exception, "PINKTEST environment variable must be defined for the testing module. It must point to the testimages directory" \n\n\n'

    commands += 'class ' + testname + '(unittest.TestCase):\n'

    for q in range(len(tests)):
        commands += generate_test_method( tests[q], q )
        
    commands += 'if __name__ == \'__main__\':\n'
    commands += '    unittest.main(testRunner=xmlrunner.XMLTestRunner(output=\'test-reports\'))\n'


    print('filename = ' + filename)
    outfile = open(filename, 'w')
    outfile.write(commands)
    outfile.close()
    return commands



def generate_test_method( test, number ):
    result = ''    
    result += '    def test_' + str(number) + '(self):\n'
    result += test + '\n'
    result += '        self.assertTrue( result == gold )\n\n\n'

    return result;


if len(argv)!=3:
#                                             0                    1                    2
    raise Exception, "error: usage: ./python_test_generator source_directory python_module_directory"

dir = argv[1]
target = argv[2]

#for q in ['seuil.c']:
for q in listdir(dir):
    opened = False
    try:
        filedesc = open(join(dir, q), "r")
        filedesc.close()
        opened = True
    except:
        print "skipping " + join(dir, q)

    if opened:
        print('processing file = ' + q )
        basename = q[:q.rfind('.')]

        tests = parse_file( dir + '/' + q )

        if len(tests) > 0:
            generate_unit_test( target + '/' + basename + '.py', tests, basename )


# LuM end of file

