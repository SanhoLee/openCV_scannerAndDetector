#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int rtnKey = 0;

int main()
{
    VideoCapture cap(0);
    Mat img;

    String XML_PATH = "xmlFile/haarcascade_russian_plate_number.xml";

    CascadeClassifier plateCascade;
    plateCascade.load(XML_PATH);

    // check xml file loading is success or not
    if (plateCascade.empty())
    {
        cout << "XML file not loaded ! " << endl;
    }

    // detect된 객체를 표시할 사각형 자료형을 갖는 벡터를 정의
    vector<Rect> plates;

    while (true)
    {
        cap.read(img);
        plateCascade.detectMultiScale(img, plates, 1.1, 3);

        // print out lines for detected boundary
        for (int i = 0; i < plates.size(); i++)
        {
            rectangle(img, plates[i].tl(), plates[i].br(), Scalar(255, 0, 255), 3);

            Mat imgCrop = img(plates[i]);
            // imshow(to_string(i), imgCrop);
            imwrite("plates/" + to_string(i) + ".png", imgCrop);
        }
        // putText(img, "FACE DETECTTED", plates[0].tl(), FONT_HERSHEY_DUPLEX, 1, Scalar(0, 0, 255), 2);
        imshow("Image", img);

        rtnKey = waitKey(1);
        if (rtnKey == 27)
            return 0;
    }
}
