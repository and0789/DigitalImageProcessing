#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    // 1. Konfigurasi File
    // Menggunakan string constant agar mudah diganti di satu tempat
    const std::string filename = "bola_merah.jpg";

    // 2. Load Citra
    cv::Mat src = cv::imread(filename);

    // Validasi apakah gambar berhasil dimuat
    if (src.empty()) {
        std::cerr << "[ERROR] Gagal memuat gambar: " << filename << std::endl;
        std::cerr << "Pastikan file berada di direktori project atau gunakan path absolut." << std::endl;
        return -1;
    }

    // 3. Persiapan Variabel Matriks
    cv::Mat gray, hsv, ycrcb;

    // 4. Konversi Ruang Warna
    // BGR ke Grayscale (Untuk analisis intensitas cahaya/bentuk)
    cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);

    // BGR ke HSV (Hue, Saturation, Value) - Sangat efektif untuk segmentasi warna
    cv::cvtColor(src, hsv, cv::COLOR_BGR2HSV);

    // BGR ke YCrCb (Luma, Chroma Red, Chroma Blue) - Efektif untuk deteksi kulit (skin detection)
    cv::cvtColor(src, ycrcb, cv::COLOR_BGR2YCrCb);

    // 5. Tampilkan Hasil
    // Menggunakan namedWindow agar ukuran jendela otomatis menyesuaikan citra
    cv::namedWindow("Original (BGR)", cv::WINDOW_AUTOSIZE);
    cv::imshow("Original (BGR)", src);

    cv::namedWindow("Grayscale", cv::WINDOW_AUTOSIZE);
    cv::imshow("Grayscale", gray);

    cv::namedWindow("HSV Space", cv::WINDOW_AUTOSIZE);
    cv::imshow("HSV Space", hsv);

    cv::namedWindow("YCrCb Space", cv::WINDOW_AUTOSIZE);
    cv::imshow("YCrCb Space", ycrcb);

    // Tunggu input user (tekan sembarang tombol untuk keluar)
    std::cout << "Tekan sembarang tombol untuk keluar..." << std::endl;
    cv::waitKey(0);

    // Bersihkan semua jendela memori GUI (Good practice)
    cv::destroyAllWindows();

    return 0;
}