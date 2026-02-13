#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>

int main() {
    // 1. Konfigurasi File
    const std::string filename = "bola_merah.jpg";

    // 2. Load Citra
    cv::Mat src = cv::imread(filename);

    // Validasi keamanan
    if (src.empty()) {
        std::cerr << "[ERROR] Gagal memuat gambar: " << filename << std::endl;
        return -1;
    }

    // 3. Konversi ke HSV
    cv::Mat hsv;
    cv::cvtColor(src, hsv, cv::COLOR_BGR2HSV);

    // 4. Memisahkan Channel (Splitting)
    // Menggunakan std::vector untuk menampung 3 matriks terpisah (H, S, dan V)
    std::vector<cv::Mat> hsv_channels;
    cv::split(hsv, hsv_channels);

    // Penjelasan Channel:
    // hsv_channels[0] -> Hue (Warna Dasar: Merah, Hijau, Biru, dst.)
    // hsv_channels[1] -> Saturation (Kepekatan: Semakin putih berarti warna makin pekat)
    // hsv_channels[2] -> Value (Kecerahan: Semakin putih berarti makin terang)

    // 5. Tampilkan Hasil
    cv::imshow("Original", src);

    // Catatan: Tampilan channel di bawah ini akan berupa Grayscale.
    // Area yang berwarna PUTIH (terang) menunjukkan nilai tinggi pada channel tersebut.
    cv::imshow("Channel 0: Hue (Jenis Warna)", hsv_channels[0]);
    cv::imshow("Channel 1: Saturation (Kepekatan)", hsv_channels[1]);
    cv::imshow("Channel 2: Value (Kecerahan)", hsv_channels[2]);

    std::cout << "Tekan sembarang tombol untuk keluar..." << std::endl;
    cv::waitKey(0);

    // Membersihkan window
    cv::destroyAllWindows();

    return 0;
}