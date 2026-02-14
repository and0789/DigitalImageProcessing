#include "pixel_level_operation_at.h"

namespace PixelLevelAt {

    void biner(const cv::Mat& A, cv::Mat& B, int T, int M, int N)
    {
        // Output B selalu 1 channel (Grayscale/Hitam Putih)
        B = cv::Mat::zeros(M, N, CV_8UC1);
        int channels = A.channels();

        for (int i = 0; i < M; i++)
        {
            for (int j = 0; j < N; j++)
            {
                int pixelValue;

                if (channels == 1) {
                    // Grayscale: ambil nilai langsung
                    pixelValue = A.at<uchar>(i, j);
                } else {
                    // RGB: hitung rata-rata 3 channel
                    cv::Vec3b pixel = A.at<cv::Vec3b>(i, j);
                    pixelValue = (pixel[0] + pixel[1] + pixel[2]) / 3;
                }

                // Thresholding
                B.at<uchar>(i, j) = (pixelValue < T) ? 0 : 255;
            }
        }
    }

    void negative(const cv::Mat& A, cv::Mat& B, int M, int N)
    {
        B.create(A.size(), A.type());
        int channels = A.channels();

        if (channels == 1) {
            // Grayscale
            for (int i = 0; i < M; i++) {
                for (int j = 0; j < N; j++) {
                    B.at<uchar>(i, j) = 255 - A.at<uchar>(i, j);
                }
            }
        } else {
            // RGB
            for (int i = 0; i < M; i++) {
                for (int j = 0; j < N; j++) {
                    cv::Vec3b pixA = A.at<cv::Vec3b>(i, j);
                    cv::Vec3b pixB;

                    // Invert setiap channel
                    pixB[0] = 255 - pixA[0];
                    pixB[1] = 255 - pixA[1];
                    pixB[2] = 255 - pixA[2];

                    B.at<cv::Vec3b>(i, j) = pixB;
                }
            }
        }
    }

    void brightening(const cv::Mat& A, int b, cv::Mat& B, int M, int N)
    {
        B.create(A.size(), A.type());
        int channels = A.channels();

        if (channels == 1) {
            // Grayscale
            for (int i = 0; i < M; i++) {
                for (int j = 0; j < N; j++) {
                    int temp = A.at<uchar>(i, j) + b;

                    // Clipping
                    if (temp < 0) B.at<uchar>(i, j) = 0;
                    else if (temp > 255) B.at<uchar>(i, j) = 255;
                    else B.at<uchar>(i, j) = (uchar)temp;
                }
            }
        } else {
            // RGB
            for (int i = 0; i < M; i++) {
                for (int j = 0; j < N; j++) {
                    cv::Vec3b pixA = A.at<cv::Vec3b>(i, j);
                    cv::Vec3b pixB;

                    for (int k = 0; k < 3; k++) {
                        int temp = pixA[k] + b;
                        // Clipping
                        if (temp < 0) pixB[k] = 0;
                        else if (temp > 255) pixB[k] = 255;
                        else pixB[k] = (uchar)temp;
                    }
                    B.at<cv::Vec3b>(i, j) = pixB;
                }
            }
        }
    }
}