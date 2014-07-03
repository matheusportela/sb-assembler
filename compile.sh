# Translator test
./bin/tradutor test/$1.asm $1.s
nasm -f elf32 $1.s
nasm -f elf32 trans/LerInteiro.s
nasm -f elf32 trans/EscreverInteiro.s
ld -m elf_i386 $1.o trans/LerInteiro.o trans/EscreverInteiro.o -o $1
#ld -m elf_i386 $1.o trans/LerInteiro.o -o $1
#ld -m elf_i386 $1.o trans/EscreverInteiro.o -o $1
readelf -x .text $1 > $1_dump.txt
readelf -x .data $1 >> $1_dump.txt

# Loader test
#./bin/carregador $1.s $1.elf
