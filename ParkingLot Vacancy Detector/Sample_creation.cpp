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