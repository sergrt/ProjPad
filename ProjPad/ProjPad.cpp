#include "stdafx.h"
#include "ProjPad.h"

ProjPad::ProjPad(QWidget *parent)
    : QMainWindow(parent) {
    ui.setupUi(this);
    // Set view components
    controller_.setupView(&ui);
}