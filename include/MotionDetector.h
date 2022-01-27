#pragma once
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

class motion_detector_base {
private:
	cv::Ptr<cv::BackgroundSubtractor> p_back_sub_;
	cv::Mat mask_;
	cv::Mat background_;
	int64_t background_frames_collected_ = 0;
	int scale_ = 2;
	int kernel_ = 5;

	[[nodiscard]] std::vector<cv::Rect> find_bboxes(const cv::Mat& mask) const;
	void scale_rect(cv::Rect &rect) const;

	void show_image(const cv::Mat& frame) const {
		cv::imshow("frame", frame);
		cv::waitKey(1);
	}

public:
	motion_detector_base();
	/*virtual*/ ~motion_detector_base();

	void init(int64_t history, int64_t threshold, bool detect_shadows);
	std::vector<cv::Rect> add_frame(const cv::Mat& frame);
	[[nodiscard]] const cv::Mat& get_background() const;

	void set_gauss_kernel(const int v) {
		kernel_ = v;
	}

	void set_md_scale (const int v) {
		scale_ = v;
	}
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
