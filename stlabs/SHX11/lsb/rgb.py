import cv2
import numpy as np
re_img1 = cv2.imread('re_img1.png')
b, g, r = cv2.split(re_img1)

ttl = re_img1.size / 3 #divide by 3 to get the number of image PIXELS

"""b, g, and r are actually numpy.ndarray types,
so you need to use the appropriate method to sum
all array elements"""
B = float(np.sum(b)) / ttl #convert to float, as B, G, and R will otherwise be
int
G = float(np.sum(g)) / ttl
R = float(np.sum(r)) / ttl
B_mean1 = list()
G_mean1 = list()
R_mean1 = list()
B_mean1.append(B)
G_mean1.append(G)
R_mean1.append(R)
