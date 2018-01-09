#pragma once
#include "Node.h"
class Observer;

class Project final {
public:
    Project();
    
    Node::Type type(int id) const;
    std::string name(int id) const;
    std::string text(int id) const;
    void updateText(int id, const std::string& text);

    void addObserver(Observer* view);
    void removeObserver(Observer* view);
//private:
    std::vector<std::shared_ptr<Node>> rootNodes_;
    void load();
private:
    std::vector<Observer*> views_;
    void notifyTextChanged(int id);
    void notifyTreeChanged();

    std::shared_ptr<Node> findById(int id) const;
    std::shared_ptr<Node> findById(std::shared_ptr<Node> node, int id) const;
};
