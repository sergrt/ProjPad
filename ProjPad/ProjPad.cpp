#include "stdafx.h"
#include "ProjPad.h"
#include <mutex>
std::once_flag flagSetupView;
bool applicationInitialized = false;

ProjPad::ProjPad(QWidget *parent)
    : QMainWindow(parent) {
    ui.setupUi(this);
    // Set view components
    
}

void ProjPad::paintEvent(QPaintEvent* event) {
    std::call_once(flagSetupView, [this]() {
        controller_.setupView(&ui);
        applicationInitialized = true;
    });
}
void ProjPad::moveEvent(QMoveEvent* event) {
    if (applicationInitialized)
        controller_.updateViewSettings(std::make_optional<QPoint>(this->pos()), std::nullopt, std::nullopt);
    QMainWindow::moveEvent(event);
}
void ProjPad::resizeEvent(QResizeEvent* event) {
    if (applicationInitialized)
        controller_.updateViewSettings(std::nullopt, std::make_optional<QSize>(this->size()), std::nullopt);
    QMainWindow::resizeEvent(event);
}
void ProjPad::changeEvent(QEvent* event) {
    if (applicationInitialized && event->type() == QEvent::WindowStateChange) {
        const QWindowStateChangeEvent* changeEvent = static_cast<QWindowStateChangeEvent*>(event);
        
        if (this->windowState() == Qt::WindowNoState)
            controller_.updateViewSettings(std::nullopt, std::nullopt, false);
        else if (changeEvent->oldState() == Qt::WindowNoState && this->windowState() == Qt::WindowMaximized)
            controller_.updateViewSettings(std::nullopt, std::nullopt, true);
    }
    QMainWindow::changeEvent(event);
}
void ProjPad::closeEvent(QCloseEvent* event) {
    if (!controller_.handleUnsavedProject())
        event->ignore();
    else
        event->accept();
}