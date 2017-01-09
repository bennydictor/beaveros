IMAGE					:= beaveros.iso
SRC						:= src
OBJ						:= obj
ISO						:= iso

FLAGS					:= DEBUG
# Flags can be changed from command line, eg:
# DEBUG=1 make all
# will temporarily turn debug flag on
DEBUG					?= 0

WITH_DEBUG_CFLAGS		:= -O0 -ggdb -DDEBUG
WITH_DEBUG_LDFLAGS		:= -O0 -ggdb -DDEBUG
WITH_DEBUG_ASFLAGS		:= --gen-debug

WITHOUT_DEBUG_CFLAGS	:= -O2
WITHOUT_DEBUG_LDFLAGS	:= -O2

COMPRESS				:= gzip

# Do not change below this line, unless you know what are you doing

MODULES			:= $(shell find src -maxdepth 1 -mindepth 1 -type d)
MODULES			:= $(MODULES:src/%=%)
BINARIES		:= $(MODULES:%=%.bin)

export SRC OBJ FLAGS DEBUG WITH_DEBUG_CFLAGS WITH_DEBUG_LDFLAGS WITH_DEBUG_ASFLAGS WITHOUT_DEBUG_CFLAGS WITHOUT_DEBUG_LDFLAGS

SHELL_EXPORT	:= $(shell cat Makefile | sed -n '/^export/s/^export //p')
SHELL_EXPORT	:= $(foreach v,$(SHELL_EXPORT),$(v)='$($(v))')

REBUILD			:= $(strip $(foreach m,$(MODULES),$(shell $(SHELL_EXPORT) $(MAKE) -f $(SRC)/$(m).mk -q || echo -n "$(m).bin ")))

DISTNAME		:= $(notdir $(PWD))

all: $(IMAGE)

$(IMAGE): $(BINARIES) $(SRC)/grub.cfg | $(ISO) $(ISO)/boot $(ISO)/boot/grub
	cp $(BINARIES) $(ISO)/boot
	cp $(SRC)/grub.cfg $(ISO)/boot/grub
	grub-mkrescue -o $@ $(ISO)

$(BINARIES):
	$(MAKE) -f $(@:%.bin=src/%.mk) all

ifneq "$(REBUILD)" ""
.PHONY: $(IMAGE) $(REBUILD)
endif

$(ISO) $(ISO)/boot $(ISO)/boot/grub:
	mkdir -p $@

clean:
	rm -rf $(IMAGE) $(BINARIES) $(ISO) $(OBJ)
	@rm -rf .bmake

run: $(IMAGE)
	@qemu-system-x86_64 $(IMAGE)

dist: clean
	tar -cvf $(DISTNAME).tar --exclude=$(DISTNAME).tar --exclude-vcs --exclude-vcs-ignores .
	$(COMPRESS) $(DISTNAME).tar

.PHONY: all clean dist run
