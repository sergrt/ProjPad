#pragma once
#include "QTreeWidget"
#include "QMouseEvent"
#include "QDateTime"

class DeselectableTreeWidget : public QTreeWidget {
    Q_OBJECT

public:
    DeselectableTreeWidget(QWidget *parent) : QTreeWidget(parent) {}
    virtual ~DeselectableTreeWidget() {}

private:
    void mousePressEvent(QMouseEvent* event) override {
        QModelIndex item = indexAt(event->pos());
        
        // Edit item if two clicks detected on the same item within predefined time interval
        if (item == lastPressedItem_) {
            const auto dt = QDateTime::currentDateTime();
            static const int editTimeLimitFrom = 1000;
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
    void mouseDoubleClickEvent(QMouseEvent* event) override {
        emit doubleClicked(itemAt(event->pos()));
        QTreeWidget::mouseDoubleClickEvent(event);
    }
    QModelIndex lastPressedItem_;
    QDateTime lastPressedTime_;

    QTreeWidgetItem* dragItem_ = nullptr;
    void startDrag(Qt::DropActions supportedActions) override {
        auto items = selectedItems();
        if (!items.empty())
            dragItem_ = items[0];
        
        QTreeWidget::startDrag(supportedActions);
    }

    void dropEvent(QDropEvent* event) override {
        QTreeWidgetItem* parent = itemAt(event->pos());

        const DropIndicatorPosition dropIndicator = dropIndicatorPosition();
        if (dropIndicator == DropIndicatorPosition::AboveItem)
            emit itemDroppedAbove(dragItem_, parent);
        else if (dropIndicator == DropIndicatorPosition::BelowItem)
            emit itemDroppedBelow(dragItem_, parent);
        else  if (dropIndicator == DropIndicatorPosition::OnItem)
            emit itemDroppedOnItem(dragItem_, parent);
        
        QTreeWidget::dropEvent(event);
        dragItem_ = nullptr;
    }
signals:
    void itemDroppedOnItem(QTreeWidgetItem* dropped, QTreeWidgetItem* parent);
    void itemDroppedAbove(QTreeWidgetItem* dropped, QTreeWidgetItem* parent);
    void itemDroppedBelow(QTreeWidgetItem* dropped, QTreeWidgetItem* parent);

    void doubleClicked(QTreeWidgetItem* item);
};
