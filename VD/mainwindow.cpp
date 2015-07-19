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

    else qDebug() << "LOAD XML OK";
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

    faceCascade.detectMultiScale(greyScaleFrame, faces, 1.2,3, CV_HAAR_SCALE_IMAGE, cv::Size(50,50));

    // draw a rectangle for all found faces
    for(vector<cv::Rect>::iterator it = faces.begin(); it != faces.end(); ++it){

        cv::rectangle(matOriginal, cv::Point(it->x + it->width, it->y + it->height), cv::Point(it->x, it->y), cv::Scalar(0,255,0,0), 1, 8, 0);
        cv::Mat face = greyScaleFrame(*it);
        cv::Mat faceResized;
        cv::resize(face, faceResized, cv::Size(faceRec.im_width, faceRec.im_height), 1.0, 1.0, cv::INTER_CUBIC);

        double confidence = 0.0;
        int prediction  = -1;
        if (faceRec.model->empty()){
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

    if(mat.channels() == 3) {                    // if 3 channel color image
        cv::cvtColor(mat, mat, CV_BGR2RGB);             // flip colors
        return QImage((uchar*)mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);       // return QImage
    } else {
        qDebug() << "in openCVMatToQImage, image was not 1 channel or 3 channel, should never get here";
    }
    return QImage();        // return a blank QImage if the above did not work
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
    cv::Mat img;
    cv::cvtColor(matOriginal, img, CV_RGB2BGR);
    QListWidgetItem *item = new QListWidgetItem;
    ui->thumbnailsList->addItem(item);
    item->setIcon(QIcon(QPixmap::fromImage((matToQImage(Mat(img))))));
    cv::cvtColor(matOriginal, img, CV_BGR2RGB);
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
}


void MainWindow::saveImages(){
    int i = 0;
    for (vector<Mat>::iterator it = recordedImages.begin(); it != recordedImages.end(); ++it)
    {
        ++i;
        imwrite(format("s%d_%d.jpg",recordedImages.size() -1, i), *it);
    }
}
