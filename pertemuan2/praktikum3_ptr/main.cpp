#include <iostream>
#include <opencv2/opencv.hpp>
#include <chrono> // Untuk mengukur kecepatan

int main() {
    // 1. Konfigurasi
    const std::string filename = "lena.jpg";

    // 2. Load Citra
    cv::Mat img = cv::imread(filename);

    if (img.empty()) {
        std::cerr << "[ERROR] Gagal memuat gambar: " << filename << std::endl;
        return -1;
    }

    // Clone agar gambar asli aman
    cv::Mat result = img.clone();

    // Validasi Channel (Pointer casting harus sesuai tipe data gambar)
    if (result.channels() != 3) {
        std::cerr << "[ERROR] Contoh ini khusus untuk gambar 3-channel (BGR)." << std::endl;
        return -1;
    }

    std::cout << "Memulai proses inversi dengan metode .ptr<>() (Pointer Access)..." << std::endl;

    // --- MULAI TIMER ---
    auto start = std::chrono::high_resolution_clock::now();

    // 3. Algoritma Inversi dengan Pointer
    // Loop Baris (Rows)
    for (int y = 0; y < result.rows; y++) {

        // AMBIL POINTER BARIS
        // Kita meminta alamat memori awal dari baris ke-y.
        // Operasi ini hanya dilakukan 1x per baris (bukan per pixel), jadi sangat hemat waktu.
        cv::Vec3b* rowPtr = result.ptr<cv::Vec3b>(y);

        // Loop Kolom (Cols)
        for (int x = 0; x < result.cols; x++) {

            // AKSES ARRAY BIASA (C-Style)
            // rowPtr[x] adalah operasi aritmatika pointer sederhana.
            // TIDAK ADA pengecekan batas (boundary check) di sini, jadi sangat cepat.

            // Inversi: 255 - nilai lama
            rowPtr[x][0] = 255 - rowPtr[x][0]; // Blue
            rowPtr[x][1] = 255 - rowPtr[x][1]; // Green
            rowPtr[x][2] = 255 - rowPtr[x][2]; // Red
        }
    }

    // --- STOP TIMER ---
    auto end = std::chrono::high_resolution_clock::now();

    // 4. Hitung Durasi
    std::chrono::duration<double, std::milli> duration = end - start;

    // 5. Tampilkan Statistik
    std::cout << "========================================" << std::endl;
    std::cout << "Ukuran Citra  : " << result.cols << " x " << result.rows << " pixels" << std::endl;
    std::cout << "Metode Akses  : rowPtr[x] (Pointer)" << std::endl;
    std::cout << "Waktu Proses  : " << duration.count() << " ms" << std::endl;
    std::cout << "========================================" << std::endl;

    // 6. Visualisasi
    cv::namedWindow("1. Original", cv::WINDOW_AUTOSIZE);
    cv::imshow("1. Original", img);

    cv::namedWindow("2. Inverted Result (Pointer)", cv::WINDOW_AUTOSIZE);
    cv::imshow("2. Inverted Result (Pointer)", result);

    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}