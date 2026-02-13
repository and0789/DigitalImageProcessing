#include <opencv2/opencv.hpp>
#include <iostream>
#include <thread> // Untuk std::this_thread
#include <chrono> // Untuk std::chrono

int main() {
    // 1. Inisialisasi Kamera
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "[ERROR] Kamera tidak terdeteksi!" << std::endl;
        return -1;
    }

    // Konfigurasi Ukuran Frame (Opsional, agar resolusi konsisten)
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);

    cv::Mat background, frame, hsv, mask1, mask2, final_output;
    cv::Mat res1, res2; // Variabel temporary untuk hasil masking

    // 2. Persiapan & Pemanasan Kamera
    // Penting: Biarkan kamera menyesuaikan cahaya (Auto Exposure/White Balance)
    // sebelum mengambil background referensi.
    std::cout << "Menyiapkan kamera... Harap tunggu..." << std::endl;
    for (int i = 0; i < 60; i++) {
        cap >> background;
    }

    std::cout << "!!! PERSIAPAN PENGAMBILAN BACKGROUND !!!" << std::endl;
    std::cout << "MINGGIR DARI KAMERA DALAM 3 DETIK..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));

    // 3. Ambil Background Referensi (Tanpa Objek/Orang)
    // Kita ambil beberapa frame lalu ambil yang terakhir untuk memastikan bersih
    for (int i = 0; i < 30; i++) {
        cap >> background;
    }

    // Flip background agar sesuai dengan efek cermin (mirror)
    // 1 = Horizontal Flip
    cv::flip(background, background, 1);

    std::cout << "Background tersimpan! Silakan masuk dengan kain biru." << std::endl;
    std::cout << "Tekan 'q' untuk keluar." << std::endl;

    // 4. Pre-alokasi Kernel Morfologi (Optimasi: Dibuat sekali saja di luar loop)
    // Kernel 3x3 untuk membersihkan noise kecil
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));

    // Definisi Range Warna Biru (HSV)
    // H: 0-180, S: 0-255, V: 0-255
    // Range Biru biasanya sekitar 100-130
    const cv::Scalar lower_blue(90, 120, 70);
    const cv::Scalar upper_blue(130, 255, 255);

    // 5. Loop Utama
    while (true) {
        cap >> frame;
        if (frame.empty()) break;

        // Efek Cermin (Harus sama dengan perlakuan ke background)
        cv::flip(frame, frame, 1);

        // Konversi ke HSV
        cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

        // --- DETEKSI KAIN BIRU ---
        // Membuat Masking: Area kain biru akan berwarna Putih (255), sisanya Hitam (0)
        cv::inRange(hsv, lower_blue, upper_blue, mask1);

        // --- PEMBERSIHAN MASK (Refinement) ---
        // MORPH_OPEN: Menghilangkan noise bintik putih kecil di background
        cv::morphologyEx(mask1, mask1, cv::MORPH_OPEN, kernel);
        // MORPH_DILATE: Memperlebar area putih agar lubang-lubang kecil di kain tertutup
        cv::morphologyEx(mask1, mask1, cv::MORPH_DILATE, kernel);

        // Membuat Mask Invers (Untuk area SELAIN kain biru / Orang)
        cv::bitwise_not(mask1, mask2);

        // --- LOGIKA INVISIBILITY ---

        // Bagian 1: Pengganti Kain (Cloak)
        // Isi area kain biru (mask1) dengan pixel dari BACKGROUND yang sudah disimpan
        cv::bitwise_and(background, background, res1, mask1);

        // Bagian 2: Orang/Objek Nyata
        // Isi area selain kain (mask2) dengan pixel dari FRAME SAAT INI
        cv::bitwise_and(frame, frame, res2, mask2);

        // Gabungkan Bagian 1 + Bagian 2
        // Karena mask1 dan mask2 saling eksklusif (tidak tumpang tindih),
        // kita bisa langsung menjumlahkannya.
        cv::add(res1, res2, final_output);

        // Tampilkan
        cv::imshow("Invisibility Cloak", final_output);
        // Debugging: Uncomment baris bawah untuk melihat maskernya
        // cv::imshow("Mask Debug", mask1);

        if (cv::waitKey(1) == 'q') break;
    }

    cap.release();
    cv::destroyAllWindows();
    return 0;
}