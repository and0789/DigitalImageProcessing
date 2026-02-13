#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    // 1. Konfigurasi
    const std::string filename = "bola_merah.jpg";

    // 2. Load Citra
    cv::Mat src = cv::imread(filename);

    if (src.empty()) {
        std::cerr << "[ERROR] Gagal memuat gambar: " << filename << std::endl;
        return -1;
    }

    // 3. Konversi ke HSV
    cv::Mat hsv;
    cv::cvtColor(src, hsv, cv::COLOR_BGR2HSV);

    // 4. Thresholding (Deteksi Warna Merah)
    // Warna merah di HSV OpenCV unik karena berada di ujung awal (0-10) DAN ujung akhir (170-180).
    // Kita perlu dua mask terpisah.

    cv::Mat mask1, mask2;

    // Range Merah Bawah (0-10)
    // H=0-10, S=100-255 (agar warna pekat), V=100-255 (agar cukup terang)
    cv::inRange(hsv, cv::Scalar(0, 100, 100), cv::Scalar(10, 255, 255), mask1);

    // Range Merah Atas (170-180)
    cv::inRange(hsv, cv::Scalar(170, 100, 100), cv::Scalar(180, 255, 255), mask2);

    // 5. Menggabungkan Mask (Operasi OR)
    // Menggabungkan hasil deteksi dari kedua range merah
    cv::Mat mask_final;
    cv::bitwise_or(mask1, mask2, mask_final);

    // 6. Visualisasi Hasil (Masking)
    // Membuat citra hitam kosong seukuran gambar asli
    cv::Mat result = cv::Mat::zeros(src.size(), src.type());

    // Menyalin piksel dari gambar asli HANYA pada area yang putih di mask_final
    src.copyTo(result, mask_final);

    // 7. Tampilkan Window
    cv::imshow("1. Original", src);
    cv::imshow("2. Mask (Binary)", mask_final); // Putih = Objek, Hitam = Latar
    cv::imshow("3. Hasil Segmentasi", result);  // Objek berwarna dengan latar hitam

    std::cout << "Tekan sembarang tombol untuk keluar..." << std::endl;
    cv::waitKey(0);

    cv::destroyAllWindows();
    return 0;
}