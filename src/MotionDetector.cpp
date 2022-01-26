#include "MotionDetector.h"

using namespace cv;

motion_detector::motion_detector() {
	std::cout << "MotionDetector()" << std::endl;

	//init()
}

motion_detector::~motion_detector() {
	std::cout << "~MotionDetector()" << std::endl;
}

void motion_detector::init(const int64_t history, const int64_t threshold, const bool detect_shadows)
{
	if (!p_back_sub_.empty())
		p_back_sub_.release();

	p_back_sub_ = createBackgroundSubtractorMOG2(history, threshold, detect_shadows);
	p_back_sub_->setShadowValue(0);
}

std::vector<cv::Rect> motion_detector::add_frame(const cv::Mat& frame)
{
	Mat resized;

	if (!frame.empty()) {
		try {
			resize(frame, resized, Size(frame.cols / scale_, frame.rows / scale_));
		} catch (std::invalid_argument& e) {
			std::cout << "resize_add_frame_err!!!" << &e << std::endl;
		}

		p_back_sub_->apply(resized, mask_);
		p_back_sub_->getBackgroundImage(background_);
		++background_frames_collected_;

		// TODO find contours & rectangles
		return std::vector<Rect>();
	}
	return std::vector<Rect>();
}

const Mat& motion_detector::get_background() const
{
	return background_;
}
