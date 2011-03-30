# Michel Couprie, 2011
# CeCILL free-software license

# histograms 

from pink import imview as imview
from pink import cpp as pink
import numpy as np
import matplotlib.pyplot as plt

global DEBUG
DEBUG=1

# conversion : from Pink image to numpy array
# MC: to be replaced by faster built-in procedure
def image_2_array(img):
    N = img.size.prod()
    a = np.empty(N)    
    for i in range(N): a[i] = img[i]
    a.resize(img.size)
    return a

# conversion : from numpy array to Pink image 
# MC: to be replaced by faster built-in procedure
# A CORRIGER - RENVOIE UNE IMAGE VIDE
def array_2_image(arr):
    S = list(arr.shape)
    N = arr.size
    img = pink.char_image(S)
    a.resize(N)
    for i in range(N):
        img[i] = int(a[i])
    return img

# histogram
def histogram(img,mask=None):
    if img.imtype() != 'uint8_t':
        print "histogram: error"
        return
    h = np.zeros(256)    
    N = img.size.prod()
    if mask == None:
        for i in range(N): h[img[i]] = h[img[i]] + 1
    else:
        for i in range(N):
            if mask[i] != 0: h[img[i]] = h[img[i]] + 1
    return h

# show histogram
def show_histo(img):
    h = histogram(img)
    x = np.array(range(256))
    y = np.zeros(256)
    fig = plt.figure()
    ax1 = fig.add_subplot(111)  # 1 row, 1 column, first (upper) plot
    ax1.vlines(x, y, h, lw=2)
    ax1.grid(True)
    ax1.axhline(0, color='black', lw=1)
    plt.show()
    return

im = pink.readimage("../images/uo.pgm")
#ar = image_2_array(im)
#im2 = array_2_image(ar)
show_histo(im)

# LuM end of file