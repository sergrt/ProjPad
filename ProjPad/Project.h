#pragma once
#include "Node.h"
#include <string>
#include <vector>
#include "ModelInterface.h"
class Observer;

class Project : public ModelInterface {
public:
    Project();
    
    Node::Type projectItemType(int id) const override;
    bool textNodeOpened(int id) const override;
    void openTextNode(int id) override;
    void focusTextNode(int id) override;

    std::vector<std::shared_ptr<Node>> openedNodes_;
    void notifyNodeOpened(int id);

    std::string nodeName(int id) const override;
    std::string nodeText(int id) const override;


    std::vector<std::shared_ptr<Node>>::iterator begin() override;
    std::vector<std::shared_ptr<Node>>::iterator end() override;
    
    void setNodeText(int id, const std::string& text) override;
    void addFolder(const std::string& name) override;

    void addObserver(Observer* view) override;
    void removeObserver(Observer* view) override;
//private:
    std::vector<std::shared_ptr<Node>> rootNodes_;
    void load() override;
private:
    std::vector<Observer*> views_;
    //void notifyTextChanged(int id);
    void notifyTreeChanged();

    std::shared_ptr<Node> findById(int id) const;
    std::shared_ptr<Node> findById(std::shared_ptr<Node> node, int id) const;
};
