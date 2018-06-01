#pragma once
#include <QDialog>
#include "ui_PasswordDialog.h"

class PasswordDialog : public QDialog {
    Q_OBJECT

public:
    PasswordDialog(QWidget *parent = Q_NULLPTR);
    ~PasswordDialog();
    std::string password() const;
private:
    Ui::PasswordDialog ui;
};
