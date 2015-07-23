#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/core/core.hpp>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/face/facerec.hpp>
#include <string>
#include "facerecogition.h"
#include "employee.h"
using namespace cv::face;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public slots:

    void processFrameAndUpdateGUI();
    void checkAllRegistered();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void saveEmployeeData();
    void resetContents();


private:
    int nb_enr;
    Ui::MainWindow *ui;
    cv::VideoCapture capWebcam;         // Capture object to use with webcam
    cv::CascadeClassifier faceCascade; //harrcascade frontal face classifier
    cv::CascadeClassifier eye_cascade; // haarcascasde eye classifier;
    cv::Mat matOriginal;                // input image
    cv::Mat greyScaleFrame; // grey sclae frame
    vector<cv::Mat> recordedImages; //Vector for recorded pictures
    cv::Mat croppedImage;
    vector<Employee> emps; //
    std::list<int> noseQueue;

    FaceRecogition faceRec;

    QTimer* qtimer;             // timer for processFrameAndUpdateGUI()

    QImage matToQImage(cv::Mat mat);       // function prototype

    void exitProgram();                    // function prototype
    void saveImages();
    Mat cropFace(cv::Mat srcImg, int eyeLeftX, int eyeLeftY, int eyeRightX, int eyeRightY, int width, int height, int faceX, int faceY, int faceWidth, int faceHeight);
    void rotate(Mat& src, double angle, Mat& dst);
    float gradienty(Mat frame);
    int plotHistogram(Mat image);
};

#endif // MAINWINDOW_H
