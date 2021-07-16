import QtQuick 2.12
import QtQuick.Window 2.12
//import QtQuick.VirtualKeyboard 2.4
import QtQuick.Controls.Material 2.3
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.11

ApplicationWindow {
    id: window
    width: 1000
    height: 707
    visible: true
    color: "#88ffffff"
    Material.theme: Material.Light
    background: Image {
        id: backgroundimage
        source: "/icon/66908482_p0_waifu2x_art_scale_tta_1.png"
    }
    header: ToolBar {
        id: toptoolbar
        Label {
            id: title
            x: font.pointSize*0.3
            text: "MCinaBox"
            font.pointSize: 22.8
        }

        ToolButton {
            id: toptoolbutton
            width: toptoolbar.height
            height: toptoolbar.height
            x: toptoolbar.width-height
            onReleased: {
                launcher_settings.open()
            }

            Text {
                x: (parent.width-contentWidth)/2
                y: (parent.height-contentHeight)/2
                color: Material.foreground
                text: "≡"
                font.pointSize: 18
            }
        }
        ToolButton {
            id: downloader_button
            width: toptoolbar.height
            height: toptoolbar.height
            x: toptoolbutton.x-width
            onReleased: {
                downloader.open()
            }
            Image {
                x: 10
                y: 10
                width: parent.width*1.2
                height: parent.height*1.5
                source: "/icon/downloader_icon.svg"
            }
        }
    }
    Popup {
        id: launcher_settings
        x: window.width*0.1
        y: window.height*0.1
        width: window.width*0.8
        height: window.height*0.8
    }
    Popup {
        id: downloader
        x: window.width*0.1
        y: window.height*0.1
        width: window.width*0.8
        height: window.height*0.8
    }

    footer: ToolBar {
        id: bottomtoolbar
        Label {
            id: game_version_label
            text: qsTr("请选择游戏版本")
            font.pointSize: 21
        }
        Button {
            x: window.width*0.99-width
            height: bottomtoolbar.height
            width: 150
            Material.background: Material.accent
            text: qsTr("开始游戏")
            font.pointSize: 19
        }
    }

    Rectangle {
        id: rectangle
        x: 0
        y: 0
        width: 200
        height: window.height
        color: "#90ffffff"
        Flickable {
            id: leftlist
            anchors.fill: parent
            contentWidth: width
            contentHeight: height
            flickableDirection: Flickable.VerticalFlick

            ColumnLayout {
                id: user_manager_layout
                width: parent.width  // @disable-check M16
                height: 50+user_list.height // @disable-check M16
                spacing: -12
                Button {
                    id: add_user_button
                    z: 2
                    Material.background: "#88ffffff"
                    Layout.preferredWidth: parent.width
                    Layout.preferredHeight: 50
                    text: qsTr("添加用户")
                    font.pointSize: 12
                    Image {
                        x:5
                        y:10
                        width: 30
                        height: 30
                        source: "/icon/81718551_p0.jpg"
                    }
                    onClicked: {
                        add_user.open()
                    }
                }
                Button {
                    id: delete_user_button
                    z: 1
                    Material.background: "#88ffffff"
                    Layout.preferredWidth: parent.width
                    Layout.preferredHeight: 50
                    text: qsTr("删除用户")
                    font.pointSize: 12
                    Image {
                        x:5
                        y:10
                        width: 30
                        height: 30
                        source: "/icon/81718551_p0.jpg"
                    }
                    onClicked: {
                        //delete_user.open()
                        Qt.inputMethod.visible=true
                        Qt.inputMethod.show()
                    }
                }
                Rectangle {
                    color: "#80ffffff"
                    z: 0
                    Layout.preferredWidth: parent.width
                    Layout.preferredHeight: user_list.contentHeight
                    ListView {
                        id: user_list
                        anchors.fill: parent
                        spacing: -12
                        interactive: false
                        model: user_manager.user_name_list
                        delegate: Button {
                            width: parent.width
                            height: 50
                            Material.background: "#00000000"
                            text: modelData
                            font.pointSize: 13
                        }
                    }
                }
            }
        }
    }

    Popup {
        id: add_user
        x: (window.width-width)/2
        y: (window.height-height)/2.6
        width: window.width*0.8
        height: window.height*0.4

        Label {
            x: parent.width*0.1
            y: parent.height*0.1
            width: parent.width*0.6
            height: (user_name.text=="")? contentWidth : user_name.contentHeight
            text: (user_name.text=="")? qsTr("用户名") : ""
            clip: false
            font.pointSize: 20
            TextInput {
                id: user_name
                height: parent.implicitHeight
                width:  parent.width
                font.letterSpacing: 0.2
                selectByMouse: true
                font.pointSize: 20
                Rectangle {
                    y: parent.contentHeight
                    width: parent.width
                    height: 1.5
                    color: Material.accentColor
                }
            }
        }
        Label {
            x: parent.width*0.1
            y: parent.height*0.45
            width: parent.width*0.6
            height: (user_password.text=="")? contentWidth : user_password.contentHeight
            text: (user_password.text=="")? qsTr("密码") : ""
            clip: false
            font.pointSize: 20
            TextInput {
                id: user_password
                height: parent.implicitHeight
                width:  parent.width
                font.letterSpacing: 0.2
                selectByMouse: true
                font.pointSize: 20
                Rectangle {
                    y: parent.contentHeight
                    width: parent.width
                    height: 1.5
                    color: Material.accentColor
                }
            }
        }
        Label{
            x: parent.width*0.1
            y: parent.height*0.8
            width: parent.width*0.6
            height: (user_logging_method.text=="")? contentWidth : user_logging_method.contentHeight
            text: (user_logging_method.text=="")? qsTr("登录方式") : ""
            clip: false
            font.pointSize: 20
            TextInput {
                id: user_logging_method
                height: parent.implicitHeight
                width:  parent.width
                font.letterSpacing: 0.2
                selectByMouse: true
                font.pointSize: 20
                Rectangle {
                    y: parent.contentHeight
                    width: parent.width
                    height: 1.5
                    color: Material.accentColor
                }
            }
        }
        Button {
            text: qsTr("取消")
            x: parent.width*0.77
            y: parent.height*0.2
            width: parent.width*0.2
            height: parent.height*0.3
            onClicked: {
                user_name.text=""
                user_password.text=""
                user_logging_method.text=""
                add_user.close()
            }
        }
        Button {
            text: qsTr("确定")
            x: parent.width*0.77
            y: parent.height*0.5
            width: parent.width*0.2
            height: parent.height*0.3
            onClicked: {
                user_manager.add_user(user_name.text,user_password.text,user_logging_method.text)
                add_user.close()
            }
        }
    }
    Popup {
        id: delete_user
        x: (window.width-width)/2
        y: (window.height-height)/2.6
        width: window.width*0.8
        height: window.height*0.2
    }

    ListView {
        id: game_list
        x: 220
        y: 16
        width: 628
        height: 460
        interactive: true
        clip: false
        model: game_version_manager.game_version_list
        spacing: -10
        delegate: Button {
            id: each_game_version_button
            width: game_list.width
            height: 60
            Material.background: highlighted? "#e0ffffff" : "#a0ffffff"
            Text {
                x: 20
                y: 10
                text: modelData
                font.pointSize: 20

            }
            Button {
                x: parent.width-parent.height
                width: parent.height
                height: parent.height
                Material.background: "#00000000"
                Image {
                    anchors.fill: parent
                    source: "/icon/setting_icon.svg"
                }
            }
            onClicked: {
                 game_list.currentItem.highlighted = null
                 game_list.currentIndex = index
                 game_list.currentItem.highlighted = each_game_version_button
                game_version_label.text = qsTr("当前游戏版本：") + game_version_manager.get_game_version_string(index)
            }
        }
    }




    /*InputPanel { //Qt virtualkeyboard,temporaryly ignore it
        id: inputPanel
        z: 99
        x: 0
        y: window.height
        width: window.width

        states: State {
            name: "visible"
            when: inputPanel.active
            PropertyChanges {
                target: inputPanel
                y: window.height - inputPanel.height
            }
        }
        transitions: Transition {
            from: ""
            to: "visible"
            reversible: true
            ParallelAnimation {
                NumberAnimation {
                    properties: "y"
                    duration: 250
                    easing.type: Easing.InOutQuad
                }
            }
        }
    }
    */
}





/*##^##
Designer {
    D{i:0;formeditorZoom:0.5}
}
##^##*/
