#ifndef __DOGOS__HARDWARE__PORT_H
#define __DOGOS__HARDWARE__PORT_H

#include <common/types.h>

namespace dogos
{
    namespace hardware
    {
        class Port
        {
        protected:
            dogos::common::uint16_t portnumber;
            Port(dogos::common::uint16_t portnumber);
            ~Port();
        };

        class Port8Bit : public Port
        {
        public:
            Port8Bit(dogos::common::uint16_t portnumber);
            ~Port8Bit();
            virtual void Write(dogos::common::uint8_t data);
            virtual dogos::common::uint8_t Read();
        };

        class Port8BitSlow : public Port8Bit
        {
        public:
            Port8BitSlow(dogos::common::uint16_t portnumber);
            ~Port8BitSlow();
            virtual void Write(dogos::common::uint8_t data);
        };

        class Port16Bit : public Port
        {
        public:
            Port16Bit(dogos::common::uint16_t portnumber);
            ~Port16Bit();
            virtual void Write(dogos::common::uint16_t data);
            virtual dogos::common::uint16_t Read();
        };

        class Port32Bit : public Port
        {
        public:
            Port32Bit(dogos::common::uint16_t portnumber);
            ~Port32Bit();
            virtual void Write(dogos::common::uint32_t data);
            virtual dogos::common::uint32_t Read();
        };
    }
}

#endif