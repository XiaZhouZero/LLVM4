#LLVM_HOME=path_to_llvm_build
LLVM_HOME=/home/zhouxia/LLVM4/build
CC=$(LLVM_HOME)/bin/clang
DIS=$(LLVM_HOME)/bin/llvm-dis
OPT=$(LLVM_HOME)/bin/opt
SIMPLEPASS=$(LLVM_HOME)/lib/LLVMObfuscation.so

game.out: game.c
	$(CC) $^ -o $@

game.bc: game.c
	$(CC) -S -emit-llvm $^ -o $@
	#$(DIS) $@ -o game.ll

after_game.bc: game.bc
	$(OPT) -S -load $(SIMPLEPASS) -Simple < $^ > $@
	#$(CC) -S -emit-llvm -gamepass $^ -o $@

after_game.ll: after_game.bc
	$(DIS) $^ -o $@

all: game game.bc after_game.bc

clean:
	rm ./game
	rm ./game.bc
