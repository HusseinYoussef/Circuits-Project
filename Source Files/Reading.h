
#include <string>
#include <iostream>
#include <conio.h>
#include <cmath>
using namespace std;

void Print_Node_Elements(float**, bool&, int);
void Choose_Element(int, int);
void Scroll_Up(int&);
void Scroll_Down(int&);
void Get_Out(bool, bool&, int&, int&);
void Enter_Data(int&, int&, float&, int);
void Store_Data(float **, int&, int, int, float, int);
void Print_All_Elements(float**);
void Delete_Table(float** Data_Table);
void Delete_Node(float**, int);
void Connect_Node(float**, int);

//////////////////////////////////////////////////////////////////// Checking on inputs functions.

bool Check_Input(float **, int, int, float, int, int, string&);     // To check after enter one element.
void One_Node_Check(float **, int);                        // To check after enter all elements.
string Error(int, float**, int);
