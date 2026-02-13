#include <opencv2/opencv.hpp>
#include <iostream>

// Variabel Global untuk menyimpan posisi Trackbar
// Di OpenCV HighGUI sederhana, variabel global sering digunakan untuk trackbar
// agar bisa diakses langsung oleh callback (meskipun di aplikasi besar sebaiknya dihindari).
const int max_value_H = 179; // OpenCV menggunakan 0-179 untuk Hue (360/2)
const int max_value = 255;   // Max untuk Saturation dan Value

int low_H = 0, low_S = 0, low_V = 0;
int high_H = 179, high_S = 255, high_V = 255;

// Nama Window sebagai konstanta agar konsisten
const std::string window_capture = "1. Original Camera";
const std::string window_detection = "2. Detection Result";
const std::string window_controls = "3. Control Panel";

// Fungsi callback trackbar (wajib ada untuk signature createTrackbar)
static void on_trackbar_change(int, void*) {
    // Kita tidak perlu isi apa-apa di sini karena variabel global
    // update otomatis lewat pointer di createTrackbar
}

int main() {
    // 1. Inisialisasi Kamera
    cv::VideoCapture cap(0);

    if (!cap.isOpened()) {
        std::cerr << "[ERROR] Kamera tidak terdeteksi!" << std::endl;
        return -1;
    }

    // 2. Setup GUI Windows
    cv::namedWindow(window_capture, cv::WINDOW_AUTOSIZE);
    cv::namedWindow(window_detection, cv::WINDOW_AUTOSIZE);

    // Window kontrol dibuat terpisah
    cv::namedWindow(window_controls, cv::WINDOW_NORMAL);
    cv::resizeWindow(window_controls, 400, 350); // Ukuran fix agar trackbar terlihat jelas

    // 3. Membuat Trackbars
    // Syntax: createTrackbar(NamaTrackbar, NamaWindow, &VariabelYangDiubah, MaxValue, Callback)
    cv::createTrackbar("Low Hue", window_controls, &low_H, max_value_H, on_trackbar_change);
    cv::createTrackbar("High Hue", window_controls, &high_H, max_value_H, on_trackbar_change);

    cv::createTrackbar("Low Sat", window_controls, &low_S, max_value, on_trackbar_change);
    cv::createTrackbar("High Sat", window_controls, &high_S, max_value, on_trackbar_change);

    cv::createTrackbar("Low Val", window_controls, &low_V, max_value, on_trackbar_change);
    cv::createTrackbar("High Val", window_controls, &high_V, max_value, on_trackbar_change);

    // Variabel matriks (di luar loop untuk efisiensi)
    cv::Mat frame, hsv, mask, result;

    std::cout << "Tekan 'q' atau 'ESC' untuk keluar." << std::endl;
    std::cout << "Geser trackbar untuk mencari warna yang diinginkan." << std::endl;

    while (true) {
        // Ambil frame
        cap >> frame;
        if (frame.empty()) break;

        // Proses HSV
        cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

        // Buat Masking berdasarkan nilai Trackbar saat ini
        // Scalar(H, S, V)
        cv::Scalar lower_bound(low_H, low_S, low_V);
        cv::Scalar upper_bound(high_H, high_S, high_V);

        cv::inRange(hsv, lower_bound, upper_bound, mask);

        // Bitwise AND untuk menampilkan warna asli hanya pada area mask
        // Area hitam di mask akan tetap hitam, area putih di mask akan mengambil warna asli
        result = cv::Scalar::all(0); // Reset result jadi hitam bersih
        cv::bitwise_and(frame, frame, result, mask);

        // Tampilkan
        cv::imshow(window_capture, frame);
        cv::imshow("Mask (Binary)", mask); // Berguna untuk melihat noise
        cv::imshow(window_detection, result);

        // Tunggu 30ms
        char key = (char)cv::waitKey(30);
        if (key == 'q' || key == 27) break;
    }

    // Cleanup
    cap.release();
    cv::destroyAllWindows();
    return 0;
}