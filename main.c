#include <stdio.h>
#include <time.h>
#include <stdlib.h>

// La funzione ora restituisce il budget aggiornato correttamente
int calcola_vittoria(int dealersum, int somma, int puntata, int budget) {
    // Logica pescata dealer
    if (dealersum < 14) {
        dealersum += (rand() % 10) + 1;
    }

    printf("\nIl dealer ha: %d. Tu hai: %d.\n", dealersum, somma);

    if (somma > 21) {
        printf("Hai sballato! Perdi %d punti.\n", puntata);
        // Il budget è già stato scalato al momento della puntata
    } 
    else if (dealersum > 21) {
        printf("Il dealer ha sballato! Hai vinto %d punti.\n", puntata * 2);
        budget += (puntata * 2);
    } 
    else if (dealersum > somma) {
        printf("Il dealer ha vinto! Hai perso la tua puntata.\n");
    } 
    else if (dealersum < somma) {
        printf("Hai vinto! Hai guadagnato %d punti.\n", puntata * 2);
        budget += (puntata * 2);
    } 
    else {
        printf("Pareggio (Push)! Ti vengono restituiti %d punti.\n", puntata);
        budget += puntata;
    }

    return budget;
}

int main() {
    int carta1, carta2, somma, asso, dealersum, budget, puntata;
    char scelta;
    
    srand(time(NULL)); // Inizializzato una sola volta all'inizio
    budget = 10;

    printf("====BLACK-JACK====\n");

    do {
        printf("==================\n");
        printf("Il tuo budget ammonta a %d punti.\n", budget);

        // Controllo puntata
        do {
            printf("Quanto vuoi puntare? ");
            scanf("%d", &puntata);
            if (puntata > budget) printf("Non hai abbastanza punti!\n");
        } while (puntata > budget || puntata <= 0);
    
        budget = budget - puntata;

        // Generazione carte (1-11)
        carta1 = (rand() % 11) + 1;
        carta2 = (rand() % 11) + 1;

        printf("Le tue carte sono: %d e %d.\n", carta1, carta2);

        // Gestione Asso
        if (carta1 == 11 || carta2 == 11) {
            printf("Hai trovato un asso! Vuoi farlo valere 1 o 11? ");
            scanf("%d", &asso);
            if (asso == 1) {
                if (carta1 == 11) carta1 = 1;
                else if (carta2 == 11) carta2 = 1;
            }
        }
    
        somma = carta1 + carta2;
        printf("Le tue carte sono: %d e %d. La cui somma equivale a %d\n", carta1, carta2, somma);

        // Generazione punteggio dealer
        dealersum = (rand() % 11) + 10; 

        // Una sola chiamata a calcola_vittoria
        budget = calcola_vittoria(dealersum, somma, puntata, budget);
        
        printf("Budget attuale: %d\n", budget);

        if (budget <= 0) {
            printf("Hai esaurito il budget!\n");
            break;
        }

        // Ciclo per scelta giocare ancora (con spazio in scanf per pulire il buffer)
        do {
            printf("Vuoi giocare di nuovo? (y/n): ");
            scanf(" %c", &scelta); 
        } while (scelta != 'y' && scelta != 'n');

    } while (scelta == 'y' && budget > 0);

    printf("Grazie per aver giocato con noi!\n");

    return 0;
}