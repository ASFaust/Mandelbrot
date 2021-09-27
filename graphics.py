import numpy as np
import cv2

#converts a grayscale image to a colored one with a random hue
def coloring(img):
    v1 = 1.0 - (np.random.rand() * np.random.rand())
    brightness = (v1 + 0.4) / (0.4 + 1.1)
    saturation = np.random.rand()
    color = np.random.randint(0, 180)

    img -= img.min()
    img /= img.max()

    img = 1.0 - img
    img = np.sqrt(img)
    #imax = img[img < 1.0].max()
    img *= brightness

    #imax = img[img < 1.0].max()
    #img[img < 1.0] *= brightness / imax

    canvas = np.ones(shape=(img.shape[0], img.shape[1], 3),dtype = np.float32)
    canvas[:, :, 0] = img
    canvas[:, :, 1] = img
    canvas[:, :, 2] = img
    canvas = (canvas * 255.0).astype(np.uint8)
    hsv = cv2.cvtColor(canvas, cv2.COLOR_BGR2HSV)
    hsv[:, :, 0] = color
    hsv[:, :, 1] = 255 - hsv[:, :, 2]
    hsv[:, :, 1] = (hsv[:, :, 1].astype(np.float32) * saturation).astype(np.uint8)
    img = cv2.cvtColor(hsv, cv2.COLOR_HSV2BGR).astype(np.float32)
    img /= img.max()
    #img = cv2.resize(img, (w, h), interpolation=cv2.INTER_AREA)
    return img

def circular(img):
    circle = np.zeros(shape = (img.shape[0]*4,img.shape[1]*4,1),dtype = np.float32)
    cx = int(max(img.shape[1],img.shape[0]) / 2.0) * 4
    circle = cv2.circle(circle,(cx,cx), cx, 1.0, -1)
    circle = cv2.resize(circle,(img.shape[0],img.shape[1]),interpolation = cv2.INTER_AREA)
    circle = np.expand_dims(circle,-1)
    canvas = img * circle + (1.0 - circle)
    return canvas
