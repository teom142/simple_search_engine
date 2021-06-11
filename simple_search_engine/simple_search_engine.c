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
	while (fscanf(ifp, "%c", &c) == 1) {	//(key data)를 읽어 해시테이블에 삽입
		c = ft_tolower(c);						//c를 하나씩 소문자로 바꾼다
		if (is_alpha(c))
			ft_strchar(key, c);				//c가 알파벳일 경우 key에 저장한다.
		else if (ft_is_space(c) && key[0])	//key에 무언가가 저장되었고, 공백문자 즉 단어를 나누는 문자가 나오면
		{
			hash_insert(key, fnum, i);		//지금까지 저장된 key값을 해시 테이블에 삽입한다
			key[0] = 0;						//key 초기화
		}
		if (ft_is_space(c) && prev_c)		//공백 단어가 나왔고, 이전 c가 공백단어가 아닐경우 단어 index가 될 i를 증가시킨다.
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
		if (!ft_is_it(curr->item.key))			//curr노드가 비어있을 경우 혹은 더미노드일 경우
		{
			if (curr->next)						//기존 next연결이 있을 경우
				new_list->next = curr->next;	//그 연결을 이어서 받아옴
			*curr = *new_list;
			free(new_list);
			word_count++;
			return;
		}
		else if (ft_strcmp(curr->item.key, key) == 0)	//만약 curr의 위치의 item.key가 삽입하려는 key와 같다면
		{
			while (curr->link)
				curr = curr->link;
			curr->link = new_list;						//link를 계속 타고 들어가 마지막 위치에 추가해준다
			return;
		}
		else {
			idx = hash(key, ++depth);		//depth변수를 이용해 rehash
			curr = &hash_table[idx];
			int j = 0;
			for (; (curr->next) && (j < depth); j++) curr = curr->next; //rehash한 위치의 next node가 있을 경우 depth보다 적은 선에서 이동을 반복한다.
			for (; j < depth; j++) {						//depth에 도달하지 못했고, next node가 없을경우
				list_ptr nextspace = (list_ptr)malloc(sizeof(list));	//더미 노드를 만드며 depth에 도달할 때까지 이동한다
				nextspace->next = NULL;
				curr->next = nextspace;						//더미노드와 이어져야 하기 때문에 curr->next를 더미 노드와 이어준다
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
		c += depth;				//depth 매개 변수를 이용해 rehash한다
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
				curr = curr->link; //같을 때
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
		idx = hash(word, curr_depth);		//depth를 늘려가며 단어를 탐색
		curr = &hash_table[idx];
		for (int i = 0; curr && i < curr_depth; i++)
			curr = curr->next;
	}
	if (!curr)	//curr이 null에 도달했다는 뜻은 search하는 단어가 없는 단어라는 뜻
	{
		printf("%s is not exist in document\n", word);
		return;
	}
	list_ptr tmp = curr;

	for (tmp = curr; tmp; tmp = tmp->link) {		//찾은 단어를 link로 타고 들어가면서 계속 doc_transition에 저장
		doc_transition[tmp->item.doc].appearance++;
		doc_transition[tmp->item.doc].doc = tmp->item.doc;
	}
	quick_sort(doc_transition, 1, MAX_DOC);			//퀵정렬 후 출력
	for (int i = 1; doc_transition[i].appearance >= 0; i++) {
		if (doc_transition[i].appearance == 0) continue;
		tmp = curr;
		prt_file = 1;
		char fname[11] = "";
		while (tmp)
		{
			if (tmp->item.doc == doc_transition[i].doc) {
				sprintf(fname, "doc%03d.txt", tmp->item.doc);
				if (prt_file)		//prt_file변수는 타이틀을 출력했는지에 대한 정보가 담긴 변수
				{
					printf("%s (%s : %d)\n", fname, tmp->item.key, doc_transition[i].appearance);	//타이틀 출력
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
		if (word.word_idx - i == 4 || word.word_idx - i == -4)		//출력하려는 단어와 4단어 차이나는 위치일 경우 ...으로 출력
			printf("... ");
		if (word.word_idx - i >= -3 && word.word_idx - i <= 3)		//출력하려는 단어와 3단어 이하로 차이나면 출력
		{
			if (word.word_idx - i == 0)
				printf("\x1b[91m" "%s " "\x1b[0m", key);			//해당 단어 출력 색 수정
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

void free_util(list_ptr curr)			//재귀함수의 성질을 이용해 할당해준 값들을 모두 free
{
	if (!curr)							//현재 free하고자 하는 포인터 변수가 NULL이면 함수 종료
		return;
	if (!ft_is_it(curr->item.key))		//더미 노드일 경우
	{
		if (curr->next)					//더미 노드의 next가 있을 경우
			free_util(curr->next);
		free(curr);
		return;
	}
	if (curr->next)						//next값이 있으면 next node도 free
		free_util(curr->next);
	if (curr->link)
		free_util(curr->link);
	free(curr);							//마지막으로 현재 포인터 변수 free
}

int is_alpha(char c)					//알파벳임을 알려주는 함수
{
	if (c >= 'a' && c <= 'z')
		return 1;
	if (c >= 'A' && c <= 'Z')
		return 1;
	return 0;
}

int	ft_is_space(char c)					//space문자 즉 공백문자이면 1 반환
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

void ft_strchar(char* str, char c)		//str뒤로 c를 하나 더 붙이는 함수
{
	char* p = str;
	while (*p)
		p++;
	*p = c;
	*(p + 1) = 0;
}

int ft_is_it(char* str)					//현재 str이 더미 노드의 str인지 확인하기 위한 함수
{
	if (!*str)
		return 0;
	if (!(*str >= 'a' && *str <= 'z'))
		return 0;
	return 1;
}

int ft_strcmp(char* str1, char* str2)	//strcmp의 응용함수
{
	comp_count++;						//이 함수를 사용한 횟수가 곧 comparison의 수
	strlwr(str1);						//두 문자열을 모두 소문자로 변환
	strlwr(str2);
	return strcmp(str1, str2);			//최종적으로 strcmp값 반환
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
		printf("검색할 단어를 입력하세요 (종료 문자 \"#\") : ");
		scanf("%s", search_word);
		if (!strcmp("#", search_word))
			break;
		strlwr(search_word);
		search(search_word);
	}
	free_all();
}