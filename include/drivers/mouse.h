#ifndef __DOGOS__DRIVERS__MOUSE_H
#define __DOGOS__DRIVERS__MOUSE_H

#include <common/types.h>
#include <hardware/interrupts.h>
#include <drivers/driver.h>
#include <hardware/port.h>

namespace dogos
{
    namespace drivers
    {
        class MouseEventHandler
        {
        public:
            MouseEventHandler();

            virtual void OnActivate();
            virtual void OnMouseDown(dogos::common::uint8_t button);
            virtual void OnMouseUp(dogos::common::uint8_t button);
            virtual void OnMouseMove(int x, int y);
        };

        class MouseDriver : public dogos::hardware::InterruptHandler, public Driver
        {
            dogos::hardware::Port8Bit dataport;
            dogos::hardware::Port8Bit commandport;
            dogos::common::uint8_t buffer[3];
            dogos::common::uint8_t offset;
            dogos::common::uint8_t buttons;

            MouseEventHandler *handler;

        public:
            MouseDriver(dogos::hardware::InterruptManager *manager, MouseEventHandler *handler);
            ~MouseDriver();
            virtual dogos::common::uint32_t HandleInterrupt(dogos::common::uint32_t esp);
            virtual void Activate();
        };
    }
}

#endif
