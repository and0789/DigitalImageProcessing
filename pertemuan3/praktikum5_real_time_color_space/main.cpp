#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

int main() {
    // 1. Inisialisasi Kamera
    // ID 0 biasanya adalah webcam default/bawaan laptop.
    // Gunakan 1 atau 2 jika menggunakan kamera eksternal USB.
    cv::VideoCapture cap(0);

    // Validasi apakah kamera berhasil dibuka
    if (!cap.isOpened()) {
        std::cerr << "[ERROR] Kamera tidak terdeteksi atau sedang digunakan aplikasi lain!" << std::endl;
        return -1;
    }

    std::cout << "Kamera berhasil dibuka." << std::endl;
    std::cout << "Tekan tombol 'q' atau 'ESC' di keyboard untuk keluar." << std::endl;

    // Persiapkan variabel matriks di luar loop untuk efisiensi alokasi memori
    cv::Mat frame, hsv, ycrcb;
    std::vector<cv::Mat> hsv_channels;

    // 2. Loop Utama (Infinite Loop)
    while (true) {
        // Ambil frame dari kamera
        cap >> frame;

        // Cek jika frame kosong (misal kamera tiba-tiba copot)
        if (frame.empty()) {
            std::cerr << "[WARNING] Frame kosong (akhir stream atau error)!" << std::endl;
            break;
        }

        // 3. Proses Konversi Warna
        // BGR ke HSV
        cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

        // BGR ke YCrCb
        cv::cvtColor(frame, ycrcb, cv::COLOR_BGR2YCrCb);

        // Memecah channel HSV untuk visualisasi Hue
        cv::split(hsv, hsv_channels);

        // 4. Tampilkan di Window
        cv::imshow("1. Webcam Asli (BGR)", frame);

        // Channel 0 dari HSV adalah Hue (Warna dasar tanpa kecerahan/saturasi)
        cv::imshow("2. HUE Channel Only", hsv_channels[0]);

        // Tampilkan ruang warna YCrCb (bagus untuk deteksi kulit nanti)
        cv::imshow("3. YCrCb Space", ycrcb);

        // 5. Kontrol Loop
        // waitKey(30) berarti menunggu 30ms.
        // Ini memberi efek sekitar ~30 FPS (1000ms / 30ms = 33.3 fps)
        char key = (char)cv::waitKey(30);

        // Keluar jika tombol 'q' atau 'ESC' (kode ASCII 27) ditekan
        if (key == 'q' || key == 27) {
            break;
        }
    }

    // 6. Bersihkan Resource
    cap.release(); // Lepaskan akses kamera
    cv::destroyAllWindows(); // Tutup semua jendela

    return 0;
}