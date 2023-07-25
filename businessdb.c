#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include<stdlib.h>
typedef struct customer{
	char*email;
	char*name;
	int shoe_size;
	char*favorite_food;
}customer;

typedef struct node{
	customer client;
struct node*next;
}node;
 node*table[1024];


// this is the hash function we are using it because we will use a hash table as a data structure. it returns a hash value we use it to determine the index of a key in the hash table
	int hash(char*str){
	int hash = 5381;	
	int h=0;	
	char c;
  while ( str[h]!= '\0') {
   
    c = str[h];
    hash = ((hash << 5) + hash) + (int)c; 
    h++;
  }
  hash=hash%1024;
  if(hash<0){
	  hash=hash*(-1);
  }
  return hash;
  }
  // this fuction makes a node and put the email address in it and returns the pointer of this node
node* create(char*str){
	node*new;
	new=malloc(sizeof(node));
	new->next=NULL;
	new->client.email=strdup(str);

	return new;
}
// this function loads up the data from the "customers.tsv" file
void load_customers_from_file(void){
    FILE *infile;
    char buf[1024];
// we open the filein reading mode
    infile = fopen("customers.tsv", "r");
    char *nextpart;
    int g;
node*helper;
int position;
// we use fgets() function to extract lines from the file and we use strtok() to seperate the fields in each line
//we fill the data in the hash table
    while(fgets(buf, 1024, infile) != NULL){
        nextpart = strtok(buf, "\t");
		g=0;
		position=hash(nextpart);
helper=table[position];
if(helper==NULL){
	table[position]=create(nextpart);
	helper=table[position];
}else{
	while(helper->next!=NULL){
		helper=helper->next;
	}
	helper->next=create(nextpart);
	helper=helper->next;
}
        while((nextpart = strtok(NULL, "\t\n")) != NULL){
		
 if(g==0){
				helper->client.name=strdup(nextpart);
			}
			if(g==1){
				helper->client.shoe_size=atoi(nextpart);
			}
        if(g==2){
			helper->client.favorite_food=strdup(nextpart);
	}
	g++;
	helper->next=NULL;

		}
	

    }


    fclose(infile);

}

// helper function
void remove_newline(char *s){
    if(!s){
        return;
    }

    char *newline;
    newline = strchr(s, '\n');
    if(newline){
        *newline = '\0';
    }
}
// this function takes a key as arguments( the key will be the address email) and search for that key in the hash table
//if it finds that key, it will return the pointer to the node which the key is inside
// if it didn't find it it will returns NULL
node* find(char*key){
	node*help=table[hash(key)];
	if(help==NULL){
		return NULL;
	}
	else{
		while(help!=NULL){
			if(strcmp(help->client.email,key)==0){
				return help;
			}else{
			help=help->next;}
		}
		return NULL;
	}
	}
	// this funtion is to add a new customer to the data base with the help of find() function
	// so if the email address is already exist in the hash table we will just edit that customer
	// if the address email does not exist in the hash table so we create a new node in the hash table 
void add(char*mail,char*NAME,int*shoe , char*FOOD){
	int position=hash(mail);
node*p;
if(table[position]==NULL){
	table[position]=create(mail);
	p=table[position];
}else{
	p=find(mail);
	if(p==NULL){
		node*help=table[position];
		while(help->next!=NULL){
			help=help->next;
		}
		help->next=create(mail);
		p=help->next;
	}
}
	p->client.name=strdup(NAME);
	p->client.shoe_size=*shoe;
	p->client.favorite_food=strdup(FOOD);




}

// this function look for a customer in the data base using his email address as a key 
// we use find() to help us
void lookup(void){
		printf("enter the email of the customer you are looking for : ");
        		char key[100];
        		gets(key);
        		
	node*help=find(key);
	if(help!=NULL){
	printf(" -customer's email : %s \n",help->client.email);
		printf(" -customer's name : %s \n",help->client.name);
			printf(" -customer's shoe size : %d \n",help->client.shoe_size);
				printf(" -customer's email : %s \n",help->client.favorite_food);
}
else{
	printf("customer not found . \n");
}

}
// this function delete a customer from the data base using his email address as a key to find him
int delete(void){
			printf("enter the email of the customer you want to delete : ");
        		char key[100];
        		gets(key);
        	
        	
		int position=hash(key);
	 node*help=table[position];
	node*helpp=NULL;
	while(help!=NULL){
	if(strcmp(help->client.email,key)==0){
		if(helpp==NULL){
			table[position]=NULL;
			return 0;
		}else{
		helpp->next=help->next;
		return 0;}
	}else{
		helpp=help;
		help=help->next;
	}
	
}
printf("customer not found . \n");
return 0;
}
// this function display all informations of customers in the data base
void display(void){
	node*help;
	bool is=false;
	for(int i=0; i<1024;i++){
		help=table[i];
		if(help!=NULL){
		while(help!=NULL){
			is=true;
			printf(" -customer's email : %s \n",help->client.email);
		printf(" -customer's name : %s \n",help->client.name);
			printf(" -customer's shoe size : %d \n",help->client.shoe_size);
				printf(" -customer's favorite food : %s \n",help->client.favorite_food);
				printf("--------------------------------\n");
				help=help->next;
}
		}
	}
		if(is==false){
			printf("there is no customer in the database");
		}
	
}
// this function save the data base after editting it
void save(void){
FILE*file=fopen("customers.tsv", "w");
node*help;
for(int u=0;u<1024;u++){
	help=table[u];
	if(help!=NULL){
		while(help!=NULL){
			fputs(help->client.email , file);
			fputs("\t",file);
			fputs(help->client.name, file);
			fputs("\t",file);
			char str[100];
			itoa(help->client.shoe_size,str,10);
			fputs(str, file);
			fputs("\t",file);
			fputs(help->client.favorite_food, file);
			fputs("\n",file);

help=help->next;
		}
	}

}

fclose(file);

}
// this function free the memory

void quit(node*fn){
	   	if(fn!=NULL){
	   	quit(fn->next);}
	
	free(fn);
	
}
int main(void){
	// we set 
    for(int i=0;i<1024;i++){
	table[i]=NULL;}
    load_customers_from_file();
    char buf[80];
    char *command;
    while(true){
        printf("command: ");
        command = fgets(buf, 80, stdin);
        if(!command){
            break;
        }
        remove_newline(buf);

        if(strcmp(buf, "quit")==0){
           for(int i=0;i<1024;i++){
           	quit(table[i]);
           }
            break;
        }else if(strcmp(buf,"add")==0){
            printf("Do the add command. \n");
			char email[100];
			char name[100];
			char fd[100];
            int size;
			
			printf("please enter an email address : ");
			gets(email);
			printf("\n'''''''''''''''''''''''''''\n");
			printf("please enter the name : ");
			gets(name);
			printf("\n'''''''''''''''''''''''''''\n");
			
			printf("please enter favorite food : ");
			gets(fd);
			printf("\n''''''''''''''''''''''''''' \n");
		
        	printf("please enter shoe size : ");
			scanf("%d",&size);
			printf("\n");
			
			
            add(email,name,&size,fd);
			printf("************************************************** \n");
        }
        else if(!strcmp(buf, "lookup")){
        	lookup();
				printf("************************************************** \n");
			}
        	else if(!strcmp(buf,"delete")){
        	
        		delete();
					printf("**************************************************\n" );
        	}
        	else if(!strcmp(buf, "list")){
        		display();
					printf("************************************************** \n");
				}
        	else if(strcmp(buf, "save")==0){
				
			save();
				 
					printf("************************************************** \n");
        	}
        else{
            printf("Unkown command!\n");
        }

        printf("Got this command: %s\n", buf);
    }

    return 0;
}

