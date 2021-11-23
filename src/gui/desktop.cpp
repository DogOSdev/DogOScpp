#include <gui/desktop.h>
#include <common/fonts.h>

using namespace dogos::common;
using namespace dogos::gui;

Desktop::Desktop(int32_t w, int32_t h, uint8_t r, uint8_t g, uint8_t b)
: CompositeWidget(0, 0, 0, w, h, r, g, b),
  MouseEventHandler()
{
    MouseX = w/2;
    MouseY = h/2;
}

Desktop::~Desktop()
{
}


void Desktop::Draw(GraphicsContext* gc)
{
    CompositeWidget::Draw(gc);

    for(int i = 0; i < 4; i++)
    {
        gc -> PutPixel(MouseX-i, MouseY, 0xFF, 0xFF, 0xFF);
        gc -> PutPixel(MouseX+i, MouseY, 0xFF, 0xFF, 0xFF);
        gc -> PutPixel(MouseX, MouseY-i, 0xFF, 0xFF, 0xFF);
        gc -> PutPixel(MouseX, MouseY+i, 0xFF, 0xFF, 0xFF);
    }

    gc -> FillRectangle(0, 200 - 20, 320, 20, 0xA8, 0xA8, 0xA8);
}

void Desktop::OnMouseDown(uint8_t button)
{
    CompositeWidget::OnMouseDown(MouseX, MouseY, button);
}

void Desktop::OnMouseUp(uint8_t button)
{
    CompositeWidget::OnMouseUp(MouseX, MouseY, button);
}

void Desktop::OnMouseMove(int x, int y)
{
    x /= 2;
    y /= 2;
    
    int32_t newMouseX = MouseX + x;
    if(newMouseX < 0) newMouseX = 0;
    if(newMouseX >= w) newMouseX = w - 1;
    
    int32_t newMouseY = MouseY + y;
    if(newMouseY < 0) newMouseY = 0;
    if(newMouseY >= h) newMouseY = h - 1;
    
    CompositeWidget::OnMouseMove(MouseX, MouseY, newMouseX, newMouseY);
    
    MouseX = newMouseX;
    MouseY = newMouseY;
}

void Desktop::OnKeyDown(char key)
{
    switch(key)
    {
        case '1':
            this->r = 0x00;
            this->g = 0x00;
            this->b = 0xA8;
            break;
        case '2':
            this->r = 0x00;
            this->g = 0xA8;
            this->b = 0x00;
            break;
        case '3':
            this->r = 0x00;
            this->g = 0xA8;
            this->b = 0xA8;
            break;
        case '4':
            this->r = 0xA8;
            this->g = 0x00;
            this->b = 0x00;
            break;
        case '5':
            this->r = 0xA8;
            this->g = 0x00;
            this->b = 0xA8;
            break;
        case '6':
            this->r = 0xA8;
            this->g = 0xA8;
            this->b = 0x00;
            break;
    }
}
