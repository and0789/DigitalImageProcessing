#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    // 1. Load Citra (Wajah orang)
    const std::string filename = "wajah_close_up.jpg";
    cv::Mat src = cv::imread(filename);

    if (src.empty()) {
        std::cerr << "[ERROR] Gagal memuat gambar: " << filename << std::endl;
        return -1;
    }

    // 2. Deteksi Area Privasi (Kulit) menggunakan YCrCb
    cv::Mat ycrcb, mask_skin;
    cv::cvtColor(src, ycrcb, cv::COLOR_BGR2YCrCb);

    // Range Kulit (Empiris)
    // Y: 0-255 (Abaikan kecerahan), Cr: 133-173, Cb: 77-127
    cv::Scalar lower(0, 133, 77);
    cv::Scalar upper(255, 173, 127);

    cv::inRange(ycrcb, lower, upper, mask_skin);

    // 3. Pembersihan Mask (PENTING)
    // Agar area sensor tidak "bolong-bolong" atau berbintik
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));

    // Morph Close: Menutup lubang hitam di dalam area putih (wajah)
    cv::morphologyEx(mask_skin, mask_skin, cv::MORPH_CLOSE, kernel);
    // Morph Open: Menghilangkan noise putih di background
    cv::morphologyEx(mask_skin, mask_skin, cv::MORPH_OPEN, kernel);

    // 4. Buat Versi Blur dari Seluruh Gambar
    cv::Mat img_blurred;
    // Kernel Size (51, 51) -> Harus ganjil. Semakin besar angka, semakin buram.
    // SigmaX = 0 (dihitung otomatis dari ukuran kernel)
    cv::GaussianBlur(src, img_blurred, cv::Size(51, 51), 0);

    // 5. Gabungkan (Selective Blurring)
    // Trik: Mulai dengan gambar asli (tajam)
    cv::Mat result = src.clone();

    // "Copy" piksel dari gambar BURAM ke gambar HASIL,
    // TAPI hanya pada posisi di mana MASK bernilai putih (255).
    // Fungsi copyTo dengan parameter mask adalah kunci efisiensi di sini.
    img_blurred.copyTo(result, mask_skin);

    // 6. Tampilkan
    cv::imshow("1. Original", src);
    cv::imshow("2. Skin Mask (Area Sensor)", mask_skin);
    cv::imshow("3. Full Blur (Reference)", img_blurred);
    cv::imshow("4. Privacy Protected Result", result);

    std::cout << "Tekan sembarang tombol untuk keluar..." << std::endl;
    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}