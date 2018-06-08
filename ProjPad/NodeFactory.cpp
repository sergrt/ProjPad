#include "stdafx.h"
#include "NodeFactory.h"
#include "FolderNode.h"
#include "TextNode.h"
#include <stdexcept>

std::unique_ptr<Node> constructNode(Node::Type t, int id) {
    if (t == Node::Type::folder)
        return std::make_unique<FolderNode>(id);
    else if (t == Node::Type::text)
        return std::make_unique<TextNode>(id);
    else
        throw std::runtime_error("Unhandled node type");
}
