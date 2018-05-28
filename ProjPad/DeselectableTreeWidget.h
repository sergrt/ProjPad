#pragma once
#include "QTreeWidget"
#include "QMouseEvent"

class DeselectableTreeWidget : public QTreeWidget {
public:
    DeselectableTreeWidget(QWidget *parent) : QTreeWidget(parent) {}
    virtual ~DeselectableTreeWidget() {}

private:
    virtual void mousePressEvent(QMouseEvent* event) {
        QModelIndex item = indexAt(event->pos());
        bool selected = selectionModel()->isSelected(indexAt(event->pos()));
        QTreeView::mousePressEvent(event);
        if ((item.row() == -1 && item.column() == -1) || selected) {
            clearSelection();
            const QModelIndex index;
            selectionModel()->setCurrentIndex(index, QItemSelectionModel::Select);
        }
    }
};
