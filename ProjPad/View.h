#pragma once
#include "Project.h"
#include "Observer.h"

class View : public Observer {
public:
    View(Project* model);
    ~View();
    void render();
    void setProjectTree(QTreeWidget* tree);
    void setTabsContainer(QTabWidget* tabWidget);
    void updateContent(int id);
    std::string getCurrentText() const;


    virtual void updateTree() override;
    virtual void updateText(int id) override;

    // From controller
    void treeSelectionChanged(int id);
private:
    Project* model_;
    QTreeWidget* tree_;
    QTabWidget* tabWidget_;

    void fillTree(std::shared_ptr<Node> node, QTreeWidgetItem* item);

    bool textNodeOpened(int id) const;
    void openTextNode(int id);
    void focusTextNode(int id);
    QTextEdit* widgetForTextNode(int id);

    void setIcon(Node::Type type, QTreeWidgetItem* item);
};

