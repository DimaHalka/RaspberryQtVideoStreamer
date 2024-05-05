#include <QApplication>
#include <QImage>
#include <QLabel>
#include <QUdpSocket>

constexpr int port = 12345;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    QUdpSocket socket;
    if(!socket.bind(QHostAddress::AnyIPv4, port)){
        qDebug() << "Error: Unable to bind to port " << port;
        return -1;
    }
    
    qDebug() << "Bound to port " << port;
    
    QLabel main_wnd;
    main_wnd.show();
    
    QObject::connect(&socket, &QUdpSocket::readyRead, [&]() {
        qDebug() << ".";

        while (socket.hasPendingDatagrams()) {
            
            QByteArray datagram;
            datagram.resize(socket.pendingDatagramSize());
            QHostAddress sender;
            quint16 senderPort;

            socket.readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

            QDataStream stream(datagram);
            int32_t width, height, step;

            QByteArray imageData;
            stream >> width >> height >> step >> imageData;

            qDebug() << width << "x" << height << "x" << step;

            QImage img(reinterpret_cast<const uchar*>(imageData.data()),
                       width, height, step, QImage::Format_RGB888);
            
            main_wnd.setPixmap(QPixmap::fromImage(img.rgbSwapped()));
            main_wnd.setScaledContents(true);
            main_wnd.adjustSize();
        }
        });
    
    return app.exec();
}
