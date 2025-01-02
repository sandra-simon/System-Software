#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

struct SYMTAB
{
	char label[10];
	char addr[10];
}ST[30];
struct OPTAB
{
	char opcode[10];
	char hexcode[10];
}OT[30];

char t1[20],t2[20],t3[20],t4[10],address[10],label[10],opcode[10],operand[10],length[10],size[10],a[10],ad[10],st_addr[10];
int o=-1,s=-1,i,j;

FILE *fp1,*fp2,*fp3,*fp4,*fp5,*fp6;

void read_optab()
{
	while(1)
	{
		o++;
		fscanf(fp3,"%s %s",OT[o].opcode,OT[o].hexcode);
		if(getc(fp3)==EOF)
		{
			break;
		}
	}
}
void read_symtab()
{
        while(1)
        {
		s++;
                fscanf(fp2,"%s %s",ST[s].label,ST[s].addr);
                if(getc(fp2)==EOF)
                {
                        break;
                }
        }
}

void read_line()
{
	strcpy(t1,"");
	strcpy(t2,"");
        strcpy(t3,"");
        strcpy(t4,"");

	fscanf(fp1,"%s",t1);
	if(getc(fp1)!='\n')
	{
		fscanf(fp1,"%s",t2);
		if(getc(fp1)!='\n')
		{
                	fscanf(fp1,"%s",t3);
                	if(getc(fp1)!='\n')
			{
				fscanf(fp1,"%s",t4);
			}
		}
	}

	if(strcmp(t1,"")!=0 && strcmp(t2,"")!=0 && strcmp(t3,"")!=0 && strcmp(t4,"")!=0)
	{
		strcpy(address,t1);
		strcpy(label,t2);
		strcpy(opcode,t3);
		strcpy(operand,t4);
	}
        else if(strcmp(t1,"")!=0 && strcmp(t2,"")!=0 && strcmp(t3,"")!=0 && strcmp(t4,"") == 0)
        {
                strcpy(address,t1);
                strcpy(label,"");
                strcpy(opcode,t2);
                strcpy(operand,t3);
        }
        else if(strcmp(t1,"")!=0 && strcmp(t2,"")!=0 && strcmp(t3,"")==0 && strcmp(t4,"")==0)
        {
		if(strcmp(t1,"END")==0)
		{
                	strcpy(address,"");
                	strcpy(label,"");
                	strcpy(opcode,t1);
                	strcpy(operand,t2);
		}
		else
		{
                        strcpy(address,t1);
                        strcpy(label,"");
                        strcpy(opcode,t2);
                        strcpy(operand,"");
		}
        }
}

int main()
{
	fp1=fopen("intermediate.txt","r");
	fp2=fopen("symtab.txt","r");
	fp3=fopen("optab.txt","r");
	fp4=fopen("length.txt","r");
	fp5=fopen("Output.txt","w");
	fp6=fopen("object_programme.txt","w");

	if(fp1==NULL||fp2==NULL||fp3==NULL||fp4==NULL||fp5==NULL||fp6==NULL)
	{
		printf("ERROR : File cannot be opened\n");
		return 1;
	}

	fscanf(fp4,"%s\t%s\t",length,size);
	read_optab();//imp
	read_symtab();//imp

	fscanf(fp1,"%s\t%s\t%s\t",label,opcode,operand);
	strcpy(st_addr,operand);

	if(strcmp(opcode,"START")==0)
	{
		fprintf(fp5,"%s\t%s\t%s\t%s\n",address,label,opcode,operand);
		fprintf(fp6,"H^%s^00%s^0000%s\n",label,operand,length);
		fprintf(fp6,"T^00%s^%s",operand,size);

		read_line();
	}
	while(strcmp(opcode,"END")!=0)
	{
		if(strcmp(opcode,"BYTE")==0)
		{
			if(operand[0]=='C')
			{
				fprintf(fp5,"%s\t%s\t%s\t%s\t",address,label,opcode,operand);
				for(i=2;operand[i]!='\'';i++)
				{
					sprintf(ad,"%x",operand[i]);
					fprintf(fp5,"%s",ad);
					fprintf(fp6,"^%s",ad);
				}
				fprintf(fp5,"\n");
			}
			else if(operand[0]=='X')
			{
				strncpy(ad,operand+2,strlen(operand)-3);
				fprintf(fp5,"%s\t%s\t%s\t%s\t%s\n",address,label,opcode,operand,ad);
				fprintf(fp6,"^%s",ad);
			}
		}
		else if(strcmp(opcode,"WORD")==0)
		{
			sprintf(a,"%06x",atoi(operand));
			fprintf(fp5,"%s\t%s\t%s\t%s\t%s\n",address,label,opcode,operand,a);
			fprintf(fp6,"^%s",a);
		}
		else if(strcmp(opcode,"RESW")==0 || strcmp(opcode,"RESB")==0)
		{
			fprintf(fp5,"%s\t%s\t%s\t%s\n",address,label,opcode,operand);//no object code
		}
		else//search optab
		{
			i=0;
			while(i<=o && strcmp(opcode,OT[i].opcode)!=0)
			{
				i++;//no change in o
			}
			if(i>o)
			{
				printf("ERROR : opcode %s not found in optab\n",opcode);//last step of searching
			}
			else
			{
				j=0;
				while(j<=s && strcmp(operand,ST[j].label)!=0)
				{
					j++;
				}
				if(j>s)
				{
					printf("ERROR : operand %s not found in symtab\n",operand);
				}
				else
				{
					fprintf(fp5,"%s\t%s\t%s\t%s\t%s%s\n",address,label,opcode,operand,OT[i].hexcode,ST[j].addr);
					fprintf(fp6,"^%s%s",OT[i].hexcode,ST[j].addr);
				}
			}
		}
		read_line();//inside while loop..bcs need the next line before the next loop
	}
	//when END is encountered
	fprintf(fp5,"%s\t%s\t%s\t%s\t%s\n","*",label,opcode,operand,"*");
	fprintf(fp6,"\nE^00%s",st_addr);

	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
	fclose(fp4);
	fclose(fp5);
	fclose(fp6);

	return 0;
}
