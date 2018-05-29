#pragma once
#include "QTreeWidget"
#include "QMouseEvent"

class DeselectableTreeWidget : public QTreeWidget {
public:
    DeselectableTreeWidget(QWidget *parent) : QTreeWidget(parent) {}
    virtual ~DeselectableTreeWidget() {}

private:
    void mousePressEvent(QMouseEvent* event) override {
        QModelIndex item = indexAt(event->pos());
        bool selected = selectionModel()->isSelected(indexAt(event->pos()));
        QTreeView::mousePressEvent(event);
        if ((item.row() == -1 && item.column() == -1) || selected) {
            clearSelection();
            const QModelIndex index;
            selectionModel()->setCurrentIndex(index, QItemSelectionModel::Select);
        }
    }
    /*
    void mouseDoubleClickEvent(QMouseEvent* event) override {
        QTreeWidgetItem* item = itemAt(event->pos());
        
        QTreeView::mouseDoubleClickEvent(event);
        if (item) {
            item->setFlags(item->flags() | Qt::ItemIsEditable);
            //this->editItem(item);
        }
    }
    */
};
