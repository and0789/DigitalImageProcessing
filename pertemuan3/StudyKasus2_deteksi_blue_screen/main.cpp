#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    // 1. Load Gambar (Presenter & Background Baru)
    // Pastikan mahasiswa memiliki file ini di folder project
    cv::Mat img_presenter = cv::imread("presenter_blue.jpg");
    cv::Mat img_background = cv::imread("eiffel_tower.jpg");

    // Validasi file
    if (img_presenter.empty() || img_background.empty()) {
        std::cerr << "[ERROR] Gagal memuat salah satu gambar!" << std::endl;
        return -1;
    }

    // 2. Samakan Ukuran Background (SANGAT PENTING)
    // Background baru harus di-resize agar sama persis dengan ukuran foto presenter.
    // Jika tidak, operasi bitwise akan error karena dimensi matriks tidak cocok.
    cv::resize(img_background, img_background, img_presenter.size());

    // 3. Konversi ke HSV untuk Deteksi Warna
    cv::Mat hsv;
    cv::cvtColor(img_presenter, hsv, cv::COLOR_BGR2HSV);

    // 4. Deteksi Warna Biru (Membuat Mask)
    // Range Biru (H: 100-130)
    cv::Scalar lower_blue(100, 150, 0);   // Saturation min 150 agar biru pekat
    cv::Scalar upper_blue(140, 255, 255);

    cv::Mat mask_blue, mask_person;
    cv::inRange(hsv, lower_blue, upper_blue, mask_blue);

    // [Opsional] Pembersihan Noise (Morphology)
    // Membersihkan bintik-bintik putih kecil di area mask
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::morphologyEx(mask_blue, mask_blue, cv::MORPH_OPEN, kernel);
    cv::morphologyEx(mask_blue, mask_blue, cv::MORPH_DILATE, kernel);

    // 5. Buat Invert Mask (Area Orang)
    // Mask Biru   : Putih = Background, Hitam = Orang
    // Mask Orang  : Putih = Orang, Hitam = Background
    cv::bitwise_not(mask_blue, mask_person);

    // 6. Operasi Bitwise (Compositing)
    cv::Mat result_bg, result_fg, final_image;

    // A. Ambil Background Baru HANYA di area biru (Mask Biru)
    // Area orang akan menjadi HITAM
    cv::bitwise_and(img_background, img_background, result_bg, mask_blue);

    // B. Ambil Presenter HANYA di area orang (Mask Person)
    // Area background biru akan menjadi HITAM
    cv::bitwise_and(img_presenter, img_presenter, result_fg, mask_person);

    // C. Gabungkan keduanya (Add)
    // Hitam (0) + Warna = Warna
    cv::add(result_bg, result_fg, final_image);

    // 7. Tampilkan Hasil
    cv::imshow("1. Original Presenter", img_presenter);
    cv::imshow("2. Mask (Blue Area)", mask_blue);
    cv::imshow("3. Extracted Person", result_fg);
    cv::imshow("4. Final Chroma Key", final_image);

    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}