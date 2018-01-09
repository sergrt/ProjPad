#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ProjPad.h"
#include "Project.h"
#include "Controller.h"
#include "View.h"

class ProjPad : public QMainWindow {
    Q_OBJECT

public:
    ProjPad(QWidget *parent = Q_NULLPTR);

private:
    Ui::ProjPadClass ui;
    Project project_;
    View view_;
    Controller controller_;
};