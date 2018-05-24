#include "stdafx.h"
#include "FolderNode.h"

FolderNode::FolderNode(int id)
    : Node{ id, Node::Type::folder } {
}
FolderNode::~FolderNode() {
}

std::string FolderNode::text() const {
    throw std::runtime_error("Trying to get text from folder node");
}

void FolderNode::setText(const std::string& text) {
    throw std::runtime_error("Trying to set text to folder node");
}