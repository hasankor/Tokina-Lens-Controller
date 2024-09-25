#ifndef BCONTROLLER_H
#define BCONTROLLER_H


#include <QQuickView>
#include <QQuickWidget>
#include <QSerialPortInfo>
#include <serial/serialsenderreceiver.h>
#include <src/serialcommunication/serialcommunication.h>


class BController : public QQuickView
{
    Q_OBJECT
    Q_PROPERTY(QString serialMessage READ serialMessage WRITE setSerialMessage NOTIFY serialMessageChanged)
    Q_PROPERTY(bool connectionStatus READ connectionStatus WRITE setConnectionStatus NOTIFY connectionStatusChanged)

public:
    explicit BController(QWindow *parent = nullptr);
    virtual ~BController();

    QString serialMessage() const;
    bool connectionStatus() const;

    Q_INVOKABLE static QVariant availablePorts();
public slots:
    void openCloseSerialPort(bool pEjectStatus);
    void setSelectedPortName(QString pSelectedPortName);
    void zoomIn();
    void zoomOut();
    void zoomStop();
    void irisOpen();
    void irisClose();
    void focusIn();
    void focusOut();
    void focusStop();
    void getZoomValue();
signals:
    void serialMessageChanged(QString serialMessage);

    void connectionStatusChanged(bool connectionStatus);

protected slots:
    void init();

private:
    void deserializeMessage();
    void setSerialMessage(QString serialMessage);
    void setConnectionStatus(bool connectionStatus);

private:
    QString                     mSerialMessage;
    QString                     mSelectedPortName;
    QByteArray                  mComingDataFromSerial;
    SerialCommunication         *mSerialCommunnication;
    bool                        mConnectionStatus;
};

#endif // BCONTROLLER_H
