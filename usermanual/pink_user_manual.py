import sys
sys.path.append('python/test')
import matplotlib
# non-interactive backend
matplotlib.use('PDF')
##
import plottest01
##
matplotlib.pyplot.savefig('dynamic/multiaxis.pdf')
print r'\includegraphics[width=0.5\textwidth]{dynamic/multiaxis.pdf}'

