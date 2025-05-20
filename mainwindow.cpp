#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QDateTime>
#include <QPainter>
#include <qvideoframe.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , originalImageScene(new QGraphicsScene(this))
    , processedImageScene(new QGraphicsScene(this))
   // , mediaPlayer(nullptr)
    //, videoWidget(nullptr)
    , currentImageIndex(0)
{
    ui->setupUi(this);

    // 初始化图形视图
    ui->originalImageView->setScene(originalImageScene);
    ui->processedImageView->setScene(processedImageScene);

    // 初始状态设置
    ui->prevImageButton->setEnabled(false);
    ui->nextImageButton->setEnabled(false);
    ui->processImageButton->setEnabled(false);
    //ui->stopVideoProcessButton->setEnabled(false);
}

MainWindow::~MainWindow()
{
    //releaseMediaPlayer();
    delete ui;
}

// 图片处理功能
void MainWindow::on_selectImageButton_clicked()
{
    QString dirPath = QFileDialog::getExistingDirectory(this, "选择图片文件夹", QDir::homePath());
    if(dirPath.isEmpty()) return;

    currentImageDir = dirPath;
    QDir dir(dirPath);
    QStringList filters;
    filters << "*.jpg" << "*.jpeg" << "*.png" << "*.bmp";
    auto imageFiles = dir.entryInfoList(filters, QDir::Files);

    if(imageFiles.isEmpty()) {
        logImageMessage("警告: 选择的文件夹中没有图片文件");
        QMessageBox::warning(this, "警告", "选择的文件夹中没有图片文件");
        return;
    }

    currentImageIndex = 0;
    ui->prevImageButton->setEnabled(imageFiles.size() > 1);
    ui->nextImageButton->setEnabled(imageFiles.size() > 1);
    ui->processImageButton->setEnabled(true);

    logImageMessage(QString("已加载图片文件夹: %1").arg(dirPath));
    displayCurrentImage();
}

void MainWindow::on_prevImageButton_clicked()
{
    QDir dir(currentImageDir);
    auto imageFiles = dir.entryInfoList(QStringList() << "*.jpg" << "*.jpeg" << "*.png" << "*.bmp", QDir::Files);
    if(imageFiles.isEmpty()) return;

    currentImageIndex = (currentImageIndex - 1 + imageFiles.size()) % imageFiles.size();
    logImageMessage(QString("显示上一张图片: %1").arg(imageFiles[currentImageIndex].fileName()));
    displayCurrentImage();
}

void MainWindow::on_nextImageButton_clicked()
{
    QDir dir(currentImageDir);
    auto imageFiles = dir.entryInfoList(QStringList() << "*.jpg" << "*.jpeg" << "*.png" << "*.bmp", QDir::Files);
    if(imageFiles.isEmpty()) return;

    currentImageIndex = (currentImageIndex + 1) % imageFiles.size();
    logImageMessage(QString("显示下一张图片: %1").arg(imageFiles[currentImageIndex].fileName()));
    displayCurrentImage();
}

void MainWindow::displayCurrentImage()
{
    QDir dir(currentImageDir);
    auto imageFiles = dir.entryInfoList(QStringList() << "*.jpg" << "*.jpeg" << "*.png" << "*.bmp", QDir::Files);
    if(imageFiles.isEmpty() || currentImageIndex >= imageFiles.size()) return;

    QString imagePath = imageFiles[currentImageIndex].absoluteFilePath();
    QPixmap originalPixmap(imagePath);

    if(originalPixmap.isNull()) {
        logImageMessage(QString("错误: 无法加载图片 %1").arg(imagePath));
        return;
    }

    originalImageScene->clear();
    originalImageScene->addPixmap(originalPixmap.scaled(ui->originalImageView->size(), Qt::KeepAspectRatio));
}

void MainWindow::on_processImageButton_clicked()
{
    if(currentImageDir.isEmpty()) return;

    QDir dir(currentImageDir);
    auto imageFiles = dir.entryInfoList(QStringList() << "*.jpg" << "*.jpeg" << "*.png" << "*.bmp", QDir::Files);
    if(imageFiles.isEmpty()) return;

    QString imagePath = imageFiles[currentImageIndex].absoluteFilePath();


    std::string outdir="./outimg";
    std::string outpath;
    std::string indir=!imagePath.isNull() ? imagePath.toStdString() : "";
    std::cout<<indir<<std::endl;
    bool predict_result= this->imageprocessor->processImage(imagePath.toStdString(),outdir,outpath);
    std::cout<<outpath<<std::endl;
    QString resultPath=QString::fromStdString(outpath);

    // 示例：直接显示原始路径（实际替换为API返回路径）
    QPixmap processed(resultPath.isEmpty() ? imagePath : resultPath);
    processedImageScene->clear();
    processedImageScene->addPixmap(processed.scaled(ui->processedImageView->size(), Qt::KeepAspectRatio));

    logImageMessage(QString("已处理图片: %1").arg(imageFiles[currentImageIndex].fileName()));
    if(predict_result)
    {
        logImageMessage(QString(" %1:图片存在登高行为").arg(imageFiles[currentImageIndex].fileName()));
    }
    else
         logImageMessage(QString(" %1:图片暂时未发现登高行为").arg(imageFiles[currentImageIndex].fileName()));
}

// 视频处理功能
// void MainWindow::on_selectVideoButton_clicked()
// {
//     QString videoPath = QFileDialog::getOpenFileName(this, "选择视频文件", QDir::homePath(),
//                                                      "视频文件 (*.mp4 *.avi *.mov)");
//     if(videoPath.isEmpty()) return;

//     releaseMediaPlayer();

//     mediaPlayer = new QMediaPlayer(this);
//     videoWidget = new QVideoWidget();

    // 清除原始视频视图布局
//     QLayoutItem* item;
//     while ((item = ui->originalVideoView->layout()->takeAt(0)) != nullptr) {
//         delete item->widget();
//         delete item;
//     }

//     // 设置视频输出
//     QVBoxLayout *layout = new QVBoxLayout(ui->originalVideoView);
//     layout->addWidget(videoWidget);
//     ui->originalVideoView->setLayout(layout);

//     mediaPlayer->setVideoOutput(videoWidget);
//     mediaPlayer->setSource(QUrl::fromLocalFile(videoPath));

//     // 连接状态改变信号，确保视频加载完成后播放
//     connect(mediaPlayer, &QMediaPlayer::mediaStatusChanged, this, [this](QMediaPlayer::MediaStatus status) {
//         if (status == QMediaPlayer::LoadedMedia) {
//             mediaPlayer->play();
//         }
//     });

//     // 直接尝试播放（如果不需要等待完全加载）
//     // mediaPlayer->play();

//     ui->startVideoProcessingButton->setEnabled(true);
//     ui->stopVideoProcessButton->setEnabled(true);
//     logVideoMessage(QString("已加载并播放视频文件: %1").arg(videoPath));
// }

// void MainWindow::on_startVideoProcessing_clicked()
// {
//     if(!mediaPlayer) {
//         logVideoMessage("错误: 没有可用的视频源");
//         return;
//     }

//     // 连接视频帧处理
//     // connect(mediaPlayer->videoSink(), &QVideoSink::videoFrameChanged, this, [this](const QVideoFrame &frame){
//     //     QImage image = frame.toImage();
//     //     if(!image.isNull()) {
//     //         // 示例处理：转换为灰度图
//     //         QImage processed = image.convertToFormat(QImage::Format_Grayscale8);

//     //         // 显示处理结果
//     //         ui->processedVideoView->scene()->clear();
//     //         ui->processedVideoView->scene()->addPixmap(
//     //             QPixmap::fromImage(processed).scaled(
//     //                 ui->processedVideoView->size(), Qt::KeepAspectRatio));
//     //     }
//     // });

//     mediaPlayer->play();
//     logVideoMessage("开始处理视频...");
// }

// void MainWindow::on_stopProcessButton_clicked()
// {
//     if(mediaPlayer) {
//         mediaPlayer->stop();
//         //disconnect(mediaPlayer->videoSink(), nullptr, this, nullptr);
//         logVideoMessage("视频处理已停止");
//     }

//     ui->stopVideoProcessButton->setEnabled(false);
//     ui->processedVideoView->scene()->clear();
// }

// void MainWindow::releaseMediaPlayer()
// {
//     if(mediaPlayer) {
//         mediaPlayer->stop();
//         delete mediaPlayer;
//         mediaPlayer = nullptr;
//     }

//     if(videoWidget) {
//         delete videoWidget;
//         videoWidget = nullptr;
//     }
// }

void MainWindow::logImageMessage(const QString &message)
{
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
    ui->imageLogTextEdit->append(QString("[%1] %2").arg(timestamp).arg(message));
}

// void MainWindow::logVideoMessage(const QString &message)
// {
//     QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
//     ui->videoLogTextEdit->append(QString("[%1] %2").arg(timestamp).arg(message));
// }
