#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(){
	int fd1 = open("./dup_example_data", O_CREAT | O_WRONLY, 0755);
	write(fd1, "Hello\n", strlen("Hello\n"));
	
	int fd2 = dup(fd1);
	write(fd2, "Hi\n", strlen("Hi\n"));
	close(fd1);
	write(fd2, "nihao\n", strlen("nihao\n"));
	close(fd2);

	return 0;
}
