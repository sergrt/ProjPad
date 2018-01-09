#include "stdafx.h"
#include "NodeFactory.h"
#include "FolderNode.h"
#include "TextNode.h"

static int nextId = 0;

std::shared_ptr<Node> constructNode(Node::Type t) {
    if (t == Node::Type::folder) {
        return std::make_shared<FolderNode>(nextId++);
    } else if (t == Node::Type::text) {
        return std::make_shared<TextNode>(nextId++);
    } else {
        throw std::runtime_error("Unhandled node type");
    }
}
