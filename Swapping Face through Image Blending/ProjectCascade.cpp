#include <stdio.h>
#include <opencv2/opencv.hpp>
using namespace std;
CvHaarClassifierCascade *facecascade;
CvMemStorage *memory;
void facialdetection(IplImage *faceframe);
int value;
int imgnum;
int fiximagesnumber = 20;