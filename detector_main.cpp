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

    String XML_PATH = "xmlFile/haarcascade_frontalface_default.xml";

    CascadeClassifier faceCascade;
    faceCascade.load(XML_PATH);

    // check xml file loading is success or not
    if (faceCascade.empty())
    {
        cout << "XML file not loaded ! " << endl;
    }

    // detect된 객체를 표시할 사각형 자료형을 갖는 벡터를 정의
    vector<Rect> faces;

    while (true)
    {
        cap.read(img);
        faceCascade.detectMultiScale(img, faces, 1.1, 3);

        // print out lines for detected boundary
        for (int i = 0; i < faces.size(); i++)
        {
            rectangle(img, faces[i].tl(), faces[i].br(), Scalar(255, 0, 255), 3);
        }
        putText(img, "FACE DETECTTED", faces[0].tl(), FONT_HERSHEY_DUPLEX, 1, Scalar(0, 0, 255), 2);
        imshow("Image", img);

        rtnKey = waitKey(1);
        if (rtnKey == 27)
            return 0;
    }
}
