CC=gcc

OBJDIR=obj
BINDIR=bin
SRCDIR=src

all: disassemble

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(SRCDIR)/%.h $(OBJDIR)
	$(CC) -c -o $@ $<

$(OBJDIR):
	mkdir $(OBJDIR)

$(BINDIR):
	mkdir $(BINDIR)

%: $(OBJDIR)/%.o $(BINDIR)
	$(CC) -o $(BINDIR)/$@ $<

.PHONY: clean

clean:
	rm -rf $(OBJDIR) $(BINDIR)


