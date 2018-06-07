#include "stdafx.h"
#include "View.h"
#include "EditContainer.h"
#include "ui_ProjPad.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QFontDialog>
#include "SetPasswordDialog.h"
#include "PasswordDialog.h"

const QString themesDir = "Themes";

View::View(ModelInterface* model, ControllerInterface* controller)
    : model_{ model }, controller_{ controller } {
    if (model_)
        model_->addObserver(this);
}

View::~View() {
    saveSession();
    if (model_)
        model_->removeObserver(this);
}
void View::saveSession() {
    // window position
    // pos, size and maximized flag already in actual state
    // because these parameters updated by main window as soon as they changed

    // opened file
    auto fileName = model_->fileName();
    session_.setFileName(fileName ? *fileName : "");
    
    // open tree nodes
    std::vector<int> expandedNodes;
    for (int i = 0; i < tree_->topLevelItemCount(); ++i)
        collectExpandedNodes(expandedNodes, tree_->topLevelItem(i));
    session_.setExpandedNodes(expandedNodes);

    // selected node
    const auto selectedItems = tree_->selectedItems();
    session_.setSelectedNode(!selectedItems.empty() ? itemId(*selectedItems[0]) : 0);

    // tree scroll position (v and h)
    session_.setTreeVScrollPos(tree_->verticalScrollBar()->value());
    session_.setTreeHScrollPos(tree_->horizontalScrollBar()->value());

    // opened tabs
    std::vector<Session::TabSession> openedTabs = collectOpenedTabs();
    session_.setOpenedTabs(openedTabs);
        
    // active tab
    session_.setActiveTab(static_cast<EditContainer*>(tabWidget_->widget(tabWidget_->currentIndex()))->id());
}
void View::restoreSession() {
    // restore window position
    QApplication::activeWindow()->move(session_.windowPos());
    QApplication::activeWindow()->resize(session_.windowSize());
    if (session_.windowMaximized())
        QApplication::activeWindow()->showMaximized();

    // restore opened file
    if (!session_.fileName().empty())
        controller_->load(session_.fileName(), std::nullopt);

    // restore open tree nodes
    const auto expandedNodes = session_.expandedNodes();
    for (const auto i : expandedNodes)
        tree_->setItemExpanded(findTreeNode(i), true);
    
    // restore selected node
    tree_->setItemSelected(findTreeNode(session_.selectedNode()), true);

    // restore tree scroll position (v and h)
    tree_->verticalScrollBar()->setValue(session_.treeVScrollPos());
    tree_->horizontalScrollBar()->setValue(session_.treeHScrollPos());

    // restore opened tabs
    std::vector<Session::TabSession> openedTabs = session_.openedTabs();
    // open tabs
    for (const auto& s : openedTabs)
        controller_->treeSelectionChanged(s.id_);

    // apply tab settiongs
    for (const auto& s : openedTabs)
        applyTabSession(s);

    // active tab
    auto t = nodeTab(session_.activeTab());
    if (t.second)
        tabWidget_->setCurrentIndex(t.first);
}
void View::updateViewSettings(const std::optional<QPoint>& pos, const std::optional<QSize>& size, const std::optional<bool> maximized) {
    if (pos)
        session_.setWindowPos(*pos);
    if (size)
        session_.setWindowSize(*size);
    if (maximized)
        session_.setWindowMaximized(*maximized);
}
int View::itemId(QTreeWidgetItem& item) {
    return item.data(0, Qt::UserRole).toInt();
}
void View::setItemId(QTreeWidgetItem& item, int id) {
    item.setData(0, Qt::UserRole, id);
}

void View::fillThemesMenu(QMenu& menu) {
    // Default action - reset theme
    QAction* a = new QAction("*default");
    connect(a, &QAction::triggered, this, [this]() {
        settings_.setFontOverride("");
        settings_.setTheme("");
        applyStyleSheetWithFontOverride("");
    });
    menu.addAction(a);

    QDir themesDir(themesDir);
    auto files = themesDir.entryList(QStringList{ "*.qss" });
    for (const auto& file : files) {
        QAction* a = new QAction(file);

        connect(a, &QAction::triggered, this, [themesDir, file, this]() {
            QFile f(themesDir.dirName() + "/" + file);
            if (f.exists() && f.open(QIODevice::Text | QIODevice::ReadOnly)) {
                settings_.setFontOverride(""); // clear global font if we load theme
                settings_.setTheme(QFileInfo(f.fileName()).fileName());
                applyStyleSheetWithFontOverride(QString(f.readAll()));
            }
        });
        menu.addAction(a);
    }
}
void View::applyStyleSheetWithFontOverride(const QString& styleSheet) {
    QApplication::activeWindow()->setStyleSheet(styleSheet + this->settings_.fontOverride());
}
void View::applyThemeWithFontOverride(const QString& themeName) {
    QFile f(themesDir + "/" + themeName);
    if (f.exists() && f.open(QIODevice::Text | QIODevice::ReadOnly))
        applyStyleSheetWithFontOverride(QString(f.readAll()));
}
void View::setupView(Ui::ProjPadClass* const ui) {
    ui->splitter->setSizes({ 200, 999 });

    tree_ = ui->treeWidget;
    tabWidget_ = ui->tabWidget;
    save_ = ui->actionSave;

    save_->setEnabled(false);
    fillThemesMenu(*ui->menuTheme);

    connect(tree_, &QTreeWidget::itemActivated, this, [this](QTreeWidgetItem* item) {
        controller_->treeSelectionChanged(itemId(*item));
    });
    ////connect(ui.textEdit, &QTextEdit::textChanged, &controller_, &Controller::onTextChanged);
    connect(ui->actionOpen, &QAction::triggered, this, [this]() {
        auto fileName = QFileDialog::getOpenFileName(QApplication::activeWindow(), "Open document", QString(), QString("*.prp"));
        // try to load without password
        // if loading failed - model will notify it needs password, and another function, with password, should be called;
        if (!fileName.isEmpty())
            controller_->load(fileName.toStdString(), std::nullopt);
    });
    connect(ui->actionSaveAs, &QAction::triggered, this, [this]() {
        auto password = model_->password();
        auto fileName = QFileDialog::getSaveFileName(QApplication::activeWindow(), "Save document as", QString(),
            password ? QString("*.pre") : QString("*.prp"));
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
    connect(ui->addText, &QPushButton::clicked, this, [this]() {
        QTreeWidgetItem* item = tree_->currentItem();
        controller_->addText("New text", item ? std::make_optional<int>(itemId(*item)) : std::nullopt);
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
    connect(tabWidget_, &QTabWidget::tabCloseRequested, this, [this](int index) {
        EditContainer* w = static_cast<EditContainer*>(tabWidget_->widget(index));
        controller_->closeNode(w->id());
        /*
        QWidget* w = tabWidget_->widget(index);
        tabWidget_->removeTab(index);
        delete w;
        */
    });
    connect(ui->actionGlobalFont, &QAction::triggered, this, [this]() {
        QFont curFont = QApplication::activeWindow()->font();
        QFontDialog fontDialog(curFont);
        
        if (fontDialog.exec() == QDialog::Accepted) {
            settings_.setFontOverride(fontToStyleSheet(fontDialog.selectedFont()));
            applyStyleSheetWithFontOverride(QApplication::activeWindow()->styleSheet());
        }
    });
    connect(ui->actionSetChangePassword, &QAction::triggered, this, [this]() {
        std::optional<std::string> currentPassword = model_->password();
        SetPasswordDialog d(currentPassword ? *currentPassword : "");

        if (d.exec() == QDialog::Accepted)
            controller_->setPassword(d.selectedPassword());
    });
    connect(tree_, &DeselectableTreeWidget::itemDroppedOnItem, this, [this](QTreeWidgetItem* dropped, QTreeWidgetItem* parent) {
        controller_->moveNode(itemId(*dropped), parent ? std::make_optional(itemId(*parent)) : std::nullopt);
    });
    connect(tree_, &DeselectableTreeWidget::itemDroppedAbove, this, [this](QTreeWidgetItem* dropped, QTreeWidgetItem* parent) {
        controller_->moveNodeAbove(itemId(*dropped), itemId(*parent));
    });
    connect(tree_, &DeselectableTreeWidget::itemDroppedBelow, this, [this](QTreeWidgetItem* dropped, QTreeWidgetItem* parent) {
        controller_->moveNodeBelow(itemId(*dropped), itemId(*parent));
    });
    connect(tree_, &DeselectableTreeWidget::doubleClicked, this, [this](QTreeWidgetItem* item) {
        if (model_->nodeType(itemId(*item)) == Node::Type::folder) {
            item->setExpanded(!item->isExpanded());
        }
    });
    
    applyThemeWithFontOverride(settings_.theme());
    
    if (settings_.restoreSession())
        restoreSession();
}
int View::fontPixelSize(const QFont& font) {
    int pixelSize = font.pixelSize();
    if (pixelSize == -1) {
        const int dpiX = qApp->desktop()->logicalDpiX();
        pixelSize = font.pointSize() / 72.0 * dpiX;
    }
    
    return pixelSize;
}

QString View::fontToStyleSheet(const QFont& font) {
    const int pixelSize = fontPixelSize(font);

    QString style = QString("QWidget {\n")
        + "    font-family: " + font.family() + ";\n"
        + "    font-size: " + QString::fromStdString(std::to_string(pixelSize)) + "px;\n"
        + "    font-weight: " + (font.bold() ? "bold;" : "normal") + ";\n"
        + "}\n";

    return style;
}
std::pair<int, QWidget*> View::nodeTab(int id) {
    for (int i = 0; i < tabWidget_->count(); ++i) {
        QWidget* w = tabWidget_->widget(i);
        if (w->property("id").toInt() == id)
            return std::make_pair(i, w);
    }
    return std::make_pair(0, nullptr);
}
void View::focusNodeTab(int itemId) {
    auto w = nodeTab(itemId);
    if (w.second) {
        tabWidget_->setCurrentIndex(w.first);
        w.second->setFocus();
    }
}

void View::openNode(int id) {
    openNodeInNewTab(id);
    //focusNodeTab(id);
}
void View::closeNode(int id) {
    auto w = nodeTab(id);
    if (w.second) {
        tabWidget_->removeTab(w.first);
        delete w.second;
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
    // No need to rename tree node here, because renaming can be triggered only from view
    //findTreeNode(id)->setText(0, model_->nodeName(id).c_str());

    // Need to rename tab if such exists
    if (model_->nodeType(id) == Node::Type::text) {
        auto w = nodeTab(id);
        if (w.second)
            tabWidget_->setTabText(w.first, model_->nodeName(id).c_str());
    }
}
void View::loadFailed() {
    QMessageBox::critical(this, "Load failed",
        "File content invalid. Loading aborted");
}

void View::filePasswordNeeded(const std::string& fileName) {
    PasswordDialog d;
    if (d.exec() == QDialog::Accepted)
        controller_->load(fileName, std::make_optional<std::string>(d.password()));
};

void View::collectExpandedNodes(std::vector<int>& nodes, QTreeWidgetItem* root) const {
    if (root->isExpanded())
        nodes.push_back(itemId(*root));
    
    for (int i = 0; i < root->childCount(); ++i) {
        QTreeWidgetItem* n = root->child(i);
        collectExpandedNodes(nodes, n);
    }
}
std::vector<Session::TabSession> View::collectOpenedTabs() const {
    std::vector<Session::TabSession> res;
    for (int i = 0; i < tabWidget_->count(); ++i) {
        Session::TabSession tabSession;
        const auto container = static_cast<EditContainer*>(tabWidget_->widget(i));
        tabSession.id_ = container->id();
        tabSession.splitterPos_ = container->splitterPos();
        tabSession.upperCursorPos_ = container->upperCursorPos().get();
        tabSession.upperVScroll_ = container->upperVScrollPos().get();
        tabSession.upperHScroll_ = container->upperHScrollPos().get();

        tabSession.lowerCursorPos_ = container->lowerCursorPos().get();
        tabSession.lowerVScroll_ = container->lowerVScrollPos().get();
        tabSession.lowerHScroll_ = container->lowerHScrollPos().get();

        res.push_back(std::move(tabSession));
    }
    return res;
}
void View::applyTabSession(const Session::TabSession& tabSession) {
    auto p = nodeTab(tabSession.id_);
    if (!p.second) //tab not found
        return;
    const auto container = static_cast<EditContainer*>(tabWidget_->widget(p.first));

    container->setSplitterPos(tabSession.splitterPos_);
    container->setUpperCursorPos(CursorPos(tabSession.upperCursorPos_));
    container->setUpperVScrollPos(ScrollPos(tabSession.upperVScroll_));
    container->setUpperHScrollPos(ScrollPos(tabSession.upperHScroll_));

    container->setLowerCursorPos(CursorPos(tabSession.lowerCursorPos_));
    container->setLowerVScrollPos(ScrollPos(tabSession.lowerVScroll_));
    container->setLowerHScrollPos(ScrollPos(tabSession.lowerHScroll_));
}