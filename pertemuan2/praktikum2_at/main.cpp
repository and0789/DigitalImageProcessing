#include <iostream>
#include <opencv2/opencv.hpp>
#include <chrono> // Library wajib untuk benchmarking waktu

int main() {
    // 1. Konfigurasi File
    const std::string filename = "lena.jpg";

    // 2. Load Citra
    cv::Mat img = cv::imread(filename);

    // Validasi Load
    if (img.empty()) {
        std::cerr << "[ERROR] Gagal memuat gambar: " << filename << std::endl;
        return -1;
    }

    // 3. Clone Citra (Agar gambar asli tidak rusak)
    cv::Mat result = img.clone();

    // Pastikan citra memiliki 3 channel (BGR) agar aman menggunakan Vec3b
    if (result.channels() != 3) {
        std::cerr << "[ERROR] Kode ini dikhususkan untuk citra berwarna (3 channel)." << std::endl;
        return -1;
    }

    std::cout << "Memulai proses inversi dengan metode .at<>() (Safe Access)..." << std::endl;

    // --- MULAI TIMER ---
    auto start = std::chrono::high_resolution_clock::now();

    // 4. Algoritma Inversi Manual (Double Loop)
    // Akses baris (rows) lalu kolom (cols)
    for (int y = 0; y < result.rows; y++) {
        for (int x = 0; x < result.cols; x++) {

            // OPTIMASI: Gunakan Reference (&)
            // Kode asli: cv::Vec3b pixel = ... (Copy data -> Lambat)
            // Kode baru: cv::Vec3b& pixel = ... (Akses alamat memori langsung -> Lebih Cepat)
            cv::Vec3b& pixel = result.at<cv::Vec3b>(y, x);

            // Operasi Inversi (Negatif Film)
            // Rumus: 255 - nilai_asli
            pixel[0] = 255 - pixel[0]; // Channel Blue
            pixel[1] = 255 - pixel[1]; // Channel Green
            pixel[2] = 255 - pixel[2]; // Channel Red

            // Karena kita pakai reference (&), kita TIDAK PERLU menulis balik:
            // result.at<cv::Vec3b>(y, x) = pixel; <--- Baris ini tidak perlu lagi
        }
    }

    // --- STOP TIMER ---
    auto end = std::chrono::high_resolution_clock::now();

    // 5. Hitung Durasi
    // Mengubah selisih waktu menjadi milidetik (ms)
    std::chrono::duration<double, std::milli> duration = end - start;

    // 6. Tampilkan Statistik
    std::cout << "========================================" << std::endl;
    std::cout << "Ukuran Citra  : " << result.cols << " x " << result.rows << " pixels" << std::endl;
    std::cout << "Metode Akses  : img.at<cv::Vec3b>(y, x)" << std::endl;
    std::cout << "Waktu Proses  : " << duration.count() << " ms" << std::endl;
    std::cout << "========================================" << std::endl;

    // 7. Visualisasi Hasil
    cv::namedWindow("1. Original", cv::WINDOW_AUTOSIZE);
    cv::imshow("1. Original", img);

    cv::namedWindow("2. Inverted Result (.at)", cv::WINDOW_AUTOSIZE);
    cv::imshow("2. Inverted Result (.at)", result);

    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}