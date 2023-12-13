#pragma once

#include <iostream>
#include "opencv2/opencv.hpp"

// ��ư�� ��ǥ�� ũ�⸦ ��Ÿ���� ����ü
struct BUTTON {
	int x;
	int y;
	int w;
	int h;
};

// ���콺 �������� ��Ÿ���� ����ü
struct MOUSE_POSE {
	int x;
	int y;
};

class PhotoShop {
private:
	// ����
	cv::Mat background;  // ��� �̹���
	cv::Mat frame;  // ���� ������ �̹���
	cv::VideoCapture cap;  // ���� ĸó ��ü

	// �� ��ư���� ��ǥ�� ũ��
	BUTTON p_open;  // �̹��� ���� ��ư
	BUTTON v_open;  // ���� ���� ��ư
	BUTTON v_capture;  // ���� ĸó ��ư
	BUTTON expansion;  // Ȯ��/��� ��ư
	BUTTON brush;  // �귯�� ��� ��ư
	BUTTON eraser;  // ���찳 ��� ��ư
	BUTTON compare;  // �̹��� �� ��ư

	MOUSE_POSE now_pos;  // ���� ���콺 ��ġ

	int redValue;
	bool video_flag;  // ���� ��� �÷���
	bool expansion_mode;  // Ȯ��/��� ��� �÷���
	double scale_factor;  // �̹��� ũ�� ����
	bool brush_mode;  // �귯�� ��� �÷���
	bool eraser_mode;
	bool picking_color;
	bool video_capture_enabled = false;
	cv::Point prev_pt;  // ���� ���콺 ��ġ
	cv::Scalar selected_color;
	cv::Mat brushWindow;
	bool brushWindowOpen;
	bool compareMode;

	// �Լ�
	cv::Mat get_background();  // ��� �̹����� �����ϴ� �Լ�
	void image_open();  // �̹����� ���� ȭ�鿡 ǥ���ϴ� �Լ�
	void video_open();  // ������ ���� ȭ�鿡 ǥ���ϴ� �Լ�
	void video_capture();  // ���� �������� ĸó�ϴ� �Լ�
	void handleExpansion(int delta, cv::Point pt);  // Ȯ��/��Ҹ� ó���ϴ� �Լ�
	void Show(std::string win_name, cv::Mat frame, int waitkey);  // �̹����� ȭ�鿡 ǥ���ϴ� �Լ�
	static void onMouse(int event, int x, int y, int flags, void* userdata);  // ���콺 �̺�Ʈ�� ó���ϴ� ���� �޼���
	void handleColoring(int x, int y);
	void handleColorPicker(int x, int y);
	void handleBrushMode();
	static void onRedTrackbarChange(int value, void* userdata);

public:
	PhotoShop();  // ������
};