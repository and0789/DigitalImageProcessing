#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono> // Library untuk analisis waktu
#include <vector>

int main() {
    // 1. Konfigurasi
    const std::string filename = "lena.jpg";
    cv::Mat imgOriginal = cv::imread(filename);

    if (imgOriginal.empty()) {
        std::cerr << "[ERROR] Gagal memuat gambar: " << filename << std::endl;
        return -1;
    }

    std::cout << "=== ANALISIS 1: BUKTI VISUAL & MEMORI (DATA SHARING) ===" << std::endl;

    // --- A. SHALLOW COPY (Salinan Dangkal) ---
    // Hanya menyalin Header dan Pointer. Data pixel TIDAK disalin.
    // Keduanya menunjuk ke alamat memori yang SAMA.
    cv::Mat imgShallow = imgOriginal;

    // --- B. DEEP COPY (Salinan Dalam) ---
    // Mengalokasikan memori BARU dan menyalin seluruh data pixel.
    // Keduanya menunjuk ke alamat memori yang BERBEDA.
    cv::Mat imgDeep = imgOriginal.clone();

    // 2. Bukti Pointer (Forensik Memori)
    // Kita cetak alamat memori tempat data pixel disimpan (.data)
    // (void*) digunakan agar cout mencetak alamat hexadecimal, bukan string.
    std::cout << "Alamat Data Original : " << (void*)imgOriginal.data << std::endl;
    std::cout << "Alamat Data Shallow  : " << (void*)imgShallow.data
              << " -> " << (imgOriginal.data == imgShallow.data ? "[SAMA - BERBAHAYA]" : "[BEDA]") << std::endl;
    std::cout << "Alamat Data Deep     : " << (void*)imgDeep.data
              << " -> " << (imgOriginal.data == imgDeep.data ? "[SAMA]" : "[BEDA - AMAN]") << std::endl;

    // 3. Bukti Modifikasi (Side Effect)
    // Kita gambar kotak MERAH di imgShallow
    cv::rectangle(imgShallow, cv::Point(50, 50), cv::Point(200, 200), cv::Scalar(0, 0, 255), 3);

    // Kita gambar lingkaran HIJAU di imgDeep
    cv::circle(imgDeep, cv::Point(300, 300), 50, cv::Scalar(0, 255, 0), -1);

    std::cout << "\nStatus Modifikasi:" << std::endl;
    std::cout << " -> Menggambar kotak merah di 'imgShallow'..." << std::endl;
    std::cout << " -> Menggambar lingkaran hijau di 'imgDeep'..." << std::endl;
    std::cout << " -> Cek 'imgOriginal', apakah ikut berubah?" << std::endl;

    // ============================================================
    // ANALISIS 2: PERFORMA & WAKTU (BENCHMARKING)
    // ============================================================
    std::cout << "\n=== ANALISIS 2: PERFORMA WAKTU (1000x Iterasi) ===" << std::endl;
    std::cout << "Mengukur biaya komputasi..." << std::endl;

    int iterations = 1000; // Ulangi 1000 kali agar perbedaannya terasa

    // A. Benchmark Shallow Copy
    auto startShallow = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < iterations; i++) {
        cv::Mat temp = imgOriginal; // Operasi O(1) - Sangat Cepat
    }
    auto endShallow = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> durationShallow = endShallow - startShallow;

    // B. Benchmark Deep Copy
    auto startDeep = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < iterations; i++) {
        cv::Mat temp = imgOriginal.clone(); // Operasi O(N) - Lambat (Alokasi + Copy)
    }
    auto endDeep = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> durationDeep = endDeep - startDeep;

    // Tampilkan Hasil Benchmark
    std::cout << "1. Waktu Shallow Copy : " << durationShallow.count() << " ms (Total)" << std::endl;
    std::cout << "2. Waktu Deep Copy    : " << durationDeep.count() << " ms (Total)" << std::endl;

    double ratio = durationDeep.count() / durationShallow.count();
    std::cout << "KESIMPULAN: Deep Copy " << ratio << "x LEBIH LAMBAT daripada Shallow Copy!" << std::endl;

    // 4. Visualisasi
    cv::namedWindow("1. Original (Ikut Kena Efek Shallow)", cv::WINDOW_AUTOSIZE);
    cv::imshow("1. Original (Ikut Kena Efek Shallow)", imgOriginal);

    cv::namedWindow("2. Shallow Copy (Ada Kotak Merah)", cv::WINDOW_AUTOSIZE);
    cv::imshow("2. Shallow Copy (Ada Kotak Merah)", imgShallow);

    cv::namedWindow("3. Deep Copy (Ada Lingkaran Hijau)", cv::WINDOW_AUTOSIZE);
    cv::imshow("3. Deep Copy (Ada Lingkaran Hijau)", imgDeep);

    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}