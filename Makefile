
GCCPARAMS = -m32 -I include -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings
ASPARAMS  = --32
LDPARAMS  = -melf_i386 

objects = 	obj/loader.o \
			obj/gdt.o \
			obj/drivers/driver.o \
			obj/hardware/port.o \
			obj/hardware/interruptstubs.o \
			obj/hardware/interrupts.o \
			obj/hardware/pci.o \
			obj/drivers/keyboard.o \
			obj/drivers/mouse.o \
			obj/drivers/vga.o \
			obj/gui/widget.o \
			obj/gui/window.o \
			obj/gui/desktop.o \
			obj/kernel.o

obj/%.o: src/%.cpp
	mkdir -p $(@D)
	g++ $(GCCPARAMS) -o $@ -c $<

obj/%.o: src/%.s
	mkdir -p $(@D)
	as $(ASPARAMS) -o $@ $<

mykernel.bin: linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)

mykernel.iso: mykernel.bin
	rm -rf out
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp $< iso/boot/
	echo 'set timeout=0' >> iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo '' >> iso/boot/grub/grub.cfg
	echo 'menuentry "DogOS" {' >> iso/boot/grub/grub.cfg
	echo '  multiboot /boot/mykernel.bin' >> iso/boot/grub/grub.cfg
	echo '  boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=$@ iso
	rm -rf iso
	rm mykernel.bin
	mkdir out
	cp $@ out
	rm $@

run: mykernel.iso
	qemu-system-x86_64 -cdrom out/mykernel.iso

.PHONY: clean
clean:
	rm -rf obj
	rm -rf out