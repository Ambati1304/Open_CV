//#include "opencv2/imgcodecs.hpp"#include "opencv2/core/core.hpp"#include "opencv2/imgcodecs.hpp"#include "opencv2/highgui/highgui.hpp"#include <iostream>#include "opencv2/imgproc/imgproc.hpp"using namespace cv;using namespace std;Mat image1 = imread("inp1.jpg", 0);Mat image2 = imread("inp2.jpg", 0);int l1 = image1.rows * image1.cols;int l2 = image2.rows * image2.cols;int *x1 = new int[l1];int *x2 = new int[l2];int main(int, char** argv){	Mat img1(280, 280, CV_8UC3, Scalar(255, 255, 255));	int a = 0;	for (int i = 0; i < image1.rows; i++)	{		for (int j = 0; j < image1.cols; j++)		{			x1[a] = image1.at<uchar>(i, j);			x2[a] = image2.at<uchar>(i, j);			img1.at<Vec3b>(x1[a], x2[a])[0] = 255;			img1.at<Vec3b>(x1[a], x2[a])[1] = 0;			img1.at<Vec3b>(x1[a], x2[a])[2] = 0;			a++;		}	}	vector<Point> data1;	Vec4f lines1;	Vec4f lines1_Fair;	Vec4f lines1_Welsch;	Vec4f lines1_Huber;		for (int i = 0; i < l1; i++)	{		data1.push_back(Point(x2[i], x1[i]));	}	// Gaussian Estimation	double Sumx1 = 0;	double Sumy1 = 0;	double Meanx1 = 0;	double Meany1 = 0;	double Sumxy1 = 0;	double Sumxx1 = 0;	double Sumyy1 = 0;	double covariance_xx1 = 0;	double covariance_yy1 = 0;	double covariance_xy1 = 0;	double start_angle = 0;	double end_angle = 360;	double angle1 = 0;	for (int i = 0; i < l1; i++)	{		Sumx1 = Sumx1 + x2[i];		Sumy1 = Sumy1 + x1[i];	}Meanx1 = Sumx1 / l2;	Meany1 = Sumy1 / l1;	cout << "Meanx:" << Meanx1 << "Mean:" << Meany1 << endl;	for (int i = 0; i < l1; i++)	{		Sumxy1 = Sumxy1 + ((x2[i] - Meanx1)*(x1[i] - Meany1));		Sumxx1 = Sumxx1 + ((x2[i] - Meanx1)*(x2[i] - Meanx1));		Sumyy1 = Sumyy1 + ((x1[i] - Meany1)*(x1[i] - Meany1));	}	covariance_xx1 = Sumxx1 / l1;	covariance_xy1 = Sumxy1 / l1;	covariance_yy1 = Sumyy1 / l1;	Mat covmat_1 = (Mat_<double>(2, 2) << covariance_xx1, covariance_xy1, covariance_xy1, covariance_yy1);	cout << covmat_1 << endl;	Mat eigenvalues_1, eigenvectors_1;	eigen(covmat_1, eigenvalues_1, eigenvectors_1);	cout << "eigenvalues_1: " << eigenvectors_1 << endl;	cout << "eigenvectors_1: " << eigenvalues_1 << endl;	double angle = atan2(eigenvectors_1.at<double>(0, 1), eigenvectors_1.at<double>(0, 0));	if (angle < 0)		angle += 6.28318530718;	angle1 = angle;	angle = 180 * angle / 3.14159265359;	cout << "angle: " << angle << endl;	double halfmajoraxissize = 5.991*sqrt(eigenvalues_1.at<double>(0));	double halfminoraxissize = 5.991*sqrt(eigenvalues_1.at<double>(1));	// Plotting of ellipse	ellipse(img1, Point((int)Meanx1, (int)Meany1), Size(sqrt(eigenvalues_1.at<double>(0)*5.991), sqrt(eigenvalues_1.at<double>(1)*5.991)), angle, start_angle, end_angle, Scalar(0, 0, 255), 2, 8, 0);		//Performing Total Least Square//	fitLine(data1, lines1, CV_DIST_L2, 0, 0.01, 0.01);	int lefty = (-lines1[2] * lines1[1] / lines1[0]) + lines1[3];	int righty = ((img1.cols - lines1[2])*lines1[1] / lines1[0]) + lines1[3];	line(img1, Point(img1.cols - 1, righty), Point(0, lefty), Scalar(0, 255, 0), 2);	cout << " lines1_L2[2]" << lines1[2] << endl;	cout << "lines1_L2[1]" << lines1[1] << endl;	cout << "lines1_L2[0]" << lines1[0] << endl;	cout << "lines1_L2[3]" << lines1[3] << endl;	// Displaying Line Fitting Image//	namedWindow("Line Fitting", 1);	imshow("Fit line", img1);	//Defining lines to perform robust estimation//	fitLine(data1, lines1_Fair, CV_DIST_FAIR, 1.3998, 0.01, 0.01);	int lefty1_F = (-lines1_Fair[2] * lines1_Fair[1] / lines1_Fair[0]) + lines1_Fair[3];	int righty1_F = ((img1.cols - lines1_Fair[2])*lines1_Fair[1] / lines1_Fair[0]) + lines1_Fair[3];	line(img1, Point(img1.cols - 1, righty1_F), Point(0, lefty1_F), Scalar(255, 255, 0), 2);	cout << " lines1_Fair[2]" << lines1_Fair[2] << endl;	cout << "lines1_Fair[1]" << lines1_Fair[1] << endl;	cout << "lines1_Fair[0]" << lines1_Fair[0] << endl;	cout << "lines1_Fair[3]" << lines1_Fair[3] << endl;	//Roubst Estimation for Fair line//	namedWindow("Robust Estimator", 1);	imshow("Fair", img1);	fitLine(data1, lines1_Welsch, CV_DIST_WELSCH, 2.9846, 0.01, 0.01);	int lefty1_H = (-lines1_Welsch[2] * lines1_Welsch[1] / lines1_Welsch[0]) + lines1_Welsch[3];	int righty1_H = ((img1.cols - lines1_Welsch[2])*lines1_Welsch[1] / lines1_Welsch[0]) + lines1_Welsch[3];	line(img1, Point(img1.cols - 1, righty1_H), Point(0, lefty1_H), Scalar(0, 0, 0), 2);	cout << " lines1_Welsch[2]" << lines1_Welsch[2] << endl; //Welsch Line is found out to be optimal 	cout << "lines1_Welsch[1]" << lines1_Welsch[1] << endl;	cout << "lines1_Welsch[0]" << lines1_Welsch[0] << endl;	cout << "lines1_Welsch[3]" << lines1_Welsch[3] << endl;	//Robust Estimation for Welsch line//	namedWindow("Robust Estimator", 1);	imshow("Welsch", img1);	fitLine(data1, lines1_Huber, CV_DIST_HUBER, 1.345, 0.01, 0.01);	int lefty1_W = (-lines1_Huber[2] * lines1_Huber[1] / lines1_Huber[0]) + lines1_Huber[3];	int righty1_W = ((img1.cols - lines1_Huber[2])*lines1_Huber[1] / lines1_Huber[0]) + lines1_Huber[3];	line(img1, Point(img1.cols - 1, righty1_W), Point(0, lefty1_W), Scalar(0, 255, 255), 2);	cout << " lines1_Huber[2]" << lines1_Huber[2] << endl;	cout << "lines1_Huber[1]" << lines1_Huber[1] << endl;	cout << "lines1_Huber[0]" << lines1_Huber[0] << endl;	cout << "lines1_Huber[3]" << lines1_Huber[3] << endl;	//Roubst Estimation for Huber line//	namedWindow("Robust Estimator", 1);	imshow("Huber", img1);	// Performing Threshold on Straight Line Fit//	Mat img2(280, 280, CV_8UC3, Scalar(255, 255, 255));	fitLine(data1, lines1, CV_DIST_L2, 0, 0.01, 0.01);	int lefty1_L2 = (-lines1[2] * lines1[1] / lines1[0]) + lines1[3];	int righty1_L2 = ((img2.cols - lines1[2])*lines1[1] / lines1[0]) + lines1[3];	line(img2, Point(img2.cols - 1, righty1_L2), Point(0, lefty1_L2), Scalar(0, 0, 255), 2);	double total_dist = 0;	float mean_dist = 0;	double pt_dist = 0;	for (int i = 0; i < l1; i++)      // Calculating perpendicular distance of data points from line//	{		pt_dist = (abs((lines1[1] * x2[i]) - (lines1[0] * x1[i]) - (lines1[1] * lines1[2]) + (lines1[0] * lines1[3]))) / sqrt(lines1[0] * lines1[0] + lines1[1] * lines1[1]);		total_dist = total_dist + pt_dist;	}	mean_dist = total_dist / l1;        //Calculating mean distance//										// Thresholding	for (int i = 0; i < l1; i++)	{		pt_dist = (abs((lines1[1] * x2[i]) - (lines1[0] * x1[i]) - (lines1[1] * lines1[2]) + (lines1[0] * lines1[3]))) / sqrt(lines1[0] * lines1[0] + lines1[1] * lines1[1]);		if (pt_dist <= 9) // Mean is not great as a threshold So taking 9//		{			img2.at<Vec3b>(x1[i], x2[i])[0] = 255;			img2.at<Vec3b>(x1[i], x2[i])[1] = 0;			img2.at<Vec3b>(x1[i], x2[i])[2] = 0;		}	}	cout << "mean distance : " << mean_dist << endl;	cout << lines1[2] << endl;	cout << lines1[1] << endl;	cout << lines1[0] << endl;	cout << lines1[3] << endl;	namedWindow("Thresholding of Line", 1);	imshow("line Thresholding", img2); 	waitKey();	return 0;}