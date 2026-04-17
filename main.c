#include <stdio.h>
#include <time.h>
#include <stdlib.h>

// Funzione per il calcolo della vittoria
int calcola_vittoria(int dealersum, int somma, int puntata, int budget) {
    if (somma > 21) {
        printf("\nHai perso %d punti perché hai sballato.\n", puntata);
        return budget;
    }

    while (dealersum < 17) {
        dealersum += (rand() % 10) + 1;
    }

    printf("\nIl dealer si ferma a: %d. Tu hai: %d.\n", dealersum, somma);

    if (dealersum > 21) {
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
    char scelta, scelta_carta;
    
    srand(time(NULL));
    budget = 10;

    printf("====BLACK-JACK====\n");

    do {
        printf("==================\n");
        printf("Il tuo budget ammonta a %d punti.\n", budget);

        do {
            printf("Quanto vuoi puntare? ");
            scanf("%d", &puntata);
            if (puntata > budget) printf("Non hai abbastanza punti!\n");
        } while (puntata > budget || puntata <= 0);
    
        budget = budget - puntata;

        carta1 = (rand() % 11) + 1;
        carta2 = (rand() % 11) + 1;

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

        while (somma < 21)
        {
            printf("Vuoi un altra carta? (y/n)");
            scanf(" %c", &scelta_carta);

            if (scelta_carta == 'y')
            {
                int nuova_carta = (rand() % 11) + 1;
                somma += nuova_carta;
                printf("Hai pescato %d. Nuova somma: %d", nuova_carta, somma);
            } else {
                break;
            }
        }
        
        if (somma > 21) {
            printf("Sballato! Hai superato 21.\n");
        }

        dealersum = (rand() % 11) + 10; 

        budget = calcola_vittoria(dealersum, somma, puntata, budget);
        
        printf("Budget attuale: %d\n", budget);

        if (budget <= 0) {
            printf("Hai esaurito il budget!\n");
            break;
        }

        do {
            printf("Vuoi giocare di nuovo? (y/n): ");
            scanf(" %c", &scelta); 
        } while (scelta != 'y' && scelta != 'n');

    } while (scelta == 'y' && budget > 0);

    printf("Grazie per aver giocato con noi!\n");

    return 0;
}