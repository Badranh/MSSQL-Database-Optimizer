
#include <stdio.h>
#include <string.h>
#include "csvparser.h"

typedef struct ColName{
	char ColumnName[64];
	int accesses;
	struct ColName* next;
} ColName;

void push(ColName **head, int accesses,char ColumnName[])
{
    ColName *temp = malloc(sizeof(ColName));
    temp->accesses = accesses;
	strcpy(temp->ColumnName,ColumnName);
    temp->next = *head;
    *head = temp;
}

void printList(ColName*head){
	while(head->next!=NULL){
		printf("Column Name: %s , Accesses: %d\n",head->ColumnName,head->accesses);
		head=head->next;		
	}
}
void getColNameFromQueryAndInsertIt(ColName **head,char* query){
	
	 int i,equalPos,j,from,byEqual=0,till=0;
    char column[64];
    
    for(i=0;i<strlen(query);i++){
        if(query[i]=='='){
           equalPos = i;
        }
    }
    if(query[equalPos-1]==' '){
    equalPos--;
    while(query[equalPos]==' '){
        equalPos--;
    }
        till = equalPos;
    }else{
        till = equalPos-1;
    }

    while(query[equalPos]!=' '){
        equalPos--;
        from=equalPos+1;
    }

    i=0;
    while(from < till+1){
        column[i++]=query[from++];
    }
    column[i]='\0'; 
	ColName* cur = *head;
		while(cur){
			if(!strcmp(cur->ColumnName,column)){
				cur->accesses++;
				return;
			}
			cur = cur->next;
	} 
	push(head,1,column); 
}


char *trim(char *str)
{
  char *end;

  while(isspace((unsigned char)*str)) str++;

  if(*str == 0) 
    return str;
  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;
  end[1] = '\0';

  return str;
}

int countList(ColName *head){
	int counter=0;
	while(head->next){
		counter++;
		head=head->next;
	}
	return counter;
}
int main()
{
	ColName *list =  malloc(sizeof(ColName));
	list->next = NULL;
	int i =  0,counts=0;
	char buf[1024],dbName[64],fileLog[64],filePath[1024];
	
	printf("please enter database name: \n");
	scanf("%s",dbName);
	
	printf("please enter full log file path (csv format sperated by ,): \n");
	scanf("%s",fileLog);
	
    CsvParser *csvparser = CsvParser_new(fileLog,",", 1);
    CsvRow *header;
    CsvRow *row;

    header = CsvParser_getHeader(csvparser);
    if (header == NULL) {
        printf("%s\n", CsvParser_getErrorMessage(csvparser));
        return 1;
    }
    char **headerFields = CsvParser_getFields(header);
  

    while ((row = CsvParser_getRow(csvparser)) ) {
        char **rowFields = CsvParser_getFields(row);
        for (i = 0 ; i < CsvParser_getNumFields(row) ; i++) {
            if(i==27 && trim(rowFields[i])!= "" ){
            	getColNameFromQueryAndInsertIt(&list,rowFields[i]);
			}
				
        }
        CsvParser_destroy_row(row);
    }
    CsvParser_destroy(csvparser);
	sprintf(buf, ".\\createIndexesBatchFile.bat %d %s", countList(list),dbName); // puts string into buf
	printf("Please Consider Adding Indexes On Columns Having More Than 100 Accesses:\n\n");
	printList(list);   
	system(buf);
    return 0;
}

