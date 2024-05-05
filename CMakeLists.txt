cmake_minimum_required(VERSION 3.5)

project(RaspberryQtVideoStreamer)

find_package(Qt6 REQUIRED COMPONENTS
    Core
    Network
    Widgets
    Multimedia
    MultimediaWidgets
)
find_package(OpenCV REQUIRED)

add_subdirectory(server)
add_subdirectory(client)

