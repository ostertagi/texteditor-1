#pragma once
#include "base_text_buffer.hpp"

class TextFile: public BaseTextBuffer
{
public:
    TextFile(std::string fileName);
    void save();
    void saveAs(std::string fileName);
private:
    std::string fileName_;
};