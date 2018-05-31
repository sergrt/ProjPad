#pragma once
class QTabWidget;
class QTreeWidget;

class ViewInterface {
public:
    virtual ~ViewInterface() {};
    virtual void setupView(Ui::ProjPadClass* const) = 0;
    virtual void focusNodeTab(int itemId) = 0;

    virtual void enableSave() = 0;
    //virtual void disableSave() = 0;
};
