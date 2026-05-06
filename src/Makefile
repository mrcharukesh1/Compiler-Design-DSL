# ============================================================
# Makefile — DSL Compiler
# ============================================================
#
# Targets:
#   make          — build the compiler binary
#   make run      — compile + run sample1.dsl
#   make ir       — compile DSL → LLVM IR → assembly → executable
#   make clean    — remove generated files
#   make test     — run all test samples
#
# Prerequisites:
#   flex, bison, gcc, llc, clang
# ============================================================

CC       = gcc
CFLAGS   = -Wall -Wextra -g -I.
LDFLAGS  =

# Source files (generated + hand-written)
SRCS = lex.yy.c parser.tab.c ast.c semantic.c symbol_table.c codegen.c main.c
OBJS = $(SRCS:.c=.o)

TARGET   = compiler
SAMPLE   = ../test/sample1.dsl
IR_OUT   = ../output/output.ll
ASM_OUT  = ../output/output.s
EXE_OUT  = ../output/executable

# ---- Default: build compiler ----
all: $(TARGET)

$(TARGET): parser.tab.c lex.yy.c $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)

# ---- Bison: generate parser ----
parser.tab.c parser.tab.h: parser.y
	bison -d parser.y

# ---- Flex: generate lexer ----
lex.yy.c: lexer.l parser.tab.h
	flex lexer.l

# ---- Run on sample1 ----
run: all
	./$(TARGET) $(SAMPLE) $(IR_OUT)

# ---- Full pipeline: DSL → IR → asm → native ----
ir: run
	@echo ""
	@echo "==> Generating assembly with llc..."
	llc $(IR_OUT) -o $(ASM_OUT)
	@echo "==> Linking with clang..."
	clang $(ASM_OUT) -o $(EXE_OUT)
	@echo ""
	@echo "==> Running executable:"
	$(EXE_OUT)

# ---- Run all test files ----
test: all
	@echo "--- Test: sample1.dsl ---"
	./$(TARGET) ../test/sample1.dsl $(IR_OUT)
	@echo ""
	@echo "--- Test: sample2.dsl ---"
	./$(TARGET) ../test/sample2.dsl $(IR_OUT)
	@echo ""
	@echo "--- Test: sample_error.dsl (expect semantic error) ---"
	-./$(TARGET) ../test/sample_error.dsl $(IR_OUT)

# ---- Clean ----
clean:
	rm -f lex.yy.c parser.tab.c parser.tab.h
	rm -f $(OBJS) $(TARGET)
	rm -f $(IR_OUT) $(ASM_OUT) $(EXE_OUT)

.PHONY: all run ir test clean
