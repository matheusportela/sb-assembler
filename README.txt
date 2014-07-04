===== Compilação =====
Para compilar todos os módulos do projeto, basta usar o comando:
    $ make

===== Uso =====
=> Compilar assembly inventado
Basta usar o comando:
    $ ./bin/assembler <arquivo>.asm <preprocessado>.pre <objeto>.obj

=> Simular arquivo objeto em assembly inventado
Basta usar o comando:
    $ ./bin/simulator<objeto>.obj

=> Traduzir assembly inventado para IA32
Basta usar o comando:
    $ ./bin/tradutor <arquivo>.asm <arquivo_ia32>.s <opcodes_ascii>.txt

Para testar a funcionalidade do arquivo traduzido para IA32, recomenda-se compilar com NASM, ligando-o às bibliotecas LerInteiro.s e EscreverInteiro.s (localizadas na pasta trans/), conforme o exemplo abaixo:

    ./bin/tradutor exemplo.asm exemplo.s exemplo_ops.txt
    nasm -f elf32 exemplo.s
    nasm -f elf32 trans/LerInteiro.s
    nasm -f elf32 trans/EscreverInteiro.s
    ld -m elf_i386 trans/LerInteiro.o trans/EscreverInteiro.o exemplo.o -o exemplo

O script "compile.sh" implementa a solução acima, bastando utilizar da seguinte forma:
    $ ./compile.sh exemplo


=> Carregar opcodes para ELF32
Para carregar o arquivo contendo os opcodes para o formato ELF32, basta utilizar o comando:
    $ ./bin/carregador <opcodes_ascii>.txt <elf32>.elf
    
===== Observações =====
- A tradução para IA32 foi testada e aprovada nos seguintes arquivos:
    * array.asm
    * fatorial.asm
    * fibonacci.asm
    * triangulo.asm
    * zerinho.asm
   
- A geração de opcodes está habilitada para o código fonte em IA32 fornecido ao programa, porém, não para as bibliotecas LerInteiro.s e EscreverInteiro.s.

- Os opcodes implementados estão na forma correta para as instruções do assembly inventado, bem como o parsing de todas as labels, exceto para as instruções de jump e para "call LerInteiro" e "call EscreverInteiro".

- A geração do arquivo em formato ELF32 está correta para qualquer arquivo contendo opcodes da forma correta.
