#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct  lista_palabras{	//creamos una estructura para hacer una lista con cada palabra t su frecuencia parcial
	char string_p[30];
	int frec_palabra1;
	int frec_palabra2;
	int doc;
	struct lista_palabras *next;
} nodo_p;

typedef struct moldehead{
	char ini_letra;
	nodo_p *lista;
	struct moldehead *next_letra;
} nodo_i;

void Add_Node( nodo_p **head, char data[30], int doc)				//crear lista y agregar nodos
{
	nodo_p *new_node = malloc(sizeof(nodo_p));			//se reserva memoria en el heap
	for (int i = 0; i < 30; ++i)						
	{
		new_node->string_p[i] = data[i];				//se copia la palabra completa en el nodo
	}
	new_node->doc = doc;
	switch (doc){
		case 1:	new_node->frec_palabra1 = 1; break;
		case 2: new_node->frec_palabra2 = 1; break;
	}
	new_node->next = *head;								//asignamos el primer nodo que tenia la lista a next del que estamos añadiendo
	*head = new_node;									// asiganamos el puntero de la lista a nuestro nuevo nodo
}

void Add_Node_ini( nodo_i **head, char iniletra)				//crear lista y agregar nodos
{
	nodo_i *new_node = malloc(sizeof(nodo_i));			//se reserva memoria en el heap
	new_node->ini_letra = iniletra;
	new_node->lista = NULL;
	new_node->next_letra = *head;								//asignamos el primer nodo que tenia la lista a next del que estamos añadiendo
	*head = new_node;									// asiganamos el puntero de la lista a nuestro nuevo nodo
}

void Find_Frec_Word(nodo_i **head, char string_in[30], int doc){			//encontrar frecuencia de la palabra ingresada, retorna 0 si la palabra no estaba en la lista
	nodo_i *current_i = *head;
	nodo_p *current_p = NULL;
	int flag = 1;
	while(current_i != NULL && flag)				//mientras la inicial no este en la lista seguimos buscando
	{
		if (current_i->ini_letra != string_in[0])
		{
			current_i = current_i->next_letra;
		}
		else{
			flag = 0;
			current_p = current_i->lista;
		}
	}
	while( current_p != NULL && flag == 0){				//mientras la palabra no este en la lista seguimos buscando
		if (strncmp( string_in, (current_p->string_p), 30) != 0)
		{
			current_p = current_p->next;
		}
		else
		{
			switch (doc){
				case 1: current_p->frec_palabra1 += 1; break;
				case 2: current_p->frec_palabra2 += 1; break;
			}
			return;
		}
	}
	if (current_i == NULL)
	{
		Add_Node_ini( head, string_in[0]);
		current_i =  *head;
	}
	if (current_p == NULL)
	{
		Add_Node( &(current_i->lista), string_in, doc);
		return;
	}
}

int Free_List(nodo_i *head, FILE **nfp1, FILE **nfp2, FILE **nfp3, FILE **nfp4){			//Elimina la lista
	nodo_i *current_i = head;
	nodo_p *current = NULL;
	nodo_p *temp;
	nodo_i *temp_i;
	int contador = 0;

	while(current_i != NULL)
	{
		temp_i = current_i;
		current = current_i->lista;
		current_i = current_i->next_letra;
		while(current != NULL){				//mientras no lleguemos al final
			fprintf(*nfp3, "%s  %i\n", current->string_p,current->frec_palabra1 + current->frec_palabra2);
			if (current->frec_palabra1 != 0)
			{
				fprintf(*nfp1, "%s  %i\n", current->string_p,current->frec_palabra1);
			}
			if (current->frec_palabra2 != 0)
			{
				fprintf(*nfp2, "%s  %i\n", current->string_p,current->frec_palabra2);
			}
			if ((current->frec_palabra2 != 0) && (current->frec_palabra1 != 0))
			{
				fprintf(*nfp4, "%s\n", current->string_p);
			}
			temp = current;
			current = current->next;		//se va eliminando el primer nodo de la lista
			contador++;
			free(temp);
		}
		free(temp_i);
	}
	fclose(*nfp1);
	fclose(*nfp2);
	fclose(*nfp3);
	fclose(*nfp4);
	return contador;
}

void ReadFile(FILE **fp, nodo_i **pl, int doc){
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
			//printf("%s %s\n", palabra, palabra_f);
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

	nodo_i *pl = NULL;
	
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