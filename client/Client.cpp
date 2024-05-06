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
        while (socket.hasPendingDatagrams()) {
            QByteArray datagram;
            datagram.resize(socket.pendingDatagramSize());
            QHostAddress sender;
            quint16 senderPort;

            socket.readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

            QDataStream stream(datagram);
            QImage q_image;
            stream >> q_image;
            
            main_wnd.setPixmap(QPixmap::fromImage(q_image.rgbSwapped()));
            main_wnd.setScaledContents(true);
            main_wnd.adjustSize();
        }
        });
    
    return app.exec();
}
