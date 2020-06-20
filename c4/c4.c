#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define ID_MAX 8
#define NAME_MAX 10
#define PHONE_NUMBER_MAX 11
#define TRUE 1
#define FALSE 0
#define SP ' '
#define ENTER '\n'

enum{
	ID,
	NAME,
	PHONE_NUMBER
};


typedef struct phone_info_t{
	char id[ID_MAX+1];
	char name[NAME_MAX+1];
	char phone_number[PHONE_NUMBER_MAX+1];
}phone_info_t;

typedef struct file_info_t{
	char* start;
	char* end;
}file_info_t;

typedef struct phone_book_t{
	int cnt;
	phone_info_t* list[1024];
}phone_book_t;

file_info_t file_info={0};
phone_book_t phone_book={0};
char* cursor=NULL;

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
	
	file_info.start=contents;
	file_info.end=file_info.start+st.st_size;
	cursor=contents;

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

phone_info_t* get_phone_info()
{
	phone_info_t* phone_info=NULL;
	int flag=ID;
	int idx=0;

	phone_info=(phone_info_t*)calloc(1,sizeof(phone_info));
	if(phone_info==NULL){
		return NULL;
	}

	while(TRUE){
		if(flag==ID){
			phone_info->id[idx]=(*cursor);
		}else if(flag==NAME){
			phone_info->name[idx]=(*cursor);
		}else if(flag==PHONE_NUMBER){
			phone_info->phone_number[idx]=(*cursor);
		}
		cursor++,idx++;	
		if(*cursor==SP) cursor++,flag++,idx=0;	
		if(*cursor==ENTER){
			cursor++;
  			return phone_info;
		}
	}
}
int get_phone_list()
{
	int idx=0;
	while(TRUE){
		if(cursor+1==file_info.end){
			break;
		}
		phone_book.list[idx]=get_phone_info();
		phone_book.cnt++;
		idx++;
	}
	return 0;
}
void free_phone_list()
{
	int idx=0;
	for(idx=0;idx<phone_book.cnt;idx++){
		free(phone_book.list[idx]);
	}
}
void print_phone_info(phone_info_t* phone_info)
{
	printf("info.id:%s \n",phone_info->id);
	printf("info.name:%s \n",phone_info->name);
	printf("info.phone_number:%s \n",phone_info->phone_number);
}
void print_phone_book()
{
	int idx=0;
	for(;idx<phone_book.cnt;idx++){
		print_phone_info(phone_book.list[idx]);
	}
}

int add_phone_number()
{
	phone_info_t* phone_info=NULL;
	int id=0;
	
	phone_info=(struct phone_info_t*)calloc(1,sizeof(phone_info_t));
	if(phone_info==NULL){
		return -1;	
	}

	printf("아이디 숫자 8자리:");
	scanf("%s",phone_info->id);
	printf("이름:");
	scanf("%s",phone_info->name);
	printf("전화번호:");
	scanf("%s",phone_info->phone_number);

	phone_book.list[phone_book.cnt]=phone_info;
	phone_book.cnt++;

}
int search_info()
{
	phone_info_t* phone_info=NULL;
	int idx=0;
	int input1=0;
	char input2[PHONE_NUMBER_MAX]={0};
	printf("검색할 속성 선택(1.아이디, 2.이름, 3.전화번호):");
	scanf("%d", &input1);
	printf("검색할 값:");
	scanf("%s", input2);
	printf("입력값:%s\n", input2);
	for(idx=0;idx<phone_book.cnt;idx++){
		phone_info=phone_book.list[idx];
		if(input1==ID+1){
			if(strcmp(input2,phone_info->id)==0) return idx;
		}else if(input1==NAME+1){
			if(strcmp(input2,phone_info->name)==0) return idx;
		}else if(input1==PHONE_NUMBER+1){
			if(strcmp(input2,phone_info->phone_number)==0) return idx;		
		}else{
			return -1;
		}
	}
}

int delete_info()
{
	int delete_idx=0;
	delete_idx=search_info();
	if(delete_idx==-1){
		return -1;
	}
	phone_book.list[delete_idx]=phone_book.list[phone_book.cnt-1];	
	phone_book.cnt--;
	return 0;
}


int write_file(char* file_path)
{
	int fd=0;
	int idx=0;
	int size=0;

	if(file_path==NULL){
		return -1;

	}
	fd=open(file_path,O_WRONLY);
	if(fd<0){
		return -1;
	}
	

	for(idx=0;idx<phone_book.cnt;idx++){	
		size=strlen(phone_book.list[idx]->id);
		write(fd,phone_book.list[idx]->id,size);
		write(fd," ",1);
		size=strlen(phone_book.list[idx]->name);
		write(fd,phone_book.list[idx]->name,size);
		write(fd," ",1);
		size=strlen(phone_book.list[idx]->phone_number);
		write(fd,phone_book.list[idx]->phone_number,size);
		write(fd,"\n",1);


	}

	close(fd);
}

int main(int argc,char** argv)
{
	file_read_calloc(argv[1]);
	get_phone_list();
	print_phone_book();

	write_file(argv[1]);
	free_phone_list();
	file_read_free();
	
	return 0;
}
