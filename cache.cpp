#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <dos.h>
#include <conio.h> 

//16����2������ȯ�Լ�����.
void Convert16to2(char *Hex_array);
double logB(double x, double base);
int Convert2to10(char input[33]);


typedef struct data_block//ĳ�����ִº��
{
 bool v;
 int c_tag;
}data_block;

float hit_ratio = 0;
float miss_ratio = 0;

int hit_count = 0;
int miss_count = 0;

int total_count =0;

/////////////////////////////�����Ͱ���

int inst_total_count =0;

float inst_hit_ratio = 0;
float inst_miss_ratio = 0;

int inst_hit_count = 0;
int inst_miss_count = 0;

////////////////////////////��ɾ��

FILE *fp;
char *Hex_array; // ���ϼ��о��16�����������
int operand; // (0�϶�read data, 1 write data)  <- data cache , 2 instruction fetch <- instruction cache
char binary[33];//16 -> 2�����κ�ȯ�������

int main()
{

	printf("---------------------------------------------------");
	printf("\n\n101899_������\n\n");
	printf("---------------------------------------------------\n\n");
 //  Ʈ���̽������о����

 if((fp=fopen("trace.din", "r")) == NULL) {
  printf("file open error");
  exit(1);
 }

 int cache_size = 0;
 int block_size = 0;

 printf("cache size (byte) : ");
 scanf("%d",&cache_size);
 printf("block size (byte) : ");
 scanf("%d",&block_size);


 float d = (cache_size/block_size);

 int index_size = logB(d, 2.0);//����2�ηα��Լ�

 int offset_size = logB(block_size,2.0);

 int tag_size = (32-index_size)-offset_size;

 char *char_tag = (char *)malloc(sizeof(char) * tag_size);//�޾ƿ�char��2�����±������Һ���
 char *char_index = (char *)malloc(sizeof(char) * index_size);//�޾ƿ�char��2�����±������Һ���


 Hex_array = (char *)malloc(sizeof(char) * 11);

 for(int i=0; i<tag_size+1; i++)//char_tag �迭�ʱ�ȭ
 {
  char_tag[i] = 0;

  if(i==tag_size+1)
   char_tag[i] = NULL;//������NULL
 }

 for(int i=0; i<index_size+1; i++)//char_index �迭�ʱ�ȭ
 {
  char_index[i] = 0;

  if(i==index_size+1)
   char_index[i] = NULL;//������NULL
 }

 data_block *D_block = (data_block *)malloc(sizeof(struct data_block) * (block_size+1));//�����ͺ��
 data_block *I_block = (data_block *)malloc(sizeof(struct data_block) * (block_size+1));//��ɾ���

 for(int i=0; i<block_size; i++)//c_tag �ʱ�ȭ
 {
  D_block[i].v = 0;//v=0�����ʱ�ȭ
  I_block[i].v = 0;
  D_block[i].c_tag=0;//������±׵�block[0].c_tag=0 ~ block[block_size].c_tag = block_size �ʱ�ȭ
  I_block[i].c_tag=0;
 }
 
 while(feof(fp)==false)  //���ϳ��������ڿ��ҷ�����.
 {
  fscanf(fp, "%d", &operand);
  fscanf(fp, "%s", Hex_array);
  Convert16to2(Hex_array);
 

  for(int i=0; i<tag_size; i++)//�޾ƿ¸޸𸮿������ڸ���������(tag_size)��Ʈ��ŭ�±׷�����
  {
   char_tag[i]=binary[i];
  }


  for(int i=tag_size-1; i<tag_size+index_size; i++)//�޾ƿ¸޸𸮿���tag_size-1��������tag_size+index_size��Ʈ��ŭ�ε���������
  {
   char_index[i-tag_size]=binary[i];
  }

  int tag = Convert2to10(char_tag);//��������10�����ιٲٴ��Լ�
  int index = Convert2to10(char_index);

  if(operand == 0 || operand == 1)  //data cahce ����
  {
   total_count++;

   if(D_block[index].c_tag == tag)//���ȣ���ε������̿�
   {
    hit_count++;//�־����ϱ�hit
   }
   else
   {
    D_block[index].c_tag = tag;
    miss_count++;//�������ϱ�miss
   }

  }

 if(operand == 2)    //inst cache ����
  {
   inst_total_count++;

   if(I_block[index].c_tag == tag)
   {
    inst_hit_count++;//�־����ϱ�hit
   }
   else
   {
    I_block[index].c_tag = tag;
    inst_miss_count++;//�������ϱ�miss
   }

  }

 }

 hit_ratio = ((float)hit_count/(float)total_count)*100;
 miss_ratio= ((float)miss_count/(float)total_count)*100;
 inst_hit_ratio = ((float)inst_hit_count/(float)inst_total_count)*100;
 inst_miss_ratio= ((float)inst_miss_count/(float)inst_total_count)*100;

 printf("\n\n///////////////data//////////////////////\n");
 
 printf("hit count : %d \n",hit_count);
 printf("miss count : %d \n\n",miss_count);

 printf("hit_ratio : %f \n",hit_ratio);
 printf("miss_ratio : %f \n\n\n",miss_ratio);


 printf("///////////////inst//////////////////////\n");

 printf("hit count : %d \n",inst_hit_count);
 printf("miss count : %d \n\n",inst_miss_count);

 printf("hit_ratio : %f \n",inst_hit_ratio);
 printf("miss_ratio : %f \n\n\n",inst_miss_ratio);

 return 0;
}



//16���� 2���� ��ȯ�Լ�.
void Convert16to2(char *Hex_array)
{
 int res=0;
 char temp;
 unsigned long Hex = strtoul(Hex_array, NULL, 16);

 for (int i = 0; i < 32; i++)
 {
  res = Hex & (0x01 << (31 - i));
  if(res == 0)
  {
   temp = '0';
   binary[i] = temp;
  }
  else
  {
   temp = '1';
   binary[i] = temp;
  }
 }

}

 

double logB(double x, double base)
{
 return log(x) / log(base);
}

 

 


int Convert2to10(char input[33])
{

 int len;                  
 long num;             
 long output;           
 bool flag;               

 while(true)           
 {
  len = 0;               
  num = 1;             
  output = 0;            
  flag = true;                            

  while(input[++len] != NULL);        


  for(int i = len - 1; i >=0; i--)                    
  {
   if(!(input[i] == '0' || input[i] == '1'))      
   {
    flag = false;                                          
    break;
   }
   if(input[i] == '1')
   {
    output += num;
   }

   num *= 2; 

  }


  if(flag)
  {
   return output;
  }

 }

}

