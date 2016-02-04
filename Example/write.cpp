#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
    if (argc < 2) exit(-1);
    VideoCapture camera(atoi(argv[1])); //Open camera

    if (!camera.isOpened()) {
        cerr << "Cannot open the video cam" << endl;
        return -1;
    }

   double width = camera.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
   double height = camera.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video

    cerr << "Frame size: " << width << " x " << height << endl;

    Mat frame;
    camera.read(frame); // read a new frame from video
    cerr << "Mat type: " << frame.type() << endl;
    int size = frame.total() * frame.elemSize();
    cerr << "Size: " << size << endl;

    while (1) {
        bool success = camera.read(frame); // read a new frame from video

        if (!success) {
            cerr << "Cannot read a frame from video stream" << endl;
            break;
        }

        size = frame.total() * frame.elemSize();
        for (int i = 0; i < width; ++i) {
            for (int j = 0; j < height; ++j) {
                cout << frame.at<Vec3b>(j, i).val[0];
                cout << frame.at<Vec3b>(j, i).val[1];
                cout << frame.at<Vec3b>(j, i).val[2];
            }
        }

        if (waitKey(30) == 27) { //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
            cerr << "esc key is pressed by user" << endl;
            break; 
        }
    }
    exit(0);
}
