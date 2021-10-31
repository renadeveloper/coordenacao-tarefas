#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

sem_t sem_cadeiras;
sem_t sem_cad_monitor;
sem_t sem_aluno_atendido;
sem_t sem_aluno_cadeira;

// Funcao que a thread do monitor irá realizar
void* f_monitor(void *v) {

  while(1) {
    sem_wait(&sem_aluno_cadeira);
    printf("Monitor atendeu um estudante.\n");
    sem_post(&sem_aluno_atendido);
  }
  return NULL;
}

// Funcao que cada thread estudante irá realizar
void* f_estudante(void* v) {
  int id = *(int*) v;

  sleep(id%3);
  if (sem_trywait(&sem_cadeiras) == 0) {
    printf("Estudante %d entrou na sala de monitoria.\n", id);
    sem_wait(&sem_cad_monitor);
    printf("Estudante %d sentou na cadeira de atendimento.\n", id);
    sem_post(&sem_aluno_cadeira);
    sem_post(&sem_cadeiras);
    sem_wait(&sem_aluno_atendido);
    sem_post(&sem_cad_monitor);
    printf("Estudante %d deixou a sala de monitoria.\n", id);
  } else
    printf("Estudante %d nao entrou na sala de monitoria.\n", id);
  return NULL;
}

int main() {
  int qtdEstudantes, qtdCadeiras;
  
  printf("Insira o número de cadeiras de espera: ");
  scanf("%d", &qtdCadeiras);

  // Tratamento da entrada para a quantidade de cadeiras
  while (qtdCadeiras <= 2) {
      printf("Por favor, insira um número maior que 2 para a quantidade de cadeiras: ");
      scanf("%d", &qtdCadeiras);
  }

  printf("Insira a quantidade de alunos a ser atendida: ");
  scanf("%d", &qtdEstudantes);

  // Tratamento da entrada para a quantidade de estudantes
  while (qtdEstudantes <= 0) {
      printf("Por favor, insira um número maior que 0 para a quantidade de estudantes: ");
      scanf("%d", &qtdEstudantes);
  }

  // Criacao das variaveis de thread dos estudantes e do monitor
  pthread_t thr_estudantes[qtdEstudantes], thr_monitor;
  int i, id[qtdEstudantes];

  // Criacao dos semaforos
  sem_init(&sem_cadeiras, 0, qtdCadeiras);
  sem_init(&sem_cad_monitor, 0, 1);
  sem_init(&sem_aluno_cadeira, 0, 0);
  sem_init(&sem_aluno_atendido, 0, 0);
  
  // Criacao das threads dos estudantes
  for (i = 0; i < qtdEstudantes; i++) {
    id[i] = i;
    pthread_create(&thr_estudantes[i], NULL, f_estudante, (void*) &id[i]);
  }

  // Criacao da thread do monitor
  pthread_create(&thr_monitor, NULL, f_monitor, NULL);
  
  // join das threads dos estudantes
  for (i = 0; i < qtdEstudantes; i++) {
    pthread_join(thr_estudantes[i], NULL);
  }
  
  return 0;
}
