TARGET					:= $(NAME).bin

CC						:= $(ARCH)-gcc
CFLAGS					:= -std=c99 -pipe -Wall -Wextra -Wshadow -ffreestanding -Wno-unused-variable 

AS          			:= $(ARCH)-as
ASFLAGS     			:=

LD						:= $(CC)
LDFLAGS     			:= -T $(SRC)/linker.ld -ffreestanding -nostdlib -z max-page-size=0x1000
LIBS        			:= gcc

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

MAKE_TREE		:= $(SRC_TREE:$(SRC)%=$(BMAKE)%)
BUILD			:= $(ALL_SRCS:$(SRC)/%=$(BMAKE)/%.b) $(BMAKE)/$(TARGET).b
DEPENDS_SRCS	:= $(CC_SRCS) $(CXX_SRCS)
DEPENDS			:= $(DEPENDS_SRCS:$(SRC)/%=$(BMAKE)/%.d) $(BMAKE)/$(TARGET).d

REBUILD			:= 	$(foreach s,$(CC_SRCS),$(shell echo $(CCCOMPILE) | diff $(s:$(SRC)/%=$(BMAKE)/%.b) - >/dev/null 2>/dev/null || echo $(s))) \
					$(foreach s,$(CXX_SRCS),$(shell echo $(CXXCOMPILE) | diff $(s:$(SRC)/%=$(BMAKE)/%.b) - >/dev/null 2>/dev/null || echo $(s))) \
					$(foreach s,$(AS_SRCS),$(shell echo $(ASCOMPILE) | diff $(s:$(SRC)/%=$(BMAKE)/%.b) - >/dev/null 2>/dev/null || echo $(s))) \
					$(shell echo $(LDCOMPILE) | diff $(BMAKE)/$(TARGET).b - >/dev/null 2>/dev/null || echo $(TARGET))

.PHONY: $(REBUILD)

all: $(TARGET)

.SECONDEXPANSION:

PERCENT := %

$(TARGET): $(OBJS) $(SRC)/linker.ld | $(BMAKE)
	$(LDCOMPILE) $(OBJS) -o $@
	@echo $(LDCOMPILE) >$(BMAKE)/$@.b

$(OBJ)/%.c.o: $(SRC)/%.c | $$(@D) $$(patsubst $(OBJ)$$(PERCENT),$(BMAKE)$$(PERCENT),$$(@D))
	$(CCCOMPILE) -c $< -o $@
	@echo $(CCCOMPILE) >$(BMAKE)/$*.c.b
	@$(CCCOMPILE) -M $< | sed -E 's:^$(notdir $*).o:$(OBJ)/$*.c.o:' >$(BMAKE)/$*.c.d

$(OBJ)/%.s.o: $(SRC)/%.s | $$(@D) $$(patsubst $(OBJ)$$(PERCENT),$(BMAKE)$$(PERCENT),$$(@D))
	$(ASCOMPILE) $< -o $@
	@echo $(ASCOMPILE) >$(BMAKE)/$*.s.b

$(OBJ_TREE):
	mkdir -p $@

$(MAKE_TREE):
	@mkdir -p $@

.PHONY: all

-include $(DEPENDS)
