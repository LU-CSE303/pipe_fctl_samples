#include <stdio.h>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <sys/types.h> 
#include <unistd.h> 
#include <wait.h>
#include <vector>
#include <iostream>
#include <linux/seccomp.h>
#include <sys/prctl.h>

using namespace std;

int main(int argc, char *argv[]) {
   int    pipe_parent_to_child[2];
   int    pipe_child_to_parent[2];
   vector<char> data;
   pid_t  cpid;
   if (pipe(pipe_parent_to_child) == -1)  {
     cout << "pipe error" << endl;
     exit(0);
   }
   if(pipe(pipe_child_to_parent) == -1){
      close(pipe_parent_to_child[0]);
      close(pipe_parent_to_child[1]);
      cout << "pipe error" << endl;
      exit(0);
   }
   string message("Hello World!");
   cpid = fork();
   if (cpid == 0) {    /* Child reads from pipe_parent_to_child  and writes to pipe_child_to_parent */
    close(pipe_parent_to_child[1]); /* Close unused write end */
    close(pipe_child_to_parent[0]); /* Close unused read end */
    char buf[16];
    if (read(pipe_parent_to_child[0], &buf, sizeof(buf)) > 0){
       // do something with the data
       data.insert(data.begin(), buf, buf+sizeof(buf));
       cout << "Read by the child process from pipe 1: " << data.data() << endl;
       for(int i=0; i<data.size(); i++){
        data[i] = toupper(data[i]);
       }
       write(pipe_child_to_parent[1], data.data(), data.size());
       cout << "Written by the child process to pipe 2: " << data.data() << endl;
    }
    close(pipe_parent_to_child[0]);
    close(pipe_child_to_parent[1]);
    exit(0);
   } else {            /* Parent writes to pipe_parent_to_child  and reads from pipe_child_to_parent */
     close(pipe_parent_to_child[0]); /* Close unused read end */
     close(pipe_child_to_parent[1]); /* Close unused write end */
     write(pipe_parent_to_child[1], message.data(), message.size());
     cout << "Written by the parent process on pipe 1: " << message << endl;
     close(pipe_parent_to_child[1]); /* Reader will see EOF */
     wait(NULL);       /* reap the child */
     char buf[16];
     cout << "Read by the parent process from pipe 2: ";
     if(read(pipe_child_to_parent[0], &buf, sizeof(buf)) > 0){
      cout << buf << endl;
    }
    exit(0);
   }
}
