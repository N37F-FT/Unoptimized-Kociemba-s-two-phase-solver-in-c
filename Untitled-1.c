#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

bool checkG1(int statecopy[]);
bool phase1(int statecopy[], int depth, int preF);
bool checkSolved(int statecopy[]);
bool phase2(int statecopy[], int depth, int preF);
void stringToState(void);
void turn(int state[], int face, int rotation);
void copyArray(int des[], int src[]);
void printString(int state[]);
void printcube(int state[]);

char faces[] = {'U', 'R', 'F', 'D', 'L', 'B'};
char rotation[] = {' ', '2', '\''};

int currDepth = 1;

char stringState[54] = "UUUUUUUUURRRRRRRRRFFFFFFFFFDDDDDDDDDLLLLLLLLLBBBBBBBBB";
//char stringState[54] = "LBFUUFUBLDLLURLRRDBDBRFBBDDUFBDDUFBRULRFLFRRLDLFDBUFRU";
//char stringState[54] = "UUUUUUUUUFFFRRRRRRLLLFFFFFFDDDDDDDDDBBBLLLLLLRRRBBBBBB";
//char stringState[54] = "UUUDUUDDUBFLLRLFFLBBRBFBBRLDUDDDDDUURRLRLRRLRFBFFBFBLF";
int state[54];

typedef struct
{
    int face;
    int rotation;
} move;


move solution[18];


int TURN_MAP[6][54] = {
    {  2,  5,  8,  1,  4,  7,  0,  3,  6, 18, 19, 20, 12, 13, 14, 15, 16, 17, 36,
      37, 38, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 45, 46,
      47, 39, 40, 41, 42, 43, 44,  9, 10, 11, 48, 49, 50, 51, 52, 53 }, //U
    {  0,  1, 51,  3,  4, 48,  6,  7, 45, 11, 14, 17, 10, 13, 16,  9, 12, 15, 18,
      19,  2, 21, 22,  5, 24, 25,  8, 27, 28, 20, 30, 31, 23, 33, 34, 26, 36, 37,
      38, 39, 40, 41, 42, 43, 44, 35, 46, 47, 32, 49, 50, 29, 52, 53 }, //R
    {  0,  1,  2,  3,  4,  5,  9, 12, 15, 29, 10, 11, 28, 13, 14, 27, 16, 17, 20,
      23, 26, 19, 22, 25, 18, 21, 24, 38, 41, 44, 30, 31, 32, 33, 34, 35, 36, 37,
       8, 39, 40,  7, 42, 43,  6, 45, 46, 47, 48, 49, 50, 51, 52, 53 }, //F
    {  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 51, 52, 53, 18,
      19, 20, 21, 22, 23, 15, 16, 17, 29, 32, 35, 28, 31, 34, 27, 30, 33, 36, 37,
      38, 39, 40, 41, 24, 25, 26, 45, 46, 47, 48, 49, 50, 42, 43, 44 }, //D
    { 18,  1,  2, 21,  4,  5, 24,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 27,
      19, 20, 30, 22, 23, 33, 25, 26, 53, 28, 29, 50, 31, 32, 47, 34, 35, 38, 41,
      44, 37, 40, 43, 36, 39, 42, 45, 46,  6, 48, 49,  3, 51, 52,  0 }, //L
    { 42, 39, 36,  3,  4,  5,  6,  7,  8,  9, 10,  0, 12, 13,  1, 15, 16,  2, 18,
      19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 17, 14, 11, 33, 37,
      38, 34, 40, 41, 35, 43, 44, 47, 50, 53, 46, 49, 52, 45, 48, 51 } //B
};

//############################################################################################################## starttt ++++

int main(void)
{
    //check if the given string cube state is valid
    int length = strlen(stringState);
    if (length != 54) {
        printf("Error: State String is not long enough. Must be 54 char long\n");
        printf("the string provided is");
        return 1;
    }

    //turn the string into an array
    stringToState();

    printString(state);
    printcube(state);

    turn(state, 1, 2);
    turn(state, 0, 0);
    turn(state, 3, 2);

    //phase 1 ###########################
    printf("###### Phase 1: ######\n");

    //if the cube is already in G1
    if (checkG1(state)) {
        printf("Already in G1");
    }
    else {
        //iterative deepening
        for (;currDepth <= 12; currDepth++) {
            printf("Depth: %i\n", currDepth);
            if (phase1(state, currDepth, -1)) {
                break;
            }
        }
    }
    //print solution
    for (int i = currDepth - 1; i > 1; i--) {
        printf("%c%c ", faces[solution[i].face], rotation[solution[i].rotation]);
    }
    printf("\n");

    //phase 2 ###########################
    printf("###### Phase 2: ######\n");

    //if the cube is already solved
    if (checkSolved(state)) {
        printf("Already solved");
        return 0;
    }
    else {
        //iterative deepening
        for (currDepth = 1; currDepth <= 18; currDepth++) {
            printf("Depth: %i\n", currDepth);
            if (phase2(state, currDepth, -1)) {
                printf("Solution: ");
                break;
            }
        }
    }
    //print solution
    for (int i = currDepth - 1; i > 1; i--) {
        printf("%c%c ", faces[solution[i].face], rotation[solution[i].rotation]);
    }
    printf("\n");
    return 0;
}

//############################################################################################################## enddd ----

bool phase1(int statecopy[], int depth, int preF)
{
    if (checkG1(statecopy)) {
        copyArray(state, statecopy);
        return true;
    }
    if (depth <= 0)
        return false;
    
    // for every face turn f, where 0 is U, 1 is R, than F, D, L and than B.
    // and for every rotation r where 0 is 90 degree, 1 is 180, and 2 is 270
    for (int f = 0; f < 6; f++){

        // prune
		// to insure that the same fcae wont be turn twice
		// prune, moves like U D is the same as D U, eleminate one of them
		if (f == preF || f == preF + 3)
			continue;
        
        for (int r = 0; r < 3; r++) {
            //copy array
            int after[54];
            copyArray(after, statecopy);

            //apply turn
            turn(after, f, r);


            if (phase1(after, depth - 1, f)) {
                solution[depth - 1].face = f;
                solution[depth - 1].rotation = r;
                return true;
            }
        }
    }
    
    return false;
}

bool checkG1(int statecopy[])
{   
    //top and bottom facelets
    for (int f = 0; f < 2; f++) {
        for (int i = 0; i < 9; i++) {
            if (!(statecopy[i + 27*f] == 0 || statecopy[i + 27*f] == 3))
                return false;
        }
    }
    //middle edges
    if (!(statecopy[21] == 2 || statecopy[21] == 5))
		return false;
	if (!(statecopy[23] == 2 || statecopy[23] == 5))
		return false;
	if (!(statecopy[48] == 2 || statecopy[48] == 5))
		return false;
	if (!(statecopy[50] == 2 || statecopy[50] == 5))
		return false;
	return true;
}

//#################################################################

bool phase2(int statecopy[], int depth, int preF)
{
    if (checkSolved(statecopy))
        return true;
    if (depth <= 0)
        return false;
       
    // for every face turn f, where 0 is U, 1 is R, than F, D, L and than B.
    // and for every rotation r where 0 is 90 degree, 1 is 180, and 2 is 270
    for (int f = 0; f < 6; f++){

        // prune
		// to insure that the same fcae wont be turn twice
		// prune, moves like U D is the same as D U, eleminate one of them
		if (f == preF || f == preF + 3)
			continue;
        
        if (f == 3 || f == 0) {
            for (int r = 0; r < 3; r++) {
                //copy array
                int after[54];
                copyArray(after, statecopy);

                //apply turn
                turn(after, f, r);


                if (phase2(after, depth - 1, f)) {
                    solution[depth - 1].face = f;
                    solution[depth - 1].rotation = r;
                    return true;
                }
            }
        }
        else {
            //copy array
            int after[54];
            copyArray(after, statecopy);

            //apply turn, only double moves
            turn(after, f, 1);

            if (phase2(after, depth - 1, f)) {
                solution[depth - 1].face = f;
                solution[depth - 1].rotation = 1;
                return true;
            }
        }
    }
    return false;
}

bool checkSolved(int statecopy[])
{
    for (int f = 0; f < 6; f++) {
        for (int i = 0; i < 9; i++) {
            if (statecopy[i + 9*f] != f)
                return false;
        }
    }
    return true;
}

//##############################################################################################################

void turn(int state[], int face, int rotation)
{
    int temp[54];
    for (int i = 0; i < 54; i++)
        temp[i] = state[i];
    if (rotation == 0) {
        for (int i = 0; i < 54; i++)
            state[TURN_MAP[face][i]] = temp[i];
    }
    else if (rotation == 1) {
         turn(state, face, 0);
         turn(state, face, 0);
    }
    else {
         for (int i = 0; i < 54; i++)
            state[i] = temp[TURN_MAP[face][i]];
    }
}

// turn the string state to int
void stringToState(void)
{
        for (int i = 0; i < 54; i++) {
        switch (stringState[i]) {
            case 'U':
                state[i] = 0;
                break;
            case 'R':
                state[i] = 1;
                break;
            case 'F':
                state[i] = 2;
                break;
            case 'D':
                state[i] = 3;
                break;
            case 'L':
                state[i] = 4;
                break;
            case 'B':
                state[i] = 5;
                break;
            default:
                printf("Error: Invalid character in the string state.\n");
                return;
        }
    }
}

void copyArray(int des[], int src[])
{
    for (int i = 0; i < 54; i++)
        des[i] = src[i];
}

void printString(int state[])
{
    for (int i = 0; i < 54; i++) {
        int temp = state[i];
        printf("%c", faces[temp]);
    }
    printf("\n");
}

void printcube(int state[])
{
    //top face
    int facetemp[4] = {4, 2, 1, 5};
    int temp = 0;
    for (int i = 0; i < 3; i++) {
        printf("      ");
            for (int j = 0; j < 3; j++) {
                temp = state[j + 3*i];
                printf("%c ", faces[temp]);
        }
        printf("\n");
    }

    // left, front, back
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 3; k++) {
                temp = state[facetemp[j]*9 + 3*i + k];
                printf("%c ", faces[temp]);
            }
        }
        printf("\n");
    }
    //down
    for (int i = 0; i < 3; i++) {
        printf("      ");
            for (int j = 0; j < 3; j++) {
                temp = state[27 + j + 3*i];
                printf("%c ", faces[temp]);
        }
        printf("\n");
    }
}