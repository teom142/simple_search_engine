#pragma warning(disable:4996)
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
	int comp = 0;
	list_ptr new_list = (list_ptr)malloc(sizeof(list));
	int idx = hash(key);
	list_ptr curr = &hash_table[idx];
	strcpy(new_list->item.key, key);
	strcpy(new_list->item.doc, fname);
	new_list->item.word_idx = i;
	new_list->link = NULL;
	new_list->next = NULL;
	int depth = 0;
	list_ptr tmp = NULL;

	while (1) {
		if (strlen(curr->item.key) == 0 || strlen(curr->item.key) > MAX_CHAR)
		{
			if (curr->next)
				new_list->next = curr->next;
			*curr = *new_list;
			free(new_list);
			word_count++;
			return;
		}
		else if (ft_strcmp(curr->item.key, key) == 0)
		{
			new_list->link = curr->link;
			curr->link = new_list;
			/*while (curr->link)
				curr = curr->link;
			curr->link = new_list;*/
			return;
		}
		else {
			idx = hashoverride(key, ++depth);
			curr = &hash_table[idx];
			int j = 0;
			for (; (curr->next) && (j < depth); j++) curr = curr->next; //만들어진 곳 끝까지 감
			for (; j < depth; j++) { //새로 만듬
				list_ptr nextspace = (list_ptr)malloc(sizeof(list));
				nextspace->next = NULL;
				curr->next = nextspace;
				curr = curr->next;
			}
		}
	}
}

unsigned long hash(char* str)
{
	unsigned long hash = 0;
	int c;

	while (c = *str++)
		hash = c + (hash << 6) + (hash << 16) - hash;

	return hash % TABLE_SIZE;
}

unsigned long hashoverride(char* str, int depth)
{
	unsigned long hash = 0;
	int c;

	while (c = *str++) {
		c += depth;
		hash = c + (hash << 6) + (hash << 16) - hash;
	}

	return hash % TABLE_SIZE;
}

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
	list_ptr curr = &hash_table[0];
	int depth = 0;
	int i = -1, j = 0;
	int count = 0;
	do {
		i = -1; count = 0;
		printf("\n[[[ Depth : %d ]]]\n\n", depth);
		while (i < TABLE_SIZE) {
			if (depth == 2)
				depth = 2;
			curr = &hash_table[++i];

			for (j = 0; j < depth; j++) {
				curr = curr->next;
				if (!curr) break;
			}

			if (j != depth) continue;
			
			if (strlen(curr->item.key) == 0 || strlen(curr->item.key) > MAX_CHAR) continue;

			printf("hash %d : ", i);
			while (curr)
			{
				count++;
				printf("%s ", curr->item.key);
				curr = curr->link; //같을 때
			}
			printf("\n");
		}
		depth++;
	} while (count != 0);
	printf("\n==== END OF PRINT ====\n");
}

void search(char* word)
{
	int idx = hashoverride(word, 0);
	int j = 0;
	int curr_depth = 0;
	comp_count = 0;
	list_ptr curr = &hash_table[idx];
	int is_it = 0;
	printf("search word : %s\n\n", word);
	while (curr && ft_strcmp(curr->item.key, word) != 0)
	{
		curr_depth++;
		idx = hashoverride(word, curr_depth);
		curr = &hash_table[idx];
		for (int i = 0; curr && i < curr_depth; i++)
			curr = curr->next;
	}
	while (curr)
	{
		is_it = 1;
		prt_word(curr->item);
		printf("\n");
		curr = curr->link;
	}
	if (is_it)
		printf("Total number of comparison : %d\n", comp_count);
	else
		printf("%s is not exist in document\n", word);
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

int ft_strcmp(char* str1, char* str2)
{
	comp_count++;
	strlwr(str1);
	strlwr(str2);
	return strcmp(str1, str2);
}

int main()
{
	char filename[11];
	for (int i = 1; i <= 10; i++)
	{
		sprintf(filename, "doc%02d.txt", i);
		read_file(filename);
	}

	show_hash_table();
	prt_index();
	//printf("test word : %d\n", test_count_word());
	//printf("used hash : %d\n", hash("used"));
	//printf("\n----------------------------\n");
	char search_word[MAX_CHAR];
	
	while (1) {
		printf("검색할 단어를 입력하세요 : ");
		scanf("%s", search_word);
		strlwr(search_word);
		search(search_word);
	}

	//printf("%u", hash("computer"));
}