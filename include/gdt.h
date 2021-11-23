#ifndef __GDT_H
#define __GDT_H

#include <common/types.h>

class GlobalDescriptorTable
{
public:
    class SegmentDescriptor
    {
    private:
        dogos::common::uint16_t limit_lo;
        dogos::common::uint16_t base_lo;
        dogos::common::uint8_t base_hi;
        dogos::common::uint8_t type;
        dogos::common::uint8_t limit_hi;
        dogos::common::uint8_t base_vhi;

    public:
        SegmentDescriptor(dogos::common::uint32_t base, dogos::common::uint32_t limit, dogos::common::uint8_t type);
        dogos::common::uint32_t Base();
        dogos::common::uint32_t Limit();

    } __attribute__((packed));

    SegmentDescriptor nullSegmentSelector;
    SegmentDescriptor unusedSegmentSelector;
    SegmentDescriptor codeSegmentSelector;
    SegmentDescriptor dataSegmentSelector;

public:
    GlobalDescriptorTable();
    ~GlobalDescriptorTable();

    dogos::common::uint16_t CodeSegmentSelector();
    dogos::common::uint16_t DataSegmentSelector();
};

#endif