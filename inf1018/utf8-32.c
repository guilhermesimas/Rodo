#include <stdio.h>
#include <stdlib.h>

unsigned int conv_litte_endian(unsigned int n)
{
	unsigned int aux, saida, mask=0xFF;
	int i;

	saida=0;

	for(i=0;i<4;i++)
	{
		aux= mask & n;
		aux<<=(24-8*i);
		saida= saida | aux;
		n>>=8;
	}

	return saida;
}

int conv8_32(FILE *arq_entrada, FILE *arq_saida, char ordem)
{
	unsigned int saida, aux;
	int num_byte =1;
	unsigned char byte_lido;
	int qtd_bytes=1;
	int i;
	
	while(fread(&byte_lido,1,1,arq_entrada)==1)
	{


		while(0x8000 & byte_lido) // verifica quantos bytes serão necessários
		{
			qtd_bytes++;

			byte_lido<<=1;
		}

		if(qtd_bytes>4) 
		{
			printf("Erro no byte %d",num_byte);
			return -1;
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
				
				fread(&byte_lido,1,1,arq_entrada);  // le proximo byte


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

		
		if(ordem=='l')//se necessário, converter de big pra little
		{
			saida=conv_litte_endian(saida);
		}


		fwrite(&saida,4,1,arq_saida); //imprime

		num_byte++;

	}

	return 0;
}