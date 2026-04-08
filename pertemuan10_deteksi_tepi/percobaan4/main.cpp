
#include <opencv2/opencv.hpp>
#include <iostream>
#include <cstdlib>
using namespace cv;
using namespace std;

int main() {
    Mat img = imread("/Users/mc/CLionProjects/PengolahanCitraDigital/img1.jpg", IMREAD_GRAYSCALE);
    if (img.empty()) { cout << "Error" << endl; return -1; }

    // Tambahkan derau salt & pepper (3%)
    Mat imgNoise = img.clone();
    for (int i = 0; i < img.rows; i++)
        for (int j = 0; j < img.cols; j++) {
            int r = rand() % 100;
            if (r < 2)      imgNoise.at<uchar>(i,j) = 0;   // pepper
            else if (r < 5) imgNoise.at<uchar>(i,j) = 255; // salt
        }

    // Sobel langsung pada citra berderau
    Mat sx, sy, sMag, sobelD;
    Sobel(imgNoise, sx, CV_32F, 1, 0, 3);
    Sobel(imgNoise, sy, CV_32F, 0, 1, 3);
    magnitude(sx, sy, sMag);
    normalize(sMag, sobelD, 0, 255, NORM_MINMAX, CV_8U);

    // Sobel setelah Gaussian blur (pre-filtering)
    Mat blurredNoise, sxB, syB, sMagB, sobelBlurD;
    GaussianBlur(imgNoise, blurredNoise, Size(5,5), 1.0);
    Sobel(blurredNoise, sxB, CV_32F, 1, 0, 3);
    Sobel(blurredNoise, syB, CV_32F, 0, 1, 3);
    magnitude(sxB, syB, sMagB);
    normalize(sMagB, sobelBlurD, 0, 255, NORM_MINMAX, CV_8U);

    // Canny (Gaussian internal)
    Mat cannyRes;
    Canny(imgNoise, cannyRes, 50, 150);

    // LoG
    Mat blurred, lap, logD;
    GaussianBlur(imgNoise, blurred, Size(5,5), 1.4);
    Laplacian(blurred, lap, CV_32F, 3);
    normalize(lap, logD, 0, 255, NORM_MINMAX, CV_8U);

    imshow("Citra Asli",              img);
    imshow("Citra + Derau",           imgNoise);
    imshow("Sobel (berderau)",        sobelD);
    imshow("Sobel + Gaussian",        sobelBlurD);
    imshow("Canny (berderau)",        cannyRes);
    imshow("LoG (berderau)",          logD);
    waitKey(0);
    return 0;
}
