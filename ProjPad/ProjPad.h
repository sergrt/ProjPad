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
    Controller controller_;
    void paintEvent(QPaintEvent* event) override;
    void moveEvent(QMoveEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void changeEvent(QEvent* event) override;
};
