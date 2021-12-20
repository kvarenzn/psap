import QtQuick 2.15

Rectangle {
    property alias judgeLineModel: judgeLineRepeater.model
    property alias holdsModel: holdsRepeater.model
    property alias tapsModel: tapsRepeater.model
    property alias dragsModel: dragsRepeater.model
    property alias flicksModel: flicksRepeater.model
    property alias backgroundColor: viewer.color

    property color lineColor: 'white'

    id: viewer

    anchors.fill: parent
    color: 'black'

    layer.enabled: true
    layer.smooth: true
    layer.samples: 4


    Repeater {
        id: judgeLineRepeater

        Rectangle {
            x: posX * viewer.width - width / 2
            y: posY * viewer.height - height / 2
            rotation: angle
            color: viewer.lineColor
            opacity: lineOpacity

            width: 65535
            height: 4

            smooth: true
            antialiasing: true
        }
    }

    Repeater {
        id: holdsRepeater

        Rectangle {
            x: posX * viewer.width - width / 2
            y: posY * viewer.height - height
            width: children[0].width
            height: children[0].height - holdPassed * viewer.height

            transformOrigin: Item.Bottom
            rotation: angle

            clip: true
            color: "transparent"

            Column {
                Image {
                    source: "../textures/Sprite/Hold_End.png"
                    width: 98.9
                    height: 5

                    smooth: true
                    mipmap: true
                    antialiasing: true

                    anchors.horizontalCenter: parent.horizontalCenter
                }

                Image {
                    source: dual ? "../textures/Sprite/Hold2HL_1.png" : "../textures/Sprite/Hold.png"
                    width: dual ? 106 : 98.9
                    height: holdLength * viewer.height

                    smooth: true
                    mipmap: true
                    antialiasing: true

                    anchors.horizontalCenter: parent.horizontalCenter
                }

                Image {
                    source: dual ? "../textures/Sprite/Hold2HL_0.png" : "../textures/Sprite/Hold_Head.png"
                    width: dual ? 105.8 : 98.9
                    height: dual ? 9.7 : 5

                    smooth: true
                    mipmap: true
                    antialiasing: true

                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }
        }
    }

    Repeater {
        id: tapsRepeater

        Image {
            source: dual ? "../textures/Texture2D/Tap2HL.png" : "../textures/Texture2D/Tap2.png"
            x: posX * viewer.width - width / 2
            y: posY * viewer.height - height / 2
            rotation: angle

            width: dual ? 108.9 : 98.9
            height: dual ? 20 : 10

            smooth: true
            mipmap: true
            antialiasing: true
        }
    }

    Repeater {
        id: dragsRepeater

        Image {
            source: dual ? "../textures/Sprite/DragHL.png" : "../textures/Sprite/Drag (1).png"
            x: posX * viewer.width - width / 2
            y: posY * viewer.height - height / 2
            rotation: angle

            width: dual ? 108.9 : 98.9
            height: dual ? 16 : 6

            smooth: true
            mipmap: true
            antialiasing: true
        }
    }

    Repeater {
        id: flicksRepeater

        Image {
            source: dual ? "../textures/Sprite/Flick2HL.png" : "../textures/Sprite/Flick2.png"
            x: posX * viewer.width - width / 2
            y: posY * viewer.height - height / 2
            rotation: angle

            width: dual ? 108.9 : 98.9
            height: dual ? 30 : 20

            smooth: true
            mipmap: true
            antialiasing: true
        }
    }
}
