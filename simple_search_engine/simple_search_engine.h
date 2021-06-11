/*simple_search_engine.h*/
#pragma once
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_CHAR 100				//�ִ� �ܾ� ����
#define TABLE_SIZE 65535			//�ؽ� ���̺� ������
#define MAX_DOC 100					//�ִ� ������ ��

typedef struct {
	char key[MAX_CHAR];
	int doc;						//�ܾ ��ġ�� ������ index
	int word_idx;					//�ش� �������� �ܾ��� ��ġ
} element;

typedef struct list* list_ptr;
typedef struct list {
	element item;
	list_ptr link;					//�ؽ����� ���� �ܾ ���� ��� link�� �̾�����
	list_ptr next;					//depth���� �� next�� Ÿ�� �� Ƚ���� ���Ѵ�.
}	list;
list hash_table[TABLE_SIZE];

typedef struct dinfo {				//�ش� ������ search word�� ���Ե� Ƚ���� �����ϴ� ����ü
	int doc;
	int appearance;
} dinfo;

int doc_count = 0;		//�� ������ ��
int word_count = 0;		//�� �ܾ��� ��
int comp_count = 0;		//comparison��
int total_comp = 0;		//��� search comparison�� ���� ����
int comparison;			//���� insert�� ���� comparison ��

int read_file(int fnum);								//������ ������ �ܾ��� ������ �����ϴ� �Լ�
void hash_insert(char* key, int fnum, int word_idx);	//���� �ܾ �ؽ� ���̺� �����ϴ� �Լ�
unsigned long hash(char* str, int depth);				//depth�� ������� rehash�� �� �� �ִ� hash�Լ�

void show_search(char* word);							//show_hash_table�Լ��� ���� search�Լ�
void show_hash_table();									//��� �ܾ search�� �� ��� search comparison�� ���ϴ� �Լ�

void prt_word(element word);							//word�� �� �ڷ� 3�ܾ �����Ͽ� ����ϴ� �Լ�
void search(char* word);
void quick_sort(dinfo list[], int left, int right);		//�ܾ���� ���� ���� �������� ����

void free_util(list_ptr curr);
void free_all();										//���� �ڵ忡�� ���� ��� �����Ҵ��� �������ִ� �Լ�

char ft_tolower(char c);								//c�� �빮���� ��� �ҹ��ڷ� ��ȯ���ִ� �Լ�
int is_alpha(char c);									//c�� ���ĺ����� �ƴ���	�Ǻ����ִ� �Լ�
void ft_strchar(char* str, char c);						//str�ڿ� c�� ���̴� �Լ�
int ft_strcmp(char* str1, char* str2);					//strcmp�� �����Լ�
int	ft_is_space(char c);								//c�� space�������� �ƴ��� �Ǻ����ִ� �Լ�
int ft_is_it(char* str);								//str�� �������� string���� �Ǻ����ִ� �Լ�