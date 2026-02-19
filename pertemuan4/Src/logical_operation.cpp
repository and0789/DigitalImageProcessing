#include "logical_operation.h"

void image_and(const cv::Mat& A, const cv::Mat& B, cv::Mat& C, int M, int N)
{
    C.create(A.size(), A.type());

    int channels = A.channels();

    for (int i = 0; i < M; i++)
    {
        const uchar* pA = A.ptr<uchar>(i);
        const uchar* pB = B.ptr<uchar>(i);
        uchar* pC = C.ptr<uchar>(i);

        if (channels == 1) {
            // Grayscale
            for (int j = 0; j < N; j++) {
                pC[j] = pA[j] & pB[j];  // Bitwise AND
            }
        } else {
            // RGB - proses setiap channel
            for (int j = 0; j < N; j++) {
                pC[j*3]   = pA[j*3]   & pB[j*3];    // B
                pC[j*3+1] = pA[j*3+1] & pB[j*3+1];  // G
                pC[j*3+2] = pA[j*3+2] & pB[j*3+2];  // R
            }
        }
    }
}

void image_or(const cv::Mat& A, const cv::Mat& B, cv::Mat& C, int M, int N)
{
    C.create(A.size(), A.type());

    int channels = A.channels();

    for (int i = 0; i < M; i++)
    {
        const uchar* pA = A.ptr<uchar>(i);
        const uchar* pB = B.ptr<uchar>(i);
        uchar* pC = C.ptr<uchar>(i);

        if (channels == 1) {
            // Grayscale
            for (int j = 0; j < N; j++) {
                pC[j] = pA[j] | pB[j];  // Bitwise OR
            }
        } else {
            // RGB - proses setiap channel
            for (int j = 0; j < N; j++) {
                pC[j*3]   = pA[j*3]   | pB[j*3];    // B
                pC[j*3+1] = pA[j*3+1] | pB[j*3+1];  // G
                pC[j*3+2] = pA[j*3+2] | pB[j*3+2];  // R
            }
        }
    }
}

void image_not(const cv::Mat& A, cv::Mat& C, int M, int N)
{
    C.create(A.size(), A.type());

    int channels = A.channels();

    for (int i = 0; i < M; i++)
    {
        const uchar* pA = A.ptr<uchar>(i);
        uchar* pC = C.ptr<uchar>(i);

        if (channels == 1) {
            // Grayscale
            for (int j = 0; j < N; j++) {
                pC[j] = ~pA[j];  // Bitwise NOT (sama dengan negatif 255-x)
            }
        } else {
            // RGB - proses setiap channel
            for (int j = 0; j < N; j++) {
                pC[j*3]   = ~pA[j*3];    // B
                pC[j*3+1] = ~pA[j*3+1];  // G
                pC[j*3+2] = ~pA[j*3+2];  // R
            }
        }
    }
}

void image_xor(const cv::Mat& A, const cv::Mat& B, cv::Mat& C, int M, int N)
{
    C.create(A.size(), A.type());

    int channels = A.channels();

    for (int i = 0; i < M; i++)
    {
        const uchar* pA = A.ptr<uchar>(i);
        const uchar* pB = B.ptr<uchar>(i);
        uchar* pC = C.ptr<uchar>(i);

        if (channels == 1) {
            // Grayscale
            for (int j = 0; j < N; j++) {
                pC[j] = pA[j] ^ pB[j];  // Bitwise XOR
            }
        } else {
            // RGB - proses setiap channel
            for (int j = 0; j < N; j++) {
                pC[j*3]   = pA[j*3]   ^ pB[j*3];    // B
                pC[j*3+1] = pA[j*3+1] ^ pB[j*3+1];  // G
                pC[j*3+2] = pA[j*3+2] ^ pB[j*3+2];  // R
            }
        }
    }
}