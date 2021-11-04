#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

// Declaração de mutex para acesso ao buffer
    sem_t buffer1, buffer2;	

// Thread que os X irão realizar
void* insere(void *item) {
    int id = *(int*) item;
    while(1) {
        sem_wait(&buffer1);
        printf("Item foi inserido no Buffer %d\n", id);
        sem_post(&buffer2);
    }
    return NULL;
}

// Thread que os demais irão realizar
void* retira(void *item) {
    int id = *(int*) item;
    sleep(id%3);
    if (sem_trywait(&buffer1) == 0) {
        sem_wait(&buffer2);
        printf("Item foi removido no Buffer %d\n", id);
        sem_post(&buffer2);
    }
    return NULL;
}

int main () {

    int qtdx, qtdy, qtdz;
    int n1, n2;

    // Tratamento da entrada para a quantidade de threads dos tipos X, Y e Z

    printf("Insira o quantidade de threads do tipo X: ");
    scanf("%d", &qtdx);

    printf("Insira o quantidade de threads do tipo Y: ");
    scanf("%d", &qtdy);

    printf("Insira o quantidade de threads do tipo Z: ");
    scanf("%d", &qtdy);

    // Tratamento da entrada para a capacidade dos buffers 1 e 2

    printf("Insira o capacidade do buffer 1: ");
    scanf("%d", &n1);

    printf("Insira o capacidade do buffer 2: ");
    scanf("%d", &n2);		

    // Criacao das variaveis de thread dos tipos X, Y e Z
    pthread_t tipoX[qtdx], tipoY[qtdy], tipoZ[qtdz];

    // Criacao dos semaforos
    sem_init (&buffer1, 0, n1);
    sem_init (&buffer2, 0, n2);

    // cria threads do tipo X
    for (int i=0; i<qtdx; i++) {
        pthread_create (&tipoX[i], NULL, insere, (void*) &i);
    }
    
    // cria threads do tipo Y
    for (int i=0; i<qtdy; i++) {
        int idy[qtdy];
        idy[i] = i;
        pthread_create (&tipoY[i], NULL, retira, (void*) &i);
    }

    // cria threads do tipo Z
    for (int i=0; i<qtdz; i++) {
        int idz[qtdz];
        idz[i] = i;
        pthread_create (&tipoZ[i], NULL, retira, (void*) &idz[i]);
    }

    // join das threads dos 2 tipos
    for (int i = 0; i < qtdx; i++) {
        pthread_join(tipoX[i], NULL);
    }

    for (int i = 0; i < qtdx; i++) {
        pthread_join(tipoY[i], NULL);
    }

    for (int i = 0; i < qtdz; i++) {
        pthread_join(tipoZ[i], NULL);
    }

    pthread_exit (NULL) ;
    return 0;
}