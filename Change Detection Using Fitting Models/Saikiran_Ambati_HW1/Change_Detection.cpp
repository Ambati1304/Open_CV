//#include "opencv2/imgcodecs.hpp"#include "opencv2/core/core.hpp"#include "opencv2/imgcodecs.hpp"#include "opencv2/highgui/highgui.hpp"#include <iostream>#include "opencv2/imgproc/imgproc.hpp"using namespace cv;using namespace std;Mat image1 = imread("inp1.jpg", 0);Mat image2 = imread("inp2.jpg", 0);int l1 = image1.rows * image1.cols;int l2 = image2.rows * image2.cols;int *x1 = new int[l1];int *x2 = new int[l2];int main(int, char** argv){	Mat img1(280, 280, CV_8UC3, Scalar(255, 255, 255));	int a = 0;	for (int i = 0; i < image1.rows; i++)	{		for (int j = 0; j < image1.cols; j++)		{			x1[a] = image1.at<uchar>(i, j);			x2[a] = image2.at<uchar>(i, j);			img1.at<Vec3b>(x1[a], x2[a])[0] = 255;			img1.at<Vec3b>(x1[a], x2[a])[1] = 0;			img1.at<Vec3b>(x1[a], x2[a])[2] = 0;			a++;		}	}		waitKey();	return 0;}