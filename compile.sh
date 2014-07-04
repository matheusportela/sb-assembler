# Translator test
./bin/tradutor test/$1.asm $1.s $1_ops.txt
rm test/$1.asm.pre
rm test/$1.asm.bin
nasm -f elf32 $1.s
nasm -f elf32 trans/LerInteiro.s
nasm -f elf32 trans/EscreverInteiro.s
ld -m elf_i386 trans/LerInteiro.o trans/EscreverInteiro.o $1.o -o $1

# Loader test
./bin/carregador $1_ops.txt $1.elf
