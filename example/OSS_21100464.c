#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jsmn.h"
#define MAX_SIZE 1024

typedef struct {
	char model[20];
  char maker[20];
  int year;
	char gastype[20];
} car_t;

static int jsoneq(const char *json, jsmntok_t *tok, const char *s);
char * readjsonfile(const char * fileName);
void printcarlist(car_t*m[], int count);
int makecarlist(const char *json, jsmntok_t *t, int r, car_t* m[]);

void jsmn_init(jsmn_parser *parser);
int jsmn_parse(jsmn_parser *parser, const char *js, size_t len,jsmntok_t *tokens, unsigned int num_tokens);

int main() {
	int keyarray[128],keyamount;
	int i;
	int r;
	int car_count;
	jsmn_parser p;
	jsmntok_t t[128]; /* We expect no more than 128 tokens */
	car_t *car[5];

	for(i = 0; i < 20; i++) {
		car[i] = (car_t*)malloc(sizeof(car_t));
	}

	char fileName[20] = "cars.json";

	jsmn_init(&p);

	char *JSON_STRING = readjsonfile(fileName);
	r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t, sizeof(t) / sizeof(t[0]));

	int k;
	car_count = makecarlist(JSON_STRING, t, r, car);
	// printf("\n\n\n car_count : %d\n", car_count);

	printcarlist(car, car_count);
	return EXIT_SUCCESS;
}

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start && strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}

char * readjsonfile(const char * filename) {
  FILE *fp = fopen(filename, "r");  //file stream open
	char *retstr = (char *)malloc(sizeof(char)); //retstr memory allocation

	if(fp != NULL) {
		char temp[MAX_SIZE];
		while(fgets(temp, MAX_SIZE, fp) != NULL) {  //while !EOF
			retstr = (char *)realloc(retstr, strlen(retstr)+strlen(temp)); //memory re-allocation with strlen(retstr)+strlen(temp) size
			strcat(retstr, temp);  //cat all lines of the file into one string
		}
		fclose(fp);
	}
	return retstr;
}

void printcarlist(car_t*m[], int count) { //구조체 배열의 내용 출력
	int i = 0;
  printf("------ model\tmaker\tyear\tgastype\n\n");
	for(i = 0; i < count; i++) {
		printf("[%d]%10s", i+1, m[i]->model);
		printf("%12s", m[i]->maker);
    printf("%6d", m[i]->year);
		printf("%10s\n", m[i]->gastype);
		printf("\n");
	}
}

int makecarlist(const char *json, jsmntok_t *t, int r, car_t* m[]) {  //구조체 배열에 내용을 담고, 그 개수를 리턴
	int size = 0;
	int i = 0, j = 0, car_count = 0;
	char year_int[20];

  for (i = 1; i < r; i++) {
    if (t[i+1].type == 2) { //JSMN_ARRAY type
			size = t[i+1].size;

			i += 4;

      for (j = car_count; j < car_count + size; i += 3, j++) {
        strncpy(m[j]->model, json + t[i].start, t[i].end - t[i].start);
				i += 2; //토큰 건너 뛰기
        strncpy(m[j]->maker, json + t[i].start, t[i].end - t[i].start);
				i += 2; //토큰 건너 뛰기
				strncpy(year_int, json + t[i].start, t[i].end - t[i].start);
       	m[j]->year = atoi(year_int);
        strncpy(m[j]->gastype, json + t[i+2].start, t[i+2].end - t[i+2].start); //토큰 건너 뛰기가 되지 않아 i+2 후 파싱 & 내용 저장
				i += 2;
      }
			i -= 5;  //구조체 배열의 첫번째로 복귀
      car_count += size;  //현재 구조체를 건너뛰고, 다음 구조체를 위해 size 만큼 더한다
		}
  }
  return car_count;
}
