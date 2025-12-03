#include <stdio.h>
#include <stdlib.h>
#include <time.h>
char *suits[] = {"♥", "♦", "♣", "♠"};
void draw_card_art(int value, int suit) {
    char v[3];
    if (value == 1) sprintf(v, "A");
    else if (value == 11) sprintf(v, "J");
    else if (value == 12) sprintf(v, "Q");
    else if (value == 13) sprintf(v, "K");
    else sprintf(v, "%d", value);
    printf("+-----+\n");
    printf("| %-3s |\n", v);
    printf("|     |\n");
    printf("|  %s  |\n", suits[suit]);
    printf("+-----+\n");
}
int draw_card_value() {
    int r = rand() % 13 + 1;
    if (r > 10) return 10;
    return r;
}

int draw_raw() {
    return rand() % 13 + 1;
}

int draw_suit() {
    return rand() % 4;
}

int hand_value(int cards[], int count) {
    int total = 0, aces = 0;
    for (int i = 0; i < count; i++) {
        int v = cards[i];
        if (v > 10) v = 10;
        if (v == 1) aces++;
        total += v;
    }
    while (aces > 0 && total + 10 <= 21) {
        total += 10;
        aces--;
    }
    return total;
}

int main() {
    srand(time(NULL));

    int balance = 1000;
    int choice;

    while (1) {
        printf("\n--- MENU ---\n");
        printf("1. Play Blackjack\n");
        printf("2. Check Balance\n");
        printf("3. Quit\n");
        printf("Choose: ");
        scanf("%d", &choice);
        if (choice == 2) {
            printf("\nYour balance: %d coins\n", balance);
            continue;
        }
        if (choice == 3) {
            printf("Goodbye!\n");
            break;
        }
        if (choice != 1) {
            printf("Invalid option\n");
            continue;
        }

        if (balance <= 0) {
            printf("You are out of coins.\n");
            break;
        }
        int bet;
        printf("Your balance: %d\n", balance);
        printf("Enter bet: ");
        scanf("%d", &bet);
        if (bet <= 0 || bet > balance) {
            printf("Invalid bet\n");
            continue;
        }
        int player_val[10], dealer_val[10], player_suit[10], dealer_suit[10];
        int p = 0, d = 0;
        char move;
        int r = draw_raw(); player_val[p] = r; player_suit[p++] = draw_suit();
        r = draw_raw(); dealer_val[d] = r; dealer_suit[d++] = draw_suit();
        r = draw_raw(); player_val[p] = r; player_suit[p++] = draw_suit();
        r = draw_raw(); dealer_val[d] = r; dealer_suit[d++] = draw_suit();
        printf("\nDealer shows:\n");
        draw_card_art(dealer_val[1], dealer_suit[1]);
        printf("Your cards:\n");
        for (int i = 0; i < p; i++) draw_card_art(player_val[i], player_suit[i]);
        printf("Your total: %d\n", hand_value(player_val, p));
        while (1) {
            printf("Hit or Stand? (h/s): ");
            scanf(" %c", &move);
            if (move == 'h') {
                r = draw_raw();
                player_val[p] = r;
                player_suit[p] = draw_suit();
                p++;
                printf("You drew:\n");
                draw_card_art(player_val[p - 1], player_suit[p - 1]);
                if (hand_value(player_val, p) > 21) {
                    printf("Bust! Lost %d coins.\n", bet);
                    balance -= bet;
                    goto show_balance;
                }
            } else break;
        }
        printf("\nDealer's turn:\n");
        for (int i = 0; i < d; i++) draw_card_art(dealer_val[i], dealer_suit[i]);
        while (hand_value(dealer_val, d) < 17) {
            r = draw_raw();
            dealer_val[d] = r;
            dealer_suit[d] = draw_suit();
            d++;
            printf("Dealer draws:\n");
            draw_card_art(dealer_val[d - 1], dealer_suit[d - 1]);
        }
        int pv = hand_value(player_val, p);
        int dv = hand_value(dealer_val, d);
        printf("Your total: %d\n", pv);
        printf("Dealer total: %d\n", dv);
        if (dv > 21) {
            printf("Dealer busts! +%d coins\n", bet);
            balance += bet;
        } else if (pv > dv) {
            printf("You win +%d coins\n", bet);
            balance += bet;
        } else if (pv < dv) {
            printf("Dealer wins -%d coins\n", bet);
            balance -= bet;
        } else {
            printf("Tie. Bet returned.\n");
        }
        show_balance:
        printf("\nYour balance is now: %d coins\n", balance);
        if (balance <= 0) {
            printf("You have no coins left.\n");
            break;
        }
    }
    return 0;
}
