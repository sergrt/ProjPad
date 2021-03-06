#pragma once
namespace Ui {
    class ProjPadClass;
}

class ControllerInterface {
public:
    virtual ~ControllerInterface() {}
    virtual void setupView(Ui::ProjPadClass* const) const = 0;
    virtual void treeSelectionChanged(int itemId) const = 0;
    virtual void load(const std::string& fileName, const std::optional<std::string>& password) const = 0;
    virtual void save(const std::string& fileName) const = 0;
    virtual void save() const = 0;
    virtual void tabTextChanged(int id, const std::string& text) const = 0;
    virtual void addFolder(const std::string& name, std::optional<int> parentId) const = 0;
    virtual void addText(const std::string& name, std::optional<int> parentId) const = 0;
    virtual void deleteNode(int itemId) const = 0;
    virtual void renameNode(int itemId, const std::string& name) const = 0;
    virtual void closeNode(int itemId) const = 0;
    virtual void setPassword(const std::string& password) const = 0;
    virtual void moveNode(int itemId, std::optional<int> parentId) const = 0;
    virtual void moveNodeAbove(int itemId, int parentId) const = 0;
    virtual void moveNodeBelow(int itemId, int parentId) const = 0;
    virtual void moveNodeAfterAll(int itemId) const = 0;
    virtual void exportProject(const std::string& fileName) const = 0;
    virtual void createNewProject() = 0;
};
