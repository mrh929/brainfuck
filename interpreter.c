#include<stdio.h>
#include<string.h>
#define MAX 1000000

FILE *f;
char source[MAX], *input_s;
int read_file(char *name){
	f = fopen(name, "r");
	if(f == NULL) return 1;
	fscanf(f, "%s", source);
	return 0;
}

int input_cnt(){
	char *s = source;
	int cnt = 0;
	while(*s){
		if(*s == ',') cnt++;
		s++;
	}
	return cnt;
}

int run(){
	int source_len = strlen(source);
	int eip = 0; // pointer EIP
	int esp = 0; // pointer of "stack"
	int eax = 0; // pointer of input_string
	int stack[1000];
	memset(stack, 0, sizeof(stack));
	
	while(1){
		if(eip == source_len) return 0;
		switch(source[eip]){
			case '>':{
				esp++;
				break;
			}
			
			case '<':{
				esp--;
				if(esp < 0) //segment fault
					return 1;
				break;
			}
			
			case '+':{
				stack[esp]++;
				break;
			}
			
			case '-':{
				stack[esp]--;
				break;
			}
			
			case '.':{  //output a letter
				printf("%c", stack[esp]);
				break;
			}
			
			case ',':{  //input a letter
				stack[esp] = input_s[eax++]; 
				break;
			}
			
			case '[':{
				if(stack[esp] == 0){
					int required_brackets = 0;
					while(1){
						if(source[eip] == '[') 
							required_brackets++;
						else if(source[eip] == ']') 
							required_brackets--;
						
						if(required_brackets == 0)
							break;
						eip++;
					}
				}
				break;
			}
			
			case ']':{
				int required_brackets = 0;
				while(1){
					if(source[eip] == ']') 
						required_brackets++;
					else if(source[eip] == '[') 
						required_brackets--;
						
					eip--;
					if(required_brackets == 0)
						break;
				}
				break;
			}
		}
		
		eip++;
	}
}

int main(int argc,char *argv[]){
	
// DEBUG
	char s1[] = "a.txt";
	char s2[] = "100\n";
	argc = 3;
	argv[1] = s1;
	argv[2] = s2;
// DEBUG	
	
	if(argc == 1){  //no input file
		printf("invalid length of argument!\n");
		printf("Usage: ./brainfucker source_file [input_string]");
		return 0;
	}
	
	if(read_file(argv[1])){  //can't open file
		printf("invalid input file!");
		return 0;
	}
	
	int cnt = input_cnt(), len = strlen(argv[2]);
	if(argc == 3){  //deal with other problems such as the length of input
		input_s = argv[2];  //input_string
		/*if(cnt > len){
			printf("input string is too short!");
			return 0;
		}else if(cnt < len){
			printf("source code only needs %d input letter(s).\n", cnt);
			printf("and the other %d letter(s) will be ignored.\n\n", len - cnt);
		}*/
	}
	
	if(run() == 0)  // run the source code
		printf("brainfuck program has exited normally.");
	else
		printf("segment fault.");
	
	return 0;
}
