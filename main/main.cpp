#include "main.h"
#include "MotionDetector.h"

#include <iostream>
#include <sstream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

int history = 500;
double threshold = 16;
bool detect_shadows = true;
std::string stream_link = R"(http://st.infosevas.ru/cam_ost_nahimova_2/tracks-v1/mono.m3u8)";

int main() {
	cv::Mat frame;
	cv::VideoCapture capture;
	capture.open(0); //stream_link
	cv::waitKey(1);

	if (!capture.isOpened()) {
		std::cout << "ERROR! Unable to open camera\n";
		return -1;
	}

	std::cout << "Start grabbing" << std::endl << "Press any key to terminate" << std::endl;

	auto obj_ptr = std::shared_ptr<motion_detector_base>();
	obj_ptr->init(history, threshold, detect_shadows);
	
	while (true) {
		capture >> frame;
		auto bboxes = obj_ptr->add_frame(frame);
	}
	
	return 0;
}