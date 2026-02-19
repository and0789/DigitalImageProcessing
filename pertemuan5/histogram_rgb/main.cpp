#include <QApplication>
#include <QLabel>
#include <QPixmap>
#include <QImage>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QFrame>
#include <QString>

#include <opencv2/opencv.hpp>

#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <array>

// Konstanta Konfigurasi
namespace Config {
    constexpr int     HIST_BINS      = 256;
    constexpr int     CANVAS_W       = 420;
    constexpr int     CANVAS_H       = 300;
    constexpr int     GRAPH_H        = 200;
    constexpr int     PADDING_LEFT   = 45;
    constexpr int     PADDING_BOTTOM = 40;
    constexpr int     PREVIEW_WIDTH  = 360;
    const std::string IMAGE_PATH     = "/Users/mc/CLionProjects/PengolahanCitraDigital/image.jpg";
}

// Warna Tema (BGR untuk OpenCV)
namespace Theme {
    const cv::Scalar BACKGROUND   = cv::Scalar(28,  28,  33);
    const cv::Scalar GRID_LINE    = cv::Scalar(55,  55,  65);
    const cv::Scalar AXIS_LINE    = cv::Scalar(130, 130, 148);
    const cv::Scalar TEXT_DIM     = cv::Scalar(120, 120, 138);
    const cv::Scalar TEXT_ACCENT  = cv::Scalar(80,  210, 175);

    // Warna bar per channel (BGR), urutan tampilan: R, G, B
    const std::array<cv::Scalar, 3> CHANNEL_COLOR = {{
        cv::Scalar(60,  60,  220),  // R
        cv::Scalar(60,  200, 60),   // G
        cv::Scalar(220, 90,  60)    // B
    }};
    const std::array<cv::Scalar, 3> CHANNEL_BRIGHT = {{
        cv::Scalar(130, 130, 255),
        cv::Scalar(130, 240, 130),
        cv::Scalar(255, 160, 120)
    }};
    const std::array<std::string, 3> CHANNEL_LABEL = {"RED", "GREEN", "BLUE"};
}

// Helper: format angka desimal
std::string formatDouble(double value, int precision = 2) {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(precision) << value;
    return ss.str();
}

// Konversi cv::Mat ke QImage
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

// Gambar grid latar belakang
void drawGrid(cv::Mat& canvas, int graphH, int padLeft, int baseY, int graphW) {
    for (int i = 1; i <= 4; ++i) {
        int y = baseY - (graphH * i / 4);
        cv::line(canvas, {padLeft, y}, {padLeft + graphW, y},
                 Theme::GRID_LINE, 1, cv::LINE_AA);
    }
    for (int val : {64, 128, 192}) {
        int x = padLeft + val * graphW / 255;
        cv::line(canvas, {x, baseY - graphH}, {x, baseY},
                 Theme::GRID_LINE, 1, cv::LINE_AA);
    }
}

// Gambar satu panel histogram per channel
cv::Mat buatPanelHistogram(const cv::Mat& hist,
                            double meanVal, double stddevVal,
                            int channelIdx)
{
    const int canvasW   = Config::CANVAS_W;
    const int canvasH   = Config::CANVAS_H;
    const int graphH    = Config::GRAPH_H;
    const int padLeft   = Config::PADDING_LEFT;
    const int padBottom = Config::PADDING_BOTTOM;
    const int graphW    = canvasW - padLeft - 10;
    const int baseY     = canvasH - padBottom;

    cv::Mat canvas(canvasH, canvasW, CV_8UC3, Theme::BACKGROUND);
    drawGrid(canvas, graphH, padLeft, baseY, graphW);

    // Normalisasi & gambar bar
    cv::Mat histNorm;
    cv::normalize(hist, histNorm, 0, graphH, cv::NORM_MINMAX);

    const int          binW   = std::max(1, graphW / Config::HIST_BINS);
    const cv::Scalar&  color  = Theme::CHANNEL_COLOR[channelIdx];
    const cv::Scalar&  bright = Theme::CHANNEL_BRIGHT[channelIdx];

    for (int i = 0; i < Config::HIST_BINS; ++i) {
        const int barH = cvRound(histNorm.at<float>(i));
        const int x1   = padLeft + i * graphW / Config::HIST_BINS;
        const int x2   = x1 + binW;
        const int y1   = baseY - barH;

        if (barH <= 0) continue;
        cv::rectangle(canvas, {x1, y1}, {x2, baseY}, color, cv::FILLED);
        if (barH > 2)
            cv::rectangle(canvas, {x1, y1}, {x2, y1 + 2}, bright, cv::FILLED);
    }

    // Garis mean
    {
        int xMean = padLeft + static_cast<int>(meanVal * graphW / 255.0);
        cv::line(canvas, {xMean, baseY - graphH}, {xMean, baseY},
                 Theme::TEXT_ACCENT, 2, cv::LINE_AA);
        cv::putText(canvas, "u",
                    {xMean + 3, baseY - graphH + 12},
                    cv::FONT_HERSHEY_SIMPLEX, 0.38, Theme::TEXT_ACCENT, 1, cv::LINE_AA);
    }

    // Sumbu X dan Y
    cv::line(canvas, {padLeft, baseY - graphH}, {padLeft, baseY},
             Theme::AXIS_LINE, 2, cv::LINE_AA);
    cv::line(canvas, {padLeft, baseY}, {padLeft + graphW, baseY},
             Theme::AXIS_LINE, 2, cv::LINE_AA);

    // Label sumbu X
    for (int val : {0, 128, 255}) {
        int x = padLeft + val * graphW / 255;
        cv::putText(canvas, std::to_string(val),
                    {x - 8, baseY + 14},
                    cv::FONT_HERSHEY_SIMPLEX, 0.33, Theme::TEXT_DIM, 1, cv::LINE_AA);
        cv::line(canvas, {x, baseY}, {x, baseY + 4}, Theme::AXIS_LINE, 1);
    }

    // Judul channel
    cv::putText(canvas, Theme::CHANNEL_LABEL[channelIdx],
                {padLeft, 22},
                cv::FONT_HERSHEY_DUPLEX, 0.65, color, 1, cv::LINE_AA);

    // Statistik di bawah judul
    cv::putText(canvas, "u: " + formatDouble(meanVal) + "  d: " + formatDouble(stddevVal),
                {padLeft, 40},
                cv::FONT_HERSHEY_SIMPLEX, 0.36, Theme::TEXT_ACCENT, 1, cv::LINE_AA);

    // Label sumbu X tengah bawah
    cv::putText(canvas, "Intensity",
                {padLeft + graphW / 2 - 28, canvasH - 4},
                cv::FONT_HERSHEY_SIMPLEX, 0.34, Theme::TEXT_DIM, 1, cv::LINE_AA);

    return canvas;
}

// Struct hasil per channel
struct ChannelResult {
    cv::Mat panel;
    double  mean;
    double  stddev;
};

// Proses semua channel RGB dari citra BGR
std::array<ChannelResult, 3> prosesRGB(const cv::Mat& citraBGR) {
    // cv::split menghasilkan: channels[0]=B, channels[1]=G, channels[2]=R
    std::array<cv::Mat, 3> channels;
    cv::split(citraBGR, channels.data());

    // Tampilkan dalam urutan R(2), G(1), B(0)
    const std::array<int, 3> order = {2, 1, 0};

    const int    histSize  = Config::HIST_BINS;
    float        range[]   = {0, 256};
    const float* histRange = {range};

    std::array<ChannelResult, 3> results;
    for (int i = 0; i < 3; ++i) {
        const cv::Mat& ch = channels[order[i]];

        cv::Mat hist;
        cv::calcHist(&ch, 1, nullptr, cv::Mat(), hist, 1, &histSize, &histRange);

        cv::Mat meanMat, stddevMat;
        cv::meanStdDev(ch, meanMat, stddevMat);

        results[i] = {
            buatPanelHistogram(hist, meanMat.at<double>(0), stddevMat.at<double>(0), i),
            meanMat.at<double>(0),
            stddevMat.at<double>(0)
        };
    }
    return results;
}

// Helper: buat QLabel dari panel OpenCV
QLabel* buatLabelPanel(const cv::Mat& panel) {
    QLabel* label = new QLabel();
    label->setPixmap(QPixmap::fromImage(matToQImage(panel)));
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet(
        "background-color: #1c1c21;"
        "border-radius: 6px;"
        "padding: 4px;"
    );
    return label;
}

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // Baca citra dalam mode color (BGR)
    cv::Mat image = cv::imread(Config::IMAGE_PATH, cv::IMREAD_COLOR);
    if (image.empty()) {
        std::cerr << "[ERROR] Gambar tidak ditemukan: " << Config::IMAGE_PATH << "\n";
        return EXIT_FAILURE;
    }

    // Proses histogram RGB
    auto results = prosesRGB(image);

    // Konversi preview
    QImage qPreview = matToQImage(image);

    // Setup window utama
    QWidget mainWindow;
    mainWindow.setWindowTitle("RGB Histogram  -  Qt + OpenCV");
    mainWindow.setStyleSheet("background-color: #1a1a1f;");

    QVBoxLayout* rootLayout = new QVBoxLayout(&mainWindow);
    rootLayout->setContentsMargins(16, 16, 16, 16);
    rootLayout->setSpacing(10);

    // Judul aplikasi
    QLabel* titleLabel = new QLabel("Analisis Histogram Citra RGB");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(
        "color: #e0e0f0;"
        "font-size: 18px;"
        "font-weight: bold;"
        "font-family: 'Helvetica Neue', sans-serif;"
        "padding-bottom: 2px;"
    );
    rootLayout->addWidget(titleLabel);

    // Lambda: buat separator tipis
    auto makeSep = [&]() -> QFrame* {
        QFrame* sep = new QFrame();
        sep->setFrameShape(QFrame::HLine);
        sep->setStyleSheet("color: #3a3a4a;");
        return sep;
    };
    rootLayout->addWidget(makeSep());

    // Baris atas: preview kiri + info kanan
    QLabel* previewTitle = new QLabel("Citra Asli (Color)");
    previewTitle->setAlignment(Qt::AlignCenter);
    previewTitle->setStyleSheet("color: #80d4b4; font-size: 12px; font-weight: bold;");
    rootLayout->addWidget(previewTitle);

    QLabel* previewLabel = new QLabel();
    previewLabel->setPixmap(
        QPixmap::fromImage(qPreview).scaledToWidth(
            Config::PREVIEW_WIDTH, Qt::SmoothTransformation));
    previewLabel->setAlignment(Qt::AlignCenter);
    rootLayout->addWidget(previewLabel);

    QLabel* infoLabel = new QLabel(
        QString("Color (BGR)  |  %1 x %2 px  |  3 Channels")
            .arg(image.cols).arg(image.rows));
    infoLabel->setAlignment(Qt::AlignCenter);
    infoLabel->setStyleSheet(
        "color: #8888aa; font-size: 11px;"
        "font-family: 'Courier New', monospace; padding: 2px;");
    rootLayout->addWidget(infoLabel);

    rootLayout->addWidget(makeSep());

    // Label section histogram
    QLabel* histTitle = new QLabel("Histogram per Channel  ( R / G / B )");
    histTitle->setAlignment(Qt::AlignCenter);
    histTitle->setStyleSheet("color: #80d4b4; font-size: 12px; font-weight: bold;");
    rootLayout->addWidget(histTitle);

    // 3 panel histogram berdampingan secara horizontal
    QHBoxLayout* histRow = new QHBoxLayout();
    histRow->setSpacing(6);
    for (int i = 0; i < 3; ++i) {
        histRow->addWidget(buatLabelPanel(results[i].panel));
    }
    rootLayout->addLayout(histRow);

    rootLayout->addWidget(makeSep());

    // Tabel ringkasan statistik (3 kolom)
    const std::array<QString, 3> chNames  = {"Red",     "Green",   "Blue"};
    const std::array<QString, 3> chColors = {"#ff6666", "#66dd66", "#6699ff"};

    QHBoxLayout* statRow = new QHBoxLayout();
    statRow->setSpacing(8);

    for (int i = 0; i < 3; ++i) {
        QLabel* statLabel = new QLabel(
            QString("<b style='color:%1; font-size:13px;'>%2</b><br/>"
                    "<span style='color:#aaaacc'>Mean   : %3</span><br/>"
                    "<span style='color:#aaaacc'>StdDev : %4</span>")
                .arg(chColors[i])
                .arg(chNames[i])
                .arg(QString::fromStdString(formatDouble(results[i].mean)))
                .arg(QString::fromStdString(formatDouble(results[i].stddev)))
        );
        statLabel->setAlignment(Qt::AlignCenter);
        statLabel->setStyleSheet(
            "background-color: #22222a;"
            "border: 1px solid #3a3a50;"
            "border-radius: 6px;"
            "padding: 10px;"
            "font-size: 12px;"
            "font-family: 'Courier New', monospace;"
        );
        statRow->addWidget(statLabel);
    }
    rootLayout->addLayout(statRow);

    mainWindow.adjustSize();
    mainWindow.show();

    return app.exec();
}