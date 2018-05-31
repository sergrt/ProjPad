#include "stdafx.h"
#include "ProjPad.h"
#include <mutex>
std::once_flag flagSetupView;

ProjPad::ProjPad(QWidget *parent)
    : QMainWindow(parent) {
    ui.setupUi(this);
    // Set view components
    
}

void ProjPad::paintEvent(QPaintEvent* event) {
    std::call_once(flagSetupView, [this]() {
        controller_.setupView(&ui);
    });
}