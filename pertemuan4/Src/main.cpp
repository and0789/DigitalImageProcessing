#include <iostream>
#include "pixel_level_operation_at.h"
#include "pixel_level_operation_ptr.h"
#include "arithmetic_operation_at.h"
#include "arithmetic_operation_ptr.h"
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

	int64 t0, t1;
	double timeAt, timePtr;

	// PIXEL LEVEL OPERATION

	std::cout << "\n--- Benchmark Pixel Level ---" << std::endl;

    // Variabel Image Hasil
    cv::Mat imgBinerAt, imgNegatifAt, imgBrightAt;
    cv::Mat imgBinerPtr, imgNegatifPtr, imgBrightPtr;

    // --- Benchmark Biner ---
    t0 = cv::getTickCount();
    PixelLevelAt::biner(image, imgBinerAt, threshold, M, N);
    t1 = cv::getTickCount();
    timeAt = (t1 - t0) / cv::getTickFrequency(); // Jangan pakai 'double' lagi
    std::cout << "Time biner At  : " << timeAt << " s" << std::endl;

    t0 = cv::getTickCount();
    PixelLevelPtr::biner(image, imgBinerPtr, threshold, M, N);
    t1 = cv::getTickCount();
    timePtr = (t1 - t0) / cv::getTickFrequency(); // Gunakan timePtr
    std::cout << "Time biner Ptr : " << timePtr << " s" << std::endl;

    // --- Benchmark Negative ---
    t0 = cv::getTickCount();
    PixelLevelAt::negative(image, imgNegatifAt, M, N);
    t1 = cv::getTickCount();
    timeAt = (t1 - t0) / cv::getTickFrequency();
    std::cout << "Time Negative At : " << timeAt << " s" << std::endl;

    t0 = cv::getTickCount();
    PixelLevelPtr::negative(image, imgNegatifPtr, M, N);
    t1 = cv::getTickCount();
    timePtr = (t1 - t0) / cv::getTickFrequency();
    std::cout << "Time Negative Ptr: " << timePtr << " s" << std::endl;

	// --- Benchmark Brightening ---
	t0 = cv::getTickCount();
	PixelLevelAt::brightening(image, 100, imgBrightPtr, M, N);
	t1 = cv::getTickCount();
	timeAt = (t1 - t0) / cv::getTickFrequency();
	std::cout << "Time brightening At : " << timeAt << " s" << std::endl;

	t0 = cv::getTickCount();
	PixelLevelPtr::brightening(image, 100, imgBrightPtr, M, N);
	t1 = cv::getTickCount();
	timePtr = (t1 - t0) / cv::getTickFrequency();
	std::cout << "Time brightening Ptr: " << timePtr << " s" << std::endl;

    // --- Test Grayscale (Ptr) ---
    cv::Mat imgGray;
    cv::cvtColor(image, imgGray, cv::COLOR_BGR2GRAY);
    cv::Mat imgBinerGray, imgNegatifGray, imgBrightGray;
    PixelLevelPtr::biner(imgGray, imgBinerGray, threshold, M, N);
    PixelLevelPtr::negative(imgGray, imgNegatifGray, M, N);
    PixelLevelPtr::brightening(imgGray, 100, imgBrightGray, M, N);

    // --- Test HSV (Ptr) ---
    cv::Mat imgHSV;
    cv::cvtColor(image, imgHSV, cv::COLOR_BGR2HSV);
    cv::Mat imgBinerHSV, imgNegatifHSV, imgBrightHSV;
    PixelLevelPtr::biner(imgHSV, imgBinerHSV, threshold, M, N);
    PixelLevelPtr::negative(imgHSV, imgNegatifHSV, M, N);
    PixelLevelPtr::brightening(imgHSV, 100, imgBrightHSV, M, N);

    // Display Pixel Level Operation (Perbaikan Typo: Prt -> Ptr)
    cv::imshow("Original", image);
    cv::imshow("Biner RGB", imgBinerPtr);
    cv::imshow("Negatif RGB", imgNegatifPtr);
    cv::imshow("Brightening RGB", imgBrightPtr);
    cv::imshow("Biner Grayscale", imgBinerGray);
    cv::imshow("Negatif Grayscale", imgNegatifGray);
    cv::imshow("Brightening Grayscale", imgBrightGray);
    cv::imshow("Biner HSV", imgBinerHSV);
    cv::imshow("Negatif HSV", imgNegatifHSV);
    cv::imshow("Brightening HSV", imgBrightHSV);

	// Operasi Aritmetika
	std::cout << "\n--- Menjalankan Operasi Aritmetika & Benchmark ---" << std::endl;

    cv::Mat img1 = cv::imread("/Users/mc/CLionProjects/PengolahanCitraDigital/image1.jpg");
    cv::Mat img2 = cv::imread("/Users/mc/CLionProjects/PengolahanCitraDigital/image2.jpg");

    if (img1.empty() || img2.empty()) {
        std::cerr << "[FATAL ERROR] Gagal memuat image1.jpg atau image2.jpg!" << std::endl;
        return -1;
    }

    // Samakan ukuran
    if (img1.size() != img2.size()) {
        std::cout << "[INFO] Resize img2 agar sama dengan img1..." << std::endl;
        cv::resize(img2, img2, img1.size()); // Resize img2, simpan ke img2
    }

    // Variabel untuk menampung hasil
    cv::Mat resAddAt, resSubAt, resMulAt, resDivAt;
    cv::Mat resAddPtr, resSubPtr, resMulPtr, resDivPtr;

    int rows = img1.rows;
    int cols = img1.cols;

    // --- BENCHMARK ADDITION ---
    std::cout << "\n[BENCHMARK] Addition:" << std::endl;

    t0 = cv::getTickCount();
    MethodAt::addition(img1, img2, resAddAt, rows, cols);
    t1 = cv::getTickCount();
    timeAt = (t1 - t0) / cv::getTickFrequency();
    std::cout << "  Method .at()  : " << timeAt << " s" << std::endl;

    t0 = cv::getTickCount();
    MethodPtr::addition(img1, img2, resAddPtr, rows, cols);
    t1 = cv::getTickCount();
    timePtr = (t1 - t0) / cv::getTickFrequency();
    std::cout << "  Method .ptr() : " << timePtr << " s" << std::endl;

    if(timePtr > 0) std::cout << "  Speedup       : " << timeAt / timePtr << "x" << std::endl;

    // --- BENCHMARK SUBTRACTION ---
    std::cout << "[BENCHMARK] Subtraction:" << std::endl;

    t0 = cv::getTickCount();
    MethodAt::subtraction(img1, img2, resSubAt, rows, cols);
    t1 = cv::getTickCount();
    timeAt = (t1 - t0) / cv::getTickFrequency();
    std::cout << "  Method .at()  : " << timeAt << " s" << std::endl;

    t0 = cv::getTickCount();
    MethodPtr::subtraction(img1, img2, resSubPtr, rows, cols);
    t1 = cv::getTickCount();
    timePtr = (t1 - t0) / cv::getTickFrequency();
    std::cout << "  Method .ptr() : " << timePtr << " s" << std::endl;

    // --- BENCHMARK MULTIPLICATION ---
    std::cout << "[BENCHMARK] Multiplication:" << std::endl;

    t0 = cv::getTickCount();
    MethodAt::multiplication(img1, img2, resMulAt, rows, cols);
    t1 = cv::getTickCount();
    timeAt = (t1 - t0) / cv::getTickFrequency();
    std::cout << "  Method .at()  : " << timeAt << " s" << std::endl;

    t0 = cv::getTickCount();
    MethodPtr::multiplication(img1, img2, resMulPtr, rows, cols);
    t1 = cv::getTickCount();
    timePtr = (t1 - t0) / cv::getTickFrequency();
    std::cout << "  Method .ptr() : " << timePtr << " s" << std::endl;

    // --- BENCHMARK DIVISION ---
	t0 = cv::getTickCount();
	MethodPtr::division(img1, img2, resMulPtr, rows, cols);
	t1 = cv::getTickCount();
	timePtr = (t1 - t0) / cv::getTickFrequency();
    MethodPtr::division(img1, img2, resDivPtr, rows, cols);

    // Display Hasil Aritmetika (Gunakan salah satu hasil, misal Ptr)
    cv::imshow("Addition Result", resAddPtr);
    cv::imshow("Subtraction Result", resSubPtr);
    cv::imshow("Multiplication Result", resMulPtr);
    cv::imshow("Division Result", resDivPtr);

	// Operasi Aritmetika Scalar
	cv::Mat resScalAdd, resScalSub, resScalMul, resScalDiv;

	MethodPtr::scalar_addition(img1, 50, resScalAdd, rows, cols);
	MethodPtr::scalar_subtraction(img1, 50, resScalSub, rows, cols);
	MethodPtr::scalar_multiplication(img1, 2, resScalMul, rows, cols);
	MethodPtr::scalar_division(img1, 2, resScalDiv, rows, cols);

	cv::imshow("Scalar Addition Result", resScalAdd);
	cv::imshow("Scalar Subtraction Result", resScalSub);
	cv::imshow("Scalar Multiplication Result", resScalMul);
	cv::imshow("Scalar Division Result", resScalDiv);

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