all: linker a6.exe a6.o

linker:  linker.c
	gcc -o linker -std=c99 -g linker.c
a6.exe: a6main.o addints.o data.o rbsywt.o readint.o writeint.o writestr.o xbsywt.o
	./linker rbsywt.o xbsywt.o writestr.o writeint.o a6main.o addints.o data.o readint.o > a6.exe
a6.o:   a6main.o addints.o data.o rbsywt.o readint.o writeint.o writestr.o xbsywt.o
	./linker -o rbsywt.o xbsywt.o writestr.o writeint.o a6main.o addints.o data.o readint.o > a6.o
a6main.o: a6main.asm
	./masm_mrd -o < a6main.asm > a6main.o
writestr.o: writestr.asm
	./masm_mrd -o < writestr.asm > writestr.o
readint.o: readint.asm
	./masm_mrd -o < readint.asm > readint.o
addints.o: addints.asm
	./masm_mrd -o < addints.asm > addints.o
writeint.o: writeint.asm
	./masm_mrd -o < writeint.asm > writeint.o
rbsywt.o: rbsywt.asm
	./masm_mrd -o < rbsywt.asm > rbsywt.o
xbsywt.o: xbsywt.asm
	./masm_mrd -o < xbsywt.asm > xbsywt.o
data.o: data.asm
	./masm_mrd -o < data.asm > data.o	
clean:
	rm linker
	rm -f *.o a6.exe
