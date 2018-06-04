#include "stdafx.h"
#include "Node.h"
#include <algorithm>

Node::Node(int id, Type type)
    : id_{ id }, type_ { type } {
}
Node::~Node() {
}

void Node::addChild(std::unique_ptr<Node> node) {
    children_.emplace_back(std::move(node));
}
void Node::insertChildBefore(std::unique_ptr<Node> node, int beforeId) {
    const auto p = std::find_if(std::cbegin(children_), std::cend(children_), [beforeId](const auto& i) {
        return i->id() == beforeId;
    });
    if (p == std::cend(children_))
        throw std::runtime_error("Cant find beforeId item");

    children_.insert(p, std::move(node));
}
void Node::insertChildAfter(std::unique_ptr<Node> node, int afterId) {
    auto p = std::find_if(std::cbegin(children_), std::cend(children_), [afterId](const auto& i) {
        return i->id() == afterId;
    });
    if (p == std::cend(children_))
        throw std::runtime_error("Cant find afterId item");
    ++p;
    children_.insert(p, std::move(node));
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