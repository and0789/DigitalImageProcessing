//
// Created by mc on 12/02/26.
//

#ifndef PENGOLAHANCITRADIGITAL_GEOMETRIC_OPERATION_H
#define PENGOLAHANCITRADIGITAL_GEOMETRIC_OPERATION_H

#include <opencv2/opencv.hpp>

// Translasi
void translasi(const cv::Mat& A, cv::Mat& B, int tx, int ty, int M, int N);

// Rotasi
void rotasi(const cv::Mat& A, cv::Mat& B, float angle, int M, int N);

// Flipping
void flip_horizontal(const cv::Mat& A, cv::Mat& B, int M, int N);
void flip_vertical(const cv::Mat& A, cv::Mat& B, int M, int N);
void flip_origin(const cv::Mat& A, cv::Mat& B, int M, int N);
void flip_diagonal(const cv::Mat& A, cv::Mat& B, int M, int N);

// Zooming
void zoom_in(const cv::Mat& A, cv::Mat& B, float scale);
void zoom_out(const cv::Mat& A, cv::Mat& B, float scale);

#endif //PENGOLAHANCITRADIGITAL_GEOMETRIC_OPERATION_H