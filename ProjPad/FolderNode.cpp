#include "stdafx.h"
#include "FolderNode.h"

FolderNode::FolderNode(int id)
    : Node{ id, Node::Type::folder } {
}
FolderNode::~FolderNode() {
}
