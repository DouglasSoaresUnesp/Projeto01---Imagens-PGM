#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct{
    char type[3];
    char rows[16];
    char columns[16];
    char max_gray[5];
} IMG_INFO;

//PROTOTIPOS
int menuOptions();
FILE * openArchive();
void closeArchive(FILE *fp);
void getHeader(IMG_INFO *header, FILE *fp);
void loading();
int *  fillMatriz(FILE *fp, int size);
void limiarizacao(int *matriz_img, int *matriz_limiarizada, int i, int j);
void negative(int *matriz_img, int *matriz_negativa, int i, int j);
void histogram(int *matriz_img, int *matriz_histograma, int i, int j);
void newArchive(int *matriz_resultante, IMG_INFO header, FILE *fp_resultante, int matriz_size, int res);

int main(void){
    FILE *fp;
    FILE *fp_resultante;
    IMG_INFO header;
    int res, matriz_size, *matriz_img, *matriz_resultante;

    //menu de opções e abertura do arquivo
    res = menuOptions();
    fp = openArchive(-1);
    getHeader(&header, fp);
    
    //convertendo header para int
    int max_rows = atoi(header.rows);
    int max_columns = atoi(header.columns);
    int max_gray = atoi(header.max_gray);

    //achando tamanho da matriz resultante
    matriz_size = max_rows*max_columns;

    //calocando matrizes
    matriz_img = (int *) malloc (matriz_size * sizeof(int));
    matriz_resultante = (int *) malloc (matriz_size * sizeof(int));

    //preenchendo matriz primária
    matriz_img = fillMatriz(fp, matriz_size);

    //verificando opção escolhida pelo usuário
    if(res == 0){
        limiarizacao(matriz_img, matriz_resultante, max_rows, max_columns);
    }
    else if(res == 1){
        negative(matriz_img, matriz_resultante, max_rows, max_columns);
    }
    else if(res == 2){
        histogram(matriz_img, matriz_resultante, max_rows, max_columns);
    }

    //gerando novo arquivo a partir do resultado
    newArchive(matriz_resultante, header, fp_resultante, matriz_size, res);

    //liberando matrizes alocadas
    free(matriz_img);
    free(matriz_resultante);

    //fechando arquivos
    closeArchive(fp);
    closeArchive(fp_resultante);
}

int menuOptions(){
    char op;
    int res;    

    printf("======MENU======\n");
    printf(" (L)imiarizacao\n");
    printf(" (N)egativo\n");
    printf(" (H)istograma\n");
    printf(" (S)air\n");
    printf("================\n");
    printf("Opcao: ");

    fflush(stdin);
    scanf("%c", &op);

    printf("\n");

    //convertendo para uppercase
    op = toupper(op);

    switch(op)
    {
    case 'L':
        res = 0;
        // limiarizacao;
        break;
    case 'N':
        res = 1;
        // negative;
        break;
    case 'H':
        res = 2;
        // histogram;
        break;
    case 'S':
        res = 3;
        exit(1);
        break;
    default:
        printf("Essa opcao nao existe.\n");
        res = -1;
        res = menuOptions();
        break;
    }

    return res;
}

FILE * openArchive(int a){
    FILE *fp;
    char *nome_arquivo = (char *) malloc (20 * sizeof(char));
    char *tipo_abertura = (char *) malloc (3 * sizeof(char));

    printf("Digite o nome do arquivo: ");
    scanf(" %s", nome_arquivo);

    //verifica se é o arquivo primario ou resultante e qual tipo de resultante sendo aberto
    if(a == -1){
        tipo_abertura = "r";
        fp = fopen(strcat(nome_arquivo,".pgm"), tipo_abertura);
    }

    if(a == 0 || a == 1){
        tipo_abertura = "w";
        fp = fopen(strcat(nome_arquivo,".pgm"), tipo_abertura);
    }

    if(a == 2){
        tipo_abertura = "w";
        fp = fopen(strcat(nome_arquivo,".txt"), tipo_abertura);
    }
    
    //verificando se abriu corretamente
    if(fp == NULL){
        printf("Nao foi possivel abrir o arquivo desejado!\n");
        getchar();
        exit(1);
        return 0;
    } else {
        printf("Arquivo aberto com sucesso!\n\n");
        rewind(fp);
        return fp;
    }

    free(nome_arquivo);
    free(tipo_abertura);
}

void closeArchive(FILE *fp){
    int r;

    //fechando arquivo
    r = fclose(fp);

    //verificando se o arquivo foi fechado com exito
    if(r == 0){
        printf("Arquivo fechado com exito!");
        exit(1);
    } else {
        printf("Arquivo nao pode ser fechado!");
    }
}

void getHeader(IMG_INFO *header, FILE *fp){
    int res;
    char c[100];
    int  x;
    char c1;
    fflush(stdin);

    //pegando tres primeiras linhas do arquivo
    fgets((*header).type, 3, fp);
    fgets(c,10, fp);
    //linha necessaria caso tenha os creditos do autor da imagem com o #
    // fgets(c,50, fp); 
    x = fscanf(fp, "%s %s\n", (*header).rows, (*header).columns);
    fscanf(fp, "\n%s\n", (*header).max_gray);
    
    //verificando se o armazenamento de arquivos deu certo
    if(x == 0){
        printf("Nenhuma entrada lida!\n");
        res = 0;
    }else{
        res = 1;
    }

    if(res == 1){
        printf("Informacoes foram armazenadas com sucesso!\n\n");
    } else {
        printf("Erro ao coletar as informacoes!\n");
        closeArchive(fp);
    }
}

void loading(){
    for(int z=0 ; z<3 ; z++){
        printf(".");
        sleep(1);
    }
    printf("\n\n");
}

int * fillMatriz(FILE *fp, int size){
    int px = 0, i = 0;
    char aux[20];

    int *matriz_aux;
    matriz_aux = (int *) malloc (size * sizeof(int));

    while(!feof(fp)){
        fscanf(fp, "%s ", aux);

        px = atoi(aux);
        matriz_aux[i] = px;
        i++;
    }
    return matriz_aux;
    free(matriz_aux);
}

void limiarizacao(int *matriz_img, int *matriz_limiarizada, int i, int j){
    int limiar = 150;
    
    //passando por toda a matriz comparando os valores com o limiar
    for(int m=0 ; m<i*j ; m++){
        if(matriz_img[m] > limiar){
            matriz_limiarizada[m] = 255;
        } else {
            matriz_limiarizada[m] = 0;
        }
    }
    loading();
    printf("Imagem limiarizada com sucesso!\n\n");
}

void negative(int *matriz_img, int *matriz_negativa, int i, int j){
    
    //passando por toda a matriz e subtraindo o valor achado de 255
    for(int m=0 ; m<i*j ; m++){
            matriz_negativa[m] = 255 - matriz_img[m];
    }
    loading();
    printf("Imagem convertida para negativa com sucesso!\n\n");
}

void histogram(int *matriz_img, int *matriz_histograma, int i, int j){
    int pos = 0, m;

    for(m=0 ; m<i*j ; m++){
        matriz_histograma[m] = 0;
    }

    for(m=0 ; m<i*j ; m++){
        pos = matriz_img[m];
        matriz_histograma[pos] += 1;
    }
    loading();
    printf("Imagem traduzida para histograma com sucesso!\n\n");
}

void newArchive(int *matriz_resultante, IMG_INFO header, FILE *fp_resultante, int matriz_size, int res){

    fp_resultante = openArchive(res);
    int x;

    //verificando se é (limiarizacao/negativo) ou histograma
    if(res == 0 || res == 1){

        //inserindo header no arquivo resultante
        fprintf(fp_resultante,"%s\n%s %s\n%s\n", header.type, header.rows, header.columns, header.max_gray);

        //preenchendo arquivo com matriz resultante
        for(int i=0 ; i<matriz_size ; i++){
            x = fprintf(fp_resultante,"%d ", matriz_resultante[i]);
        }

    } 

    //se for o histograma
    if(res == 2){
        for(int i=0 ; i<atoi(header.max_gray) ; i++){
            x = fprintf(fp_resultante,"%d: %d\n", i, matriz_resultante[i]);
        }

    }

    //verificando se houve erro na inserção de dados do arquivo
    if(x > 0){
        printf("Dados inseridos com sucesso!\n");
    } else {
        printf("Nao foi possivel inserir os dados dentro do arquivo!\n");
        closeArchive(fp_resultante);
        exit(1);
    }
}

