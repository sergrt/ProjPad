#pragma once
#include "NamedType.h"

using CursorPos = NamedType<int, struct CursorPosParameter>;
using ScrollPos = NamedType<int, struct ScrollPosParameter>;

class EditContainer : public QFrame {
    Q_OBJECT

public:
    EditContainer(int id);
    ~EditContainer();
    void setText(const std::string& text) const;
private:
    bool attached_;
    QTextEdit* upperEdit_;
    QTextEdit* lowerEdit_;
    QSplitter* splitter_;
public slots:
    void upperTextChanged();
    void lowerTextChanged();
private:
    void syncTexts(QTextEdit* initiator, QTextEdit* receiver) const;
    std::pair<CursorPos, ScrollPos> storeScrollPosAndCursorPos(QTextEdit* w) const;
    void restoreScrollPosAndCursorPos(QTextEdit* w, std::pair<CursorPos, ScrollPos> pos) const;
};
