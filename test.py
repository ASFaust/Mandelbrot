import Mandelbrot as Mandel
import numpy as np
import cv2
from graphics import coloring,circular

mb = Mandel.Mandelbrot(precision = Mandel.DOUBLE, max_it = 10000, bailout = 100.0)

p1 = mb.search(1000,0.9)
print(p1)
r = np.exp(np.random.uniform(np.log(mb.min_radius(p1,(1000,1000))),0.001))
print(r)

img = mb.render(p1, r, (1000, 1000), Mandel.RENDER_DISTANCE)
img = coloring(img)
img = circular(img)
print(img.max())
#img = mb.render(p1, r, (1000, 1000), Mandel.RENDER_DISTANCE)
while(True):
    #for i in range(1,1000):
    cv2.imshow("distance render",img)
    cv2.waitKey(1)


