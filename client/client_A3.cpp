#include <sys/socket.h>
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
#include<bits/stdc++.h>
#define Chunk_size 524288

using namespace std;
#define C_size 51200
char *client_port;
char *client_addr;
struct sockaddr;
struct client_info{
	int sockfd;
	vector<int> arrayof_bit;
	int no_ofchunck;
	string path;
	string name;
	long long int sizeoffile;
	int num;
};
struct file_info{
	string pth;
	int size;
	vector<int> chuncks;
	int nofchunck;

};
int file_size(string f_name){
	ifstream in_file(f_name,ios::binary);
	in_file.seekg(0,ios::end);
	return int(in_file.tellg());
}

string file_path(string str){
	string t;
	char buff[256];
	string parent=getcwd(buff,256);
if(str[0]=='~'||str[0]=='/'||str[0]=='.'){
	t=string(parent)+"/";
}
else{
	t=string(parent)+"/";
}
return t;
}



void *pearconnection(void *ptopconnect){





    struct client_info *cli = (struct client_info *)ptopconnect;
   string name = cli->name;
    int chunks = cli->no_ofchunck;
    int sockfd = cli->sockfd;
    string path = cli->path;
    long long int fsize67=cli->sizeoffile;
    int clinum=cli->num;
    
    int currentseek=0;
   string store=name.c_str();
    if (FILE *fi=fopen(path.c_str(),"rb"))
    {
        fclose(fi);
    }
    else
    {
        FILE *fpointer = fopen(path.c_str(),"wb");
        // if (fallocate(fileno(fpointer), 0, 0, fsize67) != 0)
        // {
        //     cout <<"file allocate"<<endl;
        // }
        fclose(fpointer);
    }
    FILE *fin=fopen(path.c_str(),"w+");
    
    for (int i = 0; i <chunks; i++)
    {
        char buff[C_size];
        bzero(buff, sizeof(buff));
        memset(&buff, 0, sizeof(buff));
        string s;
        s="senddata";
        s=s+" "+to_string(i);
        s=s+" "+name+" "+to_string(clinum);
        strcpy(buff, s.c_str());
        write(sockfd, buff, sizeof(buff));
        bzero(buff, sizeof(buff));
        memset(&buff, 0, sizeof(buff));
        cout<<"read"<<endl;
        read(sockfd, buff, sizeof(buff));        
        cout<<"write"<<endl;
     
        
         int ud=C_size;
         if(i==chunks-1){
         	ud=fsize67%C_size;
         }

      // cout<<caughtdata<<endl;
       //cout<<ud<<endl;
        int t=i * currentseek;
        fseek(fin,t,SEEK_SET);
        currentseek = ud;
        fwrite(buff, sizeof(char),ud, fin);
    }
    fclose(fin);
    return NULL;


}













string executionofcommand(string inputstring,int n,vector<string> &inputcommandsplit){
  

string dstr="";
for(int i=0;i<n;i++){
	if(inputstring[i]==' '){
		inputcommandsplit.push_back(dstr);
		dstr="";
	}
	else{
		dstr=dstr+inputstring[i];
	}
}
inputcommandsplit.push_back(dstr);
int n1=inputcommandsplit.size();
// cout<<n1<<endl;
if(inputcommandsplit[0]=="create_user"||inputcommandsplit[0]=="upload_files"||inputcommandsplit[0]=="login"||inputcommandsplit[0]=="accept_request"||inputcommandsplit[0]=="stop_share"){

     if(n1!=3){
     	cout<<"Enter command in correct format(eg: create_user/login udrasht abc@123)"<<endl;
     	return "null";
     }
     if(inputcommandsplit[0]=="login"){
     	return "login";
     }
     return "correct";

}
else if(inputcommandsplit[0]=="create_group"||inputcommandsplit[0]=="list_requests"||inputcommandsplit[0]=="join_group"||inputcommandsplit[0]=="leave_group"||inputcommandsplit[0]=="list_files"){
         if(n1!=2){
     	cout<<"Enter command in correct format(eg: create_group/group_join/leave_group/list_files <group_id>)"<<endl;
     	return "null";
     }
     return "correct";
}

else if(inputcommandsplit[0]=="list_group"||inputcommandsplit[0]=="logout"||inputcommandsplit[0]=="show_downloads"){

return "correct";
}

else if(inputcommandsplit[0]=="upload_file"){
	if(n1!=3){
     	cout<<"Enter command in correct format(eg: upload_file <file_path> <group_id>)"<<endl;
     	return "null";
     }
     return "upload_file";

}
else if(inputcommandsplit[0]=="download_file"){
	if(n1!=4){
	cout<<"Enter command in correct format(eg: download_file <group_id> <file_name> <destination_path>)"<<endl;
     return "null";
 }
     return "download_file";

}
else{
	cout<<"Enter valid command"<<endl;
	return "null";
}
}
void download_perform(vector<string> arr,map<string,file_info> file){
int numberofclient=0;
string fname = arr[2];
vector<const char *> ip;
vector<const char *> port;
vector<string> pathfile;
vector<int> listofsockfd;
int i=3;
while(i<arr.size()-2){
   ip.push_back(arr[i].c_str());
   
   i++;
   port.push_back(arr[i].c_str());
   i++;
   pathfile.push_back(arr[i].c_str());
  
   i++;

   numberofclient++;

}
vector<int> listoffd;
int chunck=stoi(arr[i]);
vector<vector<int>> vector_of_bit(numberofclient,vector<int>(chunck));
int j=0;
  
for(int i=0;i<numberofclient;i++){
	char buff[C_size];
	struct sockaddr_in addofserver;
	int sockfd;
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	bzero(&addofserver,sizeof(addofserver));
	if(sockfd==-1){
		cout<<"Socket Creation failed in download_perform"<<endl;
		exit(0);
	}
	addofserver.sin_family=AF_INET;
	addofserver.sin_port=htons(atoi(port[i]));
	int d1=inet_pton(AF_INET,ip[i], &addofserver.sin_addr);
	if(d1<=0){
		cout << "Error in inet_pton for " <<ip[i]<<endl;
	     exit(0);
	}
	int c1=connect(sockfd, (sockaddr *)&addofserver, sizeof(addofserver));
   if(c1!=0){
   	  cout<<"connection with the server failed..."<<endl;
            exit(0);
   }
   
  string sendstring="bit_vector";
   sendstring=sendstring+" "+pathfile[i]+"/"+arr[2];
   sendstring=sendstring+" "+to_string(chunck)+" "+to_string(numberofclient);
   strcpy(buff,sendstring.c_str());
   write(sockfd,buff,sizeof(buff));
   bzero(buff,sizeof(buff));
   read(sockfd,buff,sizeof(buff));
   string trackerresponse1;
    	trackerresponse1=string(buff);
    	vector<string> trackerresponsesplit1;
    	string dstr="";
    	int n2=trackerresponse1.size();
    	
for(int to=0;to<n2;to++){
	if(trackerresponse1[to]==' '){
		trackerresponsesplit1.push_back(dstr);
		//cout<<dstr<<endl;
		dstr="";
	}
	else{
		dstr=dstr+trackerresponse1[to];
	}
}
trackerresponsesplit1.push_back(dstr);
if(trackerresponsesplit1[0]=="fail"||trackerresponsesplit1[0]=="failed"||trackerresponsesplit1[0]=="file_not_exist"){
	continue;
}

listoffd.push_back(sockfd);
for(int k=0;k<trackerresponsesplit1.size();k++){
	int t3=stoi(trackerresponsesplit1[k]);
	vector_of_bit[i].push_back(t3);
}


}




pthread_t thread[numberofclient];
int l=0;
while(l<numberofclient){
	struct client_info cli;
	cli.path=arr[1]+"/"+arr[2];
	cli.name=pathfile[l]+"/"+arr[2];
	cli.no_ofchunck=chunck;
	cli.arrayof_bit=vector_of_bit[l];
	cli.sockfd=listoffd[l];
	cli.sizeoffile=stoi(arr[arr.size()-1]);
	cli.num=l+1;
	
	pthread_create(&thread[l],NULL,pearconnection,(void *)&cli);
	pthread_join(thread[l],NULL);
	l++;

}





}

string server_reaction(vector<string> trackerresponsesplit2){

if(trackerresponsesplit2[0]=="bit_vector"){
	FILE *fp;
	char *f=(char *)trackerresponsesplit2[1].c_str();
	fp=fopen(f,"rb");
	if(fp!=NULL){
     string x="";
     int z=0;
     int chunck_of=stoi(trackerresponsesplit2[2]);
     if(chunck_of>500){
     	chunck_of=500;
     }
     while(z<chunck_of-1){
      
      x=x+"1"+" ";
      z++;

     }
     x=x+"1";
     return x;
	}
	else{
		string result="file_not_exist";
		return result;

	}
}
// else if(trackerresponsesplit2[0]=="senddata"){
// 	FILE *filepointer=fopen(trackerresponsesplit2[2].c_str(),"rb");
// 	int cn=stoi(trackerresponsesplit2[1]);

// if(filepointer!=NULL){
// 	int zx=cn*C_size;
//   int fsize=fseek(filepointer,zx,SEEK_SET);
//   if(fsize==0){
//   	char buff3[C_size];
//   	int size_of_read=fread(buff3,sizeof(char),C_size,filepointer);
//   	if(size_of_read<=0){
//   		string result="not_read";
// 		   return result;
//   	}
//   	else{
//   		string strrr=string(buff3);
//   		cout<<"size_of_read"<<size_of_read<<endl;
//   		string result;
//   		result=strrr;
//   		//result=result+" "+to_string(size_of_read);
//   		//cout<<result<<endl;
//   		cout<<"line no 375"<<endl;
//   		return buff3;
//   	}

//   }
//    else{
//    	perror("seek less then zero");
//    	string result="seet non-zero";
// 		return result;
//    }



// }
// else{
// 	string result="null file";
// 	return result;
// }



// }
string result="failed";
return result;


}


// char * originaldownload(vector<string> trackerresponsesplit2){

// if(trackerresponsesplit2[0]=="senddata"){
// 	FILE *filepointer=fopen(trackerresponsesplit2[2].c_str(),"rb");
// 	int cn=stoi(trackerresponsesplit2[1]);

// if(filepointer!=NULL){
// 	int zx=cn*C_size;
//   int fsize=fseek(filepointer,zx,SEEK_SET);
//   if(fsize==0){
//   	char buff3[C_size];
//   	int size_of_read=fread(buff3,sizeof(char),C_size,filepointer);
//   	if(size_of_read<=0){
//   		string result="not_read";
//   		char buff5[C_size];
//   		bzero(buff5, sizeof(buff5));
//   		strcpy(buff5,result.c_str());
// 		   return buff5;
//   	}
//   	else{
//   		// string strrr=string(buff3);
//   		// cout<<"size_of_read"<<size_of_read<<endl;
//   		// string result;
//   		// result=strrr;
//   		//result=result+" "+to_string(size_of_read);
//   		//cout<<result<<endl;
//   		cout<<"line no 375"<<endl;
//   		return buff3;
//   	}

//   }
//    else{
//    	perror("seek less then zero");
//    	string result="seet non-zero";
// 		 		char buff6[C_size];
// 		 		bzero(buff6, sizeof(buff6));
//   		strcpy(buff6,result.c_str());
// 		   return buff6;
//    }



// }
// else{
// 	string result="null_file";
// 	 		char buff7[C_size];
// 	 		bzero(buff7, sizeof(buff7));
//   		strcpy(buff7,result.c_str());
// 		   return buff7;
// }



// }
// string result="null_file";
// 	 		char buff8[C_size];
// 	 		bzero(buff8, sizeof(buff8));
//   		strcpy(buff8,result.c_str());
// 		   return buff8;



// }




void *implimentation(void *threadclientsocket){

map<string,file_info> file;
int *sockfd_ofth=(int *)threadclientsocket;
       int  sockfd_h=*sockfd_ofth;

	
	do{
		
    cout<<"\033[0;35m";
        cout<<"ENTER COMMAND: ";
         cout<<"\033[0m";

         
        string inputstring;
        getline(cin,inputstring);
        
        vector<string> inputcommandsplit;
      int n=inputstring.size();
    string caughtvalue="null";
    
    if(n!=0){
    caughtvalue=executionofcommand(inputstring,n,inputcommandsplit);
  }
  if(caughtvalue!="null"){
  	char inputcommand[2000];
   string atsr="";
string dstr="";
int rd=0;
for(int i=0;i<n;i++){
	if(inputstring[i]==' '){
		inputcommandsplit.push_back(dstr);
		if(rd==0){
			rd=1;
			atsr=atsr+dstr;
		}
		else{
		atsr=atsr+" "+dstr;}
		dstr="";

	}
	else{
		dstr=dstr+inputstring[i];
	}
}
inputcommandsplit.push_back(dstr);
if(rd==0){
			rd=1;
			atsr=atsr+dstr;
		}
		else{
		atsr=atsr+" "+dstr;}

  	if(caughtvalue=="upload_file"){
  		int size_inputcommandsplit=inputcommandsplit.size();
  		int  last=0;
  		for(int i=inputcommandsplit[1].size()-1;i>=0;i--){
  			if(inputcommandsplit[1][i]=='/'){
  				last=i;
  				break;
  			}
      }
      if(last!=inputcommandsplit[1].size()-1){
      inputcommandsplit[1]=inputcommandsplit[1].substr(last,inputcommandsplit[1].length());}
      if(inputcommandsplit[1][0]=='.'){
      	inputcommandsplit[1]=inputcommandsplit[1].substr(1,inputcommandsplit[1].length());
      }
      file[inputcommandsplit[1]].size=file_size(inputcommandsplit[1]);
      file[inputcommandsplit[1]].pth=file_path(inputcommandsplit[1]);
                int x=ceil((double)(file[inputcommandsplit[1]].size)/C_size);
      file[inputcommandsplit[1]].nofchunck=x;
      

        vector<int> v1(x,1);
      file[inputcommandsplit[1]].chuncks=v1;
        atsr+=" "+to_string(x)+" "+to_string(file[inputcommandsplit[1]].size)+" "+file[inputcommandsplit[1]].pth;

  	}
  	else if(caughtvalue=="download_file"){
  		atsr+=" "+file_path(inputcommandsplit[3])+inputcommandsplit[3]+"/";
  	}
     
  	else if(caughtvalue=="login"){
     atsr+=" "+string(client_addr)+" "+string(client_port);
     

  	}
    
    	bzero(inputcommand,sizeof(inputcommand));
    	strcpy(inputcommand,atsr.c_str());
    	
    	write(sockfd_h,inputcommand,sizeof(inputcommand));
    	bzero(inputcommand,sizeof(inputcommand));
    	read(sockfd_h,inputcommand,sizeof(inputcommand));
    	string trackerresponse;
    	trackerresponse=string(inputcommand);
    	vector<string> trackerresponsesplit;
    	string dstr1="";
    	int n2=trackerresponse.size();
for(int i=0;i<n2;i++){
	if(trackerresponse[i]==' '){
		trackerresponsesplit.push_back(dstr1);
		dstr1="";
	}
	else{
		dstr1=dstr1+trackerresponse[i];
	}
}
trackerresponsesplit.push_back(dstr1);
if(trackerresponsesplit[0]=="done"){

	download_perform(trackerresponsesplit,file);

}
cout<<"\033[0;35m";
        

cout<<"SERVER RESPONSE: ";

cout<<"\033[0m";
cout<<trackerresponse<<endl;

if(trackerresponsesplit[0]=="exit"){

	cout<<"client exit!"<<endl;
	break;
}


       
  }
  


    //current work;
	
  
	}while(1);
  close(sockfd_h);

	return NULL;
}
void *implimentation1(void *threadclientsocket){
	cout<<"bit vector find"<<endl;
	int *sockfd_t=(int *)threadclientsocket;
	char buff[C_size];
	int sockfd_h=*sockfd_t;
	bzero(buff, sizeof(buff));
		memset(&buff, 0, sizeof(buff));
		read(sockfd_h,buff,sizeof(buff));
		string trackerresponse2=string(buff);
		vector<string> trackerresponsesplit2;
    	string dstr="";
    	int n2=trackerresponse2.size();
for(int i=0;i<n2;i++){
	if(trackerresponse2[i]==' '){
		trackerresponsesplit2.push_back(dstr);
		dstr="";
	}
	else{
		dstr=dstr+trackerresponse2[i];
	}
}
trackerresponsesplit2.push_back(dstr);
char buff34[C_size];
bzero(buff34, sizeof(buff34));
int cnp,noofclient;
if(trackerresponsesplit2[0]!="senddata"){
	cnp=stoi(trackerresponsesplit2[2]);
  noofclient=stoi(trackerresponsesplit2[3]);
string result=server_reaction(trackerresponsesplit2);
strcpy(buff34,result.c_str());
}
//cout<<result<<"line no 570"<<endl;
write(sockfd_h,buff34,sizeof(buff34));

bzero(buff, sizeof(buff));
bzero(buff34, sizeof(buff34));
memset(&buff, 0, sizeof(buff));
bzero(buff, sizeof(buff));
cout<<"chunck transfer"<<endl;
int y;
	while(cnp--){
		memset(&buff, 0, sizeof(buff));
		bzero(buff, sizeof(buff));
		read(sockfd_h,buff,sizeof(buff));
		string trackerresponse23=string(buff);
		vector<string> trackerresponsesplit23;
    	string dstr1="";
    	int n23=trackerresponse23.size();
for(int i=0;i<n23;i++){
	if(trackerresponse23[i]==' '){
		trackerresponsesplit23.push_back(dstr1);
		dstr1="";
	}
	else{
		dstr1=dstr1+trackerresponse23[i];
	}
}
trackerresponsesplit23.push_back(dstr1);
bzero(buff34, sizeof(buff34));
if(trackerresponsesplit23[0]!="senddata"){
string result1=server_reaction(trackerresponsesplit23);
strcpy(buff34,result1.c_str());

}
else{

   
	if(trackerresponsesplit23[0]=="senddata"){
		

	FILE *filepointer=fopen(trackerresponsesplit23[2].c_str(),"rb");
	int cn=stoi(trackerresponsesplit23[1]);
	
    y=cn%noofclient;
if(filepointer!=NULL){
	int zx=cn*C_size;
  int fsize=fseek(filepointer,zx,SEEK_SET);
  if(fsize==0){
  	bzero(buff34, sizeof(buff34));
  	int size_of_read=fread(buff34,sizeof(char),C_size,filepointer);
  	if(size_of_read<=0){
  		string result="not_read";
      
  		bzero(buff34, sizeof(buff34));
  		strcpy(buff34,result.c_str());

		 
  	}
  	else{
  		// string strrr=string(buff3);
  		// cout<<"size_of_read"<<size_of_read<<endl;
  		// string result;
  		// result=strrr;
  		//result=result+" "+to_string(size_of_read);
  		//cout<<result<<endl;
  		
  	  if(y==stoi(trackerresponsesplit23[3])-1){
  	  	cout<<cn<<" readed"<<endl;
  	  }
  		// return buff3;
  	}

  }
   else{
   	perror("seek less then zero");
   	string result="seet non-zero";
		 		
		 		bzero(buff34, sizeof(buff34));
  		strcpy(buff34,result.c_str());
   }



}
else{
	string result="null_file";
	 		bzero(buff34, sizeof(buff34));
  		strcpy(buff34,result.c_str());
}
fclose(filepointer);


}


}

// vector<string> trackerresponsesplit3;
//     	string dstr1="";
//     	int n3=result.size();
// for(int i=0;i<n3;i++){
// 	if(result[i]==' '){
// 		trackerresponsesplit3.push_back(dstr1);
// 		dstr1="";
// 	}
// 	else{
// 		dstr1=dstr1+result[i];
// 	}
// }
// trackerresponsesplit3.push_back(dstr1);





//cout<<result<<"line no 570"<<endl;
write(sockfd_h,buff34,sizeof(buff34));
if(y==stoi(trackerresponsesplit23[3])-1){
  	  	cout<<"Data send"<<endl;
  	  }
bzero(buff, sizeof(buff));
bzero(buff34, sizeof(buff34));
memset(&buff, 0, sizeof(buff));
//read(sockfd_h,buff,sizeof(buff));
//string strtr=string(buff);
//if(buff!="2"){
	//bzero(buff, sizeof(buff));
	//continue;
//}


//if(buff=="2"){
	//write(sockfd_h, trackerresponsesplit3[1].c_str(), sizeof(trackerresponsesplit3[1].c_str()));
//}
bzero(buff, sizeof(buff));
	}
return NULL;

}

// void *implimentation2(void *threadclientsocket){
// 	cout<<"implementation023";
	
// 	while(1){
// 		cout<<"while"<<endl;
// 		int m;
// 		cin>>m;
// 	}
// }



int main(int argc,char *argv[]){
  client_port=argv[2];
  client_addr=argv[1];
  struct sockaddr_in addressOfServer;
  const char *port;
  const char *ip;

  std::ifstream infile(argv[3]);

string inip,inport;
getline(infile,inip);
getline(infile,inport);
port=inport.c_str();
ip=inip.c_str();

	int sockinfo = socket(AF_INET, SOCK_STREAM, 0);
	if (sockinfo < 0)
	{
		cout << "socket creation fail" << endl;
		exit(1);
	}
	else{
		cout<<"Socket created..."<<endl;
	}

	 bzero(&addressOfServer,sizeof(addressOfServer));

	   addressOfServer.sin_family = AF_INET;
		addressOfServer.sin_port = htons(atoi(port));

		int init=inet_pton(AF_INET,ip,&addressOfServer.sin_addr);
		if(init<1){
			cout<<"Error in inet_pton"<<endl;
		}

		int connet = connect(sockinfo, (sockaddr *)&addressOfServer, sizeof(addressOfServer));
		
		if (connet==-1)
		{
			cout << " connection failed" << endl;
			exit(1);
		}
		else
		{
			cout << "connection successfully..." << endl;
		}
       
  
   int *cchild=(int *)malloc(sizeof(int));
   *cchild=sockinfo;
   pthread_t th;
   pthread_create(&th,NULL,implimentation,cchild);
   int sockinfo_asclint;
   struct sockaddr_in addressOfServer_asclient,addressOfclient_asclient;
   sockinfo_asclint= socket(AF_INET, SOCK_STREAM, 0);
   if (sockinfo_asclint< 0)
	{
		cout << "socket creation fail" << endl;
		exit(1);
	}
	else{
		cout<<"Socket created..."<<endl;
	}

	 bzero(&addressOfServer_asclient,sizeof(addressOfServer_asclient));

	   addressOfServer_asclient.sin_family = AF_INET;
		addressOfServer_asclient.sin_port = htons(atoi(client_port));
		addressOfServer_asclient.sin_addr.s_addr=htons(INADDR_ANY);

  if (bind(sockinfo_asclint, (sockaddr *)&addressOfServer_asclient, sizeof(addressOfServer_asclient)) != 0)
  {
    cout << "NOT BINDED" << endl;
  }
  else
  {
    cout << "BINDED" << endl;

  }


		 int listn_clint = listen(sockinfo_asclint, 5);
  if (listn_clint == 0)
  {
    cout << " listening........." << endl;
  }
  else
  {
    cout << "listening failed......" << endl;
    exit(0);
  }
  while(1){
  	int accpt;
  	socklen_t len1;
  	len1=sizeof(addressOfclient_asclient);
  	if((accpt=accept(sockinfo_asclint, (sockaddr *)&addressOfclient_asclient, &len1))<0){
          cout<<"server accept failed"<<endl;
          exit(0);

  	}
  	
int *cchild_c=(int *)malloc(sizeof(int));
*cchild_c=accpt;
pthread_t th_c;
pthread_create(&th_c,NULL,implimentation1,cchild_c);

  }
pthread_join(th,NULL);



}