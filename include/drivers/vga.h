#ifndef __DOGOS__DRIVERS__VGA_H
#define __DOGOS__DRIVERS__VGA_H

#include <common/types.h>
#include <hardware/interrupts.h>
#include <drivers/driver.h>
#include <hardware/port.h>

namespace dogos
{
    namespace drivers
    {
        class VideoGraphicsArray
        {
            hardware::Port8Bit miscPort;
            hardware::Port8Bit crtcIndexPort;
            hardware::Port8Bit crtcDataPort;
            hardware::Port8Bit sequencerIndexPort;
            hardware::Port8Bit sequencerDataPort;
            hardware::Port8Bit graphicsControllerIndexPort;
            hardware::Port8Bit graphicsControllerDataPort;
            hardware::Port8Bit attributeControllerIndexPort;
            hardware::Port8Bit attributeControllerReadPort;
            hardware::Port8Bit attributeControllerWritePort;
            hardware::Port8Bit attributeControllerResetPort;

            void WriteRegisters(common::uint8_t* registers);
            common::uint8_t* GetFrameBufferSegment();

            virtual common::uint8_t GetColorIndex(common::uint8_t r, common::uint8_t g, common::uint8_t b);

            common::uint8_t buffer[320][200];

        public:
            VideoGraphicsArray();
            ~VideoGraphicsArray();

            virtual bool SupportsMode(common::uint32_t width, common::uint32_t height, common::uint32_t colordepth);
            virtual bool SetMode(common::uint32_t width, common::uint32_t height, common::uint32_t colordepth);
            virtual void PutPixel(common::int32_t x, common::int32_t y, common::uint8_t r, common::uint8_t g, common::uint8_t b);
            virtual void PutPixel(common::int32_t x, common::int32_t y, common::uint8_t colorIndex);

            virtual void FillRectangle(common::uint32_t x, common::uint32_t y, common::uint32_t w, common::uint32_t h, common::uint8_t r, common::uint8_t g, common::uint8_t b);

            virtual void ApplyBuffer();

            virtual void DrawChar(common::uint8_t c, common::uint8_t* font, common::int32_t x, common::int32_t y, common::uint8_t r, common::uint8_t g, common::uint8_t b, common::uint32_t w, common::uint32_t h);
            virtual void DrawString(char* str, common::uint8_t* font, common::int32_t x, common::int32_t y, common::uint8_t r, common::uint8_t g, common::uint8_t b, common::uint32_t w, common::uint32_t h);
        };
    }
}

#endif