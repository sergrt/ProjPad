#include "stdafx.h"
#include "TextNode.h"

TextNode::TextNode(int id)
    : Node{ id, Node::Type::text } {
}

void TextNode::setText(const std::string& text) {
    text_ = text;
}

std::string TextNode::text() const {
    return text_;
}
