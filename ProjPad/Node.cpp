#include "stdafx.h"
#include "Node.h"

Node::Node(int id, Type type)
    : id_{ id }, type_ { type } {
}
Node::~Node() {
}

void Node::addChild(std::shared_ptr<Node>&& node) {
    children_.emplace_back(node);
}

void Node::setName(const std::string& name) {
    name_ = name;
}

int Node::id() const {
    return id_;
}
Node::Type Node::type() const {
    return type_;
}
std::string Node::name() const {
    return name_;
}

std::string Node::typeToString(Type t) {
    if (t == Node::Type::folder)
        return "folder";
    else if (t == Node::Type::text)
        return "text";
    else
        throw std::runtime_error("Unhandled type");
}
Node::Type Node::stringToType(const std::string& src) {
    if (src == "folder")
        return Node::Type::folder;
    else if (src == "text")
        return Node::Type::text;
    else
        throw std::runtime_error("Unknown type");
}