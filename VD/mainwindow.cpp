#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCore>
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    capWebcam.open(0);// associate the capture object to the default webcam

    if(capWebcam.isOpened() == false) { // if unsuccessful
        QMessageBox::information(this, "", "error: capWebcam not accessed successfully \n\n exiting program\n");        // show error message
        exitProgram();                          // and exit program
        return;                                 //
    }
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

    QImage qimgOriginal = matToQImage(matOriginal);             // convert from OpenCV Mat to Qt QImage

    ui->lblOriginal->setPixmap(QPixmap::fromImage(qimgOriginal));       // show images on form labels
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
