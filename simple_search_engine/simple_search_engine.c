#include "simple_search_engine.h"

int read_file(char* fname)
{
	doc_count++;
	int i = 0;
	char key[MAX_CHAR];
	key[0] = 0;
	char c;
	int prev_c = 1;
	FILE* ifp;
	if ((ifp = fopen(fname, "r")) == NULL) {
		printf("No such file !\n");
		exit(1);
	}
	while (fscanf(ifp, "%c", &c) == 1) {// (key data)를 읽어 해시테이블에 삽입
		//printf("%s\n", key);
		c = tolower(c);
		if (is_alpha(c))
			ft_strchar(key, c);
		else if (ft_is_space(c) && key[0])
		{
			hash_insert(key, fname, i);
			key[0] = 0;
		}
		if (ft_is_space(c) && prev_c)
			i++;
		prev_c = !ft_is_space(c);
	}
	fclose(ifp);
	return(i);
}

int	ft_is_space(char c)
{
	if (c == ' ')
		return (1);
	if (c == '\t')
		return (1);
	if (c == '\n')
		return (1);
	if (c == '\r' || c == '\f' || c == '\v')
		return (1);
	return (0);
}

void ft_strchar(char* str, char c)
{
	char* p = str;
	while (*p)
		p++;
	*p = c;
	*(p + 1) = 0;
}

int is_alpha(char c)
{
	if (c >= 'a' && c <= 'z')
		return 1;
	if (c >= 'A' && c <= 'Z')
		return 1;
	return 0;
}

void hash_insert(char* key, char* fname, int i)
{
	list_ptr new_list = (list_ptr)malloc(sizeof(list));
	int idx = hash(key);
	int j = 0;
	list_ptr curr = hash_table[idx];
	int over = 1;
	strcpy(new_list->item.key, key);
	strcpy(new_list->item.doc, fname);
	new_list->item.word_idx = i;
	new_list->link = NULL;
	if (!curr)
	{
		hash_table[idx] = new_list;
		word_count++;
		return;
	}

	if (ft_strcmp(curr->item.key, key) == 0)
	{
		comp_count++;
		while (curr->link)
			curr = curr->link;
		curr->link = new_list;
		return;
	}
	else
	{
		while (hash_table[idx + ++j] && ft_strcmp(hash_table[idx + j]->item.key, key) != 0) comp_count++;
		if (!hash_table[idx + j])
		{
			hash_table[idx + j] = new_list;
			word_count++;
			return;
		}
		curr = hash_table[idx + j];
		while (curr->link)
			curr = curr->link;
		curr->link = new_list;
	}
}

//해시 함수(folding + division(TABLE_SIZE 로 나눈 나머지))
/*int hash(char* key)
{
	return transform(key) % TABLE_SIZE;
}*/

unsigned long hash(char* str)
{
	unsigned long hash = 0;
	int c;

	while (c = *str++)
		hash = c + (hash << 6) + (hash << 16) - hash;

	return hash % TABLE_SIZE;
}

/*int hash(char* str) {
	int hash = 401;
	int c;

	while (*str) {
		hash = ((hash << 4) + (int)(*str)) % TABLE_SIZE;
		str++;
	}

	return hash % TABLE_SIZE;
}*/

// folding (key의 각 character 값을 더함)
int transform(char* key)
{
	int num = 0;
	while (*key)
		num += *key++;
	return num;
}

void show_hash_table()
{
	list_ptr curr;
	for (int i = 0; i < TABLE_SIZE; i++)
	{
		curr = hash_table[i];
		if (curr)
		{
			printf("hash %d : ", i);
			while (curr)
			{
				printf("%s ", curr->item.key);
				curr = curr->link;
			}
			printf("\n");
		}
	}
}

void search(char* word)
{
	int idx = hash(word);
	int j = 0;
	int comp = 0;
	printf("search word : %s\n\n", word);
	if (!hash_table[idx])
		return;
	while (ft_strcmp(hash_table[idx + j]->item.key, word) != 0)
	{
		comp++;
		j++;
	}
	list_ptr curr = hash_table[idx + j];
	while (curr)
	{
		prt_word(curr->item);
		printf("\n");
		curr = curr->link;
	}
	printf("Total number of comparison : %d\n", comp);
}

void prt_word(element word)
{
	int i = 0;
	char key[MAX_CHAR];
	FILE* ifp;
	if ((ifp = fopen(word.doc, "r")) == NULL) {
		printf("No such file !\n");
		exit(1);
	}
	printf("<%s>\n", word.doc);
	while (fscanf(ifp, "%s", key) == 1) {
		if (word.word_idx - i == 4 || word.word_idx - i == -4)
			printf("... ");
		if (word.word_idx - i >= -3 && word.word_idx - i <= 3)
			printf("%s ", key);
		i++;
	}
	printf("\n");
	fclose(ifp);
}

void prt_index()
{
	printf("Total number of documents : %d\n", doc_count);
	printf("Total number of indexed words: %d\n", word_count);
	printf("Total number of comparison : %d\n", comp_count);
}

int test_count_word()
{
	int ret = 0;
	for (int i = 0; i < TABLE_SIZE; i++)
		if (hash_table[i])
			ret++;
	return ret;
}

int ft_strcmp(char* str1, char* str2)
{
	strlwr(str1);
	strlwr(str2);
	return strcmp(str1, str2);
}

int main()
{
	char filename[11];
	for (int i = 1; i <= 4; i++)
	{
		sprintf(filename, "doc%03d.txt", i);
		read_file(filename);
	}

	show_hash_table();
	prt_index();
	//printf("test word : %d\n", test_count_word());
	//printf("used hash : %d\n", hash("used"));
	//printf("\n----------------------------\n");
	search("computer");

	//printf("%u", hash("computer"));
}