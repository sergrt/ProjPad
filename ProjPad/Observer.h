#pragma once
class Project;

class Observer {
public:
    virtual void updateTree() = 0;
    virtual void updateText(int id) = 0;
};
