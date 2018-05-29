#include "stdafx.h"
#include "EditContainer.h"
#include <QGridLayout>

EditContainer::EditContainer(int id)
    : attached_{ false } {
    setProperty("id", QVariant::fromValue(id));
    setLayout(new QGridLayout());

    upperEdit_ = new QTextEdit;
    lowerEdit_ = new QTextEdit;

    splitter_ = new QSplitter();
    splitter_->setOrientation(Qt::Vertical);
    splitter_->addWidget(upperEdit_);
    splitter_->addWidget(lowerEdit_);
    splitter_->setSizes({ 0,10 });

    layout()->addWidget(splitter_);

    connect(upperEdit_, &QTextEdit::textChanged, this, &EditContainer::upperTextChanged);
    connect(lowerEdit_, &QTextEdit::textChanged, this, &EditContainer::lowerTextChanged);
}
EditContainer::~EditContainer() {
    delete splitter_; // splitter will delete its children automatically
}
void EditContainer::setText(const std::string& text) const {
    const QString qtext = QString::fromStdString(text);
    upperEdit_->setText(qtext);
    // changing of upperEdit_ will cause changing of lower text automatically, so no need to call it explicitly
    //lowerEdit_->setText(qtext);
}
void EditContainer::upperTextChanged() {
    syncTexts(upperEdit_, lowerEdit_);
    emit textChanged(property("id").toInt(), upperEdit_->toPlainText().toStdString());
}
void EditContainer::lowerTextChanged() {
    syncTexts(lowerEdit_, upperEdit_);
    emit textChanged(property("id").toInt(), lowerEdit_->toPlainText().toStdString());
}
void EditContainer::syncTexts(QTextEdit* initiator, QTextEdit* receiver) const {
    receiver->blockSignals(true); // prevent circular chainging
    const auto s = storeScrollPosAndCursorPos(receiver);
    receiver->setText(initiator->toPlainText());
    restoreScrollPosAndCursorPos(receiver, s);

    receiver->blockSignals(false);
}
std::pair<CursorPos, ScrollPos> EditContainer::storeScrollPosAndCursorPos(QTextEdit* w) const {
    const QTextCursor receiverCursorOld = w->textCursor();
    const int cursorPos = receiverCursorOld.position();
    const int scrollPos = w->verticalScrollBar()->value();

    return std::make_pair(CursorPos(cursorPos), ScrollPos(scrollPos));
}
void EditContainer::restoreScrollPosAndCursorPos(QTextEdit* w, std::pair<CursorPos, ScrollPos> pos) const {
    QTextCursor receiverCursorNew = w->textCursor();
    receiverCursorNew.setPosition(pos.first.get());
    w->setTextCursor(receiverCursorNew);
    w->verticalScrollBar()->setValue(pos.second.get());
}
int EditContainer::id() const {
    return property("id").toInt();
}