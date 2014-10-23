#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct  lista_palabras{	//creamos una estructura para hacer una lista con cada palabra t su frecuencia parcial
	char string_p[30];
	int frec_palabra;
	int doc;
	struct lista_palabras *next;
} nodo_p;

void Add_Node( nodo_p **head, char data[30], int doc)				//crear lista y agregar nodos
{
	nodo_p *new_node = malloc(sizeof(nodo_p));			//se reserva memoria en el heap
	for (int i = 0; i < 30; ++i)						
	{
		new_node->string_p[i] = data[i];				//se copia la palabra completa en el nodo
	}
	new_node->doc = doc;
	new_node->next = *head;								//asignamos el primer nodo que tenia la lista a next del que estamos añadiendo
	*head = new_node;									// asiganamos el puntero de la lista a nuestro nuevo nodo
}

void Find_Frec_Word(nodo_p **head, char string_in[30], int doc){			//encontrar frecuencia de la palabra ingresada, retorna 0 si la palabra no estaba en la lista
	nodo_p *current = *head;
	int frec_current = 0;									// para retornar la frecuencia de la palabra

	while( current != NULL ){	//mientras la palabra no este en la lista seguimos buscando
		if (strncmp( string_in, (current->string_p), 30) != 0)
		{
			current = current->next;
		}
		else
		{
			current->frec_palabra += 1;
			if (current->doc != doc){
				current->doc = 3;
			}
			frec_current = current->frec_palabra;				//vemos la frecuencia de la palabra
			return;
		}
	}
	if (current == NULL)
	{
		Add_Node( head , string_in, doc);
		current = *head;
		current->frec_palabra = 1;
		return;
	}
}

int Free_List(nodo_p *head, FILE **nfp1, FILE **nfp2, FILE **nfp3, FILE **nfp4){			//Elimina la lista
	nodo_p *current = head;
	nodo_p *temp;
	int contador = 0;
	int frec = 0;

	while(current != NULL){				//mientras no lleguemos al final
		fprintf(*nfp3, "%s  %i\n", current->string_p,current->frec_palabra);
		if (current->doc != 2)
		{
			fprintf(*nfp1, "%s  %i\n", current->string_p,current->frec_palabra);
		}
		if (current->doc != 1)
		{
			fprintf(*nfp2, "%s  %i\n", current->string_p,current->frec_palabra);
		}
		if (current->doc == 3)
		{
			fprintf(*nfp4, "%s\n", current->string_p);
		}
		frec += current->frec_palabra;
		temp = current;
		current = current->next;		//se va eliminando el primer nodo de la lista
		contador++;
		free(temp);
	}

	fclose(*nfp1);
	fclose(*nfp2);
	fclose(*nfp3);
	fclose(*nfp4);
	return contador;
}

void ReadFile(FILE **fp, nodo_p **pl, int doc){
	int pointer = 0;
	char palabra_f[30] = " ";
	char palabra[30] = " ";
	int frec_temp = 0;

	while (!feof(*fp))			//mientras no estemos al final del archivo
	{
		for (int i = 0; i < 30; ++i)
		{
			palabra[i] = ' ';
			palabra_f[i] = ' ';
		}
		fscanf(*fp,"%30s", palabra);
		pointer = 0;
		for(int i = 0; i < 30; i++)
		{
		  palabra[i] = tolower(palabra[i]);			//pasar todas las letras a minusculas!
		  if( (palabra[i] > 96 && palabra[i] < 123))		//si no estamos en el rango de letras minusculas
		  {
		  	palabra_f[pointer] = palabra[i];								//borramos los caracteres diferentes a letra minuscula o numero
	  		pointer++;										//contdor de signos borrados en la palabra
		  }
		}
		if (pointer!=0)
		{
			Find_Frec_Word(pl, palabra_f, doc);		//nos devuelve la frecuencia de la palabra, si no estaba la agrega
		}
	}
	fclose(*fp);
	return;
}

int main(void)
{
	FILE *fp;
	FILE *fp2;

	FILE *fp_t1;	//file pointer del texto1
	FILE *fp_t2;	//file pointer del texto2
	FILE *fp_t3;	//file pointer del texto3
	FILE *fp_t4;	//file pointer del texto4

	int cont = 0;
	nodo_p *pl = NULL;

	fp = fopen("texto1.txt", "r");				//abrimos los archivos de texto
	fp2 = fopen("texto2.txt","r");

	fp_t1 = fopen("frec_texto1.txt","w");		//creamos un nuevo archivo donde escribiremos la frecuenia de cada palabra
	fp_t2 = fopen("frec_texto2.txt","w");
	fp_t3 = fopen("frec_textoscombinados.txt","w");
	fp_t4 = fopen("palabras_repetidas.txt","w");

	if (fp == NULL)								//verificamos si el archivo se abrio correctamente
	{
		printf("Error al abrir archivo\n");
	}
	else
	{ 	
		ReadFile( &fp, &pl, 1);
		ReadFile( &fp2, &pl, 2);
	}
	cont = Free_List( pl, &fp_t1, &fp_t2, &fp_t3, &fp_t4);
	printf("%i\n", cont);
	return 0;
}