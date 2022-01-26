#pragma once
#include <iostream>
#include <opencv2/video.hpp>

class motion_detector_base {
protected:
	cv::Ptr<cv::BackgroundSubtractorMOG2> p_back_sub_;
	cv::Mat mask_;
	cv::Mat background_;
	int64_t background_frames_collected_ = 0;
	const int scale_ = 4;

	[[nodiscard]] std::vector<cv::Rect> find_bboxes(const cv::Mat& mask) const;
public:
	motion_detector_base();
	virtual ~motion_detector_base();

	void init(int64_t history, int64_t threshold, bool detect_shadows);
	std::vector<cv::Rect> add_frame(const cv::Mat& frame);
	[[nodiscard]] const cv::Mat& get_background() const;
};

//class motion_detector final : public motion_detector_base {
//public:
//	motion_detector() {
//		std::cout << "motion_detector" << std::endl;
//	}
//
//	~motion_detector() {
//		std::cout << "motion_detector" << std::endl;
//	}
//};
