#include <cstdio>
#include <cstdlib>
#include "pruned_highway_labeling.h"
#include <sys/types.h>
#include <unistd.h>

using namespace std;

void get_proc_mem(unsigned int pid){

        char file_name[64]={0};
        FILE *fd;
        char line_buff[512]={0};
        sprintf(file_name,"/proc/%d/status",pid);

        fd =fopen(file_name,"r");
        if(nullptr == fd){
                return ;
        }

        char name[64];
        int vmsize;
        for (int i=0; i<=10;i++){

                fgets(line_buff,sizeof(line_buff),fd);
                sscanf(line_buff,"%s %u",name,&vmsize);
        }

        fgets(line_buff,sizeof(line_buff),fd);
        sscanf(line_buff,"%s %u",name,&vmsize);
        fprintf(stderr,"%s=%u\n",name,vmsize);
        fclose(fd);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: construct GRAPH LABEL\n");
        return 0;
    }
    
    PrunedHighwayLabeling phl;
    phl.ConstructLabel(argv[1]);
    phl.Statistics();
    phl.StoreLabel(argv[2]);
    int pid=getpid();
    get_proc_mem(pid);
    return 0;
}
