#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

int main() {
    // 1. Inisialisasi Kamera (Ganti angka 0 jika pakai kamera eksternal)
    // Atau gunakan cv::imread("path/to/image.jpg") jika menggunakan file statis
    cv::VideoCapture cap(0);

    if (!cap.isOpened()) {
        std::cerr << "[ERROR] Kamera tidak dapat dibuka!" << std::endl;
        return -1;
    }

    cv::Mat frame, hsv, mask;
    std::vector<cv::Mat> hsv_channels; // Vector untuk menampung 3 channel (H, S, V)

    // Target Warna: Hijau (Dalam OpenCV Hue range 0-179, Hijau sekitar 60)
    const int TARGET_HUE_GREEN = 60;

    std::cout << "Program Color Replacement Berjalan..." << std::endl;
    std::cout << "Arahkan benda BIRU ke kamera. Tekan 'q' untuk keluar." << std::endl;

    while (true) {
        cap >> frame;
        if (frame.empty()) break;

        // 2. Konversi ke HSV
        cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

        // 3. Buat Masking Warna Biru
        // Range Biru: H(100-130), S(150-255), V(50-255)
        // Saturation & Value minimal diatur agar tidak mendeteksi warna 'abu-abu' atau gelap
        cv::Scalar lower_blue(100, 150, 50);
        cv::Scalar upper_blue(130, 255, 255);

        cv::inRange(hsv, lower_blue, upper_blue, mask);

        // Opsional: Bersihkan noise pada mask
        cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
        cv::morphologyEx(mask, mask, cv::MORPH_OPEN, kernel);
        cv::morphologyEx(mask, mask, cv::MORPH_DILATE, kernel);

        // 4. Split Channel HSV
        // Kita perlu memecah agar bisa memanipulasi HUE saja tanpa mengganggu Saturation & Value
        cv::split(hsv, hsv_channels);

        // hsv_channels[0] = Hue
        // hsv_channels[1] = Saturation
        // hsv_channels[2] = Value

        // 5. Ganti Warna (The Magic Step)
        // Ubah nilai channel Hue menjadi 60 (Hijau) HANYA pada lokasi di mana mask bernilai putih (255).
        // Fungsi setTo sangat efisien menggantikan loop for.
        hsv_channels[0].setTo(cv::Scalar(TARGET_HUE_GREEN), mask);

        // Channel Saturation dan Value DIBIARKAN ASLI.
        // Ini kunci agar bayangan dan tekstur objek tetap terlihat natural.

        // 6. Gabungkan Kembali (Merge)
        cv::merge(hsv_channels, hsv);

        // 7. Konversi Balik ke BGR untuk Display
        cv::Mat result;
        cv::cvtColor(hsv, result, cv::COLOR_HSV2BGR);

        // Tampilkan
        cv::imshow("1. Original", frame);
        cv::imshow("2. Detection Mask", mask);
        cv::imshow("3. Color Replacement Result", result);

        if (cv::waitKey(1) == 'q') break;
    }

    cap.release();
    cv::destroyAllWindows();
    return 0;
}