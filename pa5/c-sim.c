#include "c-sim.h"

int mr=0;
int mw=0;
int ch=0;
int cm=0;
int t=0;





void read(int data, int bo, int in, int tag, struct cache *c){
  int i;
  struct set *sptr=c->s;

  for(i=0;i<in;i++){/*//get to the right set*/
    sptr=sptr->next;
  }

  struct block *bptr=sptr->b;
  struct offset *ptr;
  for(bptr=sptr->b;bptr!=NULL;bptr=bptr->next){/*//check every block*/
    if(bptr->tag==-1){/*//empty block*/
      cm++;
      mr++;
      bptr->tag=tag;
      bptr->t=t;
      ptr=bptr->off;

      for(i=0;i<bo;i++){
        ptr=ptr->next;
      }

      ptr->data=data;
      return;
    }else if(bptr->tag==tag){/*//found the block*/
      ptr=bptr->off;
      for(i=0;i<bo;i++){
        ptr=ptr->next;
      }

      if(ptr->data==-1){/*//empty offset*/
        cm++;
        mr++;
        ptr->data=data;
        return;
      }else if(ptr->data==data){/*//hit*/
        ch++;
        return;
      }else{/*//something that isnt the data in the offset*/
        ptr=bptr->off;

        for(ptr=bptr->off;ptr!=NULL;ptr=ptr->next){
          if(ptr->data==data){/*//if the data is in a different offset*/
            ch++;
            return;
          }
        }
        /*//data is not in any offset so replace block?
        //replace block method*/
        bptr->t=t;
        bptr->tag=tag;
        for(ptr=bptr->off;ptr!=NULL;ptr=ptr->next){
          ptr->data=-1;
        }
        ptr=bptr->off;
        for(i=0;i<bo;i++){
          ptr=ptr->next;
        }
        ptr->data=data;
        cm++;
        mr++;
        return;
        /*//end of replace block method*/



      }
    }else{/*//not the right block, go examine the next*/

    }
  }
  /*//if you got here you went through all blocks and none had
  //a matching tag; find lowest time and replace block*/
  struct block *lowest=NULL;
  for(bptr=sptr->b;bptr!=NULL;bptr=bptr->next){
    if(lowest==NULL){
      lowest=bptr;
    }else if(lowest->t>bptr->t){
      lowest=bptr;
    }
  }
  lowest->t=t;
  lowest->tag=tag;

  for(ptr=lowest->off;ptr!=NULL;ptr=ptr->next){
    ptr->data=-1;
  }
  ptr=lowest->off;
  for(i=0;i<bo;i++){
    ptr=ptr->next;
  }
  ptr->data=data;
  cm++;
  mr++;


}

void write(int data, int bo, int in, int tag, struct cache *c){
  int i;
  struct set *sptr=c->s;

  for(i=0;i<in;i++){/*//get to the right set*/
    sptr=sptr->next;
  }

  struct block *bptr=sptr->b;
  struct offset *ptr;
  for(bptr=sptr->b;bptr!=NULL;bptr=bptr->next){/*//check every block*/
    if(bptr->tag==-1){/*//empty block*/
      cm++;
      mr++;
      bptr->tag=tag;
      bptr->t=t;
      ptr=bptr->off;

      for(i=0;i<bo;i++){
        ptr=ptr->next;
      }

      ptr->data=data;
      return;
    }else if(bptr->tag==tag){/*//found the block*/
      ptr=bptr->off;
      for(i=0;i<bo;i++){
        ptr=ptr->next;
      }

      if(ptr->data==-1){/*//empty offset*/
        cm++;
        mr++;
        ptr->data=data;
        return;
      }else if(ptr->data==data){/*//hit*/
        ch++;
        return;
      }else{/*//something that isnt the data in the offset*/
        ptr=bptr->off;

        for(ptr=bptr->off;ptr!=NULL;ptr=ptr->next){
          if(ptr->data==data){/*//if the data is in a different offset*/
            ch++;
            return;
          }
        }
        /*//data is not in any offset so replace block?
        //replace block method*/
        bptr->t=t;
        bptr->tag=tag;
        for(ptr=bptr->off;ptr!=NULL;ptr=ptr->next){
          ptr->data=-1;
        }
        ptr=bptr->off;
        for(i=0;i<bo;i++){
          ptr=ptr->next;
        }
        ptr->data=data;
        cm++;
        mr++;
        return;
        /*//end of replace block method*/



      }
    }else{/*//not the right block, go examine the next*/

    }
  }
  /*//if you got here you went through all blocks and none had
  //a matching tag; find lowest time and replace block*/
  struct block *lowest=NULL;
  for(bptr=sptr->b;bptr!=NULL;bptr=bptr->next){
    if(lowest==NULL){
      lowest=bptr;
    }else if(lowest->t>bptr->t){
      lowest=bptr;
    }
  }
  lowest->t=t;
  lowest->tag=tag;

  for(ptr=lowest->off;ptr!=NULL;ptr=ptr->next){
    ptr->data=-1;
  }
  ptr=lowest->off;
  for(i=0;i<bo;i++){
    ptr=ptr->next;
  }
  ptr->data=data;
  cm++;
  mr++;


}

int get1s(int numofbits){
  int offsize=0;
  int i;
  for(i=0;i<numofbits;i++){
    offsize*=2;
    offsize+=1;

  }
  return offsize;
}

int getbits(int temp){

  int bits=0;
  while(temp>1){
    temp/=2;
    bits++;
  }
  return bits;
}

int main(int argc, char *argv[]){

  if(argc<=4){
    return 0;
  }

  int csize=atoi(argv[1]);
  int bsize=atoi(argv[3]);



  typedef enum {DIR, ASO, ASON} associativity;
  associativity assoc;
  int n=1;
  if(!strcmp(argv[2],"direct")){
    assoc=DIR;
  }else if(!strcmp(argv[2],"assoc")){
    assoc=ASO;
    n=csize/bsize;
  }else{
    assoc=ASON;
  }


  if(assoc==ASON){

  sscanf(argv[2],"assoc:%d",&n);
  }



  int numofsets=csize/(bsize*n);
  int blocksperset=n;
  if(assoc==ASO){
    numofsets=1;

  }

  FILE *trace= fopen(argv[4], "r");

  if(trace==NULL){
    return 0;
  }
  /*//set up cache*/
  struct cache *c=(struct cache *)malloc(sizeof(struct cache)*sizeof(struct set)*numofsets*sizeof(struct block)*blocksperset*sizeof(struct offset)*bsize);
  int i;

  /*//set up sets*/
  c->s=NULL;
  struct set *ptr;
  for(i=0;i<numofsets;i++){
    ptr=(struct set *)malloc(sizeof(struct set)*sizeof(struct block)*blocksperset*sizeof(struct offset)*bsize);
    ptr->b=NULL;
    ptr->next=c->s;
    c->s=ptr;
  }

  /*//set up blocks*/
  struct block *bp;
  for(ptr=c->s;ptr!=NULL;ptr=ptr->next){
    for(i=0;i<blocksperset;i++){
      bp=(struct block *)malloc(sizeof(struct block)*sizeof(struct offset)*bsize);
      bp->tag=-1;
      bp->off=NULL;
      bp->next=ptr->b;
      ptr->b=bp;
    }
  }

  /*//set up offset*/
struct offset *op;
for(ptr=c->s;ptr!=NULL;ptr=ptr->next){
  for(bp=ptr->b;bp!=NULL;bp=bp->next){
    for(i=0;i<bsize;i++){
      op=(struct offset *)malloc(sizeof(struct offset));
      op->data=-1;
      op->next=bp->off;
      bp->off=op;
    }
  }
}


  /*//test to see if mallocs worked right*/
  i=0;
  int j=0;
  int x=0;
for(ptr=c->s;ptr!=NULL;ptr=ptr->next){
  i++;
  j=0;
  for(bp=ptr->b;bp!=NULL;bp=bp->next){
    j++;
    x=0;
    for(op=bp->off;op!=NULL;op=op->next){
      x++;
    }

  }
}

int bbits=getbits(bsize);
int sbits=getbits(numofsets);


  int offsize=get1s(bbits);
  int indexoff=get1s(sbits);

  char rw;
  int data;
  int bo,in,tag;
  while(fscanf(trace,"%*x: %c %x",&rw,&data)!=NULL){

    bo=data & offsize;
    in=data>>bbits & indexoff;
    tag=data>>(bbits+sbits);

    if(rw=='R'){
      read(data,bo,in,tag,c);
    }

    if(rw=='W'){
      write(data,bo,in,tag,c);
      mw++;
    }

    t++;

  }

  printf("Memory reads: %d\nMemory writes: %d\nCache hits: %d\nCache misses: %d\n",mr,mw,ch,cm);

  return 0;
}
