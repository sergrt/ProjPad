#pragma once
class Observer;

class ModelInterface {
public:
    virtual void load(const std::string& fileName) = 0;
    virtual void save(const std::string& fileName) const = 0;
    virtual void save() const = 0;

    virtual Node::Type projectItemType(int id) const = 0;
    virtual bool textNodeOpened(int id) const = 0;
    virtual void openTextNode(int id) = 0;
    virtual void focusTextNode(int id) = 0;

    virtual std::string nodeName(int id) const = 0;
    virtual std::string nodeText(int id) const = 0;
    
    // iterator for root nodes
    virtual std::vector<std::shared_ptr<Node>>::iterator begin() = 0;
    virtual std::vector<std::shared_ptr<Node>>::iterator end() = 0;

    virtual void setNodeText(int id, const std::string& text) = 0;

    virtual void addFolder(const std::string& name) = 0;

    virtual void addObserver(Observer* view) = 0;
    virtual void removeObserver(Observer* view) = 0;
};
