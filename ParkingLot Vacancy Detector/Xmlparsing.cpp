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

	//parse the document
	string content(buffer.str());
	doc.parse<0>(&content[0]);


	int total_count = 0;
	int truePositive = 0;
	int falsePositive = 0;
	int trueNegative = 0;
	xml_node<> *pRoot = doc.first_node();
	for (xml_node<> *pNode = pRoot->first_node("space"); pNode; pNode = pNode->next_sibling())
	{
		double occupany;
		int occupied1 = 0, nValue = 0, width1 = 0, height1 = 0, x_point = 0, y_point = 0, centerX = 0, centerY = 0, angle = 0;
		char line[30] = "\0";
		xml_node<> *rotatedRect = pNode->first_node("rotatedRect");
		xml_attribute<> *occupied = pNode->first_attribute("occupied");
		xml_node<> *center_node = rotatedRect->first_node("center");
		xml_attribute<> *center_x = center_node->first_attribute("x");
		xml_attribute<> *center_y = center_node->first_attribute("y");
		xml_node<> *size = rotatedRect->first_node("size");
		xml_attribute<> *width = size->first_attribute("w");
		xml_attribute<> *height = size->first_attribute("h");
		xml_node<> *angleNode = rotatedRect->first_node("angle");
		xml_attribute<> * angleAttri = angleNode->first_attribute("d");
		xml_attribute<> *pAttr = center_node->first_attribute("y");
		xml_node<> *contour_node = pNode->first_node("contour");
		xml_node<> *points = contour_node->first_node("point");
		xml_attribute<> *x = points->first_attribute("x");
		xml_attribute<> *y = points->first_attribute("y");


		if (occupied != nullptr && pAttr != nullptr&&width != nullptr&&height != nullptr&&x != nullptr&&y != nullptr) {
			occupied1 = atoi(occupied->value());
			nValue = atoi(pAttr->value());
			width1 = atoi(width->value());
			height1 = atoi(height->value());
			x_point = atoi(x->value());
			y_point = atoi(y->value());
			centerX = atoi(center_x->value());
			centerY = atoi(center_y->value());
			angle = atoi(angleAttri->value());



			boundbox = cv::RotatedRect(Point2f(centerX, centerY), Size2f(width1, height1), angle);
			occupany = test(boundbox);
			total_count++;
		}

		if (occupied1 == 1 && occupany > threshold) {
			truePositive++;
		}
		else if (occupied1 == 0 && width1 >0 && height1>0 && occupany >threshold)
		{
			falsePositive++;
		}
		else if (occupied1 == 0 && width1 >0 && height1>0 && occupany <threshold)
		{
			trueNegative++;
		}

	}

	cout << "True Postitive " << truePositive << endl;
	cout << "False Postitive " << falsePositive << endl;
	cout << "True Negative " << trueNegative << endl;
	cout << "Total Count" << total_count << endl;
	cout << "Accuracy " << 100 * (truePositive + trueNegative) / total_count << endl;



		}



	double Xmlparsing::test(RotatedRect annotBox) {
		
		Mat temp(detectedMask.size(), CV_8UC1, Scalar::all(0));
		//detectedMask.copyTo(temp(annotBox));
		Mat M, rotated, cropped;
		M = getRotationMatrix2D(annotBox.center, annotBox.angle, 1.0);
		warpAffine(detectedMask, rotated, M, detectedMask.size(), INTER_CUBIC);
		getRectSubPix(rotated, annotBox.size, annotBox.center, cropped);
		double pixelSum = cv::sum(cropped)[0];
		double occupancy = pixelSum / (cropped.total() * 255) * 100;
		return occupancy;
	}
