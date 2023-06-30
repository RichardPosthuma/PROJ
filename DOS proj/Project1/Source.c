#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define NumCards 108
#define NumColors 4
#define NumValues 10
#define NumActions 2

typedef struct card_s {
    char color[10];
    int value;
    char action[15];
    struct card_s* pt;
    bool drawn;
} card;


card deck[NumCards];


//This function creates array for the deck of cards to be worked with
void initializeDeck() {
    int i, j, k;
    char colors[NumColors][10] = { "red", "blue", "green", "yellow" };
    char actions[NumActions][15] = { "#", "Multi Color 2" };
    k = 16;
    for (int t = 0; t < 12; t++) { // creates the wild 2 cards
        strcpy(deck[t].color, "any");
        deck[t].pt = NULL;
        deck[t].value = 2;
        strcpy(deck[t].action, actions[1]);
        deck[t].drawn = false;
    }
    for (int n = 12; n < 20; n++) {
        strcpy(deck[n].color, colors[n % 4]);
        deck[n].pt = NULL;
        deck[n].value = 11;
        strcpy(deck[n].action, actions[0]);
        deck[n].drawn = false;
    }
    int temp;
    int ref;
    for (i = 0; i < NumColors; i++) {
        for (j = 0; j < NumValues; j++) {
            if (j < 5 && j != 1) {
                temp = 3;
                ref = 3;
            }
            else if (j >= 5) {
                temp = 2;
            }
            if (temp == 2)
                if (ref == 3) {
                    k += temp + 1;
                    ref = 0;
                }
                else
                    k += temp;
            else
                if (j == 0 && i != 0)
                    k += temp - 1;
                else
                    k += temp;
            while (temp > 0) {
                strcpy(deck[k + temp].color, colors[i]);
                deck[k + temp].pt = NULL;
                deck[k + temp].value = j + 1;
                strcpy(deck[k + temp].action, "none");
                deck[k + temp].drawn = false;
                temp--;
            }


        }
    }
}

void read_file_dck(FILE* file_name) {

    char color[7];
    int num, i = 0;
    char act[10];

    while (fscanf(file_name, "%s %d %s", color, &num, act) != EOF) {
        deck[i].value = num;
        strcpy(deck[i].action, act);
        strcpy(deck[i].color, color);
        i++;
    }
}

void create_hand(int player_num, card* players) {
    int x = 0;
    card* temp, * curr_card;
    card* prev = NULL;
    for (int j = 0; j < 7; j++) {
        for (int i = 0; i < player_num; i++) {
            while (deck[x].drawn == true)
                x++;
            deck[x].drawn = true;
            curr_card = &deck[x];
            if (players[i].pt == NULL || curr_card->value <= players[i].pt->value) {
                curr_card->pt = players[i].pt;
                players[i].pt = curr_card;
            }
            else {
                temp = players[i].pt;
                while (temp->pt != NULL && temp->pt->value < curr_card->value) {
                    temp = temp->pt;
                }
                curr_card->pt = temp->pt;
                temp->pt = curr_card;
            }
        }

    }
}

void red() {
    printf("\033[1;31m");
}

void yellow() {
    printf("\033[1;33m");
}

void green() {
    printf("\033[1;32m");
}

void blue() {
    printf("\033[1;34m");
}

void normal() {
    printf("\033[0m");
}




void print_card(card curr_card) {
    char col = curr_card.color[0];
    int x = curr_card.value;
    switch (col) {
    case('r'):
        red();
        if (x > 10)
            printf("# ");
        else
            printf("%d ", x);
        normal();
        break;
    case('b'):
        blue();
        if (x > 10)
            printf("# ");
        else
            printf("%d ", x);
        normal();
        break;
    case('y'):
        yellow();
        if (x > 10)
            printf("# ");
        else
            printf("%d ", x);
        normal();
        break;
    case('g'):
        green();
        if (x > 10)
            printf("# ");
        else
            printf("%d ", x);
        normal();
        break;
    case('a'):
        printf("%d ", x);
        break;


    }
}

void player_num_str(int num, char* curr) {
    switch (num) {
    case(1):
        strcpy(curr, "one");
        break;
    case(2):
        strcpy(curr, "two");
        break;
    case(3):
        strcpy(curr, "three");
        break;
    case(4):
        strcpy(curr, "four");
        break;
    case(5):
        strcpy(curr, "five");
        break;
    case(6):
        strcpy(curr, "six");
        break;
    }
}
void remove_card(card** start, int target_pos) {
    card* current = *start;
    card* previous = NULL;
    int count = 0;
    if (current == NULL) {
        return;
    }
    if (current->pt == NULL && target_pos == 0) {
        *start = NULL;
        return;
    }
    while (current != NULL) {
        if (count == target_pos) {
            if (previous == NULL) {
                *start = current->pt;
            }
            else {
                previous->pt = current->pt;
            }
            return;
        }
        previous = current;
        current = current->pt;
        count++;
    }
}

void add_to_center(card* start, card* target) {
    card* current = start;
    while (current->pt != NULL) {
        current = current->pt;
    }
    current->pt = target;
    target->pt = NULL;
}

card* get_card(card curr_card, int card_pos) {
    card* pointer = curr_card.pt;
    for (int i = 0; i < card_pos; i++)
        pointer = pointer->pt;
    return pointer;

}

int num_cards(card player) {
    card* pointer = player.pt;
    int count = 0;
    while (pointer != NULL) {
        count += 1;
        pointer = pointer->pt;
    }
    return count;
}


void draw(card* player, int *curr_deck_pos) {
    card *temp;
    temp = player->pt;
    while (temp->pt != NULL) {
        temp = temp->pt;
    }
    while (deck[*curr_deck_pos].drawn == true)
        *curr_deck_pos = 1 + *curr_deck_pos;
    deck[*curr_deck_pos].drawn = true;
    temp->pt = &deck[*curr_deck_pos];
}

int play_game(int num_players) {
    card players[6];
    for (int index = 0; index < 6; index++) {
        players[index].pt = NULL;
    }
    int shuf_file, x, y, z, count, river_size = 2, curr_river, curr_deck_pos, saved = 0;
    bool game = true, river_change = false;
    char name[30];
    char curr_player[7];
    char garbage_grab;
    card garbage_col[15];
    card temp;
    card bruh;
    card* dummy, *dummy_clone, *dummy2;
    card* pointer, * center, * temp2;
    card *river = &bruh;
    FILE* file_name;
    printf("Press 1 to shuffle the DOS deck or 2 to load a deck from a file: ");
    scanf("%d", &shuf_file);
    if (shuf_file == 1) {
        initializeDeck();
        for (int run_time = 0; run_time < NumCards; run_time++) {
            x = run_time;
            y = rand() % 107;
            temp = deck[x];
            deck[x] = deck[y];
            deck[y] = temp;
        }
        for (int run_time = 0; run_time < NumCards; run_time++) {
            y = run_time;
            x = rand() % 107;
            temp = deck[x];
            deck[x] = deck[y];
            deck[y] = temp;
        }
        printf("The deck is shuffled. Press any key to deal cards ");
    }
    else {
        printf("Enter file name: ");
        scanf(" %s", name);
        file_name = fopen(name, "r");
        read_file_dck(file_name);
        int x;
        printf("The file has been read. Press any key to deal cards ");
    }
    scanf("%*c%c", &garbage_grab);
    if (garbage_grab) {
        create_hand(num_players, players);
        x = num_players * 7 - 1;
        *river = deck[x];
        deck[x].drawn = true;
        x += 1;
        curr_deck_pos = x;
        river->pt = &deck[x];
        deck[x].drawn = true;
    }
    while (game) {
        for (int turn = 0; turn < num_players; turn++) {
            curr_river = 0;
            
            if (river == NULL)
                river_size = 0;
            else
                river_size = num_cards(*river) + 1;
            while (river_size < 2) {
                while (deck[curr_deck_pos].drawn == true)
                    curr_deck_pos++;
                deck[curr_deck_pos].drawn = true;
                if (river == NULL)
                    river = &deck[curr_deck_pos];
                else
                    river->pt = &deck[curr_deck_pos];
                river_size++;
                if (river_size == 2)
                    river->pt->pt = NULL;
            }
            pointer = river;
            printf("Centerline ");
            while (pointer != NULL) {
                print_card(*pointer);
                pointer = pointer->pt;
            }
            printf("\n");
            player_num_str((turn + 1), curr_player);
            printf("Player's %s hand: ", curr_player);
            pointer = players[turn].pt;
            while (pointer != NULL) {
                print_card(*pointer);
                pointer = pointer->pt;
            }
            pointer = river;
            count = 0;
            river_size = num_cards(*river) + 1;
            while (count < river_size) {
                river_change = false;
                printf("\n");
                printf("How many cards do you want to play on ");
                print_card(*pointer);
                printf("(-1 to drawn new card): ");
                scanf("%d", &x);
                y = num_cards(players[turn]);
                if (x == 1) {
                    printf("Select one card 1-%d: ", y);
                    scanf("%d", &x);
                    dummy = get_card(players[turn], x - 1);
                    if (pointer->value == dummy->value || pointer->value == 11 || dummy->value == 11) {
                        remove_card(&players[turn].pt, x - 1);
                        if (strcmp(pointer->color, dummy->color) == 0) {
                            printf("You color matched and get to play 1 card(s) on the center row.\n");
                            printf("Player's %s hand: ", curr_player);
                            temp2 = players[turn].pt;
                            while (temp2 != NULL) {
                                print_card(*temp2);
                                temp2 = temp2->pt;
                            }
                            printf("\n");
                            y = num_cards(players[turn]);
                            printf("Select one additional card from 1-%d to place to the center row: ", y);
                            scanf("%d", &x);
                            dummy = get_card(players[turn], x - 1);
                            center = &river;
                            if (curr_river == 0)
                                river_change = true;
                            dummy_clone = &garbage_col[saved];
                            dummy_clone->pt = NULL;
                            dummy_clone->value = dummy->value;
                            strcpy(dummy_clone->color, dummy->color);
                            strcpy(dummy_clone->action, dummy->action);
                            dummy_clone->drawn = dummy->drawn;
                            saved++;
                            add_to_center(river, dummy_clone);
                            remove_card(center, curr_river-1);
                            remove_card(&players[turn].pt, x - 1);
                        }
                        else {
                            center = &river;
                            if (curr_river == 0) {
                                pointer = river->pt;
                                river_change = true;
                                remove_card(center, curr_river);
                                printf("Player's %s hand: ", curr_player);
                                temp2 = players[turn].pt;
                                while (temp2 != NULL) {
                                    print_card(*temp2);
                                    temp2 = temp2->pt;
                                }
                            }
                            else {
                                remove_card(center, curr_river);
                                printf("Player's %s hand: ", curr_player);
                                temp2 = players[turn].pt;
                                while (temp2 != NULL) {
                                    print_card(*temp2);
                                    temp2 = temp2->pt;
                                }
                            }
                        }

                    }


                }

                else if (x == 2) {
                    printf("Select two card 1-%d: ", y);
                    scanf("%d %d", &x, &z);
                    dummy = get_card(players[turn], x - 1);
                    dummy2 = get_card(players[turn], z - 1);
                    if (pointer->value == dummy->value + dummy2->value || pointer->value == 11 || dummy->value == 11 || dummy2->value == 11) {
                        remove_card(&players[turn].pt, x - 1);
                        remove_card(&players[turn].pt, z - 2);
                        if ((strcmp(pointer->color, dummy->color) == 0 || dummy->value == 2) && (strcmp(pointer->color, dummy2->color) == 0) || dummy2->value == 2) {
                            printf("Dealing 2 additional card(s) to other players.\n");
                            for (int p = 0; p < num_players; p++) {
                                if (p != turn) {
                                    draw(&players[p], &curr_deck_pos);
                                }

                            }
                            printf("Player's %s hand: ", curr_player);
                            temp2 = players[turn].pt;
                            while (temp2 != NULL) {
                                print_card(*temp2);
                                temp2 = temp2->pt;
                            }
                            y = num_cards(players[turn]);
                            printf("\n");
                            printf("Select one additional card from 1-%d to place to the center row: ", y);
                            scanf("%d", &x);
                            dummy = get_card(players[turn], x - 1);
                            center = &river;
                            remove_card(&players[turn].pt, x - 1);
                            if (curr_river == 0)
                                river_change = true;
                            if (x == 1) {
                                dummy_clone = &garbage_col[saved];
                                dummy_clone->pt = NULL;
                                dummy_clone->value = dummy->value;
                                strcpy(dummy_clone->color, dummy->color);
                                strcpy(dummy_clone->action, dummy->action);
                                dummy_clone->drawn = dummy->drawn;
                                saved++;
                                remove_card(center, curr_river-1);
                                add_to_center(center, dummy_clone);
                            }
                            else {
                                add_to_center(river, dummy);
                                remove_card(center, curr_river);

                            }


                        }
                        else {
                            center = &river;
                            if (curr_river == 0) {
                                pointer = river->pt;
                                river_change = true;
                                printf("Player's %s hand: ", curr_player);
                                temp2 = players[turn].pt;
                                while (temp2 != NULL) {
                                    print_card(*temp2);
                                    temp2 = temp2->pt;
                                }
                                remove_card(center, curr_river);
                            }
                            else {
                                remove_card(center, curr_river);
                                printf("Player's %s hand: ", curr_player);
                                temp2 = players[turn].pt;
                                while (temp2 != NULL) {
                                    print_card(*temp2);
                                    temp2 = temp2->pt;
                                }
                            }


                        }
                    }


                }
                else if (x == -1) {
                    draw(&players[turn], &curr_deck_pos);
                    count = river_size;
                }
                printf("\n");
                curr_river += 1;
                if (river_change == false)
                    pointer = pointer->pt;
                count += 1;
                int tun_acr = turn + 1;
                if (num_cards(players[turn]) == 0) {
                    printf("Player %d wins!", tun_acr);
                    count = river_size;
                    game = false;
                }
            }

        }
    }
    return 0;
}

int main(void) {
    for (int i = 0; i < NumCards; i++) {
        deck[i].pt = (struct card*)malloc(sizeof(card));
    }
    int player_num;
    printf("Enter number of players (2-6): ");
    scanf("%d", &player_num);
    play_game(player_num);
    return 0;
}