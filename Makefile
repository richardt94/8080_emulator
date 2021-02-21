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

clean:
	rm -rf $(OBJDIR) $(BINDIR)

%: $(OBJDIR)/%.o $(BINDIR)
	$(CC) -o $(BINDIR)/$@ $<


