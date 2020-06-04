#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


int main(){
    FILE *fp;
    int proc=0; 
    int j=0;
    int k=0;
    char nombre_archivo[50];

    printf("Escribe el nombre del archivo de prueba \n");
    fflush(stdin);
    gets(nombre_archivo);

    int num_procesos=0;
    int num_columna = num_columnas(fp, nombre_archivo);  
    int content[num_columna][3];


    fp = fopen(nombre_archivo, "r");

        for (j = 0; j < num_columna; j++){
        for(k=0;k< 3;k++)
        	fscanf(fp, "%d", &content[j][k]);
    }

    fclose(fp);

    for(j = 0; j < num_columna; j++){
    	if(content[j][0] == 0)
    		num_procesos++;
    }

    
    double size;
    int ps[num_procesos][3];

    for (j = 0; j < num_columna; j++){
    	if(content[j][0] == 0){
    		for(k = 0;k< 3;k++){
    			if(k<2){

                    ps[proc][k] = content[j][k+1];

                }
    			

    			if(k==2){
    				
    				size = (double)ps[proc][k-1];
    				ps[proc][k] = ceil(size/20);

    			}
    		}
    	proc++;
    	}
    }

    printf("\nTabla de procesos:\n");
    printf("Proceso\t\tTamaño\t\tNumero de Paginas \n");

    for (j = 0; j < num_procesos; j++){
		printf("\n");
    	for(k=0;k< 3;k++)
    		printf(" %d\t\t ", ps[j][k]);
	}
	printf("\n\n");

    int tabla_direcciones[num_columna-num_procesos][2];
    proc=0;

    printf("\nTabla de Direcciones:\n");

    for(j = 0; j< num_columna; j++){
        if(content[j][0]==0){

        	printf("\n\nProceso: %d \n",content[j][1]);
        	printf("Pág.\t\tDesp.\n");

        }

        if(content[j][0]==1){

            tabla_direcciones[proc][0]= content[j][1];
            for(k =1;k<=2;k++){

                tabla_direcciones[proc][j]= content[j][k];
                printf("%d\t\t",tabla_direcciones[proc][j]);

            }

            printf("\n");
            proc++;
        }  
    }

    int mp[5][4];
    int pag=0;
    int cont=0;
    int col = 0;
    int m = 5;
    

    for (j = 0; j < m; j++){
        mp[col][0]=col;

        if(ps[cont][2] > pag){

            mp[col][1] = ps[cont][0];
            mp[col][2] = pag;
            mp[col][3] = 0;
            col++;
        
    	}

    	cont++;
    	cont = cont % num_procesos;

    	if(cont ==0 && j>0){

        	pag++;

        }
    }


    int pasaProceso;
    int it=1;
    int ip=0;
    int x=1;
    int i=0;
    int desp=0;
    int holes=0;
    int y;
    int b=0;
    proc=0;
    j=0;

    while(j<num_columna){

    	proc=content[j][1];
    	if(content[j][0]==0){
    		j++;
    		pasaProceso=1;
    	}

    	if(pasaProceso==1){

	    	while((content[j+(it*4)-4][0] == 1) && (x+(it*4)-4) <= 4*it){

	    		ip=trae_Proceso(mp,proc,content[j+(it*4)-4][1]);

	    		if(ip==5){


                    for (i = 0; i < 5; ++i){

                        if(mp[i][1]==0){

                            holes=1;
                            break;

                        }
                    }

                    if (holes==0){

                        printf("Ha ocurrido un error, se realizarà un cambio de pagina a continuacion\n");

                    }

	    			for (i = 0; i < 4; ++i){

    					if(mp[i][3]<=mp[i+1][3]){

    						y=mp[i][3];

    					}else{

    						y=mp[i+1][3];

    					}

    				}
				    				
    				for (i = 0; i < 5; ++i){

    					if(mp[i][3]==y){

    						mp[i][1]=proc;
    						mp[i][2]=content[j+(it*4)-4][1];
    						mp[i][3]=1;
                            break;

    					}
    				}
	    		}

                if(content[j+(it*4)-4][1]==mp[ip][2]){

                    mp[ip][3]++;

                }

                if(content[j+(it*4)-4][2]>=20){

                    printf("Ocurrio un desborde, el proceso no fue aniadido\n");
                    b=1;
                    printf("Se procedio a eliminar el proceso %d\n",proc);

                    for (i = 0; i < 5; ++i){

                        if(mp[i][1]==content[j+(it*4)-4][2]){
                            mp[i][1]=0;
                            mp[i][2]=0;
                            mp[i][3]=0;
                            break;
                        }
                    }
                }

	    		x++;
                if(content[j+(it*4)-4][0]==0){
                    break;
                }
                memory_print(mp);
                if(b==0){
			
                    printf("El proceso %d se esta ejecutando...\n",proc);
		            printf("Direcciones: \n");
                    printf("Virtual: %d %d\n",content[j+(it*4)-4][1],content[j+(it*4)-4][2]);
                    printf("Real:  %d\n",(content[j+(it*4)-4][1]*mp[ip][0])
                    +content[j+(it*4)-4][2] );
		    printf("\n");
			
                }
                b=0;
	    	j++;
    		}
    	}
    	pasaProceso=0;
    	x=1;

    	if(content[j+(it*4)-4][0]==0){
            desp++;
    		printf("El proceso %d finalizo.\n",proc );
    		printf("Realizando liberacion de memoria...\n");
        
    		for (i = 0; i < 5; ++i){
    			if(mp[i][1]==proc){
            
    				mp[i][1]=0;
    				mp[i][2]=0;
    				mp[i][3]=0;
            
    			}
    		}
    	}
        if(desp==4){

            memory_print(mp);
            exit(0);

        }
    	
    	while(content[j][0]==1){

    		j++;

    		if(j==num_columna-1){

    			j=0;
    			it++;
    			i=0;

    		}
    	}
    }

    return 0;
    
    }    


int num_columnas(FILE *fp, char *nombre_archivo){
    fp = fopen(nombre_archivo, "r");
    int numchar = 0;
    int num_columna = 0;
    int i=0;


    if (fp == NULL){
        printf ("No se encontro el archivo %s\n", nombre_archivo);
        return -1;
    }

    while ((numchar = fgetc(fp)) != EOF){ 

        while(numchar != '\n' && numchar != EOF)
            numchar=fgetc(fp);

        num_columna++;
    }
    fclose(fp);
    return num_columna;
}


int trae_Proceso(int mp[5][4],int proc,int pag){

    int i;
	int res;
	
	for (i = 0; i < 5; ++i)
	{
		if((mp[i][1]==proc) && (mp[i][2]==pag)){
			res=i;
			break;
		}
		if(i==4){
			res=5;
		}
	}

	return res;
}

int memory_print(int mp[5][4]){

    printf("\n\nEn memoria tenemos:\n");
    printf("Marco, proceso, pagina y frecuencia de las paginas :");

    int i=0;
    int j=0;

    for (i = 0; i < 5; ++i){

        printf("\n");

        for (j=0; j<4; ++j){

            if (j == 3){

                printf("%d", mp[i][j]);

            }else{

                printf("%d, ",mp[i][j]);

            }
        }
    }
    printf("\n");
    return 0;
}
