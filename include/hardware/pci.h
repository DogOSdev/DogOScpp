#ifndef __DOGOS__HARDWARE__PCI_H
#define __DOGOS__HARDWARE__PCI_H

#include <common/types.h>
#include <hardware/port.h>
#include <drivers/driver.h>
#include <hardware/interrupts.h>

namespace dogos
{
    namespace hardware
    {
        enum BaseAddressRegisterType
        {
            MemoryMapping = 0,
            InputOutput = 1
        };

        class BaseAddressRegister
        {
        public:
            bool prefetchable;
            common::uint8_t* address;
            common::uint32_t size;
            BaseAddressRegisterType type;
        };

        class PeripheralComponentInterconnectDeviceDescriptor
        {
        public:
            common::uint32_t portBase;
            common::uint32_t interrupt;

            common::uint16_t bus;
            common::uint16_t device;
            common::uint16_t function;

            common::uint16_t vendor_id;
            common::uint16_t device_id;

            common::uint8_t class_id;
            common::uint8_t subclass_id;
            common::uint8_t interface_id;

            common::uint8_t revision;

            PeripheralComponentInterconnectDeviceDescriptor();
            ~PeripheralComponentInterconnectDeviceDescriptor();
        };

        class PeripheralComponentInterconnectController
        {
            hardware::Port32Bit dataPort;
            hardware::Port32Bit commandPort;
        public:
            PeripheralComponentInterconnectController();
            ~PeripheralComponentInterconnectController();

            common::uint32_t Read(common::uint16_t bus, common::uint16_t device, common::uint16_t function, common::uint32_t registeroffset);
            void Write(common::uint16_t bus, common::uint16_t device, common::uint16_t function, common::uint32_t registeroffset, common::uint32_t value);
            bool DeviceHasFunctions(common::uint16_t bus, common::uint16_t device);

            void SelectDrivers(drivers::DriverManager* driverManager, hardware::InterruptManager* interrupts);
            drivers::Driver* GetDriver(PeripheralComponentInterconnectDeviceDescriptor dev, hardware::InterruptManager* interrupts);
            PeripheralComponentInterconnectDeviceDescriptor GetDeviceDescriptor(common::uint16_t bus, common::uint16_t device, common::uint16_t function);
            BaseAddressRegister GetBaseAddrsssRegister(common::uint16_t bus, common::uint16_t device, common::uint16_t function, common::uint16_t bar);
        };
    }
}
#endif