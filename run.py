from generator import run
import numpy as np
import cv2

for i in range(400):
    seed = np.random.randint(1,2**32-1)
    img = run(seed)
    cv2.imwrite(f"results/{seed:10d}.jpg",img*255.0)

