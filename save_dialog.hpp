#pragma once
#include "base_text_buffer.hpp"

class Screen;
class TextFile;

class SaveDialog: public BaseTextBuffer
{
public:
    SaveDialog(Screen *screen, TextFile *textFile);
private:
    virtual void internalInsert(Coord &cursor, std::wstring);
    virtual std::wstring internalDelete(const Coord cursor, int = 1);
    virtual std::wstring internalBackspace(Coord &cursor, int = 1);
    void scanDirectory();
    Screen *screen_;
    TextFile *textFile_;
    Coord cursor_;
};
