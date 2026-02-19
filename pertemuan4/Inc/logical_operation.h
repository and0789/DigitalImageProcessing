#ifndef PENGOLAHANCITRADIGITAL_BOOLEAN_OPERATION_H
#define PENGOLAHANCITRADIGITAL_BOOLEAN_OPERATION_H

#include <opencv2/opencv.hpp>

void image_and(const cv::Mat& A, const cv::Mat& B, cv::Mat& C, int M, int N);
void image_or(const cv::Mat& A, const cv::Mat& B, cv::Mat& C, int M, int N);
void image_not(const cv::Mat& A, cv::Mat& C, int M, int N);
void image_xor(const cv::Mat& A, const cv::Mat& B, cv::Mat& C, int M, int N);

#endif //PENGOLAHANCITRADIGITAL_BOOLEAN_OPERATION_H