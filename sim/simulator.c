/**
 * @file   simulator.c
 * @author Matheus Vieira Portela
 * @author Lucas de Levy Oliveira
 * @date   30/05/2014
 *
 * @brief  Simulator for pseudo-assembly language.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <object_file.h>

#define DEBUG 0

obj_t memory[1000];
short int acc = 0; /* short int register */
uint16_t pc = 0; /* 16 bit program counter */

void object_file_read(char *filename, object_file_t *object_ptr)
{
    FILE *fp = fopen(filename, "rb");
    int i;
    
    fread(&object_ptr->size, sizeof(int), 1, fp); /* First word is the program size */
    fread(&object_ptr->text_section_address, sizeof(int), 1, fp); /* Second word is the text section address */
    
    object_ptr->program = malloc(sizeof(obj_t)*object_ptr->size);
    fread(object_ptr->program, sizeof(obj_t), object_ptr->size, fp);
    
    printf("\n===== %s =====\n\n", filename);
    for (i = 0; i < object_ptr->size; ++i)
        printf("(addr. %d): %d\n", i, object_ptr->program[i]);
    printf("\n==========\n");
    
    fclose(fp);
}

void add()
{
    int addr = memory[pc + 1];
    if (DEBUG) 
        printf("acc += %d (addr %d)\n", memory[addr], addr);
    acc += memory[addr];
    pc += 2;
}

void sub()
{
    int addr = memory[pc + 1];
    if (DEBUG) 
        printf("acc -= %d (addr %d)\n", memory[addr], addr);
    acc -= memory[addr];
    pc += 2;
}

void mul()
{
    int addr = memory[pc + 1];
    if (DEBUG) 
        printf("acc *= %d (addr %d)\n", memory[addr], addr);
    acc *= memory[addr];
    pc += 2;
}

void division()
{
    int addr = memory[pc + 1];

    if (memory[addr] == 0)
    {
        fprintf(stderr, "Runtime error: Division by 0\n");
        exit(1);
    }

    if (DEBUG) 
        printf("acc /= %d (addr %d)\n", memory[addr], addr);
    acc /= memory[addr];
    pc += 2;
}

void jmp()
{
    if (DEBUG) 
        printf("jmp %d\n", memory[pc + 1]);
    pc = memory[pc + 1];
}

void jmpn()
{
    if (DEBUG) 
        printf("jmpn %d\n", memory[pc + 1]);
    if (acc < 0)
        pc = memory[pc + 1];
    else
        pc += 2;
}

void jmpp()
{
    if (DEBUG) 
        printf("jmpp %d\n", memory[pc + 1]);
    if (acc > 0)
        pc = memory[pc + 1];
    else
        pc += 2;
}

void jmpz()
{
    if (DEBUG) 
        printf("jmpz %d\n", memory[pc + 1]);
    if (acc == 0)
        pc = memory[pc + 1];
    else
        pc += 2;
}

void copy()
{
    int addr_from = memory[pc + 1];
    int addr_to = memory[pc + 2];
    if (DEBUG) 
        printf("copy %d -> %d\n", addr_from, addr_to);
    memory[addr_to] = memory[addr_from];
    pc += 3;
}

void load()
{
    int addr = memory[pc + 1];
    if (DEBUG) 
        printf("acc = %d (addr %d)\n", memory[addr], addr);
    acc = memory[addr];
    pc += 2;
}

void store()
{
    int addr = memory[pc + 1];
    if (DEBUG) 
        printf("acc -> %d (addr %d)\n", memory[addr], addr);
    memory[addr] = acc;
    pc += 2;
}

void input()
{
    int addr = memory[pc + 1];
    if (DEBUG) 
        printf("input (addr %d): ", addr);
    printf("input: ");
    scanf("%hd", &memory[addr]);
    pc += 2;
}

void output()
{
    int addr = memory[pc + 1];
    if (DEBUG) 
        printf("output (addr %d): %hd\n", addr, memory[addr]);
    printf("%d\n", memory[addr]);
    pc += 2;
}

int main(int argc, char **argv)
{
    char *filename;
    int i;
    int stop_flag = 0;
    object_file_t obj;
    
    /* Parse command line arguments */
    if (argc != 2)
    {
        fprintf(stderr, "ERROR: Wrong number of arguments\n");
        fprintf(stderr, "Usage: simulator <input>\n");
        exit(-1);
    }
    filename = argv[1];
    
    /* Load program to the memory */
    object_file_read(filename, &obj);
    printf("Loading program... ");
    for (i = 0; i < obj.size; ++i)
        memory[i] = obj.program[i];
    printf("OK!\n\n");
    
    /* Setting PC to the text section address */
    pc = obj.text_section_address;
    
    while (stop_flag != 1)
    {
        if (DEBUG)
        {
            printf("\n=== Registers ===\n");
            printf("ACC: %d\n", acc);
            printf("PC: %d\n", pc);
            printf("=====\n\n");
        }
        
        switch (obj.program[pc])
        {
            case 0x1:
                add();
                break;
            case 0x2:
                sub();
                break;
            case 0x3:
                mul();
                break;
            case 0x4:
                division();
                break;
            case 0x5:
                jmp();
                break;
            case 0x6:
                jmpn();
                break;
            case 0x7:
                jmpp();
                break;
            case 0x8:
                jmpz();
                break;
            case 0x9:
                copy();
                break;
            case 0xA:
                load();
                break;
            case 0xB:
                store();
                break;
            case 0xC:
                input();
                break;
            case 0xD:
                output();
                break;
            case 0xE:
                stop_flag = 1;
                break;
            default:
                fprintf(stderr, "ERROR: Unknown instruction\n");
                exit(2);
        }
    }
    
    return 0;
}