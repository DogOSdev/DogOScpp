
#ifndef __DOGOS__HARDWARE__INTERRUPTMANAGER_H
#define __DOGOS__HARDWARE__INTERRUPTMANAGER_H

#include <gdt.h>
#include <common/types.h>
#include <hardware/port.h>

namespace dogos
{
    namespace hardware
    {
        class InterruptManager;

        class InterruptHandler
        {
        protected:
            dogos::common::uint8_t InterruptNumber;
            InterruptManager *interruptManager;
            InterruptHandler(InterruptManager *interruptManager, dogos::common::uint8_t InterruptNumber);
            ~InterruptHandler();

        public:
            virtual dogos::common::uint32_t HandleInterrupt(dogos::common::uint32_t esp);
        };

        class InterruptManager
        {
            friend class InterruptHandler;

        protected:
            static InterruptManager *ActiveInterruptManager;
            InterruptHandler *handlers[256];

            struct GateDescriptor
            {
                dogos::common::uint16_t handlerAddressLowBits;
                dogos::common::uint16_t gdt_codeSegmentSelector;
                dogos::common::uint8_t reserved;
                dogos::common::uint8_t access;
                dogos::common::uint16_t handlerAddressHighBits;
            } __attribute__((packed));

            static GateDescriptor interruptDescriptorTable[256];

            struct InterruptDescriptorTablePointer
            {
                dogos::common::uint16_t size;
                dogos::common::uint32_t base;
            } __attribute__((packed));

            dogos::common::uint16_t hardwareInterruptOffset;
            static void SetInterruptDescriptorTableEntry(dogos::common::uint8_t interrupt,
                                                         dogos::common::uint16_t codeSegmentSelectorOffset, void (*handler)(),
                                                         dogos::common::uint8_t DescriptorPrivilegeLevel, dogos::common::uint8_t DescriptorType);

            static void InterruptIgnore();

            static void HandleInterruptRequest0x00();
            static void HandleInterruptRequest0x01();
            static void HandleInterruptRequest0x02();
            static void HandleInterruptRequest0x03();
            static void HandleInterruptRequest0x04();
            static void HandleInterruptRequest0x05();
            static void HandleInterruptRequest0x06();
            static void HandleInterruptRequest0x07();
            static void HandleInterruptRequest0x08();
            static void HandleInterruptRequest0x09();
            static void HandleInterruptRequest0x0A();
            static void HandleInterruptRequest0x0B();
            static void HandleInterruptRequest0x0C();
            static void HandleInterruptRequest0x0D();
            static void HandleInterruptRequest0x0E();
            static void HandleInterruptRequest0x0F();
            static void HandleInterruptRequest0x31();

            static void HandleException0x00();
            static void HandleException0x01();
            static void HandleException0x02();
            static void HandleException0x03();
            static void HandleException0x04();
            static void HandleException0x05();
            static void HandleException0x06();
            static void HandleException0x07();
            static void HandleException0x08();
            static void HandleException0x09();
            static void HandleException0x0A();
            static void HandleException0x0B();
            static void HandleException0x0C();
            static void HandleException0x0D();
            static void HandleException0x0E();
            static void HandleException0x0F();
            static void HandleException0x10();
            static void HandleException0x11();
            static void HandleException0x12();
            static void HandleException0x13();

            static dogos::common::uint32_t HandleInterrupt(dogos::common::uint8_t interrupt, dogos::common::uint32_t esp);
            dogos::common::uint32_t DoHandleInterrupt(dogos::common::uint8_t interrupt, dogos::common::uint32_t esp);

            Port8BitSlow programmableInterruptControllerMasterCommandPort;
            Port8BitSlow programmableInterruptControllerMasterDataPort;
            Port8BitSlow programmableInterruptControllerSlaveCommandPort;
            Port8BitSlow programmableInterruptControllerSlaveDataPort;

        public:
            InterruptManager(dogos::common::uint16_t hardwareInterruptOffset, GlobalDescriptorTable *globalDescriptorTable);
            ~InterruptManager();
            dogos::common::uint16_t HardwareInterruptOffset();
            void Activate();
            void Deactivate();
        };
    }
}

#endif