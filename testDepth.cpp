#include <opencv2/opencv.hpp>
#include <iostream>
#include <highgui.h>
#include <cv.h>
#include <cxcore.h>
#include <chrono>

using namespace cv;
using namespace std;
using namespace std::chrono;

cv::Mat SGBM_depth(cv::Mat img_l, cv::Mat img_r)
{
	cv::Ptr<cv::StereoSGBM> sgbm = cv::StereoSGBM::create();
	int SADWindowSize = 15;
	sgbm->setPreFilterCap(63);
	int sgbmWinSize = SADWindowSize > 0 ? SADWindowSize : 3;
	sgbm->setBlockSize(sgbmWinSize);
	int cn = img_l.channels();
	int numberOfDisparities = 64;
	sgbm->setP1(8 * cn * sgbmWinSize * sgbmWinSize);
	sgbm->setP2(32 * cn * sgbmWinSize * sgbmWinSize);
	sgbm->setMinDisparity(0);
	sgbm->setNumDisparities(numberOfDisparities);
	sgbm->setUniquenessRatio(10);
	sgbm->setSpeckleWindowSize(100);
	sgbm->setSpeckleRange(32);
	sgbm->setDisp12MaxDiff(1);
	sgbm->setMode(cv::StereoSGBM::MODE_SGBM);


	cv::Mat disp, disp8;
	int64 t = cv::getTickCount();
	sgbm->compute(img_l, img_r, disp);
	t = cv::getTickCount() - t;
	std::cout << "SGBM algorithm Time elapsed:" << t * 1000 / cv::getTickFrequency() << std::endl;
	disp.convertTo(disp8, CV_8U, 255 / (numberOfDisparities * 16.));
	return disp8;
}

cv::Mat BM_depth(cv::Mat img_l, cv::Mat img_r)
{
	int numberOfDisparities = 64;
	cv::Ptr<cv::StereoBM> bm = cv::StereoBM::create(16, 9);
	cv::Rect roi1, roi2;
	bm->setROI1(roi1);
	bm->setROI2(roi2);
	bm->setPreFilterCap(31);
	bm->setBlockSize(9);
	bm->setMinDisparity(0);
	bm->setNumDisparities(numberOfDisparities);
	bm->setTextureThreshold(10);
	bm->setUniquenessRatio(15);
	bm->setSpeckleWindowSize(100);
	bm->setSpeckleRange(32);
	bm->setDisp12MaxDiff(1);

	cv::Mat disp, disp8;
	int64 t = cv::getTickCount();
	bm->compute(img_l, img_r, disp);
	t = cv::getTickCount() - t;
	std::cout << "BM algorithm Time elapsed:" << t * 1000 / cv::getTickFrequency() << std::endl;
	disp.convertTo(disp8, CV_8U, 255 / (numberOfDisparities * 16.));
	return disp8;
}

int main()
{
	int i = 0;
	while (i++ < 100)
	{
		cv::Mat img_l = imread("C://Users//vivedu//Desktop//img//left.jpg", 0);
		cv::Mat img_r = imread("C://Users//vivedu//Desktop//img//right.jpg", 0);

		auto startTime = chrono::high_resolution_clock::now();
		cv::Mat disp8_sgbm = SGBM_depth(img_l, img_r);

		auto middleTime = chrono::high_resolution_clock::now();
		auto durationSGBM = std::chrono::duration_cast<std::chrono::milliseconds>(middleTime - startTime).count();
		cout << "SGBM duration: " << durationSGBM << "ms" << endl;

		cv::Mat disp8_bm = BM_depth(img_l, img_r);

		auto endTime = chrono::high_resolution_clock::now();
		auto durationBM = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - middleTime).count();
		cout << "BM duration: " << durationBM << "ms" << endl;

		/*
		imshow("left", img_l);
		imshow("right", img_r);
		imshow("disparity-sgbm", disp8_sgbm);
		imshow("disparity-bm", disp8_bm);
		*/

		//imwrite("C://Users//vivedu//Desktop//img//sgbm_disparity.png", disp8_sgbm);
		//imwrite("C://Users//vivedu//Desktop//img//bm_disparity.png", disp8_bm);
	}
	system("pause");
	return 0;
}
