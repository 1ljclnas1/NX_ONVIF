cmake_minimum_required(VERSION 3.22)
project(multi_pipeline)

set(CMAKE_CXX_STANDARD 11)

find_package(PkgConfig)

pkg_check_modules(GST REQUIRED gstreamer-1.0>=1.4)
pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0)
pkg_check_modules(GSTREAMER-APP REQUIRED gstreamer-app-1.0)
pkg_check_modules(GSTREAMER-AUDIO REQUIRED gstreamer-audio-1.0)
pkg_check_modules(GSTREAMER-PBUTILS REQUIRED gstreamer-pbutils-1.0)
pkg_check_modules(GSTREAMER-FFT REQUIRED gstreamer-fft-1.0)
pkg_check_modules(GSTREAMER-RTSP-SERVER REQUIRED gstreamer-rtsp-server-1.0)
pkg_check_modules(OPENCV4 REQUIRED opencv4)
pkg_check_modules(GSTREAMER-VIDEO REQUIRED gstreamer-video-1.0)
pkg_check_modules(GSTREAMER-RTSP REQUIRED gstreamer-rtsp-1.0)
pkg_check_modules(YAML-CPP REQUIRED yaml-cpp)



include_directories(
        ${GLIB_INCLUDE_DIRS}
        ${GSTREAMER_INCLUDE_DIRS}
        ${GSTREAMER-APP_INCLUDE_DIRS}
        ${GSTREAMER-AUDIO_INCLUDE_DIRS}
        ${GSTREAMER-PBUTILS_INCLUDE_DIRS}
        ${GSTREAMER-FFT_INCLUDE_DIRS}
        ${GSTREAMER-RTSP-SERVER_INCLUDE_DIRS}
        ${OPENCV4_INCLUDE_DIRS}
        ${GSTREAMER-VIDEO_INCLUDE_DIRS}
        ${GSTREAMER-RTSP_INCLUDE_DIRS}
        ${YAML-CPP_INCLUDE_DIRS}
)

link_directories(
        ${GLIB_LIBRARY_DIRS}
        ${GSTREAMER_LIBRARY_DIRS}
        ${GSTREAMER-APP_LIBRARY_DIRS}
        ${GSTREAMER-AUDIO_LIBRARY_DIRS}
        ${GSTREAMER-PBUTILS_LIBRARY_DIRS}
        ${GSTREAMER-FFT_LIBRARY_DIRS}
        ${GSTREAMER-RTSP-SERVER_LIBRARY_DIRS}
        ${GSTREAMER-RTSP_LIBRARY_DIRS}
        ${OPENCV4_LIBRARY_DIRS}
        ${GSTREAMER-VIDEO_LIBRARY_DIRS}
        ${YAML-CPP_LIBRARY_DIRS}
)

add_definitions(-DPLATFORM_TEGRA)
aux_source_directory(src SRC)



include_directories(
        /usr/local/cuda-10.2/include
        /opt/nvidia/deepstream/deepstream-6.0/sources/includes/
        /usr/include/gstreamer-1.0/
        /usr/include/glib-2.0
        /usr/lib/aarch64-linux-gnu/glib-2.0/include
        /opt/nvidia/deepstream/deepstream-6.0/sources/apps/apps-common/includes/
        ./common/includes/
)
link_directories("/opt/nvidia/deepstream/deepstream-6.0/lib/")
link_directories("/usr/include/glib-2.0/")
link_directories("/usr/lib/aarch64-linux-gnu/glib-2.0/include/")
link_directories("/usr/local/cuda-10.2/lib64/")
link_directories("/opt/nvidia/deepstream/deepstream-6.0/sources/apps/apps-common/includes/")
link_directories(${GSTREAMER-RTSP-SERVER_LIBRARIES})

aux_source_directory(onvifDs/cpp_files ONVIFDS_CPP_SRCS)
aux_source_directory(onvifDs/c_files ONVIFDS_C_SRCS)

add_library(onvifds SHARED
        ${ONVIFDS_C_SRCS}
        ${ONVIFDS_CPP_SRCS}
        )

target_link_libraries(onvifds PRIVATE ssl crypto)

add_executable(multi_pipeline
        ${SRC}
        )


target_include_directories(multi_pipeline PRIVATE onvifDs/h_files)

target_link_libraries(multi_pipeline
        pthread
        gobject-2.0
        glib-2.0
        cudart
        nvdsgst_helper
        m
        nvds_meta
        nvdsgst_meta
        nvds_inferutils
        nvds_utils
        dl
        gstrtspserver-1.0
        cuda
        ${GSTREAMER-RTSP-SERVER_LIBRARIES}
        ${GLIB_LIBRARIES}
        ${GSTREAMER_LIBRARIES}
        ${GSTREAMER-APP_LIBRARIES}
        ${GSTREAMER-AUDIO_LIBRARIES}
        ${GSTREAMER-PBUTILS_LIBRARIES}
        ${GSTREAMER-FFT_LIBRARIES}
        ${YAML-CPP_LIBRARIES}
        onvifds
        )