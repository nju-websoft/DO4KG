#include <iostream>
#include "highway_cover_labelling.h"
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


int main(int argc, char **argv) {
  int k = atoi(argv[2]);
  HighwayLabelling *hl = new HighwayLabelling(argv[1], k);

  int topk[k];
  hl->SelectLandmarks_HD(topk);
  sort(topk, topk + k);

  // construct labelling
  hl->ConstructHighwayLabelling(topk);
  hl->StoreIndex(argv[3]);
  /*freopen("landmarks","w",stdout);
  printf("%d\n",k);
  for (int i=0;i<k;i++) printf("%d ",topk[i]);
  hl->printgraph(topk);*/
  int pid=getpid();
  get_proc_mem(pid);
  delete hl;
  get_proc_mem(pid);
  fprintf(stderr,"%lf\n",1.*clock()/CLOCKS_PER_SEC);
  exit(EXIT_FAILURE);
}
