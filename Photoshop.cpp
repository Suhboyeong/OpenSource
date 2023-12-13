#include "Photoshop.h"

using namespace std;
using namespace cv;

void PhotoShop::onMouse(int event, int x, int y, int flags, void* userdata) {
    PhotoShop* instance = static_cast<PhotoShop*>(userdata);
    if (instance != nullptr) {
        if (flags & EVENT_MOUSEWHEEL && event != EVENT_MOUSEMOVE && instance->expansion_mode == true) {
            cout << "flags: " << flags << endl;
            cout << "mouse wheel moving: " << x << " " << y << endl;
            Point pt(x, y);
            instance->handleExpansion(flags, pt);
        }
        if (flags & EVENT_FLAG_LBUTTON && event != EVENT_MOUSEMOVE) {
            if (((instance->p_open.x <= x) && (x <= instance->p_open.x + instance->p_open.w))
                && ((instance->p_open.y <= y) && (y <= instance->p_open.y + instance->p_open.h))) {
                instance->image_open();
                cout << "open image : " << x << " " << y << endl;
            }

            if (((instance->v_open.x <= x) && (x <= instance->v_open.x + instance->v_open.w))
                && ((instance->v_open.y <= y) && (y <= instance->v_open.y + instance->v_open.h))) {
                instance->video_open();
                instance->video_flag = true;
                cout << "video image : " << x << " " << y << endl;
            }

            if (((instance->v_capture.x <= x) && (x <= instance->v_capture.x + instance->v_capture.w))
                && ((instance->v_capture.y <= y) && (y <= instance->v_capture.y + instance->v_capture.h))) {
                instance->video_flag = false;
                cout << "video capture : " << x << " " << y << endl;

                // 처음으로 버튼이 눌렸을 때만 지우개 모드 활성화
                if (!instance->video_capture_enabled) {
                    instance->video_capture_enabled = true;
                    cout << "video capture enabled" << endl;
                }
                else {
                    if (!instance->eraser_mode) {
                        instance->eraser_mode = true;
                        instance->picking_color = true;
                        std::cout << "Eraser mode activated" << std::endl;
                    }
                    else {
                        instance->eraser_mode = false;
                        instance->picking_color = false;
                        std::cout << "Eraser mode not activated" << std::endl;
                    }
                }
            }

            if (((instance->expansion.x <= x) && (x <= instance->expansion.x + instance->expansion.w))
                && ((instance->expansion.y <= y) && (y <= instance->expansion.y + instance->expansion.h))) {
                instance->brush_mode = false;
                instance->expansion_mode = true;
                cout << "Expansion mode activated" << endl;
            }

            if (((instance->brush.x <= x) && (x <= instance->brush.x + instance->brush.w))
                && ((instance->brush.y <= y) && (y <= instance->brush.y + instance->brush.h))) {
                instance->expansion_mode = false;
                instance->brush_mode = true;
                cout << "Brush mode activated" << endl;
                instance->handleBrushMode();
            }

            if (instance->eraser_mode) {
                if (instance->picking_color) {
                    instance->handleColorPicker(x, y);
                }
                else {
                    instance->handleColoring(x, y);
                }
            }

            if (event == cv::EVENT_LBUTTONDOWN) {
                instance->now_pos.x = x;
                instance->now_pos.y = y;
            }

            if (((instance->eraser.x <= x) && (x <= instance->eraser.x + instance->eraser.w))
                && ((instance->eraser.y <= y) && (y <= instance->eraser.y + instance->eraser.h))) {
                if (!instance->eraser_mode) {
                    instance->eraser_mode = true;
                    instance->picking_color = true;
                    std::cout << "Eraser mode activated" << std::endl;
                }
                else {
                    instance->eraser_mode = false;
                    instance->picking_color = false;
                    std::cout << "Erasor mode not activated" << std::endl;
                }
            }


            if (((instance->compare.x <= x) && (x <= instance->compare.x + instance->compare.w))
                && ((instance->compare.y <= y) && (y <= instance->compare.y + instance->compare.h))) {
                cout << "compare : " << x << " " << y << endl;
                instance->compareMode = !instance->compareMode;

                if (instance->compareMode) {
                    // If in compare mode, show the original image
                    instance->Show("PhotoShop", instance->get_background(), 0);
                }
                else {
                    // If not in compare mode, show the current frame image
                    instance->Show("PhotoShop", instance->background, 0);
                }
            }
        }
    }
}

// PhotoShop 클래스의 생성자
PhotoShop::PhotoShop() {
    // 초기화 코드
    // loadBrushImage();
    video_flag = false;
    expansion_mode = false;
    eraser_mode = false;
    picking_color = false;
    scale_factor = 1.0;
    prev_pt = Point(0, 0);
    frame = Mat::ones(Size(480, 480), CV_8UC3) + Scalar(255, 255, 255);
    cap = VideoCapture(0);
    background = get_background();
    namedWindow("PhotoShop");
    setMouseCallback("PhotoShop", onMouse, this);
    Show("PhotoShop", background, 0);
    brushWindow = Mat::ones(Size(480, 480), CV_8UC3) + Scalar(255, 255, 255);
    brushWindowOpen = false;
    compareMode = false;
}

// 이미지를 열어 화면에 표시하는 함수
void PhotoShop::image_open() {
    // 이미지를 읽어와 화면에 표시합니다.
    frame = imread("./lenna.jpg");
    resize(frame, frame, Size(480, 480));
    frame.copyTo(background(Rect(10, 150, 480, 480)));
    Show("PhotoShop", background, 0);
}

// 비디오를 열어 화면에 표시하는 함수
void PhotoShop::video_open() {
    // 비디오를 열어 프레임을 화면에 표시합니다.
    while (cap.isOpened()) {
        cap >> frame;
        resize(frame, frame, Size(480, 480));
        frame.copyTo(background(Rect(10, 150, 480, 480)));
        Show("PhotoShop", background, 1);
        if (video_flag == false) {
            break;
        }
    }
}

// 배경 이미지를 생성하는 함수
cv::Mat PhotoShop::get_background() {
    Mat background = Mat::ones(Size(640, 640), CV_8UC3) + Scalar(100, 100, 100);

    Mat button_p_open = imread("./photo_img.png");
    resize(button_p_open, button_p_open, Size(50, 50));
    Mat button_v_open = imread("./video_img.png");
    resize(button_v_open, button_v_open, Size(50, 50));
    Mat button_v_capture = imread("./capture_img.png");
    resize(button_v_capture, button_v_capture, Size(50, 50));

    Mat button_expansion = imread("./expansion_img.png");
    resize(button_expansion, button_expansion, Size(50, 50));
    Mat button_brush = imread("./brush_img.png");
    resize(button_brush, button_brush, Size(50, 50));
    Mat button_eraser = imread("./eraser_img.png");
    resize(button_eraser, button_eraser, Size(50, 50));
    Mat button_compare = imread("./compare_img.png");
    resize(button_compare, button_compare, Size(50, 50));

    frame.copyTo(background(Rect(10, 150, 480, 480)));
    button_p_open.copyTo(background(Rect(10, 80, 50, 50)));
    {
        p_open.x = 10;
        p_open.y = 80;
        p_open.w = 50;
        p_open.h = 50;
    }
    button_v_open.copyTo(background(Rect(80, 80, 50, 50)));
    {
        v_open.x = 80;
        v_open.y = 80;
        v_open.w = 50;
        v_open.h = 50;
    }
    button_v_capture.copyTo(background(Rect(150, 80, 50, 50)));
    {
        v_capture.x = 150;
        v_capture.y = 80;
        v_capture.w = 50;
        v_capture.h = 50;
    }
    button_expansion.copyTo(background(Rect(510, 150, 50, 50)));
    {
        expansion.x = 510;
        expansion.y = 150;
        expansion.w = 50;
        expansion.h = 50;
    }
    button_brush.copyTo(background(Rect(570, 150, 50, 50)));
    {
        brush.x = 570;
        brush.y = 150;
        brush.w = 50;
        brush.h = 50;
    }
    button_eraser.copyTo(background(Rect(510, 220, 50, 50)));
    {
        eraser.x = 510;
        eraser.y = 220;
        eraser.w = 50;
        eraser.h = 50;
    }
    button_compare.copyTo(background(Rect(570, 220, 50, 50)));
    {
        compare.x = 570;
        compare.y = 220;
        compare.w = 50;
        compare.h = 50;
    }
    return background;
}

void PhotoShop::Show(std::string win_name, cv::Mat frame, int waitkey) {
    imshow(win_name, frame);
    waitKey(waitkey);
}

// 확대/축소 함수
void PhotoShop::handleExpansion(int delta, Point pt) {
    constexpr double scale_step = 0.1; // 크기 조절 스텝
    if (delta > 0) {
        // 스크롤을 올릴 때: 확대
        scale_factor += scale_step;
        if (scale_factor > 3) {
            scale_factor = 3;
        }
    }
    else if (delta < 0) {
        // 스크롤을 내릴 때: 축소
        scale_factor -= scale_step;
        if (scale_factor < 1) {
            scale_factor = 1;
        }
    }

    Mat tmp = frame.clone();
    int ScaledWidth = tmp.cols / scale_factor;
    int ScaledHeight = tmp.rows / scale_factor;
    Point pt1(0, 0), pt2(479, 479);

    if ((pt.x - ScaledWidth) < 0) {
        pt1.x = 0;
    }
    else {
        pt1.x = pt.x - ScaledWidth;
    }
    if ((pt.x + ScaledWidth) >= 480) {
        pt2.x = 479;
    }
    else {
        pt2.x = pt.x + ScaledWidth;
    }
    if ((pt.y - ScaledWidth) < 0) {
        pt1.y = 0;
    }
    else {
        pt1.y = pt.y - ScaledWidth;
    }
    if ((pt.y + ScaledWidth) >= 480) {
        pt2.y = 479;
    }
    else {
        pt2.y = pt.y + ScaledWidth;
    }

    Mat resizedImg;
    cout << "pt1: " << pt1 << " " << "pt2: " << pt2 << endl;
    vector<Point2f> src_corners(4);
    src_corners[0] = Point2f(pt1.x, pt1.y);
    src_corners[1] = Point2f(pt1.x, pt2.y);
    src_corners[2] = Point2f(pt2.x, pt2.y);
    src_corners[3] = Point2f(pt2.x, pt1.y);
    vector<Point2f> dst_corners(4);
    dst_corners[0] = Point2f(0, 0);
    dst_corners[1] = Point2f(0, 479);
    dst_corners[2] = Point2f(479, 479);
    dst_corners[3] = Point2f(479, 0);
    Mat trans = getPerspectiveTransform(src_corners, dst_corners);

    background = get_background();
    warpPerspective(tmp, resizedImg, trans, Size(480, 480));

    resizedImg.copyTo(background(Rect(10, 150, 480, 480)));
    putText(background, "Scale Factor : " + to_string(scale_factor), Point(0, 20), 0, 1, Scalar(255, 255, 255));
    Show("PhotoShop", background, 0);
    frame = resizedImg;
}

void PhotoShop::handleColorPicker(int x, int y) {
    selected_color = frame.at<Vec3b>(y, x);
    cout << "Selected Color: " << selected_color << endl;
    picking_color = false;
}

void PhotoShop::handleColoring(int x, int y) {
    if (selected_color != Scalar(0, 0, 0)) {
        circle(background, cv::Point(x, y), 10, selected_color, -1);
        Mat roi = background(Rect(x - 15, y - 15, 30, 30)); 
        for (int i = 0; i < 3; ++i) {
            GaussianBlur(roi, roi, Size(15, 15), 0); 
        }
        imshow("PhotoShop", background);
    }
    else {
        cout << "Please pick a color first." << endl;
    }
}

void PhotoShop::onRedTrackbarChange(int value, void* userdata) {
    PhotoShop* instance = static_cast<PhotoShop*>(userdata);
    if (instance) {
        cv::Mat redStatusBar(30, 300, CV_8UC3);
        redStatusBar = cv::Scalar(0, 0, value);

        cv::imshow("BrushWindow", redStatusBar);
    }
}


void PhotoShop::handleBrushMode() {
    if (brushWindowOpen) {
        cv::destroyWindow("BrushWindow");
        brushWindowOpen = false;
    }
    else {
        cv::namedWindow("BrushWindow");
        redValue = 0;
        cv::createTrackbar("Red", "BrushWindow", &redValue, 255, onRedTrackbarChange, this);
        onRedTrackbarChange(0, this);
        brushWindowOpen = true;
    }
}