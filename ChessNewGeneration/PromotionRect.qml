import QtQuick 2.11

Rectangle {
    property int type : promotionImageId.type
    property alias source : promotionImageId.source
    property var afterClick : null
    id: promotionRectId
    width: 60
    height: 60
    MouseArea{
        id: promotionRectMouseAreaId
        anchors.fill:parent
        onClicked: {
            game.promote(type)
            afterClick()
        }
    }
    Image {
        id: promotionImageId
        anchors.centerIn: parent
        source: ""
    }
}
