#pragma once
#include <string>
#include <QPoint>
#include <QSize>
#include <vector>
#include <QList>

class Session {
public:
    struct TabSession {
        int id_ = 0;
        QList<int> splitterPos_;
        int upperVScroll_ = 0;
        int upperHScroll_ = 0;
        int upperCursorPos_ = 0;

        int lowerVScroll_ = 0;
        int lowerHScroll_ = 0;
        int lowerCursorPos_ = 0;
        operator QString() const {
            QString listString = "[";
            for (const auto i : splitterPos_)
                listString += QString("%1").arg(i) + ",";
            if (listString.size() > 1)
                listString.remove(listString.size() - 1, 1);
            listString += "]";

            QString res = QString("{%1,%2,%3,%4,%5,%6,%7,%8}")
                .arg(id_)
                .arg(listString)
                .arg(upperVScroll_)
                .arg(upperHScroll_)
                .arg(upperCursorPos_)
                .arg(lowerVScroll_)
                .arg(lowerHScroll_)
                .arg(lowerCursorPos_);

            return res;
        }
        static TabSession fromString(QString str) {
            if (str.size() < 1 + 1 + 1 + 7 + 1
                || str[0] != '{'
                || str[str.size() - 1] != '}')
                throw std::runtime_error("Invalid argument");
            
            str.remove(0, 1);
            str.remove(str.size() - 1, 1);

            TabSession res;

            const int listPartStart = str.indexOf('[');
            const int listPartLength = str.indexOf(']') - str.indexOf('[') + 1;
            {
                QString listPart = str.mid(listPartStart, listPartLength);
                if (listPart.size() < 2)
                    throw std::runtime_error("Invalid argument");

                listPart.remove(0, 1);
                listPart.remove(listPart.size() - 1, 1);

                auto p = listPart.split(",");
                for (const auto& s : p)
                    res.splitterPos_.push_back(s.toInt());
            }
            str.remove(listPartStart, listPartLength);

            auto parts = str.split(',');
            if (parts.size() != 8)
                throw std::runtime_error("Invalid argument");

            res.id_ = parts[0].toInt();
            res.upperVScroll_ = parts[2].toInt();
            res.upperHScroll_ = parts[3].toInt();
            res.upperCursorPos_ = parts[4].toInt();
            res.lowerVScroll_ = parts[5].toInt();
            res.lowerHScroll_ = parts[6].toInt();
            res.lowerCursorPos_ = parts[7].toInt();

            return res;
        }
    };

    Session();
    ~Session();
    std::string fileName() const;
    QPoint windowPos() const;
    QSize windowSize() const;
    bool windowMaximized() const;
    std::vector<int> expandedNodes() const;
    int selectedNode() const;
    int treeVScrollPos() const;
    int treeHScrollPos() const;
    std::vector<TabSession> openedTabs() const;
    int activeTab() const;

    void setFileName(const std::string& fileName);
    void setWindowPos(const QPoint& pos);
    void setWindowSize(const QSize& size);
    void setWindowMaximized(bool maximized);
    void setExpandedNodes(const std::vector<int>& expandedNodes);
    void setSelectedNode(int id);
    void setTreeVScrollPos(int pos);
    void setTreeHScrollPos(int pos);
    void setOpenedTabs(const std::vector<TabSession>& openedTabs);
    void setActiveTab(int id);

private:
    void load();
    void save() const;
    std::string fileName_;
    QPoint windowPos_;
    QSize windowSize_;
    bool windowMaximized_;
    std::vector<int> expandedNodes_;
    int selectedNode_;
    int treeVScrollPos_;
    int treeHScrollPos_;
    std::vector<TabSession> openedTabs_;
    int activeTab_;
};
