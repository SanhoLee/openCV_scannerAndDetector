#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

Mat imgOriginal, imgGray, imgCanny;
Mat imgCam;

float WIDTH_SCALE = 0.5;
float HEIGTH_SCALE = 0.5;

int main()
{
    int rtn = 0;

    VideoCapture cap(0);

    // String Path = "data_src/paper.jpg";
    // imgOriginal = imread(Path);
    // resize(imgOriginal, imgOriginal, Size(), WIDTH_SCALE, HEIGTH_SCALE);

    // imshow("imgOriginal", imgOriginal);

    if (cap.isOpened())
    {
        cout << "\n Camera Found...Success ! \n"
             << endl;
    }
    else
    {
        cout << "\n sth wrong with your camera stream. \n"
             << endl;
    }

    // for webCam...
    while (true)
    {
        cap.read(imgCam);
        imshow("webcam", imgCam);
        rtn = waitKey(1);
        if (rtn == 27)
        {
            return 0;
        }
    }

    // for image...
    // rtn = waitKey(0);
    // if (rtn == 27)
    // {
    //     return 0;
    // }

    return 0;
}