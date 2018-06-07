#pragma once
#include "Project.h"
#include "View.h"
#include "ControllerInterface.h"

class Controller : public ControllerInterface {
public:
    Controller();
    ~Controller();
    void setupView(Ui::ProjPadClass* const ui) const override;
    void treeSelectionChanged(int itemId) const override;
    void addFolder(const std::string& name, std::optional<int> parentId) const override;
    void addText(const std::string& name, std::optional<int> parentId) const override;


    //void onTreeItemActivated(QTreeWidgetItem* item, int col);
    //void onTextChanged();
    void load(const std::string& fileName, const std::optional<std::string>& password) const override;
    void save(const std::string& fileName) const override;
    void save() const override;
    void tabTextChanged(int id, const std::string& text) const override;
    void deleteNode(int itemId) const override;
    void renameNode(int itemId, const std::string& name) const override;
    void closeNode(int itemId) const override;
    void setPassword(const std::string& password) const override;
    void moveNode(int itemId, std::optional<int> parentId) const override;
    void moveNodeAbove(int itemId, int parentId) const override;
    void moveNodeBelow(int itemId, int parentId) const override;

    // This is used when main window moved/resized/maximized-restored - to keep all session settings in one place (View)
    void updateViewSettings(const std::optional<QPoint>& pos, const std::optional<QSize>& size, const std::optional<bool> maximized);
private:
    ModelInterface* model_;
    ViewInterface* view_;

    int curDocId_;
};

