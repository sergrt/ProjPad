#include "stdafx.h"
#include "View.h"
#include "EditContainer.h"
#include "ui_ProjPad.h"
#include <QFileDialog>
#include <QMessageBox>

View::View(ModelInterface* model, ControllerInterface* controller)
    : model_{ model }, controller_{ controller } {
    if (model_)
        model_->addObserver(this);
}

View::~View() {
    if (model_)
        model_->removeObserver(this);
}

int View::itemId(QTreeWidgetItem& item) {
    return item.data(0, Qt::UserRole).toInt();
}
void View::setItemId(QTreeWidgetItem& item, int id) {
    item.setData(0, Qt::UserRole, id);
}

void View::setupView(Ui::ProjPadClass* const ui) {
    ui->splitter->setSizes({ 200, 999 });

    tree_ = ui->treeWidget;
    tabWidget_ = ui->tabWidget;
    save_ = ui->actionSave;

    save_->setEnabled(false);

    connect(tree_, &QTreeWidget::itemActivated, this, [this](QTreeWidgetItem* item) {
        controller_->treeSelectionChanged(itemId(*item));
    });
    ////connect(ui.textEdit, &QTextEdit::textChanged, &controller_, &Controller::onTextChanged);
    connect(ui->actionOpen, &QAction::triggered, this, [this]() {
        auto fileName = QFileDialog::getOpenFileName(QApplication::activeWindow(), "Open document", QString(), QString("*.xml"));
        if (!fileName.isEmpty())
            controller_->load(fileName.toStdString());
    });

    connect(ui->actionSaveAs, &QAction::triggered, this, [this]() {
        auto fileName = QFileDialog::getSaveFileName(QApplication::activeWindow(), "Save document as", QString(), QString("*.xml"));
        if (!fileName.isEmpty())
            controller_->save(fileName.toStdString());
    });

    connect(ui->actionSave, &QAction::triggered, this, [this]() {
        controller_->save();
    });

    connect(ui->addFolder, &QPushButton::clicked, this, [this]() {
        QTreeWidgetItem* item = tree_->currentItem();
        controller_->addFolder("New folder", item ? std::make_optional<int>(itemId(*item)) : std::nullopt);
    });

    connect(ui->deleteNode, &QPushButton::clicked, this, [this]() {
        QTreeWidgetItem* item = tree_->currentItem();
        QString confirmText = "Item will be deleted. Proceed?";
        if (item->childCount() != 0)
            confirmText += " (all children items will be deleted also)";

        int res = QMessageBox::warning(tree_, "Confirm", confirmText, QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if (res == QMessageBox::Yes)
            controller_->deleteNode(itemId(*item));
    });
    connect(tree_, &QTreeWidget::itemChanged, this, [this](QTreeWidgetItem* item, int column) {
        controller_->renameNode(itemId(*item), item->text(0).toStdString());
    });
}

void View::focusNodeTab(int itemId) {
    for (int i = 0; i < tabWidget_->count(); ++i) {
        QWidget* w = tabWidget_->widget(i);
        if (w->property("id").toInt() == itemId) {
            tabWidget_->setCurrentIndex(i);
            w->setFocus();
            return;
        }
    }
}

void View::openNode(int id) {
    openNodeInNewTab(id);
    //focusNodeTab(id);
}
void View::closeNode(int id) {
    for (int i = 0; i < tabWidget_->count(); ++i) {
        QWidget* w = tabWidget_->widget(i);
        if (w->property("id").toInt() == id) {
            tabWidget_->removeTab(i);
            return;
        }
    }
}
void View::openNodeInNewTab(int id) {
    EditContainer* e = new EditContainer(id);
    e->setText(model_->nodeText(id));
    tabWidget_->addTab(e, QString::fromStdString(model_->nodeName(id)));
    connect(e, &EditContainer::textChanged, this, &View::tabTextChanged);
}
void View::tabTextChanged(int id, const std::string& text) {
    controller_->tabTextChanged(id, text);
}

void View::updateTree() {
    if (!tree_)
        throw std::runtime_error("uninit");
    tree_->clear();

    for (auto r = model_->begin(); r != model_->end(); ++r) {
        fillTree(r->get(), nullptr);
    }
    /*
    for (const auto& r : model_) {
        fillTree(r, nullptr);
    }
    */
}
QTreeWidgetItem* View::createTreeItem(int id) {
    QTreeWidgetItem* curItem = new QTreeWidgetItem(QStringList(model_->nodeName(id).c_str()));
    curItem->setFlags(curItem->flags() | Qt::ItemIsEditable);
    setItemId(*curItem, id);
    setIcon(model_->nodeType(id), curItem);
    return curItem;
}
void View::fillTree(const Node* node, QTreeWidgetItem* item) {
    QTreeWidgetItem* curItem = createTreeItem(node->id());
    if (!item)
        tree_->addTopLevelItem(curItem);
    else
        item->addChild(curItem);

    for (const auto& r : node->children_)
        fillTree(r.get(), curItem);
}
void View::setIcon(Node::Type type, QTreeWidgetItem* item) {
    if (type == Node::Type::folder)
        item->setIcon(0, QIcon(":/ProjPad/Resources/folder.png"));
    else if (type == Node::Type::text)
        item->setIcon(0, QIcon(":/ProjPad/Resources/text.png"));
    else
        throw std::runtime_error("Unhandled type");
}

void View::enableSave() {
    save_->setEnabled(true);
}

void View::nodeDeleted(int id) {
    const auto item = findTreeNode(id);
    if (item) {
        if (item->parent()) {
            delete item->parent()->takeChild(item->parent()->indexOfChild(item));
        } else {
            auto index = tree_->indexOfTopLevelItem(item);
            delete tree_->takeTopLevelItem(index);
        }
    }
}
QTreeWidgetItem* View::findTreeNode(int id) const {
    for (int i = tree_->topLevelItemCount() - 1; i >= 0; --i) {
        const auto top = tree_->topLevelItem(i);
        const auto item = findTreeNode(top, id);
        if (item)
            return item;
    }
    return nullptr;
}

QTreeWidgetItem* View::findTreeNode(QTreeWidgetItem* cur, int id) const {
    if (itemId(*cur) == id)
        return cur;

    for (int i = cur->childCount() - 1; i >= 0; --i) {
        auto res = findTreeNode(cur->child(i), id);
        if (res)
            return res;
    }

    return nullptr;
}

void View::nodeAdded(int id, std::optional<int> parentId) {
    QTreeWidgetItem* curItem = createTreeItem(id);
    if (parentId) {
        auto item = findTreeNode(*parentId);
        if (!item)
            throw std::runtime_error("Parent node not found");

        item->addChild(curItem);

        if (!item->isExpanded())
            item->setExpanded(true);
    }

    tree_->addTopLevelItem(curItem);
}

void View::nodeRenamed(int id) {
    // No need to rename here, because renaming can be triggered only from view
    //findTreeNode(id)->setText(0, model_->nodeName(id).c_str());
}