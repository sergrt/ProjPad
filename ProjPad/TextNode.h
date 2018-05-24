#pragma once
#include "Node.h"

class TextNode : public Node {
public:
    TextNode(int id);
    
    void setText(const std::string& text) override;
    
    std::string text() const override;
private:
    std::string text_;
};

