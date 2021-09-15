#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ffmpegdecoder_contour.h"
#include "opencv2/objdetect.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
using namespace cv;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void paintEvent(QPaintEvent *);
    void button_color();

private slots:
    void on_contour_clicked();

    void on_blur_clicked();

    void on_sharpen_clicked();

    void on_rotate_left_clicked();

    void on_rotate_right_clicked();

private:
    Ui::MainWindow *ui;
    FFmpegDecoder *decoder;
    int mode;
};
#endif // MAINWINDOW_H
