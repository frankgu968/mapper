/*
 * TerminalInput.h
 *
 *  Created on: 14/03/2016
 *      Author: william
 */

#ifndef TERMINALINPUT_H
#define TERMINALINPUT_H

#include <iostream>
#include <string>
// to disable echoing on terminal
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#include "UserInstruction.h"
#include "m3.h"
#include "DisplayProcessor.h"
#include "mapProcessor.h"

using std::cout;
using std::endl;
using std::cin;

/**
 * Defines global variables for communicating between input and graphics
 */
namespace TerminalInterface {
	extern bool highlightedDirty;
	extern bool intersectionSelected;
	extern PointXY startXY;
	extern PointXY endXY;
	extern std::vector<unsigned int> highlightedSegs;
}

/**
 * Reads from terminal, instantiates UserInstructions to process the input
 */
void readFromTerminal();

/**
 * Give the user instructions on how to use the application
 */
void showHelpMessage();

#endif /* TERMINALINPUT_H */
