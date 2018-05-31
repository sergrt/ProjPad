#include "stdafx.h"
#include "Controller.h"

Controller::Controller() {
    model_ = new Project();
    view_ = new View(model_, this);
}
Controller::~Controller() {
    delete view_;
    delete model_;
}
void Controller::setupView(Ui::ProjPadClass* const ui) const {
    view_->setupView(ui);
}
void Controller::treeSelectionChanged(int itemId) const {
    if (model_->projectItemType(itemId) != Node::Type::text)
        return;
    if (!model_->textNodeOpened(itemId))
        model_->openTextNode(itemId);
    
    view_->focusNodeTab(itemId);
}
/*
void Controller::onTreeItemActivated(QTreeWidgetItem* item, int col) {
    int id = item->data(0, Qt::UserRole).toInt();
    view_->treeSelectionChanged(id);
}
*/
/*
void Controller::onTextChanged() {
    const std::string text = view_->getCurrentText();
    model_->updateText(curDocId_, text);
}
*/
void Controller::load(const std::string& fileName) const {
    model_->load(fileName);
    view_->enableSave();
}
void Controller::save(const std::string& fileName) const {
    model_->save(fileName);
}
void Controller::save() const {
    model_->save();
}

void Controller::tabTextChanged(int id, const std::string& text) const {
    model_->setNodeText(id, text);
}
void Controller::addFolder(const std::string& name, std::optional<int> parentId) const {
    model_->addFolder(name, parentId);
}
void Controller::addText(const std::string& name, std::optional<int> parentId) const {
    model_->addText(name, parentId);
}
void Controller::deleteNode(int itemId) const {
    model_->deleteNode(itemId);
}
void Controller::renameNode(int itemId, const std::string& name) const {
    model_->renameNode(itemId, name);
}
void Controller::closeNode(int itemId) const {
    model_->closeNode(itemId);
}