#include <rapidxml\rapidxml_utils.hpp>
#include <sstream>
#include <iostream>
#include <fstream>
#define BOOST_FILESYSTEM_VERSION 3
#define BOOST_FILESYSTEM_NO_DEPRECATED 
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <>

using namespace std;
namespace fs = ::boost::filesystem;
using namespace rapidxml;
void readFile(fs::path, fs::path);
void getFiles(fs::path);

ofstream pos_training_set, neg_training_set;

int main() {

	pos_training_set.open("positive.txt");
	neg_training_set.open("negative.txt");

	//Get the root node
	string currentpath = boost::filesystem::current_path().string();
	cout << currentpath;
	fs::path root[3] = { "D:\Masters\Spring'17\Computer Vision\Assignment 2\PKLot-small\PKLot\parking2\sunny","D:\Masters\Spring'17\Computer Vision\Assignment 2\PKLot-small\PKLot\parking1a\sunny","D:\Masters\Spring'17\Computer Vision\Assignment 2\PKLot-small\PKLot\parking1b\sunny" };
	for (int i = 0; i < 3; i++)
	{
		getFiles(root[i]);
	}

	pos_training_set.close();
	neg_training_set.close();
	return 0;
}

void getFiles(fs::path root)
{
	string ext = ".jpg";
	vector<fs::path> jpgPaths, xmlPaths;
	if (!fs::exists(root) || !fs::is_directory(root)) cout << "";

	fs::recursive_directory_iterator it(root);
	fs::recursive_directory_iterator endit;

	while (it != endit)
	{

		if (fs::is_regular_file(*it))
		{
			if (it->path().extension() == ".xml")
				xmlPaths.push_back(it->path());
			if (it->path().extension() == ".jpg")
				jpgPaths.push_back(it->path());
		}
		++it;

	}


	for (int i = 0; i <xmlPaths.size(); i++)
	{

		readFile(xmlPaths[i], jpgPaths[i]);

	}

}

void readFile(const fs::path filename, fs::path jpgfilename) {

	//read the xml document into file object
	xml_document<> doc;
	ifstream file(filename.string());
	stringstream buffer;
	buffer << file.rdbuf();
	file.close();

	//parse the document
	string content(buffer.str());
	doc.parse<0>(&content[0]);

	string line;
	ostringstream image_oss, oss_neg, oss_pos;
	string image_path = jpgfilename.string();
	string current_path = boost::filesystem::current_path().string();
	image_path = image_path.substr(current_path.length() - 1, image_path.length());
	image_oss << image_path << " ";
	int pos_count = 0;
	int neg_count = 0;
	xml_node<> *pRoot = doc.first_node();
	for (xml_node<> *pNode = pRoot->first_node("space"); pNode; pNode = pNode->next_sibling())
	{

		int occupied1 = 0, nValue = 0, width1 = 0, height1 = 0, x_point = 0, y_point = 0;
		char line[30] = "\0";
		xml_node<> *rotatedRect = pNode->first_node("rotatedRect");
		xml_attribute<> *occupied = pNode->first_attribute("occupied");
		xml_node<> *center_node = rotatedRect->first_node("center");
		xml_node<> *size = rotatedRect->first_node("size");
		xml_attribute<> *width = size->first_attribute("w");
		xml_attribute<> *height = size->first_attribute("h");
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
		}
		if (occupied1 == 1) {
			pos_count++;
			oss_pos << width1 << " " << height1 << " " << x_point << " " << y_point << " ";
		}
		else
		{
			neg_count++;
			oss_neg << width1 << " " << height1 << " " << x_point << " " << y_point << " ";
		}


	}
	if (pos_count > 0)
		pos_training_set << image_oss.str() << pos_count << " " << oss_pos.str() << endl;
	if (neg_count>0)
	{
		neg_training_set << image_oss.str() << neg_count << " " << oss_neg.str() << endl;
	}

}

