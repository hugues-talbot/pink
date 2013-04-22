#import pink
#import numpy as np

# creation of some 3D data
myarray = np.array(np.arange(0,5*5*5)).reshape(5,5,5)
myarray
# convert array -> pink
myimage = pink.numpy2pink(myarray)
# convert pink -> array
newarray = pink.pink2numpy(myimage)
newarray
# variance calculation
newarray.var(axis=0)
