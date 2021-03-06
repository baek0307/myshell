/*********************************************************************
 * 학과 : 컴퓨터공학과 
 * 학번 : 20133228 
 * 이름 : 백장현
 * 
 * 프로젝트명 : myshell
 *  - exit 입력시 shell종료
 *  - a; b; c 와같이 입력시 a,b,c 세개의 프로그램 차례대로 실행
 *  - username@hostname$ 형식
 * 
 *  - 실행시 불편함없도록 친절하게 설명해놨음
 *  - 입력받은 프로그램의 개수와 순서를 보여준다.
 *  - 실행시 프로그램별 실행시간을 체크하고
 *    최종적으로 총 걸린시간을 출력해주는 함수구현 
 * 
 ********************************************************************/
 

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>
#include <time.h>


#define MAX_LEN_LINE    30
#define LEN_HOSTNAME	30

void waste_time(int count, char s_arr[][30], float t_arr[]);

int main(void)
{
    char command[MAX_LEN_LINE];
    char *args[] = {command, NULL};
    int ret, status;
    clock_t start,end;  //각 프로그램별 실행시간 측정을 위해 사용될 변수
    float w_time;
    pid_t pid, cpid;
    //
    //const char terminate = "exit";
    
    while (true) {
        char *s;
        char ele_s[30];
        char ss[10][30];
        int len;
        float store_time[10];
        int count=0; //a;b;c와같이 입력받을경우 입력받은 프로그램의 수를 나타내기 위함;
        int c_count =0;

    ///////////////////////////////////////////////////////////////////////////////
    //호스트네임,유저네임 받아서 표시하는 부분
        char hostname[LEN_HOSTNAME + 1];
        memset(hostname, 0x00, sizeof(hostname));
        gethostname(hostname, LEN_HOSTNAME);
            
    //username@hostname $ 형식으로 표현하기위함 


        printf("########################################################################################\n");
        printf("@@여러개의 프로그램을 입력하는경우 a; b; c와 같은 형식으로 입력해주세요 \n@@종료를 원하시면 exit 를 입력하세요\n\n");
        //printf("########################################################################################\n");
        printf("%s@%s $ ",getpwuid(getuid())->pw_name,hostname);
        s = fgets(command, MAX_LEN_LINE, stdin);
        len = strlen(command);



          ///////////////////////////////////////////////////////////////////////////////
        //exit 입력시 프로그램종료
        if(strcmp(s,"exit\n") == 0)
        {
            printf("Shell을 종료합니다. \n");
            break;
        }
         ///////////////////////////////////////////////////////////////////////////////


    ////////////////////////////////////////////////////////////////////////////////////////////////
       //a;b;c 와같은 형식으로 입력받을 경우 a,b,c 세개의 프로그램 차례대로 실행시키는 코드
       //입력받은 ;개수+1 만큼 프로그램 돌려야 하므로
       //몇개의 프로그램을 입력했는지 출력해준다.
       for(int i =0;i<len;i++)
       {
          if( s[i] == ';')
           count++;
       }

        printf("\n########################################################################################\n");
        printf("%d 개의 프로그램을 입력하셨습니다.\n 다음과 같은 순서대로 실행됩니다. \n",count+1);

        count =0; //count 초기화

        strncpy(ele_s,s,len-1);
      
        char *ptr = strtok(ele_s,"; "); //a; b; c 형식으로 입력받으므로 "; "기준으로 문자열을 자름, 포인터 반환
        while(ptr != NULL)
        {
            printf("  %s \n" ,ptr);
            strcpy(ss[count],ptr);
            count++;
            ptr = strtok(NULL,"; ");
        }

       
    //////////////////////////////////////////////////////////////////////////////////////////////////////

   
for(int i=0;i<count;i++)
{
    printf("########################################################################################\n");
    start = clock();
   // printf("시작시간은 %f \n",(float)start);
    s = ss[i];
    strcpy(command,s);
    len =strlen(command);

    printf("%d번째 프로그램 %s을(를) 실행합니다.\n\n",i+1,s);
    
        if (s == NULL) {
            fprintf(stderr, "fgets failed\n");
            exit(1);
        }
         
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

    end = clock();
    //printf("종료시간은 %f \n",(float)end);
    w_time = (float)(end - start)/1000 ;
    store_time[i] = w_time;
    printf("프로그램 %s 의 실행시간은 %.4f초 입니다. \n",s ,w_time);
   
}


///총 걸린시간 출력해주는거 해야지

    waste_time(count,ss,store_time);
    
    }
    return 0;
}


void waste_time(int count, char s_arr[][30], float t_arr[])
{
    float add_time =0;
    printf("########################################################################################\n");
    printf("모든 프로그램이 종료되었습니다. 프로그램별 실행시간을 출력합니다.\n");

    printf("++++++++++++++++++++++++++++++++++++++++++\n");
    printf("+  실행순서  +  프로그램명  +  걸린시간  +\n");
    for(int i =0;i<count+1;i++)
    {
        if(i != count)
        {
        //printf("%d 번째 프로그램 %s 를(을) 실행하는데 걸린시간은 %.4f초 입니다.\n",i+1,s_arr[i],t_arr[i]);
            printf("+   %2d번째   +  %9s   +  %.4f초  +\n",i+1,s_arr[i],t_arr[i]);
            printf("++++++++++++++++++++++++++++++++++++++++++\n");
            add_time += t_arr[i];
        }
		else
            printf("\n모든 프로그램을 실행하는데 걸린시간은 %.4f초 입니다.\n\n\n",add_time);
        
        
    }

    return ;
}





