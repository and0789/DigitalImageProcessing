#include <iostream>
#include <opencv2/opencv.hpp>
#include <iomanip> // Library untuk merapikan output teks (setw)

// Fungsi helper untuk menerjemahkan kode Depth (int) ke String
std::string getDepthName(int depth) {
    switch (depth) {
        case CV_8U:  return "CV_8U (8-bit Unsigned)";
        case CV_8S:  return "CV_8S (8-bit Signed)";
        case CV_16U: return "CV_16U (16-bit Unsigned)";
        case CV_16S: return "CV_16S (16-bit Signed)";
        case CV_32S: return "CV_32S (32-bit Signed)";
        case CV_32F: return "CV_32F (32-bit Float)";
        case CV_64F: return "CV_64F (64-bit Double)";
        default:     return "Unknown";
    }
}

int main() {
    // 1. Load Citra
    const std::string filename = "img.png"; // Ganti dengan path gambar Anda
    cv::Mat image = cv::imread(filename);

    // 2. Validasi Load
    if (image.empty()) {
        std::cerr << "[ERROR] Gambar tidak ditemukan: " << filename << std::endl;
        return -1;
    }

    // 3. Menampilkan Properti Citra dengan Formatting Rapi
    std::cout << "=== PROPERTI CITRA ===" << std::endl;
    std::cout << std::left; // Rata kiri

    // Menggunakan setw(25) agar kolom titik dua (:) sejajar
    std::cout << std::setw(25) << "Rows (Tinggi)" << ": " << image.rows << " pixels" << std::endl;
    std::cout << std::setw(25) << "Cols (Lebar)" << ": " << image.cols << " pixels" << std::endl;
    std::cout << std::setw(25) << "Channels (Saluran)" << ": " << image.channels() << " channels" << std::endl;

    // Info Depth yang lebih informatif
    std::cout << std::setw(25) << "Depth (Kedalaman)" << ": " << image.depth() << " (" << getDepthName(image.depth()) << ")" << std::endl;
    std::cout << std::setw(25) << "Type ID" << ": " << image.type() << std::endl;

    std::cout << std::setw(25) << "Total Pixels" << ": " << image.total() << " pixels" << std::endl;
    std::cout << std::setw(25) << "Is Continuous" << ": " << (image.isContinuous() ? "Yes (Memori urut)" : "No (Ada gap/padding)") << std::endl;
    std::cout << std::setw(25) << "Elem Size (Bytes/Px)" << ": " << image.elemSize() << " bytes" << std::endl;
    std::cout << std::setw(25) << "Step (Bytes/Row)" << ": " << image.step << " bytes" << std::endl;

    std::cout << "---------------------------------------------" << std::endl;

    // 4. Akses Pixel (Safety Check Penting!)
    int x = 200;
    int y = 200;

    // Cek apakah koordinat (200, 200) ada di dalam gambar?
    if (x < image.cols && y < image.rows) {

        // Cek Tipe Channel (Color vs Grayscale)
        if (image.channels() == 3) {
            // Akses 3 Channel (BGR)
            cv::Vec3b pixel = image.at<cv::Vec3b>(y, x);
            std::cout << "Data Pixel di (" << x << ", " << y << "):" << std::endl;
            std::cout << "  - Blue : " << (int)pixel[0] << std::endl;
            std::cout << "  - Green: " << (int)pixel[1] << std::endl;
            std::cout << "  - Red  : " << (int)pixel[2] << std::endl;
        }
        else if (image.channels() == 1) {
            // Akses 1 Channel (Grayscale)
            uchar pixel = image.at<uchar>(y, x);
            std::cout << "Data Pixel di (" << x << ", " << y << "):" << std::endl;
            std::cout << "  - Intensity: " << (int)pixel << std::endl;
        }
    } else {
        std::cerr << "[WARNING] Koordinat (" << x << "," << y << ") di luar dimensi gambar!" << std::endl;
    }

    // 5. Hitung Rata-rata Warna (Mean)
    cv::Scalar avgColor = cv::mean(image);
    std::cout << "---------------------------------------------" << std::endl;
    std::cout << "Rata-rata Warna Citra:" << std::endl;
    std::cout << "  - Avg Blue : " << avgColor[0] << std::endl;
    std::cout << "  - Avg Green: " << avgColor[1] << std::endl;
    std::cout << "  - Avg Red  : " << avgColor[2] << std::endl;

    // 6. Tampilkan Gambar
    cv::imshow("Tampilan Gambar", image);
    cv::waitKey(0);

    return 0;
}