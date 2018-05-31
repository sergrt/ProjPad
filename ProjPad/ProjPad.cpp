#include "stdafx.h"
#include "ProjPad.h"

ProjPad::ProjPad(QWidget *parent)
    : QMainWindow(parent) {
    ui.setupUi(this);
    // Set view components
    
}

#include <mutex>
std::once_flag flag1;
void ProjPad::paintEvent(QPaintEvent* event) {
    std::call_once(flag1, [this]() {
        controller_.setupView(&ui);
    });

}