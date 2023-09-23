#include <iostream>
#include <fstream>
#include <string>
#include <climits>
#include <cctype> 
#include <cstring>
#include <utility>
#include <vector>

using namespace std;


int main() {
	char c, tokenchar;
	string tokentype, tokenval, identifier;
	unsigned int numLine = 1, numCol = 1, tempCol = numCol, numErrors=0;
	bool scan = true, error = false;
	bool stringOn = false;

	vector<pair<int, int>> errores;

	ifstream archivo("code.txt");
	if (archivo.is_open()) {
		while (archivo.get(c)) {
			//cout << c;
			tempCol = numCol;
			scan = true;
			if (c == ' ' || c == '\t')
			{
				if (!identifier.empty())
				{
					scan = true;
					tokentype = "IDENTIFIER";
					tokenval = identifier;
					tempCol -= identifier.size() + numErrors;
					identifier.clear();
				}
				else {
					scan = false;
					
				}
			}
			else if (c == '\n')
			{
				numLine++;
				numCol = 0;
				scan = false;
				numErrors = 0;
			}
			else if (c == '(' || c == ')' || c == '{' || c == '}' || c == '+' ||
				c == '-' || c == '/' || c == '%' || c == ';')//Single-character
			{
				tokenchar = c;
				switch (tokenchar)
				{
				case '(':tokentype = "OPEN_BRACES"; break;
				case ')':tokentype = "CLOSE_BRACES"; break;
				case '{':tokentype = "OPEN_BLOCK"; break;
				case '}':tokentype = "CLOSE_BLOCK"; break;
				case '+':tokentype = "SUM_OPERATOR"; break;
				case '-':tokentype = "SUBS_OPERATOR"; break;
				case '/':tokentype = "DIV_OPERATOR"; break;
				case '%':tokentype = "MOD_OPERATOR"; break;
				case ';': {
					tokentype = "SEMICOLON";
					if (!identifier.empty())
					{
						cout << "DEBUG SCAN - " << "IDENTIFIER" << "[" << identifier << "]"
							<< " encontrado en (" << numLine << ":" << tempCol-identifier.size() << ")" << endl;
						identifier.clear();
					}
					break; }
				}
				tokenval = string(1, c);
			}
			else if (c == '=' || c == '<' || c == '>' || c == '!')//Two-char operators
			{
				char temp;
				tokenchar = c;
				switch (tokenchar)
				{
				case '=': {
					if ((temp = archivo.peek()) == '=') {
						tokentype = "EQUAL";
						archivo.ignore(1);
						tokenval = string(1, tokenchar) + string(1, temp);
						numCol++;
					}
					else {
						tokentype = "ASSIGN";
						tokenval = string(1, tokenchar);
					} break;
				}
				case '<': {
					if ((temp = archivo.peek()) == '=') {
						tokentype = "LESS_EQUAL_THAN";
						archivo.ignore(1);
						tokenval = string(1, tokenchar) + string(1, temp);
						numCol++;
					}
					else {
						tokentype = "LESS_THAN";
						tokenval = string(1, tokenchar);
					} break;
				}
				case '>': {
					if ((temp = archivo.peek()) == '=') {
						tokentype = "GREATER_EQUAL_THAN";
						archivo.ignore(1);
						tokenval = string(1, tokenchar) + string(1, temp);
						numCol++;
					}
					else {
						tokentype = "GREATER_THAN";
						tokenval = string(1, tokenchar);
					}  break;
				}
				case '!': {
					if ((temp = archivo.peek()) == '=') {
						tokentype = "DIFFERENT_FROM";
						archivo.ignore(1);
						tokenval = string(1, tokenchar) + string(1, temp);
						numCol++;
					}
					else {
						tokentype = "NOT_OPERATOR";
						tokenval = string(1, tokenchar);
					}break;
				}

				}
			}

			else if (c == '#')//Comment
			{
				tokenchar = c;
				tokentype = "COMMENTARY";
				tokenval = string(1, tokenchar) + "...";
				archivo.ignore(INT_MAX, '\n');
				scan = false;
				cout << "DEBUG SCAN - " << tokentype << "[" << tokenval << "]"
					<< " encontrado en (" << numLine << ":" << numCol << ")" << endl;
				numLine++;
				numCol = 0;
			}
			else if (c == '"')//String literal
			{
				if (!stringOn) {
					tokenchar = c;
					tokentype = "STRING_LITERAL";
					tokenval = string(1, tokenchar) + "...";
					archivo.ignore(INT_MAX, '"');
					scan = false;
					cout << "DEBUG SCAN - " << tokentype << "[" << tokenval << "]"
						<< " encontrado en (" << numLine << ":" << numCol << ")" << endl;
					stringOn = false;
				}
				
			}

			else if (isdigit(c))//Digit
			{
				tokenchar = c;
				tokentype = "NUMBER";
				tokenval = string(1, tokenchar);
			}

			else if (isalpha(c))//id, reserved words
			{
				//if
				if (c == 'i')
				{
					
					if (archivo.peek() == 'f')
					{
						tokenchar = c;
						tokentype = "IF";
						tokenval = string(1, tokenchar) + 'f';
						archivo.ignore(1);
						numCol++;
					}
					else
					{
						identifier += c;
						scan = false;
					}

				}
				//else
				else if (c == 'e')
				{
					char b[4];
					std::streampos originalPos = archivo.tellg();
					for (auto i = 0; i < 4-1; i++)
					{
						b[i] = archivo.peek();;
						archivo.seekg(1, std::ios::cur);
					}
					b[3] = '\0';
					if (strcmp(b, "lse") == 0)
					{
						tokenchar = c;
						tokentype = "ELSE";
						tokenval = string(1, tokenchar) + "lse";
						numCol += 3;
					}
					else {
						archivo.seekg(originalPos);
						identifier += c;
						scan = false;
					}
				}
				//while
				else if (c == 'w')
				{
					char b[5];
					std::streampos originalPos = archivo.tellg();
					for (auto i = 0; i < 5 - 1; i++)
					{
						b[i] = archivo.peek();;
						archivo.seekg(1, std::ios::cur);
					}
					b[4] = '\0';
					if (strcmp(b, "hile") == 0)
					{
						tokenchar = c;
						tokentype = "WHILE";
						tokenval = string(1, tokenchar) + "hile";
						numCol += 4;
					}
					else {
						//archivo.seekg(-4, ios::cur);
						archivo.seekg(originalPos);
						identifier += c;
						scan = false;
					}
				}
				//Int
				else if (c == 'I')
				{
					char b[3];
					std::streampos originalPos = archivo.tellg();
					for (auto i = 0; i < 3 - 1; i++)
					{
						b[i] = archivo.peek();;
						archivo.seekg(1, std::ios::cur);
					}
					b[2] = '\0';
					if (strcmp(b, "nt") == 0)
					{
						tokenchar = c;
						tokentype = "DATA_INT";
						tokenval = string(1, tokenchar) + "nt";
						numCol += 2;
					}
					else {
						archivo.seekg(originalPos);
						identifier += c;
						scan = false;
					}
				}
				//String
				else if (c == 'S')
				{
					char b[6];
					std::streampos originalPos = archivo.tellg();
					for (auto i = 0; i < 6 - 1; i++)
					{
						b[i] = archivo.peek();;
						archivo.seekg(1, std::ios::cur);
					}
					b[5] = '\0';
					if (strcmp(b, "tring") == 0)
					{
						tokenchar = c;
						tokentype = "DATA_STRING";
						tokenval = string(1, tokenchar) + "tring";
						numCol += 5;
					}
					else {
						archivo.seekg(originalPos);
						identifier += c;
						scan = false;
					}
				}
				//Bool
				else if (c == 'B')
				{
					char b[4];
					std::streampos originalPos = archivo.tellg();
					for (auto i = 0; i < 4 - 1; i++)
					{
						b[i] = archivo.peek();;
						archivo.seekg(1, std::ios::cur);
					}
					b[3] = '\0';
					if (strcmp(b, "ool") == 0)
					{
						tokenchar = c;
						tokentype = "DATA_BOOL";
						tokenval = string(1, tokenchar) + "ool";
						numCol += 3;
					}
					else {
						archivo.seekg(originalPos);
						identifier += c;
						scan = false;
					}
				}
				//true
				else if (c == 't')
				{
					char b[4];
					std::streampos originalPos = archivo.tellg();
					for (auto i = 0; i < 4 - 1; i++)
					{
						b[i] = archivo.peek();;
						archivo.seekg(1, std::ios::cur);
					}
					b[3] = '\0';
					if (strcmp(b, "rue") == 0)
					{
						tokenchar = c;
						tokentype = "BOOLEAN_TRUE";
						tokenval = string(1, tokenchar) + "rue";
						numCol += 3;
					}
					else {
						archivo.seekg(originalPos);
						identifier += c;
						scan = false;
					}
				}
				//false
				else if (c == 'f')
				{
					char b[5];
					std::streampos originalPos = archivo.tellg();
					for (auto i = 0; i < 5 - 1; i++)
					{
						b[i] = archivo.peek();;
						archivo.seekg(1, std::ios::cur);
					}
					b[4] = '\0';
					if (strcmp(b, "alse") == 0)
					{
						tokenchar = c;
						tokentype = "BOOLEAN_FALSE";
						tokenval = string(1, tokenchar) + "alse";
						numCol += 4;
					}
					else {
						archivo.seekg(originalPos);
						identifier += c;
						scan = false;
					}
				}
				else {
					identifier += c;
					scan = false;
				}
			}
			else {
				errores.emplace_back(numLine, numCol);
				scan = false;
				numErrors++;
			}

			if (scan) cout << "DEBUG SCAN - " << tokentype << "[" << tokenval << "]"
				<< " encontrado en (" << numLine << ":" << tempCol << ")" << endl;
			numCol++;
		}
		archivo.close();
	}
	else {
		cout << "No se pudo abrir el archivo";
	}

	if (!errores.empty())
	{
		cout << "ERRORES DETECTADOS\n";
		for (auto i : errores)
			cout << "-> fila: " << i.first << ", columna: " << i.second << endl;
	}

	
	return 0;
}