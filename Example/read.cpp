#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 3) exit(-1);

    int width, height;
    width = atoi(argv[1]);
    height = atoi(argv[2]);

    Mat frame(height, width, CV_8UC3);
    int size = frame.total() * frame.elemSize();

    namedWindow("MyVideo",CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"

    while (1) {
        char bytes[size];
        cin.read(bytes, size);

        frame = Mat(height, width, CV_8UC3, bytes);

        imshow("MyVideo", frame); //show the frame in "MyVideo" window

        if (waitKey(30) == 27) { //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
            cout << "esc key is pressed by user" << endl;
            break; 
        }
    }
    exit(0);
}
