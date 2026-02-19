#include <QApplication>
#include <QLabel>
#include <QPixmap>
#include <QImage>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QFrame>
#include <QFont>
#include <QString>

#include <opencv2/opencv.hpp>

#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>


//  Konstanta Konfigurasi

namespace Config {
    constexpr int    HIST_BINS      = 256;
    constexpr int    CANVAS_W       = 600;
    constexpr int    CANVAS_H       = 480;
    constexpr int    GRAPH_H        = 360;
    constexpr int    PADDING_LEFT   = 50;
    constexpr int    PADDING_BOTTOM = 40;
    constexpr int    PREVIEW_WIDTH  = 300;
    const std::string IMAGE_PATH    = "/Users/mc/CLionProjects/PengolahanCitraDigital/img1.jpg";
}

//  Warna Tema (BGR untuk OpenCV)
namespace Theme {
    const cv::Scalar BACKGROUND   = cv::Scalar(30,  30,  35);   // Dark charcoal
    const cv::Scalar GRID_LINE    = cv::Scalar(60,  60,  70);   // Subtle grid
    const cv::Scalar AXIS_LINE    = cv::Scalar(140, 140, 155);  // Soft gray axis
    const cv::Scalar BAR_COLOR    = cv::Scalar(80,  180, 255);  // Sky blue bar
    const cv::Scalar BAR_BRIGHT   = cv::Scalar(120, 210, 255);  // Highlight bar
    const cv::Scalar TEXT_PRIMARY = cv::Scalar(230, 230, 240);  // Near white
    const cv::Scalar TEXT_ACCENT  = cv::Scalar(80,  210, 180);  // Teal accent
    const cv::Scalar TEXT_DIM     = cv::Scalar(130, 130, 145);  // Muted
    const cv::Scalar MEAN_LINE    = cv::Scalar(60,  200, 100);  // Green mean
    const cv::Scalar STDDEV_LINE  = cv::Scalar(60,  130, 255);  // Orange stddev
}


//  Konversi cv::Mat → QImage
QImage matToQImage(const cv::Mat& mat) {
    if (mat.type() == CV_8UC3) {
        cv::Mat rgbMat;
        cv::cvtColor(mat, rgbMat, cv::COLOR_BGR2RGB);
        return QImage(rgbMat.data, rgbMat.cols, rgbMat.rows,
                      static_cast<int>(rgbMat.step),
                      QImage::Format_RGB888).copy();
    }
    if (mat.type() == CV_8UC1) {
        return QImage(mat.data, mat.cols, mat.rows,
                      static_cast<int>(mat.step),
                      QImage::Format_Grayscale8).copy();
    }
    return {};
}

//  Helper: Format angka desimal
std::string formatDouble(double value, int precision = 2) {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(precision) << value;
    return ss.str();
}


//  Gambar Grid Latar Belakang
void drawGrid(cv::Mat& canvas, int graphH, int padLeft, int padBottom) {
    const int graphW = Config::CANVAS_W - padLeft - 10;
    const int baseY  = Config::CANVAS_H - padBottom;

    // Garis horizontal grid (5 garis)
    for (int i = 1; i <= 5; ++i) {
        int y = baseY - (graphH * i / 5);
        cv::line(canvas, {padLeft, y}, {padLeft + graphW, y},
                 Theme::GRID_LINE, 1, cv::LINE_AA);

        // Label nilai Y
        int yVal = 100 * i / 5;
        cv::putText(canvas, std::to_string(yVal) + "%",
                    {5, y + 5}, cv::FONT_HERSHEY_SIMPLEX,
                    0.35, Theme::TEXT_DIM, 1, cv::LINE_AA);
    }

    // Garis vertikal grid (4 garis: 64, 128, 192)
    for (int i = 1; i <= 3; ++i) {
        int x = padLeft + (graphW * i * 64 / 255);
        cv::line(canvas, {x, baseY - graphH}, {x, baseY},
                 Theme::GRID_LINE, 1, cv::LINE_AA);
    }
}


//  Gambar Histogram Profesional
cv::Mat buatGambarHistogram(const cv::Mat& citra) {
    // Hitung histogram
    const int   histSize  = Config::HIST_BINS;
    float       range[]   = {0, 256};
    const float* histRange = {range};
    cv::Mat hist;
    cv::calcHist(&citra, 1, nullptr, cv::Mat(), hist, 1, &histSize, &histRange);

    // Statistik
    cv::Mat mean, stddev;
    cv::meanStdDev(citra, mean, stddev);
    const double meanVal   = mean.at<double>(0);
    const double stddevVal = stddev.at<double>(0);

    // Layout
    const int canvasW   = Config::CANVAS_W;
    const int canvasH   = Config::CANVAS_H;
    const int graphH    = Config::GRAPH_H;
    const int padLeft   = Config::PADDING_LEFT;
    const int padBottom = Config::PADDING_BOTTOM;
    const int graphW    = canvasW - padLeft - 10;
    const int baseY     = canvasH - padBottom;

    // Kanvas
    cv::Mat canvas(canvasH, canvasW, CV_8UC3, Theme::BACKGROUND);

    // Grid
    drawGrid(canvas, graphH, padLeft, padBottom);

    // Normalisasi & Gambar Bar
    cv::Mat histNorm;
    cv::normalize(hist, histNorm, 0, graphH, cv::NORM_MINMAX);

    const int binW = std::max(1, graphW / histSize);

    for (int i = 0; i < histSize; ++i) {
        const int barH = cvRound(histNorm.at<float>(i));
        const int x1   = padLeft + i * graphW / histSize;
        const int x2   = x1 + binW;
        const int y1   = baseY - barH;
        const int y2   = baseY;

        if (barH <= 0) continue;

        // Bar utama
        cv::rectangle(canvas, {x1, y1}, {x2, y2}, Theme::BAR_COLOR, cv::FILLED);

        // Highlight tipis di atas bar
        if (barH > 2)
            cv::rectangle(canvas, {x1, y1}, {x2, y1 + 2}, Theme::BAR_BRIGHT, cv::FILLED);
    }

    // Garis Mean
    {
        int xMean = padLeft + static_cast<int>(meanVal * graphW / 255.0);
        cv::line(canvas, {xMean, baseY - graphH}, {xMean, baseY},
                 Theme::MEAN_LINE, 2, cv::LINE_AA);
        cv::putText(canvas, "u", {xMean + 4, baseY - graphH + 14},
                    cv::FONT_HERSHEY_SIMPLEX, 0.45, Theme::MEAN_LINE, 1, cv::LINE_AA);
    }

    // Garis StdDev (mean ± stddev)
    for (int sign : {-1, 1}) {
        int xStd = padLeft + static_cast<int>((meanVal + sign * stddevVal) * graphW / 255.0);
        xStd     = std::clamp(xStd, padLeft, padLeft + graphW);
        cv::line(canvas, {xStd, baseY - graphH / 2}, {xStd, baseY},
                 Theme::STDDEV_LINE, 1, cv::LINE_AA);
    }

    // Sumbu X & Y
    cv::line(canvas, {padLeft, baseY - graphH}, {padLeft, baseY},
             Theme::AXIS_LINE, 2, cv::LINE_AA);
    cv::line(canvas, {padLeft, baseY}, {padLeft + graphW, baseY},
             Theme::AXIS_LINE, 2, cv::LINE_AA);

    // Label Sumbu X
    for (int val : {0, 64, 128, 192, 255}) {
        int x = padLeft + val * graphW / 255;
        cv::putText(canvas, std::to_string(val),
                    {x - 8, baseY + 18}, cv::FONT_HERSHEY_SIMPLEX,
                    0.38, Theme::TEXT_DIM, 1, cv::LINE_AA);
        cv::line(canvas, {x, baseY}, {x, baseY + 4},
                 Theme::AXIS_LINE, 1, cv::LINE_AA);
    }

    // Judul
    cv::putText(canvas, "INTENSITY HISTOGRAM",
                {padLeft, 28}, cv::FONT_HERSHEY_DUPLEX,
                0.75, Theme::TEXT_PRIMARY, 1, cv::LINE_AA);

    // Panel Statistik (pojok kanan atas)
    const int statX = canvasW - 195;
    const int statY = 18;
    cv::rectangle(canvas,
                  {statX - 8, statY - 14},
                  {canvasW - 5, statY + 52},
                  cv::Scalar(50, 50, 58), cv::FILLED);
    cv::rectangle(canvas,
                  {statX - 8, statY - 14},
                  {canvasW - 5, statY + 52},
                  cv::Scalar(80, 80, 95), 1);

    cv::putText(canvas, "Mean (u)  : " + formatDouble(meanVal),
                {statX, statY + 4}, cv::FONT_HERSHEY_SIMPLEX,
                0.45, Theme::TEXT_ACCENT, 1, cv::LINE_AA);
    cv::putText(canvas, "StdDev (d): " + formatDouble(stddevVal),
                {statX, statY + 24}, cv::FONT_HERSHEY_SIMPLEX,
                0.45, Theme::TEXT_ACCENT, 1, cv::LINE_AA);
    cv::putText(canvas, "Pixels    : " + std::to_string(citra.rows * citra.cols),
                {statX, statY + 44}, cv::FONT_HERSHEY_SIMPLEX,
                0.40, Theme::TEXT_DIM, 1, cv::LINE_AA);

    // Label Sumbu
    cv::putText(canvas, "Intensity (0-255)",
                {padLeft + graphW / 2 - 60, canvasH - 4},
                cv::FONT_HERSHEY_SIMPLEX, 0.42, Theme::TEXT_DIM, 1, cv::LINE_AA);

    return canvas;
}

//  Buat Label Info (dimensi citra)
QLabel* buatLabelInfo(const cv::Mat& citra) {
    QString info = QString("Grayscale  |  %1 × %2 px  |  1 Channel")
                       .arg(citra.cols)
                       .arg(citra.rows);
    QLabel* label = new QLabel(info);
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet(
        "color: #8888aa;"
        "font-size: 11px;"
        "font-family: 'Courier New', monospace;"
        "padding: 4px;"
    );
    return label;
}


//  Main
int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // Baca Citra
    cv::Mat image = cv::imread(Config::IMAGE_PATH, cv::IMREAD_GRAYSCALE);
    if (image.empty()) {
        std::cerr << "[ERROR] Gambar tidak ditemukan: " << Config::IMAGE_PATH << "\n";
        return EXIT_FAILURE;
    }

    // Proses
    cv::Mat histMat       = buatGambarHistogram(image);
    QImage  qImageAsli    = matToQImage(image);
    QImage  qImageHist    = matToQImage(histMat);

    // Bangun GUI
    QWidget mainWindow;
    mainWindow.setWindowTitle("Histogram Citra  —  Qt + OpenCV");
    mainWindow.setStyleSheet("background-color: #1e1e23;");

    QVBoxLayout* rootLayout = new QVBoxLayout(&mainWindow);
    rootLayout->setContentsMargins(16, 16, 16, 16);
    rootLayout->setSpacing(12);

    // Judul Aplikasi
    QLabel* titleLabel = new QLabel("Analisis Histogram Citra Digital");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(
        "color: #e0e0f0;"
        "font-size: 18px;"
        "font-weight: bold;"
        "font-family: 'Helvetica Neue', sans-serif;"
        "padding-bottom: 4px;"
    );
    rootLayout->addWidget(titleLabel);

    // Separator
    QFrame* sep = new QFrame();
    sep->setFrameShape(QFrame::HLine);
    sep->setStyleSheet("color: #3a3a4a;");
    rootLayout->addWidget(sep);

    // Preview Citra Asli
    QLabel* labelAsliTitle = new QLabel("Citra Asli (Grayscale)");
    labelAsliTitle->setAlignment(Qt::AlignCenter);
    labelAsliTitle->setStyleSheet("color: #80d4b4; font-size: 12px; font-weight: bold;");
    rootLayout->addWidget(labelAsliTitle);

    QLabel* labelAsli = new QLabel();
    labelAsli->setPixmap(
        QPixmap::fromImage(qImageAsli).scaledToWidth(
            Config::PREVIEW_WIDTH, Qt::SmoothTransformation));
    labelAsli->setAlignment(Qt::AlignCenter);
    rootLayout->addWidget(labelAsli);

    rootLayout->addWidget(buatLabelInfo(image));

    // Separator
    QFrame* sep2 = new QFrame();
    sep2->setFrameShape(QFrame::HLine);
    sep2->setStyleSheet("color: #3a3a4a;");
    rootLayout->addWidget(sep2);

    // Histogram
    QLabel* labelHistTitle = new QLabel("Grafik Histogram Intensitas");
    labelHistTitle->setAlignment(Qt::AlignCenter);
    labelHistTitle->setStyleSheet("color: #80d4b4; font-size: 12px; font-weight: bold;");
    rootLayout->addWidget(labelHistTitle);

    QLabel* labelHist = new QLabel();
    labelHist->setPixmap(QPixmap::fromImage(qImageHist));
    labelHist->setAlignment(Qt::AlignCenter);
    rootLayout->addWidget(labelHist);

    // Tampilkan Window
    mainWindow.adjustSize();
    mainWindow.show();

    return app.exec();
}