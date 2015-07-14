#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public slots:
    void processFrameAndUpdateGUI();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    cv::VideoCapture capWebcam;         // Capture object to use with webcam

    cv::Mat matOriginal;                // input image

    QTimer* qtimer;             // timer for processFrameAndUpdateGUI()

    QImage matToQImage(cv::Mat mat);       // function prototype

    void exitProgram();                    // function prototype

};

#endif // MAINWINDOW_H
