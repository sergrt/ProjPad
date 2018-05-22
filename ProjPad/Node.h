#pragma once
#include <string>
#include <vector>

class Node {
public:
    enum class Type {
        folder,
        text
    };
    explicit Node(int id, Type type);
    virtual ~Node() = 0;
    void setName(const std::string& name);
    
    int id() const;
    Type type() const;
    std::string name() const;
    void addChild(std::shared_ptr<Node>&& node);
private:
    int id_;
    Type type_;
    std::string name_;
public:
    std::vector<std::shared_ptr<Node>> children_;
};
