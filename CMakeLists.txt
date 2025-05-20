cmake_minimum_required(VERSION 3.16)

project(MultimediaDetectionSystem LANGUAGES CXX)

# ==================== 基础配置 ====================
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 推荐使用 RelWithDebInfo 代替 Debug 以获得更好的性能
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

# ==================== 依赖查找 ====================
# Qt6 配置 (必须放在 add_executable 之前)
find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Widgets
    Multimedia
    MultimediaWidgets
    Concurrent
    LinguistTools
)
qt_standard_project_setup()

# 其他依赖
find_package(Threads REQUIRED)
find_package(CUDA REQUIRED)
find_package(spdlog REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Boost 1.52 REQUIRED COMPONENTS system thread)

find_library(TBB_LIBRARY
    NAMES tbb
    PATHS "/usr/lib/x86_64-linux-gnu" "/usr/local/lib"
    REQUIRED
)

# ONNX Runtime 配置
set(ONNXRuntime_ROOT "/usr/local/onnxruntime")
set(ONNXRuntime_INCLUDE_DIRS "${ONNXRuntime_ROOT}/include")
set(ONNXRuntime_LIBRARIES "${ONNXRuntime_ROOT}/lib/libonnxruntime.so")

include_directories("/usr/local/onnxruntime/include")

# OpenMP 配置
find_package(OpenMP)
if(OpenMP_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

# ==================== 源文件设置 ====================
set(APP_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui

    # 其他源文件
    ./current/Sam/SAM.cpp
    ./current/rtdetr/rtdetr.cpp
    ./current/code/object_detector.cc
    ./current/code/preprocess_op.cc
    ./current/code/utils.cc
    ./current/processors/image_processor.cpp
)

# ==================== 可执行目标 ====================
qt_add_executable(${PROJECT_NAME}
    ${APP_SOURCES}
)

# ==================== 翻译文件 ====================
# 确保 TS 文件在源码目录中
set(TRANSLATION_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/translations/UI_2_zh_CN.ts
)
qt_add_translations(${PROJECT_NAME} TS_FILES ${TRANSLATION_FILES})

# ==================== 包含目录 ====================
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${Boost_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}

    /usr/local/cuda/include
    /usr/local/include
)

# ==================== 链接库 ====================
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Multimedia
    Qt6::MultimediaWidgets
    Qt6::Concurrent

    ${CUDA_CUDART_LIBRARY}
    ${OpenCV_LIBS}

    ${TBB_LIBRARY}
    spdlog::spdlog
    Boost::system
    Boost::thread
    Threads::Threads
    /usr/local/cuda/lib64/libnvinfer.so yaml-cpp
${ONNXRuntime_LIBRARIES}

)

# ==================== 编译器选项 ====================
target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall
    -Wextra
    -pedantic
)

# 输出信息
message(STATUS "CUDA_CUDART_LIBRARY: ${CUDA_CUDART_LIBRARY}")
message(STATUS "Project configured with Qt ${Qt6_VERSION}")
