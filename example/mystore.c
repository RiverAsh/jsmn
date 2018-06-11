#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jsmn.h"
#define MAX_SIZE 1024

typedef enum {
	C_European = 0,
	C_Korean = 1
} mycategory_t;

typedef struct {
	mycategory_t cat;
	char name[20];
	char category[20];
	int price;
	char province[20];
} mymenu_t;

void PrintAll(const char *json, jsmntok_t *t, int tok_count);
void jsmn_init(jsmn_parser *parser);
int jsmn_parse(jsmn_parser *parser, const char *js, size_t len, jsmntok_t *tokens, unsigned int num_tokens);
static int jsoneq(const char *json, jsmntok_t *tok, const char *s);
char * ReadJsonFile(const char * filename);
int makemymenu(const char *json, jsmntok_t *t, int tok_count, mymenu_t *m[]);
void Printmenu(mymenu_t *m[], int menucount);

int main() {
	int i;
	int r;
	jsmn_parser p;
	jsmntok_t t[128]; /* We expect no more than 128 tokens */
	char  filename[20];

	mymenu_t *mymenu[20];

	jsmn_init(&p);

	printf("Insert a file Name: ");
	scanf("%s", filename);

	char * JSON_STRING = ReadJsonFile("mymenu.json");
	r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t, sizeof(t) / sizeof(t[0]));

	printf("JSON_STRING = %s\n",JSON_STRING);


	if (r < 0) {
		printf("Failed to parse JSON: %d\n", r);
		return 1;
	}

	if (r < 1 || t[0].type != JSMN_OBJECT) {
		printf("Object expected\n");
		return 1;
	}

	PrintAll(JSON_STRING,  t, r);

	int menucount = makemymenu(JSON_STRING, t, r, mymenu);
	Printmenu(mymenu, menucount);

	return EXIT_SUCCESS;
}

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
		strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}

char * ReadJsonFile(const char * filename) {
	FILE *fp = fopen(filename, "r");
	char *retstr = (char *)malloc(sizeof(char));

	if(fp != NULL) {
		char temp[MAX_SIZE];

		while(fgets(temp, MAX_SIZE, fp) != NULL) {

			retstr = (char *)realloc(retstr, strlen(retstr)+strlen(temp));
			strcat(retstr, temp);
		}
		fclose(fp);
	}
	return retstr;
}

int makemymenu(const char *json, jsmntok_t *t, int tok_count, mymenu_t *m[]) {
	int i = 0;
	int j = 0;
	for (i = 1; i < tok_count; i++) {
		if (jsoneq(json, &t[i], "name") == 0) {
			printf("- name : %.*s\n", t[i+1].end - t[i+1].start, json + t[i+1].start);
			strncpy(m[j]->name, json + t[i+1].start, t[i+1].end - t[i+1].start);
			i++;
		}
		else if (jsoneq(json, &t[i], "category") == 0) {
			printf("- category : %.*s\n", t[i+1].end - t[i+1].start, json + t[i+1].start);
			strncpy(m[j]->category, json + t[i+1].start, t[i+1].end - t[i+1].start);
			i++;
		}
		else if (jsoneq(json, &t[i], "price") == 0) {
			printf("- price : %.*s\n", t[i+1].end - t[i+1].start, json + t[i+1].start);
			char price[20];
			strncpy(price, json + t[i+1].start, t[i+1].end - t[i+1].start);
			m[j]->price = atoi(price);
			i++;
		}
//		else if (jsoneq(json, &t[i], "province") == 0) {
			else{
				printf("%.*s\n", t[i].end - t[i].start, json);

			printf("province ok\n");
	//		if (jsoneq(json, &t[i+1], "European") == 0) {
			if (1) {
				printf("- province : European\n");
				// printf("- province : %.*s\n", t[i+1].end - t[i+1].start, json + t[i+1].start);
				m[j]->cat = 0;
				strncpy(m[j]->province, json + t[i+1].start, t[i+1].end - t[i+1].start);
			}
			else {
				printf("- province : Korean\n");
				// printf("- province : %.*s\n", t[i+1].end - t[i+1].start, json + t[i+1].start);
				m[j]->cat = 1;
				strncpy(m[j]->province, json + t[i+1].start, t[i+1].end - t[i+1].start);
			}
			printf("\n");
			i++;
			j++;
		}

	}
	return j;
}

void PrintAll(const char *json, jsmntok_t *t, int tok_count) {
	int i = 0;
	// char typename [20] = {"JSMN_UNDEFINED", "JSMN_OBJECT", "JSMN_ARRAY", "JSMN_STRING", "JSMN_PRIMITIVE"};
	char typename [20];
	printf("======== All ========");
	// for (i = 1; i < tok_count; i++) {
	// 	#ifdef JSMN_PARENT_LINKS
	// 	printf("[%2d] %.*s (size = %d, %d ~ %d, %s) P = %d\n", i, t[i].end - t[i].start, json + t[i].start, t[i].size, t[i].start, t[i].end, typename[t[i].type], t[i].parent);
	// 	#else
	// 	printf("[%2d] %.*s (size = %d, %d ~ %d, %s)\n", i, t[i].end - t[i].start, json + t[i].start, t[i].size, t[i].start, t[i].end, typename[t[i].type]);
	// 	#endif
	// }

	for (i = 1; i < tok_count; i++) {
		printf("[%d] %.*s (size: %d, %d ~ %d,", i, t[i].end - t[i].start, json + t[i].start, t[i].size, t[i].start, t[i].end);
		switch(t[i].type) {
			case 0: strcpy(typename, "JSMN_UNDEFINED"); break;
			case 1: strcpy(typename, "JSMN_OBJECT"); break;
			case 2: strcpy(typename, "JSMN_ARRAY"); break;
			case 3: strcpy(typename, "JSMN_STRING"); break;
			case 4: strcpy(typename, "JSMN_PRIMITIVE"); break;
			default: break;
		}
		printf("%s) \n", typename);
	}
	printf("\n");
}

void Printmenu(mymenu_t *m[], int menucount) {
	int i = 0;
	printf("====== Menu List ======\n");

	for (i = 0; i < menucount; i++) {
		printf("Menu No.%d\n", i + 1);

		if(m[i]->cat == 0) {
			printf("Province: European Food\n");
		}
		else {
			printf("Province: Korean Food\n");
		}

		printf("menu name: %s\n", m[i]->name);
		printf("menu price: %d\n", m[i]->price);
		printf("menu category: %s\n", m[i]->category);
		printf("\n");
	}
}
