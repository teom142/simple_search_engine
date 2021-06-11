/*simple_search_engine.h*/
#pragma once
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_CHAR 100				//최대 단어 길이
#define TABLE_SIZE 65535			//해쉬 테이블 사이즈
#define MAX_DOC 100					//최대 문서의 수

typedef struct {
	char key[MAX_CHAR];
	int doc;						//단어가 위치한 문서의 index
	int word_idx;					//해당 문서에서 단어의 위치
} element;

typedef struct list* list_ptr;
typedef struct list {
	element item;
	list_ptr link;					//같은 거
	list_ptr next;					//다음번 거
}	list;
list hash_table[TABLE_SIZE];

typedef struct dinfo {
	int doc;
	int appearance;
} dinfo;

int doc_count = 0;
int word_count = 0;
int comp_count = 0;
int total_comp = 0;
int comparison;

int read_file(int fnum);
void hash_insert(char* key, int fnum, int word_idx);
unsigned long hash(char* str, int depth);
void show_search(char* word);
int transform(char* key);
void show_hash_table();
void prt_word(element word);
void search(char* word);
int is_alpha(char c);
void ft_strchar(char* str, char c);
int ft_strcmp(char* str1, char* str2);
int	ft_is_space(char c);
void quick_sort(dinfo list[], int left, int right);
int ft_is_it(char* str);
void free_util(list_ptr curr);
void free_all();