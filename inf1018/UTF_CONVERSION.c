#include<stdio.h>

void conv_little_endian(unsigned int * n) //modifiquei porque tenho que fazer tudo nessa baga�a
{
	unsigned int aux, saida, mask=0xFF;
	int i;

	saida=0x0;

	for(i=0;i<4;i++)
	{
		aux= mask & *n; //n � numero, � contador, mas ta sendo usado aqui como variavel porque clara ta com mania de assembly
		aux<<=(24-8*i);
		saida = saida | aux;
		(*n)>>=8;
	}

	*n=saida;

	return;
}

char GET_BOM (unsigned int byte_bom )
{
	if ( byte_bom == 0xFFFE0000 )
		return 'b';
	if ( byte_bom == 0xFEFF )
		return 'l';
	return -1;
}

int conv32_8 ( FILE *arq_entrada , FILE *arq_saida ){

	unsigned int buffer_entrada , aux , bytes=0;

	unsigned char bom , buffer_saida [ 4 ] = { 0 , 0x80 , 0x80 , 0x80 };

	int elements,escritos;
	int n , i ;



	if ( fread( &buffer_entrada , 4 , 1 , arq_entrada ) != 1 ) //arquivo n�o est� em bytes multiplos de 4 OU valor � maior que a representa��o permite
	{
		fprintf(stderr,"\nErro de entrada: Arquivo termina inesperadamente no byte %u\n",bytes);
		return -1;
	}
	//printf("\nBOM:%x\n",buffer_entrada);
	
	bom = GET_BOM ( buffer_entrada );
	
	if(bom == 'b')
	{
		//converter de little endian para big
		conv_little_endian ( &buffer_entrada );
		//printf("\nConversao:%x\n",buffer_entrada);
	}	
	
	if(bom==-1)
	{
		fprintf(stderr,"\nErro de entrada: caracter BOM em formato errado\n");
		return -1;
	}
	//printf("chegou aqui");
	do
	{		
		
		elements = fread ( &buffer_entrada , 4 , 1 , arq_entrada );

		bytes+=4;

		//printf("\nElemento:%x\n",buffer_entrada);
		if( elements == 0 )
			return 0;
		if(bom == 'b')
		{
			//converter de little endian para big
			conv_little_endian ( &buffer_entrada );
			//printf("\nConversao%x\n",buffer_entrada);
		}
		if ( buffer_entrada > 0x10FFFF ) 
		{
			fprintf(stderr,"\nErro de entrada: Algarismo n�o existe na representa��o UNICODE. Byte: %u\n",bytes);
			return -1;
		}
		if( elements != 1 )
		{
			fprintf(stderr,"\nErro de entrada: Arquivo corrompido, terminou em um estado inst�vel no byte %u\n",bytes);
			return -1;
		}
		
		
		n=1;

		aux = 0xFFFFFF80;

		if( aux & buffer_entrada )
		{
			n++;
			//printf("\n%d bytes\n",n);
			aux <<= 4 ;
			while ( aux & buffer_entrada )
			{
				n++ ;
				//printf("\n%d bytes\n",n);
				aux <<= 5 ; 
			}
		}
		aux = 0x80; // aux agora serve pra colocar os 1's no byte inicial
		buffer_saida[0] = 0;
		for(i=1;i<4;i++)
			buffer_saida[i]=0x80;
		for( i=0 ; i<n ; i++ )
		{
			if( n > 1 )
			{
				buffer_saida [ 0 ] = buffer_saida [ 0 ] | ( aux >> i ); //colocando os 1's caso use mais de 1 byte
				//printf("\nByte inicial saida:%x\n",buffer_saida[0]);
				buffer_saida[i] = (buffer_saida[i] | ( ( buffer_entrada >> ( 6 * (n-1-i) ) ) & 0x3F ) ); //mask com a parada
				//printf("\nOutros Bytes:%x\n",buffer_saida[i]);
			}
			else
			{
				buffer_saida[0] = buffer_entrada;
			}
			
			buffer_saida[i] = (buffer_saida[i] | ( ( buffer_entrada >> ( 6 * (n-1-i) ) ) & 0x3F ) ); //mask com a parada
			//printf("\nOutros Bytes:%x\n",buffer_saida[i]);
		}
		//printf("\nBuffer de saida pr� write%x%x%x%x\n",buffer_saida[0],buffer_saida[1],buffer_saida[2],buffer_saida[3]);

		escritos=fwrite ( buffer_saida , 1 , n , arq_saida ) ;
		if(escritos != n)
		{
			fprintf(stderr,"Erro de saida: Nao foi possivel escrever no arquivo");
		}
	}while( elements == 1 );
	return 0;
}
int main (void)
{
	FILE* entrada,*saida;
	saida=fopen("RESULTADO.txt","w");
	entrada=fopen("utf32_grande_big.txt","r");
	if(entrada==NULL)
	{
		printf("cao");
		return 1;
	}
	conv32_8(entrada,saida);
	return 0;
}
