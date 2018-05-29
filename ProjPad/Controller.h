#pragma once
#include "Project.h"
#include "View.h"
#include "ControllerInterface.h"

class Controller : public ControllerInterface {
public:
    Controller();
    void setupView(Ui::ProjPadClass* const ui) const override;
    void treeSelectionChanged(int itemId) const override;
    void addFolder(const std::string& name, std::optional<int> parentId) const override;
    void addText(const std::string& name, std::optional<int> parentId) const override;


    //void onTreeItemActivated(QTreeWidgetItem* item, int col);
    //void onTextChanged();
    void load(const std::string& fileName) const override;
    void save(const std::string& fileName) const override;
    void save() const override;
    void tabTextChanged(int id, const std::string& text) const override;
    void deleteNode(int itemId) const override;
    void renameNode(int itemId, const std::string& name) const override;
private:
    ModelInterface* model_;
    ViewInterface* view_;

    int curDocId_;
};

