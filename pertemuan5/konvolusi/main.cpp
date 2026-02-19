#include <QApplication>
#include <QLabel>
#include <QPixmap>
#include <QImage>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QWidget>
#include <QFrame>
#include <QScrollArea>
#include <QString>

#include <opencv2/opencv.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>

// Konstanta Konfigurasi
namespace Config {
    constexpr int     PREVIEW_WIDTH  = 280;
    const std::string IMAGE_PATH     = "/Users/mc/CLionProjects/PengolahanCitraDigital/img1.jpg";
}

// =====================================================================
//  BAGIAN 1: KONVOLUSI MANUAL (implementasi dari slide algoritma)
//  Referensi: Slide 35 - Algoritma Konvolusi 3x3
// =====================================================================

// Konvolusi manual 2D tanpa padding (pixel pinggir diabaikan)
// Referensi: Slide 30 - Solusi 1: pixel pinggir tidak dikonvolusi
cv::Mat konvolusiManual(const cv::Mat& src, const std::vector<std::vector<float>>& mask) {
    cv::Mat dst = src.clone(); // pixel pinggir tetap sama seperti citra asal
    const int M = src.rows;
    const int N = src.cols;

    for (int i = 1; i <= M - 3; ++i) {
        for (int j = 1; j <= N - 3; ++j) {
            float hasil = 0.0f;
            float sumKernel = 0.0f;

            for (int ki = 0; ki < 3; ++ki)
                for (int kj = 0; kj < 3; ++kj) {
                    hasil    += src.at<uchar>(i - 1 + ki, j - 1 + kj) * mask[ki][kj];
                    sumKernel += mask[ki][kj];
                }

            // Jika jumlah kernel != 0, bagi dengan jumlahnya (Slide 16)
            if (std::abs(sumKernel) > 1e-6f)
                hasil /= sumKernel;

            // Clamp ke [0, 255]
            dst.at<uchar>(i, j) = static_cast<uchar>(std::clamp(hasil, 0.0f, 255.0f));
        }
    }
    return dst;
}

// Konvolusi manual 2D dengan zero padding
// Referensi: Slide 31 - Zero Padding
cv::Mat konvolusiDenganPadding(const cv::Mat& src, const std::vector<std::vector<float>>& mask) {
    const int M = src.rows;
    const int N = src.cols;

    // Tambahkan border 0 di sekeliling citra
    cv::Mat padded;
    cv::copyMakeBorder(src, padded, 1, 1, 1, 1, cv::BORDER_CONSTANT, 0);

    cv::Mat dst(M, N, CV_8UC1);

    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            float hasil    = 0.0f;
            float sumKernel = 0.0f;

            for (int ki = 0; ki < 3; ++ki)
                for (int kj = 0; kj < 3; ++kj) {
                    hasil    += padded.at<uchar>(i + ki, j + kj) * mask[ki][kj];
                    sumKernel += mask[ki][kj];
                }

            if (std::abs(sumKernel) > 1e-6f)
                hasil /= sumKernel;

            dst.at<uchar>(i, j) = static_cast<uchar>(std::clamp(hasil, 0.0f, 255.0f));
        }
    }
    return dst;
}

// =====================================================================
//  BAGIAN 2: DEFINISI KERNEL / MASK
//  Referensi: Slide 37-39 - Kernel untuk berbagai keperluan
// =====================================================================
namespace Kernel {
    // Smoothing / Blur - rata-rata 3x3 (jumlah = 9, hasil dibagi 9)
    const std::vector<std::vector<float>> BLUR = {{
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1}
    }};

    // Deteksi tepi (edge detection) - jumlah = 0 (Slide 20)
    const std::vector<std::vector<float>> EDGE = {{
        {-1, -1, -1},
        {-1,  8, -1},
        {-1, -1, -1}
    }};

    // Sharpening / Penajaman tepi
    const std::vector<std::vector<float>> SHARPEN = {{
        { 0, -1,  0},
        {-1,  5, -1},
        { 0, -1,  0}
    }};

    // Emboss
    const std::vector<std::vector<float>> EMBOSS = {{
        {-2, -1,  0},
        {-1,  1,  1},
        { 0,  1,  2}
    }};
}

// =====================================================================
//  BAGIAN 3: KONVOLUSI PADA CITRA RGB (per channel)
//  Referensi: Slide 34 - Konvolusi dilakukan pada setiap kanal R, G, B
// =====================================================================
cv::Mat konvolusiRGB(const cv::Mat& srcBGR, const std::vector<std::vector<float>>& mask) {
    std::array<cv::Mat, 3> channels;
    cv::split(srcBGR, channels.data());

    // Konvolusi tiap channel secara terpisah
    for (auto& ch : channels)
        ch = konvolusiDenganPadding(ch, mask);

    cv::Mat hasil;
    cv::merge(channels.data(), 3, hasil);
    return hasil;
}

// =====================================================================
//  UTILITAS: Konversi cv::Mat ke QImage
// =====================================================================
QImage matToQImage(const cv::Mat& mat) {
    if (mat.type() == CV_8UC3) {
        cv::Mat rgb;
        cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);
        return QImage(rgb.data, rgb.cols, rgb.rows,
                      static_cast<int>(rgb.step),
                      QImage::Format_RGB888).copy();
    }
    if (mat.type() == CV_8UC1) {
        return QImage(mat.data, mat.cols, mat.rows,
                      static_cast<int>(mat.step),
                      QImage::Format_Grayscale8).copy();
    }
    return {};
}

// Helper: buat label gambar dengan judul
QWidget* buatKartu(const cv::Mat& img, const QString& judul, const QString& subjudul = "") {
    QWidget* card = new QWidget();
    card->setStyleSheet(
        "background-color: #22222a;"
        "border-radius: 8px;"
        "padding: 6px;"
    );

    QVBoxLayout* layout = new QVBoxLayout(card);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setSpacing(4);

    QLabel* imgLabel = new QLabel();
    imgLabel->setPixmap(
        QPixmap::fromImage(matToQImage(img)).scaledToWidth(
            Config::PREVIEW_WIDTH, Qt::SmoothTransformation));
    imgLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(imgLabel);

    QLabel* titleLabel = new QLabel(judul);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("color: #e0e0f0; font-size: 12px; font-weight: bold; background: transparent;");
    layout->addWidget(titleLabel);

    if (!subjudul.isEmpty()) {
        QLabel* subLabel = new QLabel(subjudul);
        subLabel->setAlignment(Qt::AlignCenter);
        subLabel->setStyleSheet("color: #8888aa; font-size: 10px; background: transparent;");
        layout->addWidget(subLabel);
    }

    return card;
}

// Helper: buat separator
QFrame* makeSep() {
    QFrame* sep = new QFrame();
    sep->setFrameShape(QFrame::HLine);
    sep->setStyleSheet("color: #3a3a4a;");
    return sep;
}

// Helper: buat label section
QLabel* makeSectionLabel(const QString& text) {
    QLabel* label = new QLabel(text);
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("color: #80d4b4; font-size: 12px; font-weight: bold;");
    return label;
}

// =====================================================================
//  MAIN
// =====================================================================
int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // Baca citra
    cv::Mat imageBGR = cv::imread(Config::IMAGE_PATH, cv::IMREAD_COLOR);
    if (imageBGR.empty()) {
        std::cerr << "[ERROR] Gambar tidak ditemukan: " << Config::IMAGE_PATH << "\n";
        return EXIT_FAILURE;
    }

    // Konversi ke grayscale untuk operasi konvolusi 1 channel
    cv::Mat imageGray;
    cv::cvtColor(imageBGR, imageGray, cv::COLOR_BGR2GRAY);

    // ── Eksperimen 1: Berbagai Kernel pada Grayscale ──────────────
    cv::Mat hasilBlur    = konvolusiManual(imageGray,   Kernel::BLUR);
    cv::Mat hasilEdge    = konvolusiManual(imageGray,   Kernel::EDGE);
    cv::Mat hasilSharpen = konvolusiManual(imageGray,   Kernel::SHARPEN);
    cv::Mat hasilEmboss  = konvolusiManual(imageGray,   Kernel::EMBOSS);

    // ── Eksperimen 2: Konvolusi Grayscale dengan & tanpa Padding ──
    cv::Mat hasilTanpaPad = konvolusiManual(imageGray,       Kernel::BLUR);
    cv::Mat hasilDenganPad = konvolusiDenganPadding(imageGray, Kernel::BLUR);

    // ── Eksperimen 3: Konvolusi pada Citra RGB ────────────────────
    cv::Mat hasilRGBBlur  = konvolusiRGB(imageBGR, Kernel::BLUR);
    cv::Mat hasilRGBEdge  = konvolusiRGB(imageBGR, Kernel::EDGE);

    // ── Bangun GUI ────────────────────────────────────────────────
    QWidget* mainWindow = new QWidget();
    mainWindow->setWindowTitle("Implementasi Konvolusi Citra  -  Qt + OpenCV");
    mainWindow->setStyleSheet("background-color: #1a1a1f;");

    QVBoxLayout* rootLayout = new QVBoxLayout(mainWindow);
    rootLayout->setContentsMargins(16, 16, 16, 16);
    rootLayout->setSpacing(10);

    // Judul
    QLabel* titleLabel = new QLabel("Implementasi Konvolusi Citra Digital");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(
        "color: #e0e0f0; font-size: 18px; font-weight: bold;"
        "font-family: 'Helvetica Neue', sans-serif; padding-bottom: 2px;");
    rootLayout->addWidget(titleLabel);

    QLabel* subTitle = new QLabel("Slide 07 - Penapisan Citra dan Konvolusi");
    subTitle->setAlignment(Qt::AlignCenter);
    subTitle->setStyleSheet("color: #8888aa; font-size: 11px;");
    rootLayout->addWidget(subTitle);

    rootLayout->addWidget(makeSep());

    // Section 1: Berbagai Kernel pada Grayscale
    rootLayout->addWidget(makeSectionLabel(
        "Eksperimen 1: Berbagai Kernel pada Citra Grayscale (tanpa padding, Slide 30)"));

    QHBoxLayout* row1 = new QHBoxLayout();
    row1->setSpacing(8);
    row1->addWidget(buatKartu(imageGray,    "Citra Asli (Grayscale)", "Input f(x,y)"));
    row1->addWidget(buatKartu(hasilBlur,    "Smoothing / Blur",       "Kernel rata-rata 3x3"));
    row1->addWidget(buatKartu(hasilEdge,    "Deteksi Tepi",           "Kernel edge 3x3 (Slide 38)"));
    row1->addWidget(buatKartu(hasilSharpen, "Sharpening",             "Kernel penajaman tepi"));
    row1->addWidget(buatKartu(hasilEmboss,  "Emboss",                 "Kernel emboss 3x3"));
    rootLayout->addLayout(row1);

    rootLayout->addWidget(makeSep());

    // Section 2: Perbandingan Padding
    rootLayout->addWidget(makeSectionLabel(
        "Eksperimen 2: Tanpa Padding vs Zero Padding (Slide 30 & 31)"));

    QHBoxLayout* row2 = new QHBoxLayout();
    row2->setSpacing(8);
    row2->addWidget(buatKartu(imageGray,       "Citra Asli",          "Input f(x,y)"));
    row2->addWidget(buatKartu(hasilTanpaPad,   "Tanpa Padding",       "Pixel pinggir tidak berubah"));
    row2->addWidget(buatKartu(hasilDenganPad,  "Dengan Zero Padding", "Pixel border diasumsikan = 0"));
    rootLayout->addLayout(row2);

    rootLayout->addWidget(makeSep());

    // Section 3: Konvolusi pada Citra RGB
    rootLayout->addWidget(makeSectionLabel(
        "Eksperimen 3: Konvolusi pada Citra RGB - per channel R, G, B (Slide 34)"));

    QHBoxLayout* row3 = new QHBoxLayout();
    row3->setSpacing(8);
    row3->addWidget(buatKartu(imageBGR,      "Citra Asli (Color)",   "Input f(x,y) - BGR"));
    row3->addWidget(buatKartu(hasilRGBBlur,  "RGB Blur",             "Konvolusi tiap channel R, G, B"));
    row3->addWidget(buatKartu(hasilRGBEdge,  "RGB Edge Detection",   "Deteksi tepi pada tiap channel"));
    rootLayout->addLayout(row3);

    rootLayout->addWidget(makeSep());

    // Info keterangan kernel
    QLabel* infoLabel = new QLabel(
        "Kernel yang digunakan:  "
        "Blur [ 1 1 1 / 1 1 1 / 1 1 1 ] / 9   |   "
        "Edge [ -1 -1 -1 / -1 8 -1 / -1 -1 -1 ]   |   "
        "Sharpen [ 0 -1 0 / -1 5 -1 / 0 -1 0 ]   |   "
        "Emboss [ -2 -1 0 / -1 1 1 / 0 1 2 ]"
    );
    infoLabel->setAlignment(Qt::AlignCenter);
    infoLabel->setWordWrap(true);
    infoLabel->setStyleSheet(
        "color: #8888aa; font-size: 10px;"
        "font-family: 'Courier New', monospace; padding: 4px;");
    rootLayout->addWidget(infoLabel);

    mainWindow->adjustSize();
    mainWindow->show();

    return app.exec();
}