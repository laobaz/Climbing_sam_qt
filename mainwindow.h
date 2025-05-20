
#pragma once
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QMediaPlayer>
#include <QVideoWidget>
//#include "current/rtdetr/rtdetr.h"
//#include "current/Sam/SAM.h"
#include "current/processors/image_processor.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    ImageProcessor *imageprocessor=nullptr;
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // 图片处理
    void on_selectImageButton_clicked();
    void on_prevImageButton_clicked();
    void on_nextImageButton_clicked();
    void on_processImageButton_clicked();

    // 视频处理
    // void on_selectVideoButton_clicked();
    // void on_startVideoProcessing_clicked();
    // void on_stopProcessButton_clicked();

private:
    Ui::MainWindow *ui;



    // 图片处理相关
    QGraphicsScene *originalImageScene;
    QGraphicsScene *processedImageScene;
    QString currentImageDir;
    int currentImageIndex;

    // 视频处理相关
   // QMediaPlayer *mediaPlayer;
   // QVideoWidget *videoWidget;

    // void releaseMediaPlayer();
    void displayCurrentImage();
    // void processCurrentImage();
     void logImageMessage(const QString &message);
    // void logVideoMessage(const QString &message);
};

