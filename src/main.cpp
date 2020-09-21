#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

const char correct[] = "correct";
const char wrong[] = "wrong";
const char youWin[] = "win";
const char youLose[] = "lose";

enum Color
{
    Black = 0,
    Blue = 1,
    Green = 2,	
    Aqua = 3,
    Red = 4,
    Purple = 5,
    Yellow = 6,
    White = 7,
    Gray = 8,
    LightBlue = 9,
    LightGreen = 10,
    LightAqua = 11,
    LightRed = 12,
    LightPurple = 13,
    LightYellow = 14,
    BrightWhite = 15,
};

enum Nivel
{
    todos = 0,
    facil = 1,
    medio = 2,
    dificil = 3
};

enum Tabela
{
    coluna_id = 0,
    coluna_dificuldade = 1,
    coluna_pergunta = 2,
    coluna_alternativa = 3
};

typedef struct Perguntas
{
    char pergunta[255];
    char respostas[5][255];
    char respostaCerta[3];
    Nivel nivel;
} Pergunta;

typedef struct _Resumo{
    int acertos = 0;
    int total = 0;
} Resumo;

using namespace std;

void clear()
{
#if defined _WIN32
    system("cls");
#elif defined(__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
    system("clear");
#elif defined(__APPLE__)
    system("clear");
#endif
}

void SetColor(int ForgC)
{
    WORD wColor;
    HANDLE hStdOut = GetStdHandle((DWORD)-11);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if(GetConsoleScreenBufferInfo(hStdOut, &csbi)){
        wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
        SetConsoleTextAttribute(hStdOut, wColor);
    }
    return;
 }

#pragma region Leitura

void sair(int i)
{
    cin.get();
    exit(i);
}

int lerPerguntas(Perguntas *perguntas, char localizacaoPerguntas[255])
{
    char secao[255];
    FILE *arquivoPerguntas = fopen(localizacaoPerguntas, "r");
    if (arquivoPerguntas == NULL) return 1;

    for (int linha = 0, coluna = 0; fscanf(arquivoPerguntas, "%[^;]%*c", secao) != EOF; coluna++)
    {
        if (secao[0] == 10) // 10 = \n (ascii)
        {
            coluna = 0;
            linha++;
        }
        if (coluna == coluna_dificuldade)
        {
            int nivel;
            sscanf(secao, "%d", &nivel);
            perguntas[linha].nivel = static_cast<Nivel>(nivel);
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
    if (arquivoRespostas == NULL) return 1;
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
    SetColor(Color::White);
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
        fclose(arquivo);
    }

    if (linhas[0] != linhas[1])
    {
        SetColor(Color::Red);
        printf("\nERRO! Os arquivos %s e %s contem numeros de linhas diferentes! %d e %d", localizacao1, localizacao2, linhas[0], linhas[1]);
        SetColor(Color::White);
        sair(1);
    }
    SetColor(Color::Green);
    printf(" Concluido com sucesso!");

    return linhas[0];
}

void carregarArquivos(Perguntas *perguntas, char localizacaoPerguntas[255], char localizacaoRespostas[255])
{
    SetColor(Color::White);
    printf("\nCarregando arquivos... por favor aguarde!");

    if (lerPerguntas(perguntas, localizacaoPerguntas) == 1)
    {
        SetColor(Color::Red);
        printf("\nErro na leitura do(s) arquivo(s)");
        SetColor(Color::White);
        sair(1);
    }
    if (lerRespostas(perguntas, localizacaoRespostas) == 1)
    {
        SetColor(Color::Red);
        printf("\nErro na leitura do(s) arquivo(s)");
        SetColor(Color::White);
        sair(1);
    }
    SetColor(Color::Green);
    printf(" Concluido com sucesso!\n");
}

int verificarNivel()
{
    bool respostaValida = false;
    int nivel = 0;
    
    Color cor1 = Color::Gray;
    Color cor2 = Color::White;

    clear();

    SetColor(cor1);
    printf ("\n*-----------------------------*");
    printf ("\n|    "); SetColor(Color::BrightWhite); printf("NIVEIS DE DIFICULDADE"); SetColor(cor1); printf("    |");
    printf ("\n*-----------------------------*");
    printf ("\n|  "); SetColor(cor2); printf ("0"); SetColor(cor1); printf (" - "); SetColor(Color::Blue);   printf("TODOS                  "); SetColor(cor1);printf("|");
    printf ("\n|  "); SetColor(cor2); printf ("1"); SetColor(cor1); printf (" - "); SetColor(cor2); printf("NIVEL "); SetColor(Color::Green);  printf("FACIL            "); SetColor(cor1);printf("|");
    printf ("\n|  "); SetColor(cor2); printf ("2"); SetColor(cor1); printf (" - "); SetColor(cor2); printf("NIVEL "); SetColor(Color::Yellow); printf("MEDIO            "); SetColor(cor1);printf("|");
    printf ("\n|  "); SetColor(cor2); printf ("3"); SetColor(cor1); printf (" - "); SetColor(cor2); printf("NIVEL "); SetColor(Color::Red);    printf("DIFICIL          "); SetColor(cor1);printf("|");
    printf ("\n*-----------------------------*");

    while (respostaValida == false)
    {
        SetColor(cor2);
	    printf ("\n\nEscolha um nivel -> ");
        scanf("%d", &nivel);
        if (nivel < 0 || nivel > 3)
        {
            SetColor(Color::Red);
            printf("\nERRO! "); SetColor(Color::LightYellow); printf("Dificuldade invalida, escolha um numero entre (0, 1, 2 ou 3).");
        }
        else
        {
            respostaValida = true;
        }
    }
    return nivel;
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
            sair(1);
        }
        fclose(file);
    }
    printf(" Encontrados!");
}

void tocarMusica(char nome[])
{
    char cmd[] = "start effects/";
    strcat(cmd, nome);
    strcat(cmd, ".mp3");
    printf("%s", cmd);
    //system(cmd);
}

#pragma endregion Leitura

void efeitoDigitar(char *texto){
    for (int i = 0; i < strlen(texto); i++){
        printf("%c", texto[i]); Sleep(50);
    }
}

#pragma region ExecutarPerguntas
Resumo executarPerguntas(Pergunta *perguntas, int total, int nivel)
{
    int acertos = 0;
    clear();
    SetColor(Color::LightGreen);
    printf("\n\t\tBOA PROVA\n\n");
    Sleep(1000);
    clear();

    Resumo resumo;

    for (int atual = 1; atual <= total; atual++)
    {
        if (perguntas[atual-1].nivel == nivel || nivel == todos)
        {
            resumo.total = resumo.total + 1;
            SetColor(Color::White);
            printf("\nPergunta n. %d: ", resumo.total);
            int atualP = atual - 1;
            SetColor(Color::Yellow);
            efeitoDigitar(perguntas[atualP].pergunta);
            SetColor(Color::White);
            int i = 0;
            for (char abcde = 'A'; abcde <= 'E'; ++abcde)
            {
                if (strlen(perguntas[atualP].respostas[i]) > 0)
                {
                    printf("\n\t%c) %s", abcde, perguntas[atualP].respostas[i]);
                }
                i++;
            }

            bool respostaValida = false;

            char resposta[3];
            while (respostaValida == false)
            {
                SetColor(Color::White);
                printf("\nSua resposta: ");
                scanf("%s", resposta);

                *resposta = toupper(*resposta);

                if ((strcmp(resposta, "A") != 0) &&
                    (strcmp(resposta, "B") != 0) &&
                    (strcmp(resposta, "C") != 0) &&
                    (strcmp(resposta, "D") != 0) &&
                    (strcmp(resposta, "E") != 0))
                {
                    SetColor(Color::Red);
                    printf("\nERRO!");SetColor(Color::Yellow); printf(" Alternativa invalida");
                }
                else
                {
                    respostaValida = true;
                }
            }

            if (strcmp(resposta, perguntas[atualP].respostaCerta) == 0)
            {
                SetColor(Color::Green);
                printf("\nParabens! Voce acertou!\n");
                system("start effects/correct.mp3");
                //tocarMusica((char *)correct);
                resumo.acertos++;

            }
            else
            {
                SetColor(Color::Red);
                printf("\nQue pena! Voce errou... a resposta certa era a alternativa %s\n", perguntas[atualP].respostaCerta);
                system("start effects/wrong.mp3");
                //tocarMusica((char *)wrong);
                
            }
            SetColor(Color::White);
            Sleep(1000);
            system("pause");
            clear();
        }
        
    }
    return resumo;
}

void exibirImagem(char localizacaoImagem[255])
{

    FILE *imagem = fopen(localizacaoImagem, "r");

    if (imagem != NULL)
    {
        char linha[100];
        while (fscanf(imagem, "%[^\n]%*c", linha) != EOF)
        {
            printf("%s\n", &linha);
        }
    }
    fclose(imagem);
}

void imprimirResultado(Resumo resumo)
{
    char imagem[25];
    float percentual = (float(resumo.acertos) / float(resumo.total)) * 100;
    if(percentual >= 50){
        SetColor(Color::Yellow);
        exibirImagem((char *)"img/trofeu.txt");
        system("start effects/win.mp3");
        //tocarMusica((char *)youWin);
    }
    else
    {
        SetColor(Color::Gray);
        exibirImagem((char *)"img/gamover.txt");
        system("start effects/lose.mp3");
        //tocarMusica((char *)youLose);
    }

    SetColor(Color::White);
    printf("\nObrigado por participar do nosso programa!");
    printf("\nTotal de perguntas: %d", resumo.total);
    printf("\nTotal de acertos: %d", resumo.acertos);
    printf("\nTotal de erros: %d", resumo.total - resumo.acertos);
    printf("\nVoce acertou %.2f%%\n\n", percentual);
}


#pragma endregion ExecutarPerguntas

void iniciarMediaPlayer()
{
    printf("Iniciando media player...");
    system("start wmplayer.exe");
    printf("Concluido!!\n");
}

int main(void)
{
    // iniciarMediaPlayer();
    char localizacaoPerguntas[255] = "perguntas.txt";
    char localizacaoRespostas[255] = "gabarito.txt";
    verificarExistencia(localizacaoPerguntas, localizacaoRespostas);
    int totalPerguntas = pegarTotalPerguntas(localizacaoPerguntas, localizacaoRespostas);
    Pergunta *perguntas = (Pergunta *)malloc(sizeof(Pergunta) * totalPerguntas);
    carregarArquivos(perguntas, localizacaoPerguntas, localizacaoRespostas);
    int nivel = verificarNivel();
    Resumo resumo = executarPerguntas(perguntas, totalPerguntas, nivel);
    imprimirResultado(resumo);
    system("pause");
    return 0;
}
