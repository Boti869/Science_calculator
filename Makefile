ifeq ($(OS),Windows_NT)
  EXE := .exe
  RUN := mycalc$(EXE)
else
  EXE :=
  RUN := ./$(TARGET)
endif

CC      := gcc
CSTD    := -std=c11
CFLAGS  := $(CSTD) -Wall -Wextra -Wpedantic -O2 -MMD -MP
LDFLAGS := -lm

TARGET := mycalc$(EXE)

SRCS := \
  main.c \
  calcs/help.c \
  calcs/Science-calc.c \
  calcs/algebra.c \
  electrics-c/electrical-laws.c \
  electrics-c/initLaws.c \
  electrics-c/AC-basics.c

OBJDIR := build

# Flatten object names into build/ to avoid nested mkdir complexity
# e.g. src/thing.c -> build/thing.o
OBJS := $(foreach s,$(SRCS),$(OBJDIR)/$(notdir $(s:.c=.o)))
DEPS := $(OBJS:.o=.d)

.PHONY: all run clean rebuild

all: $(TARGET)

$(TARGET): $(OBJS)
	@echo Linking $@
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# Ensure build dir exists (platform-specific)
ifeq ($(OS),Windows_NT)
$(OBJDIR):
	if not exist $(OBJDIR) mkdir $(OBJDIR)
else
$(OBJDIR):
	mkdir -p $(OBJDIR)
endif

# Generate a rule for each object -> source pair
# $(OBJDIR)/<basename>.o : <original source>
define make-obj-rule
$(1): $(2) | $(OBJDIR)
	@echo Compiling $(2)
	$(CC) $(CFLAGS) -c $(2) -o $(1)
endef

# Create those rules
$(foreach src,$(SRCS),$(eval $(call make-obj-rule,$(OBJDIR)/$(notdir $(src:.c=.o)),$(src))))

# Include dependency files if present
-include $(DEPS)

run: $(TARGET)
	$(RUN)

clean:
ifeq ($(OS),Windows_NT)
	@echo Cleaning (Windows)...
	@-cmd /c "if exist $(OBJDIR) rmdir /S /Q $(OBJDIR)"
	@-cmd /c "if exist $(TARGET) del /Q $(TARGET)"
else
	@echo Cleaning...
	@rm -rf $(OBJDIR) $(TARGET)
endif

rebuild: clean all
