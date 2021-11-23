#define GRAPHICS_MODE


#include <common/types.h>
#include <gdt.h>
#include <hardware/interrupts.h>
#include <hardware/pci.h>
#include <drivers/driver.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>
#ifdef GRAPHICS_MODE
#include <drivers/vga.h>
#include <gui/desktop.h>
#include <gui/window.h>
#endif

using namespace dogos;
using namespace dogos::common;
using namespace dogos::drivers;
using namespace dogos::hardware;
#ifdef GRAPHICS_MODE
using namespace dogos::gui;
#endif

static uint16_t* VideoMemory = (uint16_t*)0xb8000;

static uint8_t consoleX=0,consoleY=0;

void printf(char* str)
{
    for(int i = 0; str[i] != '\0'; ++i)
    {
        switch(str[i])
        {
            case '\n':
                consoleX = 0;
                consoleY++;
                break;
            default:
                VideoMemory[80*consoleY+consoleX] = (VideoMemory[80*consoleY+consoleX] & 0xFF00) | str[i];
                consoleX++;
                break;
        }

        if(consoleX >= 80)
        {
            consoleX = 0;
            consoleY++;
        }

        if(consoleY >= 25)
        {
            for(consoleY = 0; consoleY < 25; consoleY++)
                for(consoleX = 0; consoleX < 80; consoleX++)
                    VideoMemory[80*consoleY+consoleX] = (VideoMemory[80*consoleY+consoleX] & 0xFF00) | ' ';
            consoleX = 0;
            consoleY = 0;
        }
    }
}

void clear()
{
    for(consoleY = 0; consoleY < 25; consoleY++)
        for(consoleX = 0; consoleX < 80; consoleX++)
            VideoMemory[80*consoleY+consoleX] = (VideoMemory[80*consoleY+consoleX] & 0xFF00) | ' ';
    consoleX = 0;
    consoleY = 0;
}

void printfHex(uint8_t key)
{
    char* foo = "00";
    char* hex = "0123456789ABCDEF";
    foo[0] = hex[(key >> 4) & 0xF];
    foo[1] = hex[key & 0xF];
    printf(foo);
}


class PrintfKeyboardEventHandler : public KeyboardEventHandler
{
public:
    char cmd[256];
    uint32_t idx = 0;
    bool gettingCommand;

    char *GetLine()
    {
        while(gettingCommand);
        return cmd;
    }

    void OnKeyDown(char c)
    {
        cmd[idx++] = c;
        char* foo = " ";
        foo[0] = c;
        printf(foo);
    }
    void OnEnter()
    {
        printf("\n\"");
        printf(cmd);
        printf("\" was entered!");
        gettingCommand = false;
    }
};

class MouseToConsole : public MouseEventHandler
{
    int8_t x, y;
public:
    
    MouseToConsole()
    {
    }
    
    virtual void OnActivate()
    {
        uint16_t* VideoMemory = (uint16_t*)0xb8000;
        x = 40;
        y = 12;
        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);        
    }
    
    virtual void OnMouseMove(int xoffset, int yoffset)
    {
        static uint16_t* VideoMemory = (uint16_t*)0xb8000;
        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);

        x += xoffset;
        if(x >= 80) x = 79;
        if(x < 0) x = 0;
        y += yoffset;
        if(y >= 25) y = 24;
        if(y < 0) y = 0;

        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);
    }
    
};

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors()
{
    for(constructor* i = &start_ctors; i != &end_ctors; i++)
        (*i)();
}

void shell(PrintfKeyboardEventHandler* handler)
{
    printf("DogOS>> ");
    char* str = handler->GetLine();
    printf(str);
}

extern "C" void kernelMain(const void* multiboot_structure, uint32_t /*multiboot_magic*/)
{
    printf("===DogOS v0.0.1-031520212007===\n");

    GlobalDescriptorTable gdt;
    InterruptManager interrupts(0x20, &gdt);
    
    printf("Initializing Hardware, Stage 1\n");

    #ifdef GRAPHICS_MODE
    int32_t ScreenWidth = 320;
    int32_t ScreenHeight = 200;
    uint8_t ColorDepth = 8;
    

    Desktop desktop(ScreenWidth, ScreenHeight, 0x00, 0x00, 0xA8);
    #endif

    DriverManager drvManager;
    
        #ifndef GRAPHICS_MODE
        PrintfKeyboardEventHandler kbhandler;
        KeyboardDriver keyboard(&interrupts, &kbhandler);
        #endif

        #ifdef GRAPHICS_MODE
        KeyboardDriver keyboard(&interrupts, &desktop);
        #endif

        drvManager.AddDriver(&keyboard);

        #ifdef GRAPHICS_MODE
        MouseDriver mouse(&interrupts, &desktop);
        drvManager.AddDriver(&mouse);
        #endif

        PeripheralComponentInterconnectController PCIController;
        PCIController.SelectDrivers(&drvManager, &interrupts);

        #ifdef GRAPHICS_MODE
        VideoGraphicsArray vga;
        #endif

    printf("Initializing Hardware, Stage 2\n");
        drvManager.ActivateAll();
        
    printf("Initializing Hardware, Stage 3\n");

    clear();

    //shell(&kbhandler);

    #ifdef GRAPHICS_MODE
    bool success = vga.SetMode(ScreenWidth, ScreenHeight, ColorDepth);
    if(!success)
    {
        printf("\nBooting into graphics mode was unsuccessful! The specified graphics mode is unsupported!");
    }
    else
    {
        // 32 colors
        Window win1(&desktop, 0, 0, 10, 10, 0x00, 0x00, 0xA8);
        desktop.AddChild(&win1);
        Window win2(&desktop, 10, 0, 10, 10, 0x00, 0xA8, 0x00);
        desktop.AddChild(&win2);
        Window win3(&desktop, 20, 0, 10, 10, 0x00, 0xA8, 0xA8);
        desktop.AddChild(&win3);
        Window win4(&desktop, 30, 0, 10, 10, 0xA8, 0x00, 0x00);
        desktop.AddChild(&win4);
        Window win5(&desktop, 40, 0, 10, 10, 0xA8, 0x00, 0xA8);
        desktop.AddChild(&win5);
        Window win6(&desktop, 50, 0, 10, 10, 0xA8, 0xA8, 0x00);
        desktop.AddChild(&win6);
        Window win7(&desktop, 60, 0, 10, 10, 0xA8, 0xA8, 0xA8);
        desktop.AddChild(&win7);
        Window win8(&desktop, 70, 0, 10, 10, 0x00, 0x00, 0x57);
        desktop.AddChild(&win8);
        Window win9(&desktop, 80, 0, 10, 10, 0x00, 0x00, 0xFF);
        desktop.AddChild(&win9);
        Window win10(&desktop, 90, 0, 10, 10, 0x00, 0x00, 0xF1);
        desktop.AddChild(&win10);
    }
    #endif

    interrupts.Activate();

    while(1)
    {
        #ifdef GRAPHICS_MODE
        desktop.Draw(&vga);
        vga.ApplyBuffer();
        #endif
    }
}