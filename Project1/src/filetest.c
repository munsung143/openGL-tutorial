#include <stdio.h>

// 파일 경로를 정의하는 매크로
#define RSC_PATH(file) "../Project1/rsc/"#file

void fscanfTest() {
	FILE* file = fopen(RSC_PATH(Text.txt), "r");
	if (file != NULL) {
		int a;
		float b;
		char c;
		int rat = fscanf(file, "%c %d %f", &c, &a, &b);
		printf("%c %d %f\n", c, a, b);
		fclose(file);
	}
	else {
		perror("file not found");
	}
}

void fgetsTest() {
	FILE* file = fopen(RSC_PATH(Text.txt), "r");
	if (file != NULL) {
		char buffer[256];
		char* get = fgets(buffer, sizeof(buffer), file);
		printf("%s", buffer);
		printf("%s", get);
		printf("%c\n", buffer[0]);
		get = fgets(buffer, sizeof(buffer), file);
		printf("%s", buffer);
		printf("%s", get);
		printf("%c", buffer[0]);
		fclose(file);
	}
	else {
		perror("file not found");
	}
}