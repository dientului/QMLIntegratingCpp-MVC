#ifndef TODOMODEL_H
#define TODOMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QFile>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

class TodoItem {
public:
    explicit TodoItem(const QString &content, const bool &isDone){
        pContent = content;
        pIsDone = isDone;
    };

    QString content() const{
        return pContent;
    }
    bool isDone()const{
        return pIsDone;
    }

    bool setContent(const QString &content){
        if(pContent == content){
            return false;
        }

        pContent = content;
        return true;
    }

    bool setIsDone(const bool &isDone){
        pIsDone = isDone;
        return true;
    }

private:
    QString pContent;
    bool pIsDone;
};

class TodoModel: public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString inputFile READ inputFile WRITE setInputFile NOTIFY inputFileChanged)
signals:
    void inputFileChanged();
public:
    QString inputFile()const {
        return pInputFile;
    }
    void setInputFile(QString const &inputFile){
        if(pInputFile == inputFile){
            return;
        }

        pInputFile = inputFile;

        loadTodoListModelFromInputFile();

        emit inputFileChanged();
    }
    TodoModel(QObject *parent = nullptr): QAbstractListModel(parent){
    }
    enum TodoItemRoles{
        ContentRole = Qt::UserRole +1,
        IsDoneRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override{
        return pTodoList.length();
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override{

        if(!index.isValid()){
            return QVariant();
        }
        const TodoItem &item = pTodoList[index.row()];
        switch (role) {
        case ContentRole:
            return item.content();
        case IsDoneRole:
            return item.isDone();
        default:
            return QVariant();
        }
    }

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override{

        if(!index.isValid()){
            return false;
        }

        TodoItem &item = pTodoList[index.row()];
        switch (role) {
        case ContentRole:
            return item.setContent(value.toString());
        case IsDoneRole:
            return item.setIsDone(value.toBool());
        default:
            return false;
        }
    }

    Q_INVOKABLE void addItem(){
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        pTodoList.append(TodoItem("",false));
        endInsertRows();
    }

    Q_INVOKABLE void removeItem(const int &index){
        beginRemoveRows(QModelIndex(),index, index);
        pTodoList.remove(index, 1);
        endRemoveRows();
    }

    Q_INVOKABLE void saveTodoListModelToFile(){
        QFile saveDoc(pInputFile);

        if( !saveDoc.open(QIODevice::WriteOnly | QIODevice::Truncate)){
            qWarning("Could not open file");
            return;
        }

        QJsonObject rootObject;
        QJsonArray todolistArray;

        for(int index =0; index< pTodoList.size(); index++){

            QJsonObject todoItemJson;
            todoItemJson["noidung"] = pTodoList[index].content();
            todoItemJson["trangthai"] = pTodoList[index].isDone();
            todolistArray.append(todoItemJson);
        }

        rootObject["todolist"] = todolistArray;

        auto writeContent = QJsonDocument(rootObject).toJson();
        saveDoc.write(writeContent);
        saveDoc.close();

    }

protected:
    QHash<int, QByteArray> roleNames() const override{
        QHash<int, QByteArray> roles;
        roles[ContentRole] = "noidung";
        roles[IsDoneRole] = "trangthai";

        return roles;
    }

private:
    QList<TodoItem> pTodoList;
    QString pInputFile;

    void loadTodoListModelFromInputFile(){

        QFile loadDoc(pInputFile);

        if( !loadDoc.open(QIODevice::ReadOnly)){
            qWarning("Could not open file");
            return;
        }

        auto fileContent = loadDoc.readAll();
        QJsonDocument saveFileDoc = QJsonDocument::fromJson(fileContent);

        QJsonObject rootObject = saveFileDoc.object();

        if(rootObject.contains("todolist") && rootObject["todolist"].isArray()){

            auto todolistJsonArray = rootObject["todolist"].toArray();
            QList<TodoItem> newTodoList;
            for(int index =0; index < todolistJsonArray.size(); index++){
                auto jsonTodoItem = todolistJsonArray[index].toObject();
                auto newTodoItem = TodoItem( jsonTodoItem["noidung"].toString(),jsonTodoItem["trangthai"].toBool());
                newTodoList.append(newTodoItem);
            }
            beginResetModel();
            pTodoList = newTodoList;
            endResetModel();
            loadDoc.close();
        }else{
            qWarning("file not contain todolist key");
            return;
        }
    }
};

#endif // TODOMODEL_H
