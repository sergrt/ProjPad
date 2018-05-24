#pragma once
namespace Ui {
    class ProjPadClass;
}

class ControllerInterface {
public:
    virtual void setupView(Ui::ProjPadClass* const) const = 0;
    virtual void treeSelectionChanged(int itemId) = 0;
    virtual void load(const std::string& fileName) = 0;
    virtual void save(const std::string& fileName) = 0;
    virtual void tabTextChanged(int id, const std::string& text) = 0;
    virtual void addFolder(const std::string& name) = 0;
};
