#pragma once
#include <QPoint>
#include <QSize>
class QTabWidget;
class QTreeWidget;

class ViewInterface {
public:
    virtual ~ViewInterface() {};
    virtual void setupView(Ui::ProjPadClass* const) = 0;
    virtual void focusNodeTab(int itemId) = 0;

    virtual void enableSave() = 0;
    virtual void disableSave() = 0;
    //virtual void disableSave() = 0;
    virtual void updateViewSettings(const std::optional<QPoint>& pos, const std::optional<QSize>& size, const std::optional<bool> maximized) = 0;
    virtual bool handleUnsavedProject() = 0;
    virtual void closeProject() = 0;
    virtual void setModel(ModelInterface* model) = 0;
};
