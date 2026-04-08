#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <chrono> // Untuk mengukur waktu komputasi

using namespace cv;
using namespace std;

/**
 * =========================================================================
 * FUNGSI KHUSUS: Implementasi Rumus Matematika Murni (Tanpa OpenCV API)
 * =========================================================================
 * Rumus Matematis: g(x,y) = sum_{s=-a}^{a} sum_{t=-b}^{b} w(s,t) * f(x-s, y-t)
 * * Fungsi ini dibuat semata-mata untuk tujuan edukasi agar mahasiswa
 * melihat implementasi 1:1 dari rumus matematika di atas menggunakan
 * struktur array 2D standar di C++.
 */
vector<vector<float>> konvolusiMatematikaMurni(
    const vector<vector<float>>& f, // Citra masukan f(x,y)
    const vector<vector<float>>& w  // Matriks Kernel w(s,t)
) {
    int barisCitra = f.size();
    int kolomCitra = f[0].size();

    int barisKernel = w.size();
    int kolomKernel = w[0].size();

    // Menghitung batas radius kernel (nilai 'a' dan 'b' pada rumus)
    int a = barisKernel / 2;
    int b = kolomKernel / 2;

    // Siapkan matriks citra keluaran g(x,y), inisialisasi semua dengan 0
    vector<vector<float>> g(barisCitra, vector<float>(kolomCitra, 0.0f));

    // Looping untuk memproses setiap titik piksel (x, y) pada citra
    for (int x = 0; x < barisCitra; x++) {
        for (int y = 0; y < kolomCitra; y++) {

            float sum = 0.0f;

            // Tanda Sigma Ganda: Loop untuk menggeser kernel
            for (int s = -a; s <= a; s++) {
                for (int t = -b; t <= b; t++) {

                    // Boundary Check: Pastikan koordinat (x-s) dan (y-t) tidak keluar matriks
                    if ((x - s) >= 0 && (x - s) < barisCitra &&
                        (y - t) >= 0 && (y - t) < kolomCitra) {

                        // w(s,t): Indeks C++ tidak bisa negatif, kita geser +a dan +b agar mulai dari 0
                        float bobot = w[s + a][t + b];

                        // f(x-s, y-t): Akses nilai intensitas citra asli
                        float nilaiPiksel = f[x - s][y - t];

                        // Hitung sum of products
                        sum += bobot * nilaiPiksel;
                    }
                }
            }
            // Simpan hasil konvolusi ke dalam matriks g(x,y)
            g[x][y] = sum;
        }
    }
    return g;
}

/**
 * =========================================================================
 * FUNGSI TERAPAN: Implementasi Konvolusi menggunakan Tipe Data OpenCV
 * =========================================================================
 * Menggunakan cv::Mat agar langsung bisa membaca gambar sungguhan,
 * mengaplikasikan border padding, dan melakukan clipping angka 0-255.
 */
void konvolusiManual(const Mat& src, Mat& dst, const Mat& kernel) {
    // Pastikan citra input adalah grayscale (1 channel)
    CV_Assert(src.type() == CV_8UC1);
    // Pastikan kernel adalah tipe float untuk presisi perhitungan
    CV_Assert(kernel.type() == CV_32FC1);

    // Siapkan matriks tujuan dengan ukuran dan tipe yang sama dengan matriks asal
    dst = Mat::zeros(src.size(), src.type());

    // Hitung batas radius kernel (a dan b pada rumus matematika)
    int a = kernel.rows / 2;
    int b = kernel.cols / 2;

    // Tambahkan padding (batas) pada citra asal agar piksel di pinggir tetap bisa diproses
    Mat paddedSrc;
    copyMakeBorder(src, paddedSrc, a, a, b, b, BORDER_CONSTANT, Scalar(0));

    // Looping untuk setiap baris (y) dan kolom (x) pada citra
    for (int y = 0; y < src.rows; y++) {
        for (int x = 0; x < src.cols; x++) {

            float sum = 0.0f;

            // Looping bersarang untuk menggeser kernel (Translasi dari Tanda Sigma)
            for (int s = -a; s <= a; s++) {
                for (int t = -b; t <= b; t++) {

                    // w(s,t): Nilai elemen kernel
                    // Karena indeks matriks di C++ dimulai dari 0, kita geser (s+a) dan (t+b)
                    float w = kernel.at<float>(s + a, t + b);

                    // f(x-s, y-t): Nilai piksel dari citra asal (padded)
                    // PENTING: Pada konvolusi matematis sejati, piksel dicerminkan (dikurangi).
                    // Namun di sini kita memakai indeks positif terhadap padding
                    float f = paddedSrc.at<uchar>((y + a) - s, (x + b) - t);

                    // Sum of products
                    sum += w * f;
                }
            }

            // Batasi nilai agar tetap berada dalam rentang 0 - 255 (saturate_cast)
            dst.at<uchar>(y, x) = saturate_cast<uchar>(sum);
        }
    }
}

int main() {
    // 1. Baca citra dalam format Grayscale
    // Ganti "lena.jpg" dengan path citra yang tersedia di komputer Anda
    Mat img = imread("/Users/mc/CLionProjects/PengolahanCitraDigital/img1.jpg", IMREAD_GRAYSCALE);
    if (img.empty()) {
        cout << "Error: Gambar tidak ditemukan! Pastikan file lena.jpg ada di direktori yang sama." << endl;
        return -1;
    }

    // 2. Definisi Kernel (Contoh: Laplacian Sharpening Filter 3x3)
    Mat kernel = (Mat_<float>(3, 3) <<
         0, -1,  0,
        -1,  5, -1,
         0, -1,  0);

    Mat hasilManual, hasilOpenCV;

    // 3. Eksekusi dan ukur waktu untuk Konvolusi Manual (Versi OpenCV Mat)
    auto startManual = chrono::high_resolution_clock::now();

    konvolusiManual(img, hasilManual, kernel);

    auto endManual = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> durasiManual = endManual - startManual;

    // 4. Eksekusi dan ukur waktu untuk OpenCV Built-in (filter2D)
    auto startOpenCV = chrono::high_resolution_clock::now();

    // Perhatikan: filter2D di OpenCV secara default melakukan Korelasi.
    // Untuk menyamakan efek dengan konvolusi sejati, kernel asimetris perlu di-flip.
    // Tapi karena Laplacian simetris, hasilnya akan sama.
    filter2D(img, hasilOpenCV, -1, kernel);

    auto endOpenCV = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> durasiOpenCV = endOpenCV - startOpenCV;

    // 5. Tampilkan hasil komputasi ke konsol
    cout << "--- HASIL ANALISIS PERFORMA (OBE Lab) ---" << endl;
    cout << "Waktu Eksekusi Manual (Nested Loop): " << durasiManual.count() << " ms" << endl;
    cout << "Waktu Eksekusi OpenCV (filter2D)   : " << durasiOpenCV.count() << " ms" << endl;
    cout << "-----------------------------------------" << endl;

    // 6. Tampilkan citra ke layar
    imshow("1. Citra Asli", img);
    imshow("2. Hasil Konvolusi Manual", hasilManual);
    imshow("3. Hasil OpenCV filter2D", hasilOpenCV);

    // Tunggu input dari user untuk menutup jendela
    waitKey(0);
    destroyAllWindows();

    return 0;
}