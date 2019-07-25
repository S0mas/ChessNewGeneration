import QtQuick 2.11
import QtQuick.Controls 2.4

Dialog {
    id: promotionDialogId
    title: "Select Promotion"
    modal: true
    focus: true
    closePolicy: Popup.NoAutoClose
    width: contentId.width + 60
    height: contentId.height + 60
    contentItem: Rectangle {
        id:contentId
        width: 240
        height: 60
        Row{
            width: parent.width
            height: parent.height
            anchors.centerIn: parent
            anchors.fill: parent
            PromotionRect{
                id:rectSelectionId1
                type: 1
                afterClick: function() {
                    close()
                }
            }
            PromotionRect{
                id:rectSelectionId2
                type: 2
                afterClick: function() {
                    close()
                }
            }
            PromotionRect{
                id:rectSelectionId3
                type: 3
                afterClick: function() {
                    close()
                }
            }
            PromotionRect{
                id:rectSelectionId4
                type: 4
                afterClick: function() {
                    close()
                }
            }
        }
    }
    Connections {
        target: game
        onPromotion: {
            rectSelectionId1.source = player == 0 ? "images/queen_white.png" : "images/queen_black.png"
            rectSelectionId2.source = player == 0 ? "images/rook_white.png" : "images/rook_black.png"
            rectSelectionId3.source = player == 0 ? "images/bishop_white.png" : "images/bishop_black.png"
            rectSelectionId4.source = player == 0 ? "images/knight_white.png" : "images/knight_black.png"
            open()
        }
    }
}
