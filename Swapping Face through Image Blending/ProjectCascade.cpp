#include <stdio.h>
#include <opencv2/opencv.hpp>
using namespace std;
CvHaarClassifierCascade *facecascade;
CvMemStorage *memory;
void facialdetection(IplImage *faceframe);
int value;
int imgnum;
int fiximagesnumber = 20;

int main(int argc, char** argv)
{
	
	return 0;
}


void facialdetection(IplImage *faceframe)
{
	CvSeq *faces = cvHaarDetectObjects(faceframe, facecascade, memory, 1.15, 5,
		0, cvSize(30, 30));
	for (int i = 0; i < (faces ? faces->total : 0); i++)
	{
		CvRect *r = (CvRect *)cvGetSeqElem(faces, i);
		cvRectangle(faceframe,
			cvPoint(r->x, r->y),
			cvPoint(r->x + r->width, r->y + r->height),
			CV_RGB(0, 0, 255), 2, 8, 0);
	}
}
