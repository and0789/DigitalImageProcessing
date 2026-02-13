//
// Created by mc on 12/02/26.
//

#ifndef PENGOLAHANCITRADIGITAL_OPERASI_ARITMETIKA_H
#define PENGOLAHANCITRADIGITAL_OPERASI_ARITMETIKA_H

#include <opencv2/opencv.hpp>

void addition(const cv::Mat& A, const cv::Mat& B, cv::Mat& C, int M, int N);
void subtraction(const cv::Mat& A, const cv::Mat& B, cv::Mat& C, int M, int N);
void multiplication(const cv::Mat& A, const cv::Mat& B, cv::Mat& C, int M, int N);
void division(const cv::Mat& A, const cv::Mat& B, cv::Mat& C, int M, int N);

void scalar_addition(const cv::Mat& A, int c, cv::Mat& B, int M, int N);
void scalar_subtraction(const cv::Mat& A, int c, cv::Mat& B, int M, int N);
void scalar_multiplication(const cv::Mat& A, float c, cv::Mat& B, int M, int N);
void scalar_division(const cv::Mat& A, float c, cv::Mat& B, int M, int N);

#endif //PENGOLAHANCITRADIGITAL_OPERASI_ARITMETIKA_H