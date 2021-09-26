import Mandelbrot as Mandel
import numpy as np
import cv2

mb = Mandel.Mandelbrot(precision = Mandel.DOUBLE, max_it = 10000, bailout = 100.0)

p1 = mb.search_v1(10000,0.5)
print(p1)
zl = mb.random_zoom_level(p1,(1000,1000))
print(zl)
img = mb.render_distance(p1,zl,(1000,1000))

img[img > 1.0] = 1.0
#img = np.log2(img + 1)
#img = (np.log2(img+1))
#img[img >= 0.7] = 1.0
while(True):
    cv2.imshow("distance render",img)
    cv2.waitKey(1)


