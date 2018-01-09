#pragma once
#include "Project.h"
#include "View.h"

class Controller : public QObject {
    Q_OBJECT

public:
    Controller(Project* model, View* view);
    void onTreeItemActivated(QTreeWidgetItem* item, int col);
    //void onTextChanged();
    void onLoadClick();
private:
    Project* model_;
    View* view_;

    int curDocId_;
};

