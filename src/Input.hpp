#pragma once

#include "Vector.hpp"

class Input
{
    public:
        virtual void MouseMotion();


    private:
        bool bIsMouseClicked = false;
        Vector2 screenPos;
};