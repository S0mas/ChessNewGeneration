import QtQuick 2.11
import QtQuick.Window 2.11
Window {
    visible: true
    width: height
    height: 640

    Connections {
        target: game
        onPromotion: {

        }
    }

    Grid {
        columns: 8
        rows: 8
        spacing: 0
        width: 480 < parent.width ? parent.width : 480
        height: width
        anchors.centerIn: parent

        Repeater {
            id: repeaterId
            model: ["A1", "B1", "C1", "D1", "E1", "F1", "G1", "H1",
                    "A2", "B2", "C2", "D2", "E2", "F2", "G2", "H2",
                    "A3", "B3", "C3", "D3", "E3", "F3", "G3", "H3",
                    "A4", "B4", "C4", "D4", "E4", "F4", "G4", "H4",
                    "A5", "B5", "C5", "D5", "E5", "F5", "G5", "H5",
                    "A6", "B6", "C6", "D6", "E6", "F6", "G6", "H6",
                    "A7", "B7", "C7", "D7", "E7", "F7", "G7", "H7",
                    "A8", "B8", "C8", "D8", "E8", "F8", "G8", "H8"]
            property bool active: false
            property string originPosition: "None"
            delegate: Rectangle {
                width: parent.width/8
                height: width
                property int column: index % 8
                property int row: index / 8
                property alias image: delegateImageId.source
                property string position: modelData

                color: (row + column) % 2 == 0 ? "black" : "white"
                Image {
                    id: delegateImageId
                    anchors.centerIn: parent
                    anchors.fill:parent
                    source: getImageByPieceType(modelData)
                }
                MouseArea {
                    anchors.fill : parent
                    onClicked: {
                        if(!repeaterId.active) {
                            repeaterId.active = true
                            repeaterId.originPosition = modelData
                        }
                        else {
                            var result = game.move(repeaterId.originPosition, modelData)
                            repeaterId.active = false
                            if(result)
                                for(var i = 0; i < repeaterId.count; ++i)
                                    repeaterId.itemAt(i).image = getImageByPieceType(repeaterId.itemAt(i).position)
                        }
                    }
                }
            }
        }
    }

    function getImageByPieceType(position) {
        var type = game.getPieceTypeByPosition(position);
        if(type == 0)
            return "images/king_white.png";
        if(type == 1)
            return "images/queen_white.png";
        if(type == 2)
            return "images/rook_white.png";
        if(type == 3)
            return "images/bishop_white.png";
        if(type == 4)
            return "images/knight_white.png";
        if(type == 5)
            return "images/pawn_white.png";
        if(type == 6)
            return "images/king_black.png";
        if(type == 7)
            return "images/queen_black.png";
        if(type == 8)
            return "images/rook_black.png";
        if(type == 9)
            return "images/bishop_black.png";
        if(type == 10)
            return "images/knight_black.png";
        if(type == 11)
            return "images/pawn_black.png";
        return "";
    }
}
