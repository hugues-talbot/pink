import matplotlib.pyplot as plt

from skimage import data, filter, io

# not hard !!
def sampleproc(image):
    edges = filter.sobel(image)
    return edges

## saving procedure
def saveimages(path1, path2):
    inputimg = data.coins()
    outputimg = sampleproc(inputimg)
    io.imshow(inputimg)
    plt.savefig(path1)
    io.imshow(outputimg)
    plt.savefig(path2)

