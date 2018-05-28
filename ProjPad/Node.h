#pragma once
#include <string>
#include <vector>

class Node {
public:
    enum class Type {
        folder,
        text
    };
    static std::string typeToString(Type t);
    static Type stringToType(const std::string& src);
    explicit Node(int id, Type type);
    virtual ~Node() = 0;
    void setName(const std::string& name);
    
    int id() const;
    Type type() const;
    std::string name() const;
    void addChild(std::unique_ptr<Node> node);
    virtual std::string text() const = 0;
    virtual void setText(const std::string& text) = 0;
private:
    int id_;
    Type type_;
    std::string name_;
public:
    std::vector<std::unique_ptr<Node>> children_;
};
