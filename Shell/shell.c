#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
// #include <sys/stat.h>
#include <fcntl.h>

void givepath(char *command,char new[]);
void give_substring(char *orignal, char *buffer, int start, int end);
void hell_in_a_shell(char *input);

int main(){

    while(1){
        char input[4000], modified_input[4000];
        printf("$");
        // scanf("%[^\n]%*c", input);
        gets(input);
        // fgets(input,4000,stdin);
        // for(int i=0; i<4000 && input[i]!='\n'; ++i){
        //     modified_input[i]=input[i];
        // }
        hell_in_a_shell(input);
    }
        
    return 0;
}


void hell_in_a_shell(char *input){

    // printf("hell in a shell called\n");
    if(strcmp(input,"")==0){
        return;
    }
    if(strcmp("exit",input)==0){
            // printf("Good bye!\n");
            exit(0);
        }
        // printf("Going to fork\n");
        pid_t PID = fork();
        int cont=1;
        int global_pipe=0;
        
        if(PID==0){
            int fd[2];
            pipe(fd);

            while(cont){
                // printf("looping\n");
                // printf("Got it: %s\n",input); 
                
                    // printf("in child\n");
                    int size=0;
                    char *args[50];
                    int found_arrow=0, found_double_arrow=0, found_reverse_arrow=0, found_1_to_file=0, found_2_to_file=0, found_2_to_1=0, found_pipe=0;
                    char filename1[100],filename2[100], piped[4000];
                    int pos=0;
                    char *word = strtok (input," ");
                    while (word != NULL)                // Converting into string into array of words.
                    {
                        args[size]=word;
                        if(word[0]=='1' && word[1]=='>'){
                            found_1_to_file=size;
                            strcpy(filename1,word);
                            if(!pos)
                                pos=size;
                        }
                        else if(word[0]=='2' && word[1]=='>'){
                            if(word[2]=='&'){
                                found_2_to_1=size;
                                if(!pos)
                                    pos=size;
                            }else{
                                found_2_to_file=size;
                                strcpy(filename2,word);
                                if(!pos)
                                    pos=size;
                            }
                        }
                        else if (strcmp(word,"|")==0){
                            found_pipe=size;
                            word = strtok(NULL,"");
                            if(!pos)
                                pos=size;
                            continue;
                        }
                        else if(strcmp(word,">")==0){
                            found_arrow=size;
                            if(!pos)
                                pos=size;
                        }
                        else if(strcmp(word,">>")==0){
                            found_double_arrow=size;
                            if(!pos)
                                pos=size;
                        }
                        else if(strcmp(word,"<")==0){
                            found_reverse_arrow=size;
                            if(!pos)
                                pos=size;
                        }
                        ++size;
                        word = strtok (NULL, " ");
                    }            
                    // printf("first half: %s, other half: %s\n", args[0], args[size-1]);
                    strcpy(piped,args[size-1]);
                    char *out=NULL;

                    if(found_pipe==0){
                    cont=0;
                    }else{
                        args[pos]=NULL;
                    }
                    if(pos && (!found_pipe && !global_pipe)){    //setting null at the position of specual characters
                        // printf("\n\nHAHA\n\n\n");
                        args[pos]=NULL;
                        out=args[pos+1];

                        if(found_1_to_file){
                            // printf("CASE 1\n");
                            close(1);
                            int fd;
                            char file[100];
                            give_substring(filename1,file,2,strlen(filename1));
                            // printf("file=%s\n",file);
                            fd = creat(file, 0666);
                            // printf("fd=%d",fd);

                        }
                        if(found_2_to_file){
                            // printf("CASE 2\n");
                            close(2);
                            int fd;
                            char file[100];
                            give_substring(filename2,file,2,strlen(filename2));
                            // printf("file=%s",file);
                            fd = creat(file, 0666);
                            // printf("fd=%d",fd);

                        }
                            if(found_2_to_1){
                            // printf("CASE 3\n");
                            close(2);
                            dup(1);
                        }
                            if(found_arrow){
                           
                                close(1);
                                int fd=creat(args[found_arrow+1], 0666);
                            
                        }
                            if(found_double_arrow){
                            // printf("CASE 5\n");
                            close(1);
                            int fd;
                            fd = open(args[found_double_arrow+1], O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
                        }
                            if(found_reverse_arrow){
                            // printf("CASE 6\n");
                            close(0);
                            int fd;
                            if((fd = open(args[found_reverse_arrow+1], O_RDONLY))==-1){
                                printf("Cannot open Input file! %s\n",args[found_reverse_arrow]);
                                exit(0);
                            }
                        }


                    }else{
                        args[size]=NULL;
                    }
                    // printf("global_pipe: %d\n",global_pipe);
                    if(found_pipe || global_pipe){
                        // printf("piped: %s\n", piped);
                        int fd2[2];
                        pipe(fd2);
                        pid_t pipe_child=fork();
                        if(pipe_child==0){
                            if(found_pipe!=0 && global_pipe==0){
                            
                                close(fd2[0]);
                                close(1);
                                dup(fd2[1]);
                                close(fd2[1]);

                               if(pos!=found_pipe)
                                    args[pos]=NULL;


                                if(found_reverse_arrow){
                            // printf("CASE 6\n");
                                    close(0);
                                    int fd;
                                    if((fd = open(args[found_reverse_arrow+1], O_RDONLY))==-1){
                                        printf("Cannot open Input file! %s\n",args[found_reverse_arrow]);
                                        exit(0);
                                    }
                                }
                                int i=execvp(args[0],args);
                                printf("Failed to execute the command \"%s\" . Error code:%d\n",args[0],i);
                                exit(0);

                            }
                            else if(found_pipe!=0 && global_pipe==1){
                                // printf("HIII\n");
                                //take input from fd
                                if(pos!=found_pipe)
                                    args[pos]=NULL;


                                close(fd[1]);
                                close(0);
                                dup(fd[0]);
                                close(fd[0]);
                                //give output to fd2
                                 close(fd2[0]);
                                close(1);
                                dup(fd2[1]);
                                close(fd2[1]);
                                int i=execvp(args[0],args);
                                printf("Failed to execute the command \"%s\" . Error code:%d\n",args[0],i);
                                exit(0);

                            }
                            else{
                                // printf("cpmmad: %s\n",args[0]);
                                close(fd[1]);
                                close(0);
                                dup(fd[0]);
                                close(fd[0]);

                                // close(1);
                                // int fd=creat("hello", 0666);
                                if(pos!=found_pipe)
                                    args[pos]=NULL;

                                 if(found_1_to_file){
                                    // printf("CASE 1\n");
                                    close(1);
                                    int fd;
                                    char file[100];
                                    give_substring(filename1,file,2,strlen(filename1));
                                    // printf("file=%s\n",file);
                                    fd = creat(file, 0666);
                                    // printf("fd=%d",fd);

                                }
                                if(found_2_to_file){
                                    // printf("CASE 2\n");
                                    close(2);
                                    int fd;
                                    char file[100];
                                    give_substring(filename2,file,2,strlen(filename2));
                                    // printf("file=%s",file);
                                    fd = creat(file, 0666);
                                    // printf("fd=%d",fd);

                                }
                                    if(found_2_to_1){
                                    // printf("CASE 3\n");
                                    close(2);
                                    dup(1);
                                }
                                    if(found_arrow){
                                
                                        close(1);
                                        int fd=creat(args[found_arrow+1], 0666);
                                    
                                }
                                    if(found_double_arrow){
                                    // printf("CASE 5\n");
                                    close(1);
                                    int fd;
                                    fd = open(args[found_double_arrow+1], O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
                                }
                                    
                            
                                

                                int i=execvp(args[0],args);
                                printf("Failed to execute the command \"%s\" . Error code:%d\n",args[0],i);
                                exit(0);
                            }
                        }else{
                            // printf("waiting...\n");
                            // close(fd[0]);
                            // close(fd2[0]);
                            // if(found_pipe!=0){
                            //     close(fd[1]);

                            // }else{
                            //     close(fd[0]);
                            // }
                            // close(fd2[1]);
                            close(fd[1]);
                            wait(&pipe_child);
                            fd[0]=fd2[0];
                            fd[1]=fd2[1];
                            global_pipe=1;
                            // printf("in parent\n");
                            strcpy(input,piped);
                            // exit(0);
                            // continue;
                        }
                    
                    }else{

                        // printf("piping not found\n");
                        int i=execvp(args[0],args);
                        printf("Failed to execute the command \"%s\" . Error code:%d\n",args[0],i);
                        exit(0);
                    }
                    
                
            }
        }
        else{
            PID=wait(NULL);
            // printf("in parent\n");

            return;
        }
}

void givepath(char *command, char toRet[]){
    

    if(command[0]=='/'||command[0]=='.'){
        strcat(toRet,command);  
    }
    else{
        strcat(toRet,"/usr/bin/");
        strcat(toRet,command);  
    }


}
 //hint from https://stackoverflow.com/questions/26620388/c-substrings-c-string-slicing
void give_substring(char *orignal, char *buffer, int start, int end){  
    int base=0;
    for(int i=start;i<end && i<strlen(orignal); ++i){
        buffer[base++]=orignal[i];
    }
    buffer[base]='\0';
}

// void extract_command(char *whole, start, end, char *buffer[]){}



//    printf("piping found\n");
//                 pid_t piping = fork();
//                 if(piping==0){
//                     // printf("piping child\n");
                    
//                     close(fd[0]);
//                     close(1);
//                     dup(fd[1]);
//                     close(fd[1]);
//                     int i=execvp(args[0],args);
//                     printf("Failed to execute the command \"%s\" . Error code:%d\n",args[0],i);
//                     exit(0);

//                 }else{
//                     piping=wait(NULL);
//                     pid_t parent_pip = fork();
//                     if(parent_pip==0){
//                         close(fd[1]);
//                         close(0);
//                         dup(fd[0]);
//                         close(fd[0]);
//                         hell_in_a_shell(piped);

//                     }
//                     else{
//                         parent_pip=wait(NULL);
//                     }
//                     printf("piping parent\n");
//                 }

// /bin/ls | /bin/sort | /bin/uniq | /usr/bin/wc -l 2>&1 1>output.txt