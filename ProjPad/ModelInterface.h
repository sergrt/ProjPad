#pragma once
#include <optional>
class Observer;

class ModelInterface {
public:
    virtual ~ModelInterface() {}
    virtual void load(const std::string& fileName, const std::optional<std::string>& password) = 0;
    virtual void save(const std::string& fileName) = 0;
    virtual void save() = 0;
    virtual std::optional<std::string> password() const = 0;

    virtual bool textNodeOpened(int id) const = 0;
    virtual void openTextNode(int id) = 0;
    virtual void focusTextNode(int id) = 0;
    virtual void closeNode(int id) = 0;
    virtual void setPassword(const std::string& password) = 0;
    virtual void moveNode(int itemId, std::optional<int> parentId) = 0;
    virtual void moveNodeAbove(int itemId, int parentId) = 0;
    virtual void moveNodeBelow(int itemId, int parentId) = 0;

    virtual std::string nodeName(int id) const = 0;
    virtual std::string nodeText(int id) const = 0;
    virtual Node::Type nodeType(int id) const = 0;
    
    // iterator for root nodes
    virtual std::vector<std::unique_ptr<Node>>::iterator begin() = 0;
    virtual std::vector<std::unique_ptr<Node>>::iterator end() = 0;

    virtual void setNodeText(int id, const std::string& text) = 0;

    virtual void addFolder(const std::string& name, std::optional<int> parentId) = 0;
    virtual void addText(const std::string& name, std::optional<int> parentId) = 0;
    virtual void deleteNode(int id) = 0;
    virtual void renameNode(int id, const std::string& name) = 0;

    virtual void addObserver(Observer* view) = 0;
    virtual void removeObserver(Observer* view) = 0;
};
