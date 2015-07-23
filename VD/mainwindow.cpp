#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCore>
#include <QMessageBox>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/face/facerec.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <sqlconnection.h>


#define PI 3.14159265
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPushButton *save = ui->buttonBox->button(QDialogButtonBox::Save);
    save->setEnabled(false);


    if (!faceCascade.load("haarcascade_frontalface_alt.xml")) {
            qDebug() << "Unable to load";
        }
    else qDebug() << "LOAD FRONTALFACE XML OK";
    if(!eye_cascade.load("haarcascade_eye.xml")) qDebug() << "can't load eye cascade";
    else qDebug() << "load eye cascade OK";
    nb_enr = 0;
    faceRec = FaceRecogition();
    faceRec.loadImages();

    capWebcam.open(0);// associate the capture object to the default webcam

    if(capWebcam.isOpened() == false) { // if unsuccessful
        QMessageBox::information(this, "", "error: capWebcam not accessed successfully \n\n exiting program\n");        // show error message
        exitProgram();                          // and exit program
        return;                                 //
    }
    connect(ui->lineEdit, SIGNAL(textChanged(QString)), this, SLOT(checkAllRegistered()));
    connect(ui->lineEdit_2, SIGNAL(textChanged(QString)), this, SLOT(checkAllRegistered()));
    connect(ui->dateEdit, SIGNAL(dateChanged(QDate)), this, SLOT(checkAllRegistered()));
    connect(ui->buttonBox->button(QDialogButtonBox::Save), SIGNAL(clicked()), this, SLOT(saveEmployeeData()));
    connect(ui->buttonBox->button(QDialogButtonBox::Reset), SIGNAL(clicked()), this, SLOT(resetContents()));

    qtimer = new QTimer(this);  // instantiate timer
    connect(qtimer, SIGNAL(timeout()), this, SLOT(processFrameAndUpdateGUI()));// associate timer to processFrameAndUpdateGUI
    qtimer->start(20); // start timer, set to cycle every 20 msec (50x per sec), it will not actually cycle this often
}

MainWindow::~MainWindow()
{
    delete ui;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::exitProgram() {
    if(qtimer->isActive()) qtimer->stop();          // if timer is running, stop timer
    QApplication::quit();                           // and exit program
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::processFrameAndUpdateGUI() {
    bool blnFrameReadSuccessfully = capWebcam.read(matOriginal);                    // get next frame from the webcam

    if (!blnFrameReadSuccessfully || matOriginal.empty()) {                            // if we did not get a frame
        QMessageBox::information(this, "", "unable to read from webcam \n\n exiting program\n");        // show error via message box
        exitProgram();                                                                              // and exit program
        return;                                                                                     //
    }
    cv::cvtColor(matOriginal, greyScaleFrame, CV_BGR2GRAY);
    cv::equalizeHist(greyScaleFrame, greyScaleFrame);
    vector<cv::Rect> faces;
    faceCascade.detectMultiScale(greyScaleFrame, faces, 1.2,3, CV_HAAR_SCALE_IMAGE, cv::Size(matOriginal.size().width*0.2,matOriginal.size().height*0.2));

    // draw a rectangle for all found faces

    for(vector<cv::Rect>::iterator it = faces.begin(); it != faces.end(); ++it){

        cv::rectangle(matOriginal, cv::Point(it->x + it->width, it->y + it->height), cv::Point(it->x, it->y), cv::Scalar(0,255,0,0), 1, 8, 0);
        cv::Mat face = greyScaleFrame(*it);
        cv::Mat faceResized;
        cv::resize(face, faceResized, cv::Size(faceRec.im_width, faceRec.im_height), 1.0, 1.0, cv::INTER_CUBIC);


        int tlY = it->y;
        if(tlY < 0){
            tlY = 0;
        }
        int drY = it->y + it->height;
        if(drY > matOriginal.rows){
            drY = matOriginal.rows;
        }
        Point tl(it->x, tlY);
        Point dr(it->x + it->width, drY);
        Rect myROI(tl, dr);
        Mat croppedImage_original = matOriginal(myROI);
        Mat croppedImageGray;

        cvtColor(croppedImage_original, croppedImageGray, CV_RGB2GRAY);
        vector<Rect> eyes;
        eye_cascade.detectMultiScale(croppedImageGray, eyes, 1.1, 3, CV_HAAR_DO_CANNY_PRUNING, Size(croppedImageGray.size().width*0.2, croppedImageGray.size().height*0.2));
        int eyeLeftX = 0;
        int eyeLeftY = 0;
        int eyeRightX = 0;
        int eyeRightY = 0;

        for(vector<cv::Rect>::iterator it_eyes = eyes.begin(); it_eyes != eyes.end(); ++it_eyes)
        {
            int tlY2 = it_eyes->y + it->y;
            if(tlY2 < 0)
            {
                tlY2 = 0;
            }
            int drY2 = it_eyes->y + it_eyes->height + it->y;
            if(drY2 > matOriginal.rows)
            {
                drY2 = matOriginal.rows;
            }

            Point tl2(it_eyes->x + it->x, tlY2);
            Point dr2(it_eyes->x + it_eyes->width + it->x, drY2);

            if(eyeLeftX == 0)
            {
                rectangle(matOriginal, tl2, dr2, Scalar(255,0,0));
                eyeLeftX = it_eyes->x;
                eyeLeftY = it_eyes->y;
            }
            else if(eyeRightX == 0)
            {
                rectangle(matOriginal, tl2, dr2, Scalar(255,0,0));
                eyeRightX = it_eyes->x;
                eyeRightY = it_eyes->y;
            }

        }

        if(eyeLeftX > eyeRightX){
            croppedImage = cropFace(greyScaleFrame, eyeRightX, eyeRightY, eyeLeftX, eyeLeftY, 200, 200, it->x, it->y, it->width, it->height);
        }
        else{
            croppedImage = cropFace(greyScaleFrame, eyeLeftX, eyeLeftY, eyeRightX, eyeRightY, 200,200, it->x, it->y, it->width, it->height);
        }
        if(croppedImage.data != NULL)
        {
            if(!ui->pushButton_2->isEnabled()) ui->pushButton_2->setEnabled(true);
        }
        else{
            if(ui->pushButton_2->isEnabled()) ui->pushButton_2->setEnabled(false);
        }
        rectangle(matOriginal, tl, dr, Scalar(255,0,255));
        double confidence = 0.0;
        int prediction  = -1;
        if (faceRec.computed && !faceRec.model->empty() && croppedImage.data != NULL ){
            cv::resize(croppedImage, croppedImage, cv::Size(faceRec.im_width, faceRec.im_height), 1.0, 1.0, cv::INTER_CUBIC);

            faceRec.model->predict(faceResized, prediction, confidence);
            string box_text = cv::format("Prediction = %d COnfidence = %f", prediction, confidence);
            int posx = max(it->tl().x - 10, 0);
            int posy = max(it->tl().y - 10, 0);
            cv::putText(matOriginal, box_text, cv::Point(posx, posy), cv::FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0), 2.0);
        }
    }


    QImage qimgOriginal = matToQImage(matOriginal);             // convert from OpenCV Mat to Qt QImage
    qimgOriginal = qimgOriginal.scaled(qimgOriginal.size() / 2, Qt::KeepAspectRatio);
    ui->lblOriginal->setPixmap(QPixmap::fromImage(qimgOriginal));       // show images on form labels
    ui->frameGest->setPixmap(QPixmap::fromImage(qimgOriginal));
}
///////////////////////////////////////////////////////////////////////////////////////////////////
QImage MainWindow::matToQImage(cv::Mat mat) {
    if(mat.channels() == 1) {                           // if 1 channel (grayscale or black and white) image
            return QImage((uchar*)mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);     // return QImage
    }

    else if(mat.channels() == 3) {                    // if 3 channel color image
        cv::cvtColor(mat, mat, CV_BGR2RGB);             // flip colors
        return QImage((uchar*)mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);       // return QImage
    } else {
        qDebug() << "in openCVMatToQImage, image was not 1 channel or 3 channel, should never get here";
    }
    return QImage();        // return a blank QImage if the above did not work
}

Mat MainWindow::cropFace(Mat srcImg, int eyeLeftX, int eyeLeftY, int eyeRightX, int eyeRightY, int width, int height, int faceX, int faceY, int faceWidth, int faceHeight)
{
    Mat dstImg;
    Mat crop;

    if(!(eyeLeftX == 0 && eyeLeftY == 0)){
        int eye_directionX = eyeRightX -  eyeLeftX;
        int eye_directionY = eyeRightY - eyeLeftY;
        float rotation = atan2((float)eye_directionY, (float)eye_directionX)*180/ PI;
        rotate(srcImg, rotation, dstImg);
    }
    else
    {
         Point tl(faceX, faceY);
         Point dr((faceX + faceWidth), (faceY + faceHeight));
         Rect myROI(tl, dr);
         Mat croppedImage_original = srcImg(myROI);
         Mat nose_position_image;
         cv::resize(croppedImage_original, nose_position_image, Size(200,200), 0,0, INTER_CUBIC);
         float  rotation = gradienty(nose_position_image);
         rotate(srcImg, rotation, dstImg);
    }

    vector<Rect> tmp_faces;
    faceCascade.detectMultiScale(dstImg, tmp_faces, 1.2,3 ,CV_HAAR_DO_CANNY_PRUNING, Size(dstImg.size().width*0.2, dstImg.size().height*0.2));
    for(vector<cv::Rect>::iterator it_face = tmp_faces.begin(); it_face != tmp_faces.end(); ++it_face)
    {
        int tlY = it_face->y;
        if(tlY < 0)
        {
            tlY = 0;
        }
        int drY = it_face->y + it_face->height;
        if(drY > dstImg.rows)
        {
            drY = dstImg.rows;
        }
        Point tl(it_face->x, tlY);
        Point dr(it_face->x + it_face->width, drY);

        Rect myROI (tl, dr);
        Mat croppedImage_original = dstImg(myROI);
        Mat croppedImageGray;
        cv::resize(croppedImage_original, crop, Size(width, height), 0, 0, INTER_CUBIC);
    }
    return crop;
}

void MainWindow::rotate(Mat& src, double angle, Mat& dst){
    int len = max(src.cols, src.rows);
    Point2f pt(len / 2., len/2.);
    Mat r =  getRotationMatrix2D(pt, angle, 1.0);
    warpAffine(src, dst, r, Size(len, len));
}




void MainWindow::on_pushButton_clicked()
{
    nb_enr++;
    cv::Mat img;
    cv::cvtColor(matOriginal, img, CV_RGB2BGR);
    cv::imwrite(cv::format("image%d.jpg", nb_enr), img );
}

void MainWindow::on_pushButton_2_clicked()
{
    cv::Mat img(croppedImage);
    QListWidgetItem *item = new QListWidgetItem;
    ui->thumbnailsList->addItem(item);
    item->setIcon(QIcon(QPixmap::fromImage((matToQImage(Mat(img))))));
    recordedImages.push_back(img);
}


void MainWindow::checkAllRegistered(){

    if(ui->lineEdit->text().size() > 0 && ui->lineEdit_2->text().size() > 0 &&  !ui->buttonBox->button(QDialogButtonBox::Save)->isEnabled()){
        ui->buttonBox->button(QDialogButtonBox::Save)->setEnabled(true);
    }
    else if((ui->lineEdit->text().size() == 0 || ui->lineEdit_2->text().size() == 0) && ui->buttonBox->button(QDialogButtonBox::Save)->isEnabled())
    {
        ui->buttonBox->button(QDialogButtonBox::Save)->setEnabled(false);
    }
}

void MainWindow::saveEmployeeData(){
    bool tmp_gender;
    if (ui->comboBox->currentIndex() == 0) tmp_gender = false;
    else tmp_gender = true;
    emps.push_back(Employee(ui->lineEdit->text().toStdString(), ui->lineEdit_2->text().toStdString(), tmp_gender, ui->dateEdit->date()));
    SqlConnection::insertEmployee(emps[emps.size() -1]);
    saveImages();
    resetContents();
}


void MainWindow::resetContents(){
    ui->lineEdit->setText("");
    ui->lineEdit_2->setText("");
    ui->thumbnailsList->clear();
    emps.clear();
    faceRec.loadImages();
}


void MainWindow::saveImages(){
    QDir dir((cv::format("photos/s%d/", emps[0].getId())).c_str());
    if(!dir.exists()) dir.mkpath(".");
    int i = 0;
    for (vector<Mat>::iterator it = recordedImages.begin(); it != recordedImages.end(); ++it)
    {
        ++i;
        imwrite(format("photos/s%d/s%d_%d.jpg",emps[0].getId(), emps[0].getId(), i), *it);
    }
}

float MainWindow::gradienty(Mat frame){

    Mat src, src_gray;
    int scale = 1;
    int delta = 0;
    src_gray = frame;
    /// Generate grad_x and grad_y
    Mat grad_x, grad_y;
    Mat abs_grad_x, abs_grad_y;
    Mat magnitudes, angles;
    Mat bin;
    Mat rotated;
    int max = 0;
    int uhol = 0;
    // Rotate image to angles in which it is still possible to detect shape
    for (int i = -50; i < 50; i++)
    {
        rotate(src_gray, ((double)i / PI), rotated);
        // We apply vertical and horizontal Sobel
        Sobel(rotated, grad_x, CV_32F, 1, 0, 9, scale, delta, BORDER_DEFAULT);
        Sobel(rotated, grad_y, CV_32F, 0, 1, 9, scale, delta, BORDER_DEFAULT);
        // we calculate the angles
        cartToPolar(grad_x, grad_y, magnitudes, angles);
        // We converted to gray scale for better representation
        angles.convertTo(bin, CV_8U, 90 / PI);
        // cutout nose which is situated in the middle of the head +-
        Point tl((bin.cols / 2) - 10, (bin.rows / 2) - 20);
        Point dr((bin.cols / 2) + 10, (bin.rows / 2));
        Rect myROI(tl, dr);
        Mat working_pasik = bin(myROI);
        int current = 0;
        // we calculate the histogram and the number of angles in standard
        current = plotHistogram(working_pasik);
        // choose maximum
        if (current > max)
        {
            max = current;
            uhol = i;
        }
    }
    //add the angle to queue
    noseQueue.push_back(uhol);
    int suma = 0;

    //We calculate all the angles in the queue
    for (std::list<int>::iterator it = noseQueue.begin(); it != noseQueue.end(); it++)
    {

        suma = suma + *it;
    }
    int priemer;
    // We calculate the mean angle for the last 4 frames
    priemer = (int)((double)suma / (double)noseQueue.size());
    if (noseQueue.size() > 3)
    {
        noseQueue.pop_front();
    }
    return priemer;

}

int MainWindow::plotHistogram(Mat image)
{
    Mat dst;

    /// Establish the number of bins
    int histSize = 256;

    /// Set the ranges
    float range[] = { 0, 256 };
    const float* histRange = { range };
    bool uniform = true; bool accumulate = false;
    Mat b_hist, g_hist, r_hist;
    /// Compute the histograms:
    calcHist(&image, 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate);
    int hist_w = 750; int hist_h = 500;
    Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

    /// Normalize the result to [ 0, histImage.rows ]
    normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
    int max = 0;
    int now;
    for (int i = 1; i < histSize; i++)
    {
        now = cvRound(b_hist.at<float>(i));
        if ((i < 5))
        {
            max += now;
        }
    }
    return max;
}
