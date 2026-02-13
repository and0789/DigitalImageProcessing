#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    // 1. Load Citra (Pastikan ada gambar tomat warna-warni)
    cv::Mat src = cv::imread("tomat_campur.jpg");

    if (src.empty()) {
        std::cerr << "[ERROR] Gagal memuat gambar!" << std::endl;
        return -1;
    }

    // 2. Konversi ke HSV
    cv::Mat hsv;
    cv::cvtColor(src, hsv, cv::COLOR_BGR2HSV);

    // 3. Definisi Range Warna (Tuning Hue sangat penting di sini)

    // A. TOMAT HIJAU (Mentah)
    // Hue: Sekitar 35 - 85
    cv::Mat mask_green;
    cv::inRange(hsv, cv::Scalar(35, 50, 50), cv::Scalar(85, 255, 255), mask_green);

    // B. TOMAT ORANYE (Setengah Matang)
    // Hue: Sekitar 10 - 25 (Transisi Merah ke Kuning)
    cv::Mat mask_orange;
    cv::inRange(hsv, cv::Scalar(10, 100, 100), cv::Scalar(25, 255, 255), mask_orange);

    // C. TOMAT MERAH (Matang) - TRICKY PART
    // Merah ada di 0-10 DAN 170-180
    cv::Mat mask_red1, mask_red2, mask_red_final;
    cv::inRange(hsv, cv::Scalar(0, 100, 100), cv::Scalar(10, 255, 255), mask_red1);
    cv::inRange(hsv, cv::Scalar(170, 100, 100), cv::Scalar(180, 255, 255), mask_red2);
    // Gabungkan kedua mask merah
    cv::bitwise_or(mask_red1, mask_red2, mask_red_final);

    // 4. Bersihkan Noise (Opsional tapi disarankan)
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));

    // Fungsi lambda atau helper loop untuk membersihkan semua mask
    cv::morphologyEx(mask_green, mask_green, cv::MORPH_OPEN, kernel);
    cv::morphologyEx(mask_orange, mask_orange, cv::MORPH_OPEN, kernel);
    cv::morphologyEx(mask_red_final, mask_red_final, cv::MORPH_OPEN, kernel);

    // 5. Hitung Jumlah Piksel (Analisis Kuantitatif)
    // countNonZero menghitung berapa banyak piksel putih (nilai > 0)
    int green_pixels = cv::countNonZero(mask_green);
    int orange_pixels = cv::countNonZero(mask_orange);
    int red_pixels = cv::countNonZero(mask_red_final);

    std::cout << "--- HASIL ANALISIS ---" << std::endl;
    std::cout << "Piksel Hijau (Mentah)  : " << green_pixels << " px" << std::endl;
    std::cout << "Piksel Oranye (Semire) : " << orange_pixels << " px" << std::endl;
    std::cout << "Piksel Merah (Matang)  : " << red_pixels << " px" << std::endl;

    // Logika Sederhana Penentuan Dominasi
    if (red_pixels > green_pixels && red_pixels > orange_pixels) {
        std::cout << "KESIMPULAN: Batch ini dominan MATANG." << std::endl;
    } else if (green_pixels > red_pixels) {
        std::cout << "KESIMPULAN: Batch ini dominan MENTAH." << std::endl;
    } else {
        std::cout << "KESIMPULAN: Batch ini CAMPURAN / SETENGAH MATANG." << std::endl;
    }

    // 6. Visualisasi (Output Windows)
    // Membuat hasil visual dengan bitwise_and agar tomatnya tetap berwarna
    cv::Mat res_green, res_red, res_orange;

    // Set background hitam, tempelkan tomat sesuai mask
    cv::bitwise_and(src, src, res_green, mask_green);
    cv::bitwise_and(src, src, res_orange, mask_orange);
    cv::bitwise_and(src, src, res_red, mask_red_final);

    cv::imshow("1. Original", src);
    cv::imshow("2. Kategori: MENTAH (Hijau)", res_green);
    cv::imshow("3. Kategori: SETENGAH MATANG (Oranye)", res_orange);
    cv::imshow("4. Kategori: MATANG (Merah)", res_red);

    cv::waitKey(0);
    cv::destroyAllWindows();
    return 0;
}