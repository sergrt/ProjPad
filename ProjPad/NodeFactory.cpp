#include "stdafx.h"
#include "NodeFactory.h"
#include "FolderNode.h"
#include "TextNode.h"
#include <stdexcept>

static int nextId = 0;

std::unique_ptr<Node> constructNode(Node::Type t) {
    if (t == Node::Type::folder) {
        return std::make_unique<FolderNode>(nextId++);
    } else if (t == Node::Type::text) {
        return std::make_unique<TextNode>(nextId++);
    } else {
        throw std::runtime_error("Unhandled node type");
    }
}
