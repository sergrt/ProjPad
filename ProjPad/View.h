#pragma once
#include "Project.h"
#include "Observer.h"
#include <QTreeWidget>
#include "ControllerInterface.h"
#include "ViewInterface.h"
#include "Settings.h"


class View : public QWidget, public ViewInterface, public Observer {
    Q_OBJECT

public:
    View(ModelInterface* model, ControllerInterface* controller);
    ~View();
    void setupView(Ui::ProjPadClass* const ui) override;
    void focusNodeTab(int itemId) override;
    void openNode(int id) override;
    void closeNode(int id) override;
    void nodeDeleted(int id) override;
    void openNodeInNewTab(int id);
    void updateTree() override;

    void enableSave() override;
    void nodeAdded(int id, std::optional<int> parentId) override;
    void nodeRenamed(int id) override;
    
    //void updateText(int id) override;

private:
    ModelInterface* model_;
    ControllerInterface* controller_;

    QTreeWidget* tree_;
    QTabWidget* tabWidget_;
    QAction* save_;

    Settings settings_;

    void fillTree(const Node* node, QTreeWidgetItem* item);
    static void setIcon(Node::Type type, QTreeWidgetItem* item);
    QTreeWidgetItem* findTreeNode(int id) const;
    QTreeWidgetItem* findTreeNode(QTreeWidgetItem* cur, int id) const;
    static int itemId(QTreeWidgetItem& item);
    static void setItemId(QTreeWidgetItem& item, int id);
    QTreeWidgetItem* createTreeItem(int id);
    std::pair<int, QWidget*> nodeTab(int id);
    void fillThemesMenu(QMenu& menu);
    void applyStyleSheetWithFontOverride(const QString& styleSheet);
    void applyThemeWithFontOverride(const QString& themeName);
    static QString fontToStyleSheet(const QFont& font);
    static int fontPixelSize(const QFont& font);

    


public slots:
    void tabTextChanged(int id, const std::string& text);
};

