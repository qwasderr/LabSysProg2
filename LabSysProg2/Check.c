#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "set.h"
#define MAX_ALPH 100
#define MAX_STATES 100
#define MAX_COLLISIONS 10
#define STATE_LEN 5
#define WIT_LEN 100
int sigma1[MAX_STATES][MAX_ALPH];
int sigma2[MAX_STATES][MAX_ALPH];
int states_count = 0;
int fin_states_count = 0;
bool recognizes = 1;
bool wontstop = 0;
int alph_num1, states_num1, final_states_num1, alph_num2, states_num2, final_states_num2;
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
	for (int i = 0; i < final_states_num1; ++i) {
		if (state == final_states[i]) return true;
	}
	return false;
}
int main() {
	FILE* f;
	fopen_s(&f, "automata1.txt", "rt");
	char* alph1 = (char*)malloc(MAX_ALPH * sizeof(char));
	int* states1 = (int*)malloc(MAX_STATES * sizeof(int));
	int start_state1;
	char temp[50];
	SimpleSet states_1[50];
	SimpleSet states_2[50];
	int states_1_count = 0;
	int states_2_count = 0;
	memset(sigma1, 0, MAX_STATES * MAX_ALPH * sizeof(int));
	int* final_states1 = (int*)malloc(MAX_STATES * sizeof(int));
	char ch, buffer[32], symbol;
	for (int i = 0; i < 32; ++i) buffer[i] = NULL;
	int j = 0, i = 0, prev_state, next_state;

	while (!feof(f)) {
		fscanf(f, "%ld", &alph_num1);
		fscanf(f, "%ld", &states_num1);
		for (int i = 0; i < alph_num1; ++i) alph1[i] = (char)(97 + i);
		for (int i = 0; i < states_num1; ++i) { 
			states1[i] = i; 
			SimpleSet set;
			set_init(&set);
			sprintf(temp, "%d", states1[i]);
			set_add(&set, temp);
			states_1[states_1_count] = set;
			++states_1_count;
		
		}
		fscanf_s(f, "%ld", &start_state1);
		fscanf_s(f, "%ld", &final_states_num1);
		for (int i = 0; i < final_states_num1; ++i) fscanf_s(f, "%ld", &final_states1[i]);
		while (!feof(f)) {
			fscanf_s(f, "%ld", &prev_state);
			ch = fgetc(f);
			fscanf_s(f, "%c", &symbol);
			ch = fgetc(f);
			fscanf_s(f, "%ld", &next_state);
			sigma1[prev_state][get_alph_pos(symbol, alph1)] = next_state;
		}
	}

	fclose(f);


	fopen_s(&f, "automata2.txt", "rt");
	char* alph2 = (char*)malloc(MAX_ALPH * sizeof(char));
	int* states2 = (int*)malloc(MAX_STATES * sizeof(int));
	int start_state2;
	memset(sigma2, 0, MAX_STATES * MAX_ALPH * sizeof(int));
	int* final_states2 = (int*)malloc(MAX_STATES * sizeof(int));
	for (int i = 0; i < 32; ++i) buffer[i] = NULL;

	while (!feof(f)) {
		fscanf(f, "%ld", &alph_num2);
		fscanf(f, "%ld", &states_num2);
		for (int i = 0; i < alph_num2; ++i) alph2[i] = (char)(97 + i);
		for (int i = 0; i < states_num2; ++i) {
			states2[i] = i;
			SimpleSet set;
			set_init(&set);
			sprintf(temp, "%d", states2[i]);
			set_add(&set, temp);
			states_2[states_2_count] = set;
			++states_2_count;
		}
		fscanf_s(f, "%ld", &start_state2);
		fscanf_s(f, "%ld", &final_states_num2);
		for (int i = 0; i < final_states_num2; ++i) fscanf_s(f, "%ld", &final_states2[i]);
		while (!feof(f)) {
			fscanf_s(f, "%ld", &prev_state);
			ch = fgetc(f);
			fscanf_s(f, "%c", &symbol);
			ch = fgetc(f);
			fscanf_s(f, "%ld", &next_state);
			sigma2[prev_state][get_alph_pos(symbol, alph1)] = next_state;
		}
	}

	fclose(f);
	/*SimpleSet* stack[100][2];
	int stack_count = 0;
	stack[0][0] = &(states_1[0]);
	stack[0][1] = &(states_2[0]);
	++stack_count;
	set_union(&(states_2[0]), &(states_1[0]), &(states_2[0]));
	//set_clear(&(states_1[0]));
	while (stack_count != 0) {
		--stack_count;
		SimpleSet* q1 = stack[stack_count][0];
		SimpleSet* q2 = stack[stack_count][1];
		SimpleSet* r1=NULL;
		SimpleSet* r2=NULL;
		for (int j = 0; j < alph_num1; ++j) {
			char letter = alph1[j];
			int state1=sigma1[], state2;
			for (int i = 0; i < states_num1; ++i) {
				if (set_is_subset(q1, &(states_1[i])) == SET_TRUE) {
					r1 = &(states_1[i]);
				}
				if (set_is_subset(q2, &(states_1[i])) == SET_TRUE) {
					r2 = &(states_1[i]);
				}
			}
			for (int i = 0; i < states_num2; ++i) {
				if (set_is_subset(q1, &(states_2[i])) == SET_TRUE) {
					r1 = &(states_2[i]);
				}
				if (set_is_subset(q2, &(states_2[i])) == SET_TRUE) {
					r2 = &(states_2[i]);
				}
			}
			if (r1 != r2) {
				set_union(r2, r1, r2);
				stack[stack_count][0] = r1;
				stack[stack_count][1] = r2;
				++stack_count;
			}
		}
	}*/
	FILE* file;
	fopen_s(&file,"output.txt", "w");
	int stack[100][2];
	int stack_count = 0;
	bool flag = 0, flag2 = 0;
	int depth,statet1,statet2;
	stack[stack_count][0] = start_state1;
	stack[stack_count][1] = start_state2;
	int seen[100][2];
	char witnesses[10][10][10];
	char temp1[STATE_LEN];
	char temp2[STATE_LEN];
	char witness[WIT_LEN];
	for (int i = 0; i < WIT_LEN; ++i) witness[i] = NULL;
	for (int l = 0; l < 10; ++l) 
		for (int u = 0; u < 10; ++u) 
			for (int p=0;p<10;++p)
			witnesses[l][u][p] = NULL;
	//char next[10];
	int witness_pos = 0;
	int seen_count = 0,key1,key2;
	if ((is_final(start_state1, final_states1) && !is_final(start_state2, final_states2)) || (!is_final(start_state1, final_states1) && is_final(start_state2, final_states2))) {
		fprintf(file,"Witness : empty string\n");
		fprintf(file, "false");
		return 0;
	}
	++stack_count;
	while (stack_count != 0) {
		--stack_count;
		seen[seen_count][0] = stack[stack_count][0];
		seen[seen_count][1] = stack[stack_count][1];
		++seen_count;
		statet1 = stack[stack_count][0];
		statet2 = stack[stack_count][1];
		depth = stack_count;
		for (int i = 0; i < alph_num1; ++i) {
			int r1 = sigma1[statet1][i], r2 = sigma2[statet2][i];
			fprintf(file,"1-st automata: state %i => state %i on %c\n", statet1,r1,alph1[i]);
			fprintf(file,"2-nd automata: state %i => state %i on %c\n", statet2, r2, alph2[i]);
			
			if ((is_final(r1, final_states1) && !is_final(r2, final_states2)) || (!is_final(r1, final_states1) && is_final(r2, final_states2))) {
				key1 = r1;
				key2 = r2;
				sprintf(temp1, "%d", statet1);
				sprintf(temp2, "%d", statet2);
				int len = str_len(temp1);
				int len2 = str_len(temp2);
				temp1[len] = ' ';
				++len;
				for (int v = len; v < str_len(temp2) + len; ++v) temp1[v] = temp2[v - len];
				len = len + len2;
				temp1[len] = ' ';
				++len;
				temp1[len] = alph1[i];
				for (int m = 0; m <= len; ++m) witnesses[r1][r2][m] = temp1[m];
				while (key1 != start_state1 || key2 != start_state2) {
					char *next = witnesses[key1][key2];
					char q1[100],q2[100];
					int pos = 0, q1pos = 0, q2pos = 0;
					while (next[pos] != ' ') {
						q1[q1pos] = next[pos];
						++pos;
						++q1pos;
					}
					q1[q1pos] = '\n';
					++pos;
					while (next[pos] != ' ') {
						q2[q2pos] = next[pos];
						++pos;
						++q2pos;
					}
					q2[q2pos] = '\n';
					++pos;
					char symbol1 = next[pos];
					witness[witness_pos] = symbol1;
					++witness_pos;
					key1 = atoi(q1);
					key2 = atoi(q2);
					int th = 1;
				}
				fprintf(file, "state %i of the 1-st automata and the state %i of the second one aren't both non-final or final =>automata aren't equal\n",r1,r2);
				fprintf(file,"Witness: ");
				int wit_len = str_len(witness);
				for (int len = wit_len - 1; len >= 0; --len) fprintf(file, "%c", witness[len]);
				fclose(file);
				return 0;
			}
			else fprintf(file,"state %i of the 1-st automata and the state %i of the second one are both non-final or final => next check\n\n", r1, r2);
			flag = 0;
			flag2 = 0;
			for (int j = 0; j < stack_count; ++j) {
				if ((stack[j][0] == r1 && stack[j][1] == r2)) flag = 1;
			}
			for (int j = 0; j < seen_count; ++j) {
				if ((seen[j][0] == r1 && seen[j][1] == r2)) flag2 = 1;
			}
			
			if (flag == 0 && flag2==0) {
				stack[stack_count][0] = r1;
				stack[stack_count][1] = r2;
				++stack_count;
				sprintf(temp1, "%d", statet1);
				sprintf(temp2, "%d", statet2);
				int len = str_len(temp1);
				int len2 = str_len(temp2);
				temp1[len] = ' ';
				++len;
				for (int v = len; v < str_len(temp2) + len; ++v) temp1[v] = temp2[v - len];
				len = len + len2;
				temp1[len] = ' ';
				++len;
				temp1[len] = alph1[i];
				for (int m = 0; m <= len; ++m) witnesses[r1][r2][m] = temp1[m];
				
			}
		}
	}
	fprintf(file,"No witness\n");
	fprintf(file, "true");
	fclose(file);
}
