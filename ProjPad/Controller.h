#pragma once
#include "Project.h"
#include "View.h"
#include "ControllerInterface.h"

class Controller : public ControllerInterface {
public:
    Controller();
    void setupView(Ui::ProjPadClass* const ui) const override;
    void treeSelectionChanged(int itemId) override;
    void addFolder(const std::string& name) override;


    //void onTreeItemActivated(QTreeWidgetItem* item, int col);
    //void onTextChanged();
    void load(const std::string& fileName) override;
    void save(const std::string& fileName) override;
    void tabTextChanged(int id, const std::string& text) override;
private:
    ModelInterface* model_;
    ViewInterface* view_;

    int curDocId_;
};

