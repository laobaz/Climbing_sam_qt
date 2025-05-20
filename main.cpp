#pragma once
#include "mainwindow.h"
#include <QApplication>
#include <chrono>
#include <iomanip>
#include <math.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>
#include <utility>
#include <stdarg.h>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <sys/stat.h>

#include "current/code/object_detector.h"

#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
//#include "current/rtdetr/rtdetr.h"
//#include "current/Sam/SAM.h"
int main(int argc, char *argv[])
{
    std::cout << "staring" << std::endl;
    const char *engine_file_path_in = "/home/laoba/final_ks/UI_2/current/trt_model_xz/rtdetr_r50vd_6x_coco_xz_new.trt";
    // const char* engine_file_path_in="./current/trt_model_helmet/rtdetr_r50vd_6x_coco.trt";
    const char *config_file_path_in = "/home/laoba/final_ks/UI_2/current/trt_model_xz/infer_cfg.yml";

    //const char *output_dir = "./";
    std::vector<std::string> onnx_paths{
                                        "/home/laoba/final_ks/UI_2/current/trt_model_xz/sam/ESAM_encoder.onnx",
                                        "/home/laoba/final_ks/UI_2/current/trt_model_xz/sam/ESAM_decoder.onnx"};

    // 建立日志系统
    //  创建文件日志器（自动线程安全）
    auto logger = spdlog::basic_logger_mt("RT_SAM", "logs/app.log", true);
    //auto logger = spdlog::stdout_color_mt("console");
    set_default_logger(logger);
    set_level(spdlog::level::debug);
    spdlog::info("初始化完日志器");

    // spdlog::debug("This is a debug message");
    // spdlog::info("Welcome to spdlog!");
    // spdlog::error("Some error occurred: {}", 42);

    //std::string video_or_filePath = argv[1];
    //std::string outimgsPath = argv[2];
    //std::string models = argv[3];

    //cv::VideoCapture cap(video_or_filePath);

    Rtdetr Rt;
    std::cout<<"tip"<<std::endl;
    Rt.initialize(config_file_path_in, engine_file_path_in);
    SAM Sam;
    Sam.initialize(onnx_paths, true);
    //std::cout<<"Sam ok"<<std::endl;
    QApplication a(argc, argv);

    // 设置应用程序样式
    QApplication::setStyle("Fusion");

    // 创建并显示主窗口
    MainWindow w;
    w.show();
    w.imageprocessor=new ImageProcessor(Rt,Sam);

    return a.exec();
}
