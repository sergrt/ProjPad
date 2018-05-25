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
    
    Node::Type projectItemType(int id) const override;
    bool textNodeOpened(int id) const override;
    void openTextNode(int id) override;
    void focusTextNode(int id) override;

    std::vector<Node*> openedNodes_; // rootNodes_ shared ptr will surely outlive this Node*
    void notifyNodeOpened(int id);
    void notifyNodeClosed(int id);

    std::string nodeName(int id) const override;
    std::string nodeText(int id) const override;


    std::vector<std::unique_ptr<Node>>::iterator begin() override;
    std::vector<std::unique_ptr<Node>>::iterator end() override;
    
    void setNodeText(int id, const std::string& text) override;
    void addFolder(const std::string& name) override;

    void addObserver(Observer* view) override;
    void removeObserver(Observer* view) override;
//private:
    std::vector<std::unique_ptr<Node>> rootNodes_;
    void load(const std::string& fileName) override;
    void save(const std::string& fileName) override;
    void save() override;

    static void saveNode(const Node& node, pugi::xml_node& parentXmlNode);
    static std::unique_ptr<Node> loadNode(const pugi::xml_node& xmlNode);

    void deleteNode(int id) override;
    void closeWithChildrenNodes(Node* node);
private:
    std::vector<Observer*> views_;
    //void notifyTextChanged(int id);
    void notifyTreeChanged();

    Node* findById(int id) const;
    Node* findById(Node& node, int id) const;

    Node* findParentForId(int id) const;
    Node* findParentForId(Node& node, int id) const;

    std::optional<std::string> fileName_;
};
