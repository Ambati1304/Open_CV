#include<rapidxml\rapidxml_utils.hpp>
#include <sstream>
#include <iostream>
#include <fstream>
#include<opencv\cv.hpp>
#include <opencv2/core/core.hpp>
#include "opencv2/objdetect/objdetect.hpp"
#include <opencv2/highgui/highgui.hpp>
#include "Xmlparsing.h"

using namespace std;
using namespace rapidxml;
using namespace cv;
cv::RotatedRect boundbox;
Mat  detectedMask;

void Xmlparsing::readFile(string filename,Mat mask) {

	//read the xml document into file object
	detectedMask = mask;
	double threshold = 50;
	xml_document<> doc;
	ifstream file(filename);
	stringstream buffer;
	cout << filename;
	if (file.is_open())
	{

		buffer << file.rdbuf();
		file.close();
	}
	else {
		cout << endl << "Unable to open file" << endl;
		exit(0);
	}

		}



	double Xmlparsing::test(RotatedRect annotBox) {
		
			
	}
