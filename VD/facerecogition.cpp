#include "facerecogition.h"
#include <QDebug>
#include "sqlconnection.h"
#include <QtCore>
FaceRecogition::FaceRecogition()
{
    model = createFisherFaceRecognizer();
    computed = false;
}

FaceRecogition::~FaceRecogition()
{

}

void FaceRecogition::loadImages(){
    images.clear();
    labels.clear();
    read_files();
    if (images.size() > 0)
    {
        try
        {
            model->train(images, labels);
            im_width = images[0].cols;
            im_height = images[0].rows;
            computed = true;
        }
        catch(cv::Exception& e )
        {
            qDebug() << e.err.c_str();
        }
    }
}

void FaceRecogition::read_files()
{
    vector<int> tmpemps = SqlConnection::getEmployeeIdList();
    if(tmpemps.size() > 1)
    {
        for(std::vector<int>::iterator it = tmpemps.begin(); it != tmpemps.end(); it++)
        {
            QDir dir(cv::format("photos/s%d/", *it).c_str());
            if (dir.exists())
            {
                QStringList lis = dir.entryList(QStringList("*.jpg"));

                for (QStringList::iterator it2 = lis.begin();it2 != lis.end(); ++it2) {
                    QString current = *it2;
                    Mat tmp_img = imread(cv::format("photos/s%d/%s", *it, current.toStdString().c_str()), CV_LOAD_IMAGE_GRAYSCALE);
                    if (tmp_img.data != NULL)
                    {
                        images.push_back(tmp_img);
                        labels.push_back(*it);
                    }
                }
            }
        }
}

    }
//    ifstream file(filename.c_str(), ifstream::in);
//    if(!file){
//        qDebug() << "No valid input file";
//        return;
//    }
//    string line, path, classlabel;
//    while(getline(file, line)){
//        stringstream liness(line);
//        getline(liness, path, separator);
//        getline(liness, classlabel);
//        if(!path.empty() && !classlabel.empty() ){
//            Mat tmp = imread(path, CV_LOAD_IMAGE_GRAYSCALE);
//            if (tmp.data != NULL)
//            {
//                images.push_back(tmp);
//                labels.push_back(atoi(classlabel.c_str()));
//            }
//        }
//    }

