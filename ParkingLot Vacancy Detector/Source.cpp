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
}