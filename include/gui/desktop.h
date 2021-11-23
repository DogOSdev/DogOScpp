#ifndef __DOGOS__GUI__DESKTOP_H
#define __DOGOS__GUI__DESKTOP_H

#include <common/types.h>
#include <drivers/mouse.h>
#include <gui/widget.h>

namespace dogos
{
    namespace gui
    {
        class Desktop : public gui::CompositeWidget, public drivers::MouseEventHandler
        {
        protected:
            common::uint32_t MouseX;
            common::uint32_t MouseY;

        public:
            Desktop(common::int32_t w, common::int32_t h, common::uint8_t r, common::uint8_t g, common::uint8_t b);
            ~Desktop();

            void Draw(common::GraphicsContext* gc);

            void OnMouseDown(common::uint8_t button);
            void OnMouseUp(common::uint8_t button);
            void OnMouseMove(int x, int y);

            void OnKeyDown(char key);
        };
    }
}

#endif