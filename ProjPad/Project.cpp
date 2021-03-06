#include "stdafx.h"
#include "Project.h"
#include "NodeFactory.h"
#include "TextNode.h"
#include "Observer.h"
#include "pugixml.hpp"
#include <algorithm>
#include "Utilities.h"
#include <fstream>
#include <sha.h>

namespace xml {
    char content[] = "Content";
    char document_node[] = "DocumentNode";
    namespace attributes {
        char type[] = "type";
        char name[] = "name";
        char id[] = "id";
    }
}

Project::Project() {
    initialHash_ = calculateHash();
    maxNodeId_ = 0;
}

std::unique_ptr<Node> Project::loadNode(const pugi::xml_node& xmlNode) {
    const auto idAttribute = xmlNode.attribute(xml::attributes::id).as_int();
    if (idAttribute > maxNodeId_)
        maxNodeId_ = idAttribute;

    auto& node = constructNode(Node::stringToType(xmlNode.attribute(xml::attributes::type).as_string()), idAttribute);
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
void Project::load(const std::string& fileName, const std::optional<std::string>& password) {
    pugi::xml_document doc;

    if (password) {
        password_ = password;
        std::ifstream f(fileName, std::ios::binary);
        const int fsize = Utilities::fileSize(fileName);
        std::vector<unsigned char> data(fsize);
        f.read((char*)data.data(), fsize);
        data = Cryptopp::decrypt(data, *password_);
        auto parseResult = doc.load_buffer(data.data(), data.size());
        if (!parseResult)
            notifyLoadFailed();
    } else {
        auto parseResult = doc.load_file(fileName.c_str());
        if (!parseResult) {
            notifyPasswordNeeded(fileName);
            return;
        }
    }
    
    auto contentNode = doc.child(xml::content);
    if (!contentNode)
        throw std::runtime_error("Error loading file");

    rootNodes_.clear();

    for (const auto& c : contentNode.children())
        rootNodes_.emplace_back(loadNode(c));
    
    fileName_ = fileName;
    initialHash_ = calculateHash();
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
Node::Type Project::nodeType(int id) const {
    if (auto node = findById(id))
        return node->type();

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

void Project::addFolder(const std::string& name, std::optional<int> parentId) {
    addNode(Node::Type::folder, name, parentId);
}
void Project::addText(const std::string& name, std::optional<int> parentId) {
    addNode(Node::Type::text, name, parentId);
}
void Project::addNode(Node::Type type, const std::string& name, std::optional<int> parentId) {
    auto node = constructNode(type, ++maxNodeId_);
    const int id = node->id();
    node->setName(name);

    if (!parentId) {
        rootNodes_.emplace_back(std::move(node));
    } else {
        auto parentNode = findById(*parentId);
        if (!parentNode)
            throw std::runtime_error("Unable to find parent node");

        parentNode->addChild(std::move(node));
    }

    notifyNodeAdded(id, parentId);
}
void Project::notifyNodeAdded(int id, std::optional<int> parentId) {
    for (auto* const observer : views_)
        observer->nodeAdded(id, parentId);
}


void Project::saveNode(const Node& node, pugi::xml_node& parentXmlNode) {
    pugi::xml_node xmlNode = parentXmlNode.append_child(pugi::node_element);
    if (!xmlNode.set_name(xml::document_node))
        throw std::runtime_error("Error saving file");
    
    if (!xmlNode.append_attribute(xml::attributes::id).set_value(std::to_string(node.id()).c_str()))
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

    if (!password_) {
        if (!doc.save_file(fileName.c_str(), "    "))
            throw std::runtime_error("Error saving file");
    } else {

        struct xml_string_writer : pugi::xml_writer {
            std::string result;
            virtual void write(const void* data, size_t size) {
                result += std::string(static_cast<const char*>(data), size);
            }
        };
        xml_string_writer writer;
        doc.save(writer);
        auto x = writer.result;


        std::ofstream f(fileName.c_str(), std::ios::binary);
        auto encrypted = Cryptopp::encrypt(x, *password_);
        for (const auto c : encrypted)
            f << c;
    }

    // Update internal file name
    fileName_ = fileName;
    initialHash_ = calculateHash();
}
void Project::setPassword(const std::string& password) {
    if (password.empty())
        password_ = std::nullopt;
    else
        password_ = password;
}
void Project::save() {
    if (!fileName_)
        throw std::runtime_error("Attempting to save previously unsaved document");

    save(*fileName_);
    // hash updated in save(const std::string&)
}
std::optional<std::string> Project::password() const {
    return password_;
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
    internalDelete(id, false);
    notifyItemDeleted(id);
}
void Project::internalDelete(int id, bool release) {
    auto parentNode = findParentForId(id);
    if (parentNode) {

        auto pos = std::find_if(std::begin(parentNode->children_), std::end(parentNode->children_), [id](const auto& i) {
            return i.get()->id() == id;
        });
        
        if (pos == std::end(parentNode->children_))
            return;

        if (release)
            pos->release();

        parentNode->children_.erase(pos);
    } else {
        // delete from rootNodes_
        const auto rootPos = std::find_if(std::begin(rootNodes_), std::end(rootNodes_), [id](const auto& i) {
            return i->id() == id;
        });
        if (rootPos != rootNodes_.end()) {
            if (release)
                rootPos->release();

            rootNodes_.erase(rootPos);
        }
    }
}
void Project::notifyItemDeleted(int id) {
    for (auto* const observer : views_)
        observer->nodeDeleted(id);
}

void Project::renameNode(int id, const std::string& name) {
    findById(id)->setName(name);
    notifyNodeRenamed(id);
}

void Project::notifyNodeRenamed(int id) {
    for (auto* const observer : views_)
        observer->nodeRenamed(id);
}

void Project::closeNode(int id) {
    openedNodes_.erase(
        std::find(std::begin(openedNodes_), std::end(openedNodes_), findById(id))
    );
    notifyNodeClosed(id);
}
void Project::notifyLoadFailed() {
    for (auto* const observer : views_)
        observer->loadFailed();
}
void Project::notifyPasswordNeeded(const std::string& fileName) {
    for (auto* const observer : views_)
        observer->filePasswordNeeded(fileName);
}
void Project::moveNode(int itemId, std::optional<int> parentId) {
    std::unique_ptr<Node> node(findById(itemId));
    // delete node from parent
    internalDelete(itemId, true);
    
    if (parentId)
        findById(*parentId)->addChild(std::move(node));
    else
        rootNodes_.emplace_back(std::move(node));
}
bool Project::parentInRootNodes(int parentId) const {
    return std::any_of(std::cbegin(rootNodes_), std::cend(rootNodes_), [parentId](const auto& i) {
        return i->id() == parentId;
    });
}
void Project::moveNodeAbove(int itemId, int parentId) {
    std::unique_ptr<Node> node(findById(itemId));
    // delete node from parent
    internalDelete(itemId, true);
    
    // if parentId not in rootNodes_, use node functions
    if (!parentInRootNodes(parentId)) {
        findParentForId(parentId)->insertChildBefore(std::move(node), parentId);
    } else {
        const auto pos = std::find_if(std::cbegin(rootNodes_), std::cend(rootNodes_), [parentId](const auto& i) {
            return i->id() == parentId;
        });
        rootNodes_.insert(pos, std::move(node));
    }
}
void Project::moveNodeBelow(int itemId, int parentId) {
    std::unique_ptr<Node> node(findById(itemId));
    // delete node from parent
    internalDelete(itemId, true);

    // if parentId not in rootNodes_, use node functions
    if (!parentInRootNodes(parentId)) {
        findParentForId(parentId)->insertChildAfter(std::move(node), parentId);
    } else {
        auto pos = std::find_if(std::cbegin(rootNodes_), std::cend(rootNodes_), [parentId](const auto& i) {
            return i->id() == parentId;
        });
        ++pos;
        rootNodes_.insert(pos, std::move(node));
    }
}
void Project::moveNodeAfterAll(int itemId) {
    std::unique_ptr<Node> node(findById(itemId));
    // delete node from parent
    internalDelete(itemId, true);
    rootNodes_.insert(std::end(rootNodes_), std::move(node));
}

std::optional<std::string> Project::fileName() const {
    return fileName_;
}

std::vector<unsigned char> sha512(const std::vector<unsigned char>& data) {
    std::vector<unsigned char> hash(64);
    CryptoPP::SHA512().CalculateDigest(hash.data(), data.data(), data.size());
    return hash;
}
void combineHashes(std::vector<unsigned char>& hash1, const std::vector<unsigned char>& hash2) {
    for (int i = 0; i < hash1.size(); ++i)
        hash1[i] ^= hash2[i];
}

std::vector<unsigned char> Project::calculateHash() const {
    std::vector<unsigned char> hash(64, 0);
    
    std::string rootNodesIdString;
    for (const auto& r : rootNodes_)
        rootNodesIdString += std::to_string(r->id()) + "|";

    const auto rootNodesIdHash = sha512(std::vector<unsigned char>(rootNodesIdString.begin(), rootNodesIdString.end()));
    combineHashes(hash, rootNodesIdHash);

    for (const auto& r : rootNodes_)
        calculateHash(hash, *r);

    return hash;
}
void Project::calculateHash(std::vector<unsigned char>& hash, Node& node) const {
    std::string data = node.name() + (node.type() == Node::Type::text ? node.text() : std::string());
    const auto dataHash = sha512(std::vector<unsigned char>(data.begin(), data.end()));
    combineHashes(hash, dataHash);
    
    // also calculate hash for children ids
    std::string childrenIdString;
    for (const auto& c : node.children_)
        childrenIdString += std::to_string(c->id()) + "|";
    
    const auto childrenIdHash = sha512(std::vector<unsigned char>(childrenIdString.begin(), childrenIdString.end()));
    combineHashes(hash, childrenIdHash);

    for (const auto& c : node.children_)
        calculateHash(hash, *c);
}
bool Project::hasChanged() const {
    return initialHash_ != calculateHash();
}