import sys
import os 
import cv2 
from glob import glob 

# cwd = os.getcwd()
# input_dir = os.path.join(cwd, "*.ppm")    
# ppms = glob(input_dir)   

# counter = 1


# for ppm in ppms:
base = os.path.basename(sys.argv[1])
base = os.path.splitext(base)[0]
cv2.imwrite(base+"_rgb.png", cv2.imread(sys.argv[1]))
print('Convertion to rgb png completed!')
#    print(base)
#    cv2.imwrite(str(ppm)+".png", cv2.imread(ppm))
#    counter += 1 
