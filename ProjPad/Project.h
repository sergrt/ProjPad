#pragma once
#include "Node.h"
#include <string>
#include <vector>
#include "ModelInterface.h"
#include <optional>

namespace pugi {
    class xml_node;
}

class Observer;

class Project : public ModelInterface {
public:
    Project();
    
    bool textNodeOpened(int id) const override;
    void openTextNode(int id) override;
    void focusTextNode(int id) override;

    std::vector<Node*> openedNodes_; // rootNodes_ shared ptr will surely outlive this Node*
    void notifyNodeOpened(int id);
    void notifyNodeClosed(int id);

    std::string nodeName(int id) const override;
    std::string nodeText(int id) const override;
    Node::Type nodeType(int id) const override;

    std::vector<std::unique_ptr<Node>>::iterator begin() override;
    std::vector<std::unique_ptr<Node>>::iterator end() override;
    
    void setNodeText(int id, const std::string& text) override;
    void addFolder(const std::string& name, std::optional<int> parentId) override;
    void addText(const std::string& name, std::optional<int> parentId) override;

    void addObserver(Observer* view) override;
    void removeObserver(Observer* view) override;
//private:
    std::vector<std::unique_ptr<Node>> rootNodes_;
    void load(const std::string& fileName, const std::optional<std::string>& password) override;
    void save(const std::string& fileName) override;
    void save() override;
    std::optional<std::string> password() const override;
    void setPassword(const std::string& password) override;

    static void saveNode(const Node& node, pugi::xml_node& parentXmlNode);
    static std::unique_ptr<Node> loadNode(const pugi::xml_node& xmlNode);

    void deleteNode(int id) override;
    void renameNode(int id, const std::string& name) override;
    void closeWithChildrenNodes(Node* node);
    void closeNode(int id) override;
private:
    std::vector<Observer*> views_;
    //void notifyTextChanged(int id);
    void notifyTreeChanged();
    void notifyItemDeleted(int id);
    void notifyNodeAdded(int id, std::optional<int> parentId);
    void notifyNodeRenamed(int id);
    void notifyLoadFailed();
    void notifyPasswordNeeded(const std::string& fileName);

    Node* findById(int id) const;
    Node* findById(Node& node, int id) const;

    Node* findParentForId(int id) const;
    Node* findParentForId(Node& node, int id) const;

    void addNode(Node::Type type, const std::string& name, std::optional<int> parentId);

    std::optional<std::string> fileName_;
    std::optional<std::string> password_;
};
