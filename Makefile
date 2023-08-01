.PHONY: roku

CC := gcc
LD := $(CC)

CFLAGS := -O2 -g3

CFILES := $(shell find src -name "*.c")
OBJ := $(CFILES:.c=.o)

DEST := /usr/local/bin

PROGRAM := roku

$(PROGRAM): $(OBJ)
	@printf " LD   $@\n"
	@$(LD) $< -o $@

%.o: %.c
	@printf " CC   $^\n"
	@$(CC) $(CFLAGS) -c $< -o $@

.PHONY: install
install: $(PROGRAM)
	@printf " INSTALL\n"
	@sudo install -m 755 $(PROGRAM) $(DEST)

.PHONY: uninstall
uninstall:
	@printf " UNINSTALL\n"
	@sudo rm $(DEST)/$(PROGRAM)

.PHONY: clean
clean:
	@printf " CLEAN\n"
	@rm -rf $(OBJ) $(PROGRAM)