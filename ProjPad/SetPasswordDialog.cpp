#include "stdafx.h"
#include "SetPasswordDialog.h"
#include <QMessageBox>

SetPasswordDialog::SetPasswordDialog(const std::string& curPassword, QWidget *parent)
    : QDialog(parent) {
    ui.setupUi(this);

    connect(ui.ok, &QPushButton::clicked, this, [this]() {
        if (ui.curPassword->text().toStdString() != curPassword_) {
            QMessageBox::critical(this, "Invalid password", "Current password incorrect");
        } else {
            if (ui.password->text() != ui.confirmation->text()) {
                QMessageBox::critical(this, "Passwords mismatch", "Passwords you have entered are mismatch");
            } else {
                selectedPassword_ = ui.password->text().toStdString();
                accept();
            }
        }
    });
}

SetPasswordDialog::~SetPasswordDialog() {
}

std::string SetPasswordDialog::selectedPassword() const {
    if (!selectedPassword_)
        throw std::runtime_error("Trying to obtain unset password");
    
    return *selectedPassword_;
}
