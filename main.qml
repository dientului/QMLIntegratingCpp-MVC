import QtQuick
import QtQuick.Window
import QtQuick.Controls 2.12
import dientului.TodoModel 1.0

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

//    ListModel{
//        id: myModel
//        ListElement{
//            content:"abc"
//            isDone: false
//        }

//        ListElement{
//            content:"xyz"
//            isDone: true
//        }
//    }

    TodoModel{
        id: myModel
    }

    ListView{
        id: myTodoList
        anchors.fill: parent
        model: myModel
        height: parent.height
        header: Row{
            Button{
                text: "append/add"
                onClicked: {
                    myModel.addItem()
                }
            }
        }

        delegate: Row{
            width: parent.width
            TextField{
                text: noidung
                width: parent.width -200
                anchors.leftMargin: 20
                onTextEdited: {
                    noidung = text
                }
            }
            CheckBox{
                checked: trangthai
                onCheckedChanged: {
                    trangthai = checked
                }
            }
            Button{
                text: "Remove"
                onClicked: {
                    myModel.removeItem(index)
                }
            }
        }
    }

}
