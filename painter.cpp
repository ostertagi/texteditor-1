#include "painter.hpp"
#include "paint_device.hpp"
#include "deja_vu_sans_mono.hpp"
#include "color.hpp"
#include <SDL2/SDL_ttf.h>
#include <stdexcept>

Painter::Painter(PaintDevice *paintDevice):
    renderer_(paintDevice->renderer()),
    gLeft_(paintDevice->gLeft()),
    gTop_(paintDevice->gTop())
{
	font_ = TTF_OpenFontRW(SDL_RWFromMem((void *)DejaVuSansMono, sizeof(DejaVuSansMono)), 1, 10);
	if (font_ == nullptr)
		throw std::runtime_error("TTF_OpenFont");
}

Painter::~Painter()
{
	TTF_CloseFont(font_);
}

void Painter::drawLine(int x1, int y1, int x2, int y2)
{
    SDL_RenderDrawLine(renderer_, gLeft_ + x1, gTop_ + y1, gLeft_ + x2, gTop_ + y2);
}

void Painter::drawPoint(int x, int y)
{
    SDL_RenderDrawPoint(renderer_, gLeft_ + x, gTop_ + y);
}

void Painter::drawRect(int x, int y, int width, int height)
{
    SDL_Rect rect = { x + gLeft_, y + gTop_, width, height };
    SDL_RenderFillRect(renderer_, &rect);
}

void Painter::setColor(Color color)
{
    SDL_SetRenderDrawColor(renderer_, (color >> 24) & 0xff, (color >> 16) & 0xff, (color >> 8) & 0xff, color & 0xff);
}

void Painter::renderGlyph(wchar_t ch, int x, int y, Color fg, Color bg)
{
    auto surf = TTF_RenderGlyph_Shaded(font_, ch, toSdlColor(fg), toSdlColor(bg));
    if (surf == nullptr)
    {
        setColor(bg);
        drawRect(x, y, glyphWidth(), glyphHeight());
        return;
    }
    auto texture = SDL_CreateTextureFromSurface(renderer_, surf);
    if (texture == nullptr)
    {
        SDL_FreeSurface(surf);
        throw std::runtime_error("CreateTexture");
    }
    auto width = surf->w;
    auto height = surf->h;
    SDL_FreeSurface(surf);
    SDL_Rect rect { x + gLeft_, y + gTop_, width, height };
    SDL_RenderCopy(renderer_, texture, nullptr, &rect);
    SDL_DestroyTexture(texture);
}

int Painter::glyphWidth() const
{
    int minx, maxx, miny, maxy, advance;
    TTF_GlyphMetrics(font_, L'W', &minx, &maxx, &miny, &maxy, &advance);
    return advance;
}

int Painter::glyphHeight() const
{
    return TTF_FontLineSkip(font_);
}

