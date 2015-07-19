#ifndef FACERECOGITION_H
#define FACERECOGITION_H


#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/face/facerec.hpp>

#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;
using namespace cv;
using namespace cv::face;

class FaceRecogition
{
public:
    FaceRecogition();
    ~FaceRecogition();
    Ptr<FaceRecognizer> model;
    int im_width;
    int im_height;
    void loadImages();

private:
    vector<Mat> images;
    vector<int> labels;

private:

    void read_csv(const string &filename, char separator=';');

};

#endif // FACERECOGITION_H
