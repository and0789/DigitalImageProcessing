#include "pixel_level_operation_ptr.h"

namespace PixelLevelPtr {

    void biner(const cv::Mat& A, cv::Mat& B, int T, int M, int N)
    {
        int channels = A.channels();
        B = cv::Mat::zeros(M, N, CV_8UC1);

        for (int i = 0; i < M; i++)
        {
            const uchar* pA = A.ptr<uchar>(i);
            uchar* pB = B.ptr<uchar>(i);

            for (int j = 0; j < N; j++)
            {
                int pixelValue;

                if (channels == 1) {
                    pixelValue = pA[j];
                } else {
                    // RGB: ambil 3 channel sekaligus
                    // Asumsi: Struktur data OpenCV BGR berurutan
                    pixelValue = (pA[j*3] + pA[j*3+1] + pA[j*3+2]) / 3;
                }

                pB[j] = (pixelValue < T) ? 0 : 255;
            }
        }
    }

    void negative(const cv::Mat& A, cv::Mat& B, int M, int N)
    {
        int channels = A.channels();
        B.create(A.size(), A.type());

        for (int i = 0; i < M; i++)
        {
            const uchar* pA = A.ptr<uchar>(i);
            uchar* pB = B.ptr<uchar>(i);

            if (channels == 1) {
                // Grayscale
                for (int j = 0; j < N; j++) {
                    pB[j] = 255 - pA[j];
                }
            } else {
                // RGB
                for (int j = 0; j < N; j++) {
                    pB[j*3]   = 255 - pA[j*3];
                    pB[j*3+1] = 255 - pA[j*3+1];
                    pB[j*3+2] = 255 - pA[j*3+2];
                }
            }
        }
    }

    void brightening(const cv::Mat& A, int b, cv::Mat& B, int M, int N)
    {
        int channels = A.channels();
        B.create(A.size(), A.type());

        for (int i = 0; i < M; i++)
        {
            const uchar* pA = A.ptr<uchar>(i);
            uchar* pB = B.ptr<uchar>(i);

            if (channels == 1) {
                // Grayscale
                for (int j = 0; j < N; j++) {
                    int temp = pA[j] + b;
                    if (temp < 0) pB[j] = 0;
                    else if (temp > 255) pB[j] = 255;
                    else pB[j] = temp;
                }
            } else {
                // RGB
                for (int j = 0; j < N; j++) {
                    int temp;
                    // Loop manual untuk 3 channel seperti kode asli
                    // Channel 0
                    temp = pA[j*3] + b;
                    pB[j*3] = (temp < 0) ? 0 : ((temp > 255) ? 255 : temp);

                    // Channel 1
                    temp = pA[j*3+1] + b;
                    pB[j*3+1] = (temp < 0) ? 0 : ((temp > 255) ? 255 : temp);

                    // Channel 2
                    temp = pA[j*3+2] + b;
                    pB[j*3+2] = (temp < 0) ? 0 : ((temp > 255) ? 255 : temp);
                }
            }
        }
    }
}