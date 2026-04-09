#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;

int main() {
    Mat img = imread("/Users/mc/CLionProjects/PengolahanCitraDigital/img1.jpg", IMREAD_GRAYSCALE);
    if (img.empty()) { cout << "Error" << endl; return -1; }

    // Langkah 1: Deteksi tepi dengan Canny
    Mat edges;
    Canny(img, edges, 50, 150);

    // Langkah 2: Transformasi Hough (representasi polar rho-theta)
    vector<Vec2f> lines;
    HoughLines(
        edges,       // citra tepi biner
        lines,       // output: vektor (rho, theta)
        1,           // resolusi rho: 1 pixel
        CV_PI/180,   // resolusi theta: 1 derajat
        150          // ambang voting minimum
    );

    // Langkah 3: Gambar garis pada citra berwarna
    Mat imgColor;
    cvtColor(img, imgColor, COLOR_GRAY2BGR);

    for (size_t i = 0; i < lines.size(); i++) {
        float rho   = lines[i][0];
        float theta = lines[i][1];
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho,     y0 = b*rho;
        Point pt1(cvRound(x0 + 1000*(-b)), cvRound(y0 + 1000*(a)));
        Point pt2(cvRound(x0 - 1000*(-b)), cvRound(y0 - 1000*(a)));
        line(imgColor, pt1, pt2, Scalar(0,0,255), 1, LINE_AA);
    }

    cout << "Garis terdeteksi: " << lines.size() << endl;

    imshow("Citra Asli",        img);
    imshow("Citra Tepi",        edges);
    imshow("Garis Terdeteksi",  imgColor);
    imwrite("hasil_hough.jpg",  imgColor);
    waitKey(0);
    return 0;
}
