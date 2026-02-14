#include "arithmetic_operation_at.h"

namespace MethodAt
{
    void addition(const cv::Mat& A, const cv::Mat& B, cv::Mat& C, int M, int N)
    {
        // Pastikan output memiliki ukuran dan tipe yang sama
        C.create(A.size(), A.type());

        int channels = A.channels();

        if (channels == 1) {
            // Grayscale
            for (int i = 0; i < M; i++) {
                for (int j = 0; j < N; j++) {
                    int temp = A.at<uchar>(i, j) + B.at<uchar>(i, j);

                    // Clipping ke range 0-255
                    if (temp > 255)
                        C.at<uchar>(i, j) = 255;
                    else
                        C.at<uchar>(i, j) = (uchar)temp;
                }
            }
        } else {
            // RGB
            for (int i = 0; i < M; i++) {
                for (int j = 0; j < N; j++) {
                    // Ambil piksel dari A dan B sebagai vektor 3 elemen
                    cv::Vec3b pixA = A.at<cv::Vec3b>(i, j);
                    cv::Vec3b pixB = B.at<cv::Vec3b>(i, j);
                    cv::Vec3b pixC;

                    for (int c = 0; c < 3; c++) { // Loop untuk B, G, R
                        int temp = pixA[c] + pixB[c];
                        pixC[c] = (temp > 255) ? 255 : (uchar)temp;
                    }

                    // Simpan ke C
                    C.at<cv::Vec3b>(i, j) = pixC;
                }
            }
        }
    }

    void subtraction(const cv::Mat& A, const cv::Mat& B, cv::Mat& C, int M, int N)
    {
        C.create(A.size(), A.type());
        int channels = A.channels();

        if (channels == 1) {
            // Grayscale
            for (int i = 0; i < M; i++) {
                for (int j = 0; j < N; j++) {
                    int temp = A.at<uchar>(i, j) - B.at<uchar>(i, j);

                    // Logika user: Jika negatif, set ke 255
                    if (temp < 0)
                        C.at<uchar>(i, j) = 255;
                    else
                        C.at<uchar>(i, j) = (uchar)temp;
                }
            }
        } else {
            // RGB
            for (int i = 0; i < M; i++) {
                for (int j = 0; j < N; j++) {
                    cv::Vec3b pixA = A.at<cv::Vec3b>(i, j);
                    cv::Vec3b pixB = B.at<cv::Vec3b>(i, j);
                    cv::Vec3b pixC;

                    for (int c = 0; c < 3; c++) {
                        int temp = pixA[c] - pixB[c];
                        pixC[c] = (temp < 0) ? 255 : (uchar)temp;
                    }
                    C.at<cv::Vec3b>(i, j) = pixC;
                }
            }
        }
    }

    void multiplication(const cv::Mat& A, const cv::Mat& B, cv::Mat& C, int M, int N)
    {
        C.create(A.size(), A.type());
        int channels = A.channels();

        if (channels == 1) {
            // Grayscale
            for (int i = 0; i < M; i++) {
                for (int j = 0; j < N; j++) {
                    int temp = A.at<uchar>(i, j) * B.at<uchar>(i, j);

                    if (temp < 0) C.at<uchar>(i, j) = 0;
                    else if (temp > 255) C.at<uchar>(i, j) = 255;
                    else C.at<uchar>(i, j) = (uchar)temp;
                }
            }
        } else {
            // RGB
            for (int i = 0; i < M; i++) {
                for (int j = 0; j < N; j++) {
                    cv::Vec3b pixA = A.at<cv::Vec3b>(i, j);
                    cv::Vec3b pixB = B.at<cv::Vec3b>(i, j);
                    cv::Vec3b pixC;

                    for (int c = 0; c < 3; c++) {
                        int temp = pixA[c] * pixB[c];
                        if (temp < 0) pixC[c] = 0;
                        else if (temp > 255) pixC[c] = 255;
                        else pixC[c] = (uchar)temp;
                    }
                    C.at<cv::Vec3b>(i, j) = pixC;
                }
            }
        }
    }

    void division(const cv::Mat& A, const cv::Mat& B, cv::Mat& C, int M, int N)
    {
        C.create(A.size(), A.type());
        int channels = A.channels();

        if (channels == 1) {
            // Grayscale
            for (int i = 0; i < M; i++) {
                for (int j = 0; j < N; j++) {
                    uchar valB = B.at<uchar>(i, j);
                    if (valB == 0) {
                        C.at<uchar>(i, j) = 0;
                    } else {
                        int temp = A.at<uchar>(i, j) / valB;
                        if (temp < 0) C.at<uchar>(i, j) = 0;
                        else if (temp > 255) C.at<uchar>(i, j) = 255;
                        else C.at<uchar>(i, j) = (uchar)temp;
                    }
                }
            }
        } else {
            // RGB
            for (int i = 0; i < M; i++) {
                for (int j = 0; j < N; j++) {
                    cv::Vec3b pixA = A.at<cv::Vec3b>(i, j);
                    cv::Vec3b pixB = B.at<cv::Vec3b>(i, j);
                    cv::Vec3b pixC;

                    for (int c = 0; c < 3; c++) {
                        if (pixB[c] == 0) {
                            pixC[c] = 0;
                        } else {
                            int temp = pixA[c] / pixB[c];
                            if (temp < 0) pixC[c] = 0;
                            else if (temp > 255) pixC[c] = 255;
                            else pixC[c] = (uchar)temp;
                        }
                    }
                    C.at<cv::Vec3b>(i, j) = pixC;
                }
            }
        }
    }

    void scalar_addition(const cv::Mat& A, int c, cv::Mat& B, int M, int N)
    {
        B.create(A.size(), A.type());
        int channels = A.channels();

        if (channels == 1) {
            // Grayscale
            for (int i = 0; i < M; i++) {
                for (int j = 0; j < N; j++) {
                    int temp = A.at<uchar>(i, j) + c;
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
                        int temp = pixA[k] + c;
                        if (temp < 0) pixB[k] = 0;
                        else if (temp > 255) pixB[k] = 255;
                        else pixB[k] = (uchar)temp;
                    }
                    B.at<cv::Vec3b>(i, j) = pixB;
                }
            }
        }
    }

    void scalar_subtraction(const cv::Mat& A, int c, cv::Mat& B, int M, int N)
    {
        B.create(A.size(), A.type());
        int channels = A.channels();

        if (channels == 1) {
            // Grayscale
            for (int i = 0; i < M; i++) {
                for (int j = 0; j < N; j++) {
                    int temp = A.at<uchar>(i, j) - c;
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
                        int temp = pixA[k] - c;
                        if (temp < 0) pixB[k] = 0;
                        else if (temp > 255) pixB[k] = 255;
                        else pixB[k] = (uchar)temp;
                    }
                    B.at<cv::Vec3b>(i, j) = pixB;
                }
            }
        }
    }

    void scalar_multiplication(const cv::Mat& A, float c, cv::Mat& B, int M, int N)
    {
        B.create(A.size(), A.type());
        int channels = A.channels();

        if (channels == 1) {
            // Grayscale
            for (int i = 0; i < M; i++) {
                for (int j = 0; j < N; j++) {
                    float temp = A.at<uchar>(i, j) * c;
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
                        float temp = pixA[k] * c;
                        if (temp < 0) pixB[k] = 0;
                        else if (temp > 255) pixB[k] = 255;
                        else pixB[k] = (uchar)temp;
                    }
                    B.at<cv::Vec3b>(i, j) = pixB;
                }
            }
        }
    }

    void scalar_division(const cv::Mat& A, float c, cv::Mat& B, int M, int N)
    {
        B.create(A.size(), A.type());

        // Cek pembagian dengan 0
        if (c == 0) {
            B = cv::Mat::zeros(M, N, A.type());
            return;
        }

        int channels = A.channels();

        if (channels == 1) {
            // Grayscale
            for (int i = 0; i < M; i++) {
                for (int j = 0; j < N; j++) {
                    float temp = A.at<uchar>(i, j) / c;
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
                        float temp = pixA[k] / c;
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