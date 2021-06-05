#pragma once
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_CHAR 100
#define TABLE_SIZE 65569
#define DEPTH 10

typedef struct {
	char key[MAX_CHAR];
	char doc[10]; // 몇 번째 document
	int word_idx; // 몇 번째 
} element;

typedef struct list* list_ptr;
typedef struct list {
	element item;
	list_ptr link; //같은 거
	list_ptr next; //다음번 거
}	list;
list hash_table[TABLE_SIZE];

typedef struct head* head_ptr;
typedef struct head {
	list_ptr linked_head;
	head_ptr next;
} head;
head_ptr mainp;


int doc_count = 0;
int word_count = 0;
int comp_count = 0;

int read_file(char* fname);
void hash_insert(char* key, char* fname, int word_idx);
unsigned long hash(char* str);
int transform(char* key);
void show_hash_table();
void prt_word(element word);
void search(char* word);
int is_alpha(char c);
void ft_strchar(char* str, char c);
int ft_strcmp(char* str1, char* str2);
int	ft_is_space(char c);