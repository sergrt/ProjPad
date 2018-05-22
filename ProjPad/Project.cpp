#include "stdafx.h"
#include "Project.h"
#include "NodeFactory.h"
#include "TextNode.h"
#include "Observer.h"

Project::Project() {
}
void Project::load() {
    rootNodes_.clear();

    auto n1 = constructNode(Node::Type::folder);
    n1->setName("Folder #1");
    rootNodes_.push_back(n1);

    auto n2 = constructNode(Node::Type::folder);
    n2->setName("Folder #2");
    rootNodes_.push_back(n2);
    

    auto n3 = constructNode(Node::Type::text);
    n3->setName("Text #1");
    TextNode* t = static_cast<TextNode*>(n3.get());
    t->setText("1234567890");

    //rootNodes_.push_back(n3);
    n2->addChild(std::move(n3));


    auto n4 = constructNode(Node::Type::text);
    n4->setName("Text #2");
    TextNode* t4 = static_cast<TextNode*>(n4.get());
    t4->setText("abcdefghijklmn");

    //rootNodes_.push_back(n3);
    n2->addChild(std::move(n4));


    auto n5 = constructNode(Node::Type::text);
    n5->setName("Root text");
    TextNode* t5 = static_cast<TextNode*>(n5.get());
    t5->setText("mmmmmmmmmzzzzzzzzzzzzzz");

    rootNodes_.push_back(n5);
    auto n6 = constructNode(Node::Type::text);
    n6->setName("Text #3");
    TextNode* t6 = static_cast<TextNode*>(n6.get());
    t6->setText("lllllllllllllllllllllll");
    n5->addChild(std::move(n6));






    notifyTreeChanged();
}

std::shared_ptr<Node> Project::findById(std::shared_ptr<Node> node, int id) const {
    if (node->id() == id)
        return node;

    for (const auto& n : node->children_) {
        if (auto f = findById(n, id))
            return f;
    }

    return nullptr;
}

std::shared_ptr<Node> Project::findById(int id) const {
    for (const auto& r : rootNodes_) {
        if (auto node = findById(r, id))
            return node;
    }
    return nullptr;
}

Node::Type Project::projectItemType(int id) const {
    if (auto node = findById(id))
        return node->type();

    throw std::runtime_error("Item not found");
}
bool Project::textNodeOpened(int id) const {
    for (const auto& x : openedNodes_) {
        if (x->id() == id) {
            return true;
            
        }
    }
    return false;

    /*
    const auto x = std::find(std::cbegin(openedNodes_), std::cend(openedNodes_), [id](const std::shared_ptr<Node> node) {
        return node->id() == id;
    });
    return true;// != std::cend(openedNodes_);
    */
}
void Project::openTextNode(int id) {
    openedNodes_.push_back(findById(id));
    notifyNodeOpened(id);
}
void Project::focusTextNode(int id) {
}
void Project::notifyNodeOpened(int id) {
    for (const auto v : views_) {
        v->openNode(id);
    }
}



std::string Project::nodeName(int id) const {
    if (auto node = findById(id))
        return node->name();

    throw std::runtime_error("Item not found");
}
std::string Project::nodeText(int id) const {
    if (auto node = findById(id))
        return (static_cast<TextNode*>(node.get()))->text();

    throw std::runtime_error("Item not found");
}



void Project::setNodeText(int id, const std::string& text) {
    if (auto node = findById(id)) {
        auto item = static_cast<TextNode*>(node.get());
        item->setText(text);
        //notifyTextChanged(id);
        return;
    }

    throw std::runtime_error("Item not found");
}
/*
void Project::notifyTextChanged(int id) {
    for (auto* const observer : views_)
        observer->updateText(id);
}
*/
void Project::notifyTreeChanged() {
    for (auto* const observer : views_)
        observer->updateTree();
}


std::vector<std::shared_ptr<Node>>::iterator Project::begin() {
    return std::begin(rootNodes_);
}
std::vector<std::shared_ptr<Node>>::iterator Project::end() {
    return std::end(rootNodes_);
}

void Project::addObserver(Observer* view) {
    views_.push_back(view);
}
void Project::removeObserver(Observer* view) {
    std::remove(std::begin(views_), std::end(views_), view);
}

void Project::addFolder(const std::string& name) {
    auto node = constructNode(Node::Type::folder);
    node->setName(name);
    rootNodes_.emplace_back(std::move(node));
    notifyTreeChanged();
}