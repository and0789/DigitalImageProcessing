
#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>
using namespace cv;
using namespace std;

int main() {
    // Baca citra dan konversi ke grayscale
    Mat img = imread("/Users/mc/CLionProjects/PengolahanCitraDigital/img1.jpg", IMREAD_GRAYSCALE);
    if (img.empty()) {
        cout << "Error: Citra tidak ditemukan!" << endl;
        return -1;
    }

    int rows = img.rows;
    int cols = img.cols;

    Mat Gx    = Mat::zeros(rows, cols, CV_32F);
    Mat Gy    = Mat::zeros(rows, cols, CV_32F);
    Mat Gxy   = Mat::zeros(rows, cols, CV_32F);
    Mat edges = Mat::zeros(rows, cols, CV_8U);

    // Diferensial maju (forward differential)
    for (int i = 0; i < rows - 1; i++) {
        for (int j = 0; j < cols - 1; j++) {
            float gx = (float)img.at<uchar>(i+1, j) - (float)img.at<uchar>(i, j);
            float gy = (float)img.at<uchar>(i, j+1) - (float)img.at<uchar>(i, j);
            Gx.at<float>(i, j)  = gx;
            Gy.at<float>(i, j)  = gy;
            Gxy.at<float>(i, j) = sqrt(gx*gx + gy*gy);
        }
    }

    // Normalisasi ke 0-255 untuk ditampilkan
    Mat Gx_d, Gy_d, Gxy_d;
    normalize(Gx,  Gx_d,  0, 255, NORM_MINMAX, CV_8U);
    normalize(Gy,  Gy_d,  0, 255, NORM_MINMAX, CV_8U);
    normalize(Gxy, Gxy_d, 0, 255, NORM_MINMAX, CV_8U);

    // Thresholding
    int T = 30;
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            edges.at<uchar>(i,j) = (Gxy.at<float>(i,j) > T) ? 255 : 0;

    imshow("Citra Asli",        img);
    imshow("Gradien Gx",        Gx_d);
    imshow("Gradien Gy",        Gy_d);
    imshow("Kekuatan Tepi",     Gxy_d);
    imshow("Citra Tepi (T=30)", edges);
    waitKey(0);
    return 0;
}
