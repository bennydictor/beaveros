IMAGE					:= beaveros.iso
SRC						:= src
OBJ						:= obj
ISO						:= iso

CFLAGS					:= -std=c99 -Wall -Wextra -Wshadow -ffreestanding
LDFLAGS					:= -ffreestanding -nostdlib -z max-page-size=0x1000
LIBS					:= gcc

DIST					:= beaver
GZIP					:= gzip

all: $(IMAGE)

TARGETS					:= loader kernel

OBJS_loader				:= $(shell find $(SRC)/loader -type f | egrep '\.[sc]$$')
OBJS_loader				:= $(OBJS_loader:$(SRC)/%=$(OBJ)/%.o)

$(OBJS_loader):	CC		:= i686-elf-gcc
$(OBJS_loader):	CFLAGS	+= -I$(SRC)/loader/include/
$(OBJS_loader):	AS		:= i686-elf-as
loader.bin:		LD		:= i686-elf-gcc
loader.bin:		LDFLAGS	+= -T $(SRC)/loader/linker.ld
loader.bin: $(SRC)/loader/linker.ld

OBJS_kernel				:= $(shell find $(SRC) -not -path '$(SRC)/loader/*' -type f | egrep '\.[sc]$$')
OBJS_kernel				:= $(OBJS_kernel:$(SRC)/%=$(OBJ)/%.o)

$(OBJS_kernel):	CC		:= x86_64-elf-gcc
$(OBJS_kernel):	CFLAGS	+= -I$(SRC)/include/ -mcmodel=large -mno-red-zone -mno-mmx -mno-sse -mno-sse2
$(OBJS_kernel):	AS		:= x86_64-elf-as
kernel.bin:		LD		:= x86_64-elf-gcc
kernel.bin:		LDFLAGS	+= -T $(SRC)/linker.ld
kernel.bin: $(SRC)/linker.ld

FLAGS					:= DEBUG
# Flags can be changed from command line, eg:
# DEBUG=1 make all
# will temporarily turn debug flag on
DEBUG					?= 0

WITH_DEBUG_CFLAGS		:= -O0 -g -DDEBUG
WITH_DEBUG_ASFLAGS		:= --gen-debug
WITH_DEBUG_LDFLAGS		:= -O0 -g -DDEBUG

WITHOUT_DEBUG_CFLAGS	:= -O2
WITHOUT_DEBUG_LDFLAGS	:= -O2

# Do not change below this line, unless you know what are you doing

ENABLED_FLAGS			:= $(foreach f,$(FLAGS),$(shell test '$($(f))' == 1 && echo $(f)))
DISABLED_FLAGS			:= $(foreach f,$(FLAGS),$(shell test '$($(f))' != 1 && echo $(f)))

BINARIES				:= $(TARGETS:%=%.bin)
OBJS					:= $(foreach t,$(TARGETS),$(OBJS_$(t)))
DEPS					:= $(OBJS:$(OBJ)/%.o=.bmake/%.d)

CFLAGS					+= $(foreach f,$(ENABLED_FLAGS),$(WITH_$(f)_CFLAGS))
CFLAGS					+= $(foreach f,$(DISABLED_FLAGS),$(WITHOUT_$(f)_CFLAGS))

ASFLAGS					+= $(foreach f,$(ENABLED_FLAGS),$(WITH_$(f)_ASFLAGS))
ASFLAGS					+= $(foreach f,$(DISABLED_FLAGS),$(WITHOUT_$(f)_ASFLAGS))

LDFLAGS					+= $(foreach f,$(ENABLED_FLAGS),$(WITH_$(f)_LDFLAGS))
LDFLAGS					+= $(foreach f,$(DISABLED_FLAGS),$(WITHOUT_$(f)_LDFLAGS))

.SECONDEXPANSION:

PERCENT					:= %

$(IMAGE): $(BINARIES) $(SRC)/grub.cfg | $(ISO) $(ISO)/boot $(ISO)/boot/grub
	@echo 'GRUB    $(IMAGE)'
	@cp $(TARGETS:%=%.bin) $(ISO)/boot
	@cp $(SRC)/grub.cfg $(ISO)/boot/grub
	@grub-mkrescue -o $@ $(ISO) >/dev/null 2>/dev/null

$(BINARIES): $$(OBJS_$$(patsubst $(PERCENT).bin,$(PERCENT),$$@)) | .bmake
	@echo 'LD      $@'
	@$(LD) $(LDFLAGS) $^ -o $@
	@echo $(LD) $(LDFLAGS) >.bmake/$@.b

$(OBJ)/%.c.o: $(SRC)/%.c | $$(dir $$@) $$(dir .bmake/$$*)
	@echo 'CC      $*.c.o'
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo $(CC) $(CFLAGS) >.bmake/$*.c.b
	@$(CC) $(CFLAGS) -M $< | sed -E 's:^$(notdir $*).o:$(OBJ)/$*.c.o:' >.bmake/$*.c.d

$(OBJ)/%.s.o: $(SRC)/%.s | $$(dir $$@) $$(dir .bmake/$$*)
	@echo 'AS      $*.s.o'
	@$(AS) $(ASFLAGS) $< -o $@
	@echo $(AS) $(ASFLAGS) >.bmake/$*.s.b

$(ISO) $(ISO)/boot $(ISO)/boot/grub $(sort $(dir $(OBJS)) $(patsubst $(OBJ)/%,.bmake/%,$(dir $(OBJS)))):
	@mkdir -p $@

clean:
	@rm -rf $(OBJ) $(ISO) $(IMAGE) $(BINARIES) $(DIST).tar.gz

dist: clean
	@echo 'TAR     $(DIST).tar'
	@tar -cvf $(DIST).tar --exclude-vcs --exclude-vcs-ignores --exclude $(DIST).tar --transform='s:^./:$(DIST)/:' ./* >/dev/null 2>/dev/null
	@echo 'GZIP    $(DIST).tar.gz'
	@$(GZIP) $(DIST).tar

run: all
	@qemu-system-x86_64 $(IMAGE)

run-debug: all
	@qemu-system-x86_64

-include $(DEPS)

.PHONY: all clean dist run
