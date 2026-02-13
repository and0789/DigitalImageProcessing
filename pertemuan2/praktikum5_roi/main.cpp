#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    // 1. Load Citra
    // Ganti "lena.jpg" dengan file gambar yang tersedia
    std::string filename = "lena.jpg";
    cv::Mat img = cv::imread(filename);

    if (img.empty()) {
        std::cerr << "[ERROR] Gagal memuat gambar: " << filename << std::endl;
        return -1;
    }

    // 2. Clone Citra
    // Kita buat salinan agar gambar asli (img) tetap bersih untuk perbandingan
    cv::Mat result = img.clone();

    std::cout << "Dimensi Gambar: " << result.cols << "x" << result.rows << std::endl;

    // ============================================================
    // KASUS 1: MODIFIKASI ROI (BLUR EFFECT)
    // ============================================================

    // Tentukan koordinat kotak (x, y, lebar, tinggi)
    cv::Rect roiBlurBox(100, 100, 200, 200);

    // [OPTIMASI SAFETY] Pastikan kotak tidak keluar dari batas gambar!
    // Operator '&' akan mengambil irisan (intersection) antara kotak kita dengan ukuran gambar.
    roiBlurBox = roiBlurBox & cv::Rect(0, 0, result.cols, result.rows);

    // Ambil ROI dari citra 'result'
    // PENTING: 'faceRegion' hanya header baru yang menunjuk ke memori milik 'result'.
    // Ini adalah SHALLOW COPY. Tidak ada data piksel baru yang dibuat.
    cv::Mat faceRegion = result(roiBlurBox);

    // Terapkan Gaussian Blur pada ROI
    // Efeknya akan langsung muncul di 'result' juga!
    cv::GaussianBlur(faceRegion, faceRegion, cv::Size(51, 51), 0);

    // ============================================================
    // KASUS 2: MODIFIKASI ROI (INVERSI WARNA MANUAL)
    // ============================================================

    cv::Rect roiInvertBox(300, 300, 150, 150);
    // Safety Check lagi
    roiInvertBox = roiInvertBox & cv::Rect(0, 0, result.cols, result.rows);

    cv::Mat invertRegion = result(roiInvertBox); // Shallow copy lagi

    // Inversi manual menggunakan pointer (seperti Percobaan 3)
    // Ini membuktikan bahwa ROI bisa diakses per-pixel layaknya citra biasa
    for (int y = 0; y < invertRegion.rows; y++) {

        cv::Vec3b* rowPtr = invertRegion.ptr<cv::Vec3b>(y);

        for (int x = 0; x < invertRegion.cols; x++) {
            rowPtr[x][0] = 255 - rowPtr[x][0]; // Blue
            rowPtr[x][1] = 255 - rowPtr[x][1]; // Green
            rowPtr[x][2] = 255 - rowPtr[x][2]; // Red
        }
    }

    // ============================================================
    // VISUALISASI BATAS (Overlay Kotak)
    // ============================================================

    // Gambar kotak HIJAU di sekeliling area Blur
    cv::rectangle(result, roiBlurBox, cv::Scalar(0, 255, 0), 2);

    // Gambar kotak MERAH di sekeliling area Invert
    cv::rectangle(result, roiInvertBox, cv::Scalar(0, 0, 255), 2);

    // Tampilkan Hasil
    // 'img'    : Tetap utuh (karena kita clone di awal)
    // 'result' : Berubah (ada blur & inversi) karena ROI-nya dimodifikasi

    cv::namedWindow("1. Original Master (Utuh)", cv::WINDOW_AUTOSIZE);
    cv::imshow("1. Original Master (Utuh)", img);

    cv::namedWindow("2. Result (Induk ROI)", cv::WINDOW_AUTOSIZE);
    cv::imshow("2. Result (Induk ROI)", result);

    cv::namedWindow("3. Extracted Face (Hanya ROI)", cv::WINDOW_AUTOSIZE);
    cv::imshow("3. Extracted Face (Hanya ROI)", faceRegion);

    std::cout << "Perhatikan: Mengubah jendela 'faceRegion' otomatis mengubah 'result'!" << std::endl;

    cv::waitKey(0);
    cv::destroyAllWindows();
    return 0;
}