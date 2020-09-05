#include <iostream>
#include <string>
#include <stdio.h>
#include <string.h>

using namespace std;

struct perg
{
    char pergunta[255];
    char respostas[3][255];
    int respostaCerta;
};

#pragma region Leitura

void sair(int i){
    system("pause");
    exit(i);
}


int lerPerguntas(perg perguntas[], char localizacao[])
{
    FILE *arquivoPerguntas = fopen(localizacao, "r");
    char secao[255];

    for (int linha = 0, naLinha = 0; fscanf(arquivoPerguntas, "%[^;]%*c", secao) != EOF; naLinha++)
    {
        if (naLinha > 4)
        {
            naLinha = 0;
            linha++;
        }
        else if (naLinha == 1)
        {
            strcpy(perguntas[linha].pergunta, secao);
        }
        else if (naLinha > 1)
        {
            strcpy(perguntas[linha].respostas[naLinha - 2], secao);
        }
    }

    fclose(arquivoPerguntas);
    return 0;
}

int lerRespostas(perg perguntas[], char localizacao[])
{
    FILE *arquivoRespostas = fopen(localizacao, "r");
    int secao;

    for (int linha = 0; fscanf(arquivoRespostas, "%d", &secao) != EOF; linha++)
    {
        perguntas[linha].respostaCerta = secao;
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
void carregarArquivos(perg *perguntas, char localizacao1[], char localizacao2[])
{
    printf("\nCarregando arquivos... por favor aguarde!");
    if (lerPerguntas(perguntas, localizacao1) == 1)
    {
        printf("\nErro na leitura do(s) arquivo(s)");
        sair(1);
    }
    if (lerRespostas(perguntas, localizacao2) == 1)
    {
        printf("\nErro na leitura do(s) arquivo(s)");
        sair(1);
    }
    printf(" Concluido com sucesso!");
}

void verificarExistencia(char localizacao1[], char localizacao2[]){
    printf("\nProcurando arquivos... ");
    char *localizacao[2];
    localizacao[0] = localizacao1;
    localizacao[1] = localizacao2;
    for (int i = 0; i < 2; i++){
        FILE *file = fopen(localizacao[i], "r");
        if (file == NULL){
            printf("\nArquivo nao existe! %s", localizacao[i]);
            sair(1);
        }
        fclose(file);
    }
    printf(" Encontrados!");

}

#pragma endregion Leitura

#pragma region ExecutarPerguntas
int executarPerguntas(struct perg *perguntas, int total)
{
    int acertos = 0;
    for (int atual = 1; atual <= total; atual++)
    {
        printf("\nPergunta n. %d:\n", atual);
        int atualP = atual - 1;

        printf("\n%s", perguntas[atualP]);
        for (int i = 0; i < 3; i++)
        {
            printf("\n\tAlternativa %d: %s", i + 1, perguntas[atualP].respostas[i]);
        }
        int resposta = -999;
        while (resposta < 1 || resposta > 3)
        {
            if (resposta != -999)
            {
                printf("\nERRO! Alternativa invalida");
            }
            printf("\nSua resposta: ");
            cin >> resposta;
        }

        int respostaCerta = perguntas[atualP].respostaCerta;
        if (resposta == respostaCerta)
        {
            printf("\nParabens! Voce acertou!");
            acertos++;
        }
        else
        {
            printf("\nQue pena! Voce errou... a resposta certa era a alternativa %d", perguntas[atual].respostaCerta);
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
    printf("\nVoce acertou %.2f%%", (float(acertos) / float(totalPerguntas)) * 100);
}
#pragma endregion ExecutarPerguntas


int main(void)
{
    char localizacaoPerguntas[] = "src/perguntas.txt";
    char localizacaoRespostas[] = "src/gabarito.txt";
    verificarExistencia(localizacaoPerguntas, localizacaoRespostas);    
    int totalPerguntas = pegarTotalPerguntas(localizacaoPerguntas, localizacaoRespostas);
    perg perguntas[totalPerguntas];
    carregarArquivos(perguntas, localizacaoPerguntas, localizacaoRespostas);
    int acertos = executarPerguntas(perguntas, totalPerguntas);
    imprimirResultado(totalPerguntas, acertos);
    system("pause");
    return 0;
}
