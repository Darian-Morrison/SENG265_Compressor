#include <assert.h>
#include <ctype.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"

#define ENCODE_MODE 0
#define DECODE_MODE 1
#define EOT '\03'
#define magicNum1 "\xab\xba\xbe\xef"
#define magicNum2 "\xda\xaa\xaa\xad"

void MtF(charval_t** data);
void i_MtF(charval_t** data);
void RL(charval_t** data);
void i_RL(charval_t** data);
void output_list(charval_t* data, FILE* outfile);
charval_t* data_list(FILE* infile);
void decode(char *infile_name, char *outfile_name);
void encode(char *infile_name, char *outfile_name);

void output_list(charval_t* data, FILE* outfile){
	//run through list, if has char val out put it
	charval_t* curr=NULL;
	for(curr=data; curr!=NULL;curr=curr->next){
		if(curr->val>=0){		
			fputc(curr->val+128,outfile);
		}
		if(curr->c != '\0'){
			fwrite(&curr->c,sizeof(unsigned char),1,outfile);
		}
	
	}
	
}

void i_RL(charval_t** data){
	charval_t* temp = NULL;
	int count=0;
	charval_t* curr=NULL;
	//run through list and replace 0 and count with required # of 1s
	for(curr=*data ;curr!= NULL;curr=curr->next){
		if(curr->val == 0){
			curr->val = 1;
			count=curr->next->val;
			curr=curr->next;
			curr->val =1;
			while(count-2>0){
				temp=new_charval('\0',1);
				temp->next = curr->next;
				curr->next = temp;
				curr=temp;
				count--;	
			}
		}
	}
}

void RL(charval_t** data){
	//iterate through list if
	//if 1 save node
	charval_t* temp = NULL;
	charval_t* first = NULL;
	int count = 0;
	charval_t* curr=NULL;
	for(curr=*data ;curr!= NULL;curr=curr->next){
		if(curr->val == 1){
			count++;
			if(first ==NULL){
				first = curr;
			}
		}else if(count>2){
		//change first two values of nodes to 0,count respectively
			first->val = 0;
			first->next->val = count;
		//temp set to first val to free
			temp=first->next->next;
		//firsr set to node holding count and its next now points to curr
			first=first->next;
			first->next=curr;
			while(count-2>0){
				first=temp;
				temp=temp->next;
				free_charval(first);
				count--;
			}
			first = NULL;
			count=0;
		}else{
			count=0;
			first = NULL;

		}
	}
}

void i_MtF(charval_t** data){
	charval_t* dict = NULL;
	charval_t* new = NULL;
	charval_t* next = NULL;
	charval_t* curr=NULL;
	//run through data
	for(curr=*data; curr!=NULL;curr=curr->next){
		//retreive val from dictionary
		new=find_c(&dict,(curr->val)-1);
		//if not in dict take next char and put in dict
		//replace current node with next and remove next
		if(new==NULL){
			new=new_charval(curr->next->c,curr->next->val);
			dict=add_end(dict,new);
			//replace curr->next values
			curr->val=curr->next->val;
			curr->c=curr->next->c;
			//remove next
			next=curr->next;
			curr->next=next->next;
			free_charval(next);
		}else{
			new=pop_c(&dict,curr->val-1);
			curr->val=new->val;
			curr->c=new->c;
			dict=add_front(dict,new);
		}
	}
	empty_list(dict);
}

void MtF(charval_t** data){
	//make dictionary
	charval_t* dict = NULL;
	charval_t* new = NULL;
	//iterate through data list
	charval_t* curr=NULL;
	for(curr=*data ;curr!= NULL;curr=curr->next){
		int index= index_c(dict,curr->c);
		//if not in list,add to back of list and give val of new pos+1 in dict
		if(index == -1){
			new = new_charval(curr->c,-1);
			dict=add_end(dict,new);
			curr->val = index_c(dict,curr->c)+1;
			
		}else{
		//if in list give val to pos and pop to front
			dict=add_front(dict, pop_c(&dict,index));
			curr->val=index+1;
			curr->c = '\0';	
		}
		
	}
	empty_list(dict);
}

charval_t* data_list(FILE* infile){
	charval_t* list=NULL;
	charval_t* new=NULL;
	unsigned char c;
	while(c=fgetc(infile)){
		
		if(feof(infile)){break;}
		if(c<=127){
			new=new_charval(c,-1);
		}else if (c>127){
			new=new_charval('\0',((int)c)-128);
		}
		list=add_end(list,new);
	}
	return list;
}
void usage() {
    fprintf(stderr, "usage: phase2 [--encode|--decode] " \
                    "--infile <filename> --outfile <filename>\n"
    );
}
void decode(char *infile_name, char *outfile_name){
	FILE *infile;
	//open infile
	infile=fopen(infile_name,"r");
	if(infile==NULL){
		fprintf(stderr,"Unable to open infile\n");
		exit(1);	
	}
	//read magic nums and block size 
	char inNums[5];
	int blockSize[2]; 
	fread(inNums,sizeof(char),4,infile);
	if((strncmp(inNums,magicNum2,4)!=0)){
		fprintf(stderr,"Wrong file type for transformation\n");
		exit(1);
	}
	fread(blockSize,sizeof(int),1,infile);
	//open outfile
	FILE *outfile=fopen(outfile_name,"w");
	//write magic numbers and block size to file
	fwrite(magicNum1,sizeof(char),4,outfile);
	fwrite(blockSize,sizeof(int),1,outfile);
	//read infile into list.
	charval_t* list = data_list(infile);
		//close infile 
	fclose(infile);
	//preform i_RL encoding
	i_RL(&list);
	//preform i_MtF transformation
	i_MtF(&list);
	//output list to file.
	output_list(list,outfile);
	//close outfile
	empty_list(list);
	fclose(outfile);
}

void encode(char *infile_name, char *outfile_name){
	//open infile
	FILE *infile=fopen(infile_name,"r");	
	if(infile==NULL){
		fprintf(stderr,"Unable to open infile\n");
		exit(1);	
	}
	//read file magic nums and blocksize
	char inNums[5];
	int blockSize[2]; 
	fread(inNums,sizeof(char),4,infile);
	if((strncmp(inNums,magicNum1,4)!=0)){
		fprintf(stderr,"Wrong file type for transformation\n");
		exit(1);
	}
	fread(blockSize,sizeof(int),1,infile);
	//close infile 
	
	//open outfile
	FILE *outfile=fopen(outfile_name,"w");
	//write magic numbers and block size to file
	fwrite(magicNum2,sizeof(char),4,outfile);
	fwrite(blockSize,sizeof(int),1,outfile);
	//Move data to list and close infile
	charval_t* list=data_list(infile);
	fclose(infile);
	//preform MtF transform
	MtF(&list);
	//preform RL encoding
	RL(&list);
	//output list to stream.
	output_list(list,outfile);
	//close outfile
	empty_list(list);
	fclose(outfile);
}

int main(int argc, char *argv[]) {
    int c;
    char *infile_name = NULL;
    char *outfile_name = NULL;
    int mode;
    int encode_flag = 0;
    int decode_flag = 0;

    /*
     * Based on http://bit.ly/2tHBpo1
     */
    for (;;) {
        static struct option long_options[] = {
            {"encode",     no_argument,       0, 'e'},
            {"decode",     no_argument,       0, 'd'},
            {"infile",     required_argument, 0, 'i'},
            {"outfile",    required_argument, 0, 'o'},
            {0, 0, 0, 0}
        };
        int option_index = 0;

        c = getopt_long (argc, argv, "efi:o:",
            long_options, &option_index);

        if (c == -1) {
            break;
        }

        switch (c) {
        case 'i':
            infile_name = optarg;
            break;
        case 'o':
            outfile_name = optarg;
            break;
        case 'e':
            encode_flag = 1;
            break;
        case 'd':
            decode_flag = 1;
            break;
        default:
            fprintf(stderr, "shouldn't be here...");
            assert(0);
        }
    }

    if (encode_flag == 0 && decode_flag == 0) {
        usage();
        exit(1);
    } else if (encode_flag == 1 && decode_flag == 1) {
        fprintf(stderr, "usage: choose one of --decode or --encode\n");
        exit(1);
    } else if (encode_flag == 1) {
        mode = ENCODE_MODE;
    } else if (decode_flag == 1) {
        mode = DECODE_MODE; 
    } else {
        fprintf(stderr, "shouldn't be here...\n");
        assert(0);
    }

    if (infile_name == NULL) {
        usage();
        exit(1);
    }
        
    if (outfile_name == NULL) {
        usage();
        fprintf(stderr, "%s: need --outfile <filename>\n", argv[0]);
        exit(1);
    }

    if (mode == ENCODE_MODE) {
	encode(infile_name,outfile_name);
    } else if (mode == DECODE_MODE) {

 
	decode(infile_name,outfile_name);

    } else {
        /* Shouldn't be here!! */
        assert(0);
    }

    return 0;
}
