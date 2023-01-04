#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define nom_max 15
#define idade_max 3
#define tele_max 14

// Organização dos Dados de Cada Elemento da Lista
#define AGE (sizeof(char) * nom_max) //age vai ser o tamanho do nome
#define NUMB (AGE + (sizeof(char) * idade_max)) //numb vai ser o age, ja declarado ali atras mais a idade max
#define ANTERIOR (NUMB + (sizeof(char) * tele_max)) //assim ele vai adicionando um a um
#define NEXT (ANTERIOR + sizeof(void **))
// espaço ocupado por cada elemento da lista
#define SIZE_PESSOA (NEXT + sizeof(void **))

//dividimos em 2, uma parte para os dados e outro para a organuzação dos dados
// Organizacao dos dados auxiliares, tres primeiras, bem como numero de itens da lista, seu inicio e final
#define VARIAVEL_AUX (sizeof(void **))
#define NAME (VARIAVEL_AUX + sizeof(int))
#define NUM_ITENS (NAME + (sizeof(char) * nom_max))
#define LAST (NUM_ITENS + sizeof(int)) 
#define FIRST (LAST + sizeof(void **))
// espaço ocupado por pbuffer
#define SIZE_PBUFFER (FIRST + sizeof(void **))


int menu(void *pBuffer){ //criamos o menu
    *(int *)(pBuffer + VARIAVEL_AUX) = 0;

    while (*(int *)(pBuffer + VARIAVEL_AUX) < 1 || *(int *)(pBuffer + VARIAVEL_AUX) > 5){
        printf("\n1- Incluir\n2- Buscar\n3- Listar\n4- Apagar\n5- Sair\n\nEscolha uma opcao: ");
        scanf("%d", &*(int *)(pBuffer + VARIAVEL_AUX));
        getchar();
    }

    return *(int *)(pBuffer + VARIAVEL_AUX);
}

void adicionarPessoa(void *pBuffer){

  void *tracer, *ppBuffer; // o ppbuffer foi criado para poder tratar separadamente as variaveis auxiliares
  //que manipulam os ponteiros da lista
    void *newpe = (void *)malloc(SIZE_PESSOA);//newp onde vou alocar as pessoas, por isso uso o malloc e o tamanho sizepessoas

    if (!newpe){// aqui ele testa para ver se da problema na memoria
        printf("\nProblema ao alocar memória para a nova pessoa\n");
        return;
    }

    printf("\ncoloque o nome: ");
    scanf("%14[^\n]%*c", (char *)newpe);
    printf("coloque a idade: ");
    scanf("%s", (char *)(newpe + AGE));
    getchar();
    printf("coloque o telefone: ");
    scanf("%s", (char *)(newpe + NUMB));
    getchar();

    *(void **)(newpe + ANTERIOR) = NULL; //testa pra ver se o anterior é null (para ver se é o primeiro)
    *(void **)(newpe + NEXT) = NULL;//testa pra ver se é o ultimo

    if (*(int *)(pBuffer + NUM_ITENS) == 0){ //testamos para ver se a lista ta vazia
        *(void **)(pBuffer + LAST) = newpe;//se tiver newpe vai ser o primeiro e o ultimo
        *(void **)(pBuffer + FIRST) = newpe;
        *(int *)(pBuffer + NUM_ITENS) += 1;// soma mais um em num_itens da lista

        return;
    }

    ppBuffer = *(void **)(pBuffer + FIRST);//vai apontar pro primeiro da lista


    do{
        if (strcmp((char *)newpe, (char *)ppBuffer) < 0){ //se a string que entrou menor que string que estamos na  lista entramos no if,agora q a gente coloca em ordem alfabetica, com o strcmp

            *(void **)(newpe + ANTERIOR) = *(void **)(ppBuffer + ANTERIOR); //ai colocamos o anterior de ppbuffer apontando para o newpe
            *(void **)(newpe + NEXT) = ppBuffer; //e o next do newpe vai ser o que esta ppbuffer

            if (*(int *)(pBuffer + NUM_ITENS) > 1 && *(void **)(ppBuffer + ANTERIOR) != NULL){ //depois testamos se a lista esta vazia ou nao
                tracer = *(void **)(ppBuffer + ANTERIOR); // se a lista nao estiver vazia, tracer vai ter o conteudo de anterior do ppbuffer
                *(void **)(tracer + NEXT) = newpe; // e newpe  vai ser o ????????????????
            }
            *(void **)(ppBuffer + ANTERIOR) = newpe;

            if (*(void **)(newpe + ANTERIOR) == NULL){//se o anterior de newpe for null,
                *(void **)(pBuffer + FIRST) = newpe; //newpe vai ser o primeiro da lista
            }

            *(int *)(pBuffer + NUM_ITENS) += 1;// adicionamos 1 ao numero de itens da nossa lista
            return;
        }

        if (*(void **)(ppBuffer + NEXT) == NULL){ //se o proximo de ppbuffer for igual a null, termina o programa, pois sera o ultimo da lista
            break;
        }
        ppBuffer = *(void **)(ppBuffer + NEXT);
    } while (ppBuffer != NULL);//enquanto ppbuffer ppbuffer for diferente de nulo

    *(void **)(newpe + ANTERIOR) = ppBuffer; //newpe+anterior vai ser ppbuffer
    *(void **)(ppBuffer + NEXT) = newpe; //ppbuffer+next vai apontar pra newpe
    *(void **)(pBuffer + LAST) = newpe; //e pbuffer+last vai apontar pra newpe
    //pq vai ser o unico elemento da lista

    *(int *)(pBuffer + NUM_ITENS) += 1; //vamos acrescentar 1 a numero de itens
    return;
}

void listarTodas(void *pBuffer)
{
  void *ppBuffer;
  if (*(int *)(pBuffer + NUM_ITENS) == 0){//nessa função se numero de itens for igual a 0 é pq n tem ng na lista
        printf("\nNinguém foi cadastrado ainda.\n");
        return;
    }

  //  pBuffer = *(void **)(pBuffer + FIRST);
    ppBuffer = *(void **)(pBuffer + FIRST); //vamos colocar o conteudo de pbuffer firs em ppbuffer




    do{
        printf("\n Nome: %s \nIdade: %s\n Telefone: %s\n",
               (char *)ppBuffer, (char *)(ppBuffer + AGE), (char *)(ppBuffer + NUMB)); //vamos printar os nomes 
        ppBuffer = *(void **)(ppBuffer + NEXT);
    } while (ppBuffer != NULL);//ate ppbuffer for diferente de null

    return;
}

void buscarPessoa(void *pBuffer)
{
    if (*(int *)(pBuffer + NUM_ITENS) == 0){ 
        printf("\nNinguém foi cadastrado ainda.\n");
        return;
    }

    printf("\nInforme o nome que deseja buscar: ");
    scanf("%14[^\n]%*c", (char *)(pBuffer + NAME));

    void *aux = (pBuffer + NAME);

    pBuffer = *(void **)(pBuffer + FIRST);

    while (pBuffer != NULL){
        if (strcmp((char *)aux, pBuffer) == 0){
            printf("\nNome: %s\nIdade: %s\nTelefone: %s\n",
                   (char *)pBuffer, (char *)(pBuffer + AGE), (char *)(pBuffer + NUMB));
            return;
        } else{
            pBuffer = *(void **)(pBuffer + NEXT);
        }
    }
    printf("\nNome nao encontrado.\n");

    return;
}

void apagarIndividuo(void *pBuffer){

    void *aux;
    void *ppBuffer;

    if (*(int *)(pBuffer + NUM_ITENS) == 0){
        printf("\nNinguém foi cadastrado ainda.\n");
        return;
    }

    printf("\nIndivíduo a ser removido: ");
    scanf("%14[^\n]%*c", (char *)(pBuffer + NAME));
    
    void *nome = (char *)(pBuffer + NAME); //ponteiro nome vai apontar pra quem foi colocado p ser removido

    ppBuffer = *(void **)(pBuffer + FIRST); //ppbuffer vai apontar pro primeiro da lista

    do{
        if (strcmp((char *)ppBuffer, (char *)nome) == 0){//se ele comparar e for igual, ou seja ele achar a palavra, vamos iniciar os testes
            if (*(void **)(ppBuffer + ANTERIOR) != NULL){//agora ele testa se o anterior for diferente de null(ou seja nao e o primeiro da lista)
                aux = *(void **)(ppBuffer + ANTERIOR);//a aux guarda o anterior de ppbuffer 
                *(void **)(aux + NEXT) = *(void **)(ppBuffer + NEXT);//depois a auxiliar vai apontar para o proximo(que era o prox do q apagamos)
            } else{//se for o primeiro da lista, vai apagar ele e colocar o proximo como primeiro
                *(void **)(pBuffer + FIRST) = *(void **)(ppBuffer + FIRST);
            }
            if (*(void **)(ppBuffer + NEXT) != NULL){// se nao for o ultimo da lista
                aux = *(void **)(ppBuffer + NEXT);
                *(void **)(aux + ANTERIOR) = *(void **)(ppBuffer + ANTERIOR);//a aux vai fazer o anterior de ppbuffer apontar para o prox de ppbuffer
            } else{//se for o ultimo
                *(void **)(pBuffer + LAST) = *(void **)(ppBuffer + ANTERIOR);// ele so vai excluir 
            }


            *(int *)(pBuffer + NUM_ITENS) =  *(int *)(pBuffer + NUM_ITENS) - 1; //vai decrementar 1
	    pBuffer = ppBuffer; 
	    free(pBuffer);

            printf("\nIndivíduo removido com sucesso.\n");
            return;
        }
        ppBuffer = *(void **)(ppBuffer + NEXT);
    } while (ppBuffer != NULL);

    printf("\nNome não encontrado.\n");
    free(nome);
   }

void limparLista(void *pBuffer)
{
  void *aux, *ppBuffer;
    ppBuffer = *(void **)(pBuffer + FIRST);

    while (ppBuffer != NULL){
        aux = *(void **)(ppBuffer + NEXT);
        free(ppBuffer);
        ppBuffer = aux;
    }
    free(aux);
    
    printf("\nLista limpa com sucesso.\n");

    return;
}

int main(){

    void *pBuffer = (void *)calloc(1, SIZE_PBUFFER);

    if (!pBuffer){
        printf("\nErro ao alocar memória para pBuffer.\n");
        return (-1);
    }

    *(int *)(pBuffer + NUM_ITENS) = 0;
    *(void **)(pBuffer) = NULL;
    *(void **)(pBuffer + FIRST) = NULL;
    *(void **)(pBuffer + LAST) = NULL;

    for (;;){
        *(int *)(pBuffer + VARIAVEL_AUX) = menu(pBuffer);

        switch (*(int *)(pBuffer + VARIAVEL_AUX)){
        case 1:
            adicionarPessoa(pBuffer);
            break;
        case 2:
            buscarPessoa(pBuffer);
            break;
        case 3:
            listarTodas(pBuffer);
            break;
        case 4:
            apagarIndividuo(pBuffer);
            break;
        case 5:
            limparLista(pBuffer);
            free(pBuffer);
            exit(0);
            break;
        default:
            break;
        }
    }
}
