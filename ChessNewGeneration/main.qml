import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.4
Window {
    visible: true
    width: height
    height: 640



    PromotionDialog {
        id: promotionDialogId
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        width: 240
        height: 60
    }

    Chessboard {
        anchors.centerIn: parent
    }
}
