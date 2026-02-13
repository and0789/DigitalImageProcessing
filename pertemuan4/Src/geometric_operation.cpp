#include "geometric_operation.h"
#include <cmath>

// Translasi - menggeser gambar
void translasi(const cv::Mat& A, cv::Mat& B, int tx, int ty, int M, int N)
{
    B = cv::Mat::zeros(M, N, A.type());

    int channels = A.channels();

    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            int new_i = i + ty;
            int new_j = j + tx;

            // Cek apakah masih dalam batas
            if (new_i >= 0 && new_i < M && new_j >= 0 && new_j < N)
            {
                if (channels == 1) {
                    B.at<uchar>(new_i, new_j) = A.at<uchar>(i, j);
                } else {
                    B.at<cv::Vec3b>(new_i, new_j) = A.at<cv::Vec3b>(i, j);
                }
            }
        }
    }
}

// Rotasi - memutar gambar
void rotasi(const cv::Mat& A, cv::Mat& B, float angle, int M, int N)
{
    B = cv::Mat::zeros(M, N, A.type());

    // Konversi angle ke radian
    float radian = angle * M_PI / 180.0;
    float cos_theta = cos(radian);
    float sin_theta = sin(radian);

    // Pusat rotasi
    int cx = N / 2;
    int cy = M / 2;

    int channels = A.channels();

    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            // Rotasi dengan rumus transformasi
            int x = j - cx;
            int y = i - cy;

            int new_x = (int)(x * cos_theta - y * sin_theta) + cx;
            int new_y = (int)(x * sin_theta + y * cos_theta) + cy;

            // Cek boundary
            if (new_x >= 0 && new_x < N && new_y >= 0 && new_y < M)
            {
                if (channels == 1) {
                    B.at<uchar>(i, j) = A.at<uchar>(new_y, new_x);
                } else {
                    B.at<cv::Vec3b>(i, j) = A.at<cv::Vec3b>(new_y, new_x);
                }
            }
        }
    }
}

// Flip Horizontal - cermin kiri-kanan
void flip_horizontal(const cv::Mat& A, cv::Mat& B, int M, int N)
{
    B.create(A.size(), A.type());

    int channels = A.channels();

    for (int i = 0; i < M; i++)
    {
        const uchar* pA = A.ptr<uchar>(i);
        uchar* pB = B.ptr<uchar>(i);

        if (channels == 1) {
            for (int j = 0; j < N; j++) {
                pB[j] = pA[N - 1 - j];
            }
        } else {
            for (int j = 0; j < N; j++) {
                int src_idx = (N - 1 - j) * 3;
                int dst_idx = j * 3;
                pB[dst_idx]   = pA[src_idx];
                pB[dst_idx+1] = pA[src_idx+1];
                pB[dst_idx+2] = pA[src_idx+2];
            }
        }
    }
}

// Flip Vertical - cermin atas-bawah
void flip_vertical(const cv::Mat& A, cv::Mat& B, int M, int N)
{
    B.create(A.size(), A.type());

    int channels = A.channels();

    for (int i = 0; i < M; i++)
    {
        const uchar* pA = A.ptr<uchar>(M - 1 - i);
        uchar* pB = B.ptr<uchar>(i);

        if (channels == 1) {
            for (int j = 0; j < N; j++) {
                pB[j] = pA[j];
            }
        } else {
            for (int j = 0; j < N; j++) {
                pB[j*3]   = pA[j*3];
                pB[j*3+1] = pA[j*3+1];
                pB[j*3+2] = pA[j*3+2];
            }
        }
    }
}

// Flip Origin - rotasi 180 derajat (flip horizontal + vertical)
void flip_origin(const cv::Mat& A, cv::Mat& B, int M, int N)
{
    B.create(A.size(), A.type());

    int channels = A.channels();

    for (int i = 0; i < M; i++)
    {
        const uchar* pA = A.ptr<uchar>(M - 1 - i);
        uchar* pB = B.ptr<uchar>(i);

        if (channels == 1) {
            for (int j = 0; j < N; j++) {
                pB[j] = pA[N - 1 - j];
            }
        } else {
            for (int j = 0; j < N; j++) {
                int src_idx = (N - 1 - j) * 3;
                int dst_idx = j * 3;
                pB[dst_idx]   = pA[src_idx];
                pB[dst_idx+1] = pA[src_idx+1];
                pB[dst_idx+2] = pA[src_idx+2];
            }
        }
    }
}

// Flip Diagonal - transpose pada garis x = y
void flip_diagonal(const cv::Mat& A, cv::Mat& B, int M, int N)
{
    // Hasil transpose: ukuran bertukar (M x N menjadi N x M)
    B.create(N, M, A.type());

    int channels = A.channels();

    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (channels == 1) {
                B.at<uchar>(j, i) = A.at<uchar>(i, j);
            } else {
                B.at<cv::Vec3b>(j, i) = A.at<cv::Vec3b>(i, j);
            }
        }
    }
}

// Zoom In - perbesar gambar
void zoom_in(const cv::Mat& A, cv::Mat& B, float scale)
{
    int new_M = (int)(A.rows * scale);
    int new_N = (int)(A.cols * scale);

    B.create(new_M, new_N, A.type());

    int channels = A.channels();

    for (int i = 0; i < new_M; i++)
    {
        for (int j = 0; j < new_N; j++)
        {
            // Nearest neighbor interpolation
            int src_i = (int)(i / scale);
            int src_j = (int)(j / scale);

            // Boundary check
            if (src_i >= A.rows) src_i = A.rows - 1;
            if (src_j >= A.cols) src_j = A.cols - 1;

            if (channels == 1) {
                B.at<uchar>(i, j) = A.at<uchar>(src_i, src_j);
            } else {
                B.at<cv::Vec3b>(i, j) = A.at<cv::Vec3b>(src_i, src_j);
            }
        }
    }
}

// Zoom Out - perkecil gambar
void zoom_out(const cv::Mat& A, cv::Mat& B, float scale)
{
    int new_M = (int)(A.rows / scale);
    int new_N = (int)(A.cols / scale);

    if (new_M < 1) new_M = 1;
    if (new_N < 1) new_N = 1;

    B.create(new_M, new_N, A.type());

    int channels = A.channels();

    for (int i = 0; i < new_M; i++)
    {
        for (int j = 0; j < new_N; j++)
        {
            // Nearest neighbor interpolation
            int src_i = (int)(i * scale);
            int src_j = (int)(j * scale);

            // Boundary check
            if (src_i >= A.rows) src_i = A.rows - 1;
            if (src_j >= A.cols) src_j = A.cols - 1;

            if (channels == 1) {
                B.at<uchar>(i, j) = A.at<uchar>(src_i, src_j);
            } else {
                B.at<cv::Vec3b>(i, j) = A.at<cv::Vec3b>(src_i, src_j);
            }
        }
    }
}