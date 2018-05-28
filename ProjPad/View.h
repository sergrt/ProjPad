#pragma once
#include "Project.h"
#include "Observer.h"
#include <QTreeWidget>
#include "ControllerInterface.h"
#include "ViewInterface.h"



class View : public QObject, public ViewInterface, public Observer {
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
    
    //void updateText(int id) override;

private:
    ModelInterface* model_;
    ControllerInterface* controller_;

    QTreeWidget* tree_;
    QTabWidget* tabWidget_;
    QAction* save_;

    void fillTree(const Node* node, QTreeWidgetItem* item);
    void setIcon(Node::Type type, QTreeWidgetItem* item);
    QTreeWidgetItem* findTreeNode(QTreeWidgetItem* cur, int id) const;
public slots:
    void tabTextChanged(int id, const std::string& text);
};

