#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


#define ID_MAX 8
#define NAME_MAX 10
#define PHONE_NUMBER_MAX 11

typedef struct phone_info_t{
	int id[ID_MAX];
	char name[NAME_MAX];
	char phone_number[PHONE_NUMBER_MAX];
	struct phone_info_t* next;
}phone_info_t;

typedef struct file_info_t{
	char* start;
	char* end;
}file_info_t;

file_info_t file_info={0};

int file_read_calloc(char* file_path)
{
	int fd=0;
	struct stat st={0};
	char* contents=NULL;
	size_t nbytes=0;
	
	
	
	if(file_path==NULL){
		printf("파일 패스가 널");
		return -1;	
	}
	if(stat(file_path,&st)==-1){
		printf("파일 정보 실패");
		return -1;
	}
	/*파일 열기*/
	fd=open("test",O_RDONLY);
	if(fd<0){
		printf("파일 열기 실패\n");
		return -1;
	}
	/*저장할 공간*/
	contents=(char*)calloc(1,st.st_size);
	if(contents==NULL){
		printf("콜록 실패");
		close(fd);
		return -1;
	}
	/*read contensts*/
	nbytes=read(fd,contents,st.st_size);
	if(nbytes<0){
		printf("읽기 실패");
		close(fd);
		return -1;
	}
	
	printf("내용:%s",contents);
	file_info.start=contents;
	file_info.end=file_info.start+st.st_size;


	close(fd);
	return 0;
}


int file_read_free()
{
	if(file_info.start==NULL){
		return -1;
	}
	free(file_info.start);
	return 0;
}


int main(void)
{
	file_read_calloc("test");
	printf("%s",file_info.start);

	file_read_free();
	
	return 0;
}
