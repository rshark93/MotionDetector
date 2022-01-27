#include "main.h"
#include "MotionDetector.h"

#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

auto scale = 2;
auto md_scale = 2;
auto history = 500;
auto threshold = 127.0;
auto kernel = 3;
auto detect_shadows = false;
auto stream_link = minsk_tsum;

int main(const int argc, char** argv)
{
	if (argc != 2) std::cout << "Usage:\t" << "[input stream link]" << std::endl;
	else stream_link = argv[1];
	
	cv::Mat frame;
	cv::VideoCapture capture;
	capture.open(stream_link);
	cv::waitKey(1);

	if (!capture.isOpened()) {
		std::cout << "ERROR! Unable to open camera\n";
		return -1;
	}

	std::cout << "Start grabbing" << std::endl << "Press any key to terminate" << std::endl;

	auto obj_ptr = std::make_shared<motion_detector_base>();
	obj_ptr->init(history, threshold, detect_shadows);
	obj_ptr->set_gauss_kernel(kernel);
	obj_ptr->set_md_scale(md_scale);
	
	while (true) {
		capture >> frame;

		if (scale != 1)
			cv::resize(frame, frame, frame.size() / scale);

		auto bboxes = obj_ptr->add_frame(frame);

		for (const auto &bbox : bboxes)
			rectangle(frame, bbox, cv::Scalar(255, 127, 0), 1);

		imshow("Frame", frame);
		cv::waitKey(1);

		//cv::imshow("FG Mask", obj_ptr->get_background());
	}
	
	return 0;
}