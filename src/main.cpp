#include <iostream>
#include <string.h>
#include <stdio.h>



// Facil = '1',
// Medio = '2',
// Dificil = '3'
// Todos = '0'


struct Perguntas
{
    char pergunta[255];
    char respostas[5][255];
    char respostaCerta[3];
    char nivel;
};

enum tabela
{
    coluna_id = 0,
    coluna_dificuldade = 1,
    coluna_pergunta = 2,
    coluna_alternativa = 3
};

#pragma region Leitura

void sair(int i)
{
    system("pause");
    exit(i);
}

int lerPerguntas(Perguntas *perguntas, char localizacaoPerguntas[255])
{
    char secao[255];
    FILE *arquivoPerguntas = fopen(localizacaoPerguntas, "r");

    for (int linha = 0, coluna = 0; fscanf(arquivoPerguntas, "%[^;]%*c", secao) != EOF; coluna++)
    {
        if (secao[0] == 10) // "\n" igual a 10 tabela ascii
        {
            coluna = 0;
            linha++;
        }

        if (coluna == coluna_dificuldade)
        {
            perguntas[linha].nivel = secao[0];
        }
        else if (coluna == coluna_pergunta)
        {
            strcpy(perguntas[linha].pergunta, secao);
        }
        else if (coluna >= coluna_alternativa)
        {
            strcpy(perguntas[linha].respostas[coluna - 3], secao);
        }
    }

    fclose(arquivoPerguntas);
    return 0;
}

int lerRespostas(Perguntas *perguntas, char localizacaoRespostas[255])
{
    FILE *arquivoRespostas = fopen(localizacaoRespostas, "r");
    int numero;
    char resposta[3];

    while (fscanf(arquivoRespostas, "%d-%s", &numero, resposta) != EOF)
    {
        strcpy(perguntas[numero - 1].respostaCerta, resposta);
    }

    fclose(arquivoRespostas);
    return 0;
}

int pegarTotalPerguntas(char localizacao1[], char localizacao2[])
{
    printf("\nVerificando arquivos...Por favor aguarde!");
    int linhas[2];
    char *localizacao[2];
    localizacao[0] = localizacao1;
    localizacao[1] = localizacao2;
    for (int i = 0; i < 2; i++)
    {
        FILE *arquivo = fopen(localizacao[i], "r");
        char secao[255];
        for (int linha = 1; fscanf(arquivo, "%[^\n]%*c", secao) != EOF; linha++)
        {
            linhas[i] = linha;
        }
    }

    if (linhas[0] != linhas[1])
    {
        printf("\nERRO! Os arquivos %s e %s contem numeros de linhas diferentes! %d e %d", localizacao1, localizacao2, linhas[0], linhas[1]);
        sair(1);
    }
    printf(" Concluido com sucesso!");

    return linhas[0];
}

void carregarArquivos(Perguntas *perguntas, char localizacaoPerguntas[255], char localizacaoRespostas[255])
{
    printf("\nCarregando arquivos... por favor aguarde!");

    if (lerPerguntas(perguntas, localizacaoPerguntas) == 1)
    {
        printf("\nErro na leitura do(s) arquivo(s)");
        sair(1);
    }
    if (lerRespostas(perguntas, localizacaoRespostas) == 1)
    {
        printf("\nErro na leitura do(s) arquivo(s)");
        sair(1);
    }
    printf(" Concluido com sucesso!");
}

void verificarExistencia(char localizacaoPerguntas[255], char localizacaoRespostas[255])
{
    printf("\nProcurando arquivos... ");
    char *localizacao[2];
    localizacao[0] = localizacaoPerguntas;
    localizacao[1] = localizacaoRespostas;
    for (int i = 0; i < 2; i++)
    {
        FILE *file = fopen(localizacao[i], "r");
        if (file == NULL)
        {
            printf("\n\nErro ao abrir o arquivo: %s", localizacao[i]);
            // perror(strcat("\nErro no arquivo ", strcat(localizacao[i], "'")));
            sair(1);
        }
        fclose(file);
    }
    printf(" Encontrados!");
}

#pragma endregion Leitura

#pragma region ExecutarPerguntas
int executarPerguntas(struct Perguntas *perguntas, int total)
{
    int acertos = 0;
    system("cls");

    for (int atual = 1; atual <= total; atual++)
    {
        printf("\nPergunta n. %d: ", atual);
        int atualP = atual - 1;

        printf("\n%s", perguntas[atualP].pergunta);
        int i = 0;
        for (char abcde = 'a'; abcde <= 'e'; ++abcde)
        {
            if (strcmp(perguntas[atualP].respostas[i], "") != 0)
            {
                printf("\n\t%c) %s", toupper(abcde), perguntas[atualP].respostas[i]);
            }
            i++;
        }

        bool respostaValida = false;

        char resposta[3];
        while (respostaValida == false)
        {
            printf("\nSua resposta: ");
            scanf("%s", resposta);

            *resposta = toupper(*resposta);

            if ((strcmp(resposta, "A") != 0) &&
                (strcmp(resposta, "B") != 0) &&
                (strcmp(resposta, "C") != 0) &&
                (strcmp(resposta, "D") != 0) &&
                (strcmp(resposta, "E") != 0))
            {
                printf("\nERRO! Alternativa invalida");
            }
            else
            {
                respostaValida = true;
            }
        }

        if (strcmp(resposta, perguntas[atualP].respostaCerta) == 0)
        {
            printf("\nParabens! Voce acertou!\n");
            acertos++;
        }
        else
        {
            printf("\nQue pena! Voce errou... a resposta certa era a alternativa %s\n", perguntas[atualP].respostaCerta);
        }
    }

    return acertos;
}

void imprimirResultado(int totalPerguntas, int acertos)
{
    printf("\nObrigado por participar do nosso programa!");
    printf("\nTotal de perguntas: %d", totalPerguntas);
    printf("\nTotal de acertos: %d", acertos);
    printf("\nTotal de erros: %d", totalPerguntas - acertos);
    printf("\nVoce acertou %.2f%%\n\n", (float(acertos) / float(totalPerguntas)) * 100);
}
#pragma endregion ExecutarPerguntas

int main(void)
{
    char localizacaoPerguntas[255] = "perguntas.txt";
    char localizacaoRespostas[255] = "gabarito.txt";
    verificarExistencia(localizacaoPerguntas, localizacaoRespostas);
    int totalPerguntas = pegarTotalPerguntas(localizacaoPerguntas, localizacaoRespostas);
    Perguntas *perguntas = (Perguntas *)malloc(sizeof(Perguntas) * totalPerguntas);
    
    carregarArquivos(perguntas, localizacaoPerguntas, localizacaoRespostas);
    int acertos = executarPerguntas(perguntas, totalPerguntas);
    imprimirResultado(totalPerguntas, acertos);
    system("pause");
    return 0;
}
