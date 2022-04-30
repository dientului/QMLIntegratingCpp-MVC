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
        inputFile: "saveFile.json" // relative path  -> absolute path: <working directory> + <relative path> -> access file tren disk
    }

    ListView{
        id: myTodoList
        anchors.fill: parent
        model: myModel
        header: Row{
            Button{
                text: "append/add"
                onClicked: {
                    myModel.addItem()
                }
            }
            Button{
                text: "save"
                onClicked: {
                    myModel.saveTodoListModelToFile()
                }
            }
        }
        spacing: 20

        delegate: Row{
            width: parent.width
            anchors.left: parent.left
            anchors.leftMargin: 20
            TextField{
                text: noidung
                width: parent.width -200
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
