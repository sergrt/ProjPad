#include "stdafx.h"
#include "EditContainer.h"
#include <QGridLayout>
#include <QScrollBar>

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
    layout()->setMargin(2);

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
QList<int> EditContainer::splitterPos() const {
    return splitter_->sizes();
}
ScrollPos EditContainer::upperVScrollPos() const {
    return ScrollPos(upperEdit_->verticalScrollBar()->value());
}
ScrollPos EditContainer::upperHScrollPos() const {
    return ScrollPos(upperEdit_->horizontalScrollBar()->value());
}
CursorPos EditContainer::upperCursorPos() const {
    return CursorPos(upperEdit_->textCursor().position());
}
ScrollPos EditContainer::lowerVScrollPos() const {
    return ScrollPos(lowerEdit_->verticalScrollBar()->value());
}
ScrollPos EditContainer::lowerHScrollPos() const {
    return ScrollPos(lowerEdit_->horizontalScrollBar()->value());
}
CursorPos EditContainer::lowerCursorPos() const {
    return CursorPos(lowerEdit_->textCursor().position());
}

void EditContainer::setSplitterPos(const QList<int>& pos) {
    splitter_->setSizes(pos);
}
void EditContainer::setUpperVScrollPos(ScrollPos pos) {
    upperEdit_->verticalScrollBar()->setValue(pos.get());
}
void EditContainer::setUpperHScrollPos(ScrollPos pos) {
    upperEdit_->horizontalScrollBar()->setValue(pos.get());
}
void EditContainer::setUpperCursorPos(CursorPos pos) {
    QTextCursor textCursor = upperEdit_->textCursor();
    textCursor.setPosition(pos.get());
    upperEdit_->setTextCursor(textCursor);
}
void EditContainer::setLowerVScrollPos(ScrollPos pos) {
    lowerEdit_->verticalScrollBar()->setValue(pos.get());
}
void EditContainer::setLowerHScrollPos(ScrollPos pos) {
    lowerEdit_->horizontalScrollBar()->setValue(pos.get());
}
void EditContainer::setLowerCursorPos(CursorPos pos) {
    QTextCursor textCursor = lowerEdit_->textCursor();
    textCursor.setPosition(pos.get());
    lowerEdit_->setTextCursor(textCursor);
}