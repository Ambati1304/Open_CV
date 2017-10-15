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