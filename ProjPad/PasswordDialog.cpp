#include "stdafx.h"
#include "PasswordDialog.h"
#include <QPushButton>

PasswordDialog::PasswordDialog(QWidget *parent)
    : QDialog(parent) {
    ui.setupUi(this);
    connect(ui.open, &QPushButton::clicked, this, [this]() {
        accept();
    });
}

PasswordDialog::~PasswordDialog() {
}
std::string PasswordDialog::password() const {
    return ui.password->text().toStdString();
}
