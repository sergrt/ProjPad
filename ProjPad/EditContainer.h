#pragma once
#include "NamedType.h"
#include <qframe.h>
#include <QSplitter>
#include <QTextEdit>

using CursorPos = NamedType<int, struct CursorPosParameter>;
using ScrollPos = NamedType<int, struct ScrollPosParameter>;

class EditContainer : public QFrame {
    Q_OBJECT

public:
    EditContainer(int id);
    ~EditContainer();
    void setText(const std::string& text) const;
    int id() const;
    QList<int> splitterPos() const;
    ScrollPos upperVScrollPos() const;
    ScrollPos upperHScrollPos() const;
    CursorPos upperCursorPos() const;
    ScrollPos lowerVScrollPos() const;
    ScrollPos lowerHScrollPos() const;
    CursorPos lowerCursorPos() const;

    void setSplitterPos(const QList<int>& pos);
    void setUpperVScrollPos(ScrollPos pos);
    void setUpperHScrollPos(ScrollPos pos);
    void setUpperCursorPos(CursorPos pos);
    void setLowerVScrollPos(ScrollPos pos);
    void setLowerHScrollPos(ScrollPos pos);
    void setLowerCursorPos(CursorPos pos);
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
signals:
    void textChanged(int id, const std::string& text);
};
