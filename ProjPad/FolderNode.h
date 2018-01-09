#pragma once
#include "Node.h"

class FolderNode : public Node {
public:
    FolderNode(int id);
    virtual ~FolderNode() override;
};

