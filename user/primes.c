#include "kernel/types.h"
#include "user/user.h"

int num_count = 34;
int num_size = 4;

void func(int *input, int num){
    //管道中还剩下一个数
	if(num == 1){
		printf("prime %d\n", *input);
		return;
	}
	int p[2],i;
	int prime = *input;
	int temp;
	printf("prime %d\n", prime);
	pipe(p);
    if(fork() == 0){
        for(i = 0; i < num; i++){
            temp = *(input + i);
			write(p[1], (char *)(&temp), num_size);
		}
        exit(0);
    }
	close(p[1]);
	if(fork() == 0){
        //指针代表往后移动的个数
		int ptr = 0;
        
		char buffer[num_size];
		while(read(p[0], buffer, num_size) != 0){
			temp = *((int *)buffer);
			if(temp % prime != 0){
				*input = temp;
				input += 1;
				ptr++;
			}
		}
		func(input - ptr, ptr);
		exit(0);
    }
	wait(0);
	wait(0);
}

int main(){
    int input[num_count];
	int i = 0;
	for(; i < num_count; i++){
		input[i] = i+2;
	}
	func(input, num_count);
    exit(0);
}
