#include "stdafx.h"
#include "Session.h"
#include <QSettings>

namespace SessionFile {
    const QString fileName = "session.ini";
    const QString delimiter = ";";
    namespace Project {
        const QString sectionName = "Project";

        const QString fileName = "FileName";
    }
    namespace Window {
        const QString sectionName = "Window";

        const QString posX = "PositionX";
        const QString posY = "PositionY";
        const QString sizeX = "SizeX";
        const QString sizeY = "SizeY";
        const QString isMaximized = "IsMaximized";
    }
    namespace ProjectTree {
        const QString sectionName = "ProjectTree";

        const QString expandedNodes = "ExpandedNodes";
        const QString selectedNode = "SelectedNode";
        const QString vScrollPos = "VScrollPos";
        const QString hScrollPos = "HScrollPos";
    }
    namespace TabsSession {
        const QString sectionName = "TabsSession";

        const QString openedTabs = "OpenedTabs";
        const QString activeTab = "ActiveTab";
    }

}

Session::Session() {
    load();
}
Session::~Session() {
    save();
}
void Session::load() {
    QSettings s(SessionFile::fileName, QSettings::IniFormat);
    
    s.beginGroup(SessionFile::Project::sectionName);
    fileName_ = s.value(SessionFile::Project::fileName).toString().toStdString();
    s.endGroup();

    s.beginGroup(SessionFile::Window::sectionName);
    windowPos_ = QPoint(s.value(SessionFile::Window::posX, 0).toInt(),
        s.value(SessionFile::Window::posY, 0).toInt());
    windowSize_ = QSize(s.value(SessionFile::Window::sizeX, 1024).toInt(),
        s.value(SessionFile::Window::sizeY, 768).toInt());
    windowMaximized_ = s.value(SessionFile::Window::isMaximized, false).toBool();
    s.endGroup();

    s.beginGroup(SessionFile::ProjectTree::sectionName);
    QString expandedNodesString = s.value(SessionFile::ProjectTree::expandedNodes, "").toString();
    auto nodes = expandedNodesString.split(SessionFile::delimiter);
    expandedNodes_.clear();
    expandedNodes_.reserve(nodes.size());
    for (const auto& n : nodes)
        expandedNodes_.push_back(n.toInt());
        
    selectedNode_ = s.value(SessionFile::ProjectTree::selectedNode, 0).toInt();
    treeVScrollPos_ = s.value(SessionFile::ProjectTree::vScrollPos, 0).toInt();
    treeHScrollPos_ = s.value(SessionFile::ProjectTree::hScrollPos, 0).toInt();
    s.endGroup();

    s.beginGroup(SessionFile::TabsSession::sectionName);
    QString openedTabsString = s.value(SessionFile::TabsSession::openedTabs).toString();
    auto parts = openedTabsString.split(SessionFile::delimiter, QString::SplitBehavior::SkipEmptyParts);
    openedTabs_.clear();
    for (const auto& p : parts)
        openedTabs_.push_back(TabSession::fromString(p));

    activeTab_ = s.value(SessionFile::TabsSession::activeTab).toInt();
    s.endGroup();
}
void Session::save() const {
    QSettings s(SessionFile::fileName, QSettings::IniFormat);

    s.beginGroup(SessionFile::Project::sectionName);
    s.setValue(SessionFile::Project::fileName, QString::fromStdString(fileName()));
    s.endGroup();

    s.beginGroup(SessionFile::Window::sectionName);
    s.setValue(SessionFile::Window::posX, windowPos_.x());
    s.setValue(SessionFile::Window::posY, windowPos_.y());
    s.setValue(SessionFile::Window::sizeX, windowSize_.width());
    s.setValue(SessionFile::Window::sizeY, windowSize_.height());
    s.setValue(SessionFile::Window::isMaximized, windowMaximized_);
    s.endGroup();

    s.beginGroup(SessionFile::ProjectTree::sectionName);
    QString expandedNodesString;
    for (const auto i : expandedNodes_)
        expandedNodesString += QString("%1").arg(i) + SessionFile::delimiter;
    
    if (!expandedNodesString.isEmpty())
        expandedNodesString.remove(expandedNodesString.size() - 1, 1);

    s.setValue(SessionFile::ProjectTree::expandedNodes, expandedNodesString);
    s.setValue(SessionFile::ProjectTree::selectedNode, selectedNode_);
    s.setValue(SessionFile::ProjectTree::vScrollPos, treeVScrollPos_);
    s.setValue(SessionFile::ProjectTree::hScrollPos, treeHScrollPos_);
    s.endGroup();

    s.beginGroup(SessionFile::TabsSession::sectionName);
    QString openedTabsString;

    for (const auto& t : openedTabs_)
        openedTabsString += t + SessionFile::delimiter;
    if (!openedTabsString.isEmpty())
        openedTabsString.remove(openedTabsString.size() - 1, 1);
    
    s.setValue(SessionFile::TabsSession::openedTabs, openedTabsString);
    s.setValue(SessionFile::TabsSession::activeTab, activeTab_);
    s.endGroup();
}
std::string Session::fileName() const {
    return fileName_;
}
QPoint Session::windowPos() const {
    return windowPos_;
}
QSize Session::windowSize() const {
    return windowSize_;
}
bool Session::windowMaximized() const {
    return windowMaximized_;
}
std::vector<int> Session::expandedNodes() const {
    return expandedNodes_;
}
int Session::selectedNode() const {
    return selectedNode_;
}
int Session::treeVScrollPos() const {
    return treeVScrollPos_;
}
int Session::treeHScrollPos() const {
    return treeHScrollPos_;
}
std::vector<Session::TabSession> Session::openedTabs() const {
    return openedTabs_;
}
int Session::activeTab() const {
    return activeTab_;
}

void Session::setFileName(const std::string& fileName) {
    fileName_ = fileName;
}
void Session::setWindowPos(const QPoint& pos) {
    windowPos_ = pos;
}
void Session::setWindowSize(const QSize& size) {
    windowSize_ = size;
}
void Session::setWindowMaximized(bool maximized) {
    windowMaximized_ = maximized;
}
void Session::setExpandedNodes(const std::vector<int>& expandedNodes) {
    expandedNodes_ = expandedNodes;
}
void Session::setSelectedNode(int id) {
    selectedNode_ = id;
}
void Session::setTreeVScrollPos(int pos) {
    treeVScrollPos_ = pos;
}
void Session::setTreeHScrollPos(int pos) {
    treeHScrollPos_ = pos;
}
void Session::setOpenedTabs(const std::vector<TabSession>& openedTabs) {
    openedTabs_ = openedTabs;
}
void Session::setActiveTab(int id) {
    activeTab_ = id;
}
