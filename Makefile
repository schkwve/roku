#
# Copyright (c) 2023 Jozef Nagy
# 
# Use of this source code is governed by an MIT-style
# license that can be found in the LICENSE file or at
# https://opensource.org/licenses/MIT.
#
#

.PHONY: roku

CC := gcc
LD := $(CC)

CFLAGS := -O2 -g3 -Wall -Wextra -Werror -pedantic -std=c99

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