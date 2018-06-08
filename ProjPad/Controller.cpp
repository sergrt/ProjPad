#include "stdafx.h"
#include "Controller.h"
#include <fstream>
#include "Utilities.h"

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
    if (model_->nodeType(itemId) != Node::Type::text)
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
void Controller::load(const std::string& fileName, const std::optional<std::string>& password) const {
    model_->load(fileName, password);
    view_->enableSave();
}
void Controller::save(const std::string& fileName) const {
    model_->save(fileName);
    view_->enableSave();
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
void Controller::setPassword(const std::string& password) const {
    model_->setPassword(password);
}
void Controller::moveNode(int itemId, std::optional<int> parentId) const {
    model_->moveNode(itemId, parentId);
}
void Controller::moveNodeAbove(int itemId, int parentId) const {
    model_->moveNodeAbove(itemId, parentId);
}
void Controller::moveNodeBelow(int itemId, int parentId) const {
    model_->moveNodeBelow(itemId, parentId);
}
void Controller::moveNodeAfterAll(int itemId) const {
    model_->moveNodeAfterAll(itemId);
}

void Controller::updateViewSettings(const std::optional<QPoint>& pos, const std::optional<QSize>& size, const std::optional<bool> maximized) {
    view_->updateViewSettings(pos, size, maximized);
}
void exportNode(std::ofstream& f, const Node& node) {
    const std::string nodeName = node.name();
    
    f << nodeName << "\n";
    const char fillChar = node.type() == Node::Type::folder ? '=' : '-';
    for (int i = 0; i < Utilities::utf8len(nodeName); ++i)
        f << fillChar;
    f << "\n" << "\n";

    if (node.type() == Node::Type::text)
        f << node.text() << "\n" << "\n";

    for (const auto& c : node.children_) {
        exportNode(f, *c);
    }
}
void Controller::exportProject(const std::string& fileName) const {
    std::ofstream f(fileName);
    for (auto i = model_->begin(); i != model_->end(); ++i)
        exportNode(f, *i->get());
}
bool Controller::handleUnsavedProject() const {
    const bool allowClose = model_->hasChanged() ? view_->handleUnsavedProject() : true;
    return allowClose;
}
void Controller::createNewProject() {
    const bool allowClose = model_->hasChanged() ? view_->handleUnsavedProject() : true;
    if (allowClose) {
        view_->closeProject();
        view_->disableSave();
        ModelInterface* newModel = new Project();
        view_->setModel(newModel);
        
        delete model_;
        model_ = newModel;
    }
}
