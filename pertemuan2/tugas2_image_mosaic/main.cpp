#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    // 1. Load Citra Sumber
    std::string filename = "lena.jpg";
    cv::Mat src = cv::imread(filename);

    if (src.empty()) {
        std::cerr << "[ERROR] Gagal memuat gambar!" << std::endl;
        return -1;
    }

    // 2. Standarisasi Ukuran (Resize)
    // Penting: Agar mozaik rapi, semua potongan harus berukuran sama.
    // Kita kecilkan jadi 300x300 agar muat di layar laptop.
    int chunkWidth = 300;
    int chunkHeight = 300;
    cv::Size contentSize(chunkWidth, chunkHeight);

    cv::Mat imgResized;
    cv::resize(src, imgResized, contentSize);

    // 3. Persiapan Variabel Border
    int borderSize = 15; // Tebal garis putih (pixel)

    // 4. Hitung Ukuran Canvas Total
    // Lebar  = (Gambar Kiri) + Border + (Gambar Kanan)
    // Tinggi = (Gambar Atas) + Border + (Gambar Bawah)
    int canvasWidth = (chunkWidth * 2) + borderSize;
    int canvasHeight = (chunkHeight * 2) + borderSize;

    // 5. Buat Canvas Kosong (Background Putih)
    // CV_8UC3 = 8-bit Unsigned, 3 Channels (Color)
    // Scalar(255, 255, 255) = Putih
    cv::Mat mosaicCanvas(canvasHeight, canvasWidth, CV_8UC3, cv::Scalar(255, 255, 255));

    // ==========================================
    // PENYUSUNAN MOZAIK MENGGUNAKAN ROI
    // ==========================================

    // --- POSISI 1: Kiri Atas (Original) ---
    // x = 0, y = 0
    cv::Rect roi1(0, 0, chunkWidth, chunkHeight);

    // Ambil akses ke area tersebut di canvas
    cv::Mat target1 = mosaicCanvas(roi1);
    // Copy gambar ke area tersebut
    imgResized.copyTo(target1);


    // --- POSISI 2: Kanan Atas (Invert Color) ---
    // x = Lebar + Border, y = 0
    cv::Rect roi2(chunkWidth + borderSize, 0, chunkWidth, chunkHeight);

    cv::Mat target2 = mosaicCanvas(roi2);
    // Operasi Inversi (Negatif Film) langsung di canvas
    cv::bitwise_not(imgResized, target2);


    // --- POSISI 3: Kiri Bawah (Grayscale) ---
    // x = 0, y = Tinggi + Border
    cv::Rect roi3(0, chunkHeight + borderSize, chunkWidth, chunkHeight);

    cv::Mat target3 = mosaicCanvas(roi3);
    cv::Mat grayTemp;
    // Ubah ke Gray, lalu convert balik ke BGR agar channelnya cocok dengan canvas (3 channel)
    cv::cvtColor(imgResized, grayTemp, cv::COLOR_BGR2GRAY);
    cv::cvtColor(grayTemp, target3, cv::COLOR_GRAY2BGR);


    // --- POSISI 4: Kanan Bawah (Gaussian Blur) ---
    // x = Lebar + Border, y = Tinggi + Border
    cv::Rect roi4(chunkWidth + borderSize, chunkHeight + borderSize, chunkWidth, chunkHeight);

    cv::Mat target4 = mosaicCanvas(roi4);
    cv::GaussianBlur(imgResized, target4, cv::Size(15, 15), 0);

    // ==========================================
    // TAMPILKAN HASIL
    // ==========================================

    std::cout << "Ukuran Canvas: " << canvasWidth << " x " << canvasHeight << std::endl;
    std::cout << "Ukuran Chunk : " << chunkWidth << " x " << chunkHeight << std::endl;
    std::cout << "Border Size  : " << borderSize << " px" << std::endl;

    cv::namedWindow("2x2 Image Mosaic", cv::WINDOW_AUTOSIZE);
    cv::imshow("2x2 Image Mosaic", mosaicCanvas);

    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}