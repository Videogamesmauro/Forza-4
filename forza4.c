#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define righe 6
#define colonne 7
#define VUOTO ' '
#define ROSSO 'O'
#define GIALLO '0'
#define vincita 4

// colori
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_ORANGE "\x1b[38;5;208m"
#define ANSI_COLOR_B_RED "\x1b[41m"
#define ANSI_COLOR_B_GREEN "\x1b[42m"
#define ANSI_COLOR_B_YELLOW "\x1b[43m"
#define ANSI_COLOR_GRAY "\x1b[90m"
#define ANSI_COLOR_RESET "\x1b[0m"
#define ANSI_RESET_ALL "\x1b[0m"

void inizializza_campo(int riga, int colonna, char mat[riga][colonna]);
void stampaCampo(int riga, int colonna, char mat[riga][colonna]);
int controlloVincita(char giocatore, int riga, int colonna, char mat[riga][colonna]);
int inserisciPallina(int col, char giocatore, int riga, int colonna, char mat[riga][colonna]);
void inserisciNomiGiocatori(char *giocatore1, char *giocatore2);
void cancellaSchermo();
void visualizzaRegole();
void visualizzaCrediti();


int main() {
  char campo[righe][colonne];
  char giocatore1[50] = "Rosso";
  char giocatore2[50] = "Giallo";
  int turno, scelta, colonna, altraPartita;

  while (1) {

    cancellaSchermo();

    printf(ANSI_COLOR_B_YELLOW "M" ANSI_COLOR_GREEN "e" ANSI_COLOR_RED "n"ANSI_COLOR_BLUE "u:" ANSI_COLOR_RESET"\n");
    printf(ANSI_COLOR_GREEN "1 - Inserisci nomi giocatori e inizia la partita\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_BLUE "2 - Regole\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_MAGENTA "3 - Crediti\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_ORANGE "4 - Uscita\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_RED "Scelta: " ANSI_COLOR_RESET);
    scanf("%d", &scelta);

    switch (scelta) {
    case 1:
        inserisciNomiGiocatori(giocatore1, giocatore2);
        break;
    case 2:
        visualizzaRegole();
        sleep(5);
        break;
    case 3:
        visualizzaCrediti();
        sleep(2);
    case 4:
        exit(0);
        default:
        printf(ANSI_COLOR_RED "Scelta non valida! Riprova!\n" ANSI_COLOR_RESET);
        sleep(2);
        break;
    }

    if (scelta == 1) {
      do {
        turno = 0; // 0: rosso, 1: giallo
        inizializza_campo(righe, colonne, campo);
        stampaCampo(righe, colonne, campo);

        while (1) {
          printf("Giocatore " ANSI_COLOR_B_GREEN "%s" ANSI_COLOR_RESET ", inserisci la colonna (0-6): ", turno == 0 ? giocatore1 : giocatore2);
          scanf("%d", &colonna);

          if (inserisciPallina(colonna, turno == 0 ? ROSSO : GIALLO, righe, colonne, campo)) {
            cancellaSchermo();
            stampaCampo(righe, colonne, campo);
            if (controlloVincita(turno == 0 ? ROSSO : GIALLO, righe, colonne, campo)) {
              printf(ANSI_COLOR_GREEN "Il giocatore %s ha vinto!\n" ANSI_COLOR_RESET, turno == 0 ? giocatore1 : giocatore2);
              break;
            }
            turno = (turno + 1) % 2;
          } else {
            printf(ANSI_COLOR_RED "Mossa non valida. Riprova.\n" ANSI_COLOR_RESET);
          }
        }

        printf(ANSI_COLOR_GREEN "Vuoi fare un'altra partita? (1 - Si, 0 - No): " ANSI_COLOR_RESET);
        scanf("%d", &altraPartita);
        if (altraPartita == 0) {
            exit(0);
        }
        cancellaSchermo();
      } while (altraPartita);
    }
  }
  return 0;
}

void inizializza_campo(int riga, int colonna, char mat[riga][colonna]) {
  int i, j;
  for (i = 0; i < riga; i++) {
    for (j = 0; j < colonna; j++) {
      mat[i][j] = VUOTO;
    }
  }
}

void stampaCampo(int riga, int colonna, char mat[riga][colonna]) {
  printf("\n");
  int i, j;
  for (i = 0; i < riga; i++) {
    printf("|");
    for (j = 0; j < colonna; j++) {
      if (mat[i][j] == ROSSO) {
        printf(ANSI_COLOR_RED "%c" ANSI_COLOR_RESET "|", mat[i][j]);
      } else if (mat[i][j] == GIALLO) {
        printf(ANSI_COLOR_YELLOW "%c" ANSI_COLOR_RESET "|", mat[i][j]);
      } else {
        printf("%c|", mat[i][j]);
      }
    }
    printf("\n");
  }
  printf("---------------\n");
}

int controlloVincita(char giocatore, int riga, int colonna, char mat[riga][colonna]) {
  // Controllo orizzontale
  int i, j, k;
  for (i = 0; i < righe; i++) {
    for (j = 0; j <= colonne - vincita; j++) {
      int c = 0;
      for (k = 0; k < vincita; k++) {
        if (mat[i][j + k] == giocatore) {
          c++;
        } else {
          c = 0;
        }
        if (c == vincita) {
          return 1;
        }
      }
    }
  }

  // Controllo verticale
  for (j = 0; j < colonne; j++) {
    for (i = 0; i <= righe - vincita; i++) {
      int c = 0;
      for (k = 0; k < vincita; k++) {
        if (mat[i + k][j] == giocatore) {
          c++;
        } else {
          c = 0;
        }
        if (c == vincita) {
          return 1;
        }
      }
    }
  }

  // Controllo diagonale (da sinistra a destra)
  for (i = 0; i <= righe - vincita; i++) {
    for (j = 0; j <= colonne - vincita; j++) {
      int c = 0;
      for (k = 0; k < vincita; k++) {
        if (mat[i + k][j + k] == giocatore) {
          c++;
        } else {
          c = 0;
        }
        if (c == vincita) {
          return 1;
        }
      }
    }
  }

  // Controllo diagonale (da destra a sinistra)
  for (i = 0; i <= righe - vincita; i++) {
    for (j = colonne - 1; j >= vincita - 1; j--) {
      int c = 0;
      for (k = 0; k < vincita; k++) {
        if (mat[i + k][j - k] == giocatore) {
          c++;
        } else {
          c = 0;
        }
        if (c == vincita) {
          return 1;
        }
      }
    }
  }
  return 0;
}

int inserisciPallina(int col, char giocatore, int riga, int colonna, char mat[riga][colonna]) {
  if (col < 0 || col >= colonna) {
    return 0;
  }
  int i;
  for (i = righe - 1; i >= 0; i--) {
    if (mat[i][col] == VUOTO) {
      mat[i][col] = giocatore;
      return 1;
    }
  }
  return 0;
}

void cancellaSchermo() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void inserisciNomiGiocatori(char *giocatore1, char *giocatore2) {
  printf(ANSI_COLOR_BLUE "Inserisci il nome del giocatore 1: " ANSI_COLOR_RESET);
  scanf("%s", giocatore1);
  printf(ANSI_COLOR_BLUE "Inserisci il nome del giocatore 2: " ANSI_COLOR_RESET);
  scanf("%s", giocatore2);
}

void visualizzaRegole() {
  printf("\nLe regole del gioco sono:\n\n");
  printf("- Il gioco viene giocato su una griglia verticale composta da 6 righe e 7 colonne.\n");
  printf("- I giocatori scelgono a turno una colonna in cui far cadere il proprio pezzo.\n");
  printf("- Lo scopo del gioco è allineare quattro pezzi dello stesso colore in orizzontale, verticale o diagonale.\n");
  printf("- Il primo giocatore a ottenere un allineamento di quattro pezzi vince la partita.\n\n");
}

void visualizzaCrediti() {
  printf("\nQuesto gioco è stato sviluppato da : \n\nMauro Retini Causa\nMarco Bruno\nDaniel Lombardo\nFrancesco Montagna.\n\nScarica il gioco da https://github.com/Videogamesmauro/Forza-4\n\n");
}
