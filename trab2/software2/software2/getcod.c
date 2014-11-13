#include <stdio.h>
#include <stdlib.h>
#include<string.h>


/*static void error (const char *msg, int line) {
  fprintf(stderr, "erro %s na linha %d\n", msg, line);
  exit(EXIT_FAILURE);
}

void checkVar(char var, int idx, int line) {
  switch (var) {
    case 'v':
      if ((idx < 0) || (idx > 5))
       error("operando invalido", line);
      break;
    case 'p':
      if ((idx < 0) || (idx > 5))
       error("operando invalido", line);
      break;
    default:
       error("operando invalido", line);
   }
}*/

typedef struct pendencias {
	int byte_ini;
	int linha;

}Pendencias;
typedef int (*funcp) ();


void retorno ( unsigned char * cod, int * pos, char varc, int idx)
{
	int i;
	char var;

	if(varc=='$')
	{
		cod[(*pos)]=0xb8;
		(*pos)++;

		for(i=0;i<4;i++)
		{
			cod[(*pos)] = (idx & 0xff);
			(*pos)++;
			idx>>=8;
		}
	}
	else
	{
		cod[(*pos)]=0x8b;
		(*pos)++;
		cod[(*pos)]=0x45;
		(*pos)++;

		if(varc=='v')
		{
			var=(char)(-(8+4*idx));
			cod[(*pos)]=var;
			(*pos)++;
		}
		else
		{
			var=(char)(8+4*idx);
			cod[(*pos)]=var;
			(*pos)++;
		}
	}

	cod[(*pos)] = 0x89;
	(*pos)++;
	cod[(*pos)] = 0xec;
	(*pos)++;
	cod[(*pos)] = 0x5d;
	(*pos)++;
	cod[(*pos)] = 0xc3;
	(*pos)++;
}

void atribuicao (unsigned char * cod, int * pos, char op, char var0, char varc1, char varc2, int idx0, int idx1, int idx2)
{
	int i;
	char var;

	// movl varc1, %ecx

	if(varc1=='$')
	{
		cod[(*pos)]=0xb9;
		(*pos)++;
		printf("idx1: %d\n",idx1);
		for(i=0;i<4;i++)
		{
			cod[(*pos)] = (idx1 & 0xff);
			(*pos)++;
			idx1>>=8;
		}
	}
	else
	{
		cod[(*pos)]=0x8b;
		(*pos)++;
		cod[(*pos)]=0x4d;
		(*pos)++;

		if(varc1=='v')
		{
			var=(char)(-(8+4*idx1));
			cod[(*pos)]=var;
			(*pos)++;
		}
		else
		{
			var=(char)(8+4*idx1);
			cod[(*pos)]=var;
			(*pos)++;
		}
	}

	// addl/imul/subl varc2, %ecx

	if(op=='+')
	{
		if(varc2=='$')
		{
			if((unsigned int)idx2<(16*16))
			{
				cod[(*pos)]=0x83;
				(*pos)++;
				cod[(*pos)]=0xc1;
				(*pos)++;
				cod[(*pos)]=(char)idx2;
				(*pos)++;
			}
			else
			{
				cod[(*pos)]=0x81;
				(*pos)++;
				cod[(*pos)]=0xc1;
				(*pos)++;

				for(i=0;i<4;i++)
				{
					cod[(*pos)] = (idx2 & 0xff);
					(*pos)++;
					idx2>>=8;
				}
			}

		}
		else
		{
			cod[(*pos)]=0x03;
			(*pos)++;
			cod[(*pos)]=0x4d;
			(*pos)++;

			if(varc2=='v')
			{
				var=(char)(-(8+4*idx2));
				cod[(*pos)]=var;
				(*pos)++;
			}
			else
			{
				var=(char)(8+4*idx2);
				cod[(*pos)]=var;
				(*pos)++;
			}
		}
	}
	else if(op=='*')
	{
		if(varc2=='$')
		{
			if((unsigned int)idx2<(16*16))
			{
				cod[(*pos)]=0x6b;
				(*pos)++;
				cod[(*pos)]=0xc9;
				(*pos)++;
				cod[(*pos)]=(char)idx2;
				(*pos)++;
			}
			else
			{
				cod[(*pos)]=0x69;
				(*pos)++;
				cod[(*pos)]=0xc9;
				(*pos)++;

				for(i=0;i<4;i++)
				{
					cod[(*pos)] = (idx2 & 0xff);
					(*pos)++;
					idx2>>=8;
				}
			}

		}
		else
		{
			cod[(*pos)]=0x0f;
			(*pos)++;
			cod[(*pos)]=0xaf;
			(*pos)++;
			cod[(*pos)]=0x4d;
			(*pos)++;

			if(varc2=='v')
			{
				var=(char)(-(8+4*idx2));
				cod[(*pos)]=var;
				(*pos)++;
			}
			else
			{
				var=(char)(8+4*idx2);
				cod[(*pos)]=var;
				(*pos)++;
			}
		}
	}
	else if(op=='-')
	{
		if(varc2=='$')
		{
			if((unsigned int)idx2<(16*16))
			{
				cod[(*pos)]=0x83;
				(*pos)++;
				cod[(*pos)]=0xe9;
				(*pos)++;
				cod[(*pos)]=(char)idx2;
				(*pos)++;
			}
			else
			{
				cod[(*pos)]=0x81;
				(*pos)++;
				cod[(*pos)]=0xe9;
				(*pos)++;

				for(i=0;i<4;i++)
				{
					cod[(*pos)] = (idx2 & 0xff);
					(*pos)++;
					idx2>>=8;
				}
			}

		}
		else
		{
			cod[(*pos)]=0x2b;
			(*pos)++;
			cod[(*pos)]=0x4d;
			(*pos)++;

			if(varc2=='v')
			{
				var=(char)(-(8+4*idx2));
				cod[(*pos)]=var;
				(*pos)++;
			}
			else
			{
				var=(char)(8+4*idx2);
				cod[(*pos)]=var;
				(*pos)++;
			}
		}
	}

	//movl %ecx, var0

	cod[(*pos)]=0x89;
	(*pos)++;
	cod[(*pos)]=0x4d;
	(*pos)++;

	if(var0=='v')
	{
		var=(char)(-(8+4*idx0));
		cod[(*pos)]=var;
		(*pos)++;
	}
	else
	{
		var=(char)(8+4*idx0);
		cod[(*pos)]=var;
		(*pos)++;
	}


}

void ifeq ( unsigned char * cod, int *pos, int * ini_linhas, int * pos_pend, Pendencias * pend[], int linha, char varc0, char varc1, int idx0, int idx1, int num)
{
	int i, pos_num;
	char var;

	//codigo cmp
	
	if(varc0=='$')
	{
		if(varc1=='$')
		{
			//ambos constantes
			//movl varc0,%ecx

			cod[(*pos)]=0xb9;
			(*pos)++;

			for(i=0;i<4;i++)
				{
					cod[(*pos)] = (idx0 & 0xff);
					(*pos)++;
					idx0>>=8;
				}

			//cmp varc1,%ecx

			if((unsigned int)idx0<(16*16))
			{
				cod[(*pos)]=0x83;
				(*pos)++;
				cod[(*pos)]=0xf9;
				(*pos)++;
				cod[(*pos)]=(char)idx0;
				(*pos)++;
			}
			else
			{
				cod[(*pos)]=0x81;
				(*pos)++;
				cod[(*pos)]=0xf9;
				(*pos)++;

				for(i=0;i<4;i++)
				{
					cod[(*pos)] = (idx0 & 0xff);
					(*pos)++;
					idx0>>=8;
				}
			}
			
		}
		else // varc1=='$'
		{
			if(varc1=='v')
			{
				// 0 constante 1 local
				//cmp varc0,-(8+4i)(%ebp)

				if( ((unsigned int)idx0) < (16*16) )
				{
					cod[(*pos)]=0x83;
					(*pos)++;
					cod[(*pos)]=0x7d;
					(*pos)++;
					printf("\nblabla\n");
					printf("%d\n",idx0);
					var=(char)(-(8+4*idx1));//------------------------------------------------------------------------------------------------------------
					printf("\nbla: %x\n",var);
					cod[(*pos)]=var;
					(*pos)++;

					cod[(*pos)]=(char)idx0;
					(*pos)++;
				}
				else
				{
					cod[(*pos)]=0x81;
					(*pos)++;
					cod[(*pos)]=0x7d;
					(*pos)++;

					var=(char)(-(8+4*idx0));
					cod[(*pos)]=var;
					(*pos)++;

					for(i=0;i<4;i++)
					{
						cod[(*pos)] = (idx0 & 0xff);
						(*pos)++;
						idx0>>=8;
					}
				}


			}
			else
			{
				// 0 constante 1 parametro
				//cmp varc0, (8+4i)(%ebp)

				if((unsigned int)idx0<(16*16))
				{
					cod[(*pos)]=0x83;
					(*pos)++;
					cod[(*pos)]=0x7d;
					(*pos)++;

					var=(char)(8+4*idx1);
					cod[(*pos)]=var;
					(*pos)++;

					cod[(*pos)]=(char)idx0;
					(*pos)++;
				}
				else
				{
					cod[(*pos)]=0x81;
					(*pos)++;
					cod[(*pos)]=0x7d;
					(*pos)++;

					var=(char)(8+4*idx1);
					cod[(*pos)]=var;
					(*pos)++;

					for(i=0;i<4;i++)
					{
						cod[(*pos)] = (idx0 & 0xff);
						(*pos)++;
						idx0>>=8;
					}
				}
			}
		}
	}
	else // varc0=='$'
	{
		if(varc0=='v')
		{
			if(varc1=='$')
			{
				// 0 local 1 constante
				//cmp varc1, -(8+4i)(%ebp)

				if((unsigned int)idx1<(16*16))
				{
					cod[(*pos)]=0x83;
					(*pos)++;
					cod[(*pos)]=0x7d;
					(*pos)++;

					var=(char)(-(8+4*idx0));
					cod[(*pos)]=var;
					(*pos)++;

					cod[(*pos)]=(char)idx1;
					(*pos)++;
				}
				else
				{
					cod[(*pos)]=0x81;
					(*pos)++;
					cod[(*pos)]=0x7d;
					(*pos)++;

					var=(char)(-(8+4*idx0));
					cod[(*pos)]=var;
					(*pos)++;

					for(i=0;i<4;i++)
					{
						cod[(*pos)] = (idx1 & 0xff);
						(*pos)++;
						idx1>>=8;
					}
				}
			}
			else // varc1=='$'
			{

				//movl varc0, %ecx
				//feito

				cod[(*pos)]=0x8b;
				(*pos)++;
				cod[(*pos)]=0x4d;
				(*pos)++;

				var=(char)(-(8+4*idx0));
				cod[(*pos)]=var;
				(*pos)++;

				cod[(*pos)]=0x39;
				(*pos)++;
				cod[(*pos)]=0x4d;
				(*pos)++;

				if(varc1=='v')
				{
					// 0 local 1 local
					//cmp %ecx, -(8+4i)(%ebp)

					var=(char)(-(8+4*idx1));
					cod[(*pos)]=var;
					(*pos)++;

				}
				else
				{
					// 0 local 1 parametro
					//cmp %ecx, (8+4i)(%ebp)


					var=(char)(8+4*idx1);
					cod[(*pos)]=var;
					(*pos)++;
				}
			}
		}
		else // (varc0=='v')
		{
			//varc0 parametro

			if(varc1=='$')
			{
				// 0 parametro 1 constante
				//cmp varc1,(8+4i)(%ebp)

				if((unsigned int)idx1<(16*16))
				{
					cod[(*pos)]=0x83;
					(*pos)++;
					cod[(*pos)]=0x7d;
					(*pos)++;

					var=(char)(8+4*idx0);
					cod[(*pos)]=var;
					(*pos)++;

					cod[(*pos)]=(char)idx1;
					(*pos)++;
				}
				else
				{
					cod[(*pos)]=0x81;
					(*pos)++;
					cod[(*pos)]=0x7d;
					(*pos)++;

					var=(char)(8+4*idx0);
					cod[(*pos)]=var;
					(*pos)++;

					for(i=0;i<4;i++)
					{
						cod[(*pos)] = (idx1 & 0xff);
						(*pos)++;
						idx1>>=8;
					}
				}
			}
			else
			{

				//movl, varc0,%ecx
				//feito

				cod[(*pos)]=0x8b;
				(*pos)++;
				cod[(*pos)]=0x4d;
				(*pos)++;

				var=(char)(8+4*idx0);
				cod[(*pos)]=var;
				(*pos)++;

				cod[(*pos)]=0x83;
				(*pos)++;
				cod[(*pos)]=0x7d;
				(*pos)++;

				cod[(*pos)]=0x39;
				(*pos)++;
				cod[(*pos)]=0x4d;
				(*pos)++;


				if(varc1=='v')
				{
					// 0 parametro 1 local
					//cmp %ecx, -(8+4i)(%ebp)

					var=(char)(-(8+4*idx1));
					cod[(*pos)]=var;
					(*pos)++;

				}
				else
				{
					// ambos parametro
					//cmp %ecx, (8+4i)(%ebp)

					var=(char)(8+4*idx1);
					cod[(*pos)]=var;
					(*pos)++;
				}
			}
		}
	}


	if(num <= linha)
	{
		 pos_num = ini_linhas[num-1] - (*pos + 2) ;
		 printf("\nini_linhas[%d] (%d) - (*pos(%d) + 2)\n",num,ini_linhas[num],*pos);

		 printf(" pos_num: %x \n" , pos_num ) ;

		 if(pos_num < 0xffffff80 )//quando vira 4 bytes
		 {
			
			 cod[(*pos)]=0x0f;
			 (*pos)++;
			 cod[(*pos)]=0x84;
			 (*pos)++;
			
			 pos_num-=4;
			 
			 for(i=0;i<4;i++)
			 {
				cod[(*pos)] = (pos_num & 0xff);
				(*pos)++;
				pos_num>>=8;
			 }
		 }
		 else // pos_num >= 0x80 || pos_num < 0xffffff80
		 {
			cod[(*pos)] = 0x74;
			(*pos)++;

			cod[(*pos)]=(unsigned char)pos_num;
			(*pos)++;
		 }
	}
	else // num <= linha
	{

		pend[*pos_pend]->linha=num;
		pend[*pos_pend]->byte_ini=*pos;

		(*pos)+=6; // pior caso: 0f 84 XX XX XX XX || melhor caso: 74 XX -----------------checar

		(*pos_pend)++;

	}
}

void preenche_pendencias (unsigned char * cod, int * ini_linhas, int pos_pend, Pendencias ** pend)
{
	int i,j,pos_num,diff_lin;

	for(i=0;i<pos_pend;i++)
	{
		
		pos_num = ini_linhas[(pend[i]->linha)-1] - (pend[i]->byte_ini + 2) ;

		 printf(" pos_num: %x \n" , pos_num ) ;

		 if(pos_num >= 0x80 || pos_num < 0xffffff80 )//quando vira 4 bytes
		 {
			 if(pos_num < 0)
			 {
				 pos_num-=4; //um byte a mais para "voltar"
			 }
			 cod[(pend[i]->byte_ini)]=0x0f;
			 cod[(pend[i]->byte_ini)+1]=0x84;
			 
			 for(i=0;i<4;i++)
			 {
				cod[ ( pend[i]->byte_ini ) + 2 + i] = (pos_num & 0xff);
				pos_num>>=8;
			 }
		 }
		 else // pos_num >= 0x80 || pos_num < 0xffffff80
		 {
			 cod[pend[i]->byte_ini] = 0x74;

			 cod[ (pend[i]->byte_ini) + 1 ]=(unsigned char)pos_num;
		
			 for(i=0;i<4;i++)
			 {
				cod[ ( pend[i]->byte_ini ) + 2 + i] = 0;
			 }
		 }
	}
}
funcp geracod(FILE * f, int * pos)
{
	unsigned char codigo[1000];
	int ini_linhas[50], posicao=0, pos_pend=0, linha=0;
	Pendencias * pend;
	char c;

	pend = (Pendencias*)malloc(50*sizeof(Pendencias));

	codigo[posicao]=0x55;
	posicao++;
	codigo[posicao]=0x89;
	posicao++;
	codigo[posicao]=0xe5;
	posicao++;
	codigo[posicao]=0x83;
	posicao++;
	codigo[posicao]=0xec;
	posicao++;
	codigo[posicao]=0x14;
	posicao++;

  while ((c = fgetc(f)) != EOF) {

	  printf("entrou no while\n");
	  ini_linhas[linha]=posicao;

    switch (c) {
      case 'r': {  /* retorno */
        int idx;
        char var;
        fscanf(f, "et %c%d", &var, &idx);
        
		retorno (codigo,&posicao,var,idx);

        break;
      }
		case 'v': case 'p': {  /* atribuicao */
        int idx0, idx1, idx2;
        char var0 = c, var1, var2;
        char op;
        fscanf(f, "%d := %c%d %c %c%d",
                   &idx0, &var1, &idx1,&op, &var2, &idx2);
        
        atribuicao(codigo, &posicao, op, var0, var1, var2, idx0, idx1, idx2);

        break;
      }
		case 'i': {  /* ifeq */
        int idx0, idx1, num;
        char var0, var1;

        fscanf(f, "feq %c%d %c%d %d", &var0, &idx0, &var1, &idx1, 
                                                &num);

        ifeq(codigo,&posicao,ini_linhas,&pos_pend, &pend, linha,var0,var1,idx0,idx1,num);

        break;
      }
	}

	linha++;
	(*pos) = posicao;

  }

	preenche_pendencias (codigo, ini_linhas, pos_pend, &pend);

	return (funcp) codigo;

}

  int main(void)
{
	funcp fun;
	int i,fim,vp[10],res;
	unsigned char * codigo;
	FILE * f = fopen("teste.txt","r");
	

	if(f==NULL)
	{
		printf("erro\n");
		return 1;
	}

	printf("vai gerar codigo\n");
	codigo = (unsigned char*)geracod(f,&fim);
	fun = geracod(f,&fim);
	
	for(i=0;i<fim;i++)
	{
		printf("%x \n",codigo[i]);
	}

	printf("%d",(*fun)());

	return 0;
}
