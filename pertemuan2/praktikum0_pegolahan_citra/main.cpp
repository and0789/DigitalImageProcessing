#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip> // Untuk merapikan output angka

// ==========================================
// FUNGSI 1: Manual Downsampling (Spatial Resolution)
// ==========================================
void manualDownsampling(const cv::Mat& src, cv::Mat& dst, int scaleFactor) {
    if (scaleFactor < 1) scaleFactor = 1;

    int newWidth = src.cols / scaleFactor;
    int newHeight = src.rows / scaleFactor;

    // Inisialisasi matriks tujuan dengan tipe data yang sama dengan sumber
    dst = cv::Mat::zeros(cv::Size(newWidth, newHeight), src.type());

    for (int y = 0; y < dst.rows; y++) {
        for (int x = 0; x < dst.cols; x++) {
            // Mapping koordinat (Decimation / Nearest Neighbor)
            int srcY = y * scaleFactor;
            int srcX = x * scaleFactor;

            // Boundary check (mencegah akses di luar memori)
            srcY = std::min(srcY, src.rows - 1);
            srcX = std::min(srcX, src.cols - 1);

            // Copy pixel (Handle Grayscale vs Color secara otomatis)
            if (src.channels() == 3) {
                dst.at<cv::Vec3b>(y, x) = src.at<cv::Vec3b>(srcY, srcX);
            } else {
                dst.at<uchar>(y, x) = src.at<uchar>(srcY, srcX);
            }
        }
    }
}

// ==========================================
// FUNGSI 2: Manual Quantization (Intensity Resolution)
// ==========================================
void manualQuantization(const cv::Mat& src, cv::Mat& dst, int k_bit) {
    // Validasi input bit (1-8)
    if (k_bit < 1) k_bit = 1;
    if (k_bit > 8) k_bit = 8;

    // Masking bit rendah
    // Contoh 2 bit: Keep 2 bit teratas (11000000 = 0xC0)
    // Logika: Geser 0xFF (11111111) ke kiri sebanyak (8 - k_bit)
    uchar maskValue = 0xFF << (8 - k_bit);

    // OPTIMASI:
    // Daripada looping manual pixel-by-pixel, kita gunakan operasi matriks OpenCV.
    // Ini memanfaatkan instruksi CPU (SIMD) yang jauh lebih cepat.

    // Buat Scalar mask (berlaku untuk 1 channel atau 3 channel)
    cv::Scalar maskScalar(maskValue, maskValue, maskValue);

    // Operasi Bitwise AND langsung pada Matriks
    cv::bitwise_and(src, maskScalar, dst);
}

// ==========================================
// FUNGSI 3: Analisis Data Pixel (Forensik)
// ==========================================
void analyzeValues(const cv::Mat& img, const std::string& label) {
    std::vector<int> values;
    values.reserve(img.total()); // Optimasi alokasi memori

    // Scan pixel (Sampling)
    if (img.channels() == 1) {
        // Grayscale: Ambil semua pixel
        // Menggunakan pointer agar iterasi sangat cepat
        const uchar* ptr = img.ptr<uchar>();
        for (size_t i = 0; i < img.total(); ++i) {
            values.push_back((int)ptr[i]);
        }
    } else {
        // RGB: Ambil Channel Blue saja sebagai sampel
        for (int y = 0; y < img.rows; y++) {
            for (int x = 0; x < img.cols; x++) {
                values.push_back((int)img.at<cv::Vec3b>(y, x)[0]);
            }
        }
    }

    // Sort dan Hapus Duplikat
    std::sort(values.begin(), values.end());
    auto last = std::unique(values.begin(), values.end());
    values.erase(last, values.end());

    // Tampilkan Output
    std::cout << label << ":" << std::endl;
    std::cout << "   -> Total Variasi Warna Unik: " << values.size() << " Level." << std::endl;
    std::cout << "   -> Sampel Nilai: { ";

    // Logika print: Jika <= 20 tampilkan semua, jika banyak potong tengahnya
    if (values.size() <= 20) {
        for (size_t i = 0; i < values.size(); ++i) {
            std::cout << values[i] << (i < values.size() - 1 ? ", " : "");
        }
    } else {
        // Tampilkan 10 awal
        for (size_t i = 0; i < 10; ++i) std::cout << values[i] << ", ";
        std::cout << "... (" << values.size() - 15 << " data lainnya) ..., ";
        // Tampilkan 5 akhir
        for (size_t i = values.size() - 5; i < values.size(); ++i) {
            std::cout << values[i] << (i < values.size() - 1 ? ", " : "");
        }
    }
    std::cout << " }" << std::endl;
    std::cout << "----------------------------------------------------" << std::endl;
}

// ==========================================
// MAIN PROGRAM
// ==========================================
int main() {
    // 1. Load Citra
    // Ganti nama file sesuai kebutuhan. Pastikan file ada di folder project.
    std::string filepath = "lena.jpg";
    cv::Mat imgOriginal = cv::imread(filepath);

    if (imgOriginal.empty()) {
        std::cerr << "[ERROR] Gambar tidak ditemukan: " << filepath << std::endl;
        std::cerr << "Pastikan file gambar berada satu folder dengan file .cpp atau .exe" << std::endl;
        return -1;
    }

    // --- PARAMETER SIMULASI ---
    int scale = 16;     // Faktor downsampling (Mengecilkan resolusi spasial)
    int bitDepth = 2;   // Target bit depth (Resolusi intensitas, 2 bit = 4 warna)

    // --- PROSES ---

    // A. Downsampling
    cv::Mat imgSmall;
    manualDownsampling(imgOriginal, imgSmall, scale);

    // B. Quantization (Bit Depth Reduction)
    cv::Mat imgGray, imgLowBitGray, imgLowBitRGB;

    // Buat versi Gray asli
    cv::cvtColor(imgOriginal, imgGray, cv::COLOR_BGR2GRAY);

    // Proses RGB
    manualQuantization(imgOriginal, imgLowBitRGB, bitDepth);
    // Proses Gray
    manualQuantization(imgGray, imgLowBitGray, bitDepth);

    // --- BAGIAN BUKTI TERTULIS (CONSOLE OUTPUT) ---
    std::cout << "\n=== 1. ANALISIS RESOLUSI SPASIAL (SAMPLING) ===" << std::endl;
    std::cout << "Original Size : " << imgOriginal.cols << " x " << imgOriginal.rows
              << " (" << imgOriginal.total() << " pixels)" << std::endl;
    std::cout << "Downsampled   : " << imgSmall.cols << " x " << imgSmall.rows
              << " (" << imgSmall.total() << " pixels)" << std::endl;
    std::cout << "Kesimpulan    : Resolusi berkurang " << (scale * scale)
              << "x lipat (Pixel dibuang)." << std::endl;

    std::cout << "\n=== 2. ANALISIS RESOLUSI INTENSITAS (BIT DEPTH) ===" << std::endl;

    // Perhitungan Dinamis
    int numLevels = std::pow(2, bitDepth);
    int interval = 256 / numLevels;

    std::cout << "Target Bit Depth: " << bitDepth << " bits" << std::endl;
    std::cout << "Teori Jumlah Warna (2^" << bitDepth << "): " << numLevels << " level warna." << std::endl;
    std::cout << "Interval (Jarak antar warna): " << interval << std::endl;

    std::cout << "\n=== 3. BUKTI DATA PIXEL (FORENSIK DATA) ===" << std::endl;
    std::cout << "Membandingkan data pixel SEBELUM dan SESUDAH diproses:\n" << std::endl;

    // 1. Data Asli
    analyzeValues(imgGray, "[BEFORE] Citra Asli Grayscale (8-bit)");

    // 2. Data RGB Hasil (Ambil sampel channel Blue)
    analyzeValues(imgLowBitRGB, "[AFTER]  Citra RGB Quantized (Channel Blue)");

    // 3. Data Gray Hasil
    analyzeValues(imgLowBitGray, "[AFTER]  Citra Grayscale Quantized");

    std::cout << "Note: Nilai pixel menjadi lompat-lompat (diskrit) karena bit rendah dibuang." << std::endl;

    // --- TAMPILKAN HASIL VISUAL ---

    // Normalisasi agar hasil raw yang gelap bisa dilihat polanya dengan jelas (Opsional)
    // Tanpa ini, gambar 2-bit akan terlihat sangat gelap (nilai max cuma 192)
    cv::Mat imgGrayDisplay;
    cv::normalize(imgLowBitGray, imgGrayDisplay, 0, 255, cv::NORM_MINMAX);

    cv::namedWindow("1. Original RGB", cv::WINDOW_AUTOSIZE);
    cv::imshow("1. Original RGB", imgOriginal);

    cv::namedWindow("2. Downsampled (Low Res)", cv::WINDOW_AUTOSIZE);
    cv::imshow("2. Downsampled (Low Res)", imgSmall);

    cv::namedWindow("3. Quantized RGB (False Contour)", cv::WINDOW_AUTOSIZE);
    cv::imshow("3. Quantized RGB (False Contour)", imgLowBitRGB);

    // Visualisasi urutan grayscale
    cv::imshow("4. Original Gray", imgGray);
    cv::imshow("5. Quantized Gray (Raw Data)", imgLowBitGray);
    cv::imshow("6. Quantized Gray (Normalized)", imgGrayDisplay);

    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}