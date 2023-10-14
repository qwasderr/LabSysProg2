#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "set.h"
#define MAX_ALPH 100
#define MAX_STATES 100
#define MAX_COLLISIONS 10
int sigma[MAX_STATES][MAX_ALPH][MAX_COLLISIONS];
int states_count = 0;
int fin_states_count = 0;
bool recognizes = 1;
bool wontstop = 0;
int alph_num, states_num, final_states_num;
int str_len(char* t) {
	return (strchr(t, 0) - t) / sizeof(char);
}
int get_alph_pos(char find, char* alph) {
	int i = 0;
	while (alph[i] != find) ++i;
	return i;
}
void get_states(FILE* f, int* states) {
	char ch, buffer[32];
	for (int i = 0; i < 32; ++i) buffer[i] = NULL;
	int i = 0, j = 0;
	while (1) {
		ch = fgetc(f);

		if (ch == ' ' || ch == '\n') {
			states[j] = atoi(buffer);
			++j;
			for (int i = 0; i < 32; ++i) buffer[i] = NULL;
			i = 0;
			if (ch == '\n') break;
			continue;

		}
		else {
			buffer[i] = ch;
			++i;
		}
	}
	states_count = j;
}
void get_final_states(FILE* f, int* final_states) {
	int j = 0, i = 0;
	char ch = fgetc(f);
	char buffer[32];
	for (int i = 0; i < 32; ++i) buffer[i] = NULL;
	while (1) {
		ch = fgetc(f);

		if (ch == ' ' || ch == '\n') {
			final_states[j] = atoi(buffer);
			++j;
			for (int i = 0; i < 32; ++i) buffer[i] = NULL;
			i = 0;
			if (ch == '\n') break;
			continue;

		}
		else {
			buffer[i] = ch;
			++i;
		}
	}
	fin_states_count = j;
}
bool is_final(int state, int* final_states) {
	char s;
	for (int i = 0; i < final_states_num; ++i) {
		if (state == final_states[i]) return true;
	}
	return false;
}
SimpleSet* states_after_word(char* word, int state, int* states, char* alph, int* final_states) {
	SimpleSet states2;
	SimpleSet tempStates;
	set_init(&states2);
	set_init(&tempStates);
	char s[100];
	sprintf(s, "%d", state);
	bool flag;
	//_itoa_s(state,s,10);
	//s = (char*)state;
	set_add(&states2, s);
	int ttt = str_len(word);
	for (int i = 0; i < str_len(word); ++i) {
		//int t = sizeof(states) / sizeof(int);
		flag = 0;
		for (int j = 0; j < states_num; ++j) {
			sprintf(s, "%d", states[j]);
			//s = (char*)states[j];
			if (set_contains(&states2, s) == SET_TRUE) {
				int temp1 = states[j];
				char hgj = word[i];
				int temp2 = get_alph_pos(word[i], alph);
				if (sigma[states[j]][get_alph_pos(word[i], alph)][0] > 0) {
					flag = 1;
					for (int k = 1; k <= sigma[states[j]][get_alph_pos(word[i], alph)][0]; ++k) {
						//s = (char*)(sigma[states[j]][get_alph_pos(word[i], alph)][k]);
						sprintf(s, "%d", sigma[states[j]][get_alph_pos(word[i], alph)][k]);
						/*if (!is_final(sigma[states[j]][get_alph_pos(word[i], alph)][k], final_states)) {
							set_add(&tempStates, s);
						}*/
						set_add(&tempStates, s);
					}

				}
			}
		}
		//states2=remove_final(tempStates, final_states);
		//states2 = tempStates;
		set_clear(&states2);
		for (int k = 0; k < states_num; ++k) {
			sprintf(s, "%d", states[k]);
			if (set_contains(&tempStates, s) == SET_TRUE) set_add(&states2, s);
		}
		//int k = set_contains(&states2, "1");
		set_clear(&tempStates);
		if (flag == 0) {
			printf("Automata doesn't recognize the word\n");
			recognizes = 0;
			return &states2;
		}
		//int v = set_contains(&states2, "1");
	}
	set_destroy(&tempStates);
	return &states2;
}
SimpleSet* bfs(SimpleSet set, char* alph, int* states) {
	//const int size = states_count;
	int size_alph = alph_num;
	//char c;
	char c[100];
	int* marked = (int*)malloc(states_num * sizeof(int));
	for (int i = 0; i < states_num; ++i) marked[i] = 0;
	int* queue = (int*)malloc(states_num * sizeof(int));
	int head = 0, tail = 0;
	for (int i = 0; i < states_num; ++i) marked[i] = 0;
	for (int j = 0; j < states_num; ++j) {
		//c = (char)states[j];
		sprintf(c, "%d", states[j]);
		if (set_contains(&set, c) == SET_TRUE) {
			queue[head] = states[j];
			++head;
			marked[j] = 1;
		}
	}
	set_clear(&set);
	while (head != tail) {
		for (int i = 0; i < size_alph; ++i) {
			for (int j = 1; j <= sigma[queue[tail]][i][0]; ++j) {
				sprintf(c, "%d", sigma[queue[tail]][i][j]);
				int temp = set_add(&set, c);
				if (temp != SET_ALREADY_PRESENT) {
					queue[head] = sigma[queue[tail]][i][j];
					++head;
				}
			}
		}
		++tail;
	}
	return &set;

}
SimpleSet* delete_finals(SimpleSet set, int* final_states, char* word, int* states) {
	char c[100];
	int count = 0;
	for (int i = 0; i < states_num; ++i) {
		if (!is_final(states[i], final_states)) {
			sprintf(c, "%d", states[i]);
			if (set_contains(&set, c) == SET_TRUE) { set_remove(&set, c); ++count; }
		}
	}
	if (count == 0 && set_length(&set) == 0 && recognizes != 0) {
		printf("Automata will be undefined and won't get to the final state on %s+w1\n", word); wontstop = 1;
	}
	else if (set_length(&set) == 0 && recognizes != 0) printf("Automata won't get to the final state after w0 => w1 doesn't exist\n");
	return &set;
}
bool check(SimpleSet set) {
	if (set_length(&set) > 0 && wontstop != 1) return true; return false;
}
int main() {
	FILE* f;
	fopen_s(&f, "test.txt", "rt");
	char* alph = (char*)malloc(MAX_ALPH * sizeof(char));
	int* states = (int*)malloc(MAX_STATES * sizeof(int));
	int start_state;
	char word[50];
	//scanf("%s", &word);

	memset(sigma, 0, MAX_STATES * MAX_ALPH * MAX_COLLISIONS * sizeof(int));
	//char *final_states= (char*)malloc(MAX_STATES * sizeof(char));
	int* final_states = (int*)malloc(MAX_STATES * sizeof(int));
	char ch, buffer[32], symbol;
	for (int i = 0; i < 32; ++i) buffer[i] = NULL;
	int j = 0, i = 0, prev_state, next_state;
	
	while (!feof(f)) {
		fscanf(f, "%s", word);
		fscanf(f, "%ld", &alph_num);
		fscanf(f, "%ld", &states_num);
		for (int i = 0; i < alph_num; ++i) alph[i] =(char)(97 + i);
		for (int i = 0; i < states_num; ++i) states[i] = i;
		//ch = fgetc(f);
		//fgets(&word, MAX_ALPH, f);
		//fgets(alph, MAX_ALPH, f);
		//get_states(f, states);
		//fgets(&start_state,10,f);
		fscanf_s(f, "%ld", &start_state);
		//alph = get_alph(f);
		//states =get_states(f);
		fscanf_s(f, "%ld", &final_states_num);
		for (int i = 0; i < final_states_num; ++i) fscanf_s(f, "%ld", &final_states[i]);
		//get_final_states(f, final_states);
		while (!feof(f)) {
			fscanf_s(f, "%ld", &prev_state);
			ch = fgetc(f);
			fscanf_s(f, "%c", &symbol);
			ch = fgetc(f);
			fscanf_s(f, "%ld", &next_state);
			sigma[prev_state][get_alph_pos(symbol, alph)][sigma[prev_state][get_alph_pos(symbol, alph)][0] + 1] = next_state;
			++sigma[prev_state][get_alph_pos(symbol, alph)][0];
		}
	}

	fclose(f);
	/*for (int i = 0; i < alph_num; ++i) printf("%c\n", alph[i]);
	for (int k = 0; k < 11; ++k) {
		for (int h = 0; h < 11; ++h) {
			for (int t=1;t<= sigma[k][h][0];++t)
			printf("%i %i %i \n", k, h, sigma[k][h][t]);
		}
		printf("%\n");
	}*/

	SimpleSet* __states;
	__states = states_after_word(word, start_state, states, alph, final_states);
	__states = bfs(*__states, alph, states);
	__states = delete_finals(*__states, final_states, word,states);
	bool answer = check(*__states);
	printf("%s", answer ? "true" : "false");
	set_destroy(__states);
	free(alph);
	free(states);
	free(final_states);

}
