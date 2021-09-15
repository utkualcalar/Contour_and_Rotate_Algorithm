//Yasar Utku Alcalar
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ffmpegdecoder_contour.h"
#include "login_contour.h"
#include <unistd.h>
#include <QPainter>
#include <QInputDialog>
#include <opencv2/opencv.hpp>
#include <QWidget>
#include <iostream>
#include <QElapsedTimer>
#include <QDebug>


using namespace std;
using namespace cv;

bool a = false;
int k, k_2;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Login l;
    l.exec();
    decoder = new FFmpegDecoder(l.getRtspURL().toStdString());
    decoder->connect();
    mode = l.getMode().toInt();

    if(decoder->isConnected()) {
        new std::thread(&FFmpegDecoder::decode, decoder);
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::paintEvent(QPaintEvent *)
{   cv::Mat src;
    static QElapsedTimer t;
    //t.start();
    button_color();

    if(decoder->isConnected())
    {
        decoder->mtx.lock();
        if(!decoder->decodedImgBuf.empty())
        {
            src = decoder->decodedImgBuf.front().clone();
            decoder->decodedImgBuf.pop_front();
        }
        decoder->mtx.unlock();

        if(!src.empty())
        {
            double angle;

            if (a == true){

                angle = 90 * k + 270 * k_2;

                // get the center coordinates of the image to create the 2D rotation matrix
                Point2f center((src.cols - 1) / 2.0, (src.rows - 1) / 2.0);
                // using getRotationMatrix2D() to get the rotation matrix
                Mat rotation_matix = getRotationMatrix2D(center, angle, 1.0);
                // we will save the resulting image in rotated_image matrix
                warpAffine(src, src, rotation_matix, src.size());
            }

            Mat img_gray;
            cvtColor(src, img_gray, COLOR_BGR2GRAY);
            blur( img_gray, img_gray, Size(3,3) );

            Mat thresh;
            Canny( img_gray, thresh, 100, 200 );

            vector<vector<Point>> contours;
            vector<Vec4i> hierarchy;
            findContours(thresh, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE);

            Mat contour_image = Mat::zeros( thresh.size(), CV_8UC3 );

            for( size_t i = 0; i< contours.size(); i++ )
            {
                //Scalar color = Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
                drawContours( contour_image, contours, (int)i, Scalar(0, 255, 0), 2, LINE_8, hierarchy, 0 );
            }

            Mat sharp_img;
            Mat kernel = (Mat_<double>(3,3) << 0, -1, 0, -1,  5, -1, 0, -1, 0);

            filter2D(src, sharp_img, -1 , kernel, Point(-1, -1), 0, BORDER_DEFAULT);

            Mat blurred;
            cv::blur(src, blurred, Size(5,5));

            uchar *data;

            if   (mode ==1)
            {
                data = contour_image.data;
            }

            else if (mode ==2)
            {
                data = blurred.data;
            }

            else if (mode ==3)
            {
                data = sharp_img.data;
            }

            else {
                src.release();
                update();
                qDebug() << "Invalid Number!\nPlease enter a valid number from the list";
                return;
            }


            QPainter painter(this);
            QImage image = QImage(data, src.cols, src.rows, QImage::Format_RGB888);
            QPixmap  pix =  QPixmap::fromImage(image);
            painter.drawPixmap(0, 0, ui->widget->width(), ui->widget->height(), pix);
            this->setWindowTitle("Contour Detection");
            //         qDebug() << t.elapsed();


            if (t.elapsed())
                painter.drawText(10, 10, QString("FPS: %1").arg(1000/t.restart()));
            update();
            usleep(15000);
        } else {
            update();
            usleep(10000);
        }
        src.release();
    } else {
        update();
        usleep(10000);
    }
}


void MainWindow::on_contour_clicked()
{
    mode = 1;
}

void MainWindow::on_blur_clicked()
{
    mode = 2;
}

void MainWindow::on_sharpen_clicked()
{
    mode = 3;
}

void MainWindow::on_rotate_left_clicked()
{
    k = k + 1;
    a = true;
}

void MainWindow::on_rotate_right_clicked()
{
    k_2 = k_2 + 1;
    a = true;
}

void MainWindow::button_color()
{
    if (mode == 1)
    {
        ui->contour->setStyleSheet("QPushButton{ background-color: yellow }");
        ui->blur->setStyleSheet("QPushButton{ background-color: white }");
        ui->sharpen->setStyleSheet("QPushButton{ background-color: white }");
    }

    if (mode == 2)
    {
        ui->contour->setStyleSheet("QPushButton{ background-color: white }");
        ui->blur->setStyleSheet("QPushButton{ background-color: yellow }");
        ui->sharpen->setStyleSheet("QPushButton{ background-color: white }");
    }

    if (mode == 3){
        ui->contour->setStyleSheet("QPushButton{ background-color: white }");
        ui->blur->setStyleSheet("QPushButton{ background-color: white }");
        ui->sharpen->setStyleSheet("QPushButton{ background-color: yellow }");}
}

