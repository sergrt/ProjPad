#pragma once
#include "Node.h"
#include <memory>

std::unique_ptr<Node> constructNode(Node::Type t, int id);
