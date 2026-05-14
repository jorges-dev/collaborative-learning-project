/*
 * SNAKE EM C  -  versao Windows e Dev-C++ Embarcadero
 * =====================================================
 * COMO COMPILAR NO DEV-C++:
 *   1. Abrir o Dev-C++
 *   2. File > New > Source File
 *   3. Colar este codigo (ou File > Open para abrir o arquivo)
 *   4. Execute > Compile and Run  (ou pressionar F11)
 *
 * Nao precisa instalar nenhuma biblioteca extra.
 * Usa apenas:
 *   stdio.h    - printf (entrada e saida padrao)
 *   stdlib.h   - rand() e srand() (numeros aleatorios)
 *   conio.h    - _kbhit() e _getch() (teclado sem Enter)
 *   windows.h  - Sleep(), cursor e limpeza de tela
 *   time.h     - time() (semente para o aleatorio)
 */

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

/* ─────────────────────────────────────────────────────────────────────────────
   CONSTANTES  (substituidas pelo compilador antes de compilar)
   ───────────────────────────────────────────────────────────────────────────── */
#define LARGURA    40    /* colunas do campo                                     */
#define ALTURA     20    /* linhas do campo                                      */
#define MAX_CORPO  200   /* tamanho maximo que a cobra pode atingir              */
#define VELOCIDADE 130   /* pausa em milissegundos entre frames (menor = rapido) */

/* Codigos das setas no Windows.
   _getch() retorna 224 primeiro, depois um destes codigos: */
#define SETA_CIMA    72
#define SETA_BAIXO   80
#define SETA_ESQUER  75
#define SETA_DIREITA 77

/* ─────────────────────────────────────────────────────────────────────────────
   STRUCT  -  agrupa x e y num tipo so chamado Ponto
   ───────────────────────────────────────────────────────────────────────────── */
typedef struct {
    int x;   /* coluna (cresce para a direita) */
    int y;   /* linha  (cresce para baixo)     */
} Ponto;

/* ─────────────────────────────────────────────────────────────────────────────
   VARIAVEIS GLOBAIS
   ───────────────────────────────────────────────────────────────────────────── */
Ponto cobra[MAX_CORPO]; /* cobra[0] = cabeca, cobra[1] = pescoco ...            */
int   tamanho;          /* quantos segmentos a cobra tem agora                  */
Ponto comida;           /* posicao atual da comida no campo                     */
int   dx, dy;           /* direcao: dx=1 direita | dx=-1 esquerda              */
                        /*          dy=1 baixo   | dy=-1 cima                  */
int   pontos;           /* placar                                               */
int   rodando;          /* 1 = jogo ativo | 0 = encerrar                       */

/* ─────────────────────────────────────────────────────────────────────────────
   FUNCOES DO TERMINAL WINDOWS
   ───────────────────────────────────────────────────────────────────────────── */

/* Esconde o cursor piscante do terminal */
void esconder_cursor() {
    CONSOLE_CURSOR_INFO info;
    info.dwSize   = 1;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
}

/* Limpa a tela (equivalente ao cls do prompt do Windows) */
void limpar_tela() {
    system("cls");
}

/* ─────────────────────────────────────────────────────────────────────────────
   FUNCOES DO JOGO
   ───────────────────────────────────────────────────────────────────────────── */

/*
 * nova_comida()
 * Sorteia posicao aleatoria dentro do campo (sem cair nas bordas).
 * +1 e -2 garantem que a comida nao aparece em cima do # da borda.
 */
void nova_comida() {
    comida.x = 1 + rand() % (LARGURA - 2);
    comida.y = 1 + rand() % (ALTURA  - 2);
}

/*
 * iniciar()
 * Configura o estado inicial antes do jogo comecar.
 */
void iniciar() {
    int i;
    tamanho = 4;   /* cobra comeca com 4 segmentos */
    dx      = 1;   /* indo para a direita          */
    dy      = 0;
    pontos  = 0;
    rodando = 1;

    /* srand com time(NULL) garante sequencias diferentes a cada partida */
    srand((unsigned int)time(NULL));

    /* Posiciona a cobra no centro, enfileirada horizontalmente */
    for (i = 0; i < tamanho; i++) {
        cobra[i].x = LARGURA / 2 - i;
        cobra[i].y = ALTURA  / 2;
    }

    nova_comida();
}

/*
 * ler_entrada()
 * Le a tecla pressionada sem travar o programa.
 * _kbhit() retorna 1 se ha tecla disponivel, sem bloquear.
 * Setas no Windows enviam 2 bytes: 224 seguido do codigo da direcao.
 * Regra anti-inversao: nao pode virar 180 graus.
 */
void ler_entrada() {
    int tecla;
    if (!_kbhit()) return;

    tecla = _getch();

    if (tecla == 224 || tecla == 0) {
        tecla = _getch();
        switch (tecla) {
            case SETA_CIMA:    if (dy ==  0) { dx =  0; dy = -1; } break;
            case SETA_BAIXO:   if (dy ==  0) { dx =  0; dy =  1; } break;
            case SETA_ESQUER:  if (dx ==  0) { dx = -1; dy =  0; } break;
            case SETA_DIREITA: if (dx ==  0) { dx =  1; dy =  0; } break;
        }
    } else if (tecla == 'q' || tecla == 'Q') {
        rodando = 0;
    }
}

/*
 * mover()
 * Avanca a cobra um passo.
 * Logica: percorre de tras para frente, cada segmento copia o anterior.
 * Depois a cabeca avanca com dx e dy.
 *
 * Antes (indo direita):  cabeca[10,10] [9,10] [8,10]
 * Depois:                cabeca[11,10] [10,10] [9,10]
 */
void mover() {
    int i;
    for (i = tamanho - 1; i > 0; i--) {
        cobra[i] = cobra[i - 1];   /* cada segmento assume posicao do anterior */
    }
    cobra[0].x += dx;
    cobra[0].y += dy;
}

/*
 * verificar()
 * Checa colisoes e coleta de comida apos cada passo.
 */
void verificar() {
    int i;
    int hx = cobra[0].x;
    int hy = cobra[0].y;

    /* Colisao com parede */
    if (hx <= 0 || hx >= LARGURA - 1 ||
        hy <= 0 || hy >= ALTURA  - 1) {
        rodando = 0;
        return;
    }

    /* Colisao com o proprio corpo */
    for (i = 1; i < tamanho; i++) {
        if (cobra[i].x == hx && cobra[i].y == hy) {
            rodando = 0;
            return;
        }
    }

    /* Comeu a comida */
    if (hx == comida.x && hy == comida.y) {
        if (tamanho < MAX_CORPO) tamanho++;
        pontos += 10;
        nova_comida();
    }
}

/*
 * desenhar()
 * Redesenha o campo inteiro a cada frame.
 * Percorre todas as celulas e decide o que imprimir em cada uma.
 * Isso cria a ilusao de movimento (como um flipbook).
 */
void desenhar() {
    int x, y, i, achou;
    limpar_tela();

    for (y = 0; y < ALTURA; y++) {
        for (x = 0; x < LARGURA; x++) {

            /* Borda */
            if (y == 0 || y == ALTURA - 1 || x == 0 || x == LARGURA - 1) {
                printf("#");
                continue;
            }

            /* Comida */
            if (x == comida.x && y == comida.y) {
                printf("*");
                continue;
            }

            /* Cobra: O para cabeca, o para corpo */
            achou = 0;
            for (i = 0; i < tamanho; i++) {
                if (cobra[i].x == x && cobra[i].y == y) {
                    printf("%c", i == 0 ? 'O' : 'o');
                    achou = 1;
                    break;
                }
            }

            if (!achou) printf(" ");   /* celula vazia */
        }
        printf("\n");
    }

    printf("Pontos: %d   |   Setas = mover   |   Q = sair\n", pontos);
}

/* ─────────────────────────────────────────────────────────────────────────────
   MAIN  -  todo programa C comeca aqui
   ───────────────────────────────────────────────────────────────────────────── */
int main() {
    esconder_cursor();
    iniciar();

    /*
     * GAME LOOP
     * Roda continuamente ate rodando == 0.
     * Padrao: ler entrada > mover > verificar > desenhar > pausar.
     * Esse mesmo padrao existe em todo jogo, de Snake a jogos 3D.
     */
    while (rodando) {
        ler_entrada();
        mover();
        verificar();
        desenhar();
        Sleep(VELOCIDADE);   /* pausa Windows em milissegundos */
    }

    /* Tela de fim de jogo */
    limpar_tela();
    printf("\n\n");
    printf("   *** FIM DE JOGO ***\n\n");
    printf("   Pontuacao final: %d\n\n", pontos);
    printf("   Pressione qualquer tecla para sair...\n");
    _getch();

    return 0;
}
