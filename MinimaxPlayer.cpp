/*
 * MinimaxPlayer.cpp
 *
 *  Created on: Apr 17, 2015
 *      Author: wong
 */
#include <iostream>
#include <assert.h>
#include "MinimaxPlayer.h"
#include <map>

using namespace std;

typedef struct Action {
	int col;
	int row;
} Action;

typedef struct State {
	OthelloBoard board;
	int utility;
	State* parent;
	Action actionUsed;
} State;



State successor(State parent, Action action, char symbol);
State newState(OthelloBoard board, int utility, State* parent, Action action);
State newState(State* s);
Action MinimaxDecision(State initial, char symbol);
int MinValue(State state, char symbol);
int MaxValue(State state, char symbol);
bool TerminalTest(State state, char symbol);
char changeTurn(char symbol);

Action ACTIONS[] = {
	{ col: 0, row : 0 },
	{ col: 0, row : 1 },
	{ col: 0, row : 2 },
	{ col: 0, row : 3 },
	{ col: 1, row : 0 },
	{ col: 1, row : 1 },
	{ col: 1, row : 2 },
	{ col: 1, row : 3 },
	{ col: 2, row : 0 },
	{ col: 2, row : 1 },
	{ col: 2, row : 2 },
	{ col: 2, row : 3 },
	{ col: 3, row : 0 },
	{ col: 3, row : 1 },
	{ col: 3, row : 2 },
	{ col: 3, row : 3 }
};

char currentPlayer;
char opponent;

MinimaxPlayer::MinimaxPlayer(char symb) :
		Player(symb) {

}

MinimaxPlayer::~MinimaxPlayer() {

}

void MinimaxPlayer::get_move(OthelloBoard* b, int& col, int& row) {
	State initial = { OthelloBoard(*b), 0, 0 };
	currentPlayer = symbol;
	opponent = changeTurn(symbol);
	Action result = MinimaxDecision(initial, symbol);

	col = result.col;
	row = result.row;
}

Action MinimaxDecision(State initial, char symbol) {
	vector<State> states = vector<State>();

	for (Action a : ACTIONS) {
		if (initial.board.is_legal_move(a.col, a.row, symbol)) {
			State child = successor(initial, a, symbol);
			int utility = MinValue(child, symbol);
			child.utility = utility;
			states.push_back(child);
		}
	}

	State max = initial;
	for (State s : states) {
		if (max.utility < s.utility) {
			max = s;
		}
	}

	return max.actionUsed;
}

int MaxValue(State state, char symbol) {
	symbol = changeTurn(symbol);

	if (TerminalTest(state, symbol)) {
		return state.utility;
	}

	int v = -100000;

	for (Action a : ACTIONS) {
		if (state.board.is_legal_move(a.col, a.row, symbol)) {
			State succ = successor(state, a, symbol);
			int u = MaxValue(succ, symbol);
			if (u > v) {
				v = u;
			}
		}
	}

	return v;
}

int MinValue(State state, char symbol) {
	symbol = changeTurn(symbol);

	if (TerminalTest(state, symbol)) {
		return state.utility;
	}

	int v = 100000;

	for (Action a : ACTIONS) {
		if (state.board.is_legal_move(a.col, a.row, symbol)) {
			State succ = successor(state, a, symbol);
			int u = MaxValue(succ, symbol);
			if (u < v) {
				v = u;
			}
		}
	}

	return v;
}

bool TerminalTest(State state, char symbol) {
	bool isTerminal = true;
	for (Action a : ACTIONS) {
		if (state.board.is_legal_move(a.col, a.row, symbol)) {
			isTerminal = false;
			break;
		}
	}
	return isTerminal;
}

char changeTurn(char symbol) {
	if (symbol == 'X') {
		return 'O';
	}
	else {
		return 'X';
	}
}

State successor(State parent, Action action, char symbol) {
	State childState = newState(parent.board, (parent.utility + 1), &parent, action);
	childState.board.play_move(action.col, action.row, symbol);
	childState.utility += (childState.board.count_score(symbol) - childState.board.count_score(opponent));

	return childState;
}

State newState(OthelloBoard board, int utility, State* parent, Action action) {
	State s = { board: OthelloBoard(board), utility: utility, parent: parent, actionUsed: action };
	return s;
}

State newState(State* s) {
	State dup = { board: OthelloBoard(s->board), utility: s->utility, parent: s->parent, actionUsed: s->actionUsed };
	return dup;
}

MinimaxPlayer* MinimaxPlayer::clone() {
	MinimaxPlayer* result = new MinimaxPlayer(symbol);
	return result;
}
