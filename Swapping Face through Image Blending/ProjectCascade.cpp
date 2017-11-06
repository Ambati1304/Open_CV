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
	string RootCascade = "MyCascades/";
	string Extension = ".xml";
	string filename = RootCascade + "myfaced" + Extension;
	facecascade = (CvHaarClassifierCascade*)cvLoad(filename.c_str());
	memory = cvCreateMemStorage(0);
	IplImage* inputImage = cvLoadImage("MyInputs/GT.jpg", 1);

	if (!facecascade || !memory || !inputImage)
	{
		printf("Initialization Failed: %s\n",
			(!facecascade) ? " describe cascade not found !\n" :
			(!memory) ? " Sufficient Memory not allocated !\n" :
			" The input file can not be found!\n");
		system("pause");
		return 0;
	}
	facialdetection(inputImage);
	cvShowImage("Detected Face", inputImage);
	cvReleaseImage(&inputImage);
	cvWaitKey(0);
	char memorybuffer[20];
	for (imgnum = 0; imgnum <= fiximagesnumber; imgnum++)
	{
		sprintf_s(memorybuffer, "MyInputs/P%04d.bmp", imgnum);
		printf("\n");
		inputImage = cvLoadImage(memorybuffer, CV_LOAD_IMAGE_UNCHANGED);
		printf("Input Image = P%04d.bmp\n", imgnum);
		facialdetection(inputImage);
		cvShowImage("Face Detection", inputImage);
		cvReleaseImage(&inputImage);
		value = cvWaitKey(0);
		if (value == 'e' || value == 'E')
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
