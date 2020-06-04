
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int main(){
    FILE *fp;
    int j=0,k=0;
    int Proceso=0; //i es el numero de filas de entrada, k y l contadores
    int Entrada[Numero_Filas][3];  
    int Cantidad_Procesos=0;
	
    char nombre_archivo[50];
    fflush(stdin);
    printf("Ingresa el nombre del archivo\n");
    gets(nombre_archivo);
    int num_columnas = Numero_Filas_Funcion(Archivo);
    Archivo = fopen(nombre_archivo, "r");

        for (j = 0; j < Numero_Filas; j++){
        for(k=0;k< 3;k++)
        	fscanf(Archivo, "%d", &Entrada[j][k]);
    }

    printf("\n---------------ENTRADA-------------------------\n");
    printf("Registro\tProceso\t\tTamaño\n");


    for (j = 0; j < Numero_Filas; j++){
    	printf("\n");
        for(k=0;k< 3;k++)
        	printf(" %d\t\t", Entrada[j][k]);
    }
    printf(" \n\n ");

    fclose(Archivo);

    for(j = 0; j < Numero_Filas; j++){
    	if(Entrada[j][0] == 0)
    		Cantidad_Procesos++;//Todos los procesos empiezan en el registro 0 por ello cada que 
    	//se lea un 0 podemos decir que se trata de un  procesos nuevo.
    }

    int tabla_procesos[Cantidad_Procesos][3];
    double Tamano;

    for (j = 0; j < Numero_Filas; j++){
    	if(Entrada[j][0] == 0){
    		for(k = 0;k< 3;k++){
    			if(k<2)
    			tabla_procesos[Proceso][k] = Entrada[j][k+1];//Llenamos en la nueva tabla el numero de 
    		//proceso y ademas el tamaño del mismo.
    			if(k==2){
    				//Hariamos la cuenta para calcular el numero de paginas requerido para cada proceso
    				//y adjuntarlo en la tercera columna de la tabla nueva 
    				Tamano = (double)tabla_procesos[Proceso][k-1];//Lo declaramos como double para que se pueda
    				//dividir y no mande errores.
    				tabla_procesos[Proceso][k] = ceil(Tamano/20);//La funcion ceil nos da el techo del numero
    				//para asi siempre asignar la cantidad exacta o mas de tal forma que quepa el proceso
    			}
    		}
    	Proceso++;
    	}
    }

    printf("\n---------------TABLA PROCESOS-------------------------\n");
    printf("Proceso\t\tTamaño\t\tNumero Paginas (Calculado)\n");

    for (j = 0; j < Cantidad_Procesos; j++){
		printf("\n");
    	for(k=0;k< 3;k++)
    		printf(" %d\t\t ", tabla_procesos[j][k]);
	}
	printf("\n\n");

    int tabla_direcciones[Numero_Filas-Cantidad_Procesos][2];
    Proceso=0;

    printf("\n---------------TABLA DIRECCIONES POR PROCESO-------------------------\n");

    for(j = 0; j< Numero_Filas; j++){
        if(Entrada[j][0]==0){
        	printf("\n\n-----PROCESO %d------ \n",Entrada[j][1]);
        	printf("Pagina\t\tDesplazamiento\n");
        }

        if(Entrada[j][0]==1){
            tabla_direcciones[Proceso][0]= Entrada[j][1];
            for(k =1;k<=2;k++){
                tabla_direcciones[Proceso][j]= Entrada[j][k];
                printf("%d\t\t",tabla_direcciones[Proceso][j]);
            }
            printf("\n");
            Proceso++;
        }  
    }

    //Carga Inicial
    int pagina=0;
    int marco = 5;
    int fila = 0;
    int marco_pagina[5][4];
    int Contador=0;

    for (j = 0; j < marco; j++){
        marco_pagina[fila][0]=fila;//Definimos el numero de marco esto será estatico de 0 a 4

        if(tabla_procesos[Contador][2] > pagina){
        marco_pagina[fila][1] = tabla_procesos[Contador][0];
        marco_pagina[fila][2] = pagina;//la pagina que toque
        marco_pagina[fila][3] = 0;
        fila++;//Aumentamos el numero de fila
    	}

    	Contador++;
    	Contador = Contador % Cantidad_Procesos;
    	if(Contador ==0 && j>0)
        	pagina++;
    }

    //Inicial
    int cont4=1;
    int vuelta=1;
    int Indice_Proceso=0;
    int Acceso=1;
    Proceso=0;
    j=0;
    int i=0;
    int Terminados=0;
    int Menor;
    int Huecos_Presentes=0;
    int desbordamiento=0;

    //Frecuencias Iniciales
    while(j<Numero_Filas){

    	Proceso=Entrada[j][1];
    	if(Entrada[j][0]==0){
    		j++;
    		Acceso=1;
    	}

    	if(Acceso==1){

	    	while((Entrada[j+(vuelta*4)-4][0]==1) && (cont4+(vuelta*4)-4)<=4*vuelta){
	    		Indice_Proceso=Busqueda_Proceso(marco_pagina,Proceso,Entrada[j+(vuelta*4)-4][1]);
	    		if(Indice_Proceso==5){


                    for (i = 0; i < 5; ++i){
                        if(marco_pagina[i][1]==0){
                            Huecos_Presentes=1;
                            break;
                        }
                    }

                    if (Huecos_Presentes==0)
                    {
                        printf("Error de página, se procede al cambio.\n");
                    }

	    			for (i = 0; i < 4; ++i){
    					if(marco_pagina[i][3]<=marco_pagina[i+1][3]){
    						Menor=marco_pagina[i][3];
    					}
    					else{
    						Menor=marco_pagina[i+1][3];
    					}
    				}
				    				
    				for (i = 0; i < 5; ++i){
    					if(marco_pagina[i][3]==Menor){
    						marco_pagina[i][1]=Proceso;
    						marco_pagina[i][2]=Entrada[j+(vuelta*4)-4][1];
    						marco_pagina[i][3]=1;
                            break;
    					}
    				}
	    		}

                if(Entrada[j+(vuelta*4)-4][1]==marco_pagina[Indice_Proceso][2]){
                    marco_pagina[Indice_Proceso][3]++;//Aumentamos la frecuencia 
                }

                if(Entrada[j+(vuelta*4)-4][2]>=20){
                    printf("Error de desbordamiento, no se anexará el proceso.\n");
                    desbordamiento=1;
                    printf("Proceso eliminado: %d\n",Proceso);
                    for (i = 0; i < 5; ++i){
                        if(marco_pagina[i][1]==Entrada[j+(vuelta*4)-4][2]){
                            marco_pagina[i][1]=0;
                            marco_pagina[i][2]=0;
                            marco_pagina[i][3]=0;
                            break;
                        }
                    }
                }

	    		cont4++;
                if(Entrada[j+(vuelta*4)-4][0]==0){
                    break;
                }
                Impresion_Memoria(marco_pagina);
                if(desbordamiento==0){
                    printf("Proceso en ejecucion: %d\n",Proceso);
                    printf("Direccion Virtual= %d %d\n",Entrada[j+(vuelta*4)-4][1],Entrada[j+(vuelta*4)-4][2]);
                    printf("Direccion Real= %d\n\n",(Entrada[j+(vuelta*4)-4][1]*marco_pagina[Indice_Proceso][0])
                    +Entrada[j+(vuelta*4)-4][2] );
                }
                desbordamiento=0;
	    		j++;
    		}
    	}
    	Acceso=0;
    	cont4=1;

    	if(Entrada[j+(vuelta*4)-4][0]==0){
            Terminados++;
    		printf("Termino el proceso %d procedemos a liberar memoria\n",Proceso );
    		for (i = 0; i < 5; ++i){
    			if(marco_pagina[i][1]==Proceso){
    				marco_pagina[i][1]=0;
    				marco_pagina[i][2]=0;
    				marco_pagina[i][3]=0;
    			}
    		}
    	}
        if(Terminados==4){
            Impresion_Memoria(marco_pagina);
            exit(0);
        }
    	
    	while(Entrada[j][0]==1){
    		j++;
    		if(j==Numero_Filas-1){
    			j=0;
    			vuelta++;
    			i=0;
    		}
    	}
    }

    return 0;
    
    }    


int num_columnas(FILE *fp, char *nombre_archivo){
	
    int c = 0;
    int num_columna = 0;
    int i=0;
    fp = fopen(nombre_archivo, "r");
    


    if (Archivo == NULL){
        printf ("No se encotró el archivo con el nombre %s\n", nombre_archivo);
        return -1;
    }

    while ((c = fgetc(fp)) != EOF){

        while(Caracter != '\n' && Caracter != EOF)
            Caracter=fgetc(Archivo);

        Contador_Columna++;
    }
    fclose(Archivo);
    return Contador_Columna;
}

int Impresion_Memoria(int marco_pagina[5][4]){
	int l=0;
	int k=0;
    printf("\n---------------ESTADO DE MEMORIA-------------------------\n");
    printf("Marco\tProceso\tPagina\tFrecuencia\n");

    for (l = 0; l < 5; ++l)
    {
    	printf("\n");
    	for (k = 0; k < 4; ++k)
    	{
    		printf("%d\t",marco_pagina[l][k]);
    	}
    }

    printf("\n");

return 0;
}

int Busqueda_Proceso(int marco_pagina[5][4],int Proceso,int Pagina){

	int Regreso=0;
	int i;
	for (i = 0; i < 5; ++i)
	{
		if((marco_pagina[i][1]==Proceso) && (marco_pagina[i][2]==Pagina)){
			Regreso=i;
			break;
		}
		if(i==4){
			Regreso=5;
		}
	}

	return Regreso;
}
