#pragma once
#include "QTreeWidget"
#include "QMouseEvent"
#include "QDateTime"

class DeselectableTreeWidget : public QTreeWidget {
public:
    DeselectableTreeWidget(QWidget *parent) : QTreeWidget(parent) {}
    virtual ~DeselectableTreeWidget() {}

private:
    void mousePressEvent(QMouseEvent* event) override {
        QModelIndex item = indexAt(event->pos());
        
        // Edit item if two clicks detected on the same item within predefined time interval
        if (item == lastPressedItem_) {
            const auto dt = QDateTime::currentDateTime();
            static const int editTimeLimitFrom = 500;
            static const int editTimeLimitTo = 3000;

            if (lastPressedTime_.msecsTo(dt) >= editTimeLimitFrom && lastPressedTime_.msecsTo(dt) <= editTimeLimitTo)
                this->editItem(itemAt(event->pos()));

            // update last pressed time
            lastPressedTime_ = QDateTime::currentDateTime();
        } else {
            lastPressedItem_ = item;
            lastPressedTime_ = QDateTime::currentDateTime();
        }
        // Uncomment to select-deselect when same item clicked twice
        //bool selected = selectionModel()->isSelected(indexAt(event->pos()));
        QTreeView::mousePressEvent(event);
        if ((item.row() == -1 && item.column() == -1) /*|| selected*/) {
            clearSelection();
            const QModelIndex index;
            selectionModel()->setCurrentIndex(index, QItemSelectionModel::Select);
        }
    }
    QModelIndex lastPressedItem_;
    QDateTime lastPressedTime_;
};
