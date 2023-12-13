#pragma once

#include <iostream>
#include "opencv2/opencv.hpp"

// 버튼의 좌표와 크기를 나타내는 구조체
struct BUTTON {
	int x;
	int y;
	int w;
	int h;
};

// 마우스 포지션을 나타내는 구조체
struct MOUSE_POSE {
	int x;
	int y;
};

class PhotoShop {
private:
	// 변수
	cv::Mat background;  // 배경 이미지
	cv::Mat frame;  // 현재 프레임 이미지
	cv::VideoCapture cap;  // 비디오 캡처 객체

	// 각 버튼들의 좌표와 크기
	BUTTON p_open;  // 이미지 열기 버튼
	BUTTON v_open;  // 비디오 열기 버튼
	BUTTON v_capture;  // 비디오 캡처 버튼
	BUTTON expansion;  // 확대/축소 버튼
	BUTTON brush;  // 브러쉬 모드 버튼
	BUTTON eraser;  // 지우개 모드 버튼
	BUTTON compare;  // 이미지 비교 버튼

	MOUSE_POSE now_pos;  // 현재 마우스 위치

	int redValue;
	bool video_flag;  // 비디오 모드 플래그
	bool expansion_mode;  // 확대/축소 모드 플래그
	double scale_factor;  // 이미지 크기 비율
	bool brush_mode;  // 브러쉬 모드 플래그
	bool eraser_mode;
	bool picking_color;
	bool video_capture_enabled = false;
	cv::Point prev_pt;  // 이전 마우스 위치
	cv::Scalar selected_color;
	cv::Mat brushWindow;
	bool brushWindowOpen;
	bool compareMode;

	// 함수
	cv::Mat get_background();  // 배경 이미지를 생성하는 함수
	void image_open();  // 이미지를 열어 화면에 표시하는 함수
	void video_open();  // 비디오를 열어 화면에 표시하는 함수
	void video_capture();  // 비디오 프레임을 캡처하는 함수
	void handleExpansion(int delta, cv::Point pt);  // 확대/축소를 처리하는 함수
	void Show(std::string win_name, cv::Mat frame, int waitkey);  // 이미지를 화면에 표시하는 함수
	static void onMouse(int event, int x, int y, int flags, void* userdata);  // 마우스 이벤트를 처리하는 정적 메서드
	void handleColoring(int x, int y);
	void handleColorPicker(int x, int y);
	void handleBrushMode();
	static void onRedTrackbarChange(int value, void* userdata);

public:
	PhotoShop();  // 생성자
};