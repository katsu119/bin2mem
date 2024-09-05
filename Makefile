TARGET := main

RISCV_PREFIX := riscv64-unknown-elf-
GCC := $(RISCV_PREFIX)gcc
LD := $(RISCV_PREFIX)ld
AS := $(RISCV_PREFIX)as
OBJDUMP := $(RISCV_PREFIX)objdump
OBJCOPY := $(RISCV_PREFIX)objcopy

GCC_OPTIONS := -c -march=rv32i -mabi=ilp32 -mno-relax
AS_OPTIONS := -march=rv32i -mabi=ilp32 -mno-relax
LD_OPTIONS := -T bram.ld -m elf32lriscv -nostdlib --no-relax --print-memory-usage

DEASSEMBLY_OBJDUMP_OPTIONS := 
OBJDUMP_OPTIONS := 
OBJCOPY_OPTIONS := 

ASM_SRC := $(wildcard src/*.S)
ASM_OBJ := $(ASM_SRC:.S=.o)
C_SRC := $(wildcard src/*.c)
C_OBJ := $(C_SRC:.c=.o)

BIN2MEM_OPT := -b -n 4

all: $(TARGET).mem

%.o: %.c
	$(GCC) $(GCC_OPTIONS) $< -o $@

%.o: %.S
	$(AS) $(AS_OPTIONS) $< -o $@
	$(OBJCOPY) --remove-section .riscv.attributes $@

$(TARGET): $(ASM_OBJ) $(C_OBJ)
	$(LD) $(LD_OPTIONS) $^ -o $@

$(TARGET).bin: $(TARGET)
	$(OBJCOPY) -O binary --only-section=.text $< $@

$(TARGET).mem: $(TARGET).bin
	./bin2mem $< $(BIN2MEM_OPT) -o $@

deassembly: 
	$(OBJDUMP) -D $(TARGET)
info: 
	$(OBJDUMP) -x $(TARGET)

.PHONY: clean all

clean:
	rm $(C_OBJ) $(ASM_OBJ)
