the folder consists of 2 trained xml files 
1) caslbp.xml --> trained with LBP features
2) cashaar.xml--> trained with Haar Features

Sample_creation.cpp is written to create samples from corresponding groundtruth.xml for training.
which needs Rapid.xml packages 

to install, open projet and type "Install-Package rapidxml" in nuget package manager console.

  Source.cpp will take in image, its corresponding .xml file and Trained Cascade.xml and performs object detection
which also automatically analyses the image and gives out accuracy along with True and False Positive.

the folder also contains one header file Xmlparsing.h