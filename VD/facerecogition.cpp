#include "facerecogition.h"
#include <QDebug>
FaceRecogition::FaceRecogition()
{
    model = createFisherFaceRecognizer();


}

FaceRecogition::~FaceRecogition()
{

}

void FaceRecogition::loadImages(){
    read_csv("csv_images.csv");
    if (images.size() > 0)
    {
        model->train(images, labels);
        im_width = images[0].cols;
        im_height = images[0].rows;
    }

}


void FaceRecogition::read_csv(const string &filename, char separator)
{
    ifstream file(filename.c_str(), ifstream::in);
    if(!file){
        qDebug() << "No valid input file";
        return;
    }
    qDebug() << "file exists";
    string line, path, classlabel;
    while(getline(file, line)){
        stringstream liness(line);
        getline(liness, path, separator);
        getline(liness, classlabel);
        if(!path.empty() && !classlabel.empty() ){
            Mat tmp = imread(path, CV_LOAD_IMAGE_GRAYSCALE);
            if (tmp.data != NULL)
            {
                images.push_back(tmp);
                labels.push_back(atoi(classlabel.c_str()));
            }

        }
    }
}
