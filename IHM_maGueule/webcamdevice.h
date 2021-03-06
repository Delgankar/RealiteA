#ifndef WEBCAMDEVICE_H
#define WEBCAMDEVICE_H

#include <QThread>
#include <QFileDialog>
#include <QMessageBox>

#include <opencv2/opencv.hpp>
#include <Chehra.h>

#define NBR_DETECTION_MODES 4
#define NBRSAVEDIMAGES      5
#define NBRFACEPOINTSDETECTED   6

#define NBRIMAGESCALIB	20
#define COLCHESSBOARD	9
#define ROWCHESSBOARD	6
#define SAVEDPATH "../rsc/mires/mire"

enum detectMode{noDetection = 0, faceDetection = 1, chessDetection = 2, qrDetection = 3};


class WebcamDevice : public QThread
{
    Q_OBJECT
public:
    explicit WebcamDevice(QObject *parent = 0);
    ~WebcamDevice();

    cv::Mat* getWebcam() { return _frame; }

    void stop() { _isRunning = false; }
    void play() { _isRunning = true; }

    void initModels();

signals:
    void updateWebcam();
    void updateScene(cv::Mat, cv::Mat);
    void shutdownSignal();

public slots:
  void switchMode(int mode) { _detect = (detectMode)mode; }

protected:
    void run();

private:
    // member

    void calibrateCam(FileStorage *fs);
    void calibrate();
    bool detecterVisage(std::vector<cv::Point2f> *pointsVisage);
    void faceRT();
    void chessRT();
    void qrRT();


    // attributes

    bool _isRunning;
    detectMode _detect;
    cv::VideoCapture _vcap;
    cv::Mat _rotVecs;
    cv::Mat _tvecs;
    cv::Mat _cameraMatrix;
    cv::Mat _distCoeffs;
    cv::Mat* _frame;

    Chehra* _chehra;
    std::vector<cv::Point3f> _pointsVisage3D;
    std::vector<std::vector<cv::Point2f>> _images;


    int _nbrColChess;
    int _nbrRowChess;
    float _chessSize;

    int _nbrLoopSinceLastDetection;
    double _focalePlane;
    //double _corrector;
};

#endif // WEBCAMDEVICE_H
