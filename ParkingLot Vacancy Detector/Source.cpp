#include <opencv2/core/core.hpp>
#include "opencv2/objdetect/objdetect.hpp"
#include <opencv2/highgui/highgui.hpp>
#include<opencv\cv.hpp>
#include <iostream>
#include <iterator>
#include "Xmlparsing.h"

using namespace cv;
using namespace std;


class Cars_detector {

public:	    //variables kept public but precaution taken all over the code

	Mat image_input;          //main input image
	Mat image_main_result;    //the final result
	Mat storage;              //introduced to stop detection of same car more than once
	Mat draw;


	CascadeClassifier cascade;    //the main cascade classifier
	CascadeClassifier checkcascade;

	void getimage(Mat src) //getting the input image
	{

		if (!src.data)
		{
			cout << "src not filled" << endl;
		}

		else
		{
			image_input = src.clone();
			storage = src.clone();              //initialising storage
			image_main_result = src.clone();    //initialising result

			cout << "got image" << endl;
		}
	}
	void cascade_load(string cascade_string)            //loading the main cascade
	{
		cascade.load(cascade_string);

		if (!cascade.load(cascade_string))
		{
			cout << endl << "Could not load classifier cascade" << endl;

		}
		else
		{

			cout << "cascade : " << cascade_string << " loaded" << endl;
		}

	}

	Mat findcars() {
		int i = 0;

		Mat img = storage.clone();
		vector<Rect> cars;

		Mat gray;

		cvtColor(img, gray, CV_BGR2GRAY);


		Mat resize_image(cvRound(img.rows), cvRound(img.cols), CV_8UC1);

		//////testing
		//Then define your mask image
		//	cv::Mat mask = cv::Mat::zeros(img.size(), img.type());


		resize(gray, resize_image, resize_image.size(), 0, 0, INTER_LINEAR);
		equalizeHist(resize_image, resize_image);
		imshow("test", resize_image);
		const static Scalar colors[] = { CV_RGB(0,0,255),CV_RGB(0,255,0),CV_RGB(255,0,0),CV_RGB(255,255,0),CV_RGB(255,0,255),CV_RGB(0,255,255),CV_RGB(255,255,255),CV_RGB(128,0,0),CV_RGB(0,128,0),CV_RGB(0,0,128),CV_RGB(128,128,128),CV_RGB(0,0,0) };
		cascade.detectMultiScale(resize_image, cars, 1.1, 3, 0, Size(10, 10), Size(80, 80));
		cout << cars.size();

		Mat mask(img.size(), CV_8UC1, Scalar::all(0));

		for (vector<Rect>::const_iterator main = cars.begin(); main != cars.end(); main++, i++)
		{

			mask(cars[i]).setTo(Scalar::all(255));
			Scalar color = colors[i % 8];
			int x0 = cvRound(main->x);
			int y0 = cvRound(main->y);
			int x1 = cvRound((main->x + main->width - 1));
			int y1 = cvRound((main->y + main->height - 1));

			rectangle(image_main_result, cvPoint(x0, y0), cvPoint(x1, y1), Scalar(0, 255, 255), 1.5, 0);
			//	rectangle(image_main_result, main->tl(),main->br(), Scalar(0, 255, 255), 1.5, 0);//detecting boundary rectangle over the final result

		}
		//	imshow("Mask", mask);
		return mask;
	}




}