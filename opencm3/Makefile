
TARGET 			= systick

# Opencm3 library version
OPENCM3_DIR = libopencm3
LIBNAME		= opencm3_stm32f1

# Configure micro-controller
MCU_FAMILY	= STM32F1
LDSCRIPT 	= stm32f1.ld
CPU			= cortex-m3
INSTR_SET	= thumb
FLOAT_ABI	= soft

# compiler option
OPT			:= -Os
CSTD		?= -std=c99
CXXSTD		:= c++17

# Project specific configuration
BUILD_DIR 	:= build
SRC_DIR 	:= src
INC_DIRS	= include $(OPENCM3_DIR)/include

# Compiler configuration
PREFIX		?= arm-none-eabi
CC			:= $(PREFIX)-gcc
CXX			:= $(PREFIX)-g++
LD			:= $(PREFIX)-gcc
AR			:= $(PREFIX)-ar
AS			:= $(PREFIX)-as
SIZE		:= $(PREFIX)-size
OBJCOPY		:= $(PREFIX)-objcopy
OBJDUMP		:= $(PREFIX)-objdump
GDB			:= $(PREFIX)-gdb


# collect source files and generate object files
SRCS 		:= $(shell find $(SRC_DIR) -name '*.cpp' -or -name '*.c')	
OBJS 		:= $(SRCS:%.cpp=$(BUILD_DIR)/%.o) 				# replace .c with .o


# Define stm32 family macro
DEFS		+= -D$(MCU_FAMILY)

# header library include flsgs
INC_FLAGS 	= $(addprefix -I,$(INC_DIRS))

# Target-specific flags
CPU_FLAGS	?= -mfloat-abi=$(FLOAT_ABI) -m$(INSTR_SET) -mcpu=$(CPU)

# C flags
CFLAGS		+= $(OPT) -std=$(CSTD) $(CPU_FLAGS) $(DEFS) $(INC_FLAGS)
CXXFLAGS	+= $(OPT) -std=$(CXXSTD) $(CPU_FLAGS) $(DEFS) $(INC_FLAGS)

# Warning options for C and CXX compiler
CFLAGS		+= -Wall -Wextra -Wundef -Wshadow -Wredundant-decls -Wmissing-prototypes -Wstrict-prototypes
CXXFLAGS	+= -Wall -Wextra -Wundef -Wshadow -Wredundant-decls -Weffc++

# Code generator options
CFLAGS		+= -fno-common -ffunction-sections -fdata-sections
CXXFLAGS	+= -fno-common -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti

# add debug flags if build type is debug
ifeq ($(BUILD_TYPE), Debug)
CFLAGS 		+= -g -gdwarf-2
CXXFLAGS	+= -g -gdwarf-2
endif

# Dependency flags
CPPFLAGS	+= -MMD -MP
CFLAGS		+= -MMD -MP

# Linker flags
LDFLAGS		+= --static -nostartfiles
LDFLAGS		+= -T$(LDSCRIPT)
LDFLAGS		+= $(ARCH_FLAGS) $(DEBUG)
LDFLAGS		+= -Wl,-Map=$(*).map -Wl,--cref
LDFLAGS		+= -Wl,--gc-sections
ifeq ($(V),99)
LDFLAGS		+= -Wl,--print-gc-sections
endif

# Used libraries
LDFLAGS		+= -L$(OPENCM3_DIR)/lib
LDLIBS		+= -l$(LIBNAME)
LDLIBS		+= -Wl,--start-group -lc -lgcc -lnosys -Wl,--end-group

.SUFFIXES: .elf .bin .hex .srec .list .map .images
.SECONDEXPANSION:
.SECONDARY:

all: elf size
size: $(BUILD_DIR)/$(TARGET).size
elf: $(BUILD_DIR)/$(TARGET).elf
bin: $(BUILD_DIR)/$(TARGET).bin
hex: $(BUILD_DIR)/$(TARGET).hex
srec: $(BUILD_DIR)/$(TARGET).srec
list: $(BUILD_DIR)/$(TARGET).list

GENERATED_BINARIES=$(TARGET).elf $(TARGET).bin $(TARGET).hex $(TARGET).srec $(TARGET).list $(TARGET).map

%.bin: %.elf
	$(OBJCOPY) -Obinary $(*).elf $(*).bin

%.hex: %.elf
	$(OBJCOPY) -Oihex $(*).elf $(*).hex

%.srec: %.elf
	$(OBJCOPY) -Osrec $(*).elf $(*).srec

%.list: %.elf
	$(OBJDUMP) -S $(*).elf > $(*).list

%.elf %.map: $(OBJS) $(LDSCRIPT)
	@echo "LD => $@"
	@$(LD) $(TGT_LDFLAGS) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $(*).elf

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo "CC" $< " ==> " $@
	@$(CC) $(CFLAGS)  $(CPPFLAGS) -o $@ -c $<

$(BUILD_DIR)/%.o: %.cxx
	@mkdir -p $(dir $@)
	@echo "CXX" $< " ==> " $@
	@$(CXX)  $(CXXFLAGS) $(CPPFLAGS) -o $(*).o -c $(*).cxx

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo "CXX" $< " ==> " $@
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(INCFLAGS) -o $@ -c $<

%.size: %.elf
	@echo "Output code size:"
	@$(SIZE) -A -d $(*).elf | egrep 'text|data|bss' | awk ' \
    function human(x) { \
        if (x<1000) {return x} else {x/=1024} \
        s="kMGTEPZY"; \
        while (x>=1000 && length(s)>1) \
            {x/=1024; s=substr(s,2)} \
        return int(x+0.5) substr(s,1,1) \
    } \
	{printf("%-10s %-8s\n", $$1, human($$2))} \
'

flash: elf
	openocd -d2 -f interface/stlink.cfg -c "transport select hla_swd" -f target/stm32f1x.cfg -c "program {$(BUILD_DIR)/$(TARGET).elf}  verify reset; shutdown;"


clean:
	@# $(RM) $(GENERATED_BINARIES) generated.* $(OBJS) $(OBJS:%.o=%.d)
	$(RM) -rf $(BUILD_DIR)

.PHONY: images clean stylecheck styleclean elf bin hex srec list
-include $(OBJS:.o=.d)
