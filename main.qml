import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12
import QtQuick.Shapes 1.12
import Qt.labs.settings 1.1

ApplicationWindow {
    id: mainWindow
    visible: true
    width: 1280
    height: 745

    footer: RowLayout {
        height: 25
        Text {
            text: "Hello World"
        }
    }

    Settings {
        property alias x: mainWindow.x
        property alias y: mainWindow.y
        property alias width: mainWindow.width
        property alias height: mainWindow.height
    }

    TabBar {
        id: tabBar
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: 80

        TabButton {
            id: libraryTabBtn
            anchors.top: parent.top
            anchors.left: parent.left
            width: tabBar.width
            height: tabBar.parent.height / 3
            icon.name: 'folder-documents'
            text: '数据库'
            display: AbstractButton.TextUnderIcon
        }

        TabButton {
            id: phoneTabBtn
            anchors.top: libraryTabBtn.bottom
            anchors.left: parent.left
            width: tabBar.width
            height: tabBar.parent.height / 3
            icon.name: 'folder-network'
            text: '设备'
            display: AbstractButton.TextUnderIcon
        }

        TabButton {
            id: settingsTabBtn
            anchors.top: phoneTabBtn.bottom
            anchors.left: parent.left
            width: tabBar.width
            height: tabBar.parent.height / 3
            icon.name: 'folder-text'
            text: '设置'
            display: AbstractButton.TextUnderIcon
        }
    }

    StackLayout {
        anchors.top: parent.top
        anchors.left: tabBar.right
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        currentIndex: tabBar.currentIndex

        Item {
            id: libraryTab
            Column {
                x: parent.width - width
                y: 0
                Text {
                    text: `pos: (${libraryTabBtn.x}, ${libraryTabBtn.y}), size: (${libraryTabBtn.width}, ${libraryTabBtn.height})`
                }

                Text {
                    text: `pos: (${phoneTabBtn.x}, ${phoneTabBtn.y}), size: (${phoneTabBtn.width}, ${phoneTabBtn.height})`
                }

                Text {
                    text: `pos: (${settingsTabBtn.x}, ${settingsTabBtn.y}), size: (${settingsTabBtn.width}, ${settingsTabBtn.height})`
                }
            }
        }

        Item {
            id: phoneTab
        }

        Item {
            id: settingsTab

            Rectangle {
                id: viewerWrapper

                anchors.fill: parent

                property real currentTime: 0.0

                Item {
                    property real width_: parent.height * 16 / 9
                    property real isVertical: width_ > parent.width

                    width: isVertical ? parent.width : width_
                    height: isVertical ? parent.width * 9 / 16 : parent.height

                    anchors.centerIn: parent

                    ChartViewer {
                        id: viewer
                        judgeLineModel: judgeLines
                        tapsModel: tapNotes
                        dragsModel: dragNotes
                        flicksModel: flickNotes
                        holdsModel: holdNotes

                        lineColor: 'yellow'
                        backgroundColor: '#77000000'
                    }

                    Item {
                        anchors.fill: parent
                        Repeater {

                        }

                        MouseArea {

                        }
                    }


                    Text {
                        x: parent.width - width - 10
                        y: 0
                        text: 'time: ' + viewerWrapper.currentTime.toFixed(3)
                        color: 'white'
                    }
                }

                Row {
                    x: parent.width - width
                    y: parent.height - height

                    Button {
                        text: '|<'
                        onClicked: {
                            viewerWrapper.currentTime = frameManager.getPrevKeyFrame(viewerWrapper.currentTime)
                            frameManager.renderTime(viewerWrapper.currentTime)
                        }
                    }

                    Button {
                        text: '<<'
                        Timer {
                            id: longPressTimerPrev1

                            interval: 16
                            repeat: true
                            running: false

                            onTriggered: {
                                viewerWrapper.currentTime -= 1.0 / 60
                                if (viewerWrapper.currentTime < 0) {
                                    viewerWrapper.currentTime = 0
                                    return
                                }
                                frameManager.renderTime(viewerWrapper.currentTime)
                            }
                        }

                        onPressedChanged: {
                            if (pressed) {
                                longPressTimerPrev1.running = true
                            } else {
                                longPressTimerPrev1.running = false
                            }
                        }
                    }

                    Button {
                        text: '<'
                        Timer {
                            id: longPressTimerPrev

                            interval: 1
                            repeat: true
                            running: false

                            onTriggered: {
                                viewerWrapper.currentTime -= 0.001
                                if (viewerWrapper.currentTime < 0) {
                                    viewerWrapper.currentTime = 0
                                    return
                                }
                                frameManager.renderTime(viewerWrapper.currentTime)
                            }
                        }

                        onPressedChanged: {
                            if (pressed) {
                                longPressTimerPrev.running = true
                            } else {
                                longPressTimerPrev.running = false
                            }
                        }
                    }

                    Button {
                        text: '>'

                        Timer {
                            id: longPressTimerNext

                            interval: 1
                            repeat: true
                            running: false

                            onTriggered: {
                                viewerWrapper.currentTime += 0.001
                                frameManager.renderTime(viewerWrapper.currentTime)
                            }
                        }

                        onPressedChanged: {
                            if (pressed) {
                                longPressTimerNext.running = true
                            } else {
                                longPressTimerNext.running = false
                            }
                        }
                    }

                    Button {
                        text: '>>'

                        Timer {
                            id: longPressTimerNext1

                            interval: 16
                            repeat: true
                            running: false

                            onTriggered: {
                                viewerWrapper.currentTime += 1.0 / 60
                                frameManager.renderTime(viewerWrapper.currentTime)
                            }
                        }

                        onPressedChanged: {
                            if (pressed) {
                                longPressTimerNext1.running = true
                            } else {
                                longPressTimerNext1.running = false
                            }
                        }
                    }

                    Button {
                        text: '>|'
                        onClicked: {
                            viewerWrapper.currentTime = frameManager.getNextKeyFrame(viewerWrapper.currentTime)
                            frameManager.renderTime(viewerWrapper.currentTime)
                        }
                    }
                }
            }
        }
    }
}
