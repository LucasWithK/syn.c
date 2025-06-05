
CFLAGS = -Wall -Wextra -g -std=c17
LDFLAGS =

TARGET = sync

SRCDIR = src
BUILDDIR = build
OBJDIR = $(BUILDDIR)/obj
BINDIR = $(BUILDDIR)/bin

SOURCES := $(wildcard $(SRCDIR)/*.c)
OBJECTS := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))

EXECUTABLE = $(BINDIR)/$(TARGET)

.PHONY: all clean run directories

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) | $(BUILDDIR) $(BINDIR)
	$(CC) $(LDFLAGS) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

directories: $(BUILDDIR) $(OBJDIR) $(BINDIR)

$(BUILDDIR):
	@mkdir -p $@

$(OBJDIR):
	@mkdir -p $@

$(BINDIR):
	@mkdir -p $@

run: $(EXECUTABLE)
	./$(EXECUTABLE)

clean:
	rm -rf $(BUILDDIR)
