#pragma once

#include <QDialog>
#include "ui_SetPasswordDialog.h"
#include <optional>

class SetPasswordDialog : public QDialog {
    Q_OBJECT

public:
    SetPasswordDialog(const std::string& curPassword, QWidget *parent = Q_NULLPTR);
    ~SetPasswordDialog();

    std::string selectedPassword() const;
private:
    std::string curPassword_;
    std::optional<std::string> selectedPassword_;
    Ui::SetPasswordDialog ui;
};
