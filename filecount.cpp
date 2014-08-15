#include <stdio.h>
#include <map>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <iostream>//cout<<endl;

using namespace std;
map<string,int> fileExtList;
map<string,int> fileExtList_size;

void Printdir(char* path){
    struct stat statbuf;
    DIR *dir;
    struct dirent *sdir;
    dir = opendir(path);
    if( NULL == dir){
        perror("opendir");
    }
    chdir(path);
    while ( (sdir = readdir(dir)) != NULL ){
        lstat(sdir->d_name,&statbuf);
        if( S_ISDIR(statbuf.st_mode)){
            if( (strcmp(sdir->d_name,".") == 0) || (strcmp(sdir->d_name,"..")== 0))continue;
            Printdir(sdir->d_name);
        }else{
            if(0)printf("%s %5d\n",sdir->d_name,statbuf.st_size);
            string fileExt=(string)sdir->d_name;
            int i=fileExt.rfind(".");
            if(i==fileExt.npos){
                fileExt='.';
            }else{
                fileExt=fileExt.substr(fileExt.rfind("."));
            }
            //cout<<fileExt<<endl;
            map<string,int>::iterator l_it;
            l_it=fileExtList.find(fileExt);
            if(l_it==fileExtList.end()){
                fileExtList[fileExt]=0;
                fileExtList_size[fileExt]=0;
            }
            fileExtList[fileExt]++;
            fileExtList_size[fileExt]+=statbuf.st_size;
        }
    }
    chdir("..");
    closedir(dir);
}
int main(int argc, char ** argv){
    char* path = (char*)".";
    if( argc >= 2 ){
        path = argv[1];
    }
    Printdir(path);
    map<string,int>::iterator iter;
    for(iter=fileExtList.begin();iter!=fileExtList.end();iter++){
        cout<<iter->first<<"\t"<<iter->second<<"\t"<<fileExtList_size[iter->first]<<"\n";
    }
}

