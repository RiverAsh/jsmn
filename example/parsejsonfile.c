#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jsmn.h"
#define MAX_SIZE 1024

void jsmn_init(jsmn_parser *parser);
int jsmn_parse(jsmn_parser *parser, const char *js, size_t len, jsmntok_t *tokens, unsigned int num_tokens);
static int jsoneq(const char *json, jsmntok_t *tok, const char *s);
char * ReadJsonFile(const char * filename);
// char *read_string_from_console();
void PrintAll(const char *json, jsmntok_t *t, int tok_count);
void PrintKeys(const char *json, jsmntok_t *t, int tok_count);
void PrintValues(const char *json, jsmntok_t *t, int tok_count, int *keys);
int FindKeys(const char *json, jsmntok_t *t, int tok_count, int *keys);

int main() {
	int i;
	int r;
	jsmn_parser p;
	jsmntok_t t[128]; /* We expect no more than 128 tokens */
	char  filename[20] = "data.json";

	jsmn_init(&p);

	// printf("Insert a file Name: ");
	// scanf("%s", filename);
	// strncpy(filename, "data.json", 20);

	char * JSON_STRING = ReadJsonFile(filename);
	printf("%s\n",JSON_STRING);
	r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t, sizeof(t) / sizeof(t[0]));

	if (r < 0) {
		printf("Failed to parse JSON: %d\n", r);
		return 1;
	}

	/* Assume the top-level element is an object */
	if (r < 1 || t[0].type != JSMN_OBJECT) {
		printf("Object expected\n");
		return 1;
	}

// #ifdef DEBUG_MODE
// 	printf("r의 값은 %d입니다", r);
// #endif

	/* Loop over all keys of the root object */
	for (i = 1; i < r; i++) {
		if (jsoneq(JSON_STRING, &t[i], "name") == 0) {
			printf("- name: %.*s\n", t[i+1].end - t[i+1].start, JSON_STRING + t[i+1].start);
			i++;
		}

		else if (jsoneq(JSON_STRING, &t[i], "univ") == 0) {
			printf("- univ: %.*s\n", t[i+1].end - t[i+1].start, JSON_STRING + t[i+1].start);
			i++;
		}

		else if (jsoneq(JSON_STRING, &t[i], "major") == 0) {
			printf("- major: %.*s\n", t[i+1].end - t[i+1].start, JSON_STRING + t[i+1].start);
			i++;
		}

		else if (jsoneq(JSON_STRING, &t[i], "group") == 0) {
			int j;
			printf("- groups:\n");
			if(t[i+1].type != JSMN_ARRAY) {
				continue;
			}
			for (j = 0; j < t[i+1].size; j++) {
				jsmntok_t *g = &t[i+j+2];
				printf(" * %.*s\n", g->end - g->start, JSON_STRING + g->start);
			}
			i += t[i+1].size + 1;
		}

		else {
			printf("Unexpected key: %.*s\n", t[i].end - t[i].start, JSON_STRING + t[i].start);
		}
	}

	PrintAll(JSON_STRING, t, r);
	PrintKeys(JSON_STRING, t, r);

	int keyarrays[128], keyamount;
	keyamount = FindKeys(JSON_STRING, t, r, keyarrays);
	printf("Number of Keys = %d\n", keyamount);
	int k = 0;
	for(k = 0; k < keyamount; k++) {
		printf("--> %d\n", keyarrays[k]);
	}

	PrintValues(JSON_STRING, t, r, keyarrays);

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
		fgets(temp, MAX_SIZE, fp);
		temp[strlen(temp)-1] = '\0';

		retstr = (char *)realloc(retstr, strlen(temp)+strlen(retstr));
		strcat(retstr, temp);

		while(!feof(fp)) {
			fgets(temp, MAX_SIZE, fp);
			temp[strlen(temp)-1] = '\0';

			retstr = (char *)realloc(retstr, strlen(retstr)+strlen(temp));
			strcat(retstr, temp);
		}
		fclose(fp);
	}
	return retstr;
}

void PrintAll(const char *json, jsmntok_t *t, int tok_count) {
	int i = 0;
	char typename [5][20] = {"JSMN_UNDEFINED", "JSMN_OBJECT", "JSMN_ARRAY", "JSMN_STRING", "JSMN_PRIMITIVE"};
	printf("======== All ========");
	for (i = 1; i < tok_count; i++) {
		#ifdef JSMN_PARENT_LINKS
		printf("[%2d] %.*s (size = %d, %d ~ %d, %s) P = %d\n", i, t[i].end - t[i].start, json + t[i].start, t[i].size, t[i].start, t[i].end, typename[t[i].type], t[i].parent);
		#else
		printf("[%2d] %.*s (size = %d, %d ~ %d, %s)\n", i, t[i].end - t[i].start, json + t[i].start, t[i].size, t[i].start, t[i].end, typename[t[i].type]);
		#endif
	}
	printf("\n");
}

void PrintKeys(const char *json, jsmntok_t *t, int tok_count) {
	int i = 0;
	int j = 1;
	printf("======== All Keys ========\n");
	for(i = 1; i < tok_count; i++) {
		#ifdef JSMN_PARENT_LINKS
		if(t[i].size == 1 && t[i].type > 2) {
			printf("[%2d] %.*s(%d) P = %d\n", j, t[i].end - t[i].start, json + t[i].start, i, t[i].parent);
			j++;
		}
		#else
		if(t[i].size == 1 && t[i].type > 2) {
			printf("[%2d] %.*s(%d)\n", j, t[i].end - t[i].start, json + t[i].start, i);
			j++;
		}
		#endif
	}
}

void PrintValues(const char *json, jsmntok_t *t, int tok_count, int *keys) {
	int j = 0;
	printf("======== All Values ========");
	for(j = 0; j < tok_count; j++) {
		printf("%.*s : %.*s\n", t[keys[j]].end = t[keys[j]].start, json + t[keys[j]].start, t[keys[j]+1].end - t[keys[j]+1].start, json + t[keys[j]+1].start);
	}
}

int FindKeys(const char *json, jsmntok_t *t, int tok_count, int *keys) {
	int i = 0;
	int j = 0;
	for(i = 1; i < tok_count; i++) {
		if(t[i].size == 1 && t[i].type > 2) {
			keys[j] = i;
			j++;
		}
	}
	return j;
}
