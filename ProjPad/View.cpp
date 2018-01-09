#include "stdafx.h"
#include "View.h"
#include "EditContainer.h"

View::View(Project* model)
    : model_ { model } {
    if (model_)
        model_->addObserver(this);
}

View::~View() {
    if (model_)
        model_->removeObserver(this);
}

void View::setProjectTree(QTreeWidget* tree) {
    tree_ = tree;
}
void View::setTabsContainer(QTabWidget* tabWidget) {
    tabWidget_ = tabWidget;
}

void View::updateTree() {
    if (!tree_)
        throw std::runtime_error("uninit");
    tree_->clear();

    for (const auto& r : model_->rootNodes_)
        fillTree(r, nullptr);
}
void View::fillTree(std::shared_ptr<Node> node, QTreeWidgetItem* item) {
    QTreeWidgetItem* curItem = new QTreeWidgetItem(QStringList(node->name().c_str()));
    curItem->setData(0, Qt::UserRole, QVariant::fromValue(node->id()));
    setIcon(node->type(), curItem);
    if (!item)
        tree_->addTopLevelItem(curItem);
    else
        item->addChild(curItem);

    for (const auto& r : node->children_)
        fillTree(r, curItem);
}
void View::setIcon(Node::Type type, QTreeWidgetItem* item) {
    if (type == Node::Type::folder)
        item->setIcon(0, QIcon(":/ProjPad/Resources/folder.png"));
    else if (type == Node::Type::text)
        item->setIcon(0, QIcon(":/ProjPad/Resources/text.png"));
    else
        throw std::runtime_error("Unhandled type");
}
void View::updateText(int id) {
    // if itemName is in view
    QTextEdit* textEdit = widgetForTextNode(id);
    if (!textEdit)
        return;

    textEdit->blockSignals(true);
    QTextCursor c = textEdit->textCursor();
    auto xx = c.position();
    textEdit->setText(QString::fromStdString(model_->text(id)));
    QTextCursor c1 = textEdit->textCursor();
    c1.setPosition(xx);
    textEdit->setTextCursor(c1);
    textEdit->blockSignals(false);
}
void View::treeSelectionChanged(int id) {
    if (model_->type(id) != Node::Type::text)
        return;

    if (!textNodeOpened(id))
        openTextNode(id);
    else
        focusTextNode(id);
}

bool View::textNodeOpened(int id) const {
    for (int i = 0; i < tabWidget_->count(); ++i) {
        QWidget* w = tabWidget_->widget(i);
        if (w->property("id").toInt() == id)
            return true;
    }
    return false;
}
void View::openTextNode(int id) {
    EditContainer* e = new EditContainer(id);
    e->setText(model_->text(id));
    tabWidget_->addTab(e, QString::fromStdString(model_->name(id)));
    focusTextNode(id);
}
void View::focusTextNode(int id) {
    for (int i = 0; i < tabWidget_->count(); ++i) {
        QWidget* w = tabWidget_->widget(i);
        if (w->property("id").toInt() == id) {
            tabWidget_->setCurrentIndex(i);
            w->setFocus();
            return;
        }
    }
}
QTextEdit* View::widgetForTextNode(int id) {
    for (int i = 0; i < tabWidget_->count(); ++i) {
        QWidget* w = tabWidget_->widget(i);
        if (w->property("id").toInt() == id)
            return static_cast<QTextEdit*>(w);
    }
    return nullptr;
}