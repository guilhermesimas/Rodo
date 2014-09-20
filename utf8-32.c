#include <stdio.h>
#include <stdlib.h>

int conv8_32(FILE *arq_entrada, FILE *arq_saida, char ordem)
{
	int saida, aux;
	int num_byte =1;
	unsigned char byte_lido;
	int qtd_bytes=1;
	int i;
	
	while(/*houver algo no arquivo*/0)
	{


	// le byte

	while(0x8000 & byte_lido)
	{
		qtd_bytes++;

		byte_lido<<=1;
	}

	if(qtd_bytes==1)
		saida=(0x0 | byte_lido);

	else
	{
		aux=byte_lido;

		aux<<=(4*(qtd_bytes-2));

		saida=(0x0 | aux);

		for(i=0;i<qtd_bytes;i++)
		{
			// le proximo byte

			//checa se byte esta correto

			if(!(0x8000 & byte_lido) || (0x4000 & byte_lido) ) // verifica se o primeiro byte é 1 e se o segundo é 0
			{
			  printf("Erro no byte %d",num_byte);
			  return -1;
			}

			aux=byte_lido;

			aux=(aux & 0x7FFF);  // zera primeiro byte

			aux<<=(6*(qtd_bytes-2-i)); // shifta até a posição correta

			saida=(saida | aux);
		}
	}

		
		//imprime byte, se necessário, converter de big pra little

		num_byte++;

	}
}