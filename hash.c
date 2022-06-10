#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define VAZIO "VAZIO"
#define RETIRADO "VAZIO"
#define M 7
#define N 100


//Definição da estrutura da tabela hash
typedef unsigned int TipoApontador;
typedef char TipoChave[N];
typedef unsigned TipoPesos[N];
typedef struct TipoItem {
    TipoChave Chave;
    int Codigo;
} TipoItem;
typedef unsigned int TipoIndice;
typedef TipoItem TipoDicionario[M];

//Cores para a impressão do terminal
void Yellow(){
    printf("\033[0;33m");
}

void Red(){
    printf("\033[1;31m");
}

void Green(){
    printf("\033[1;32m");
}

void White(){
    printf("\033[0m");
}

//Função para inicializar a tabela hash
void Inicializa(TipoDicionario Dicionario){
    int i;
    for(i=0;i<M;i++){
        memcpy(Dicionario [i].Chave, VAZIO, N);
        Dicionario [i].Codigo = -1;
    }
}

//Função para gerar pesos de forma aleatória
void GeraPesos(TipoPesos Pesos){
    int i;
    struct timeval semente;
    gettimeofday(&semente,NULL);
    srand((int)(semente.tv_sec + 1000000*semente.tv_usec));
    for(i=0;i<M;i++){
        Pesos[i] = 1+(int)(100000.0 * rand()/(RAND_MAX+1.0));
    }  
}

//Função para gerar o índice da tabela hash
TipoIndice Transformacao(TipoChave Chave, TipoPesos Pesos){
    int i;
    unsigned int soma = 0;
    int comprimento = strlen(Chave);
    for (i=0;i<comprimento;i++){
        soma += (unsigned int)Chave[i] * Pesos[i];
    }
    return soma % M;
}

//Função para pesquisar um item na tabela hash e calcular os seekings
TipoApontador Pesquisa(TipoChave Chave, TipoPesos Pesos, TipoDicionario Dicionario){
    unsigned int i = 0;
    unsigned int Inicial;
    int seeking = 0;
    Inicial = Transformacao(Chave, Pesos);
    
    while(strcmp (Dicionario[(Inicial + i)%M].Chave, VAZIO) != 0 && 
        strcmp(Dicionario[(Inicial + i)%M].Chave, Chave) != 0 && i < M){
            i++;
            seeking++;
    }
    printf ("%s\t| %d SK\n", Chave, i);
    if(strcmp(Dicionario[(Inicial + i)%M].Chave, Chave) == 0)
    return (Inicial + i)%M;
    else return M;
}


//Função para inserir um item na tabela hash
void Insere(TipoItem Item, TipoPesos Pesos, TipoDicionario Dicionario){
    unsigned int i = 0;
    unsigned int Inicial;
    if (Pesquisa(Item.Chave, Pesos, Dicionario) < M){
        Red();
        printf ("Erro: %s ja inserido.\n", Item.Chave, Item.Codigo);
        White();
        return;
    }
    Inicial  = Transformacao(Item.Chave, Pesos);
    while (strcmp(Dicionario[(Inicial + i)%M].Chave, VAZIO) != 0 && 
        strcmp (Dicionario[(Inicial + i)%M].Chave, RETIRADO) != 0 && i < M){
            i++;
    }
    if (i<M){
        strcpy (Dicionario[(Inicial + i)%M].Chave, Item.Chave);
        Dicionario[(Inicial + i)%M].Codigo = Item.Codigo;
    }
    
    else {
        Red();
        printf ("Erro: Tabela cheia. %s nao inserido.\n", Item.Chave);
        White();
    }
}


//Função para imprimir a tabela hash	
void Printar(TipoDicionario Dicionario){
     for (int i =0; i<M; i++){
        if(strcmp(Dicionario[i].Chave, VAZIO) == 0){
            Red();
            printf("%s\t| %d\n", Dicionario[i].Chave, Dicionario[i].Codigo);
            White();
            
        }
        else if (strcmp(Dicionario[i].Chave, RETIRADO) == 0){
            Red();
            printf("%s\t| %d\n", Dicionario[i].Chave, Dicionario[i].Codigo);
            White();
        }

        else  printf("%s\t| %d\n", Dicionario[i].Chave, Dicionario[i].Codigo);
        
    }

}

//Função para pesquisar um determinado nome e excluir um determinado nome
TipoApontador PT(TipoChave Chave, TipoPesos Pesos, TipoDicionario Dicionario){
    unsigned int i = 0;
    unsigned int Inicial;
    Inicial = Transformacao(Chave, Pesos);
    while(strcmp (Dicionario[(Inicial + i)%M].Chave, VAZIO) != 0 && 
        strcmp(Dicionario[(Inicial + i)%M].Chave, Chave) != 0 && i < M){
            i++;
    }
    if(strcmp(Dicionario[(Inicial + i)%M].Chave, Chave) == 0)
    return (Inicial + i)%M;
    else return M;
}

//Função para verificar se existe um item na tabela hash
void EstaNaTabela(TipoChave Chave, TipoPesos Pesos, TipoDicionario Dicionario){
    if (PT(Chave, Pesos, Dicionario) < M){
        Yellow();
        printf ("%s esta na tabela\n", Chave);
        Red();
        printf("%s\t| %d\n", Dicionario[PT(Chave, Pesos, Dicionario)].Chave, Dicionario[PT(Chave, Pesos, Dicionario)].Codigo);
        White();
    }

    else {
        Red();
        printf ("%s nao esta na tabela\n", Chave);
        White();

    }
    return;
}

//Função para excluir um item da tabela hash
void Retira(TipoChave Chave, TipoPesos Pesos, TipoDicionario Dicionario){
    TipoIndice Indice;
    Indice = PT(Chave, Pesos, Dicionario);
    if (Indice < M){
        Yellow();
        printf ("%s Removido\n", Dicionario[Indice].Chave);
        White();
        memcpy (Dicionario[Indice].Chave, RETIRADO, N);
        Dicionario[Indice].Codigo = -1;
    }
    if (Indice == M){
        Red();
        printf ("Erro: %s nao esta na tabela.\n", Chave);
        White();
    }
    
}


//Função main
int main(){
    TipoDicionario Dicionario;
    TipoPesos Pesos;
    TipoItem Item;
    FILE *arquivo;
    int i = 0;
    char nome[N];
    char nome2[N];

    Inicializa(Dicionario);
    GeraPesos(Pesos);

    
    arquivo = fopen("tripulacao.txt", "r");
    if (arquivo == NULL){
        printf("Erro ao abrir o arquivo\n");
        return 1;
    }
    Green();
    printf("\nTabela Hash:\n\n");
    Printar(Dicionario);
    White();

    Green();
    printf("\nPressione enter para continuar...\n");
    White();
    getchar();


    Green();
    printf("\nInserindo...");
    printf("\nCalculando os seekings...\n\n");
    White();
    
    while (fscanf(arquivo, "%s",  Item.Chave) != EOF){
        fscanf(arquivo, "%d", &Item.Codigo);
        Insere(Item, Pesos, Dicionario);
    }
    fclose(arquivo);

    Green();
    printf("\nPressione enter para continuar...\n");
    White();
    getchar();
    
    Green();
    printf ("\nTabela:\n");
    White();
    Printar(Dicionario);

    Green();
    printf("\nPressione enter para continuar...\n");
    White();
    getchar();

    Green();
    printf("\nPesquise por alguem da tabela:\n");
    White();
    scanf("%s", nome);
    EstaNaTabela(nome, Pesos, Dicionario);

    Green();
    printf("\nRetire alguem da tabela:\n");
    White();
    scanf("%s", nome2);
    Retira(nome2, Pesos, Dicionario);
    Green();
    printf("\nTabela:\n");
    White();
    Printar(Dicionario);

    
    
    Green();
    printf("\nFim do programa\n");
    White();
    
    return 0;
    system ("pause");

}







