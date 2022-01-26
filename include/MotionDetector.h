#pragma once
#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

class motion_detector {
private:
	cv::Ptr<cv::BackgroundSubtractorMOG2> p_back_sub_;
	cv::Mat mask_;
	cv::Mat background_;
	int64_t background_frames_collected_ = 0;
	const int scale_ = 4;
public:
	motion_detector();
	~motion_detector();

	void init(int64_t history, int64_t threshold, bool detect_shadows);
	std::vector<cv::Rect> add_frame(const cv::Mat& frame);
	
	[[nodiscard]] const cv::Mat& get_background() const;
};
