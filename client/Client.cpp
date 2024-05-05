#include <QCoreApplication>
#include <QImage>
#include <QLabel>
#include <QUdpSocket>

constexpr int port = 12345;

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    
    QUdpSocket socket;
    socket.bind(QHostAddress::AnyIPv4, port);

    QObject::connect(&socket, &QUdpSocket::readyRead, [&]() {
        QLabel main_wnd;
        main_wnd.show();
        
        while (socket.hasPendingDatagrams()) {
            QByteArray datagram;
            datagram.resize(socket.pendingDatagramSize());
            QHostAddress sender;
            quint16 senderPort;

            socket.readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

            QDataStream stream(datagram);
            int32_t width, height;
            stream >> width >> height;

            QByteArray imageData;
            stream >> imageData;

            QImage img(reinterpret_cast<const uchar*>(imageData.data()),
                       width, height, QImage::Format_RGB888);
            
            main_wnd.setPixmap(QPixmap::fromImage(img.rgbSwapped()));
            main_wnd.setScaledContents(true);
            main_wnd.adjustSize();
        }
        });
    
    return app.exec();
}
