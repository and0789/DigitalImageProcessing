#include <iostream>
#include "pixel_level_operation.h"
#include "arithmetic_operation.h"
#include "logical_operation.h"
#include "geometric_operation.h"

int main()
{
    cv::Mat image = cv::imread("/Users/mc/CLionProjects/PengolahanCitraDigital/img1.jpg");
    if (image.empty()) {
        std::cout << "Error: Image not found!" << std::endl;
        return -1;
    }

    int M = image.rows;
    int N = image.cols;
    int threshold = 127;

	// PIXEL LEVEL OPERATION

    // Test RGB
    cv::Mat imgBiner, imgNegatif, imgBright;
    biner(image, imgBiner, threshold, M, N);
    negative(image, imgNegatif, M, N);
	brightening(image, 100, imgBright, M, N);

    // Test Grayscale
    cv::Mat imgGray;
    cv::cvtColor(image, imgGray, cv::COLOR_BGR2GRAY);
    cv::Mat imgBinerGray, imgNegatifGray, imgBrightGray;
    biner(imgGray, imgBinerGray, threshold, M, N);
    negative(imgGray, imgNegatifGray, M, N);
	brightening(imgGray, 100, imgBrightGray, M, N);

    // Test HSV
    cv::Mat imgHSV;
    cv::cvtColor(image, imgHSV, cv::COLOR_BGR2HSV);
    cv::Mat imgBinerHSV, imgNegatifHSV, imgBrightHSV;
    biner(imgHSV, imgBinerHSV, threshold, M, N);
    negative(imgHSV, imgNegatifHSV, M, N);
	brightening(imgHSV, 100, imgBrightHSV, M, N);

    // Display Pixel Level Operation
    cv::imshow("Original", image);
    cv::imshow("Biner RGB", imgBiner);
    cv::imshow("Negatif RGB", imgNegatif);
	cv::imshow("Brightening RGB", imgBright);
    cv::imshow("Biner Grayscale", imgBinerGray);
    cv::imshow("Negatif Grayscale", imgNegatifGray);
	cv::imshow("Brightening Grayscale", imgBrightGray);
    cv::imshow("Biner HSV", imgBinerHSV);
    cv::imshow("Negatif HSV", imgNegatifHSV);
	cv::imshow("Brightening HSV", imgBrightHSV);

	// Operasi Aritmetika
	cv::Mat img1 = cv::imread("/Users/mc/CLionProjects/PengolahanCitraDigital/image1.jpg");
	cv::Mat img2 = cv::imread("/Users/mc/CLionProjects/PengolahanCitraDigital/image2.jpg");

	if (img1.empty() || img2.empty()) {
		std::cerr << "[FATAL ERROR] Gagal memuat image1.jpg atau image2.jpg!" << std::endl;
		std::cerr << "Cek path file kembali." << std::endl;
		return -1;
	}

	if (img1.size() != img2.size()) {
		std::cout << "[WARNING] Ukuran img1 dan img2 berbeda!" << std::endl;
		std::cout << "Img1: " << img1.cols << "x" << img1.rows << std::endl;
		std::cout << "Img2: " << img2.cols << "x" << img2.rows << std::endl;
		std::cout << "Melakukan resize img2 agar sama dengan img1..." << std::endl;

		// Solusi: Resize img2 agar sama persis dengan img1
		cv::resize(img1, img1, img2.size());
	}

	cv::Mat resultAddition, resultSubtraction, resultMultiplication, resultDivision;

	addition(img1, img2, resultAddition, img1.rows, img1.cols);
	subtraction(img1, img2, resultSubtraction, img1.rows, img1.cols);
	multiplication(img1, img2, resultMultiplication, img1.rows, img1.cols);
	division(img1, img2, resultDivision, img1.rows, img1.cols);

	cv::imshow("Addition Result", resultAddition);
	cv::imshow("Subtraction Result", resultSubtraction);
	cv::imshow("Multiplication Result", resultMultiplication);
	cv::imshow("Division Result", resultDivision);

	// Operasi Aritmetika Scalar
	cv::Mat resultScalarAddition, resultScalarSubtraction, resultScalarMultiplication, resultScalarDivision;

	scalar_addition(img1, 50, resultScalarAddition, img1.rows, img1.cols);
	scalar_subtraction(img1, 50, resultScalarSubtraction, img1.rows, img1.cols);
	scalar_multiplication(img1, 2, resultScalarMultiplication, img1.rows, img1.cols);
	scalar_division(img1, 2, resultScalarDivision, img1.rows, img1.cols);

	cv::imshow("Scalar Addition Result", resultScalarAddition);
	cv::imshow("Scalar Subtraction Result", resultScalarSubtraction);
	cv::imshow("Scalar Multiplication Result", resultScalarMultiplication);
	cv::imshow("Scalar Division Result", resultScalarDivision);

	// Operasi Logika
	cv::Mat result_and, result_or, result_not, result_xor;

	image_and(img1, img2, result_and, img1.rows, img1.cols);
	image_or(img1, img2, result_or, img1.rows, img1.cols);
	image_not(img1, result_not, img1.rows, img1.cols);
	image_xor(img1, img2, result_xor, img1.rows, img1.cols);

	cv::imshow("AND Result", result_and);
 	cv::imshow("OR Result", result_or);
	cv::imshow("NOT Result", result_not);
	cv::imshow("XOR Result", result_xor);

	// Operasi Geometri
	cv::Mat imgTraslasi, imgRotasi;
	cv::Mat imgFlipHorizontal, imgFlipVertical, imgFlipOrigin, imgFlipDiagonal;
	cv::Mat imgZoomIn, imgZoomOut;

	translasi(img1, imgTraslasi, 100, 100, img1.rows, img1.cols);
	rotasi(img1, imgRotasi, 45, img1.rows, img1.cols);
	flip_horizontal(img1, imgFlipHorizontal, img1.rows, img1.cols);
	flip_vertical(img1, imgFlipVertical, img1.rows, img1.cols);
	flip_origin(img1, imgFlipOrigin, img1.rows, img1.cols);
	flip_diagonal(img1, imgFlipDiagonal, img1.rows, img1.cols);
	zoom_in(img1, imgZoomIn, 2);
	zoom_out(img1, imgZoomOut, 2);

	cv::imshow("Translasi Result", imgTraslasi);
	cv::imshow("Rotasi Result", imgRotasi);
	cv::imshow("Flip Horizontal Result", imgFlipHorizontal);
	cv::imshow("Flip Vertical Result", imgFlipVertical);
	cv::imshow("Flip Origin Result", imgFlipOrigin);
	cv::imshow("Flip Diagonal Result", imgFlipDiagonal);
	cv::imshow("Zoom In Result", imgZoomIn);
	cv::imshow("Zoom Out Result", imgZoomOut);

    cv::waitKey(0);
    return 0;
}