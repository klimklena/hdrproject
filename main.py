# Supported image formats:
# .bmp, .dib, .jpeg, .jpg, .jpe, .jp2, .png, .pbm, .pgm, .ppm, .sr, .ras, .tiff,
# .tif

import cv2
import os

data_dir = 'data/'
scene_dir = 'scene1/'  # directory containing one videoframes' sequence with
                        # 3 different exposure settings used, sorted in the
                        # order of L-M-H-L-M-H-etc.
result_dir = 'results/'  # write results here

imgs_lst = os.listdir(data_dir+scene_dir)
images = []

for imgname in imgs_lst:
    img_path = data_dir+scene_dir+imgname
    if os.path.isfile(img_path):
        images.append(cv2.imread(img_path))

# Process images:
res_images = []
for n in range(0, len(images)):
    # cv2.imshow('Frame '+str(n), images[n])
    res_images.append(images[n])

# Save result:
for n in range(0, len(res_images)):
    cv2.imwrite(result_dir+str(n)+'.png', res_images[n])
