#include <err.h>
#include <fcntl.h>
#include <libelf.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>
#include <unistd.h>

/*O programa simplesmente SAI e manda 42 para o SO*/
unsigned char code[] = {
    0xBB, 0x2A, 0x00, 0x00, 0x00, /* movl $42, %ebx */
    0xB8, 0x01, 0x00, 0x00, 0x00, /* movl $1, %eax */
    0xCD, 0x80            /* int $0x80 */
};

#define LOADADDR    0x08048000 /*endereÁo virtual onde sera carregado o programa*/


/* ELF FORMAT

EHDR: a tabela de cabeÁalho do arquivo ELF com informaÁıes gerais
PHDR: a tablea de cabeÁalho do Programa com informaÁıes especÌficas do programa

+----------------------------------+  <- LOADADDR (0x08048000) EndereÁo a ser carregado (virtual)
|  The ELF Exec Header.            |  
+----------------------------------+
|  The ELF PHDR Table.             |
+----------------------------------+ <- ehdr->e_entry, DEVE apontar a esta regi„o do arquivo.
|  The ".text" section.            |
+----------------------------------+ <- O final da regi„o a ser carregada deste objeto.
|  The section name string table   |    
|  (optional).                     |
+----------------------------------+
|  Section headers:                |
|  - Header for section ".text".   |
|  - Section name string table     |
|    header.                       |
+----------------------------------+
*/

int read_program(char *filename)
{
    FILE *fp = fopen(filename, "r");
    char c;
    
    while(fgets(&c, 1, fp) != NULL)
    {
        printf("%d\n", c);
    }
    
    fclose(fp);
}

void load(char *infile, char *outfile)
{
  int           fd;
  Elf           *e;
  Elf_Scn       *scn;
  Elf_Data      *data;
  Elf32_Ehdr    *ehdr;
  Elf32_Phdr    *phdr;
  Elf32_Shdr    *shdr;
  
  size_t ehdrsz, phdrsz;
  
  if (elf_version(EV_CURRENT) == EV_NONE)
    errx(EX_SOFTWARE,"elf_version is ev_none? %s\n",elf_errmsg(-1));
  if ((fd = open(outfile, O_WRONLY | O_CREAT, 0777)) < 0)
    errx(EX_OSERR, "open %s\n",elf_errmsg(-1));
  if ((e = elf_begin(fd, ELF_C_WRITE, NULL)) == NULL)
    errx(EX_SOFTWARE,"elf_begin %s\n",elf_errmsg(-1));
  if ((ehdr = elf32_newehdr(e)) == NULL)
    errx(EX_SOFTWARE,"elf32_newehdr %s\n",elf_errmsg(-1));

  ehdr->e_version = 1;  

  /*O cÛdigo a ser executado deve ser colocado apÛs a tabela PHDR, logo devemos calcular o tamanho das tabelas EHDR e PHDR*/ 
  ehdrsz = elf32_fsize(ELF_T_EHDR, 1, EV_CURRENT);
  phdrsz = elf32_fsize(ELF_T_PHDR, 1, EV_CURRENT);	
	
  ehdr->e_ident[EI_DATA] = ELFDATA2LSB; /*indica que a codificaÁ„o È complemento de 2 com o byte menos significativo ocupando o endereÁo menor. Pode ser mudado para ELFDATA2MSB onde o byte mais significativo deve ocupar o endereÁo menor*/
  ehdr->e_ident[EI_CLASS] = ELFCLASS32; /*Identifica que eh ELF de 32 bits*/
  ehdr->e_machine = EM_386; /*Identifica a arquitetura necess·ria para rodar o arquivo, neste casso INTEL 80386*/
  ehdr->e_type = ET_EXEC; /*Identifica que È um arquivo executavÈl*/
  ehdr->e_entry = LOADADDR + ehdrsz + phdrsz; /*Aponta o lugar onde comeÁa o cÛdigo (endereÁo virtual + tamanho de EHDR + tamanho de PHDR*/
  
  
  if ((phdr = elf32_newphdr(e,1)) == NULL)
    errx(EX_SOFTWARE,"elf32_newphdr %s\n",elf_errmsg(-1));
  
  if ((scn = elf_newscn(e)) == NULL)
    errx(EX_SOFTWARE,"elf32_newscn %s\n", elf_errmsg(-1));

  if ((data = elf_newdata(scn)) == NULL)
     errx(EX_SOFTWARE,"elf32_newdata %s\n", elf_errmsg(-1));

   /*data È da bibleoteca libelf, usada para colocar os dados nas seÁıes*/
   data->d_align = 1; /*liga a seÁ„o a sua respectiva SHDR*/
   data->d_off = 0LL;
   data->d_buf = code; /*ponteiro ao inicio do cÛdigo*/
   data->d_type = ELF_T_BYTE; /*o cÛdigo vai usar dados no formato ELF_T_BYTE*/
   data->d_size = sizeof(code); /*tamanho do cÛdigo*/
   data->d_version = EV_CURRENT;
  
   if ((shdr = elf32_getshdr(scn)) == NULL)
     errx(EX_SOFTWARE,"elf32_getshdr %s\n", elf_errmsg(-1));

   /*SHDR È a tabela de seÁ„o, cada seÁ„o deve ter sua SHDR para serem ligadas (linking) no arquivo objeto, mas s„o opcionais no arquivo executavÈl*/
   shdr->sh_name = 1;     /*especifica o nome da seÁ„o*/
   shdr->sh_type = SHT_PROGBITS; /*identifica o tipo de seÁ„o, neste caso bits protegidos. Que n„o devem ser ligados nem modificados*/
   shdr->sh_flags = SHF_EXECINSTR | SHF_ALLOC; /*ponteiros que indicam que esta seÁ„o possui dados de execuÁ„o e que devem ser alocados em memÛria*/
   shdr->sh_addr = LOADADDR + ehdrsz + phdrsz;  /*sh_addr È ponteiro para a seÁ„o*/ 	
  
  if ((phdr = elf32_newphdr(e,1)) == NULL)
   errx(EX_SOFTWARE,"elf32_newphdr %s\n", elf_errmsg(-1));

   
   phdr->p_type = PT_LOAD; /*descreve o tipo de segmento a ser interpretado, neste caso: SEGMENTO CARREGAVEL*/
   phdr->p_offset = 0;
   phdr->p_filesz = ehdrsz + phdrsz + sizeof(code); /*indica o tamanho da da imagem, tamanho das tabelas + tamanho do cÛdigo*/
   phdr->p_memsz = phdr->p_filesz; /*tamanho de bytes necess·rio em memÛria para carregar a imagem*/
   phdr->p_vaddr = LOADADDR; /*endereÁo virtual onde o primeiro byte do programa deve estar em memÛria*/
   phdr->p_paddr = phdr->p_vaddr; /*endereÁo fÌsico (normalmente ignorado j· que È utilizado o endereÁo virtual)*/
   phdr->p_align = 4; /*Valores 0 e 1 indicam que n„o precisa de alinhamento, se n„o deve ser potencia de 2*/
   phdr->p_flags = PF_X | PF_R; /*Flags para o tipo TEXT*/

  elf_flagphdr(e, ELF_C_SET, ELF_F_DIRTY);
  
  if (elf_update(e, ELF_C_WRITE) < 0 )
    errx(EX_SOFTWARE,"elf32_update_2 %s\n",elf_errmsg(-1));
  
  elf_end(e);
  close(fd);
  
  return 0;
}