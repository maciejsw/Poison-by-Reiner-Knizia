#ifdef _MSC_VER
#pragma warning(disable:4996)
#endif
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <ctime>
using namespace std;

#define max_cards 129
#define max_players 6
#define max_value 99
#define max_colors 7

enum colors {
	green,
	blue,
	red,
	violet,
	yellow,
	white,
	black,
};

struct card {
	int value;
	int color;
};

card* create_card(int value, int color) {
	card* new_card = (card*)malloc(sizeof(card)+1); //create card
	new_card->value = value;
	new_card->color = color;

	return new_card;
}

void clear_deck(card* cards[], int deck_size) {
	for (int i = 0; i < deck_size; i++) { //empty cards array
		cards[i] = create_card(0, 0);
	}
}


void create_deck(int values[], card* cards[], int kettles, int gree, int green_value, int others) { //creating deck from classic input
	for (int i = 0; i < gree; i++) cards[i] = create_card(green_value, green);
	for (int i = 0; i < kettles ; i++) {
		int k = 0;
		for (int j = gree + (others * i); j < (gree + others + (others * i)); j++) {
			cards[j] = create_card(values[k], i+1);
			k++;
		} 

	}
}

void print_deck(card** cards, int gree, int kettles, int others) { //printing cards, not used
	char list[8][7] = {"green", "blue", "red", "violet", "yellow", "white", "black" };
	for (int i = 0; i <(gree + others*kettles); i++) cout << cards[i]->value << " " << list[cards[i]->color] << " ";
}

void shuffle(card** cards, int deck_size) { //casual shuffling using rand
	srand(time(NULL));
	for (int j = 0; j < 7; j++){
		for (int i = 0; i < deck_size; i++) {
			card* temp = cards[i];
			int temp2 = rand() % (deck_size - i) + i;
			cards[i] = cards[temp2];
			cards[temp2] = temp;
		}
	}
}

void deal_cards(card** cards, int players, card*** player_cards, int deck_size) { //dealing the cards to players
	for (int i = 0; i < players; i++) {
		int k = 0;
		for (int j = i; j < deck_size; j += players) {
			player_cards[i][k] = cards[j];
			k++;
		}

	}
}

void print_game(card*** player_cards, card*** player_deck, card** piles_cards[], int piles, int players, FILE* file) {
	char list[8][7] = { "green", "blue", "red", "violet", "yellow", "white", "black" };

	for (int i = 0; i < players; i++) { 
		fprintf(file, "\n%d player hand cards: ", i+1);
		cout << endl;
		cout << i+1 <<" "<< "player hand cards: ";
		for (int j = 0; j < max_cards; j++) {
			if ((player_cards[i][j]->value) == 0) break;
			fprintf(file, "%d %s ", player_cards[i][j]->value, list[player_cards[i][j]->color]);
			cout << player_cards[i][j]->value << " " << list[player_cards[i][j]->color] << " ";
		}
		fprintf(file, "\n%d player deck cards: ", i + 1);
		cout << endl;
		cout << i + 1 << " " << "player deck cards:" << " ";
		for (int j = 0; j < max_cards; j++) {
			if ((player_deck[i][j]->value) == 0) break;
			fprintf(file, "%d %s ", player_deck[i][j]->value, list[player_deck[i][j]->color]);
			cout << player_deck[i][j]->value << " " << list[player_deck[i][j]->color] << " ";
		}
	}
	for (int i = 0; i < piles; i++) {
		fprintf(file, "\n%d pile cards: ", i + 1);
		cout << endl;
		cout << i + 1 << " pile cards: ";
		
		for (int j = 0; j < max_cards; j++) {
			if ((piles_cards[i][j]->value) == 0) break;
			fprintf(file, "%d %s ", piles_cards[i][j]->value, list[piles_cards[i][j]->color]);
			cout << piles_cards[i][j]->value << " " << list[piles_cards[i][j]->color] << " ";
		}
	}
}

int is_letter(char a) {
	if (a >= 'A' && a <= 'Z')	return 1;
	if (a >= 'a' && a <= 'z')	return 1;
	return 0;
}

int is_number(char a) {
	if ((a >= '0' && a <= '9'))	return 1;
	return 0;
}

int game_init(FILE* file) {
	char buffer[30]; //reading one of first three lines
	char temp[7];
	int i, ch;
	for (i = 0; ((ch = fgetc(file)) != EOF) && (ch != '\n'); i++)
	{
		buffer[i] = (char)ch;
	}
	buffer[i] = '\0';
	int k = 0;
	int p = 0;
	while (buffer[k] != '\0') {
		while (!is_number(buffer[k])) k++;
		while (is_number(buffer[k])) {
			temp[p] = buffer[k];
			p++;
			k++;
		}
		temp[p] = '\0';
		return atoi(temp);

	}
	return -1;
	
}

void load_game(int players, card** player_cards[], card** player_deck[], int colors_number[], FILE* file) { //loading players
	char buffer[2000];
	char temp[7];
	int i, ch;
	int pl = -1;
	int cards = 0;
	for (int j = 0; j < (2 * players); j++) { //loading one line
		for (i = 0; ((ch = fgetc(file)) != EOF) && (ch != '\n'); i++)
		{
			buffer[i] = (char)ch;
		}
		buffer[i] = '\0';

		int k = 0; //iterative variables
		int p = 0;
		cards = 0;
		
		if (j % 2 == 0) { //insert to the hand
			pl++;
			
			while (buffer[k] != ':')k++; //go all the way to ':'
			k++;

			while (buffer[k] != '\0') {
				while (buffer[k] == ' ') k++;
				if (buffer[k] == '\0') break;
				p = 0;
				temp[0] = '\0';
				while (is_number(buffer[k])) { //i see number, insert it to the card
					temp[p] = buffer[k];
					p++;
					k++;
				}
				temp[p] = '\0';
				int temp_value = atoi(temp);
				(player_cards[pl][cards]->value) = temp_value;
				p = 0;
				while (buffer[k] == ' ') k++;
				while (is_letter(buffer[k])) { //i see letter, put the color to the card
					temp[p] = buffer[k];
					p++;
					k++;
				}
				temp[p] = '\0';

				switch (temp[0]) {
				case 'g': 
					player_cards[pl][cards]->color = green;
					break;
				case 'r':
					player_cards[pl][cards]->color = red;
					break;
				case 'y':
					player_cards[pl][cards]->color = yellow;
					break;
				case 'v':
					player_cards[pl][cards]->color = violet;
					break;
				case 'w':
					player_cards[pl][cards]->color = white;
					break;
				case 'b':
					if (temp[2] == 'u') player_cards[pl][cards]->color = blue;
					else player_cards[pl][cards]->color = black;
					break;
				}
				cards++;

			}
			k = 1;
			//int cards = 0; //for cards counting
			while (buffer[k] != '\0') { //counting the occurrences of the colors
				//if (is_number(buffer[k]) && (!is_number(buffer[k - 1]))) cards++; //as above
				if (is_letter(buffer[k]) && buffer[k - 1] == ' ') {
					switch (buffer[k]) {
					case 'g':	
						colors_number[green]++;
						break;
					case 'r':
						colors_number[red]++;
						break;
					case 'y':
						colors_number[yellow]++;
						break;
					case 'v':
						colors_number[violet]++;
						break;
					case 'w':
						colors_number[white]++;
						break;
					case 'b':
						if (buffer[k + 2] == 'u') colors_number[blue]++;
						else colors_number[black]++;
						break;
					}

				}
				k++;
			}
		}
		else { //insert to the deck, as above
			while (buffer[k] != ':')k++;
			k++;
			while (buffer[k] != '\0') {
				while (buffer[k] == ' ') k++;
				if (buffer[k] == '\0') break;
				p = 0;
				temp[0] = '\0';
				while (is_number(buffer[k])) {
					temp[p] = buffer[k];
					p++;
					k++;
				}
				temp[p] = '\0';
				int temp_value = atoi(temp);
				(player_deck[pl][cards]->value) = temp_value;
				p = 0;
				while (buffer[k] == ' ') k++;
				while (is_letter(buffer[k])) {
					temp[p] = buffer[k];
					p++;
					k++;
				}
				temp[p] = '\0';

				switch (temp[0]) {
				case 'g':
					player_deck[pl][cards]->color = green;
					break;
				case 'r':
					player_deck[pl][cards]->color = red;
					break;
				case 'y':
					player_deck[pl][cards]->color = yellow;
					break;
				case 'v':
					player_deck[pl][cards]->color = violet;
					break;
				case 'w':
					player_deck[pl][cards]->color = white;
					break;
				case 'b':
					if (temp[2] == 'u') player_deck[pl][cards]->color = blue;
					else player_deck[pl][cards]->color = black;
					break;
				}
				cards++;

			}
			k = 1;
			//int cards = 0;
			while (buffer[k] != '\0') {
				//if (is_number(buffer[k]) && (!is_number(buffer[k - 1]))) cards++;
				if (is_letter(buffer[k]) && buffer[k - 1] == ' ') {
					switch (buffer[k]) {
					case 'g':
						colors_number[green]++;
						break;
					case 'r':
						colors_number[red]++;
						break;
					case 'y':
						colors_number[yellow]++;
						break;
					case 'v':
						colors_number[violet]++;
						break;
					case 'w':
						colors_number[white]++;
						break;
					case 'b':
						if (buffer[k + 2] == 'u') colors_number[blue]++;
						else colors_number[black]++;
						break;
					}

				}
				k++;
			}
		}
		//player_cards[j] = cards; //for saving the cards number
	}
	
}

void load_piles(int piles, card** piles_cards[], int colors_number[], FILE* file) { //read piles

	char buffer[2000];
	int i, ch;
	char temp[7];
	
	for (int j = 0; j < piles; j++) { // insert cards to the piles, as in load_game
		for (i = 0; ((ch = fgetc(file)) != EOF) && (ch != '\n'); i++)
		{
			buffer[i] = (char)ch;
		}
		buffer[i] = '\0';

		int k = 0;
		int p = 0;
		int cards = 0;

		while (buffer[k] != ':')k++;
		k++;
		
		while (buffer[k] != '\0') {
			while (buffer[k] == ' ')k++;
			if (buffer[k] == ' ')break;
			p = 0;
			temp[0] = '\0';
			while (is_number(buffer[k])) {
				temp[p] = buffer[k];
				p++;
				k++;
			}
			temp[p] = '\0';
			int temp_value = atoi(temp);
			piles_cards[j][cards]->value = temp_value;
			p = 0;
			while (buffer[k] == ' ') k++;
			while (is_letter(buffer[k])) {
				temp[p] = buffer[k];
				p++;
				k++;
			}
			temp[p] = '\0';

			switch (temp[0]) {
			case 'g':
				piles_cards[j][cards]->color = green;
				break;
			case 'r':
				piles_cards[j][cards]->color = red;
				break;
			case 'y':
				piles_cards[j][cards]->color = yellow;
				break;
			case 'v':
				piles_cards[j][cards]->color = violet;
				break;
			case 'w':
				piles_cards[j][cards]->color = white;
				break;
			case 'b':
				if (temp[2] == 'u') piles_cards[j][cards]->color = blue;
				else piles_cards[j][cards]->color = black;
				break;
			}
			
			cards++;
		}
		k = 1;
		//int cards = 0;
		while (buffer[k] != '\0') {
			//if (is_number(buffer[k]) && (!is_number(buffer[k - 1]))) cards++;
			if (is_letter(buffer[k]) && buffer[k - 1] == ' ') {
				switch (buffer[k]) {
				case 'g':
					colors_number[green]++;
					break;
				case 'r':
					colors_number[red]++;
					break;
				case 'y':
					colors_number[yellow]++;
					break;
				case 'v':
					colors_number[violet]++;
					break;
				case 'w':
					colors_number[white]++;
					break;
				case 'b':
					if (buffer[k + 2] == 'u') colors_number[blue]++;
					else colors_number[black]++;
					break;
				}
			}
			k++;
		}
		//player_cards[j] = cards;
	}	
}

void print_players_numbers(int players, int cards[]) { //print info about the number of player's cards
	int k = 1;
	for (int i = 0; i < 2 * players; i += 2) {
		cout << k << " " << "player has " << cards[i] << " " << "cards on hand" << endl;
		cout << k << " " << "player has " << cards[i + 1] << " " << "cards in front of him" << endl;
		k++;
	}
}

void print_piles(int piles, int cards[]) { //print the number of cards on the pile
	for (int i = 0; i < piles; i++) {
		cout << "there are " << cards[i] << " cards on " << i + 1 << " pile" << endl;
	}
}

void print_colors(int colors_number[]) { //print info about the number of the cards of given color
	int is_equal = 1;
	char list[8][7] = { "green", "blue", "red", "violet", "yellow", "white", "black" };
	for (int i = 1; i < 6; i++) {
		if ((colors_number[i] != colors_number[i + 1]) && (colors_number[i] != 0 && colors_number[i + 1] != 0)) {
			is_equal = 0;
			break;
		}
	}
	if (is_equal) cout << "The number cards of all colors is equal: " << colors_number[1] << endl;
	else {
		cout << "At least two colors with a different number of cards were found:" << endl;
		for (int i = 1; i < 7; i++) {
			if (colors_number[i] > 0) cout << list[i] << " cards are " << colors_number[i] << endl;
		}
	}
}

int check_green(card** player_cards[], card** player_deck[], card** piles_cards[], int players, int piles) { //check green cards
	int green_val = 0;

	for (int i = 0; i < players; i++) { // check hands
		for (int j = 0; j < max_cards; j++) {
			if (player_cards[i][j] -> value == 0) break;
			if (player_cards[i][j]->color == green) {
				if (green_val == 0) green_val = player_cards[i][j]->value;
				else if (green_val != player_cards[i][j]->value) {
					return -1;
				}
			}
		}
	}
	for (int i = 0; i < players; i++) { // check decks
		for (int j = 0; j < max_cards; j++) {
			if (player_deck[i][j]->value == 0) break;
			if (player_deck[i][j]->color == green) {
				if (green_val == 0) green_val = player_deck[i][j]->value;
				else if (green_val != player_deck[i][j]->value) {
					return -1;
				}
			}
		}
	}
	for (int i = 0; i < piles; i++) { // check piles
		for (int j = 0; j < max_cards; j++) {
			if (piles_cards[i][j]->value == 0) break;
			if (piles_cards[i][j]->color == green) {
				if (green_val == 0) green_val = piles_cards[i][j]->value;
				else if (green_val != piles_cards[i][j]->value) {
					return -1;
				}
			}
		}
	}
	return green_val;
}

void print_green(int colors_number[], int green_value, FILE* file) { //print info about green cards
	if (green_value == -1) {
		cout << "Different green cards values occurred";
		fprintf(file, "Different green cards values occurred");
		system("pause");
	}
	/*
	else if (green_value == 0) {
		cout << "Green cards does not exist";
		fprintf(file, "Green cards does not exist");
		system("pause");
	}
	else {
		cout << "Found " << colors_number[green] << " green cards, all with " << green_value << " value";
		fprintf(file, "Found %d green cards, all with %d value", colors_number[green], green_value);
		system("pause");
	}*/
}

int count_values(card** players_cards[], card** players_deck[], card** piles_cards[], int* colors_values[], int players, int piles, int player_cards_number[], int piles_values[]) {
	
	for (int j = 0; j < players; j++) {
		for (int k = 0; k<max_cards; k++) {
			if (players_cards[j][k]->value == 0) break;
			int temp = players_cards[j][k]->value;
			int temp2 = players_cards[j][k]->color;
			colors_values[temp2][temp]++;
			player_cards_number[j]++;
		}
	}

	for (int j = 0; j < players; j++) {
		for (int k = 0; k < max_cards; k++) {
			if (players_deck[j][k]->value == 0) break;
			int temp = players_deck[j][k]->value;
			int temp2 = players_deck[j][k]->color;
			colors_values[temp2][temp]++;
		}
	}

	for (int j = 0; j < piles; j++) {
		piles_values[j] = 0;
		for (int k = 0; k<max_cards; k++) {
			if (piles_cards[j][k]->value == 0) break;
			int temp = piles_cards[j][k]->value;
			int temp2 = piles_cards[j][k]->color;
			colors_values[temp2][temp]++;
			piles_values[j] += piles_cards[j][k]->value;
		}
	}

	for (int i = 2; i < piles+1; i++) {
		for (int j = 0; j<max_value; j++) {
			if ((colors_values[1][j] != colors_values[i][j])) return 0;
		}
	}

	return 1;

}

void print_colors_values(int* colors_values[], int is_equal, int piles) {

	char list[8][7] = { "green", "blue", "red", "violet", "yellow", "white", "black" };

	if (is_equal) { //if values of cards of all colors are identical then print
		cout << "The values of cards of all colors are identical:" << endl;
		for (int i = 0; i < max_value; i++) {
			if (colors_values[1][i] > 0) {
				for (int j = 0; j < colors_values[1][i]; j++) cout << i << " ";
			}
		}
	}
	else { //otherwise print too
		cout << "The values of cards of all colors are not identical:" << endl;
		for (int i = 1; i < piles+1; i++) {
			cout << list[i] << " cards values: ";
			for (int j = 0; j < max_value; j++) {
				if (colors_values[i][j] > 0) {
					for (int k = 0; k < colors_values[i][j]; k++) cout << j << " ";
				}
			}
			cout << endl;
		}
	}
}

void check_gamestate(card** piles_cards[], int active_player, int players, int explosion, int piles, int player_cards_number[], int piles_values[], FILE* file) {
	
	for (int i = 0; i < active_player-1; i++) player_cards_number[i]++; //delete usuwam possible players' moves

	int max = 0;
	int min = max_cards;
	int ok = 1; //is the game state valid

	for (int i = 0; i < players; i++) { //find min and max card among players
		if (player_cards_number[i] > max) max = player_cards_number[i];
		else if (player_cards_number[i] < min) min = player_cards_number[i];
	}
	if (max - min > 1) {
		cout << "The number of players cards on hand is wrong" << endl;
		fprintf(file, "The number of players cards on hand is wrong");
		ok = 0; //gamestate invalid
	}

	for (int i = 0; i < piles; i++) { //check if the pile is one-color only
		int temp = piles_cards[i][0]->color;
		for (int j = 1; j < max_cards; j++) {
			if (piles_cards[i][j]->value == 0) break;
			if (piles_cards[i][j]->color != temp && piles_cards[i][j]->color != green && temp!=green) { //if there are two colors different from green then error
				cout << "Two different colors were found on the " << i + 1 << " pile" << endl;
				fprintf(file, "Two different colors were found on the %d pile", i + 1);
				system("pause");
				ok = 0; //gamestate invalid
				break;
			}

		}
	}

	for (int i = 0; i < piles; i++) {
		if (piles_values[i] > explosion) { //pile should've already exploded
			cout << "Pile number " << i + 1 << " should explode earlier" << endl;
			fprintf(file, "Pile number %d should explode earlier", i + 1);
			system("pause");
			ok = 0; //gamestate invalid
		}
	}

	//if (ok) cout << "Current state of the game is ok" << endl; //jak ok to ok



}

int check_color(card** piles_cards[], int piles, int card_color) {

	for (int i = 0; i < piles; i++)
	{
		for (int j = 0; j < max_cards; j++) {
			if (piles_cards[i][j]->value == 0) break;
			if (piles_cards[i][j]->color == card_color) return i; //seek for card in needed color
			else if (piles_cards[i][j]->color != 0) break; //if i find the invalid one then break
		}
	}
	return -1;
}

int find_empty(card** piles_cards[], int piles, int piles_values[], int card_color) {
	int min_val = -1;
	for (int i = 0; i < piles; i++) { //find first empty pile
		for (int j = 0; j < max_cards; j++) {
			if (piles_cards[i][j]->value == 0) {
				if (min_val == -1) min_val = i;
				else if (piles_values[i] < piles_values[min_val]) min_val = i;
				break;
			};
			if (piles_cards[i][j]->color != 0 && card_color != green ) break;
		}
	}
	return min_val;
}

int simple(card** players_cards[], int* active_player, card** piles_cards[], int piles, int piles_values[]) { //simple move
	//find proper color
	int prop_pile = players_cards[*active_player - 1][0]->color == green ? 0 : check_color(piles_cards, piles, players_cards[*active_player - 1][0]->color);  //call the function for first card of active player
	if(prop_pile == -1) prop_pile = find_empty(piles_cards, piles, piles_values, players_cards[*active_player - 1][0]->color);
	return prop_pile;
}

int smart(card** player_cards[], card** piles_cards[], int piles, int whoami, int piles_values[], int explosion) {//optimal move

	int min_card = 0; // find the minimal card's value
	for (int i = 1; i < max_cards; i++) {
		if (player_cards[whoami][i]->value == 0) break;
		if (player_cards[whoami][min_card]->value > player_cards[whoami][i]->value) min_card = i;
	}

	int min_pile = -1; // find all the cards with this value and check the pile value afterr adding each of them, choose the best one
	for (int i = 0; i < max_cards; i++) {
		if(player_cards[whoami][i]->value == 0) break;
		if (player_cards[whoami][i]->value == player_cards[whoami][min_card]->value) {
			int temp = check_color(piles_cards, piles, player_cards[whoami][i]->color);
			if (temp == -1) temp = find_empty(piles_cards, piles, piles_values, player_cards[whoami][i]->color);
			if (min_pile == -1) {
				min_pile = temp;
				min_card = i;
			}
			else if (piles_values[temp] < piles_values[min_pile]) {
				min_pile = temp;
				min_card = i;
			}
		}
	}

	if ((piles_values[min_pile] + player_cards[whoami][min_card]->value) <= explosion) { //if pile doesn't expplode then throw the lowest card to the lowest pile
		card* temp_card = player_cards[whoami][0];
		player_cards[whoami][0] = player_cards[whoami][min_card];
		player_cards[whoami][min_card] = temp_card;
		return min_pile;
	}

	int max_card = 0;//if it explodes then find biggest card and blow up the lowest pile
	for (int i = 1; i < max_cards; i++) {
		if (player_cards[whoami][i]->value == 0) break;
		if (player_cards[whoami][max_card]->value < player_cards[whoami][i]->value) max_card = i;
	}

	min_pile = -1;
	for (int i = 0; i < max_cards; i++) {
		if(player_cards[whoami][i]->value == 0) break;
		if (player_cards[whoami][i]->value == player_cards[whoami][max_card]->value) {
			int temp = check_color(piles_cards, piles, player_cards[whoami][i]->color);
			if (temp == -1) temp = find_empty(piles_cards, piles, piles_values, player_cards[whoami][i]->color);
			if (min_pile == -1) {
				min_pile = temp;
				max_card = i;
			}
			else if (piles_values[temp] < piles_values[min_pile]) {
				min_pile = temp;
				max_card = i;
			}
		}
	}

	card* temp_card = player_cards[whoami][0];
	player_cards[whoami][0] = player_cards[whoami][max_card];
	player_cards[whoami][max_card] = temp_card;
	return min_pile;

}

int optimal(card** player_cards[], card** piles_cards[], int piles, int whoami, int piles_values[], int explosion) {//posuniecie optymalne2
	int end = 0; //find the index of last card
	for (int i = 0; i < max_cards; i++) {
		if (player_cards[whoami][i]->value == 0) {
			end = i-1;
			break;
		}
	}
	for (int i = end; i > 0; i--) { //sort cards in hand descendingly
		for (int j = 0; j < i; j++) {
			if (player_cards[whoami][j]->value > player_cards[whoami][j + 1]->value) {
				card* temp = player_cards[whoami][j];
				player_cards[whoami][j] = player_cards[whoami][j + 1];
				player_cards[whoami][j + 1] = temp;
			}
		}
	}

	for (int i = 0; i < max_cards; i++) { //find the biggest card that won't blow up the pile
		if (player_cards[whoami][i]->value == 0) break;
		int temp = check_color(piles_cards, piles, player_cards[whoami][i]->color);
		if (temp == -1) temp = find_empty(piles_cards, piles, piles_values, player_cards[whoami][i]->color);
		if (piles_values[temp] + player_cards[whoami][i]->value <= explosion) {
			card* temp_card = player_cards[whoami][0];
			player_cards[whoami][0] = player_cards[whoami][i];
			player_cards[whoami][i] = temp_card;
			return temp;
		}
	}

	int max_card = 0;//if I can't hen blow up the lowest pile with biggest card
	for (int i = 1; i < max_cards; i++) {
		if (player_cards[whoami][i]->value == 0) break;
		if (player_cards[whoami][max_card]->value < player_cards[whoami][i]->value) max_card = i;
	}

	int min_pile = -1;
	for (int i = 0; i < max_cards; i++) {
		if (player_cards[whoami][i]->value == 0) break;
		if (player_cards[whoami][i]->value == player_cards[whoami][max_card]->value) {
			int temp = check_color(piles_cards, piles, player_cards[whoami][i]->color);
			if (temp == -1) temp = find_empty(piles_cards, piles, piles_values, player_cards[whoami][i]->color);
			if (min_pile == -1) {
				min_pile = temp;
				max_card = i;
			}
			else if (piles_values[temp] < piles_values[min_pile]) {
				min_pile = temp;
				max_card = i;
			}
		}
	}

	card* temp_card = player_cards[whoami][0];
	player_cards[whoami][0] = player_cards[whoami][max_card];
	player_cards[whoami][max_card] = temp_card;
	return min_pile;
}

void move(card** players_cards[], card** piles_cards[], card** players_deck[], int* active_player, int players, int explosion, int piles_values[], int prop_pile) {//wykonuje ruch
		for (int i = 0; i < max_cards; i++) { //throw card to the given pile
			if (piles_cards[prop_pile][i]->value == 0) {
				piles_cards[prop_pile][i]->value = players_cards[*active_player-1][0]->value;
				piles_cards[prop_pile][i]->color = players_cards[*active_player-1][0]->color;
				break;
			}
			
		}
		
	//if the pile explodes return cards to the player
		if ((piles_values[prop_pile] + players_cards[*active_player - 1][0]->value) > explosion) {
			int k = 0;
			while (players_deck[*active_player - 1][k]->value != 0)k++;
			for (int i = 0; i < max_cards; i++) {
				if (piles_cards[prop_pile][i]->value == 0)break;
				players_deck[*active_player - 1][k]->value = piles_cards[prop_pile][i]->value;
				players_deck[*active_player - 1][k]->color = piles_cards[prop_pile][i]->color;
				piles_cards[prop_pile][i]->value = 0;
				piles_cards[prop_pile][i]->color = 0;
				k++;
			}
		}
		
		//move cards in player's hand after his move
		int j = 1;
		while (players_cards[*active_player - 1][j]->value != 0) {
			players_cards[*active_player - 1][j - 1]->color = players_cards[*active_player - 1][j]->color;
			players_cards[*active_player - 1][j - 1]->value = players_cards[*active_player - 1][j]->value;
			j++;
		}
		players_cards[*active_player - 1][j - 1]->value = 0;
		
	(*active_player)++; //change active player to the next one
	if (*active_player > players) (*active_player) -= players;
}

void check_immunity(card** players_cards[], card** players_deck[], int players, int piles, int* player_colors_number[], int* players_immunity[]) {//sprawdzam nietykalnosc
	
	char list[8][7] = { "green", "blue", "red", "violet", "yellow", "white", "black" };

	for (int i = 0; i < players; i++) { //count colors in every player's hand
		for (int j = 0; j < max_cards; j++) {
			if (players_cards[i][j]->value == 0)break;
			int temp = players_cards[i][j]->color;
			player_colors_number[i][temp]++;
		}
	}

	for (int i = 0; i < players; i++) { //count colors in every player's deck
		for (int j = 0; j < max_cards; j++) {
			if (players_deck[i][j]->value == 0)break;
			int temp = players_deck[i][j]->color;
			player_colors_number[i][temp]++;
		}
	}
	for (int i = 1; i < piles+1; i++) { //check player's immunity to the color, starting from one as it's impossible to be immune to green
		int max = 0, max_player = -1;
		for (int j = 0; j < players; j++) {
			if (player_colors_number[j][i] > max) { //if I find player with more colors then switch to him
				max = player_colors_number[j][i];
				max_player = j;
			}
			else if (player_colors_number[j][i] == max && max > 0) { //if there are to players with the same values then there's no immunity
				max_player = -1;
			}
		}
		if (max_player != -1) players_immunity[max_player][i] = 1;
	}

}

void end_round(card** players_cards[], card** players_deck[], int players, int piles, int* players_immunity[], int scoreboard[], FILE* file) {//finish the round

	char list[8][7] = { "green", "blue", "red", "violet", "yellow", "white", "black" };

	for (int i = 0; i < players; i++) { //cound the score in hand
		for (int j = 0; j < max_cards; j++) {
			if (players_cards[i][j]->value == 0) break;
			int temp = players_cards[i][j]->color;
			if (temp == green) scoreboard[i] += 2;
			else if (!players_immunity[i][temp]) scoreboard[i]++;
		}
	}

	for (int i = 0; i < players; i++) {//count the score in deck
		for (int j = 0; j < max_cards; j++) {
			if (players_deck[i][j]->value == 0) break;
			int temp = players_deck[i][j]->color;
			if (temp == green) scoreboard[i] += 2;
			else if (!players_immunity[i][temp]) scoreboard[i]++;
		}
	}
	
	for (int i = 1; i < piles+1; i++) {//print immunities (in polish for some reason)
		for (int j = 0; j < players; j++) {
			if (players_immunity[j][i]){
				cout << "Na kolor " << list[i] << " odporny jest gracz " << j+1<< endl;
				fprintf(file, "\nNa kolor %s odporny jest gracz %d", list[i], j + 1);
				}
		}
	}

	for (int i = 0; i < players; i++) {//print score (as above)
		cout << "Wynik gracza " << i + 1 << " = " << scoreboard[i] << endl;
		fprintf(file, "\nWynik gracza %d = %d", i + 1, scoreboard[i]);
	}
}

int main(int argc, char* argv[]) {
	int whoami = argc > 1 ? atoi(argv[1]) : -1; //program knows which player he is

	if (whoami == 1) { //if he's first then he initializes the game
		char path[] = { "input.txt" };
		FILE* file = fopen(path,"w"); //open the file

		int players, active_player = 1, piles, explosion, gree, green_value, others;
		cin >> players >> piles >> explosion >> gree >> green_value >> others; //read the data from console

		int* actvp = &active_player;
		int deck_size = (gree + (others * piles));//calculate the deck's size

		int* values;
		values = (int*)malloc(sizeof(int) * others);

		for (int i = 0; i < others; i++) {
			cin >> values[i];
		}

		card** cards;
		cards = (card**)malloc(deck_size * sizeof(card*));

		card*** player_cards; //hand of each player
		player_cards = (card***)malloc(players * sizeof(card**) + 1);
		for (int i = 0; i < players; i++) player_cards[i] = (card**)malloc((max_cards + 1) * sizeof(card*));
		for (int i = 0; i < players; i++) clear_deck(player_cards[i], (max_cards + 1)); //cleared

		card*** player_deck; //deck of each player
		player_deck = (card***)malloc(players * sizeof(card**) + 1);
		for (int i = 0; i < players; i++) player_deck[i] = (card**)malloc((max_cards + 1) * sizeof(card*));
		for (int i = 0; i < players; i++) clear_deck(player_deck[i], (max_cards + 1)); // cleared

		card*** piles_cards; //each pile
		piles_cards = (card***)malloc((piles + 1) * sizeof(card**));
		for (int i = 0; i < piles + 1; i++) piles_cards[i] = (card**)malloc((max_cards + 1) * sizeof(card*));
		for (int i = 0; i < piles + 1; i++) clear_deck(piles_cards[i], (max_cards + 1)); //cleared

		int* player_cards_number = (int*)malloc(sizeof(int) * players); //number of cards on each player's hand
		for (int i = 0; i < players; i++) player_cards_number[i] = 0; //cleared

		int* piles_values = (int*)malloc(sizeof(int) * piles); //values of the cards on each pile
		for (int i = 0; i < piles; i++) piles_values[i] = 0; //cleared

		int** colors_values; //values of each color cards
		colors_values = (int**)malloc(sizeof(int*) * max_colors);
		for (int i = 0; i < piles + 1; i++) colors_values[i] = (int*)malloc(sizeof(int) * max_value);
		for (int i = 0; i < piles + 1; i++) { //cleared
			for (int j = 0; j < max_value; j++) {
				colors_values[i][j] = 0;
			}
		}

		int is_equal = count_values(player_cards, player_deck, piles_cards, colors_values, players, piles, player_cards_number, piles_values); //check if each color has the same values and some other stuff

		create_deck(values, cards, piles, gree, green_value, others); //create cards
		shuffle(cards, deck_size);//shuffle
		deal_cards(cards, players, player_cards, deck_size);//deal

		cout << endl; //print game state
		cout << "cards left = " << deck_size << endl;
		cout << "active player = " << active_player << endl;
		cout << "players number = " << players << endl;
		cout << "explosion threshold = " << explosion;
		fprintf(file, "cards left = %d\nactive player = %d\nplayers number = %d\nexplosion threshold = %d", deck_size, active_player, players, explosion);
		print_game(player_cards, player_deck, piles_cards, piles, players, file);
		fclose(file);
		free(values);
		free(cards);
		free(player_cards); //free memory
		free(piles_cards);
		free(colors_values);
		free(player_deck);
		free(player_cards_number);
	}

	/*cout << "active player = 1" << endl; //potrzebne do pierwszych zadan
	cout << "players number = " << players << endl;
	print_players(player_cards, players, deck_size);
	for (int i = 0; i < kettles; i++) cout << i+1 << " " << "pile cards:" << endl;
	print_deck(cards, gree, kettles, others);*/ 


	
	while (1) { //do the move on the state from the file
		char path[] = { "input.txt" };
		FILE* file = fopen(path, "r"); //open file
		
		if (file == NULL) { //if i can't then wait and try again
			Sleep(2000 + (whoami - 1) *1500);
			continue;
		}
		int game_point = game_init(file); //read how many cards are left in the players' hands so i know when to end the game

		if (game_point == -1) { //if i opened the file and have no data, then it had to be overwritten in this particular moment, so I wait and try again
			Sleep(2000 + (whoami - 1) *1500);
			continue;
		}
		
		if (!game_point) { //if there are no cards in hand then finish the game
			fclose(file);
			break;
		}

		game_point--; //player plays the card, so remove it from hand

		int active_player = game_init(file); //read current player
		
		if (active_player != whoami) { //if not an active player then wait for own turn
			fclose(file);
			Sleep(2000 + (whoami - 1) * 1500);
			cout << "Waiting" << endl;
			continue;
		}

		int* actvp = &active_player; //for one of the fucntions sake

		int players = game_init(file); //read number of players
		int explosion = game_init(file); //and explosion treshold

		card*** player_cards; //each player's hand
		player_cards = (card***)malloc(players * sizeof(card**) + 1);
		for (int i = 0; i < players; i++) player_cards[i] = (card**)malloc((max_cards + 1) * sizeof(card*));
		for (int i = 0; i < players; i++) clear_deck(player_cards[i], (max_cards + 1)); //cleared

		card*** player_deck; //each player's deck
		player_deck = (card***)malloc(players * sizeof(card**) + 1);
		for (int i = 0; i < players; i++) player_deck[i] = (card**)malloc((max_cards + 1) * sizeof(card*));
		for (int i = 0; i < players; i++) clear_deck(player_deck[i], (max_cards + 1)); // cleared

		int* player_cards_number = (int*)malloc(sizeof(int) * players); //number of cards on each player's hand
		for (int i = 0; i < players; i++) player_cards_number[i] = 0; //cleared

		int colors_number[7] = { 0, 0, 0, 0, 0, 0, 0 }; //number of cards for each color, cleared

		int piles = 0; //number of piles, cleared

		load_game(players, player_cards, player_deck, colors_number, file); //loading the game state for each player

		for (int i = 1; i < 7; i++) if (colors_number[i])piles++; //set the piles number basing on colors

		int* piles_values = (int*)malloc(sizeof(int) * piles); //values of cards on each pile
		for (int i = 0; i < piles; i++) piles_values[i] = 0; //cleared

		card*** piles_cards; //each pile
		piles_cards = (card***)malloc((piles + 1) * sizeof(card**));
		for (int i = 0; i < piles + 1; i++) piles_cards[i] = (card**)malloc((max_cards + 1) * sizeof(card*));
		for (int i = 0; i < piles + 1; i++) clear_deck(piles_cards[i], (max_cards + 1)); //cleared

		load_piles(piles, piles_cards, colors_number, file); //loading the game state for each pile
		fclose(file);

		check_green(player_cards, player_deck, piles_cards, players, piles);
		print_green(colors_number, check_green(player_cards, player_deck, piles_cards, players, piles), file);
		check_gamestate(piles_cards, whoami, players, explosion, piles, player_cards_number, piles_values, file);

		int** colors_values; //values of cards for each color
		colors_values = (int**)malloc(sizeof(int*) * max_colors);
		for (int i = 0; i < piles + 1; i++) colors_values[i] = (int*)malloc(sizeof(int) * max_value);
		for (int i = 0; i < piles + 1; i++) { //cleared
			for (int j = 0; j < max_value; j++) {
				colors_values[i][j] = 0;
			}
		}

		int is_equal = count_values(player_cards, player_deck, piles_cards, colors_values, players, piles, player_cards_number, piles_values); //check if each color has the same values

		if(whoami==1)move(player_cards, piles_cards, player_deck, actvp, players, explosion, piles_values, smart(player_cards, piles_cards, piles, whoami-1, piles_values, explosion)); //first player plays smart strategy
		if(whoami==2)move(player_cards, piles_cards, player_deck, actvp, players, explosion, piles_values, optimal(player_cards, piles_cards, piles, whoami - 1, piles_values, explosion));//second player plays optimal strategy
		
		int** player_colors_number = (int**)malloc(sizeof(int*) * players + 1); //cards of given color of each player
		for (int i = 0; i < players; i++) player_colors_number[i] = (int*)malloc(sizeof(int) * max_colors + 1);
		for (int i = 0; i < players; i++) { //cleared
			for (int j = 0; j < max_colors; j++) {
				player_colors_number[i][j] = 0;
			}
		}

		int** players_immunity = (int**)malloc(sizeof(int*) * players + 1); //save immunities
		for (int i = 0; i < players; i++) players_immunity[i] = (int*)malloc(sizeof(int) * max_colors + 1);
		for (int i = 0; i < players; i++) { //cleared
			for (int j = 0; j < max_colors; j++) {
				players_immunity[i][j] = 0;
			}
		}
		int* scoreboard = (int*)malloc(sizeof(int) * players + 1); //score
		for (int i = 0; i < players; i++) scoreboard[i] = 0;

		//print_colors_values(colors_values, is_equal, piles); //pozostalosci z poprzednich zadan
		//print_piles(piles, piles_cards);
		//print_colors(colors_number);
		//print_green(colors_number, check_green(player_cards, player_deck, piles_cards, players, piles));
		//check_gamestate(piles_cards, active_player, players, explosion, piles, player_cards_number, piles_values);

		file = fopen(path, "w");

		cout << endl; //print the gamestate
		cout << "cards left = " << game_point << endl;
		cout << "active player = " << active_player << endl;
		cout << "players number = " << players << endl;
		cout << "explosion threshold = " << explosion;
		fprintf(file, "cards left = %d\nactive player = %d\nplayers number = %d\nexplosion threshold = %d", game_point, active_player, players, explosion);
		print_game(player_cards, player_deck, piles_cards, piles, players, file);

		if (!game_point) { //if no cards on hands then end the game
			check_immunity(player_cards, player_deck, players, piles, player_colors_number, players_immunity); //check immunities
			cout << endl;
			end_round(player_cards, player_deck, players, piles, players_immunity, scoreboard, file); //finish the round and print the state
			fclose(file);
			free(player_cards); //free memory
			free(piles_cards);
			free(colors_values);
			free(player_deck);
			free(player_cards_number);
			free(piles_values);
			free(player_colors_number);
			free(players_immunity);
			free(scoreboard);
			break;
		}

		fclose(file);


		free(player_cards); //free memory
		free(piles_cards);
		free(colors_values);
		free(player_deck);
		free(player_cards_number);
		free(piles_values);
		free(player_colors_number);
		free(players_immunity);
		free(scoreboard);
	}
	return 0; //end 
}
