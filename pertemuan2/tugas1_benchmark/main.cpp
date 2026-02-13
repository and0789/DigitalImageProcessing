#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono> // Untuk pengukuran waktu presisi tinggi
#include <vector>
#include <iomanip> // Untuk merapikan tabel output

// Fungsi bantuan untuk mengukur durasi
// Mengembalikan waktu dalam milidetik (ms)
double measure_at(const cv::Mat& img, int iterations) {
    cv::Mat temp = img.clone();
    auto start = std::chrono::high_resolution_clock::now();

    for (int k = 0; k < iterations; k++) {
        for (int y = 0; y < temp.rows; y++) {
            for (int x = 0; x < temp.cols; x++) {
                // Metode 1: .at() dengan Safety Check
                cv::Vec3b& pixel = temp.at<cv::Vec3b>(y, x);
                pixel[0] = 255 - pixel[0];
                pixel[1] = 255 - pixel[1];
                pixel[2] = 255 - pixel[2];
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::milli>(end - start).count();
}

double measure_ptr(const cv::Mat& img, int iterations) {
    cv::Mat temp = img.clone();
    auto start = std::chrono::high_resolution_clock::now();

    for (int k = 0; k < iterations; k++) {
        for (int y = 0; y < temp.rows; y++) {
            // Metode 2: Row Pointer
            // Ambil alamat awal baris sekali saja per loop y
            cv::Vec3b* rowPtr = temp.ptr<cv::Vec3b>(y);

            for (int x = 0; x < temp.cols; x++) {
                // Akses array biasa (Pointer Arithmetic)
                rowPtr[x][0] = 255 - rowPtr[x][0];
                rowPtr[x][1] = 255 - rowPtr[x][1];
                rowPtr[x][2] = 255 - rowPtr[x][2];
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::milli>(end - start).count();
}

double measure_direct(const cv::Mat& img, int iterations) {
    cv::Mat temp = img.clone();

    // Validasi Continuous (Wajib untuk direct access)
    if (!temp.isContinuous()) {
        return -1.0;
    }

    auto start = std::chrono::high_resolution_clock::now();

    for (int k = 0; k < iterations; k++) {
        // Metode 3: Direct Data Pointer (Continuous)
        // Menganggap matriks sebagai array 1 dimensi panjang
        uchar* pData = temp.data;
        size_t totalElements = temp.total() * temp.channels(); // Total bytes

        for (size_t i = 0; i < totalElements; ++i) {
            pData[i] = 255 - pData[i];
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::milli>(end - start).count();
}

int main() {
    // Definisi Ukuran Uji
    std::vector<int> sizes = {256, 512, 1024, 2048}; // Ditambah 2048 untuk stress test
    int iterations = 100; // Ulangi 100x agar waktu terukur jelas

    std::cout << "=== BENCHMARK AKSES PIXEL (Rata-rata " << iterations << " iterasi) ===" << std::endl;
    std::cout << std::left
              << std::setw(15) << "Resolution"
              << std::setw(15) << "Pixels"
              << std::setw(20) << "1. at<> (Safe)"
              << std::setw(20) << "2. ptr<> (Row)"
              << std::setw(20) << "3. Direct (Raw)" << std::endl;
    std::cout << std::string(90, '-') << std::endl;

    for (int size : sizes) {
        // Buat Citra Random
        cv::Mat img(size, size, CV_8UC3);
        cv::randu(img, cv::Scalar(0, 0, 0), cv::Scalar(255, 255, 255));

        // Jalankan Test
        double t_at = measure_at(img, iterations);
        double t_ptr = measure_ptr(img, iterations);
        double t_direct = measure_direct(img, iterations);

        std::cout << std::left
                  << size << "x" << size << std::setw(8) << ""
                  << std::setw(15) << (size*size)
                  << std::fixed << std::setprecision(2) << t_at << " ms" << std::setw(12) << ""
                  << std::fixed << std::setprecision(2) << t_ptr << " ms" << std::setw(12) << ""
                  << std::fixed << std::setprecision(2) << t_direct << " ms" << std::endl;
    }

    std::cout << "\nCatatan: Waktu adalah total untuk " << iterations << " kali proses." << std::endl;

    return 0;
}