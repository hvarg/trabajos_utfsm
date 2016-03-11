#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>

/*** Macros. ***/
#define MAX_LINE    4096 // Largo maximo de las lineas en el archivo de entrada.
#define MY_SEED     time(NULL) // Semilla para random. Puede ser un entero.
/* readline() se utiliza en la funcion readfile() para leer una linea del
 * archivo de entrada y borrar el ultimo salto de linea, "\r\n" en este caso. */
#define readline()  fgets(line,MAX_LINE,input);line[strcspn(line,"\r\n")]='\0'
#define OUTPUT_FILE "results" //Nombre default del archivo de salida.

/*** Variables globales. ***/
/* Del archivo de configuracion: */
int nbus,         // Numero de buses.
    cbus,         // Capacidad de los buses.
    nest,         // Numero de estaciones.
    *cest = NULL, // Array con el numero de buses en las estaciones.
    npep,         // Numero de puntos de encuentro.
    tpep,         // Total de personas.
    *cpep = NULL, // Array con el numero de personas en los puntos de encuentro.
    nref,         // Numero de refugios.
    tref,         // Capacidad total de los refugios.
    *cref = NULL,      // Array con la capacidad de los refugios.
    *crefcpy   = NULL, // Copia del array anterior.
    **dest_pep = NULL, // Array 2D con dist entre estaciones y pts de encuentro.
    **dpep_ref = NULL; // Array 2D con dist entre pts de encuetro y refugios.
/* Del algoritmo implementado: */
struct route{           // Estructura de datos de la ruta para cada bus.
    int     time;       // Tiempo que ha gastado actualmente el bus.
    short   where;      // En que estacion o refugio se encuentra actualmente.
    bool    in_depot;   // Donde esta el bus: true=estacion, false=refugio.
    char    *rec;       // String con la ruta (recorrido).
};
struct route *tour = NULL;      // Array con las rutas. tour[id_bus]


/*** Funciones ***/
/* La siguiente funcion fue obtenida del comentario de Laurence Gonsalves en:
 *http://stackoverflow.com/questions/822323/how-to-generate-a-random-number-in-c
 * el 23/06/2015. */
/* Returns an integer in the range [0, n).
 * Uses rand(), and so is affected-by/affects the same seed. */
int randint(int n) {
    if ((n - 1) == RAND_MAX) {
        return rand();
    } else {
        long end = RAND_MAX / n; // truncate skew
        assert (end > 0L);
        end *= n;
        int r;
        while ((r = rand()) >= end);
        return r % n;
    }
}

/* Limpia la memoria. */
void cleanup(){
    int i = 0;
    free(cest);
    free(cpep);
    free(cref);
    for(i = 0; i < nest; free(dest_pep[i++]));
    free(dest_pep);
    for(i = 0; i < npep; free(dpep_ref[i++]));
    free(dpep_ref);
    if(tour != NULL)
        free(tour);
}

/* Imprime a stdout la memoria en el mismo formato que ingreso. */
void printmem(){
    int i, j;
    printf("%d: %d\n%d:", nbus, cbus, nest);
    for(i = 0; i < nest; printf(" %d", cest[i++]));
    printf("\n%d: %d:", npep, tpep);
    for(i = 0; i < npep; printf(" %d", cpep[i++]));
    printf("\n%d: %d:", nref, tref);
    for(i = 0; i < nref; printf(" %d", cref[i++]));
    printf("\n\n");
    for(i = 0; i < nest; i++){
        printf("%d: ",i+1);
        for(j = 0; j < npep; printf("%d ", dest_pep[i][j++]));
        printf("\n");
    }
    printf("\n");
    for(i = 0; i < npep; i++){
        printf("%d: ",i+1);
        for(j = 0; j < nref; printf("%d ", dpep_ref[i][j++]));
        printf("\n");
    }
}

/* Lee el archivo de entrada y carga los datos en memoria. 
 * char *filename: nombre del archivo a analizar.
 *
 * returns:  0 Archivo leido correctamente.
 *          -1 No se pudo leer el archivo de entrada.
 *          -2 Errores leyendo el archivo de entrada.   */
int readfile(const char *filename){
    FILE    *input;
    char    line[MAX_LINE];
    int     i = 0, j = 0, sum = 0,
            rcode = 0;      // Retorno sin errores.
    if( (input = fopen(filename, "r")) != NULL ){
        /* Sin comprobaciones intermedias. */
        readline();     // Numero y capacidad de buses.
        nbus = atoi(strtok(line, ": "));
        cbus = atoi(strtok(NULL, " "));
        readline();     // Numero y capacidad de los refugios.
        nest = atoi(strtok(line, ": "));
        cest = (int *) malloc(sizeof(int)*nest);
        for(i = 0; i < nest; cest[i++] = atoi(strtok(NULL, " ")));
        readline();     // Numero y capacidades de los pts de encuentro.
        npep = atoi(strtok(line, ": "));
        tpep = atoi(strtok(NULL, ": "));
        cpep = (int *) malloc(sizeof(int)*npep);
        for(i = 0; i < npep; cpep[i++] = atoi(strtok(NULL, " ")));
        readline();     // Numero y capacidades de los refugios.
        nref = atoi(strtok(line, ": "));
        tref = atoi(strtok(NULL, ": "));
        cref = (int *) malloc(sizeof(int)*nref);
        for(i = 0; i < nref; cref[i++] = atoi(strtok(NULL, " ")));
        /* Se crea una copia para el analisis final. */
        crefcpy = (int *) malloc(sizeof(int)*nref);
        memcpy(crefcpy, cref, sizeof(int)*nref);
        /* Asignando memoria para los array en 2D: */
        dest_pep = (int**) malloc(sizeof(int*)*nest); //dest_pep[est][pto enc]
        for(i = 0; i < nest; dest_pep[i++] = (int*) malloc(sizeof(int)*npep));
        dpep_ref = (int**) malloc(sizeof(int*)*npep); //dpep_ref[pto enc][ref]
        for(i = 0; i < npep; dpep_ref[i++] = (int*) malloc(sizeof(int)*nref));
        /* Leyendo arrays 2D desde el archivo: */
        readline(); // Linea en blanco.
        for(i = 0; i < nest; i++){
            readline();
            strtok(line,": ");      // Se descarta la estacion.
            for(j = 0; j < npep; dest_pep[i][j++] = atoi(strtok(NULL," ")));
        }
        readline(); // Linea en blanco.
        for(i = 0; i < npep; i++){
            readline();
            strtok(line,": ");      // Se descarta el pto de encuentro.
            for(j = 0; j < nref; dpep_ref[i][j++] = atoi(strtok(NULL," ")));
        }
        fclose(input);
        /* Comprobando consistencia de los datos: */
        for(i = 0; i < npep; sum += cpep[i++]);
        if(sum != tpep){
            fprintf(stderr,
                    "Los puntos de encuentro tienen datos incorrectos.\n");
            rcode = -2;
        }
        sum = 0;
        for(i = 0; i < nref; sum += cref[i++]);
        if(sum != tref){
            fprintf(stderr, "Los refugios tienen datos incorrectos.\n");
            rcode = -2;
        }
    } else{
        fprintf(stderr, 
                "No se puede leer el archivo de entrada \"%s\".\n", filename);
        rcode = -1;
    }
    return rcode;
}

/* Inicializa las estructuras de tour de cada bus. */
void data_init(){
    int i, j;
    tour = (struct route*) malloc(sizeof(struct route)*nbus);
    for(i = 0; i < nbus; i++){
        tour[i].time = 0;
        tour[i].in_depot = true;
        tour[i].rec = (char*) malloc(sizeof(char)*(tpep/cbus)*8); // Peor caso.
        for(j = 0; j < nest; j++){  // Asigna secuencialmente los buses a las
            if(cest[j] > 0){        // estaciones.
                tour[i].where = j;
                sprintf(tour[i].rec, "E%02d", j+1);
                cest[j] -= 1;
                break;
            }
        }
    }
}

void write_data(FILE *out){
    int i;
    fprintf(out, 
            "Bus | Time | Est,  Trip1,   Trip2,   Trip3,   Trip4,   Trip5\n");
    fprintf(out, 
            "----+------+--------------------------------------------------\n");
    for(i = 0; i < nbus; i++)
        fprintf(out, " %02d |  %02d  | %s\n", i+1, tour[i].time, tour[i].rec);
    fprintf(out,"\nRefugio:  |");
    for(i = 0; i < nref; fprintf(out, "|  %02d ", ++i));
    fprintf(out,"\nPersonas: |");
    for(i = 0; i < nref; fprintf(out, "| %03d ", crefcpy[i] - cref[i++]));
    fprintf(out,"\n");
}

/* Obtiene el bus con tiempo minimo, si hay un empate selecciona al azar. 
 *
 * returns: id del bus con tiempo minimo. */
int get_min_time(){
    bool draw = false;  // true si hay un empate.
    int i, j = 0,
        id = 0,         // ID del bus con menor tiempo actualmente.
        ncand,          // Nro de candidatos en caso de empate.
        *candidates,    // Candidatos en caso de empate.
        tmin;           // Tiempo minimo en caso de empate.
    for(i = 1; i < nbus; i++){
        if(tour[i].time < tour[id].time){   // Si es menor lo guarda.
            draw = false;
            id   = i;
        } else if(tour[i].time == tour[id].time){   // Si es igual verifica.
            if(!draw){
                draw  = true;
                ncand = 2;
                tmin  = tour[id].time;
            } else{
                ncand += 1;
            }
        }
    }
    if(draw){   // Si detecto un empate debe elegir al azar.
        candidates = (int *) malloc(sizeof(int)*ncand);
        for(i = 0; i < nbus; i++)
            if(tour[i].time == tmin)
                candidates[j++] = i;
        id = candidates[randint(ncand)];
    }
    return id;
}

/* Obtiene el punto de encuentro con gente mas cercano a cierta estacion, si hay
 * empate elige al azar.
 * int id_est: id estacion a la cual buscaremos el pto mas cercano.
 *
 * returns: id del punto de encuentro mas cercano a la estacion. */
int get_closest_pep_from_est(int id_est){
    bool draw = false;  // true si hay un empate.
    int i, j = 0,
        id_pep,         // ID del pto de encuentro mas cercano.
        ncand,          // Nro de candidatos en caso de empate.
        *candidates,    // Candidatos en caso de empate.
        dmin;           // Distancia minima en caso de empate.
    for(i = 0; i < npep; i++){  // busca id inicial con gente.
        if(cpep[i] > 0){
            id_pep = i;
            break;
        }
    }
    // Lo siguiente igual a get_min_time()
    for(i = id_pep + 1; i < npep; i++){
        if(cpep[i] > 0){
            if(dest_pep[id_est][i] < dest_pep[id_est][id_pep]){
                draw   = false;
                id_pep = i;
            } else if(dest_pep[id_est][i] == dest_pep[id_est][id_pep]){
                if(!draw){
                    draw  = true;
                    ncand = 2;
                    dmin  = dest_pep[id_est][i];
                } else{
                    ncand += 1;
                }
            }
        }
    }
    if(draw){   // Si detecto un empate debe elegir al azar.
        candidates = (int *) malloc(sizeof(int)*ncand);
        for(i = 0; i < npep; i++)
            if(dest_pep[id_est][i] == dmin && cpep[i] > 0)
                candidates[j++] = i;
        id_pep = candidates[randint(ncand)];
    }
    return id_pep;
}

/* Obtiene el punto de encuentro con gent mas cercano a cierto refugio, si hay
 * empate elgie al azar. Es igual a closest_pep_from_est, pero desde el refugio.
 * int id_ref: id refugio al cual buscaremos el pto mas cercano.
 *
 * returns: id del punto de encuentro mas cercano al refugio. */
int get_closest_pep_from_ref(int id_ref){
    bool draw = false;
    int i, j = 0,
        id_pep,
        ncand,
        *candidates,
        dmin;
    for(i = 0; i < npep; i++){
        if(cpep[i] > 0){
            id_pep = i;
            break;
        }
    }
    for(i = id_pep + 1; i < npep; i++){
        if(cpep[i] >0){
            if(dpep_ref[i][id_ref] < dpep_ref[id_pep][id_ref]){
                draw   = false;
                id_pep = i;
            } else if(dpep_ref[i][id_ref] == dpep_ref[id_pep][id_ref]){
                if(!draw){
                    draw  = true;
                    ncand = 2;
                    dmin  = dpep_ref[i][id_ref];
                } else{
                    ncand += 1;
                }
            }
        }
    }
    if(draw){   // Si detecto un empate debe elegir al azar.
        candidates = (int *) malloc(sizeof(int)*ncand);
        for(i = 0; i < npep; i++)
            if(dpep_ref[i][id_ref] == dmin && cpep[i] > 0)
                candidates[j++] = i;
        id_pep = candidates[randint(ncand)];
    }
    return id_pep;
}

/* Obtiene el pto de encuentor con gente mas cercano a cierto bus.
 * int id_bus: id del bus objetivo.
 *
 * returns: id del punto de encuentro mas cercano al bus. */
int get_closest_pep(int id_bus){
    if(tour[id_bus].in_depot)
        return get_closest_pep_from_est(tour[id_bus].where);
    else
        return get_closest_pep_from_ref(tour[id_bus].where);
}

/* Obtiene el refugio con capacidad mas cercano a cierto punto de encuentro.
 * Es el casi igual que get_closest_pep_from_ref pero desde el pto de encuentro
 * al refugio esta vez.
 * int id_pep: id del punto de encuentro.
 *
 * returns: id del refugio mas cercano. */
int get_closest_ref(int id_pep){
    bool draw = false;
    int i, j = 0,
        id_ref,
        ncand,
        *candidates,
        dmin;
    for(i = 0; i < nref; i++){
        if(cref[i] > 0){
            id_ref = i;
            break;
        }
    }
    for(i = id_ref + 1; i < nref; i++){
        if(cref[i] >0){
            if(dpep_ref[id_pep][i] < dpep_ref[id_pep][id_ref]){
                draw   = false;
                id_ref = i;
            } else if(dpep_ref[id_pep][i] == dpep_ref[id_pep][id_ref]){
                if(!draw){
                    draw  = true;
                    ncand = 2;
                    dmin  = dpep_ref[id_pep][i];
                } else{
                    ncand += 1;
                }
            }
        }
    }
    if(draw){   // Si detecto un empate debe elegir al azar.
        candidates = (int *) malloc(sizeof(int)*ncand);
        for(i = 0; i < nref; i++)
            if(dpep_ref[id_pep][i] == dmin && cref[i] > 0)
                candidates[j++] = i;
        id_ref = candidates[randint(ncand)];
    }
    return id_ref;
}

/* Metodo greeady de generacion de soluciones. 
 *
 * returns: numero de iteraciones. */
int greedy(){
    int min_time_bus,
        closest_pep,
        closest_ref,
        dist_travel,
        iter = 0;
    while(tpep != 0){
        iter++;
        dist_travel  = 0;
        min_time_bus = get_min_time();
        closest_pep  = get_closest_pep(min_time_bus);
        dist_travel += tour[min_time_bus].in_depot? 
            dest_pep[tour[min_time_bus].where][closest_pep]:
            dpep_ref[closest_pep][tour[min_time_bus].where];
        closest_ref  = get_closest_ref(closest_pep);
        dist_travel += dpep_ref[closest_pep][closest_ref];
        tour[min_time_bus].time    += dist_travel;
        tour[min_time_bus].where    = closest_ref;
        tour[min_time_bus].in_depot = false;
        cref[closest_ref] -= cbus;
        cpep[closest_pep] -= cbus;
        tpep -= cbus;
        sprintf(tour[min_time_bus].rec, "%s, (%02d,%02d)", 
                tour[min_time_bus].rec, closest_pep + 1, closest_ref + 1);
    }
    return iter;
}

int main(int argc, const char * args[]){
    int rcode = EXIT_SUCCESS;
    if(argc < 2){
        fprintf(stderr, "Se necesita un archivo de entrada.\n");
        return EXIT_FAILURE;
    }
    if(readfile(args[1]))
        rcode = EXIT_FAILURE;
    else{
        FILE *output_file = NULL;
        int  maxtime=0, iter, i;
        if(argc > 2)
            if( (output_file = fopen(args[2], "w")) == NULL)
                fprintf(stderr, 
                        "No se pudo escribir el archivo \"%s\".\n", args[2]);
        if(output_file == NULL)
            output_file = fopen(OUTPUT_FILE, "w");

        srand(MY_SEED);     // Semilla para el random.
        data_init();        // Inicializacion de los datos.
        iter = greedy();    // Creacion de soluciones
        write_data(output_file);
        for(i = 0; i < nbus; i++)
            if(maxtime < tour[i].time) maxtime = tour[i].time;
        printf("El tiempo total es de: %d\n", maxtime);
        printf("Obtenido en %d iteraciones.\n", iter);
        fprintf(output_file, 
                "\nLos buses que determinan la duracion de la evacuacion son:");
        for(i = 0; i < nbus; i++)
            if(tour[i].time == maxtime)
                fprintf(output_file, " Bus#%02d", i+1);
        fprintf(output_file, "\n");
        fclose(output_file);
    }
    cleanup();
    return rcode;
}

/* vim: set ts=4 sw=4 tw=80 : */
