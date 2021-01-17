/*
	Program:	Project 5
	Name:		Yee Chan, Jason Novillo, and Dimitri Herr
	Date:		04/30/2019
	Purpose:	Turing machine
*/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
using namespace std;

//Simulates a state machine
struct stateTrans
{
	char state;
	char readChar;
	char nextState;
	char writeChar;
	char move;
};

void turingMachine(stateTrans[], string, char[], int, int, char, char, ofstream&);		//prototype for turing()

int main() 
{	
	const int MAXTAPE = 100;								//Constants used for state struct
	const int MAXSTATES = 25;								//^^^^^^^
	const short STARTPOS = 40;								//^^
	const char STARTSTATE = '0';							//^^
	string fileName;										//Name of the file
	string wTape;											//used to save the tape
	ifstream input;											//input file
	ofstream output("output.txt");							//output file
	struct stateTrans states[MAXSTATES];					//creates the array of instructions
	char state, readChar, nextState, writeChar, move;		//following used to input instructions
	char maxState = '0';									//keeps track of the last state					
	char tape[MAXTAPE];										//represents the tape
	int slot = 0;											//each slot represents an instruction	
	int lTape;												//saves the length of the tape
	
	//For loop initializes the tape to be blank.
	for (int i = 0; i < MAXTAPE; i++)
		tape[i] = 'B';

	//Runs until user enters a correct file name
	do
	{
		//Prompt for file name
		cout << "Enter the name of the file: ";
		getline(cin, fileName);
		input.open(fileName.c_str());//attempts to open file
		//Outputs based on failure
		if (input.fail())
			cout << "File not found, try again" << endl;
	} while (input.fail());

	//Inputs the tape
	getline(input, wTape);

	//saves the tape to the char array
	for (int i = 0; i < wTape.length(); i++)
		tape[STARTPOS + i] = wTape[i];
	lTape = wTape.length();//saves the tape length
	//Saves the instructions from the file
	while (!input.eof())
	{
		input >> state >> readChar >> nextState >> writeChar >> move;	//inputs each value
		states[slot].state = state;										//saves the corresponding values
		states[slot].readChar = readChar;								//^^
		states[slot].nextState = nextState;								//^^
		states[slot].writeChar = writeChar;								//^^
		states[slot].move = move;										//^^
		//Finds the max state if the new slot has a greater max state
		if (states[slot].state > maxState)
			maxState = states[slot].state;
		slot++;//goes to the next slot
	}
	/*
		Outputs stats
	*/
	//Creates the header
	output << "\nTape starts at " << STARTPOS << " length = " << lTape << " highlights position " << STARTPOS << " tape =" << endl;
	//Shows first character and blank
	output << "  B  |" << wTape[0] << "|";
	//Outputs rest of the tape
	for (int i = 1; i < wTape.length(); i++)
		output << "  " << wTape[i];
	//Outputs the final character in the tape.
	output << "  B" << endl << endl;
	//For loop outputs the instructions for the turing machine.
	for (int i = 0; i < slot; i++)
	{
		//Outputs the instructions for the turing machine.
		output << states[i].state << " ";
		output << states[i].readChar << " ";
		output << states[i].nextState << " ";
		output << states[i].writeChar << " ";
		output << states[i].move << endl;
	}

	turingMachine(states, wTape, tape, STARTPOS, 0, STARTSTATE, maxState, output);//Calls the turing machine
	output << "  B" << endl << endl;//outputs blank and new line
	cout << endl << "Output found at output.txt" << endl;
	system("pause");
	return 0;
}

//Turing machines needs the states, the string version of the tape, the tape, the index on the tape where to start, start slot at 0 to begin,
//the lastState, and an output file
void turingMachine(stateTrans states[], string wTape, char tape[], int tapeIndex, int sSlot, char state, char lastState,ofstream &output)
{
	//If the current state equals the other state, then commence
	if (states[sSlot].state == state)
	{
		//Reads through the tape using the current slot's readchar
		//Write if it finds one, moves to next state if not found
		if (states[sSlot].readChar == tape[tapeIndex])
		{
			state = states[sSlot].nextState;			//goes to nextstate
			tape[tapeIndex] = states[sSlot].writeChar;	//write character to tape
			//Tests to move left or right
			if (states[sSlot].move == 'R')
				tapeIndex++;//goes right
			else
				tapeIndex--;//goes left
			sSlot = 0;
			turingMachine(states, wTape, tape, tapeIndex, sSlot, state, lastState,output);//goes through turing again
		}
		else
		{
			sSlot++;//goes to the next slot
			turingMachine(states, wTape, tape, tapeIndex, sSlot, state, lastState,output);//goes through turing again
		}
	}
	else if (states[sSlot].state > lastState)
	{
		//Outputs header
		output << "\nExecution halts in state " << state << endl;
		output << "Tape starts at " << sSlot << " length = " << wTape.length() << " highlights position " << state << " tape =" << endl << "  B";

		//Outputs tape
		for (int i = 40; i < 40 + wTape.length(); i++)
		{
			if (tapeIndex == i)
				output << "  |" << tape[i] << "|";
			else
				output << "  " << tape[i];
		}
		output << "  B" << endl << endl;
		return;//ends the function
	}
	else
	{
		sSlot++;//goes to the next slot
		turingMachine(states, wTape, tape, tapeIndex, sSlot, state, lastState,output);//goes through turing again
	}
}