import Mandelbrot as Mandel
import numpy as np
import cv2

#converts a grayscale image to a colored one with a random hue
def color_td(img):
    #v1 = 1.0 - (np.random.rand() * np.random.rand())
    brightness = np.random.uniform(0.5,0.9)
    saturation = np.random.rand()
    color = np.random.randint(0, 180)

    #for i in range(2):
    #    img[:, :, i] -= img[:, :, i].min()
    #    img[:, :, i] /= img[:, :, i].max()
    #img[:,:,1] = img[:,:,1] ** 0.2

    i1 = img[:,:,0] #we want the brighter regions of that

    i1 -= i1.min()
    i1 /= i1.max()
    i1 = 1.0 - i1
    i2 = img[:, :, 1]  # and the darker regions of that
    s1 = [i2 >= 1.0]
    s2 = [i2 < 1.0]

    i2[s1] = 1.0

    i1[s1] = np.maximum(np.ones_like(i1)[s1] * 0.2, i1[s1])
    i1[s2] = np.minimum(i2[s2],i1[s2])

    i1 -= i1.min()
    i1 /= i1.max()
    i1 *= brightness

    hsv = np.zeros(shape=(img.shape[0], img.shape[1], 3),dtype = np.uint8)
    hsv[:,:,0] = i1 * 255.0
    hsv[:,:,1] = i1 * 255.0
    hsv[:,:,2] = i1 * 255.0
    hsv = cv2.cvtColor(hsv, cv2.COLOR_BGR2HSV)
    hsv[:, :, 0] = color
    hsv[:, :, 1] = 255 - (i1 * 255.0).astype(np.uint8)
    hsv[:, :, 1] = (hsv[:, :, 1].astype(np.float32) * saturation).astype(np.uint8)
    #hsv[:, :, 2] = (i1.astype(np.float32) * 255.0).astype(np.uint8)
    img = cv2.cvtColor(hsv, cv2.COLOR_HSV2BGR).astype(np.float32)
    img /= img.max()
    return img

def circular(img):
    circle = np.zeros(shape = (img.shape[0]*4,img.shape[1]*4,1),dtype = np.float32)
    cx = int(max(img.shape[1],img.shape[0]) / 2.0) * 4
    circle = cv2.circle(circle,(cx,cx), cx, 1.0, -1)
    circle = cv2.resize(circle,(img.shape[0],img.shape[1]),interpolation = cv2.INTER_AREA)
    circle = np.expand_dims(circle,-1)
    return img * circle + (1.0 - circle)


def run(seed = 0):
    if seed == 0:
        seed = np.random.randint(0,2**32-1)
    np.random.seed(seed)

    mb = Mandel.Mandelbrot(precision = Mandel.DOUBLE, max_it = 1000, bailout = 20.0)

    alpha = np.random.rand() * 0.9 + 0.09
    p1 = mb.search(1000,alpha,seed)
    r = np.exp(np.random.uniform(np.log(mb.min_radius(p1,(3000,3000))),0.001))
    img = mb.render(p1, r, (3000, 3000), Mandel.RENDER_TIME_DISTANCE)
    img = color_td(img)
    img = cv2.resize(img,(1000,1000),interpolation = cv2.INTER_AREA)
    img = circular(img)
    return img

