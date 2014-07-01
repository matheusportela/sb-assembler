./bin/tradutor test/$1.asm $1.s
nasm -f elf32 $1.s
nasm -f elf32 trans/LerInteiro.s
nasm -f elf32 trans/EscreverInteiro.s
ld -m elf_i386 $1.o LerInteiro.o EscreverInteiro.o -o $1
