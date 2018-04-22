DEVICE          = STM32F415RGT6
OPENCM3_DIR     = ../libopencm3
OBJS            += main.o host.o xbee.o cbuf.o

CFLAGS          += -Og -ggdb3
CPPFLAGS	+= -MD
LDFLAGS         += -static -nostartfiles
LDLIBS          += -Wl,--start-group -lc -lgcc -lnosys -Wl,--end-group

include $(OPENCM3_DIR)/mk/genlink-config.mk
include $(OPENCM3_DIR)/mk/gcc-config.mk

.PHONY: clean all

all: binary.elf binary.hex

clean:
	$(Q)$(RM) -rf binary.* *.o

include $(OPENCM3_DIR)/mk/genlink-rules.mk
include $(OPENCM3_DIR)/mk/gcc-rules.mk

ifeq ($(BMP_PORT),)
BMP_PORT_CANDIDATES := $(wildcard \
/dev/serial/by-id/usb-Black_Sphere_Technologies_Black_Magic_Probe_*-if00 \
/dev/cu.usbmodem*1)
ifeq ($(words $(BMP_PORT_CANDIDATES)),1)
BMP_PORT := $(BMP_PORT_CANDIDATES)
else
BMP_PORT = $(error Black Magic Probe gdb serial port not found, please provide the device name via the BMP_PORT variable parameter$(if \
$(BMP_PORT_CANDIDATES), (found $(BMP_PORT_CANDIDATES))))
endif
endif
%.flash: %.elf
	@printf "  BMP $(BMP_PORT) $(*).elf (flash)\n"
	$(Q)$(GDB) -nx --batch \
		   -ex 'target extended-remote $(BMP_PORT)' \
		   -x black_magic_probe_flash.scr \
		   $(*).elf
