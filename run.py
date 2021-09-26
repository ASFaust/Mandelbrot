from Mandelbrot import find_border_threaded
import numpy as np
import cv2
import requests

w, h = 1000,1000
aa_fac = 3

w_aa = int(w*aa_fac)
h_aa = int(h*aa_fac)

img,info = find_border_threaded(4,4000,1000,w_aa,h_aa)
img = img.astype(np.float32)
pos = info[1] + info[2] * 1.0j
pos = pos.__format__('.32f').replace("j","i")
zoom = info[0]

def color_img(img):
    v1 = 1.0 - (np.random.rand() * np.random.rand())
    brightness = (v1 + 0.4) / (0.4 + 1.1)
    saturation = np.random.rand()
    color = np.random.randint(0,180)
    
    img -= img.min()
    img /= img.max()
    img = 1.0 - img
    imax = img[img < 1.0].max()
    img[img < 1.0] *= brightness / imax
    if(np.random.randint(0,2) == 1):
        img[img < 1.0] = 1.0 - img[img < 1.0]
        img -= img.min()
        img /= img.max()
        brightness = 1.0 - brightness

    canvas = np.ones(shape = (h_aa,w_aa,3))
    canvas[:,:,0:1] = img
    canvas[:,:,1:2] = img
    canvas[:,:,2:3] = img
    canvas = (canvas * 255.0).astype(np.uint8)
    hsv = cv2.cvtColor(canvas, cv2.COLOR_BGR2HSV)
    hsv[:,:,0] = color
    hsv[:,:,1] = 255 - hsv[:,:,2]
    hsv[:,:,1] = (hsv[:,:,1].astype(np.float32) * saturation).astype(np.uint8)
    img = cv2.cvtColor(hsv, cv2.COLOR_HSV2BGR)
    img = cv2.resize(img,(w,h),interpolation = cv2.INTER_AREA)
    return img

img = color_img(img)

path = "/home/andi/bots/mandelbot/temp.png"
token = open("/home/andi/bots/mandelbot/token","r").read()

cv2.imwrite(path,img)
text = f"Position:\n{pos}\n\nRadius:\n1/{int(zoom)}"
params = (
    ('message', text),
    ('access_token', token),
    )

files = {'source':(path,open(path,"rb"),'image/png',{'Expires': '0'})}

response = requests.post('https://graph.facebook.com/v3.2/me/photos', params = params,files = files)

