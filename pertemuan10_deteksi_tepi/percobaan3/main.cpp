#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;

int main() {
    Mat img = imread("/Users/mc/CLionProjects/PengolahanCitraDigital/img1.jpg", IMREAD_GRAYSCALE);
    if (img.empty()) { cout << "Error" << endl; return -1; }

    // Sobel via OpenCV (pembanding)
    Mat sX, sY, sMag, sobelD;
    Sobel(img, sX, CV_32F, 1, 0, 3);
    Sobel(img, sY, CV_32F, 0, 1, 3);
    magnitude(sX, sY, sMag);
    normalize(sMag, sobelD, 0, 255, NORM_MINMAX, CV_8U);

    // Operator Canny dengan tiga variasi threshold
    Mat c1, c2, c3;
    Canny(img, c1,  50, 150); // threshold standar
    Canny(img, c2,  10,  50); // threshold rendah
    Canny(img, c3, 100, 200); // threshold tinggi

    // Laplacian of Gaussian (LoG)
    Mat blurred, lap, logD;
    GaussianBlur(img, blurred, Size(5,5), 1.4);
    Laplacian(blurred, lap, CV_32F, 3);
    normalize(lap, logD, 0, 255, NORM_MINMAX, CV_8U);

    imshow("Citra Asli",        img);
    imshow("Sobel (OpenCV)",    sobelD);
    imshow("Canny T=50,150",    c1);
    imshow("Canny T=10,50",     c2);
    imshow("Canny T=100,200",   c3);
    imshow("LoG sigma=1.4",     logD);

    imwrite("hasil_canny_std.jpg",  c1);
    imwrite("hasil_canny_low.jpg",  c2);
    imwrite("hasil_canny_high.jpg", c3);
    imwrite("hasil_log.jpg",        logD);
    waitKey(0);
    return 0;
}
