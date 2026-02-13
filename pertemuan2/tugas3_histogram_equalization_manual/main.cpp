#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <cmath> // Untuk fungsi round()

int main() {
    // 1. Load Citra (Wajib Grayscale untuk Histogram Equalization dasar)
    std::string filename = "lena.jpg";
    cv::Mat src = cv::imread(filename, cv::IMREAD_GRAYSCALE);

    if (src.empty()) {
        std::cerr << "[ERROR] Gagal memuat gambar!" << std::endl;
        return -1;
    }

    // ============================================================
    // BAGIAN 1: IMPLEMENTASI MANUAL (Tanpa equalizeHist)
    // ============================================================

    cv::Mat manualResult = src.clone();

    // A. Hitung Histogram (Frekuensi Pixel)
    int hist[256] = {0}; // Inisialisasi 0 semua

    for (int y = 0; y < src.rows; y++) {
        // Menggunakan ptr<> sesuai perintah tugas
        uchar* ptr = src.ptr<uchar>(y);
        for (int x = 0; x < src.cols; x++) {
            hist[ptr[x]]++;
        }
    }

    // B. Hitung CDF (Cumulative Distribution Function)
    // CDF[i] = hist[0] + hist[1] + ... + hist[i]
    int cdf[256] = {0};
    cdf[0] = hist[0];
    for (int i = 1; i < 256; i++) {
        cdf[i] = cdf[i-1] + hist[i];
    }

    // C. Buat Lookup Table (LUT) - Normalisasi
    // Rumus Equalization: h(v) = round( (cdf(v) - cdf_min) / (TotalPixel - cdf_min) * 255 )
    // Catatan: Rumus sederhana round(cdf(v)/Total * 255) juga sering dipakai,
    // tapi rumus OpenCV menggunakan 'cdf_min' agar lebih akurat.

    uchar lut[256];
    int totalPixels = src.rows * src.cols;
    int cdfMin = 0;

    // Cari cdf non-zero pertama (cdf_min)
    for(int i=0; i<256; i++) {
        if(cdf[i] > 0) {
            cdfMin = cdf[i];
            break;
        }
    }

    for (int i = 0; i < 256; i++) {
        // Penerapan Rumus Normalisasi ke rentang 0-255
        float normalized = (float)(cdf[i] - cdfMin) / (float)(totalPixels - cdfMin);
        lut[i] = (uchar)std::round(normalized * 255.0f);
    }

    // D. Terapkan LUT ke Citra (Mapping)
    for (int y = 0; y < manualResult.rows; y++) {
        uchar* ptr = manualResult.ptr<uchar>(y); // Pointer baris
        for (int x = 0; x < manualResult.cols; x++) {
            // Ganti nilai lama dengan nilai baru dari tabel
            ptr[x] = lut[ptr[x]];
        }
    }

    // ============================================================
    // BAGIAN 2: IMPLEMENTASI OPENCV (Sebagai Benchmark)
    // ============================================================

    cv::Mat opencvResult;
    cv::equalizeHist(src, opencvResult);

    // ============================================================
    // BAGIAN 3: ANALISIS PERBANDINGAN (Difference Check)
    // ============================================================

    // Hitung selisih absolut antara Manual vs OpenCV
    cv::Mat diff;
    cv::absdiff(manualResult, opencvResult, diff);

    // Cek apakah ada pixel yang beda (Non-Zero)
    int totalErrors = cv::countNonZero(diff);

    std::cout << "=== ANALISIS HASIL ===" << std::endl;
    std::cout << "Jumlah pixel total: " << totalPixels << std::endl;
    std::cout << "Jumlah pixel beda : " << totalErrors << " (Manual vs OpenCV)" << std::endl;

    if (totalErrors == 0) {
        std::cout << "KESIMPULAN: Implementasi manual 100% AKURAT menyamai OpenCV!" << std::endl;
    } else {
        std::cout << "KESIMPULAN: Ada sedikit perbedaan (biasanya karena pembulatan float/double)." << std::endl;
    }

    // Tampilkan
    cv::imshow("1. Original Gray", src);
    cv::imshow("2. Manual Equalization", manualResult);
    cv::imshow("3. OpenCV equalizeHist", opencvResult);

    // Trik Visual: Tampilkan bedanya (biasanya hitam semua jika sama)
    // Kita kali 50 supaya kalau ada beda dikit langsung kelihatan putih
    cv::imshow("4. Difference (x50 amp)", diff * 50);

    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}