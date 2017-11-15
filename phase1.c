/*
Seng265 Assignment 1
Darian Morrison
May 27th, 2017
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILENAME_LEN 40
#define MAX_STRING_LEN 22

int compare_strings(const void *a, const void *b);




int main(int argc,char *argv[]){
//File declarations
	int op=0;
	int i,j,len;
	char infilename[MAX_FILENAME_LEN];
	char outfilename[MAX_FILENAME_LEN];
	char magicNum[]={'\xab','\xba','\xbe','\xef','\x14','\x00','\x00','\x00'};
	FILE *infile;
	FILE *outfile;

//FILE OPENING STARTS
	for(i=1; i<argc; i++){
		if(strcmp(argv[i],"--backward") == 0){
			op=1;
		}
		if(strcmp(argv[i],"--forward") == 0){
			op=2;
		}
		if(strcmp(argv[i],"--infile") == 0){
			if(i+1 >= argc){
				fprintf(stderr,"Unable to open infile\n");
				exit(1);	
			}strncpy(infilename,argv[i+1],MAX_FILENAME_LEN);
		}
		if(strcmp(argv[i],"--outfile") == 0){
			if(i+1 >= argc){
				fprintf(stderr,"Unable to open outfile\n");
				exit(1);	
			}strncpy(outfilename,argv[i+1],MAX_FILENAME_LEN);
		}
	}
	if(op == 0){
		fprintf(stderr,"Please provide --forward or --backward in command line\n");
		exit(1); 
	}
	outfile=fopen(outfilename,"w");
	if(outfile == NULL){
		fprintf(stderr,"Unable to open outfile\n");
		exit(1);	
	}
	infile=fopen(infilename,"r");
	if(infile==NULL){
		fprintf(stderr,"Unable to open infile\n");
		exit(1);	
	}
//ARGUMENTS HAVE BEEN TAKEN PERFORM OPERATIONS
	if(op == 2){
		//Forward op declerations
		char st[MAX_STRING_LEN-1]={};
		char matrix[MAX_STRING_LEN-1][MAX_STRING_LEN-1]={};

		//Write magic numbers/block size
		fwrite(magicNum,sizeof(char),sizeof(magicNum),outfile);
		//read file
		while(!(feof(infile))){

			len=0;
			while(len<20){
				if((st[len]=fgetc(infile))==EOF){
					break;
				}
				len++;
			}st[len]=0x03;
			len++;

			//Build Matrix
			for(i=0; i<len; i++){
			matrix[0][i]=st[i];
			}
			//preform forward transformation
			char temp;
			for(i=1; i<len; i++){
				temp=matrix[i-1][0];
				for(j=0; j<len-1;j++){
					matrix[i][j]=matrix[i-1][j+1];
				}
				matrix[i][len-1]=temp;	
			}

			//sort matrix
			qsort(matrix,len,sizeof(char)*(MAX_STRING_LEN-1),compare_strings);
			for(i=0;i<(MAX_STRING_LEN-1);i++){
				st[i]=matrix[i][len-1];
			}

			//write transformation to file
			char forward [len];
			if(st[0] != 0x03){
				strncpy(forward,st,len);
				fwrite(forward,sizeof(char),sizeof(forward),outfile);
			}
		}
	
	
	}if(op == 1){
		//backward op declerations
		char inNumbers[9];
		char matrix[MAX_STRING_LEN][MAX_STRING_LEN]={};
		char st[MAX_STRING_LEN-1]={};


		fread(inNumbers,sizeof(char),sizeof(magicNum),infile);
		if((strncmp(inNumbers,magicNum,sizeof(magicNum))!=0)){
			fprintf(stderr,"Wrong file type for transformation\n");
			exit(1);
		}
	//read file after nums
		while(!(feof(infile))){
			len=0;
			while(len<(MAX_STRING_LEN-1)){
				if((st[len]=fgetc(infile))==EOF){
					break;
				}
				len++;
			}
			char matrix[MAX_STRING_LEN][MAX_STRING_LEN]={};
			char temp[MAX_STRING_LEN][MAX_STRING_LEN]={};
			for(i=0,j=0;j<len;j++){
				matrix[j][i]=st[j];
			}
			int k=0;
			while(k < len){
				for(i=0;i<len;i++){
					for(j=0;j<len;j++){
						temp[i][j]=matrix[i][j];
					}
				}
			
				k++;
				qsort(temp,len,sizeof(char)*MAX_STRING_LEN,compare_strings);
				for(j=0;j<len;j++){
					matrix[j][k]=temp[j][k-1];
				}
			}
			for(i=0;i<len;i++){
				if(matrix[i][len-1] == 0x03){
					strncpy(st,matrix[i],len);			
				}
			}
			
		//write transformation to file
			if(len>0){
				char backward[len-1];
				strncpy(backward,st,len-1);
				fwrite(backward,sizeof(char),sizeof(backward),outfile);
			}
		}
	}
//End close files
	fclose(infile);
	fclose(outfile);
	
}

int compare_strings(const void *a, const void *b){
	char *sa = (char*)a;
	char *sb = (char*)b;

	return strncmp(sa,sb,MAX_STRING_LEN);
}



