#pragma once
namespace Ui {
    class ProjPadClass;
}

class ControllerInterface {
public:
    virtual void setupView(Ui::ProjPadClass* const) const = 0;
    virtual void treeSelectionChanged(int itemId) const = 0;
    virtual void load(const std::string& fileName) const = 0;
    virtual void save(const std::string& fileName) const = 0;
    virtual void save() const = 0;
    virtual void tabTextChanged(int id, const std::string& text) const = 0;
    virtual void addFolder(const std::string& name) const = 0;
    virtual void deleteNode(int itemId) const = 0;
};
