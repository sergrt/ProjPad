#pragma once
#include "Node.h"

class FolderNode : public Node {
public:
    FolderNode(int id);
    ~FolderNode() override;
    std::string text() const override;
    void setText(const std::string& text) override;
};

