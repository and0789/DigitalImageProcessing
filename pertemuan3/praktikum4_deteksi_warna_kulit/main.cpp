#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    // 1. Konfigurasi File
    const std::string filename = "wajah_orang.jpg";

    // 2. Load Citra
    cv::Mat src = cv::imread(filename);

    if (src.empty()) {
        std::cerr << "[ERROR] Gagal memuat gambar: " << filename << std::endl;
        return -1;
    }

    // 3. Konversi Ruang Warna (BGR -> YCrCb)
    // Y = Luma (Kecerahan), Cr = Chroma Red, Cb = Chroma Blue
    // Ini lebih robust terhadap perubahan cahaya dibanding RGB biasa.
    cv::Mat ycrcb;
    cv::cvtColor(src, ycrcb, cv::COLOR_BGR2YCrCb);

    // 4. Definisi Range Kulit (Skin Thresholds)
    // Nilai empiris umum untuk kulit manusia (berbagai ras):
    // Cr: 133 - 173
    // Cb: 77 - 127
    // Y : 0 - 255 (Kita abaikan kecerahan agar deteksi bekerja di tempat gelap/terang)

    const cv::Scalar lower_skin(0, 133, 77);
    const cv::Scalar upper_skin(255, 173, 127);

    cv::Mat mask;
    cv::inRange(ycrcb, lower_skin, upper_skin, mask);

    // 5. Operasi Morfologi (Noise Removal)
    // Menggunakan MORPH_OPEN (Erosi diikuti Dilasi)
    // Tujuannya: Menghilangkan bintik putih kecil (noise) di background,
    // tapi mempertahankan area kulit yang besar.
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3));
    cv::morphologyEx(mask, mask, cv::MORPH_OPEN, kernel);

    // Opsional: MORPH_CLOSE (Dilasi diikuti Erosi)
    // Gunakan ini jika ada lubang-lubang hitam kecil DI DALAM area wajah
    // cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, kernel);

    // 6. Visualisasi Hasil Akhir
    cv::Mat result = cv::Mat::zeros(src.size(), src.type());
    src.copyTo(result, mask);

    // 7. Tampilkan Window
    cv::imshow("1. Original", src);
    cv::imshow("2. Skin Mask (Binary)", mask);
    cv::imshow("3. Skin Detected", result);

    std::cout << "Tekan sembarang tombol untuk keluar..." << std::endl;
    cv::waitKey(0);

    cv::destroyAllWindows();
    return 0;
}