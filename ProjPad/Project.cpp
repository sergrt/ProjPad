#include "stdafx.h"
#include "Project.h"
#include "NodeFactory.h"
#include "TextNode.h"
#include "Observer.h"
#include "pugixml.hpp"
#include <algorithm>

namespace xml {
    char content[] = "Content";
    char document_node[] = "DocumentNode";
    namespace attributes {
        char type[] = "type";
        char name[] = "name";
    }
}

Project::Project() {
}

std::unique_ptr<Node> Project::loadNode(const pugi::xml_node& xmlNode) {
    auto& node = constructNode(Node::stringToType(xmlNode.attribute(xml::attributes::type).as_string()));
    auto nameAttribute = xmlNode.attribute(xml::attributes::name);
    if (!nameAttribute)
        throw std::runtime_error("Error loading file");

    node->setName(nameAttribute.as_string());

    for (const auto& c : xmlNode.children()) {
        if (c.type() == pugi::node_pcdata)
            node->setText(c.value());
        else
            node->addChild(loadNode(c));
    }

    return std::move(node);
}
void Project::load(const std::string& fileName) {
    rootNodes_.clear();

    pugi::xml_document doc;
    /*auto parseResult = */doc.load_file(fileName.c_str());
    auto contentNode = doc.child(xml::content);
    if (!contentNode)
        throw std::runtime_error("Error loading file");

    for (const auto& c : contentNode.children())
        rootNodes_.emplace_back(loadNode(c));
    
    fileName_ = fileName;
    notifyTreeChanged();
}

Node* Project::findById(Node& node, int id) const {
    if (node.id() == id)
        return &node;

    for (const auto& n : node.children_) {
        if (const auto f = findById(*n, id))
            return f;
    }

    return nullptr;
}

Node* Project::findById(int id) const {
    for (const auto& r : rootNodes_) {
        if (const auto node = findById(*r, id))
            return node;
    }
    throw std::runtime_error("Item not found");
}

Node* Project::findParentForId(Node& node, int id) const {
    for (const auto& c : node.children_) {
        if (c->id() == id)
            return &node;
    }

    for (const auto& n : node.children_) {
        if (auto f = findParentForId(*n, id))
            return f;
    }

    return nullptr;
}

Node* Project::findParentForId(int id) const {
    for (const auto& r : rootNodes_) {
        if (const auto node = findParentForId(*r, id))
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
    for (const auto v : views_)
        v->openNode(id);
}
void Project::notifyNodeClosed(int id) {
    for (const auto v : views_)
        v->closeNode(id);
}


std::string Project::nodeName(int id) const {
    if (auto node = findById(id))
        return node->name();

    throw std::runtime_error("Item not found");
}
std::string Project::nodeText(int id) const {
    if (auto node = findById(id))
        return static_cast<TextNode*>(node)->text();

    throw std::runtime_error("Item not found");
}



void Project::setNodeText(int id, const std::string& text) {
    if (auto node = findById(id)) {
        auto item = static_cast<TextNode*>(node);
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


std::vector<std::unique_ptr<Node>>::iterator Project::begin() {
    return std::begin(rootNodes_);
}
std::vector<std::unique_ptr<Node>>::iterator Project::end() {
    return std::end(rootNodes_);
}

void Project::addObserver(Observer* view) {
    views_.push_back(view);
}
void Project::removeObserver(Observer* view) {
    views_.erase(std::find(std::begin(views_), std::end(views_), view));
}

void Project::addFolder(const std::string& name) {
    auto node = constructNode(Node::Type::folder);
    node->setName(name);
    rootNodes_.emplace_back(std::move(node));
    notifyTreeChanged();
}


void Project::saveNode(const Node& node, pugi::xml_node& parentXmlNode) {
    pugi::xml_node xmlNode = parentXmlNode.append_child(pugi::node_element);
    if (!xmlNode.set_name(xml::document_node))
        throw std::runtime_error("Error saving file");
    
    if (!xmlNode.append_attribute(xml::attributes::name).set_value(node.name().c_str()))
        throw std::runtime_error("Error saving file");
    
    if (!xmlNode.append_attribute(xml::attributes::type).set_value(Node::typeToString(node.type()).c_str()))
        throw std::runtime_error("Error saving file");

    if (node.type() == Node::Type::text) {
        pugi::xml_node xmlNodeText = xmlNode.append_child(pugi::node_pcdata);
        if (!xmlNodeText.set_value(node.text().c_str()))
            throw std::runtime_error("Error saving file");
    }

    for (const auto& c : node.children_)
        saveNode(*c, xmlNode);
}
void Project::save(const std::string& fileName) {
    pugi::xml_document doc;
    auto node = doc.append_child(xml::content);

    for (const auto& n : rootNodes_)
        saveNode(*n, node);

    if (!doc.save_file(fileName.c_str(), "    "))
        throw std::runtime_error("Error saving file");

    // Update internal file name
    fileName_ = fileName;
}

void Project::save() {
    if (!fileName_)
        throw std::runtime_error("Attempting to save previously unsaved document");

    save(*fileName_);
}

void Project::closeWithChildrenNodes(Node* node) {
    for (const auto& c : node->children_)
        closeWithChildrenNodes(c.get());
    
    const auto openedPos = std::find(std::cbegin(openedNodes_), std::cend(openedNodes_), node);
    if (openedPos != openedNodes_.end()) {
        notifyNodeClosed(node->id());
        openedNodes_.erase(openedPos);
    }
}
void Project::deleteNode(int id) {
    const auto item = findById(id);
    if (!item)
        throw std::runtime_error("Item not found");

    closeWithChildrenNodes(item);
    
    // delete from parent
    auto parentNode = findParentForId(item->id());
    if (parentNode) {
        parentNode->children_.erase(
            std::find_if(std::begin(parentNode->children_),
                std::end(parentNode->children_),
                [item](const auto& i) {
                    return i.get() == item;
                }));
        
    }
    
    const auto rootPos = std::find_if(std::begin(rootNodes_), std::end(rootNodes_), [item](const auto& i) {
        return i.get() == item;
    });
    if (rootPos != rootNodes_.end())
        rootNodes_.erase(rootPos);
        
    notifyItemDeleted(id);
}

void Project::notifyItemDeleted(int id) {
    for (auto* const observer : views_)
        observer->nodeDeleted(id);
}