#include<stdio.h>

int conv32_8 ( FILE *arq_entrada, FILE *arq_saida ){

	unsigned int buffer_entrada , aux;

	unsigned char bom , buffer_saida[4]={0,0x80,0x80,0x80};

	if ( fread( &buffer_entrada , 4 , 1 , arq_entrada ) != 4 || buffer_entrada > 0x10FFFF ) //arquivo não está em bytes multiplos de 4 OU valor é maior que a representação permite
	{
		//erro
		return -1;
	}
	//bom=gets_organization(); DESCOBRE SE É LITTLE/BIG

	while( fread ( &buffer_entrada , 1 , 4 , arq_entrada) == 4 )
	{
		int n=1,i;
		aux=0xFFFFFF80;
		if( aux & buffer_entrada )
		{
			n++;
			aux <<= 4 ;
			while ( aux & buffer_entrada )
			{
				n++ ;
				aux <<= 5 ; 
			}
		}
		aux = 0x80; // aux agora serve pra colocar os 1's no byte inicial
		for( i=0 ; i<n ; i++ )
		{
			if(n>1)	buffer_saida[0] = buffer_saida[0] | (aux>>i); //colocando os 1's caso use mais de 1 byte
			
			buffer_saida[i] = buffer_saida[i] | ( ( buffer_entrada >> ( 6 * (n-1-i) ) ) & 0x3F ); //mask com a parada			
		}
		for(i=0;i<n;i++)
		{
			//escrever buffer_saida[i] no arquivo
		}
		return 0;
	}
}