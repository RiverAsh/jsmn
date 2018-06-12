#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jsmn.h"

typedef enum {
 C_European = 0,
 C_Korean = 1
} mycategory_t;

typedef struct {
	mycategory_t cat;
	char name[20];
	char category[10];
	int price;
} mymenu_t;

static int jsoneq(const char *json, jsmntok_t *tok, const char *s);
char * ReadStringFile(const char * fileName);
void PrintAll(const char *json,jsmntok_t *t,int tok_count);
void PrintMenu(mymenu_t*m[], int count);
int FindKeys(const char *json, jsmntok_t *t, int tok_count, int * keys);
void PrintKeys(const char *json, jsmntok_t *t, int tok_count);
int makemymenu(const char *json, jsmntok_t *t, int r, mymenu_t* m[]);
int Order(mymenu_t *m[],int count);

void jsmn_init(jsmn_parser *parser);
int jsmn_parse(jsmn_parser *parser, const char *js, size_t len,jsmntok_t *tokens, unsigned int num_tokens);

int main() {
	int keyarray[128],keyamount;
	int i;
	int r;
	int menu_count;
	jsmn_parser p;
	jsmntok_t t[128]; /* We expect no more than 128 tokens */
	mymenu_t *mymenu[20];

	for(i = 0; i < 20; i++) {
		mymenu[i] = (mymenu_t*)malloc(sizeof(mymenu_t));
	}

	char fileName[20];

	jsmn_init(&p);

	printf("Insert a file Name : ");
	scanf("%s", fileName);

	char *JSON_STRING = ReadStringFile(fileName);
	r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t, sizeof(t) / sizeof(t[0]));

	keyamount = FindKeys(JSON_STRING,t,r,keyarray);
	PrintAll(JSON_STRING,t,r);
	PrintKeys(JSON_STRING,t,r);
	printf("Keyamount is %d\n",keyamount);
	int b=0;
	for (b=0; b < keyamount; b++) {
		printf("--> %d\n", keyarray[b]);
	}

	int k;
	menu_count = makemymenu(JSON_STRING, t, r, mymenu);
	printf("\n\n\n menu_count : %d\n", menu_count);

	PrintMenu(mymenu, menu_count);
	Order(mymenu, menu_count);
	return EXIT_SUCCESS;
}

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start && strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}

char * ReadStringFile(const char * fileName) {
	char a[20];
	char *b;
	FILE *f1;
	f1 = fopen(fileName, "r");

	b = (char*)malloc(sizeof(char));
	//a = (char*)malloc(sizeof(char));

	while(1){
	  fgets(a, sizeof(a), f1);
		if(feof(f1)) {
			break;
		}

	  b = (char*)realloc(b, strlen(b)+strlen(a));
	  //printf("\n%d\n", strlen(a));
	  strncat(b, a, strlen(a));
	}

	fclose(f1);
	return b;
}

void PrintAll(const char *json, jsmntok_t *t, int tok_count) {
	int i = 0;
	char t_type[20];
	for(i = 1; i < tok_count; i++) {
		printf("[%d] %.*s (size: %d, %d~%d,", i, t[i].end-t[i].start, json + t[i].start, t[i].size, t[i].start, t[i].end);

		switch(t[i].type) {
			case 0: strcpy(t_type,"JSMN_UNDEFIEND"); break;
			case 1: strcpy(t_type,"JSMN_OBJECT"); break;
			case 2: strcpy(t_type,"JSMN_ARRAY"); break;
			case 3: strcpy(t_type,"JSMN_STRING"); break;
			case 4: strcpy(t_type,"JSMN_PRIMITIVE"); break;
			default: break;
		}

		printf("%s) \n", t_type);
	}
}

void PrintMenu(mymenu_t*m[], int count) {
	int i = 0;
	for(i = 0; i < count; i++) {
		switch(m[i]->cat) {
			case 0: printf("- European\n"); break;
			case 1: printf("- Korean\n"); break;
		}

		printf("- name  : %s\n", m[i]->name);
		printf("- area  : %s\n", m[i]->category);
		printf("- price : %d\n", m[i]->price);
		printf("\n");
	}
}

int FindKeys(const char *json, jsmntok_t *t, int tok_count, int * keys) {
	int i, j = 0;
	for (i = 1; i < tok_count; i++) {
		if (t[i].size >= 1 && t[i].type == 3) {
			keys[j++] = i;
		}
	}
	return j;
}

void PrintKeys(const char *json, jsmntok_t *t, int tok_count) {
	int i, j = 0;
	printf("********All Keys*******\n");
	for (i = 1; i < tok_count; i++) {
		if (t[i].size < 1 || t[i].type != 3) {
			continue;
		}
		else {
			j++;
			printf("\n[%d] %.*s (%d)", j, t[i].end - t[i].start, json + t[i].start, i);
		}
	}
	puts("");
}

int makemymenu(const char *json, jsmntok_t *t, int r, mymenu_t* m[]) {
	int size = 0;
	int i = 0, j = 0, menu_count = 0;
	int type = 0;
	char price_int[20];

  for (i = 1; i < r; i++) {
    if (t[i+1].type == 2) {
			if (!strncmp("European", json + t[i].start, t[i].end - t[i].start)){
				type=0;
			}
			else if (!strncmp("Korean", json + t[i].start, t[i].end-t[i].start)){
				type=1;
			}
			else {
				type =0;
			}

			size = t[i+1].size;

			i += 4;

      for (j = menu_count; j < menu_count + size; i += 3, j++) {
				m[j]->cat = type;
        strncpy(m[j]->name, json + t[i].start, t[i].end - t[i].start);
				i += 2;
        strncpy(m[j]->category, json + t[i].start, t[i].end - t[i].start);
				i += 2;
				strncpy(price_int, json + t[i].start, t[i].end - t[i].start);
       	m[j]->price = atoi(price_int);
      }
			i -= 3;
      menu_count += size;
		}
  }
  return menu_count;
}

int Order(mymenu_t *m[], int count) {
	int answer;
	int receipt = 0;
	int i = 0;

	while(1) {
		//ask
		printf("Would you like to order? (1. Yes  2. No, Pay Bill)\n");
		scanf("%d", &answer);

		if (answer == 2) {
			break;
		}

		while (1) {
			for (i = 0; i < count; i++) {
				printf("[%d]\tname : %s\n\t%s\n\t%d Won\n\n", i+1, m[i]->name, m[i]->category, m[i]->price);
			}

			printf("Select a number of food you want to eat : ");
			scanf("%d", &answer);

			if (answer < 1 || answer > count) {
				printf("Type right index please.......\n");
			}
			else {
				break;
			}
		}

		receipt += m[answer-1]->price;
	}

	printf("Your cost is %d Won \n", receipt);
	return receipt;
}
