#include "stdafx.h"
#include "ProjPad.h"

ProjPad::ProjPad(QWidget *parent)
    : QMainWindow(parent)
    , view_{ &project_ }
    , controller_{ &project_, &view_ } {
    ui.setupUi(this);

    connect(ui.treeWidget, &QTreeWidget::itemActivated, &controller_, &Controller::onTreeItemActivated);
    //connect(ui.textEdit, &QTextEdit::textChanged, &controller_, &Controller::onTextChanged);
    connect(ui.actionOpen, &QAction::triggered, &controller_, &Controller::onLoadClick);
    
    ui.splitter->setSizes({ 200, 999 });

    view_.setProjectTree(ui.treeWidget);
    view_.setTabsContainer(ui.tabWidget);
}