CC = gcc
OBJ_DIR=obj

_EXE := \
	cwe22_path_traversal \
	cwe78_os_command_injection \
	cwe120_classic_buffer_overflow \
	cwe121_stack_buffer_overflow \
	cwe131_incorrect_buffer_size \
	cwe134_uncontrolled_format_string
EXE := $(addprefix $(OBJ_DIR)/, $(_EXE))

all: $(OBJ_DIR) $(EXE)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

$(OBJ_DIR)/%: %.c
	$(CC) -o $@ $(CFLAGS) $<

clean:
	rm -rf $(OBJ_DIR)
