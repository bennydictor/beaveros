NAME					:= beaveros
KERNEL					:= $(NAME).bin
IMAGE					:= $(NAME).iso
SRC						:= src
OBJ						:= obj
ISO						:= iso

CC						:= i686-elf-gcc
CFLAGS					:= -std=c99 -pipe -Wall -Wextra -Wshadow -ffreestanding -Wno-unused-variable 

AS          			:= i686-elf-as
ASFLAGS     			:=

LD						:= $(CC)
LDFLAGS     			:= -T src/linker.ld -ffreestanding -nostdlib
LIBS        			:= gcc

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

# Do not change below this line, unless you know what are you doing

SRC_TREE		:= $(shell find $(SRC) -type d)
CC_SRCS			:= $(shell find $(SRC) -type f -regex '.*\.c')
AS_SRCS			:= $(shell find $(SRC) -type f -regex '.*\.s')
ALL_SRCS		:= $(CC_SRCS) $(CXX_SRCS) $(AS_SRCS)

ENABLED_FLAGS	:= $(foreach f,$(FLAGS),$(shell test '$($(f))' == 1 && echo $(f)))
DISABLED_FLAGS	:= $(foreach f,$(FLAGS),$(shell test '$($(f))' != 1 && echo $(f)))

CFLAGS			:= $(foreach f,$(ENABLED_FLAGS),$(WITH_$(f)_CFLAGS)) $(CFLAGS)
CXXFLAGS		:= $(foreach f,$(ENABLED_FLAGS),$(WITH_$(f)_CXXFLAGS)) $(CXXFLAGS)
ASFLAGS			:= $(foreach f,$(ENABLED_FLAGS),$(WITH_$(f)_ASFLAGS)) $(ASFLAGS)
LDFLAGS			:= $(foreach f,$(ENABLED_FLAGS),$(WITH_$(f)_LDFLAGS)) $(LDFLAGS)

CFLAGS			:= $(foreach f,$(DISABLED_FLAGS),$(WITHOUT_$(f)_CFLAGS)) $(CFLAGS)
CXXFLAGS		:= $(foreach f,$(DISABLED_FLAGS),$(WITHOUT_$(f)_CXXFLAGS)) $(CXXFLAGS)
ASFLAGS			:= $(foreach f,$(DISABLED_FLAGS),$(WITHOUT_$(f)_ASFLAGS)) $(ASFLAGS)
LDFLAGS			:= $(foreach f,$(DISABLED_FLAGS),$(WITHOUT_$(f)_LDFLAGS)) $(LDFLAGS)

ifndef LD
$(error LD is undefined. Define it in Makefile)
LD				:= ld
endif
LDCOMPILE		:= $(strip $(LD) $(LDFLAGS))

ifdef CC_SRCS
ifndef CC
$(error CC is undefined. Define it in Makefile)
CC				:= gcc
endif
CCCOMPILE		:= $(strip $(CC) $(CFLAGS) -I include)
endif

ifdef AS_SRCS
ifndef AS
$(error AS is undefined. Define it in Makefile)
AS				:= as
endif
ASCOMPILE		:= $(strip $(AS) $(ASFLAGS))
endif

OBJ_TREE		:= $(SRC_TREE:$(SRC)%=$(OBJ)%)
OBJS			:= $(ALL_SRCS:$(SRC)/%=$(OBJ)/%.o)

MAKE_TREE		:= $(SRC_TREE:$(SRC)%=.bmake%)
BUILD			:= $(ALL_SRCS:$(SRC)/%=.bmake/%.b) .bmake/$(KERNEL).b
DEPENDS_SRCS	:= $(CC_SRCS) $(CXX_SRCS)
DEPENDS			:= $(DEPENDS_SRCS:$(SRC)/%=.bmake/%.d) .bmake/$(KERNEL).d

REBUILD			:= 	$(foreach s,$(CC_SRCS),$(shell echo $(CCCOMPILE) | diff $(s:$(SRC)/%=.bmake/%.b) - >/dev/null 2>/dev/null || echo $(s))) \
					$(foreach s,$(CXX_SRCS),$(shell echo $(CXXCOMPILE) | diff $(s:$(SRC)/%=.bmake/%.b) - >/dev/null 2>/dev/null || echo $(s))) \
					$(foreach s,$(AS_SRCS),$(shell echo $(ASCOMPILE) | diff $(s:$(SRC)/%=.bmake/%.b) - >/dev/null 2>/dev/null || echo $(s))) \
					$(shell echo $(LDCOMPILE) | diff .bmake/$(KERNEL).b - >/dev/null 2>/dev/null || echo $(KERNEL))

DISTNAME		:= $(notdir $(PWD))

.PHONY: $(REBUILD)

all: beaveros.iso

.SECONDEXPANSION:

PERCENT := %

$(IMAGE): $(KERNEL) $(SRC)/grub.cfg
	mkdir -p $(ISO)/boot/grub
	cp $< $(ISO)/boot
	cp $(SRC)/grub.cfg $(ISO)/boot/grub
	grub-mkrescue -o $@ $(ISO)

$(KERNEL): $(OBJS) | .bmake
	$(LDCOMPILE) $^ -o $@
	@echo $(LDCOMPILE) >.bmake/$@.b

$(OBJ)/%.c.o: $(SRC)/%.c | $$(@D) $$(patsubst $(OBJ)$$(PERCENT),.bmake$$(PERCENT),$$(@D))
	$(CCCOMPILE) -c $< -o $@
	@echo $(CCCOMPILE) >.bmake/$*.c.b
	@$(CCCOMPILE) -M $< | sed -E 's:^$*.o:$(OBJ)/$*.c.o:' >.bmake/$*.c.d
#	@$(CCCOMPILE) -M $< | sed -E 's/^$*.o/$(OBJ)\/$*.c.o/' >.bmake/$*.c.d

$(OBJ)/%.s.o: $(SRC)/%.s | $$(@D) $$(patsubst $(OBJ)$$(PERCENT),.bmake$$(PERCENT),$$(@D))
	$(ASCOMPILE) $< -o $@
	@echo $(ASCOMPILE) >.bmake/$*.s.b

$(OBJ_TREE):
	mkdir -p $@

$(MAKE_TREE):
	@mkdir -p $@

run: $(IMAGE)
	@qemu-system-x86_64 -cdrom $(IMAGE)

clean:
	rm -rf $(KERNEL) $(IMAGE) $(OBJ) $(ISO)
	@rm -rf .bmake

dist: clean
	tar -cvf $(DISTNAME).tar --exclude=$(DISTNAME).tar .
	$(COMPRESS) $(DISTNAME).tar

.PHONY: all clean

-include $(DEPENDS)
