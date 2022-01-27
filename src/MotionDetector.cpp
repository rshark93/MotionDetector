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

	auto sub = createBackgroundSubtractorMOG2(history, threshold, detect_shadows);
	sub->setShadowValue(0);
	p_back_sub_ = sub;
}

std::vector<cv::Rect> motion_detector_base::add_frame(const cv::Mat& frame) {
	Mat resized;

	if (!frame.empty()) {
		try {
			if (scale_ != 1) {
				resize(frame, resized, Size(frame.cols / scale_, frame.rows / scale_));
				resize(frame, resized, Size(frame.cols * scale_, frame.rows * scale_));
			}
			
		} catch (std::invalid_argument& e) {
			std::cout << "resize_add_frame_err!!!" << &e << std::endl;
		}

		p_back_sub_->apply(frame /*resized*/, mask_);
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

	cv::GaussianBlur(mask, mask, Size(kernel_, kernel_), 0);
	cv::threshold(mask, mask, 100, 255, THRESH_BINARY);
	show_image(mask);
	
	findContours(mask, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);
	for (const auto& contour : contours) {
		auto rect = boundingRect(contour);
		//scale_rect(rect);
		found_bboxes.push_back(rect);
	}
	return found_bboxes;
}

const Mat& motion_detector_base::get_background() const {
	return background_;
}


void motion_detector_base::scale_rect(cv::Rect &rect) const
{
	rect.x *= scale_;
	rect.y /= scale_;
	rect.width *= scale_;
	rect.height *= scale_;	
}
