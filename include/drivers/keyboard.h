#ifndef __DOGOS__DRIVERS__KEYBOARD_H
#define __DOGOS__DRIVERS__KEYBOARD_H

#include <common/types.h>
#include <hardware/interrupts.h>
#include <drivers/driver.h>
#include <hardware/port.h>

namespace dogos
{
    namespace drivers
    {
        class KeyboardEventHandler
        {
        public:
            KeyboardEventHandler();

            virtual void OnKeyDown(char);
            virtual void OnKeyUp(char);
            virtual void OnEnter();
            virtual void ReadLine();
            virtual void KeyPressed();
        };

        class KeyboardDriver : public dogos::hardware::InterruptHandler, public Driver
        {
            dogos::hardware::Port8Bit dataport;
            dogos::hardware::Port8Bit commandport;

            KeyboardEventHandler *handler;

        public:
            KeyboardDriver(dogos::hardware::InterruptManager *manager, KeyboardEventHandler *handler);
            ~KeyboardDriver();
            virtual dogos::common::uint32_t HandleInterrupt(dogos::common::uint32_t esp);
            virtual void Activate();
        };
    }
}

#endif