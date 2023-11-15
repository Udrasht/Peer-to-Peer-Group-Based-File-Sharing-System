#include <sys/socket.h>
#include<bits/stdc++.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <iostream>
#include <pthread.h>
using namespace std;
#define Chunk_size 524288
struct file_info{

string path;
string username;
long long int size;
int countofchunck;


};
struct download_info{
string gpid;
string fname;



};
struct user_info{
  string client_port;
  string client_addr;
set<string> files;
string username;
string password;
int loginstatus=0;
};
struct group_info{
  set<string> no_request;
  int member_count=0;
  set<string> fileinfo;
  string admin;
  set<string> users;
  string name;



};

map<string,vector<pair<string,string>>> allowner;
map<string,group_info> group_det;
map<string,user_info> client_det;
map<string,file_info> file_det;
vector<download_info> down;
vector<pair<string,string>> compl1;


void *implimentation(void *p_client_socket){
  string str;
  cin>>str;
  if(str=="quit"){
    exit(-1);
  }
  return NULL;
}
string command_process(string clientquerystring,string &currentuser){
 vector<string> clientquerystring_array;
 int n=clientquerystring.size();
 string demo="";
 for(int i=0;i<n;i++){
  if(clientquerystring[i]==' '){
    clientquerystring_array.push_back(demo);
    demo="";
  }
  else{
    demo=demo+clientquerystring[i];
  }
  
 }
 clientquerystring_array.push_back(demo);

 int n1=clientquerystring_array.size();
 
if(clientquerystring_array[0]=="create_user"){
  if(currentuser!="null"){
    string result="you can't create account first logout current acount then login";
    return result;
  }
  if(client_det.find(clientquerystring_array[1])!=client_det.end()){
    string result="username exist, try with diffrent username";
    return result;
  }
  else{
  client_det[clientquerystring_array[1]].username=clientquerystring_array[1];
  client_det[clientquerystring_array[1]].password=clientquerystring_array[2];
  client_det[clientquerystring_array[1]].loginstatus=0;

string result="Account created successfully please login to connect with server";
return result;}
}
else if(clientquerystring_array[0]=="login"){
  if(n1!=5){
    string result="Please enter username and password in correct formet";
    return result;
  }
  else{
    if(client_det.find(clientquerystring_array[1])!=client_det.end()){
      if(client_det[clientquerystring_array[1]].password==clientquerystring_array[2]){
        if(client_det[clientquerystring_array[1]].loginstatus==0){

          currentuser=clientquerystring_array[1];
           client_det[clientquerystring_array[1]].loginstatus=1;
           client_det[clientquerystring_array[1]].client_port=clientquerystring_array[4].c_str();
           client_det[clientquerystring_array[1]].client_addr=clientquerystring_array[3].c_str();
          
            string result="login successfully";
            return result;


         }
           else{
             string result="already login";
                return result;
           }
      }
      else{
         string result="wrong password";
    return result;
      }

    }
    else{
      string result="You are not the valid user please first create account and then try to login";
    return result;
    }

  }

}

else if(clientquerystring_array[0]=="create_group"){
  if(currentuser!="null"){
    if(group_det.find(clientquerystring_array[1])!=group_det.end()){
      string result="group already exist";
    return result;
    }
    else{
      group_det[clientquerystring_array[1]].member_count++;
      group_det[clientquerystring_array[1]].admin=client_det[currentuser].username;
      group_det[clientquerystring_array[1]].users.insert(client_det[currentuser].username);
      group_det[clientquerystring_array[1]].name=clientquerystring_array[1];
      string result="group created successfully";
      return result;

    }


    
  }
  else{
    string result="first login please to create group";
    return result;
  }

}

else if(clientquerystring_array[0]=="leave_group"){
  if(currentuser!="null"){
    if(group_det.find(clientquerystring_array[1])==group_det.end()){
      string result="group does't exist";
    return result;
    }
    if(group_det[clientquerystring_array[1]].users.find(currentuser)==group_det[clientquerystring_array[1]].users.end()){
      string result="you are not the member of group:"+group_det[clientquerystring_array[1]].name;
    return result;
    }
    if(group_det[clientquerystring_array[1]].admin==currentuser){
      string result="admin cant leav the group";
    return result;
    }
    group_det[clientquerystring_array[1]].member_count--;
    group_det[clientquerystring_array[1]].users.erase(currentuser);
    string result="leave group successfully";
    return result;

  }
  else{
    string result="first login please to leave group";
    return result;
  }

  
}
else if(clientquerystring_array[0]=="join_group"){
  if(group_det.find(clientquerystring_array[1])!=group_det.end()){
    if(currentuser!="null"){
    if(group_det[clientquerystring_array[1]].users.find(currentuser)!=group_det[clientquerystring_array[1]].users.end()){
      string result="you are already member of group";
    return result;
    }
    else{
      group_det[clientquerystring_array[1]].no_request.insert(currentuser);
      string result="join group request send";
      return result;

    }
  }
  else{
    string result="please login to join group";
    return result;
  }

  }
  else{
    string result="group does't exist";
    return result;
  }

}
else if(clientquerystring_array[0]=="list_group"){
  
  if(group_det.size()==0){
    string result="no group exixt";
    return result;

  }
  else{

    map<string,group_info>:: iterator it;
    string result="";
   
    for(it=group_det.begin();it!=group_det.end();++it){
             result=result+(*it).first+"  ";
    }
    return result;
  }
}
else if(clientquerystring_array[0]=="list_files"){
  if(group_det.find(clientquerystring_array[1])!=group_det.end()){
    if(group_det[clientquerystring_array[1]].fileinfo.size()==0){
      string result="No file exist in group"+clientquerystring_array[1];
    return result;
    }
    else{
      set<string>:: iterator it;
    string result="";
    for(it=group_det[clientquerystring_array[1]].fileinfo.begin();it!=group_det[clientquerystring_array[1]].fileinfo.end();it++){
             result=result+(*it)+"  ";
    }
    return result;

    }

  }
  else{
    string result="group does't exist";
    return result;

  }
}
else if(clientquerystring_array[0]=="list_requests"){
  if(currentuser!="null"){
    if(group_det.find(clientquerystring_array[1])==group_det.end()){
      string result="no such group exist";
    return result;
    }
    else{
      if(group_det[clientquerystring_array[1]].admin!=currentuser){
        string result="you are not admin of this group so you can't see request list";
    return result;
      }
      else{
            if(group_det[clientquerystring_array[1]].no_request.size()==0){
                  string result="no request found in group: "+group_det[clientquerystring_array[1]].name;
                  return result;
            }
            else{
                     set<string>:: iterator it;
                      string result="";
           for(it=group_det[clientquerystring_array[1]].no_request.begin();it!=group_det[clientquerystring_array[1]].no_request.end();it++){
             result=result+(*it)+"  ";
            }
          return result;
            }
      }
    }

  }
  else{
    string result="please login to see requests";
    return result;
  }

}

else if(clientquerystring_array[0]=="accept_request"){
  if(currentuser=="null"){
    string result="please login to see accept_requests";
    return result;
  }
  if(group_det.find(clientquerystring_array[1])==group_det.end()){
    string result="No such group exixt please enter correct group name";
    return result;
  }
  if(group_det[clientquerystring_array[1]].admin!=currentuser){
      string result="you are not admin of this group please login as admin";
    return result;
      
  }
  if(group_det[clientquerystring_array[1]].no_request.find(clientquerystring_array[2])==group_det[clientquerystring_array[1]].no_request.end()){
    string result="no request of this user to join this group";
    return result;
  }
  group_det[clientquerystring_array[1]].users.insert(clientquerystring_array[2]);
  group_det[clientquerystring_array[1]].member_count++;
  group_det[clientquerystring_array[1]].no_request.erase(clientquerystring_array[2]);
   return "accep request successfully";

  }
  else if(clientquerystring_array[0]=="logout"){
    client_det[currentuser].loginstatus=0;
    currentuser="null";
    string result="logout successfully";
    return result;
  }

  else if(clientquerystring_array[0]=="upload_file"){

    if(currentuser=="null"){
      string result="please login to upload_file";
    return result;

    }
    else{
    if(group_det.find(clientquerystring_array[2])==group_det.end()){
      string result="group does't exist";
    return result;
    }
    else{
      //some bug in the question
      if(group_det[clientquerystring_array[2]].fileinfo.find(clientquerystring_array[1])==group_det[clientquerystring_array[2]].fileinfo.end()){
                int size_inputcommandsplit=clientquerystring_array[1].size();
      int  last=0;
      for(int i=clientquerystring_array[1].size()-1;i>=0;i--){
        if(clientquerystring_array[1][i]=='/'){
          last=i;
          break;
        }
      }
      if(last!=clientquerystring_array[1].size()-1){
      clientquerystring_array[1]=clientquerystring_array[1].substr(last,clientquerystring_array[1].size());}
      if(clientquerystring_array[1][0]=='.'){
        clientquerystring_array[1]=clientquerystring_array[1].substr(1,clientquerystring_array[1].size());
      }
      file_det[clientquerystring_array[1]].countofchunck=stoi(clientquerystring_array[3]);
      file_det[clientquerystring_array[1]].size=stoi(clientquerystring_array[4]);
      group_det[clientquerystring_array[2]].fileinfo.insert(clientquerystring_array[1]);
      file_det[clientquerystring_array[1]].username=currentuser;
      file_det[clientquerystring_array[1]].path=clientquerystring_array[1];
      allowner[clientquerystring_array[1]].push_back({currentuser,clientquerystring_array[5]});


      string result="upload successfully";
      return result;

      
      }
      else{
        string result="file already exist in the group";
         int  last=0;
      for(int i=clientquerystring_array[1].size()-1;i>=0;i--){
        if(clientquerystring_array[1][i]=='/'){
          last=i;
          break;
        }
      }
      if(last!=clientquerystring_array[1].size()-1){
      clientquerystring_array[1]=clientquerystring_array[1].substr(last,clientquerystring_array[1].size());}
      if(clientquerystring_array[1][0]=='.'){
        clientquerystring_array[1]=clientquerystring_array[1].substr(1,clientquerystring_array[1].size());
      }
        allowner[clientquerystring_array[1]].push_back({currentuser,clientquerystring_array[5]});
        return result;

      }

    }
  }
}


else if(clientquerystring_array[0]=="download_file"){
  if(currentuser=="null"){
    string result="please login to download file";
    return result;
  }
  if(group_det.find(clientquerystring_array[1])==group_det.end()){
    string result="group not exist";
    return result;
  }
  if(group_det[clientquerystring_array[1]].users.find(currentuser)==group_det[clientquerystring_array[1]].users.end()){
    string result="you are not the member of this group"+clientquerystring_array[1];
    return result;
  }
  if(group_det[clientquerystring_array[1]].fileinfo.find(clientquerystring_array[2])==group_det[clientquerystring_array[1]].fileinfo.end()){
    string result="no such file exist";
    return result;
  }
  int  last=0;
      for(int i=clientquerystring_array[2].size()-1;i>=0;i--){
        if(clientquerystring_array[2][i]=='/'){
          last=i;
          break;
        }
      }
      if(last!=clientquerystring_array[2].size()-1){
      clientquerystring_array[2]=clientquerystring_array[2].substr(last,clientquerystring_array[2].size());}
      if(clientquerystring_array[2][0]=='.'){
        clientquerystring_array[2]=clientquerystring_array[2].substr(1,clientquerystring_array[2].size());
      }
      
      string result="done "+clientquerystring_array[4]+" "+clientquerystring_array[2];
      int pq=0;
      for(int i=0;i<allowner[clientquerystring_array[2]].size();i++){
        if(client_det[allowner[clientquerystring_array[2]][i].first].loginstatus==1&&group_det[clientquerystring_array[1]].users.find(allowner[clientquerystring_array[2]][i].first)!=group_det[clientquerystring_array[1]].users.end())
             { 
              pq++;
        result=result+" "+string(client_det[allowner[clientquerystring_array[2]][i].first].client_addr)+" "+string(client_det[allowner[clientquerystring_array[2]][i].first].client_port)+" "+allowner[clientquerystring_array[2]][i].second;
          } 
      }

      allowner[clientquerystring_array[2]].push_back({currentuser,clientquerystring_array[4]});
      pair<string,string> pr;
      pr.first=group_det[clientquerystring_array[1]].name;
      pr.second=clientquerystring_array[2];
      compl1.push_back(pr);

      
      result=result+" "+to_string(file_det[clientquerystring_array[2]].countofchunck)+" "+to_string(file_det[clientquerystring_array[2]].size);
      if(pq==0){
        result="file present in the group but all the server down which contain this file";
      }
      return result;


}
else if(clientquerystring_array[0]=="stop_share"){
  if(currentuser=="null"){
    string result="please login for stop share file";
    return result;
  }
  else if(group_det.find(clientquerystring_array[1])==group_det.end()){
    string result="group not exist";
    return result;
  }
  else if(group_det[clientquerystring_array[1]].admin!=currentuser){
    string result="you are not the admin of group only admin have this authority";
    return result;
  }
 else if(group_det[clientquerystring_array[1]].fileinfo.find(clientquerystring_array[2])==group_det[clientquerystring_array[1]].fileinfo.end()){
    string result=clientquerystring_array[2]+" file not exist in this group";
    return result;
  }
  else{
     group_det[clientquerystring_array[1]].fileinfo.erase(clientquerystring_array[2]);
     string result="operation success.... ";
    return result;
  }

}
else if(clientquerystring_array[0]=="show_downloads"){
  if(currentuser=="null"){
    string result="please login for stop share file";
    return result;
  }
  int compl1size=compl1.size();
  if(compl1size==0){
    string result="Empty";
    return result;
  }
  else{
     string result="compleately downloaded files are ->";
     for(int iy=0;iy<compl1size;iy++){
      result=result+"(groupname="+compl1[iy].first+" filename="+compl1[iy].second+"),";
     }
     return result;

  }

}
else{
  string result="not valid command";
  return result; 
}
return "fault....";

}
void *implimentation11(void *clintsocket){
  
  char clintquery[2000];
  string currentuser="null";

  do{
    bzero(clintquery,2000);
    read(int(*(int *)(clintsocket)),clintquery,sizeof(clintquery));
    
    string clientquerystring=string(clintquery),serverresponseto_client;
 
    cout<<"CLIENT QUERIES: "<<clientquerystring<<endl;
   serverresponseto_client=command_process(clientquerystring,currentuser);
    /*char hays[2000];
    strcpy(hays,clientquerystring.c_str());
    write(int(*(int *)(clintsocket)),hays,sizeof(hays));
*/
    //current work;
   char buff2[2000];
   
   strcpy(buff2,serverresponseto_client.c_str());
   write(int(*(int *)(clintsocket)),buff2,sizeof(buff2));
   bzero(clintquery,2000);

    

  }while(1);
  return NULL;
}

int main(int argc,char *argv[]){
	
 const char *ip;
  std::ifstream infile(argv[1]);
  string aip;
   string aport;
  getline(infile,aip);
  
  getline(infile,aport);
   const char *port;
  ip=aip.c_str();
  port=aport.c_str();

   
   int sockinfo = socket(AF_INET, SOCK_STREAM, 0);

  if (sockinfo != -1)
  {
    cout << "socket created in a server" << endl;
  }
  else
  {
    cout << "socket not created in a server" << endl;
    exit(1);
  }
  struct sockaddr_in addressOfServer, addressOfClient;
   bzero(&addressOfServer,sizeof(addressOfServer));

  addressOfServer.sin_family = AF_INET;

  addressOfServer.sin_addr.s_addr = inet_addr(ip);
  addressOfServer.sin_port = htons(atoi(port));
  if (bind(sockinfo, (struct sockaddr *)&addressOfServer, sizeof(addressOfServer)) != 0)
  {
    cout << "NOT BINDED" << endl;
  }
  else
  {
    cout << "BINDED" << endl;
  }

   int listn = listen(sockinfo, 10);
  if (listn == 0)
  {
    cout << "server listening........." << endl;
  }
  else
  {
    cout << "listening failed......" << endl;
    exit(0);
  }
  pthread_t th;
  pthread_create(&th,NULL,implimentation,NULL);
  while(1){
      socklen_t len=sizeof(addressOfClient);
      int *cchild=(int *)malloc(sizeof(int));
  	int finalaccept = accept(sockinfo, (sockaddr *)&addressOfClient, &len);
  if (finalaccept < 0)
  {
    cout << "server accept failed..." << endl;
    exit(0);
  }
  else
  {
    cout << "server accept request from ip: " <<inet_ntoa(addressOfClient.sin_addr)<<"and port.no: "<<ntohs(addressOfClient.sin_port)<< endl;
  }

    *cchild=finalaccept;
    pthread_t th;
    pthread_create(&th,NULL,implimentation11,cchild);

  
    }

    //break;
  close(sockinfo);
  }

