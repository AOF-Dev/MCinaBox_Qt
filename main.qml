import QtQuick 2.12
import QtQuick.Window 2.12
//import QtQuick.VirtualKeyboard 2.4
import QtQuick.Controls.Material 2.3
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.11

ApplicationWindow {
    id: window
    width: 1000
    height: 500
    visible: true
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
            x: 5
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
                    font.pointSize: 15
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
                    font.pointSize: 15
                    Image {
                        x:5
                        y:10
                        width: 30
                        height: 30
                        source: "/icon/81718551_p0.jpg"
                    }
                    onClicked: (user_list.count==0)?  {} : (user_list.currentIndex==-1)? {} : delete_user.open()
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
                        currentIndex: -1
                        delegate: Button {
                            id: each_user_button
                            width: parent.width
                            height: 50
                            Material.background: highlighted? "#60E91E63" : "#00000000"
                            text: modelData
                            font.pointSize: 13
                            onClicked: {
                                if (user_list.currentIndex!=-1) user_list.currentItem.highlighted = false
                                user_list.currentIndex = index
                                user_list.currentItem.highlighted = true
                            }
                        }
                    }
                }
            }
            Button {
                id: add_new_game_button
                Material.background: "#88ffffff"
                y: user_manager_layout.height+50
                width: parent.width
                height: 50
                text: qsTr("游戏设置")
                font.pointSize: 15
                Image {
                    x:5
                    y:10
                    width: 30
                    height: 30
                    source: "/icon/81718551_p0.jpg"
                }
                /*onClicked: {
                    game_stackView.push(busy_page)
                    install_new_game.get_new_game_list()
                }*/
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
            enabled: false
            x: parent.width*0.1
            y: parent.height*0.45
            width: parent.width*0.6
            height: (user_password.text=="")? contentWidth : user_password.contentHeight
            text: (user_password.text=="")? qsTr("密码（暂未实现）") : ""
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
            enabled: false
            x: parent.width*0.1
            y: parent.height*0.8
            width: parent.width*0.6
            height: (user_logging_method.text=="")? contentWidth : user_logging_method.contentHeight
            text: (user_logging_method.text=="")? qsTr("登录方式（暂未实现）") : ""
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
                user_name.text = ""
                user_password.text = ""
                user_logging_method.text = ""
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
        Text {
            text: qsTr("你确定？")
            font.pointSize: 28
        }
        Button {
            text: qsTr("确定")
            font.pointSize: 29
            Material.foreground: Material.Purple
            Material.background: "#00000000"
            x: parent.width*0.58
            y: parent.height*0.25
            width: parent.width*0.15
            height: parent.height*0.7
            onClicked: {
                user_manager.delete_user(user_list.currentIndex)
                delete_user.close()
            }
        }
        Button {
            text: qsTr("取消")
            font.pointSize: 29
            Material.foreground: Material.Purple
            Material.background: "#00000000"
            x: parent.width*0.8
            y: parent.height*0.25
            width: parent.width*0.15
            height: parent.height*0.7
            onClicked: delete_user.close()
        }
    }


    StackView {
        id: game_stackView
        x: leftlist.width*1.006
        width: window.width-leftlist.width
        height: window.height
        Component.onCompleted: push(game_list_page)
    }
    Component {
        id: game_list_page
        Rectangle {
            color: "#00000000"
            anchors.fill: parent
            Button {
                id: add_new_game_button
                Material.background: "#aaffffff"
                y: -height*0.1
                width: parent.width
                height: parent.height*0.2
                text: qsTr("添加游戏")
                font.pointSize: 18
                onClicked: {
                    game_stackView.push(busy_page)
                    install_new_game.get_new_game_list()
                }
            }
            ListView {
                id: game_list
                y: add_new_game_button.height*1.05
                width: parent.width
                height: parent.height
                interactive: true
                clip: true
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
        }

    }
    Component {
        id: busy_page
        Rectangle {
            anchors.fill: parent
            color: "#00000000"
            BusyIndicator {
                id: busy1
                x: (parent.width-width)/2
                y: (parent.height-height)/2
                width: parent.width*0.2
                height: width
                visible: true
                Connections {
                    target: install_new_game
                    onGet_completed: {
                        game_stackView.push(choose_install_game_version_page)
                    }
                }
                Connections {
                    target: install_new_game
                    onGet_failed: {
                        game_stackView.push(get_failed_page)
                    }
                }
            }
        }
    }
    Component {
        id: get_failed_page
        Rectangle {
            color: "#00000000"
            Button {
                id: back_button2
                Material.background: "#aaffffff"
                y: -height*0.1
                width: parent.width
                height: parent.height*0.2
                text: qsTr("返回")
                font.pointSize: 18
                onClicked: {
                    game_stackView.pop()
                    game_stackView.pop()
                }
            }
            Rectangle {
                y: back_button2.height
                width: parent.width
                height: parent.height*0.5
                color: "#80ffffff"
                Text {
                    x: (parent.width-width)/2
                    y: (parent.height-height)/2
                    text: qsTr("获取游戏版本列表失败\n（如果重新联网后依然失败，请重新打开本应用）")
                    horizontalAlignment: Text.AlignHCenter
                    font.pointSize: 25
                }
            }
        }
    }

    Component {
        id: choose_install_game_version_page
        Rectangle {
            color: "#00000000"
            Button {
                id: back_button1
                Material.background: "#aaffffff"
                y: -height*0.1
                width: parent.width
                height: parent.height*0.2
                text: qsTr("返回")
                font.pointSize: 18
                onClicked: {
                    game_stackView.pop()
                    game_stackView.pop()
                }
            }
            Button {
                id: snapshot_version_button
                Material.background: "#88ffffff"
                y: back_button1.height*1.05
                width: parent.width
                height: window.height/6
                text: qsTr("快照版")
                font.pointSize: 15
            }
            Button {
                id: release_version_button
                Material.background: "#88ffffff"
                y: snapshot_version_button.y+snapshot_version_button.height*1.02
                width: parent.width
                height: window.height/6
                text: qsTr("正式版")
                font.pointSize: 15
            }
            Button {
                id: old_version_button
                Material.background: "#88ffffff"
                y: release_version_button.y+snapshot_version_button.height*1.02
                width: parent.width
                height: window.height/6
                text: qsTr("旧版")
                font.pointSize: 15
            }
        }
    }
    Component {
        id: choose_install_options_page
        Rectangle {
            color: "#00000000"
            anchors.fill: parent
            Button {
                width: parent.width
                height: 50
                enabled: false
                text: qsTr("安装Optifine（未实现）")
            }
            Button {
                y: 52
                width: parent.width
                height: 50
                enabled: false
                text: qsTr("安装Forge（未实现）")
            }
            Button {
                y: 104
                width: parent.width
                height: 50
                enabled: false
                text: qsTr("安装Fabric（未实现）")
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
