#include "Enums.h"
#include "bcontroller.h"
#include <QDateTime>
#include <QQmlContext>
#include <QTimer>
#include <QQmlEngine>
#include <QApplication>
#include <utils.h>
#include <QSerialPortInfo>

BController::BController(QWindow *parent)
    : QQuickView(parent)
    , mSerialMessage("")
    , mConnectionStatus(false)
{
    //begin: object registery
    qmlRegisterType<BController>();
    //end:

    rootContext()->setContextProperty("controller", this);

    mSerialCommunnication = new SerialCommunication();
    QObject::connect(mSerialCommunnication, &SerialCommunication::newMessageFromRemote, this, [this](QByteArray pDatas){
        mComingDataFromSerial = pDatas;
        deserializeMessage();
//        qDebug()<<"message:"<<mComingDataFromSerial;
    });

    setResizeMode(SizeRootObjectToView);

    connect(this, &BController::statusChanged, this, [=](QQuickView::Status pStatus){
        qDebug() << "qml status changed" << pStatus;
        if(pStatus == QQuickView::Ready)
            init();
    });

    setSource(QUrl(QStringLiteral("qrc:/ui/qml/main.qml")));
}

BController::~BController()
{
    if(mSerialCommunnication){
        setConnectionStatus(false);
        mSerialCommunnication->stop();
        mSerialCommunnication->deleteLater();
    }
}

void BController::init()
{
    QTimer::singleShot(10, this, [](){
        availablePorts();
    });
}

void BController::deserializeMessage()
{
    QString tRawADCValue;
    // message type "gzXXX\r\n"   XXX = value size = 7
    if(mComingDataFromSerial.size() == 7 && mComingDataFromSerial.mid(0,2) == "gz" && mComingDataFromSerial.lastIndexOf("\n")){
        tRawADCValue = mComingDataFromSerial.mid((mComingDataFromSerial.indexOf("gz") + 2),3);

        float tPotVal = tRawADCValue.toInt() - 153;

        if(tPotVal < 0)
            tPotVal = 0;

        float tRealZoomValue = (((-33.0 / 420.0) * tPotVal) + 33.0);

        if(tRealZoomValue < 1)
            tRealZoomValue = 1;

        setSerialMessage(QString::number(static_cast<int>(tRealZoomValue)));
    }
}

void BController::openCloseSerialPort(bool pEjectStatus)
{
    if(!pEjectStatus){
        mSerialCommunnication->setPortName(mSelectedPortName);
        mSerialCommunnication->setBaudRate(QSerialPort::BaudRate::Baud9600);
        mSerialCommunnication->setDataBits(QSerialPort::DataBits::Data8);
        mSerialCommunnication->setParity(QSerialPort::Parity::NoParity);
        mSerialCommunnication->setFlowControl(QSerialPort::FlowControl::NoFlowControl);
        mSerialCommunnication->setStopBits(QSerialPort::StopBits::OneStop);
        mSerialCommunnication->start();
        setConnectionStatus(true);
    }
    else{
        mSerialCommunnication->stop();
        setConnectionStatus(false);
    }
}

void BController::setSelectedPortName(QString pSelectedPortName)
{
    mSelectedPortName = pSelectedPortName;
}

void BController::zoomIn()
{
    QByteArray tBuffer;

    tBuffer.append("z+");

    mSerialCommunnication->send(tBuffer);
}

void BController::zoomOut()
{
    QByteArray tBuffer;

    tBuffer.append("z-");

    mSerialCommunnication->send(tBuffer);
}

void BController::zoomStop()
{
    QByteArray tBuffer;

    tBuffer.append("zs");

    mSerialCommunnication->send(tBuffer);
}

void BController::irisOpen()
{
    QByteArray tBuffer;

    tBuffer.append("i+");

    mSerialCommunnication->send(tBuffer);
}

void BController::irisClose()
{
    QByteArray tBuffer;

    tBuffer.append("i-");

    mSerialCommunnication->send(tBuffer);
}

void BController::focusIn()
{
    QByteArray tBuffer;

    tBuffer.append("f+");

    mSerialCommunnication->send(tBuffer);
}

void BController::focusOut()
{
    QByteArray tBuffer;

    tBuffer.append("f-");

    mSerialCommunnication->send(tBuffer);
}

void BController::focusStop()
{
    QByteArray tBuffer;

    tBuffer.append("fs");

    mSerialCommunnication->send(tBuffer);
}

void BController::getZoomValue()
{
    QByteArray tBuffer;

    tBuffer.append("gz");

    mSerialCommunnication->send(tBuffer);
}

void BController::setConnectionStatus(bool connectionStatus)
{
    if (mConnectionStatus == connectionStatus)
        return;

    mConnectionStatus = connectionStatus;
    emit connectionStatusChanged(mConnectionStatus);
}

void BController::setSerialMessage(QString serialMessage)
{
    if (mSerialMessage == serialMessage)
        return;

    mSerialMessage = serialMessage;
    emit serialMessageChanged(mSerialMessage);
}

QVariant BController::availablePorts()
{
    QList<QSerialPortInfo> tPortsAvailable = QSerialPortInfo::availablePorts();
    QStringList tAvailablePortsName;

    for(const QSerialPortInfo& portInfo : tPortsAvailable) {
        tAvailablePortsName<<portInfo.portName();
    }

    return QVariant::fromValue(tAvailablePortsName);
}

bool BController::connectionStatus() const
{
    return mConnectionStatus;
}


QString BController::serialMessage() const
{
    return mSerialMessage;
}


