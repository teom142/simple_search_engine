/*simple_search_engine.c*/
#pragma warning(disable:4996)
#include "simple_search_engine.h"

int read_file(int fnum)
{
	int i = 0;
	char key[MAX_CHAR];
	key[0] = 0;
	char c;
	int prev_c = 1;
	FILE* ifp;

	char fname[11];
	sprintf(fname, "doc%03d.txt", fnum);

	if ((ifp = fopen(fname, "r")) == NULL) {
		return;
	}
	doc_count++;
	while (fscanf(ifp, "%c", &c) == 1) {	//(key data)�� �о� �ؽ����̺� ����
		c = ft_tolower(c);						//c�� �ϳ��� �ҹ��ڷ� �ٲ۴�
		if (is_alpha(c))
			ft_strchar(key, c);				//c�� ���ĺ��� ��� key�� �����Ѵ�.
		else if (ft_is_space(c) && key[0])	//key�� ���𰡰� ����Ǿ���, ���鹮�� �� �ܾ ������ ���ڰ� ������
		{
			hash_insert(key, fnum, i);		//���ݱ��� ����� key���� �ؽ� ���̺� �����Ѵ�
			key[0] = 0;						//key �ʱ�ȭ
		}
		if (ft_is_space(c) && prev_c)		//���� �ܾ ���԰�, ���� c�� ����ܾ �ƴҰ�� �ܾ� index�� �� i�� ������Ų��.
			i++;
		prev_c = !ft_is_space(c);
	}
	fclose(ifp);
	return(i);
}

void hash_insert(char* key, int fnum, int i)
{
	list_ptr new_list = (list_ptr)malloc(sizeof(list));
	int idx = hash(key, 0);
	list_ptr curr = &hash_table[idx];
	strcpy(new_list->item.key, key);
	new_list->item.doc = fnum;
	new_list->item.word_idx = i;
	new_list->link = NULL;
	new_list->next = NULL;
	int depth = 0;
	list_ptr tmp = NULL;

	while (1) {
		if (!ft_is_it(curr->item.key))			//curr��尡 ������� ��� Ȥ�� ���̳���� ���
		{
			if (curr->next)						//���� next������ ���� ���
				new_list->next = curr->next;	//�� ������ �̾ �޾ƿ�
			*curr = *new_list;
			free(new_list);
			word_count++;
			return;
		}
		else if (ft_strcmp(curr->item.key, key) == 0)	//���� curr�� ��ġ�� item.key�� �����Ϸ��� key�� ���ٸ�
		{
			while (curr->link)
				curr = curr->link;
			curr->link = new_list;						//link�� ��� Ÿ�� �� ������ ��ġ�� �߰����ش�
			return;
		}
		else {
			idx = hash(key, ++depth);		//depth������ �̿��� rehash
			curr = &hash_table[idx];
			int j = 0;
			for (; (curr->next) && (j < depth); j++) curr = curr->next; //rehash�� ��ġ�� next node�� ���� ��� depth���� ���� ������ �̵��� �ݺ��Ѵ�.
			for (; j < depth; j++) {						//depth�� �������� ���߰�, next node�� �������
				list_ptr nextspace = (list_ptr)malloc(sizeof(list));	//���� ��带 ����� depth�� ������ ������ �̵��Ѵ�
				nextspace->next = NULL;
				curr->next = nextspace;						//���̳��� �̾����� �ϱ� ������ curr->next�� ���� ���� �̾��ش�
				curr = curr->next;
			}
		}
	}
}

void quick_sort(dinfo list[], int left, int right)
{
	int i = left, j = right + 1;
	dinfo temp;
	if (left < right) {

		dinfo pivot = list[left];
		while (i < j) {
			while (list[++i].appearance > pivot.appearance);
			while (list[--j].appearance < pivot.appearance);
			if (i < j) {
				temp = list[i];
				list[i] = list[j];
				list[j] = temp;
			}
		}
		temp = list[j];
		list[j] = list[left];
		list[left] = temp;

		quick_sort(list, left, j - 1);
		quick_sort(list, j + 1, right);
	}
}

unsigned long hash(char* str, int depth)
{
	unsigned long hash = 0;
	int c;

	while (c = *str++) {
		c += depth;				//depth �Ű� ������ �̿��� rehash�Ѵ�
		hash = c + (hash << 6) + (hash << 16) - hash;
	}

	return hash % TABLE_SIZE;
}

void show_hash_table()
{
	list_ptr curr = &hash_table[0];
	int depth = 0;
	int i = -1, j = 0;
	int count = 0;
	do {
		i = -1; count = 0;
		while (i < TABLE_SIZE) {
			if (depth == 2)
				depth = 2;
			curr = &hash_table[++i];

			for (j = 0; j < depth; j++) {
				curr = curr->next;
				if (!curr) break;
			}

			if (j != depth) continue;

			if (!ft_is_it(curr->item.key)) continue; // strlen(curr->item.key) == 0 || strlen(curr->item.key) > MAX_CHAR) continue;

			if (curr) show_search(curr->item.key);
			while (curr)
			{
				count++;
				curr = curr->link; //���� ��
			}
		}
		depth++;
	} while (count != 0);
	printf("\n==== END OF PRINT ====\n");
	printf("Search Comparison Average : %.4f\n\n", (double)total_comp / (double)word_count);
	printf("TABLE SIZE : %d\n", TABLE_SIZE);
}

void show_search(char* word)
{
	int idx = hash(word, 0);
	int curr_depth = 0;
	comp_count = 0;
	list_ptr curr = &hash_table[idx];
	int prt_file = 1;

	dinfo doc_transition[MAX_DOC + 1] = { 0 };

	while (curr && ft_strcmp(curr->item.key, word) != 0)
	{
		curr_depth++;
		idx = hash(word, curr_depth);
		curr = &hash_table[idx];
		for (int i = 0; curr && i < curr_depth; i++)
			curr = curr->next;
	}

	list_ptr tmp = curr;

	for (tmp = curr; tmp; tmp = tmp->link) {
		doc_transition[tmp->item.doc].appearance++;
		doc_transition[tmp->item.doc].doc = tmp->item.doc;
	}
	quick_sort(doc_transition, 1, MAX_DOC);
	for (int i = 1; doc_transition[i].appearance >= 0; i++) {
		if (doc_transition[i].appearance == 0) continue;
		tmp = curr;
		prt_file = 1;
		char fname[11] = "";
		while (tmp)
		{
			if (tmp->item.doc == doc_transition[i].doc) {
			}
			tmp = tmp->link;
		}
	}
	total_comp += comp_count;
}

void search(char* word)
{
	int idx = hash(word, 0);
	int curr_depth = 0;
	comp_count = 0;
	list_ptr curr = &hash_table[idx];
	int prt_file = 1;

	printf("search word : %s\n\n", word);

	dinfo doc_transition[MAX_DOC + 1] = { 0 };

	while (curr && ft_strcmp(curr->item.key, word) != 0)
	{
		curr_depth++;
		idx = hash(word, curr_depth);		//depth�� �÷����� �ܾ Ž��
		curr = &hash_table[idx];
		for (int i = 0; curr && i < curr_depth; i++)
			curr = curr->next;
	}
	if (!curr)	//curr�� null�� �����ߴٴ� ���� search�ϴ� �ܾ ���� �ܾ��� ��
	{
		printf("%s is not exist in document\n", word);
		return;
	}
	list_ptr tmp = curr;

	for (tmp = curr; tmp; tmp = tmp->link) {		//ã�� �ܾ link�� Ÿ�� ���鼭 ��� doc_transition�� ����
		doc_transition[tmp->item.doc].appearance++;
		doc_transition[tmp->item.doc].doc = tmp->item.doc;
	}
	quick_sort(doc_transition, 1, MAX_DOC);			//������ �� ���
	for (int i = 1; doc_transition[i].appearance >= 0; i++) {
		if (doc_transition[i].appearance == 0) continue;
		tmp = curr;
		prt_file = 1;
		char fname[11] = "";
		while (tmp)
		{
			if (tmp->item.doc == doc_transition[i].doc) {
				sprintf(fname, "doc%03d.txt", tmp->item.doc);
				if (prt_file)		//prt_file������ Ÿ��Ʋ�� ����ߴ����� ���� ������ ��� ����
				{
					printf("%s (%s : %d)\n", fname, tmp->item.key, doc_transition[i].appearance);	//Ÿ��Ʋ ���
					prt_file = 0;
				}
				prt_word(tmp->item);
				printf("\n");
			}
			tmp = tmp->link;
		}
	}
	printf("Total number of comparison : %d\n", comp_count);
}

void prt_word(element word)
{
	int i = 0;
	char key[MAX_CHAR];
	FILE* ifp;
	char fname[200];

	sprintf(fname, "doc%03d.txt", word.doc);
	if ((ifp = fopen(fname, "r")) == NULL) {
		printf("No such file !\n");
		return;
	}
	while (fscanf(ifp, "%s", key) == 1) {
		if (word.word_idx - i == 4 || word.word_idx - i == -4)		//����Ϸ��� �ܾ�� 4�ܾ� ���̳��� ��ġ�� ��� ...���� ���
			printf("... ");
		if (word.word_idx - i >= -3 && word.word_idx - i <= 3)		//����Ϸ��� �ܾ�� 3�ܾ� ���Ϸ� ���̳��� ���
		{
			if (word.word_idx - i == 0)
				printf("\x1b[91m" "%s " "\x1b[0m", key);			//�ش� �ܾ� ��� �� ����
			else
				printf("%s ", key);
		}
		i++;
	}
	printf("\n");
	fclose(ifp);
}

void prt_index()
{
	printf("Total number of documents : %d\n", doc_count);
	printf("Total number of indexed words: %d\n", word_count);
	printf("Total number of comparison : %d\n", comparison);
}

void free_all()
{
	list_ptr curr;

	for (int i = 0; i < TABLE_SIZE; i++)
	{
		curr = &hash_table[i];
		free_util(curr->next);
		free_util(curr->link);
	}
}

void free_util(list_ptr curr)			//����Լ��� ������ �̿��� �Ҵ����� ������ ��� free
{
	if (!curr)							//���� free�ϰ��� �ϴ� ������ ������ NULL�̸� �Լ� ����
		return;
	if (!ft_is_it(curr->item.key))		//���� ����� ���
	{
		if (curr->next)					//���� ����� next�� ���� ���
			free_util(curr->next);
		free(curr);
		return;
	}
	if (curr->next)						//next���� ������ next node�� free
		free_util(curr->next);
	if (curr->link)
		free_util(curr->link);
	free(curr);							//���������� ���� ������ ���� free
}

int is_alpha(char c)					//���ĺ����� �˷��ִ� �Լ�
{
	if (c >= 'a' && c <= 'z')
		return 1;
	if (c >= 'A' && c <= 'Z')
		return 1;
	return 0;
}

int	ft_is_space(char c)					//space���� �� ���鹮���̸� 1 ��ȯ
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

void ft_strchar(char* str, char c)		//str�ڷ� c�� �ϳ� �� ���̴� �Լ�
{
	char* p = str;
	while (*p)
		p++;
	*p = c;
	*(p + 1) = 0;
}

int ft_is_it(char* str)					//���� str�� ���� ����� str���� Ȯ���ϱ� ���� �Լ�
{
	if (!*str)
		return 0;
	if (!(*str >= 'a' && *str <= 'z'))
		return 0;
	return 1;
}

int ft_strcmp(char* str1, char* str2)	//strcmp�� �����Լ�
{
	comp_count++;						//�� �Լ��� ����� Ƚ���� �� comparison�� ��
	strlwr(str1);						//�� ���ڿ��� ��� �ҹ��ڷ� ��ȯ
	strlwr(str2);
	return strcmp(str1, str2);			//���������� strcmp�� ��ȯ
}

char ft_tolower(char c)
{
	if (c >= 'A' && c <= 'Z')
		c += 'a' - 'A';
	return c;
}

int main()
{
	for (int i = 1; i <= MAX_DOC; i++) read_file(i);
	comparison = comp_count;
	show_hash_table();
	prt_index();
	char search_word[MAX_CHAR] = "";

	while (1) {
		printf("�˻��� �ܾ �Է��ϼ��� (���� ���� \"#\") : ");
		scanf("%s", search_word);
		if (!strcmp("#", search_word))
			break;
		strlwr(search_word);
		search(search_word);
	}
	free_all();
}