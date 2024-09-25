import QtQuick 2.12
import QtQuick.Controls 2.12
import "../qml/common/customcombobox"
import "../qml/common/custombutton"


Rectangle {

    property int screenWidth                                    : controller.width
    property int screenHeight                                   : controller.height
    property var serialMessage                                  : controller.serialMessage
    property bool cameraConnectionStatus                        : controller.connectionStatus


    id                                                          : rootRectId
    visible                                                     : true
    anchors.centerIn                                            : parent
    width                                                       : screenWidth
    height                                                      : screenHeight
    color                                                       : "#112B3C"

    Component.onCompleted                                       : controller.setSelectedPortName(availablePortsComboboxId.currentText)
    onSerialMessageChanged                                      : zoomValueLabelId.text = serialMessage
    onCameraConnectionStatusChanged                             : {
        if(cameraConnectionStatus)
            getZoomTimerId.running = true
        else
            getZoomTimerId.running = false
    }

    // to check available serial ports
    Timer{
        running                                                 : true
        repeat                                                  : true
        interval                                                : 20
        onTriggered                                             : availablePortsComboboxId.model = controller.availablePorts()
    }

    // to get zoom value continuously
    Timer{
        id                                                      : getZoomTimerId
        running                                                 : false
        repeat                                                  : true
        interval                                                : 20
        onTriggered                                             : controller.getZoomValue()
    }


    Grid{
        id                                                      : buttonsGridId
        anchors.top                                             : parent.top
        anchors.topMargin                                       : parent.height / 10
        anchors.horizontalCenter                                : parent.horizontalCenter
        columns                                                 : 2
        columnSpacing                                           : 10
        rowSpacing                                              : 10
        CustomButton{
            width                                               : 80
            height                                              : width / 2
            radius                                              : 20
            text                                                : "Zoom (+)"
            color                                               : isHovered ? "#F66B0E" : "#205375"
            onPressed                                           : {
                controller.zoomIn()
                controller.getZoomValue()
            }
            onReleased                                          : {
                controller.zoomStop()
                controller.getZoomValue()
            }
        }
        CustomButton{
            width                                               : 80
            height                                              : width / 2
            radius                                              : 20
            text                                                : "Zoom (-)"
            color                                               : isHovered ? "#F66B0E" : "#205375"
            onPressed                                           : {
                controller.zoomOut()
                controller.getZoomValue()
            }
            onReleased                                          : {
                controller.zoomStop()
                controller.getZoomValue()
            }
        }
        CustomButton{
            width                                               : 80
            height                                              : width / 2
            radius                                              : 20
            text                                                : "Focus (+)"
            color                                               : isHovered ? "#F66B0E" : "#205375"
            onPressed                                           : controller.focusIn()
            onReleased                                          : controller.focusStop()
        }
        CustomButton{
            width                                               : 80
            height                                              : width / 2
            radius                                              : 20
            text                                                : "Focus (-)"
            color                                               : isHovered ? "#F66B0E" : "#205375"
            onPressed                                           : controller.focusOut()
            onReleased                                          : controller.focusStop()
        }
        CustomButton{
            width                                               : 80
            height                                              : width / 2
            radius                                              : 20
            text                                                : "Open Iris"
            color                                               : isHovered ? "#F66B0E" : "#205375"
            onClicked                                           : controller.irisOpen()
        }
        CustomButton{
            width                                               : 80
            height                                              : width / 2
            radius                                              : 20
            text                                                : "Close Iris"
            color                                               : isHovered ? "#F66B0E" : "#205375"
            onClicked                                           : controller.irisClose()
        }
    }

    Row{
        id                                                      : portsRowId
        anchors.top                                             : buttonsGridId.bottom
        anchors.topMargin                                       : parent.height / 30
        anchors.horizontalCenter                                : parent.horizontalCenter
        spacing                                                 : 5
        Label {
            height                                              : 40
            width                                               : 50
            text                                                : "Port : "
            color                                               : "white"
            font.pixelSize                                      : 15
            verticalAlignment                                   : Text.AlignVCenter
        }
        CommonComboBox{
            id                                                  : availablePortsComboboxId
            width                                               : 130
            height                                              : 40
            model                                               : controller.availablePorts()
            onCurrentTextChanged                                : controller.setSelectedPortName(currentText)
        }
        CustomButton{
            property bool ejectStatus                           : true

            width                                               : 80
            height                                              : width / 2
            color                                               : isHovered ? "#F66B0E" : "#205375"
            enabled                                             : availablePortsComboboxId.count !== 0
            radius                                              : 20
            txt.text                                            : {
                if(!ejectStatus)
                    return "Çıkart"
                else
                    return "Tak"
            }
            onClicked                                           : {
                ejectStatus = !ejectStatus
                controller.openCloseSerialPort(ejectStatus)
            }
        }
    }

    Row{
        anchors.top                                             : portsRowId.bottom
        anchors.topMargin                                       : parent.height / 30
        anchors.horizontalCenter                                : parent.horizontalCenter
        Label{
            text                                                : "Zoom Değeri : "
            color                                               : "white"
            font.pixelSize                                      : 15
        }
        Label{
            id                                                  : zoomValueLabelId
            text                                                : serialMessage
            color                                               : "white"
            font.pixelSize                                      : 15
        }
    }
}

