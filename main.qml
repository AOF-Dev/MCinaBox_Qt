import QtQuick 2.12
import QtQuick.Window 2.3
//import QtQuick.VirtualKeyboard 2.4
import QtQuick.Controls.Material 2.3
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.11

Window {
    id: window
    width: 1000
    height: 500
    visible: true
    Material.theme: Material.Light
    property int index_a: 0
    property int index_b: 0
    property string new_game_name: ""
    Image {
        id: backgroundimage
        anchors.fill: parent
        source: "/icon/66908482_p0_waifu2x_art_scale_tta_1.png"
        fillMode: Image.PreserveAspectCrop
    }
    ToolBar {
        id: toptoolbar
        z: 10
        width: parent.width
        height: 51
        Label {
            id: title
            x: font.pointSize*0.3
            text: "MCinaBox"
            font.pixelSize: 40
        }
        ToolButton {
            id: back_button
            x: parent.width-width
            width: parent.height
            visible: game_stackView.depth != 1
            text: "\u25C4"
            font.pixelSize: 25
            onReleased: {
                game_stackView.pop()
                if (game_stackView.depth == 2) game_stackView.pop() //no more depth plz
            }
        }
        ToolButton {
            id: refresh_button
            x: game_stackView.depth == 1 ? parent.width-width : back_button.x-width
            width: parent.height
            text: "\u21BA"
            font.pixelSize: 25
            onReleased: {
                if (game_stackView.depth == 1 ) game_version_manager.fresh_game_version_list()
            }
        }
        Label {
            id: progressbar
            x: parent.width/2
            height: parent.height
            width: contentWidth
            text: qsTr("无下载")
            font.pixelSize: 30
            Connections {
                target: checker
                onFresh_downloaded: progressbar.text = qsTr("下载中")+checker.to_downloaded()+"/"+checker.to_total()
                onDownload_Completed: progressbar.text = qsTr("下载完成")
            }
        }
    }

    ToolBar {
        id: bottomtoolbar
        z: 10
        y: parent.height-height
        height: 51
        width: parent.width
        Label {
            id: game_version_label
            x: 5
            text: qsTr("请选择游戏版本")
            font.pixelSize: 34
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
        y: toptoolbar.height
        width: 200
        height: window.height
        color: "#90ffffff"
        Flickable {
            id: leftlist
            anchors.fill: parent
            contentWidth: width
            contentHeight: height
            flickableDirection: Flickable.VerticalFlick
            Button {
                id: add_user_button
                z: 2
                Material.background: "#88ffffff"
                y: 0
                width: parent.width
                height: 50
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
                y: 38
                width: parent.width
                height: 50
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
                y: 76
                width: parent.width
                height: user_list.contentHeight
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
                        font.pointSize: 18
                        onClicked: {
                            if (user_list.currentIndex!=-1) user_list.currentItem.highlighted = false
                            user_list.currentIndex = index
                            user_list.currentItem.highlighted = true
                        }
                    }
                }
            }

            Button {
                id: game_setting_button
                Material.background: "#88ffffff"
                z: 1
                y: user_list.y+user_list.height+80
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
            }
            Button {
                enabled: game_stackView.depth==1 ? true : false
                Material.background: "#88ffffff"
                z: 0
                y: user_list.y+user_list.height+118
                width: parent.width
                text: qsTr("添加游戏")
                font.pointSize: 15
                Image {
                    x:5
                    y:10
                    width: 30
                    height: 30
                    source: "/icon/81718551_p0.jpg"
                }
                onClicked: {
                    game_stackView.push(busy_page)
                    install_new_game.get_new_game_list()
                }
            }
            Button {
                Material.background: "#88ffffff"
                z: -1
                y: user_list.y+user_list.height+160
                width: parent.width
                text: qsTr("关于")
                font.pointSize: 15
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
        y: toptoolbar.height
        width: window.width-x
        height: window.height-toptoolbar.height-bottomtoolbar.height
        Component.onCompleted: push(game_list_page)

    }
    Component {
        id: game_list_page
        Rectangle {
            color: "#00000000"
            Button {
                id: add_new_game_button
                visible: game_list.count==0 ? true : false
                Material.background: "#aaffffff"
                y: -height*0.1
                width: parent.width
                height: parent.height*0.2
                text: qsTr("还没有游戏，点击添加游戏")
                font.pointSize: 18
                onClicked: {
                    game_stackView.push(busy_page)
                    install_new_game.get_new_game_list()
                }
            }
            Component.onCompleted: game_version_manager.fresh_game_version_list
            ListView {
                id: game_list
                y: game_list.count==0 ? add_new_game_button.height*1.05 : 0
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
                        font.pointSize: 30

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
                    onGet_Completed: {
                        game_stackView.push(choose_install_game_version_page_root)
                    }
                }
                Connections {
                    target: install_new_game
                    onGet_Failed: {
                        game_stackView.push(get_failed_page)
                    }
                }
                Connections {
                    target: checker
                    onFresh_downloaded: game_stackView.pop(null)
                }
            }
        }
    }
    Component {
        id: get_failed_page
        Rectangle {
            color: "#00000000"
            Button {
                id: back_button_failed
                Material.background: "#aaffffff"
                y: -height*0.1
                width: parent.width
                height: parent.height*0.2
                text: qsTr("返回")
                font.pointSize: 18
                onClicked: {
                    game_stackView.pop(null)
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
                    text: qsTr("获取游戏版本列表失败")
                    horizontalAlignment: Text.AlignHCenter
                    font.pointSize: 25
                }
            }
        }
    }

    Component {
        id: choose_install_game_version_page_root
        Rectangle {
            color: "#00000000"
            Column {
                anchors.fill: parent
                spacing: 10
                Button {
                    id: snapshot_version_button
                    Material.background: "#88ffffff"
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: parent.width
                    height: window.height/5
                    text: qsTr("快照版（有问题）")
                    font.pointSize: 15
                    onClicked: {
                        game_stackView.push(choose_install_game_version_page_snapshot)
                        index_a = 1
                    }
                }
                Button {
                    id: release_version_button
                    Material.background: "#88ffffff"
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: parent.width
                    height: window.height/5
                    text: qsTr("正式版")
                    font.pointSize: 15
                    onClicked: {
                        game_stackView.push(choose_install_game_version_page_release)
                        index_a = 2
                    }
                }
                Button {
                    id: old_version_button
                    Material.background: "#88ffffff"
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: parent.width
                    height: window.height/5
                    text: qsTr("旧版（有问题）")
                    font.pointSize: 15
                    onClicked: {
                        game_stackView.push(choose_install_game_version_page_old)
                        index_a = 3
                    }
                }
            }


        }
    }
    Component {
        id: choose_install_game_version_page_snapshot
        Rectangle {
            color: "#00000000"
            ListView {
                id: snapshot_list
                width: parent.width
                height: parent.height
                spacing: -10
                model: install_new_game.snapshot_version
                interactive: true
                clip: true
                delegate: Button {
                    width: snapshot_list.width
                    height: 60
                    Material.background: "#a0ffffff"
                    Text {
                        x: 20
                        y: 10
                        text: modelData
                        font.pointSize: 20
                    }
                    onClicked: {
                        game_stackView.push(choose_install_options_page)
                        index_b = index
                        new_game_name = modelData
                    }
                }
            }
        }
    }
    Component {
        id: choose_install_game_version_page_release
        Rectangle {
            color: "#00000000"
            ListView {
                id: release_list
                width: parent.width
                height: parent.height
                spacing: -10
                model: install_new_game.release_version
                clip: true
                interactive: true
                delegate: Button {
                    width: release_list.width
                    height: 60
                    Material.background: "#a0ffffff"
                    Text {
                        x: 20
                        y: 10
                        text: modelData
                        font.pointSize: 20
                    }
                    onClicked: {
                        game_stackView.push(choose_install_options_page)
                        index_b = index
                        new_game_name = modelData
                    }
                }
            }
        }
    }
    Component {
        id: choose_install_game_version_page_old
        Rectangle {
            color: "#00000000"
            ListView {
                id: old_list
                width: parent.width
                height: parent.height
                spacing: -10
                model: install_new_game.old_version
                clip: true
                delegate: Button {
                    width: old_list.width
                    height: 60
                    Material.background: "#a0ffffff"
                    Text {
                        x: 20
                        y: 10
                        text: modelData
                        font.pointSize: 20
                    }
                    onClicked: {
                        game_stackView.push(choose_install_options_page)
                        index_b = index
                        new_game_name = modelData
                    }
                }
            }
        }
    }
    Component {
        id: choose_install_options_page
        Rectangle {
            color: "#00000000"
            Column {
                anchors.fill: parent
                spacing: 5
                Button {
                    anchors.horizontalCenter: parent.horizontalCenter
                    height: parent.height/5.3
                    width: parent.width
                    enabled: false
                    text: qsTr("安装Optifine（未实现）")
                }
                Button {
                    anchors.horizontalCenter: parent.horizontalCenter
                    height: parent.height/5.3
                    width: parent.width
                    enabled: false
                    text: qsTr("安装Forge（未实现）")
                }
                Button {
                    anchors.horizontalCenter: parent.horizontalCenter
                    height: parent.height/5.3
                    width: parent.width
                    enabled: false
                    text: qsTr("安装Fabric（未实现）")
                }
                TextInput {
                    id: new_game_name_textinput
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: parent.width
                    height: parent.height/6
                    text: new_game_name
                    horizontalAlignment: Text.AlignHCenter
                    selectByMouse: true
                    font.pointSize: 20
                    onTextChanged: {
                        for(var i = 0; i < game_version_manager.count_game_version_list(); ++i) if (game_version_manager.get_game_version_string(i) == text) install_button.enabled = false
                        else install_button.enabled = true
                    }
                }
                Button {
                    id: install_button
                    z: 0
                    x: width
                    anchors.horizontalCenter: parent.horizontalCenter
                    height: parent.height/5
                    width: parent.width/2
                    enabled: true
                    text: enabled? qsTr("安装") : qsTr("安装（游戏名已存在）")
                    font.pointSize: 18
                    onClicked: {
                        install_new_game.set_index(index_a,index_b)
                        install_new_game.set_new_game_name(new_game_name_textinput.text)
                        install_new_game.get_new_game_json()
                        game_stackView.push(busy_page)
                    }
                }
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
    D{i:0;autoSize:true;formeditorZoom:0.1;height:480;width:640}
}
##^##*/
