#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

#define QUESTION "Do you want another transaction"

int get_requestion(char *);
void set_cr_noecho_mode();
void tty_mode(int);

int main()
{
	int response;
	tty_mode(0);
	set_cr_noecho_mode();
	response=get_requestion(QUESTION);
	tty_mode(1);
	return response;
}

/*
 *purpose:询问并等待回答y/n
 *method:使用getchar
 *returns:0=>yes,1=>no
 */
 
int get_requestion(char *question )
{
	int input;
	printf("%s(y/n)?\n",question);
	while(1){
		switch(input=getchar())
		{
			case 'Y':
				return 0;
			case 'y':
				return 0;
			case 'N':
				return 1;
			case 'n':
				return 1;
			case EOF:
				return 1;
			default:
				printf("\ncan't understand %c, ",input);
				printf("please type y or n.\n");
		}
	}
}


void set_cr_noecho_mode()
{
	struct termios ttystate;
	tcgetattr(0,&ttystate);
	ttystate.c_lflag &= ~ICANON;	//no buffering
	ttystate.c_lflag &= ~ECHO;	//no echo either
	ttystate.c_cc[VMIN] = 1;
	tcsetattr(0,TCSANOW,&ttystate);
}

void tty_mode(int how)
{
	static struct termios original_mode;
	if(how == 0){
		tcgetattr(0,&original_mode);
	}else{
		tcsetattr(0,TCSANOW,&original_mode);
	}
}
