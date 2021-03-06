#include <opencv2/opencv.hpp>
#include <iostream>
#include <sstream>
#include <fstream>

#define NBRIMAGESCALIB	20
#define COLCHESSBOARD	9
#define ROWCHESSBOARD	6

int main()
{
	cv::Mat imCalib[NBRIMAGESCALIB];
	cv::Mat cameraMatrix, distCoeffs;

	cv::Mat rvecs, tvecs;
	std::vector<cv::Point2f> chessCornersInit;
	std::vector<cv::Point3f> chessCorners3D;

	for(int j = 0; j < COLCHESSBOARD; j++)
	{
		for(int k = 0; k < ROWCHESSBOARD; k++)
		{
			cv::Point3f corner(j * 26.0f, k * 26.0f, 0.0f);
			chessCorners3D.push_back(corner);
		}
	}

	cv::FileStorage fs("../rsc/intrinsicMatrix.yml", cv::FileStorage::READ);

    fs["cameraMatrix"] >> cameraMatrix;
    fs["distCoeffs"] >> distCoeffs;

    fs.release();

	for(int i = 0; i < NBRIMAGESCALIB; i++)
	{
		std::ostringstream oss;
		oss << "../rsc/mires/mire" << i + 1 << ".png";
		imCalib[i] = cv::imread(oss.str());
		cv::cvtColor(imCalib[i], imCalib[i], CV_BGR2GRAY);
	
		std::ostringstream filename;
		filename << "../rsc/mires/RTMatrix" << i + 1 << ".yml";
		cv::FileStorage fsRT(filename.str(), cv::FileStorage::WRITE);

		bool patternfound = cv::findChessboardCorners(imCalib[i], cv::Size(ROWCHESSBOARD, COLCHESSBOARD), chessCornersInit);

		if(patternfound)
			cv::cornerSubPix(imCalib[i], chessCornersInit, cv::Size(5, 5), cv::Size(-1, -1), cv::TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));

		cv::solvePnP(chessCorners3D, chessCornersInit, cameraMatrix, distCoeffs, rvecs, tvecs);

		cv::Mat rotVec(3, 3, CV_64F);
		cv::Rodrigues(rvecs, rotVec);

		cv::Mat matRT = cv::Mat::zeros(4, 4, CV_64F);

		matRT.at<double>(0, 0) = rotVec.at<double>(0, 0);
		matRT.at<double>(0, 1) = rotVec.at<double>(0, 1);
		matRT.at<double>(0, 2) = rotVec.at<double>(0, 2);
		matRT.at<double>(1, 0) = rotVec.at<double>(1, 0);
		matRT.at<double>(1, 1) = rotVec.at<double>(1, 1);
		matRT.at<double>(1, 2) = rotVec.at<double>(1, 2);
		matRT.at<double>(2, 0) = rotVec.at<double>(2, 0);
		matRT.at<double>(2, 1) = rotVec.at<double>(2, 1);
		matRT.at<double>(2, 2) = rotVec.at<double>(2, 2);

		matRT.at<double>(0, 3) = tvecs.at<double>(0);
		matRT.at<double>(1, 3) = tvecs.at<double>(1);
		matRT.at<double>(2, 3) = tvecs.at<double>(2);

		matRT.at<double>(3, 0) = 0;
		matRT.at<double>(3, 1) = 0;
		matRT.at<double>(3, 2) = 0;
		matRT.at<double>(3, 3) = 1;

		fsRT << "matriceRT" << matRT;
		fsRT << "matriceR" << rotVec;
		fsRT << "matriceT" << tvecs;

		fsRT.release();

		cv::imshow("image", imCalib[i]);

		cv::waitKey(0);
	}

	return 0;
}