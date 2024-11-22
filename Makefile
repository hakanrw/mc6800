.PHONY: all
all: 68h obj/out

68h: main.c chips/chips/mc6800.h
	$(CC) $< -o $@

obj/out: obj/code.p
	p2bin obj/code.p bin
	mv bin.bin obj/out

obj/code.p: code.asm
	asl code.asm -cpu 6800
	mv code.p obj

.PHONY: clean
clean:
	rm -rf obj 68h
	mkdir obj

.PHONY: run
run: all
	./68h

.PHONY: test
test: 68h
	sh ./test.sh
