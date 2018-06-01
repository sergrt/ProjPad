#pragma once
#include <optional>
#include <string>

class Observer {
public:
    virtual void updateTree() = 0;
    //virtual void updateText(int id) = 0;
    virtual void openNode(int id) = 0;
    virtual void closeNode(int id) = 0;
    virtual void nodeDeleted(int id) = 0;
    virtual void nodeAdded(int id, std::optional<int> parentId) = 0;
    virtual void nodeRenamed(int id) = 0;
    virtual void loadFailed() = 0;
    virtual void filePasswordNeeded(const std::string& fileName) = 0;
};
