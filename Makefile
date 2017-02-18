IMAGE                   := beaveros.iso
SRC                     := src
OBJ                     := obj
ISO                     := iso

QEMU                    := qemu-system-x86_64
QEMUFLAGS               := -machine q35 -drive format=raw,file=$(IMAGE)

CFLAGS                  := -std=gnu99 -Wall -Wextra -Wshadow -ffreestanding
ASFLAGS                 := -DASSEMBLER
LDFLAGS                 := -ffreestanding -nostdlib -z max-page-size=0x1000
LIBS                    := gcc

DIST                    := beaveros

all: $(IMAGE)

TARGETS                 := loader kernel

SRCS_loader             := $(shell find -L $(SRC)/loader -type f | egrep '\.[csS]$$')
CC_loader               := i686-elf-gcc
CFLAGS_loader           = $(CFLAGS) -I$(SRC)/loader/include/
AS_loader               := $(CC_loader)
ASFLAGS_loader          = $(ASFLAGS)
LD_loader               := $(CC_loader)
LDFLAGS_loader          = $(LDFLAGS) -T $(SRC)/loader/linker.ld
loader.bin: $(SRC)/loader/linker.ld

SRCS_kernel             := $(shell find -L $(SRC) -not -path '$(SRC)/loader/*' -type f | egrep '\.[sc]$$')

CC_kernel               := x86_64-elf-gcc
CFLAGS_kernel           = $(CFLAGS) -I$(SRC)/include/ -mcmodel=large -mno-red-zone -mno-mmx -mno-sse -mno-sse2
AS_kernel               := $(CC_kernel)
ASFLAGS_kernel          = $(ASFLAGS)
LD_kernel               := $(CC_kernel)
LDFLAGS_kernel          = $(LDFLAGS) -T $(SRC)/linker.ld
kernel.bin: $(SRC)/linker.ld

FLAGS                   := DEBUG WERROR
DEBUG                   ?= 0
WERROR                  ?= 0

WITH_DEBUG_CFLAGS       := -O0 -g -DDEBUG
WITH_DEBUG_ASFLAGS      := $(WITH_DEBUG_CFLAGS)
WITH_DEBUG_LDFLAGS      := $(WITH_DEBUG_CFLAGS)
WITH_DEBUG_QEMUFLAGS    := -no-shutdown -no-reboot -d int -monitor stdio -s

WITHOUT_DEBUG_CFLAGS    := -O2
WITHOUT_DEBUG_LDFLAGS   := -O2

WITH_WERROR_CFLAGS      := -Werror
WITH_WERROR_ASFLAGS     := -Werror
WITH_WERROR_LDFLAGS     := -Werror

# Do not change below this line, unless you know what are you doing

ENABLED_FLAGS           := $(foreach f,$(FLAGS),$(shell test '$($(f))' == 1 && echo $(f)))
DISABLED_FLAGS          := $(foreach f,$(FLAGS),$(shell test '$($(f))' != 1 && echo $(f)))

CFLAGS                  += $(foreach f,$(ENABLED_FLAGS),$(WITH_$(f)_CFLAGS))
CFLAGS                  += $(foreach f,$(DISABLED_FLAGS),$(WITHOUT_$(f)_CFLAGS))

ASFLAGS                 += $(foreach f,$(ENABLED_FLAGS),$(WITH_$(f)_ASFLAGS))
ASFLAGS                 += $(foreach f,$(DISABLED_FLAGS),$(WITHOUT_$(f)_ASFLAGS))

QEMUFLAGS_BASE          := $(QEMUFLAGS)
QEMUFLAGS               += $(foreach f,$(ENABLED_FLAGS),$(WITH_$(f)_QEMUFLAGS))
QEMUFLAGS               += $(foreach f,$(DISABLED_FLAGS),$(WITHOUT_$(f)_QEMUFLAGS))

LDFLAGS                 += $(foreach f,$(ENABLED_FLAGS),$(WITH_$(f)_LDFLAGS))
LDFLAGS                 += $(foreach f,$(DISABLED_FLAGS),$(WITHOUT_$(f)_LDFLAGS))
LDFLAGS                 += $(LIBS:%=-l%)

DIRS                    := $(ISO) $(ISO)/boot $(ISO)/boot/grub
QUIET_DIRS              :=
DEPS                    :=

ifeq ($(findstring n,$(MAKEFLAGS)),)
$(shell mkdir -p .bmake)
$(shell $(MAKE) -sn | grep "^echo '" | wc -l >.bmake/all_files)
$(shell echo 0 >.bmake/completed_files)
endif

define C_FILE_TEMPLATE
DIRS                    += $(dir $(2:$(SRC)/%=$(OBJ)/%))
QUIET_DIRS              += $(dir $(2:$(SRC)/%=.bmake/%))
DEPS                    += $(2:$(SRC)/%=.bmake/%.d)

$(2:$(SRC)/%=$(OBJ)/%.o): $(2) | $(dir $(2:$(SRC)/%=$(OBJ)/%)) $(dir $(2:$(SRC)/%=.bmake/%))
ifdef VERBOSE
	$$(CC_$(1)) $$(CFLAGS_$(1)) -c $$< -o $$@
else
	@echo 'CC      $$(@:$(OBJ)/%=%)'
	@$$(CC_$(1)) $$(CFLAGS_$(1)) -c $$< -o $$@
endif
	@echo $$(CC_$(1)) $$(CFLAGS_$(1)) >$(2:$(SRC)/%=.bmake/%.b)

$(2:$(SRC)/%=.bmake/%.d): $(2) | $(dir $(2:$(SRC)/%=.bmake/%))
	@$$(CC_$(1)) $$(CFLAGS_$(1)) -MM $$< | \
	sed -E 's:$(patsubst %.c,%.o,$(notdir $(2))):$(2:$(SRC)/%=$(OBJ)/%.o) $(2:$(SRC)/%=.bmake/%.d):' >$(2:$(SRC)/%=.bmake/%.d)

.PHONY: $$(shell echo $$(CC_$(1)) $$(CFLAGS_$(1)) | diff - $(2:$(SRC)/%=.bmake/%.b) 2>/dev/null || echo $(2:$(SRC)/%=$(OBJ)/%.o))
endef

define AS_FILE_TEMPLATE
DIRS                    += $(dir $(2:$(SRC)/%=$(OBJ)/%))
QUIET_DIRS              += $(dir $(2:$(SRC)/%=.bmake/%))
DEPS                    += $(2:$(SRC)/%=.bmake/%.d)

$(2:$(SRC)/%=$(OBJ)/%.o): $(2) | $(dir $(2:$(SRC)/%=$(OBJ)/%)) $(dir $(2:$(SRC)/%=.bmake/%))
ifdef VERBOSE
	$$(AS_$(1)) $$(ASFLAGS_$(1)) -c $$< -o $$@
else
	@echo 'AS      $$(@:$(OBJ)/%=%)'
	@$$(AS_$(1)) $$(ASFLAGS_$(1)) -c $$< -o $$@
endif
	@echo $$(AS_$(1)) $$(ASFLAGS_$(1)) >$(2:$(SRC)/%=.bmake/%.b)

$(2:$(SRC)/%=.bmake/%.d): $(2) | $(dir $(2:$(SRC)/%=.bmake/%))
	@$$(CC_$(1)) $$(CFLAGS_$(1)) -MM $$< | \
	sed -E 's:$(patsubst %.S,%.o,$(notdir $(2))):$(2:$(SRC)/%=$(OBJ)/%.o) $(2:$(SRC)/%=.bmake/%.d):' >$(2:$(SRC)/%=.bmake/%.d)

.PHONY: $$(shell echo $$(AS_$(1)) $$(ASFLAGS_$(1)) | diff - $(2:$(SRC)/%=.bmake/%.b) 2>/dev/null || echo $(2:$(SRC)/%=$(OBJ)/%.o))
endef

define TARGET_TEMPLATE
OBJS_$(1)               := $(SRCS_$(1):$(SRC)/%=$(OBJ)/%.o)

$(1).bin: $$(OBJS_$(1))
ifdef VERBOSE
	$$(LD_$(1)) $$(LDFLAGS_$(1)) $$(OBJS_$(1)) -o $$@
	@echo $$(LD_$(1)) $$(LDFLAGS_$(1)) >$(1:%=.bmake/%.bin.b)
else
	@echo 'LD      $$@'
	@$$(LD_$(1)) $$(LDFLAGS_$(1)) $$(OBJS_$(1)) -o $$@
	@echo $$(LD_$(1)) $$(LDFLAGS_$(1)) >$(1:%=.bmake/%.bin.b)
endif

.PHONY: $$(shell echo $$(LD_$(1)) $$(LDFLAGS_$(1)) | diff - $(1:%=.bmake/%.bin.b) 2>/dev/null || echo $(1).bin)

$(foreach f,$(filter %.c,$(SRCS_$(1))),$(eval $(call C_FILE_TEMPLATE,$(1),$(f))))
$(foreach f,$(filter %.s %.S,$(SRCS_$(1))),$(eval $(call AS_FILE_TEMPLATE,$(1),$(f))))
endef

$(IMAGE): $(TARGETS:%=%.bin) $(SRC)/grub.cfg | $(ISO) $(ISO)/boot $(ISO)/boot/grub
ifdef VERBOSE
	cp $(TARGETS:%=%.bin) $(ISO)/boot
	cp $(SRC)/grub.cfg $(ISO)/boot/grub
	grub-mkrescue -o $@ $(ISO) >/dev/null 2>/dev/null
else
	@echo 'GRUB    $@'
	@cp $(TARGETS:%=%.bin) $(ISO)/boot
	@cp $(SRC)/grub.cfg $(ISO)/boot/grub
	@grub-mkrescue -o $@ $(ISO) >/dev/null 2>/dev/null
endif

$(foreach t,$(TARGETS),$(eval $(call TARGET_TEMPLATE,$(t))))

$(sort $(DIRS)):
ifdef VERBOSE
	mkdir -p $@
else
	@mkdir -p $@
endif

$(sort $(QUIET_DIRS)):
	@mkdir -p $@

clean:
ifdef VERBOSE
	rm -rf $(IMAGE) $(TARGETS:%=%.bin) $(ISO) $(OBJ) $(DIST).tar.bz2 .bmake
else
	@rm -rf $(IMAGE) $(TARGETS:%=%.bin) $(ISO) $(OBJ) $(DIST).tar.bz2 .bmake
endif

clean-deps:
	@find .bmake -type f -regex '.*\.d' | xargs rm -f

dist: clean
ifdef VERBOSE
	tar -cvf $(DIST).tar --exclude $(DIST).tar --exclude-vcs --exclude-vcs-ignores .
	bzip2 $(DIST).tar
else
	@echo 'TAR     $(DIST)'
	@tar -cf $(DIST).tar --exclude $(DIST).tar --exclude-vcs --exclude-vcs-ignores .
	@echo 'BZIP2   $(DIST).tar'
	@bzip2 $(DIST).tar
endif

run:
ifdef VERBOSE
	$(QEMU) $(QEMUFLAGS)
else
	@$(QEMU) $(QEMUFLAGS)
endif

run-debug:
ifdef VERBOSE
	$(QEMU) $(QEMUFLAGS_BASE) $(WITH_DEBUG_QEMUFLAGS)
else
	@$(QEMU) $(QEMUFLAGS) $(WITH_DEBUG_QEMUFLAGS)
endif

todo-list:
	@find src -type f | xargs cat | fgrep $$'TODO\nFIXME' | sed -E -e 's/.*(TODO|FIXME):/\1:/' -e 's/\s*\*\///'

.PHONY: all clean clean-deps dist run run-debug todo-list

-include $(DEPS)
