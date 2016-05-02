#include "count.h"

typedef struct Node node;
node *table[1000];

int key=1000;
int count=0;

void insert(unsigned long long data, unsigned long long val){

	unsigned long long index=val%key;
	if(table[index]==NULL){
		node *tmp=(struct Node *) malloc(sizeof(struct Node));
		tmp->data=data;
		tmp->next=table[index];
		table[index]=tmp;
	}else{
		node *tmp;
		for(tmp=table[index];tmp!=NULL;tmp=tmp->next){
			if(tmp->data==data){

				return;
			}
		}
		tmp=(struct Node *) malloc(sizeof(struct Node));
		tmp->data=data;
		tmp->next=table[index];
		table[index]=tmp;
	}
	count++;

}




int main(int argc, char *argv[]){

    if(argc==1){
  		printf("error\n");
  		return 0;

  	}

  	FILE *txtfile= fopen(argv[1], "r");

  	if(txtfile==NULL){

  		printf("error\n");
  		return 0;
  	}

		int it=0;
		for(it=0;it<1000;it++){
			table[it]=NULL;
		}


    unsigned long long hexNum;
    unsigned long long x;



    while(fscanf(txtfile,"%llx\n", &hexNum)!=EOF){


        x=(unsigned long long)hexNum;


					insert(hexNum,x);


    }
		printf("%d\n",count );

    fclose(txtfile);
    return 0;
}
