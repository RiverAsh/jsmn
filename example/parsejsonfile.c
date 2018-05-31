#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../jsmn.h"
#define MAX_SIZE 1024

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
		strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}

char * ReadString(const char * fileName) {
	// char  a[20];
	// char * b;
	// FILE *f1;
	// f1 = fopen(fileName, "r");
	//
	// b = (char *)malloc(sizeof(char));
	// //a=(char*)malloc(sizeof(char));
	//
	// while (1) {
	// 	fgets(a, sizeof(a), f1);
	// 	if (feof(f1))break;
	// 	b = (char*)realloc(b, strlen(b) + strlen(a));
	// 	//printf("\n%d\n",strlen(a));
	// 	strncat(b, a, strlen(a));
	// }
	//
	// fclose(f1);
	// return b;
	char *retstr;
	FILE *fp = fopen(fileName, "r");

	if(fp! = NULL) {
		char temp[MAX_SIZE];
		fgets(temp, MAX_SIZE, fp);
		temp[strlen(temp)-1] = '\0';

		retstr = (char *)malloc(strlen(temp));
		strncpy(retstr, temp, strlen(temp));

		while(!feof(fp)) {
			fgets(temp, MAX_SIZE, fp);
			temp[strlen(temp) - 1] = '\0';

			retstr = (char *)realloc(retstr, strlen(retstr) + strlen(temp));
			strncpy(retstr + strlen(retstr), temp, strlen(temp));
		}
		fclose(fp);
	}
	return retstr;
}

/*
char *read_string_from_console() {
	char temp[MAX_SIZE];
	fgets(temp, MAX_SIZE, stdin);
	temp[strlen(temp)-1] = '\0';

	char *retstr = (char *) malloc(strlen(temp));
	strncpy(retstr, temp, strlen(temp));

	while((fgets(temp, MAX_SIZE, stdin))[0] != '\n') {
		temp[strlen(temp)-1] = '\0';
		retstr = (char *)realloc(retstr, strlen(temp));
		strncpy(retstr+strlen(retstr), temp, strlen(temp));
	}
}
*/

void printall(const char *json, jsmntok_t *t, int tok_count) {
	int i = 0;
	char t_type[20];

	for (i = 1; i < tok_count; i++) {
		printf("[%d] %.*s (size: %d, %d~%d,", i, t[i].end - t[i].start, json + t[i].start, t[i].size, t[i].start, t[i].end);

		switch (t[i].type) {
		case 0: strcpy(t_type, "JSMN_UNDEFIEND"); break;
		case 1: strcpy(t_type, "JSMN_OBJECT"); break;
		case 2: strcpy(t_type, "JSMN_ARRAY"); break;
		case 3: strcpy(t_type, "JSMN_STRING"); break;
		case 4: strcpy(t_type, "JSMN_PRIMITIVE"); break;
		default:  break;
		}

		printf("%s) \n", t_type);
	}
}

void jsmn_init(jsmn_parser *parser);
int jsmn_parse(jsmn_parser *parser, const char *js, size_t len, jsmntok_t *tokens, unsigned int num_tokens);

int main() {
	int i;
	int r;
	jsmn_parser p;
	jsmntok_t t[128]; /* We expect no more than 128 tokens */
	char  fileName[20];

	jsmn_init(&p);

	printf("Insert a file Name:");
	// scanf("%s", fileName);
	strncpy(fileName, "data.json", 20);

	char * JSON_STRING = ReadString(fileName);
	//	printf("%s\n",JSON_STRING);
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

#ifdef DEBUG_MODE
	printf("r의 값은 %d입니다", r);
#endif

	/* Loop over all keys of the root object */
	for (i = 1; i < r; i++) {
		if (jsoneq(JSON_STRING, &t[i], "name") == 0) {
			/* We may use strndup() to fetch string value */
			//printf("%.*s",t[i].end-t[i].start,
						  //             JSON_STRING + t[i].start);
			printf("- Name: %.*s\n", t[i + 1].end - t[i + 1].start,
				JSON_STRING + t[i + 1].start);
			i++;

#ifdef DEBUG_MODE
			printf("User의 token은 %d번째입니다", i);
#endif
		}

		else if (jsoneq(JSON_STRING, &t[i], "keywords") == 0) {
			/* We may additionally check if the value is either "true" or "false" */
			printf("- Keywords: %.*s\n", t[i + 1].end - t[i + 1].start,
				JSON_STRING + t[i + 1].start);
			i++;

#ifdef DEBUG_MODE
			printf("Admin의 token은 %d번째입니다", i);
#endif
		}

		else if (jsoneq(JSON_STRING, &t[i], "description") == 0) {
			/* We may want to do strtol() here to get numeric value */
			printf("- Description: %.*s\n", t[i + 1].end - t[i + 1].start,
				JSON_STRING + t[i + 1].start);
			i++;

#ifdef DEBUG_MODE
			printf("Uid의 token은 %d번째입니다", i);
#endif
		}

		else if (jsoneq(JSON_STRING, &t[i], "type") == 0) {
			/* We may want to do strtol() here to get numeric value */
			printf("- type: %.*s\n", t[i + 1].end - t[i + 1].start,
				JSON_STRING + t[i + 1].start);
			i++;

#ifdef DEBUG_MODE
#endif
		}

		else if (jsoneq(JSON_STRING, &t[i], "url") == 0) {
			/* We may want to do strtol() here to get numeric value */
			printf("- url: %.*s\n", t[i + 1].end - t[i + 1].start,
				JSON_STRING + t[i + 1].start);
			i++;
#ifdef DEBUG_MODE
			printf("Uid의 token은 %d번째입니다", i);
#endif
		}

		else if (jsoneq(JSON_STRING, &t[i], "frameworks") == 0) {
			/* We may want to do strtol() here to get numeric value */
			printf("- Frameworks: %.*s\n", t[i + 1].end - t[i + 1].start,
				JSON_STRING + t[i + 1].start);
			i++;

#ifdef DEBUG_MODE
			printf("Uid의 token은 %d번째입니다", i);
#endif
		}

		else if (jsoneq(JSON_STRING, &t[i], "platforms") == 0) {
			/* We may want to do strtol() here to get numeric value */
			printf("- Platfroms: %.*s\n", t[i + 1].end - t[i + 1].start,
				JSON_STRING + t[i + 1].start);
			i++;

#ifdef DEBUG_MODE
			printf("Uid의 token은 %d번째입니다", i);
#endif
		}

		else if (jsoneq(JSON_STRING, &t[i], "exclude") == 0) {
			/* We may want to do strtol() here to get numeric value */
			printf("- Excludes: %.*s\n", t[i + 1].end - t[i + 1].start,
				JSON_STRING + t[i + 1].start);
			i++;

#ifdef DEBUG_MODE
			printf("Uid의 token은 %d번째입니다", i);
#endif
		}

		else if (jsoneq(JSON_STRING, &t[i], "examples") == 0) {
			int j;
			printf("- Exampleis:\n");
			if (t[i + 1].type != JSMN_ARRAY) {
				continue; /* We expect groups to be an array of strings */
			}

			for (j = 0; j < t[i + 1].size; j++) {
				jsmntok_t *g = &t[i + j + 2];
				printf("  * %.*s\n", g->end - g->start, JSON_STRING + g->start);
			}

			i += t[i + 1].size + 1;
		}

		else if (jsoneq(JSON_STRING, &t[i], "repository") == 0) {
			int j;

			printf("- Repository:\n");

			if (t[i + 1].type != JSMN_ARRAY) {
				continue; /* We expect groups to be an array of strings */
			}

			for (j = 0; j < t[i + 1].size; j++) {
				jsmntok_t *g = &t[i + j + 2];
				printf("  * %.*s\n", g->end - g->start, JSON_STRING + g->start);
			}

			i += t[i + 1].size + 1;

#ifdef DEBUG_MODE
			printf("Object인 groups의  token은 %d번째입니다", i);
#endif

		}

		else {
			printf("Unexpected key: %.*s\n", t[i].end - t[i].start,
				JSON_STRING + t[i].start);
		}
	}

	printall(JSON_STRING, t, r);

	return EXIT_SUCCESS;
}
