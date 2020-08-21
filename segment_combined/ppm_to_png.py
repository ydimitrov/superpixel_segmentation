import os 
import cv2 
from glob import glob 

cwd = os.getcwd()
input_dir = os.path.join(cwd, "input/*.ppm")    
ppms = glob(input_dir)   

# counter = 1 

for ppm in ppms:
    base = os.path.basename(ppm)
    base = os.path.splitext(base)[0]
    cv2.imwrite(base+".png", cv2.imread(ppm))
#    print(base)
#    cv2.imwrite(str(ppm)+".png", cv2.imread(ppm))
#    counter += 1 
