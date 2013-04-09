import sys
sys.path.append('python/test')
import matplotlib
import numpy as np
# non-interactive backend
matplotlib.use('PDF')
## real work done here

import plottest02

## plotting done here
plottest02.saveimages('dynamic/sampleinput.pdf', 'dynamic/sampleoutput.pdf')
## result
print r'\subfigure[]{\includegraphics[width=0.45\textwidth]{dynamic/sampleinput.pdf}}'
print r'\subfigure[]{\includegraphics[width=0.45\textwidth]{dynamic/sampleoutput.pdf}}'

