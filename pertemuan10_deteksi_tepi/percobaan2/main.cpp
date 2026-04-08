#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>
using namespace cv;
using namespace std;

// Konvolusi 2D manual
Mat konvolusi2D(const Mat& img, const Mat& kernel) {
    int rows = img.rows, cols = img.cols;
    int kR = kernel.rows, kC = kernel.cols;
    int pR = kR/2, pC = kC/2;
    Mat hasil = Mat::zeros(rows, cols, CV_32F);
    for (int i = pR; i < rows-pR; i++)
        for (int j = pC; j < cols-pC; j++) {
            float sum = 0.0f;
            for (int m = 0; m < kR; m++)
                for (int n = 0; n < kC; n++)
                    sum += (float)img.at<uchar>(i+m-pR, j+n-pC)
                         * kernel.at<float>(m, n);
            hasil.at<float>(i, j) = sum;
        }
    return hasil;
}

Mat kekuatanTepi(const Mat& Gx, const Mat& Gy) {
    Mat hasil = Mat::zeros(Gx.rows, Gx.cols, CV_32F);
    for (int i = 0; i < Gx.rows; i++)
        for (int j = 0; j < Gx.cols; j++)
            hasil.at<float>(i,j) = sqrt(
                pow(Gx.at<float>(i,j),2) + pow(Gy.at<float>(i,j),2));
    return hasil;
}

int main() {
    Mat img = imread("/Users/mc/CLionProjects/PengolahanCitraDigital/img1.jpg", IMREAD_GRAYSCALE);
    if (img.empty()) { cout << "Error" << endl; return -1; }

    // Mask Sobel
    float dSx[] = {-1,0,1,-2,0,2,-1,0,1};
    float dSy[] = { 1,2,1, 0,0,0,-1,-2,-1};
    Mat Sx(3,3,CV_32F,dSx), Sy(3,3,CV_32F,dSy);

    // Mask Prewitt
    float dPx[] = {-1,0,1,-1,0,1,-1,0,1};
    float dPy[] = {-1,-1,-1,0,0,0,1,1,1};
    Mat Px(3,3,CV_32F,dPx), Py(3,3,CV_32F,dPy);

    // Mask Roberts (2x2)
    float dRx[] = {1,0,0,-1};
    float dRy[] = {0,1,-1,0};
    Mat Rx(2,2,CV_32F,dRx), Ry(2,2,CV_32F,dRy);

    Mat sobel   = kekuatanTepi(konvolusi2D(img,Sx), konvolusi2D(img,Sy));
    Mat prewitt = kekuatanTepi(konvolusi2D(img,Px), konvolusi2D(img,Py));
    Mat roberts = kekuatanTepi(konvolusi2D(img,Rx), konvolusi2D(img,Ry));

    Mat sD, pD, rD;
    normalize(sobel,   sD, 0, 255, NORM_MINMAX, CV_8U);
    normalize(prewitt, pD, 0, 255, NORM_MINMAX, CV_8U);
    normalize(roberts, rD, 0, 255, NORM_MINMAX, CV_8U);

    imshow("Citra Asli", img);
    imshow("Sobel",      sD);
    imshow("Prewitt",    pD);
    imshow("Roberts",    rD);

    imwrite("hasil_sobel.jpg",   sD);
    imwrite("hasil_prewitt.jpg", pD);
    imwrite("hasil_roberts.jpg", rD);
    waitKey(0);
    return 0;
}
