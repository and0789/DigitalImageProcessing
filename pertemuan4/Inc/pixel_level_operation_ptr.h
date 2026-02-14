#ifndef PIXEL_LEVEL_OPERATION_PTR_H
#define PIXEL_LEVEL_OPERATION_PTR_H

#include <opencv2/opencv.hpp>

namespace PixelLevelPtr {
    void biner(const cv::Mat& A, cv::Mat& B, int T, int M, int N);
    void negative(const cv::Mat& A, cv::Mat& B, int M, int N);
    void brightening(const cv::Mat& A, int b, cv::Mat& B, int M, int N);
}

#endif // PIXEL_LEVEL_OPERATION_PTR_H