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
	list_ptr link;					//해쉬값이 같고 단어도 같을 경우 link로 이어진다
	list_ptr next;					//depth값은 즉 next를 타고 들어간 횟수를 말한다.
}	list;
list hash_table[TABLE_SIZE];

typedef struct dinfo {				//해당 문서에 search word가 포함된 횟수를 저장하는 구조체
	int doc;
	int appearance;
} dinfo;

int doc_count = 0;		//총 문서의 수
int word_count = 0;		//총 단어의 수
int comp_count = 0;		//comparison수
int total_comp = 0;		//평균 search comparison을 위한 변수
int comparison;			//최종 insert에 사용된 comparison 수

int read_file(int fnum);								//파일을 읽은뒤 단어의 정보를 저장하는 함수
void hash_insert(char* key, int fnum, int word_idx);	//읽은 단어를 해쉬 테이블에 저장하는 함수
unsigned long hash(char* str, int depth);				//depth를 기반으로 rehash도 할 수 있는 hash함수

void show_search(char* word);							//show_hash_table함수를 위한 search함수
void show_hash_table();									//모든 단어를 search한 뒤 평균 search comparison을 구하는 함수

void prt_word(element word);							//word를 앞 뒤로 3단어를 포함하여 출력하는 함수
void search(char* word);
void quick_sort(dinfo list[], int left, int right);		//단어수가 가장 많은 문서부터 정렬

void free_util(list_ptr curr);
void free_all();										//이전 코드에서 해준 모든 동적할당을 해제해주는 함수

char ft_tolower(char c);								//c가 대문자일 경우 소문자로 변환해주는 함수
int is_alpha(char c);									//c가 알파벳인지 아닌지	판별해주는 함수
void ft_strchar(char* str, char c);						//str뒤에 c를 붙이는 함수
int ft_strcmp(char* str1, char* str2);					//strcmp의 응용함수
int	ft_is_space(char c);								//c가 space문자인지 아닌지 판별해주는 함수
int ft_is_it(char* str);								//str이 정상적인 string인지 판별해주는 함수