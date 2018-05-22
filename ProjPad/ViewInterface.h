#pragma once
class QTabWidget;
class QTreeWidget;

class ViewInterface {
public:
    virtual void setupView(Ui::ProjPadClass* const) = 0;
    virtual void focusNodeTab(int itemId) = 0;
};
