#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <iomanip> // Untuk format angka koma

// Fungsi Helper untuk format byte ke MB
double bytesToMB(unsigned long long bytes) {
    return bytes / (1024.0 * 1024.0);
}

int main() {
    // 1. Buat Citra 4K (3840 x 2160), 3 Channel (Warna)
    // Kita gunakan citra hitam kosong saja agar tidak perlu load file eksternal
    int width = 3840;
    int height = 2160;

    std::cout << "=== MEMORY PROFILING SIMULATION ===" << std::endl;
    std::cout << "Membuat Citra 4K (" << width << "x" << height << ")..." << std::endl;

    cv::Mat original = cv::Mat::zeros(height, width, CV_8UC3);

    // Hitung ukuran 1 gambar raw
    // total() = jumlah pixel, elemSize() = byte per pixel (3 byte untuk RGB)
    unsigned long long singleImageSize = original.total() * original.elemSize();

    std::cout << "Ukuran 1 Citra 4K: " << bytesToMB(singleImageSize) << " MB" << std::endl;
    std::cout << "-------------------------------------------" << std::endl;

    // ==========================================
    // SKENARIO 1: 10 SHALLOW COPIES
    // ==========================================
    std::vector<cv::Mat> shallowList;

    for(int i=0; i<10; i++) {
        // Operator '=' hanya menyalin header (Shallow)
        cv::Mat temp = original;
        shallowList.push_back(temp);
    }

    // Analisis Shallow
    // Karena semua menunjuk ke data yang sama, memori hanya bertambah seukuran Header
    // Kita abaikan ukuran header (karena sangat kecil, ~bytes)
    unsigned long long totalShallow = singleImageSize;

    std::cout << "[1] 10 Shallow Copies:" << std::endl;
    std::cout << "    -> Logic: Semua variabel menunjuk ke 1 blok memori data yang sama." << std::endl;
    std::cout << "    -> Total Estimasi Memori: " << bytesToMB(totalShallow) << " MB" << std::endl;


    // ==========================================
    // SKENARIO 2: 10 DEEP COPIES
    // ==========================================
    std::vector<cv::Mat> deepList;

    for(int i=0; i<10; i++) {
        // Fungsi .clone() mengalokasikan memori baru (Deep)
        cv::Mat temp = original.clone();
        deepList.push_back(temp);
    }

    // Analisis Deep
    // Setiap clone punya blok data sendiri
    unsigned long long totalDeep = singleImageSize * 10; // Ditambah 1 master jika dihitung total 11

    std::cout << "\n[2] 10 Deep Copies:" << std::endl;
    std::cout << "    -> Logic: Ada 10 alokasi memori baru yang terpisah." << std::endl;
    std::cout << "    -> Total Estimasi Memori: " << bytesToMB(totalDeep) << " MB" << std::endl;

    // ==========================================
    // KESIMPULAN / ANALISIS
    // ==========================================
    std::cout << "-------------------------------------------" << std::endl;
    double ratio = (double)totalDeep / (double)totalShallow;

    std::cout << "ANALISIS AKHIR:" << std::endl;
    std::cout << "Deep Copy memakan memori " << ratio << " kali lipat lebih banyak daripada Shallow Copy." << std::endl;

    // Tahan console
    std::cin.get();
    return 0;
}