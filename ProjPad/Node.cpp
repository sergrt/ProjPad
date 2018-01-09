#include "stdafx.h"
#include "Node.h"

Node::Node(int id, Type type)
    : id_{ id }, type_ { type } {
}
Node::~Node() {
}

void Node::addChild(std::shared_ptr<Node>&& node) {
    children_.push_back(node);
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
