#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

Mat imgOriginal, imgGray, imgBlur, imgCanny, imgDil, imgErode;
Mat imgThre;
vector<Point> initialPoint;

float WIDTH_SCALE = 0.5;
float HEIGTH_SCALE = 0.5;

int checkImgLoaded(Mat imgSrc)
{
    if (imgSrc.empty())
    {
        cout << "\n ERROR :: no image loaded ! \n"
             << endl;
        return -1;
    }
    else
    {
        cout << "\n Success :: IMG loaded ! \n"
             << endl;
        return 0;
    }
}

vector<Point> getContours(Mat imgDil)
{

    // Description  : 입력된 이미지에서 외곽 테두리를 찾습니다.
    // input        : mask 처리된 이미지 소스
    // return       : 검출된 윤곽을 둘러싸는 직사각형에서 상단 센터 좌표를 반환함
    //
    // contours     : 각 도형의 윤관석을 표현하는 포인트 요소를 벡터로 표현해서 가지고 있게 된다.
    // hierarchy    : 4개 정수를 그 요소로 하는 벡터를 정의해줬다.

    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;

    // 소스 이미지로 부터 윤곽선을 찾고, contour 벡터 안에 Point정보를 저장한다.
    findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    // conPoly      : 윤곽선을 구성하는 데이터를 다각형 형태로 담기 위한 변수 선언.
    // boundRect    : 검출한 윤곽선을 감싸는 직사각형 도형을 생성할건데, 직사각형 데이터를 담아주기 위한 준비.
    vector<vector<Point>> conPoly(contours.size());
    vector<Rect> boundRect(contours.size());

    vector<Point> biggest;
    int maxArea = 0;

    for (int i = 0; i < contours.size(); i++)
    {
        int area = contourArea(contours[i]);
        if (area > 1000)
        {

            // contours요소의 아크 길이를 리턴. 두번째 요소는 아크가 닫혀있으면(boolean true) 그 값을 리턴하는 듯.
            float peri = arcLength(contours[i], true);
            // 감지한 윤곽에 대해서, 해당 윤곽이 사각형인지 삼각형인지를 파악한다. 선분 갯수를 계산해서 아웃풋 어레이로 반환한다.
            approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);

            if (area > maxArea && conPoly[i].size() == 4)
            {
                drawContours(imgOriginal, conPoly, i, Scalar(255, 0, 255), 5);

                // 사각형 요소에 대해서 조건문을 진행시키고, maxArea를 갱신하면서 최대 넓이를 갖는 윤곽선을 찾아낸다.
                biggest = {conPoly[i][0], conPoly[i][1], conPoly[i][2], conPoly[i][3]};
                maxArea = area;
            }

            // 도형의 윤곽선을 그려준다. 다각형 형태의 윤곽선을 뿌려줬기 때문에, 원일 경우 매끄럽지않은 각이 있는 원 형태를 볼수 있다.
            // drawContours(imgOriginal, conPoly, i, Scalar(255, 0, 255), 2);
            // rectangle(imgOriginal, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 5);
        }
    }

    // 넓이가 최대가 되는 사각형 윤곽선 네점 을 리턴한다.
    return biggest;
}

Mat preProcessing(Mat img)
{
    cvtColor(img, imgGray, COLOR_BGR2GRAY);
    GaussianBlur(imgGray, imgBlur, Size(3, 3), 3, 0);
    Canny(imgBlur, imgCanny, 25, 75);

    Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
    dilate(imgCanny, imgDil, kernel);
    // erode(imgDil, imgErode, kernel);

    return imgDil;
}

void drawPoints(vector<Point> points, Scalar color)
{
    for (int i = 0; i < points.size(); i++)
    {
        circle(imgOriginal, points[i], 10, color, FILLED);
        putText(imgOriginal, to_string(i), points[i], FONT_HERSHEY_PLAIN, 3, color, 3);
    }
}

int main()
{
    int rtn = 0;

    String Path = "data_src/paper.jpg";
    imgOriginal = imread(Path);
    checkImgLoaded(imgOriginal);

    resize(imgOriginal, imgOriginal, Size(), WIDTH_SCALE, HEIGTH_SCALE);

    /*    Steps to do...

    1. img Preprocessing
    2. get Contours - biggest
    3. Warp Img.

    */

    // 1. img Preprecessing
    imgThre = preProcessing(imgOriginal);

    // 2. get Contours - biggest
    // 사각형을 이루는 포인트를 반환한다.
    initialPoint = getContours(imgThre);
    drawPoints(initialPoint, Scalar(0, 0, 255));

    imshow("imgOriginal", imgOriginal);
    imshow("img Dilation", imgThre);

    rtn = waitKey(0);
    if (rtn == 27)
    {
        return 0;
    }
}