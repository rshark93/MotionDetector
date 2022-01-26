#include "MotionDetector.h"

using namespace cv;

motion_detector_base::motion_detector_base() {
	std::cout << "MotionDetector()" << std::endl;
}

motion_detector_base::~motion_detector_base() {
	std::cout << "~MotionDetector()" << std::endl;
}

void motion_detector_base::init(const int64_t history, const int64_t threshold, const bool detect_shadows) {
	if (!p_back_sub_.empty())
		p_back_sub_.release();

	p_back_sub_ = createBackgroundSubtractorMOG2(history, threshold, detect_shadows);
	p_back_sub_->setShadowValue(0);
}

std::vector<cv::Rect> motion_detector_base::add_frame(const cv::Mat& frame) {
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

		return find_bboxes(mask_);
	}
	return std::vector<Rect>();
}

std::vector<cv::Rect> motion_detector_base::find_bboxes(const cv::Mat& mask) const {
	std::vector<Rect> found_bboxes;
	std::vector<std::vector<Point> > contours;
	std::vector<Vec4i> hierarchy;
	findContours(mask, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);
	for (const auto& contour : contours)
		found_bboxes.push_back(boundingRect(contour));
	return found_bboxes;
}

const Mat& motion_detector_base::get_background() const {
	return background_;
}
