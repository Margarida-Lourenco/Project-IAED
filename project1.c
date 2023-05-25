/* iaed-23 - ist1107137 - project1 */

/*
 * Arquivo: project1.c
 * Autor: Margarida Lourenco
 * Descricao: Programa que permite a gestao de uma rede de transportes publicos.
*/

/*Bibliotecas*/

#include <stdio.h>
#include <string.h>

/*####################### DEFENICAO DE COONSTANTES ###########################*/

#define MAX_CARREIRAS 200   /*numero maximo de carreiras*/
#define MAX_PARAGENS 10000  /*numero maximo de paragens*/

#define NOME_CARREIRA_MAX 20 + 1  /*dimensao maxima do nome da carreira*/
#define NOME_PARAGEM_MAX 50 + 1   /*dimensao maxima do nome da paragem*/
#define INVERSO 7 + 1             /*dimensao da palavra inverso*/
#define NOVA_LINHA 2              /*estado de nova linha*/  

/*Constantes booleanas*/
#define TRUE -1             
#define FALSE -2      

/*Erros*/
#define ERRO_PARAGEM_NAO_EXISTE "no such stop.\n"
#define ERRO_CARREIRA_NAO_EXISTE "no such line.\n"
#define ERRO_NAO_EXTREMO_CARREIRA "link cannot be associated with bus line.\n"
#define ERRO_INVERSO "incorrect sort option.\n"
#define ERRO_PARAGEM_EXISTE "stop already exists.\n"
#define ERRO_CUSTO_DURACAO "negative cost or duration.\n"
#define LOCALIZACAO_INVALIDA "invalid lication\n"

/* ########################### TIPOS DE DADOS ############################## */

typedef struct{
    char nomeParagem[NOME_PARAGEM_MAX];
    double latitude;
    double longitude;
    int num_carreiras;
}Paragem;

typedef struct{
    char nomeCarreira[NOME_CARREIRA_MAX];
    Paragem paragens[MAX_PARAGENS];
    int num_paragens;  
    double custo_total;
    double duracao_total;                     
}Carreira;

/* ######################### VARIAVEIS GLOBAIS ############################# */

Carreira carreiras[MAX_CARREIRAS];  /*array de carreiras*/
int numCarreiras = 0;               /*numero de carreiras*/               
Paragem Paragens[MAX_PARAGENS];     /*array de paragens*/
int numParagens = 0;                /*numero de paragens*/

/* ######################## PROTOTIPOS DE FUNCOES ########################## */

void adiciona_lista_carreiras();
void adiciona_lista_paragens();
void lista_intersecoes_carreiras();
void adiciona_ligacoes();
void mostra_carreira();

/* ############################## MAIN ##################################### */

int main(){
    int comando;
    while ((comando = getchar()) != EOF){
        switch(comando){ 
        case 'q': return 0;  /*termina o programa*/
        case 'c': adiciona_lista_carreiras();
            break;
        case 'p': adiciona_lista_paragens();
            break;
        case 'l': adiciona_ligacoes();
            break;
        case 'i': lista_intersecoes_carreiras();
            break;
        case 'f': mostra_carreira();
            break;
        }
    }
    return 0;
}

/* ############################ FUNCOES #################################### */

/*----------------------------- INPUT ---------------------------------------*/

/*funcao auxiliar que le uma palavra do standard input. Caso essa palavra 
esteja entre aspas, guarda a palavra sem as aspas*/
int lePalavra(char s[], int max) {
    int c, i = 0, aspas = 0;
    while ((c = getchar()) == ' ');
    while (c != EOF && c != '\n' && i < max) {
        if (aspas == 1) {  
            if (c == '\"')   
                aspas = 0;
            else
                s[i++] = c;
        }
        else if (aspas == 0) {
            if (c == '\"')
                aspas = 1;
            else if (c == ' ')
                break;
            else
                s[i++] = c;
        }
        c = getchar();
    }     
    if (i == 0)
        return 0;
    s[i] = '\0';
    if (c == '\n')
        return NOVA_LINHA;
    return 1;
}

/*---------------------------- COMANDO C ------------------------------------*/

/*Verifica se uma carreira existe. Caso exista, retorna o indice da mesma no
array carreiras*/
int eh_carreira(char nome_carreira[]) {
    int i;
    for (i = 0; i < numCarreiras; i++) {
        if (strcmp(nome_carreira, carreiras[i].nomeCarreira) == 0)
            return i;
    }
    return FALSE;
}

/*Cria uma nova carreira*/
void nova_carreira(char nome_carreira[]) {
    strcpy(carreiras[numCarreiras].nomeCarreira, nome_carreira);
    carreiras[numCarreiras].num_paragens = 0;
    carreiras[numCarreiras].custo_total = 0;
    carreiras[numCarreiras].duracao_total = 0;
    numCarreiras++;
} 

/*imprime todas as carreiras e respetivas paragens de origem e destino,
numero de paragens, custo e duracao total*/
void imprime_carreiras(){
    int l;
    for (l = 0; l < numCarreiras; l++) {
        if (strlen(carreiras[l].nomeCarreira) < 6)
            printf("%s ", carreiras[l].nomeCarreira);
                if(carreiras[l].num_paragens == 0)  /*carreira sem paragens*/
                    printf("%d %.2f %.2f\n", carreiras[l].num_paragens, 
                    carreiras[l].custo_total, carreiras[l].duracao_total);
                else {
                    printf("%s %s ", carreiras[l].paragens[0].nomeParagem, 
                    carreiras[l].paragens[carreiras[l].num_paragens-1].nomeParagem);
                    printf("%d %.2f %.2f\n", carreiras[l].num_paragens, 
                    carreiras[l].custo_total, carreiras[l].duracao_total);
                }
        }
}

/*verifica se a string dada e igual a palavra "inverso" ou uma das 
suas abreviaturas ate 3 caracteres*/
int eh_inverso(char inverso[]) {
    if (strcmp(inverso, "inverso") == 0) 
        return TRUE;
    else if (strcmp(inverso, "inv") == 0) 
        return TRUE;
    else if (strcmp(inverso, "inve") == 0) 
        return TRUE;
    else if (strcmp(inverso, "inver") == 0) 
        return TRUE;
    else if (strcmp(inverso, "invers") == 0) 
        return TRUE;
    else
        return FALSE;
}

/*funcao do comando c - adiciona e lista carreiras */
void adiciona_lista_carreiras(){
    char nome_carreira[NOME_CARREIRA_MAX];
    char inverso[INVERSO];
    int j=0, i=0, estado=0;

    if ((estado = lePalavra(nome_carreira, NOME_CARREIRA_MAX))){
        /*Se carreira existe*/
        if ((i = eh_carreira(nome_carreira)) != FALSE) { 
            if(estado != NOVA_LINHA){  /*Se o nome_carreira nao termina com \n*/
                lePalavra(inverso, INVERSO);
                if(eh_inverso(inverso)==FALSE) /*se inverso nao e valido*/
                    printf("%s", ERRO_INVERSO);
                else{
                    /*inverso e valido, imprime paragens por ordem inversa*/
                    for (j = carreiras[i].num_paragens-1; j > 0; j--) 
                        printf("%s, ", carreiras[i].paragens[j].nomeParagem);
                    printf("%s\n", carreiras[i].paragens[0].nomeParagem);
                }
            }
            else{ /*nao leu inverso, imprime paragens pela ordem de criacao*/
                if (carreiras[i].num_paragens == 0) 
                return;
                for (j = 0; j < (carreiras[i].num_paragens-1); j++) 
                    printf("%s, ", carreiras[i].paragens[j].nomeParagem);
                printf("%s\n", carreiras[i].paragens[j].nomeParagem);  
            }     
        }
        else /*nome da carreira nao existe*/
            nova_carreira(nome_carreira);
    }
    else /*Saida sem argumentos*/
        imprime_carreiras();
}

/*------------------------------- COMANDO P ---------------------------------*/

/*Cria uma nova paragem*/
void nova_paragem(char nome_paragem[], double lon, double lat){
    strcpy(Paragens[numParagens].nomeParagem, nome_paragem);
    Paragens[numParagens].longitude = lon;
    Paragens[numParagens].latitude = lat;
    Paragens[numParagens].num_carreiras = 0;
    numParagens++;
}

/*Verifica se uma paragem existe. Se existir, retorna o indice da mesma no 
array Paragens*/
int eh_paragem(char nome_paragem[]){
    int i;
    for (i = 0; i < numParagens; i++) {
        if (strcmp(nome_paragem, Paragens[i].nomeParagem) == 0)
            return i;
    }
    return FALSE;
}

/*funcao do comando p - adiciona e lista paragens*/
void adiciona_lista_paragens(){
    char nome_paragem[NOME_PARAGEM_MAX];
    double longi, lati;
    int a, j;

    if(lePalavra(nome_paragem, NOME_PARAGEM_MAX)>0){
        if(scanf("%lf %lf", &longi, &lati) == 2){
            if(eh_paragem(nome_paragem)==FALSE)  /*se paragem nao existe*/
                if (lati < -90 || lati > 90)
                    printf("%s", LOCALIZACAO_INVALIDA);    /* aquiii*/
                else if (longi < -180 || longi > 180)
                    printf("%s", LOCALIZACAO_INVALIDA);
                else
                nova_paragem(nome_paragem, longi, lati);
            else
                printf("%s: %s", nome_paragem, ERRO_PARAGEM_EXISTE);
        }
        else{
            a = eh_paragem(nome_paragem);
            if(a!=FALSE)
            /*Se a paragem existe, imprime a sua latitude e longitude*/
                printf("%16.12f %16.12f\n", Paragens[a].longitude, 
                Paragens[a].latitude);
            else
                printf("%s: %s", nome_paragem, ERRO_PARAGEM_NAO_EXISTE);
        }
    } 
    else /*Saida sem argumentos, imprime todas as paragens*/
        for (j = 0; j < numParagens; j++) {
            printf("%s: %16.12f %16.12f %d\n", Paragens[j].nomeParagem, 
            Paragens[j].longitude, Paragens[j].latitude, 
            Paragens[j].num_carreiras);
        }
}

/*------------------------------- COMANDO L ---------------------------------*/

/*adiciona uma paragem no inicio*/
void adiciona_inicio(int carr, int parag, char paragem[]){
    int i;
    for (i = carreiras[carr].num_paragens; i > 0; i--) {
        strcpy(carreiras[carr].paragens[i].nomeParagem, 
        carreiras[carr].paragens[i-1].nomeParagem);
    }
    strcpy(carreiras[carr].paragens[0].nomeParagem, paragem);
    carreiras[carr].num_paragens++;
    Paragens[parag].num_carreiras++;
}

/*adiciona uma paragem no fim*/
void adiciona_fim(int carr, char paragem[]){
    strcpy(carreiras[carr].paragens[carreiras[carr].num_paragens].nomeParagem, 
    paragem);
    carreiras[carr].num_paragens++;
}         

/*verifica se o nome da paragem do argumento corresponde a paragem de origem da 
carreira de indice carr*/
int eh_origem(int carr, char paragem[]){
    if (strcmp(paragem, carreiras[carr].paragens[0].nomeParagem) == 0) 
        return 0;
    return FALSE;
}

/*verifica se o nome da paragem do argumento corresponde a paragem de destino da 
carreira de indice carr*/
int eh_destino(int carr, char paragem[]){
    if (strcmp(paragem, carreiras[carr].paragens[carreiras[carr].num_paragens-1].nomeParagem) == 0) 
        return 0;
    return FALSE;
}

/*acrescenta o custo e duracao a carreira*/
void soma_cust_durac(int carr, double cust, double durac){
    carreiras[carr].custo_total += cust;
    carreiras[carr].duracao_total += durac;
}

/*funcao que levanta os erros do comando l*/
int erros_l(int carr, char origem[], char destino[], double cust, double durac){
    if (eh_paragem(origem) == FALSE){
            printf("%s: %s", origem, ERRO_PARAGEM_NAO_EXISTE);
            return 1;
    }
    else if (eh_paragem(destino) == FALSE){
            printf("%s: %s", destino, ERRO_PARAGEM_NAO_EXISTE);
            return 1;
    }
    else if (carreiras[carr].num_paragens>0 && 
            (eh_origem(carr, destino)!=0 && eh_destino(carr, origem)!=0)){
            printf("%s", ERRO_NAO_EXTREMO_CARREIRA);
            return 1;
    }
    else if (cust < 0 || durac < 0){
            printf("%s",ERRO_CUSTO_DURACAO);
            return 1;
    }
    return 0;
}

/*funcao comando l - adiciona ligacoes*/
void adiciona_ligacoes(){
    char nome_carr[NOME_CARREIRA_MAX];
    char destino[NOME_PARAGEM_MAX], origem[NOME_PARAGEM_MAX];
    double cust=0, durac=0;
    int carr, parag1, parag2;

if (lePalavra(nome_carr, NOME_CARREIRA_MAX) && lePalavra(origem, NOME_PARAGEM_MAX) && 
    lePalavra(destino, NOME_PARAGEM_MAX) && scanf("%lf %lf", &cust, &durac) == 2){

    carr = eh_carreira(nome_carr);
    if(carr== FALSE)
        printf("%s: %s", nome_carr, ERRO_CARREIRA_NAO_EXISTE);
    else{
        parag1 = eh_paragem(origem); parag2= eh_paragem(destino);
        if (erros_l(carr, origem, destino, cust, durac)==0){  
            if (carreiras[carr].num_paragens == 0){  /*carreira sem paragens*/
                adiciona_inicio(carr, parag1, origem); adiciona_fim(carr, destino);
                soma_cust_durac(carr, cust, durac);
                Paragens[parag2].num_carreiras++;
            }
            else{
                if (eh_origem(carr, destino)==0 && eh_destino(carr, origem)==0) 
                    adiciona_fim(carr, destino); /*carreiras circulares*/
                else if (eh_destino(carr, origem)==0){
                    adiciona_fim(carr, destino); 
                    Paragens[parag2].num_carreiras++;
                }
                else
                    adiciona_inicio(carr, parag1, origem);
                soma_cust_durac(carr, cust, durac);
            }
        }
    }
}
}

/*------------------------------- COMANDO I ---------------------------------*/

/*ordena as carreiras por ordem alfabetica*/
void selection_sort(char arr[][NOME_CARREIRA_MAX], int n) {
    int i, j, min_idx;
    /*variavel temporaria - armazena o nome da carreira durante uma troca*/
    char temp[NOME_CARREIRA_MAX]; 
    for (i=0; i<n-1; i++) {
        min_idx = i;
        for (j=i+1; j<n; j++) {
            if (strcmp(arr[j], arr[min_idx]) < 0) { 
                min_idx = j;  /*guarda indice da carreira com a menor string*/
            }
        }
        if (min_idx != i) {  /*excuta as trocas*/
            strcpy(temp, arr[i]);
            strcpy(arr[i], arr[min_idx]);
            strcpy(arr[min_idx], temp);
        }
    }
}

/*funcao comando i - percorre todas as paragens e, para as que possuem mais que
uma carreira, imprime as carreiras associadas a mesma, por ordem alfabetica*/
void lista_intersecoes_carreiras(){
    int i ,j, k, l;
    /*array temporario que guarda as carreiras de uma paragem*/
    char carreiras_temp[MAX_PARAGENS][NOME_CARREIRA_MAX]; 
     
    for (i=0; i<numParagens; i++){
        if (Paragens[i].num_carreiras > 1){ 
            printf("%s %d: ", Paragens[i].nomeParagem, Paragens[i].num_carreiras);
            l = 0;
            for (j=0; j<numCarreiras; j++)
                for (k=0; k<carreiras[j].num_paragens; k++){
                    /* Verifica se a paragem atual é a mesma da paragem da carreira atual */
                    if (strcmp(carreiras[j].paragens[k].nomeParagem, Paragens[i].nomeParagem) == 0){ 
                        /* Copia o nome da carreira para o array temporário */
                        strcpy(carreiras_temp[l], carreiras[j].nomeCarreira); 
                        l++;  /* Incrementa o índice do array temporário */
                        break;
                    }
                }
            /*ordena por ordem alfabetica*/
            selection_sort(carreiras_temp, Paragens[i].num_carreiras);
            for (j=0; j<Paragens[i].num_carreiras-1; j++)
                printf("%s ", carreiras_temp[j]);
            printf("%s", carreiras_temp[j]);
            printf("\n");
        }
    }
}

/*----------------------------------- COMANDO F -------------------------------*/

void mostra_carreira(){
    char n_paragem[NOME_PARAGEM_MAX];
    int /*paragem_idx,*/ i;

    if (lePalavra(n_paragem, NOME_PARAGEM_MAX)>0){
        /*paragem_idx = eh_paragem(n_paragem);*/
        if (eh_paragem(n_paragem) == FALSE)
            printf("%s: %s", n_paragem, ERRO_PARAGEM_NAO_EXISTE);
        else {
            for (i = 0; i < numCarreiras-1; i++){
                if (strcmp(n_paragem, carreiras[i].paragens[carreiras[i].num_paragens-1].nomeParagem) == 0)
                    printf("%s ", carreiras[i].nomeCarreira);
            }
        }
    }
}

/*mostra as carreiras com fim na paragem indicada, pela ordem ce criacao separadas por espaço*/
