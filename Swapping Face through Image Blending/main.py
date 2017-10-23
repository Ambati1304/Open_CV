# coding: utf-8

# In[94]:

import cv2
import dlib
import numpy
import sys
from PIL import Image
import matplotlib.pyplot as plt

# In[95]:

PREDICTOR_PATH = "shape_predictor_68_face_landmarks.dat"
SCALE_FACTOR = 1
FEATHER_AMOUNT = 11
detector = dlib.get_frontal_face_detector()
predictor = dlib.shape_predictor(PREDICTOR_PATH)
FACE_POINTS = list(range(17, 68))
MOUTH_POINTS = list(range(48, 61))
RIGHT_BROW_POINTS = list(range(17, 22))
LEFT_BROW_POINTS = list(range(22, 27))
RIGHT_EYE_POINTS = list(range(36, 42))
LEFT_EYE_POINTS = list(range(42, 48))
NOSE_POINTS = list(range(27, 35))
JAW_POINTS = list(range(0, 17))
# Points used to line up the images.
ALIGN_POINTS = (LEFT_BROW_POINTS + RIGHT_EYE_POINTS + LEFT_EYE_POINTS +
                               RIGHT_BROW_POINTS + NOSE_POINTS + MOUTH_POINTS)
OVERLAY_POINTS = [
   LEFT_EYE_POINTS + RIGHT_EYE_POINTS + LEFT_BROW_POINTS + RIGHT_BROW_POINTS,
    NOSE_POINTS + MOUTH_POINTS,
]
COLOUR_CORRECT_BLUR_FRAC = .8



# In[96]:

#haar face detector
def haarFaceDetector(image):
    face_cascade = cv2.CascadeClassifier('myfacedetector.xml')
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    faces = face_cascade.detectMultiScale(gray, 1.3, 5)
    return faces
#get the images and landmark
def readImageAndLandmarks(fname):
    image = cv2.imread(fname, cv2.IMREAD_COLOR)
    image = cv2.resize(image, (image.shape[1] * SCALE_FACTOR,
                         image.shape[0] * SCALE_FACTOR))
    lMarks = getLandmarks(image)
    return image, lMarks

#color cohesion technique to correct the color
def colorCohesion(image1, image2, landmarks1):
    blur_amount = COLOUR_CORRECT_BLUR_FRAC * numpy.linalg.norm(
                              numpy.mean(landmarks1[LEFT_EYE_POINTS], axis=0) -
                              numpy.mean(landmarks1[RIGHT_EYE_POINTS], axis=0))
    blur_amount = int(blur_amount)
    if blur_amount % 2 == 0:
        blur_amount += 1
    image1_blur = cv2.GaussianBlur(image1, (blur_amount, blur_amount), 0)
    image2_blur = cv2.GaussianBlur(image2, (blur_amount, blur_amount), 0)
    image2_blur += (128 * (image2_blur <= 1.0)).astype(image2_blur.dtype)
    return (image2.astype(numpy.float64) * image1_blur.astype(numpy.float64) /
                                                image2_blur.astype(numpy.float64))

def getLandmarks(im):
    rects = detector(im, 1)
    
    if len(rects) > 1:
        raise TooManyFaces
    if len(rects) == 0:
        raise NoFaces

    return numpy.matrix([[p.x, p.y] for p in predictor(im, rects[0]).parts()])

#get the Convex Hull
def drawConvexHull(image, points, color):
    points = cv2.convexHull(points)
    cv2.fillConvexPoly(image, points, color=color)



