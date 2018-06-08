#include "stdafx.h"
#include "NodeFactory.h"
#include "FolderNode.h"
#include "TextNode.h"
#include <stdexcept>

static int nextId = 0;

std::unique_ptr<Node> constructNode(Node::Type t, int id) {
    if (id == 0)
        id = ++nextId;

    if (t == Node::Type::folder) {
        return std::make_unique<FolderNode>(id);
    } else if (t == Node::Type::text) {
        return std::make_unique<TextNode>(id);
    } else {
        throw std::runtime_error("Unhandled node type");
    }
}
