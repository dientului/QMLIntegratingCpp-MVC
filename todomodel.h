#ifndef TODOMODEL_H
#define TODOMODEL_H

#include <QObject>
#include <QAbstractListModel>


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

public:
    TodoModel(QObject *parent = nullptr): QAbstractListModel(parent){

        pTodoList.append(TodoItem("abc", false));
        pTodoList.append(TodoItem("xyz", true));
        pTodoList.append(TodoItem("aaaa", true));

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
protected:
    QHash<int, QByteArray> roleNames() const override{
        QHash<int, QByteArray> roles;
        roles[ContentRole] = "noidung";
        roles[IsDoneRole] = "trangthai";

        return roles;
    }

private:
    QList<TodoItem> pTodoList;
};

#endif // TODOMODEL_H
