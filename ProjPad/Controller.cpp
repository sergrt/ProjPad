#include "stdafx.h"
#include "Controller.h"

Controller::Controller(Project* model, View* view)
    : model_{ model }, view_{ view } {
}

void Controller::onTreeItemActivated(QTreeWidgetItem* item, int col) {
    int id = item->data(0, Qt::UserRole).toInt();
    view_->treeSelectionChanged(id);
    /*
    curDocId_ = id;
    if (model_->type(id) == Node::Type::text) {
        view_->updateContent(id);
    }
    */
}
/*
void Controller::onTextChanged() {
    const std::string text = view_->getCurrentText();
    model_->updateText(curDocId_, text);
}
*/
void Controller::onLoadClick() {
    model_->load();
}