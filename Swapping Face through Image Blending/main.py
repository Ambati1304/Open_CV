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

# In[97]:


def Transformation(points1, points2):
    points1 = points1.astype(numpy.float64)
    points2 = points2.astype(numpy.float64)

    c1 = numpy.mean(points1, axis=0)
    c2 = numpy.mean(points2, axis=0)
    points1 -= c1
    points2 -= c2

    s1 = numpy.std(points1)
    s2 = numpy.std(points2)
    points1 /= s1
    points2 /= s2
    
    U, S, Vt = numpy.linalg.svd(points1.T * points2)

    # The R we seek is in fact the transpose of the one given by U * Vt. This
    # is because the above formulation assumes the matrix goes on the right
    # (with row vectors) where as our solution requires the matrix to be on the
    # left (with column vectors).
    R = (U * Vt).T
    
    return numpy.vstack([numpy.hstack(((s2 / s1) * R,
                                       c2.T - (s2 / s1) * R * c1.T)),
                         numpy.matrix([0., 0., 1.])])

# In[98]:

class TooManyFaces(Exception):
    pass

class NoFaces(Exception):
    pass

# In[99]:

def getFaceMask(im, landmarks):
    im = numpy.zeros(im.shape[:2], dtype=numpy.float64)

    for group in OVERLAY_POINTS:
    #for group in FACE_POINTS:
        drawConvexHull(im,
                         landmarks[group],
                         color=1)
    
    im = numpy.array([im, im, im]).transpose((1, 2, 0))

    im = (cv2.GaussianBlur(im, (FEATHER_AMOUNT, FEATHER_AMOUNT), 0) > 0) * 1.0
    im = cv2.GaussianBlur(im, (FEATHER_AMOUNT, FEATHER_AMOUNT), 0)
    
    return im

def imageWarp(im, M, dshape):
    output_im = numpy.zeros(dshape, dtype=im.dtype)
    cv2.warpAffine(im,
                   M[:2],
                   (dshape[1], dshape[0]),
                   dst=output_im,
                   borderMode=cv2.BORDER_TRANSPARENT,
                   flags=cv2.WARP_INVERSE_MAP)
    return output_im

# In[100]:

def faceSwap(image1,image2):
    
    image1, landmarks1 = readImageAndLandmarks(image1)
    image2, landmarks2 = readImageAndLandmarks(image2)

    M = Transformation(landmarks1[ALIGN_POINTS],
                              landmarks2[ALIGN_POINTS])
    
    #M = Transformation(landmarks1[FACE_POINTS],
     #                             landmarks2[FACE_POINTS])

    mask = getFaceMask(image2, landmarks2)

    plt.imshow(mask, interpolation='nearest')
    plt.savefig("mask1.jpg")

    
    maskWarp = imageWarp(mask, M, image1.shape)
    #combinedMask = numpy.max([getFaceMask(image1, landmarks1), mask],axis=0)
    
    plt.imshow(maskWarp, interpolation='nearest')
    plt.savefig("mask2.jpg")
    
    combinedMask = numpy.max([getFaceMask(image1, landmarks1), maskWarp],
                          axis=0)

    plt.imshow(combinedMask, interpolation='nearest')
    plt.show()
    
    image2Warp = imageWarp(image2, M, image1.shape)
    correctedWarp = colorCohesion(image1, image2Warp, landmarks1)
    
    plt.imshow(correctedWarp, interpolation='nearest')
    plt.show()
    
    correctedWarp = colorCohesion(image1, image2Warp, landmarks1)

    swappedImage = image1 * (1.0 - combinedMask) + correctedWarp * combinedMask

    cv2.imwrite('output5.jpg', swappedImage)


# In[101]:

faceSwap("face.jpg","face2.jpg")


