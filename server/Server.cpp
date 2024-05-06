#include <QApplication>
#include <QTimer>
#include <QLabel>
#include <QUdpSocket>
#include <opencv2/opencv.hpp>

using namespace cv;

constexpr int port = 12345;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    std::unique_ptr<QLabel> p_debug_wnd;

    std::vector<std::string> args(argv, argv + argc);
    if (std::find(args.begin(), args.end(), "--debug") != args.end()) {
        p_debug_wnd = std::make_unique<QLabel>();
        p_debug_wnd->show();
    }

    QUdpSocket socket;
    if(!socket.bind(QHostAddress::AnyIPv4, port)){
        qDebug() << "Error: Unable to bind to port " << port;
        return -1;
    }

    qDebug() << "Broadcasting on port " << port;

    VideoCapture cap(0);
    if (!cap.isOpened()) {
        qDebug() << "Error: Unable to open the webcam";
        return -1;
    }

    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&]() {
        Mat frame;
        cap >> frame;
        QImage q_image(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
        QByteArray data;
        QDataStream stream(&data, QIODevice::WriteOnly);
        stream << q_image.scaled(50, 50);
        socket.writeDatagram(data, QHostAddress::Broadcast, port);
        if(p_debug_wnd) {
            p_debug_wnd->setPixmap(QPixmap::fromImage(q_image.rgbSwapped()));
            p_debug_wnd->setScaledContents(true);
            p_debug_wnd->adjustSize();
        }
    });

    timer.start(1000); // Update interval in milliseconds

    return app.exec();
}
