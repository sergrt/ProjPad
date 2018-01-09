#pragma once
#include "Node.h"

class TextNode : public Node {
public:
    TextNode(int id);
    
    void setText(const std::string& text);
    
    std::string text() const;
private:
    std::string text_;
};

