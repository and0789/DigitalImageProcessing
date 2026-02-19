#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>


void tampilkanHistogram(const cv::Mat& citra) {
    // 1. Hitung Histogram
    // Menggunakan cv::Mat, cv::calcHist
    int histSize = 256;
    float range[] = { 0, 256 };
    const float* histRange = { range };

    cv::Mat hist;
    // Parameter calcHist membutuhkan pointer dan referensi cv::Mat
    cv::calcHist(&citra, 1, 0, cv::Mat(), hist, 1, &histSize, &histRange);

    // 2. Buat Citra Kosong untuk Menampung Grafik
    int hist_w = 512, hist_h = 400;

    // cvRound juga ada di dalam namespace cv
    int bin_w = cvRound((double) hist_w / histSize);

    // CV_8UC3 adalah macro, tapi Scalar ada di namespace cv::
    cv::Mat histImage(hist_h, hist_w, CV_8UC3, cv::Scalar(0,0,0));

    // 3. Normalisasi agar pas di window
    // cv::normalize dan cv::NORM_MINMAX
    cv::normalize(hist, hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());

    // 4. Gambar Garis-garis Histogram
    for(int i = 1; i < histSize; i++) {
        cv::line(histImage,
             cv::Point(bin_w * (i-1), hist_h - cvRound(hist.at<float>(i-1))),
             cv::Point(bin_w * (i), hist_h - cvRound(hist.at<float>(i))),
             cv::Scalar(255, 0, 0), 2, 8, 0);
    }

    // Tampilkan
    cv::imshow("Histogram Citra", histImage);
}

int main() {
    // 1. Baca Citra
    // Gunakan cv::imread dan cv::IMREAD_GRAYSCALE
    cv::Mat image = cv::imread("/Users/mc/CLionProjects/PengolahanCitraDigital/image1.jpg", cv::IMREAD_GRAYSCALE);

    if(image.empty()) {
        std::cout << "Error: Gambar tidak ditemukan!" << std::endl;
        return -1;
    }

    // Tampilkan citra asli
    cv::imshow("Citra Asli", image);

    // 2. Panggil fungsi histogram
    tampilkanHistogram(image);

    // Tunggu input user (hanya satu waitKey di akhir main biasanya cukup)
    cv::waitKey(0);

    return 0;
}