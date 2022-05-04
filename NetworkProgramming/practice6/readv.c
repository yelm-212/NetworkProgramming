/* readv.c */
#include <stdio.h>
#include <sys/uio.h>
#define BUF_SIZE 100

int main(int argc, char *argv[])
{
	struct iovec vec[2];
	char buf1[BUF_SIZE]={0,};
	char buf2[BUF_SIZE]={0,};
	int str_len;

	vec[0].iov_base=buf1;
	vec[0].iov_len=5;
	vec[1].iov_base=buf2;
	vec[1].iov_len=BUF_SIZE;

	str_len=readv(0, vec, 2);
	printf("Read bytes: %d\n", str_len);
	printf("First message: %s\n", buf1); 
	// newline 까지 포함해 5개 문자 - 4개 문자를 받아들일 수 있는 문자열 buf1
	printf("Second message: %s\n", buf2);
	// BUF_SIZE -1 개 문자를 받아들일 수 있는 문자열 buf2
	return 0;
}