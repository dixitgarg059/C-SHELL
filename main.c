#include "global.h"
void sigintHandler (int sig_num)
{


   if(previousid==-1)                   // previous id stores the pid of most recent process
   {
       return ;
   }
    kill(previousid,9);                // sending signal 9 to kill the process
   
}
void sigtstpHandler(int sig_num) 
{ 
    
    if(previousid==-1)
        return;
   
    for(int i=0;i<jobslen;i++)         // job whose pid is previous id now is pushed in background
    {                                  // so its status must be 0
        if(jobs[i].id==previousid)
        {
            jobs[i].status=1;
            break;
        }
    }
    kill(previousid,19);              // sending signal 19 to stop the process
}

int main()
{
    

    getcwd(tilda,100);
    strcpy(previous,"INVALID");
    signal(SIGINT, sigintHandler);   // signals
    signal(SIGTSTP, sigtstpHandler);
   
    cron1=0;
    while(1)
    {
        previousid=-1;


        fflush(stderr);
        int pid,pid_status;
        while((pid = waitpid(-1, &pid_status, WNOHANG | WUNTRACED)) >0){

            if(pid==cronjobpid)             
                break;
            if(WIFEXITED(pid_status))       // check whether process exited or not
            {

                int j; 
                for(int i=0;i<jobslen;i++)
                {
                    if(jobs[i].id==pid)
                    {
                        j=i;
                        break;
                    }

                }
                jobs[j].status=0;
                fprintf(stderr, "pid = %d  and name = %s exited normally.\n",pid,jobs[j].name);

            }
            else if(WIFSIGNALED(pid_status))
            {

                int j;                        // check whether process is signaled or not
                for(int i=0;i<jobslen;i++)
                {
                    if(jobs[i].id==pid)
                    {
                        j=i;
                        jobs[i].status=0;
                        break;

                    }

                }
                fprintf(stderr, "pid = %d  and name = %s terminated by signal\n",pid,jobs[j].name);  

            }        
        }

        begin();
        char argu[2000];
        if(takeInput(argu)==0)
            continue;

        int len=strlen(argu);            // keys is storing ascii values of key pressed
        int keys[1000];                  // (for implimenting history)
        for(int i=0;i<len;i++)
        {
            keys[i]=(int)argu[i];
        }

        int l=0,flag=1,up=0;
        while(l<len)
        {
            if((l+2)<len)
            {
                if(keys[l]==27 && keys[l+1]==91)
                {
                    if(keys[l+2]==65)
                        up++;
                    else if(keys[l+2]==66)
                        up--;
                    else
                    {
                        flag=0;
                        break;
                    }
                    l+=3;
                }
                else
                {
                    flag=0;
                    break;
                }

            }
            else{
                flag=0;
                break;
            }
        }
        if(flag==1)                        // add history and seperate command and execute
        {char arg1[2000];
            giverecenthistory(up,arg1);
            addhistory(arg1);
            printf("%s\n",arg1);
            commandsep(arg1);
        }
        else{
            addhistory(argu);
            commandsep(argu);   
        }
    }
    return 0;
}
