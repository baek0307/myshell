#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>


#define MAX_LEN_LINE    10
#define LEN_HOSTNAME	30



int main(void)
{
    char command[MAX_LEN_LINE];
    char *args[] = {command, NULL};
    int ret, status;
    pid_t pid, cpid;
    //
    //const char terminate = "exit";
    
    while (true) {
        char *s;
        char *copy_s;
        
        int len;
        int count =1; //a;b;c와같이 입력받을경우 
        int c_count =0;

    ///////////////////////////////////////////////////////////////////////////////
    //호스트네임,유저네임 받아서 표시하는 부분
        char hostname[LEN_HOSTNAME + 1];
        memset(hostname, 0x00, sizeof(hostname));
        gethostname(hostname, LEN_HOSTNAME);
            
    //username@hostname $ 형식으로 표현하기위함 

        //printf("MyShell $ ");
        printf("%s@%s $ ",getpwuid(getuid())->pw_name,hostname);
        s = fgets(command, MAX_LEN_LINE, stdin);
        //copy_s 에 s 의 정보를 복사해준다
        strcpy(copy_s,s);
        len = strlen(command);

    ////////////////////////////////////////////////
       //a;b;c 와같은 형식으로 입력받을 경우 a,b,c 세개의 프로그램 차례대로 실행시키는 코드
       //입력받은 ;개수+1 만큼 프로그램 돌려야 하므로
       for(int i =0;i<len;i++)
       {
           s[i] =";";
           count++;
           printf("%d 개의 프로그램을 입력하셨습니다. \n",count);
       }

        char *ptr = strtok(s,"; "); //a; b; c 형식으로 입력받으므로 "; "기준으로 문자열을 자름, 포인터 반환
        
        //while(ptr != NULL)

        if (s == NULL) {
            fprintf(stderr, "fgets failed\n");
            exit(1);
        }

        ///////////////////////////////////////////////////////////////////////////////
        //exit 입력시 프로그램종료
        if(strcmp(s,"exit\n") == 0)
        {
            printf("Shell을 종료합니다. \n");
            break;
        }
         ///////////////////////////////////////////////////////////////////////////////
       

        
        printf("%d\n", len);
        if (command[len - 1] == '\n') {
            command[len - 1] = '\0'; 
        }
        
        printf("[%s]\n", command);
      
      // pid = fork();
      // 성공시 부모프로세스에서는 자식 프로세스의 PID 값을 반환받음
      //        자식프로세스에서는 0 값을 반환받음
      // 실패시 음수값(-1) 반환

        pid = fork();
        if (pid < 0) {
            fprintf(stderr, "fork failed\n");
            exit(1);
        } 
        if (pid != 0) {  /* parent */
            cpid = waitpid(pid, &status, 0);
            if (cpid != pid) {
                fprintf(stderr, "waitpid failed\n");        
            }
            printf("Child process terminated\n");
            if (WIFEXITED(status)) {
                printf("Exit status is %d\n", WEXITSTATUS(status)); 
            }
        }
        else {  /* child */
            ret = execve(args[0], args, NULL);
            if (ret < 0) {
                fprintf(stderr, "execve failed\n");   
                return 1;
            }
        } 
    }
    return 0;
}