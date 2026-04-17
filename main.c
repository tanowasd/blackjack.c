#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// --- LOGICA DI GIOCO ---
int calcola_vittoria_grafico(int dealersum, int somma, int puntata, int* budget, char* messaggio) {
    if (somma > 21) {
        sprintf(messaggio, "SBALLATO! Hai perso %d punti.", puntata);
        return 0; 
    }
    while (dealersum < 17) {
        dealersum += (rand() % 10) + 1;
    }
    if (dealersum > 21 || somma > dealersum) {
        sprintf(messaggio, "VINTO! Il dealer ha %d. Guadagni %d.", dealersum, puntata * 2);
        *budget += (puntata * 2);
    } 
    else if (dealersum > somma) {
        sprintf(messaggio, "PERSO! Il dealer ha %d.", dealersum);
    } 
    else {
        sprintf(messaggio, "PAREGGIO! Il dealer ha %d. Riprendi %d.", dealersum, puntata);
        *budget += puntata;
    }
    return dealersum;
}

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Blackjack - Slider Puntata");
    SetTargetFPS(60);
    srand(time(NULL));

    int budget = 100;
    int puntata = 10;
    int sommaGiocatore = 0;
    int dealersum = 0;
    bool turnoFinito = false;
    bool partitaInCorso = false;
    char messaggioEsito[100] = "Trascina la barra per puntare e premi Nuova Mano";

    // --- NOTE: CUSTOMIZZAZIONE SLIDER ---
    Rectangle sliderBar = { 500, 50, 250, 10 }; // Posizione e lunghezza barra
    float sliderPos = 550; // Posizione iniziale del cursore (X)
    bool dragging = false;

    Rectangle btnNuovaMano = { 50, 500, 150, 50 };
    Rectangle btnHit = { 250, 500, 150, 50 };
    Rectangle btnStand = { 450, 500, 150, 50 };

    while (!WindowShouldClose()) {
        Vector2 mousePos = GetMousePosition();

        // --- LOGICA SLIDER ---
        if (!partitaInCorso || turnoFinito) {
            // Inizia trascinamento
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && 
                CheckCollisionPointCircle(mousePos, (Vector2){sliderPos, sliderBar.y + 5}, 15)) {
                dragging = true;
            }
            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) dragging = false;

            if (dragging) {
                sliderPos = mousePos.x;
                // Limiti della barra
                if (sliderPos < sliderBar.x) sliderPos = sliderBar.x;
                if (sliderPos > sliderBar.x + sliderBar.width) sliderPos = sliderBar.x + sliderBar.width;

                // Calcolo puntata proporzionale: (Posizione attuale / Lunghezza totale) * Budget
                float percentuale = (sliderPos - sliderBar.x) / sliderBar.width;
                puntata = (int)(percentuale * budget);
                if (puntata < 1) puntata = 1; // Puntata minima 1
            }
        }

        // --- LOGICA BOTTONI ---
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (CheckCollisionPointRec(mousePos, btnNuovaMano) && (!partitaInCorso || turnoFinito) && budget > 0) {
                budget -= puntata;
                sommaGiocatore = ((rand() % 10) + 1) + ((rand() % 10) + 1);
                dealersum = (rand() % 10) + 1;
                sprintf(messaggioEsito, "Partita avviata! Puntata: %d", puntata);
                partitaInCorso = true;
                turnoFinito = false;
            }
            if (partitaInCorso && !turnoFinito && CheckCollisionPointRec(mousePos, btnHit)) {
                sommaGiocatore += (rand() % 10) + 1;
                if (sommaGiocatore > 21) {
                    calcola_vittoria_grafico(0, sommaGiocatore, puntata, &budget, messaggioEsito);
                    turnoFinito = true;
                }
            }
            if (partitaInCorso && !turnoFinito && CheckCollisionPointRec(mousePos, btnStand)) {
                dealersum = calcola_vittoria_grafico(dealersum, sommaGiocatore, puntata, &budget, messaggioEsito);
                turnoFinito = true;
            }
        }

        BeginDrawing();
            ClearBackground(DARKGREEN);

            // Disegno Slider
            DrawText("SCOMMESSA", sliderBar.x, sliderBar.y - 25, 15, GOLD);
            DrawRectangleRec(sliderBar, LIGHTGRAY); // Barra sfondo
            // --- NOTE: CUSTOMIZZAZIONE CURSORE ---
            DrawCircle(sliderPos, sliderBar.y + 5, 12, dragging ? RED : MAROON); 
            
            // UI Testi
            DrawText(TextFormat("BUDGET: %d", budget), 20, 20, 25, GOLD);
            DrawText(TextFormat("PUNTATA ATTUALE: %d", puntata), 500, 80, 20, WHITE);
            DrawText(messaggioEsito, screenWidth/2 - MeasureText(messaggioEsito, 20)/2, 300, 20, RAYWHITE);

            if (partitaInCorso) {
                DrawText(TextFormat("TU: %d", sommaGiocatore), screenWidth/2 - 50, 400, 40, WHITE);
                if (turnoFinito) DrawText(TextFormat("BANCO: %d", dealersum), screenWidth/2 - 70, 150, 30, LIGHTGRAY);
                else DrawText("BANCO: ?", screenWidth/2 - 70, 150, 30, LIGHTGRAY);
            }

            // Disegno Bottoni
            DrawRectangleRec(btnNuovaMano, CheckCollisionPointRec(mousePos, btnNuovaMano) ? ORANGE : DARKGRAY);
            DrawText("NUOVA MANO", btnNuovaMano.x + 15, btnNuovaMano.y + 15, 18, WHITE);

            if (partitaInCorso && !turnoFinito) {
                DrawRectangleRec(btnHit, CheckCollisionPointRec(mousePos, btnHit) ? LIGHTGRAY : GRAY);
                DrawText("HIT", btnHit.x + 55, btnHit.y + 15, 20, BLACK);
                DrawRectangleRec(btnStand, CheckCollisionPointRec(mousePos, btnStand) ? LIGHTGRAY : GRAY);
                DrawText("STAND", btnStand.x + 45, btnStand.y + 15, 20, BLACK);
            }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}