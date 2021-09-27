import Mandelbrot as Mandel
import numpy as np
import cv2
from graphics import coloring,circular

seed = np.random.randint(0,2**32-1)
np.random.seed(seed)

mb = Mandel.Mandelbrot(precision = Mandel.DOUBLE, max_it = 10000, bailout = 2.0)

alpha = np.random.rand() * 0.9 + 0.09
p1 = mb.search(1000,alpha,seed)
r = np.exp(np.random.uniform(np.log(mb.min_radius(p1,(1000,1000))),0.001))
img = mb.render(p1, r, (1000, 1000), Mandel.RENDER_TIME)
#img = np.sqrt(img)
#img = cv2.resize(img,(1000,1000),interpolation = cv2.INTER_AREA)
#img = np.sqrt(img)
img -= img.min()
img /= img.max()
img = 1.0 - img
img[img > 0.0] = 1.0
#img = coloring(img)
#img = circular(img)
#img[img == 0] = 1
print(img.max())
#img = mb.render(p1, r, (1000, 1000), Mandel.RENDER_DISTANCE)
while(True):
    #for i in range(1,1000):
    cv2.imshow("distance render",img)
    cv2.waitKey(1)

