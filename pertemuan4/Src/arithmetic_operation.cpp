#include "arithmetic_operation.h"

void addition(const cv::Mat& A, const cv::Mat& B, cv::Mat& C, int M, int N)
{
    // Pastikan A dan B punya ukuran dan tipe yang sama
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
                int temp = pA[j] + pB[j];

                // Clipping ke range 0-255
                if (temp > 255)
                    pC[j] = 255;
                else
                    pC[j] = temp;
            }
        } else {
            // RGB - proses setiap channel
            for (int j = 0; j < N; j++) {
                int temp;

                // Channel 0 (B)
                temp = pA[j*3] + pB[j*3];
                pC[j*3] = (temp > 255) ? 255 : temp;

                // Channel 1 (G)
                temp = pA[j*3+1] + pB[j*3+1];
                pC[j*3+1] = (temp > 255) ? 255 : temp;

                // Channel 2 (R)
                temp = pA[j*3+2] + pB[j*3+2];
                pC[j*3+2] = (temp > 255) ? 255 : temp;
            }
        }
    }
}

void subtraction(const cv::Mat& A, const cv::Mat& B, cv::Mat& C, int M, int N)
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
                int temp = pA[j] - pB[j];

                // Jika negatif, set ke 255 (nyatakan objek berwarna putih)
                if (temp < 0)
                    pC[j] = 255;
                else
                    pC[j] = temp;
            }
        } else {
            // RGB - proses setiap channel
            for (int j = 0; j < N; j++) {
                int temp;

                // Channel 0 (B)
                temp = pA[j*3] - pB[j*3];
                pC[j*3] = (temp < 0) ? 255 : temp;

                // Channel 1 (G)
                temp = pA[j*3+1] - pB[j*3+1];
                pC[j*3+1] = (temp < 0) ? 255 : temp;

                // Channel 2 (R)
                temp = pA[j*3+2] - pB[j*3+2];
                pC[j*3+2] = (temp < 0) ? 255 : temp;
            }
        }
    }
}

void multiplication(const cv::Mat& A, const cv::Mat& B, cv::Mat& C, int M, int N)
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
                int temp = pA[j] * pB[j];

                // Clipping
                if (temp < 0)
                    pC[j] = 0;
                else if (temp > 255)
                    pC[j] = 255;
                else
                    pC[j] = temp;
            }
        } else {
            // RGB - proses setiap channel
            for (int j = 0; j < N; j++) {
                int temp;

                // Channel 0 (B)
                temp = pA[j*3] * pB[j*3];
                if (temp < 0)
                    pC[j*3] = 0;
                else if (temp > 255)
                    pC[j*3] = 255;
                else
                    pC[j*3] = temp;

                // Channel 1 (G)
                temp = pA[j*3+1] * pB[j*3+1];
                if (temp < 0)
                    pC[j*3+1] = 0;
                else if (temp > 255)
                    pC[j*3+1] = 255;
                else
                    pC[j*3+1] = temp;

                // Channel 2 (R)
                temp = pA[j*3+2] * pB[j*3+2];
                if (temp < 0)
                    pC[j*3+2] = 0;
                else if (temp > 255)
                    pC[j*3+2] = 255;
                else
                    pC[j*3+2] = temp;
            }
        }
    }
}

void division(const cv::Mat& A, const cv::Mat& B, cv::Mat& C, int M, int N)
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
                // Cek pembagian dengan 0
                if (pB[j] == 0) {
                    pC[j] = 0;  // atau 255, tergantung kebutuhan
                } else {
                    int temp = pA[j] / pB[j];

                    // Clipping
                    if (temp < 0)
                        pC[j] = 0;
                    else if (temp > 255)
                        pC[j] = 255;
                    else
                        pC[j] = temp;
                }
            }
        } else {
            // RGB - proses setiap channel
            for (int j = 0; j < N; j++) {
                int temp;

                // Channel 0 (B)
                if (pB[j*3] == 0)
                    pC[j*3] = 0;
                else {
                    temp = pA[j*3] / pB[j*3];
                    if (temp < 0)
                        pC[j*3] = 0;
                    else if (temp > 255)
                        pC[j*3] = 255;
                    else
                        pC[j*3] = temp;
                }

                // Channel 1 (G)
                if (pB[j*3+1] == 0)
                    pC[j*3+1] = 0;
                else {
                    temp = pA[j*3+1] / pB[j*3+1];
                    if (temp < 0)
                        pC[j*3+1] = 0;
                    else if (temp > 255)
                        pC[j*3+1] = 255;
                    else
                        pC[j*3+1] = temp;
                }

                // Channel 2 (R)
                if (pB[j*3+2] == 0)
                    pC[j*3+2] = 0;
                else {
                    temp = pA[j*3+2] / pB[j*3+2];
                    if (temp < 0)
                        pC[j*3+2] = 0;
                    else if (temp > 255)
                        pC[j*3+2] = 255;
                    else
                        pC[j*3+2] = temp;
                }
            }
        }
    }
}

void scalar_addition(const cv::Mat& A, int c, cv::Mat& B, int M, int N)
{
    B.create(A.size(), A.type());

    int channels = A.channels();

    for (int i = 0; i < M; i++)
    {
        const uchar* pA = A.ptr<uchar>(i);
        uchar* pB = B.ptr<uchar>(i);

        if (channels == 1) {
            // Grayscale
            for (int j = 0; j < N; j++) {
                int temp = pA[j] + c;

                // Clipping
                if (temp < 0)
                    pB[j] = 0;
                else if (temp > 255)
                    pB[j] = 255;
                else
                    pB[j] = temp;
            }
        } else {
            // RGB - proses setiap channel
            for (int j = 0; j < N; j++) {
                int temp;

                // Channel 0
                temp = pA[j*3] + c;
                pB[j*3] = (temp < 0) ? 0 : ((temp > 255) ? 255 : temp);

                // Channel 1
                temp = pA[j*3+1] + c;
                pB[j*3+1] = (temp < 0) ? 0 : ((temp > 255) ? 255 : temp);

                // Channel 2
                temp = pA[j*3+2] + c;
                pB[j*3+2] = (temp < 0) ? 0 : ((temp > 255) ? 255 : temp);
            }
        }
    }
}

void scalar_subtraction(const cv::Mat& A, int c, cv::Mat& B, int M, int N)
{
    B.create(A.size(), A.type());

    int channels = A.channels();

    for (int i = 0; i < M; i++)
    {
        const uchar* pA = A.ptr<uchar>(i);
        uchar* pB = B.ptr<uchar>(i);

        if (channels == 1) {
            // Grayscale
            for (int j = 0; j < N; j++) {
                int temp = pA[j] - c;

                // Clipping
                if (temp < 0)
                    pB[j] = 0;
                else if (temp > 255)
                    pB[j] = 255;
                else
                    pB[j] = temp;
            }
        } else {
            // RGB - proses setiap channel
            for (int j = 0; j < N; j++) {
                int temp;

                // Channel 0
                temp = pA[j*3] - c;
                pB[j*3] = (temp < 0) ? 0 : ((temp > 255) ? 255 : temp);

                // Channel 1
                temp = pA[j*3+1] - c;
                pB[j*3+1] = (temp < 0) ? 0 : ((temp > 255) ? 255 : temp);

                // Channel 2
                temp = pA[j*3+2] - c;
                pB[j*3+2] = (temp < 0) ? 0 : ((temp > 255) ? 255 : temp);
            }
        }
    }
}

void scalar_multiplication(const cv::Mat& A, float c, cv::Mat& B, int M, int N)
{
    B.create(A.size(), A.type());

    int channels = A.channels();

    for (int i = 0; i < M; i++)
    {
        const uchar* pA = A.ptr<uchar>(i);
        uchar* pB = B.ptr<uchar>(i);

        if (channels == 1) {
            // Grayscale
            for (int j = 0; j < N; j++) {
                float temp = pA[j] * c;

                // Clipping
                if (temp < 0)
                    pB[j] = 0;
                else if (temp > 255)
                    pB[j] = 255;
                else
                    pB[j] = (uchar)temp;
            }
        } else {
            // RGB - proses setiap channel
            for (int j = 0; j < N; j++) {
                float temp;

                // Channel 0
                temp = pA[j*3] * c;
                pB[j*3] = (temp < 0) ? 0 : ((temp > 255) ? 255 : (uchar)temp);

                // Channel 1
                temp = pA[j*3+1] * c;
                pB[j*3+1] = (temp < 0) ? 0 : ((temp > 255) ? 255 : (uchar)temp);

                // Channel 2
                temp = pA[j*3+2] * c;
                pB[j*3+2] = (temp < 0) ? 0 : ((temp > 255) ? 255 : (uchar)temp);
            }
        }
    }
}

void scalar_division(const cv::Mat& A, float c, cv::Mat& B, int M, int N)
{
    B.create(A.size(), A.type());

    int channels = A.channels();

    // Cek pembagian dengan 0
    if (c == 0) {
        B = cv::Mat::zeros(M, N, A.type());
        return;
    }

    for (int i = 0; i < M; i++)
    {
        const uchar* pA = A.ptr<uchar>(i);
        uchar* pB = B.ptr<uchar>(i);

        if (channels == 1) {
            // Grayscale
            for (int j = 0; j < N; j++) {
                float temp = pA[j] / c;

                // Clipping
                if (temp < 0)
                    pB[j] = 0;
                else if (temp > 255)
                    pB[j] = 255;
                else
                    pB[j] = (uchar)temp;
            }
        } else {
            // RGB - proses setiap channel
            for (int j = 0; j < N; j++) {
                float temp;

                // Channel 0
                temp = pA[j*3] / c;
                pB[j*3] = (temp < 0) ? 0 : ((temp > 255) ? 255 : (uchar)temp);

                // Channel 1
                temp = pA[j*3+1] / c;
                pB[j*3+1] = (temp < 0) ? 0 : ((temp > 255) ? 255 : (uchar)temp);

                // Channel 2
                temp = pA[j*3+2] / c;
                pB[j*3+2] = (temp < 0) ? 0 : ((temp > 255) ? 255 : (uchar)temp);
            }
        }
    }
}