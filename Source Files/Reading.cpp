#include "Reading.h"

void One_Node_Check(float ** Data_Table, int size)		//A function checks if an element connected to one node.
{
	bool One_Node;

	if (size == 1)
		Error(3, Data_Table, 0);
	else {
		for (int i = 0; i<size; i++) {
			One_Node = true;
			for (int j = 0; j<size; j++) {
				if (i != j && Data_Table[0][i] == Data_Table[0][j] && Data_Table[1][i] == Data_Table[1][j])
					One_Node = false;		//This item is correct (connected to more than one node).
			}
			if (One_Node)
				Error(3, Data_Table, i);
		}
	}
}

void Print_Node_Elements(float** Data_Table, bool &found, int Node_Num) {
	cout << "*** Node " << Node_Num << " ***" << endl;
	found = false;

	for (int column = 0; column<100; column++) {
		if (Data_Table[3][column] == Node_Num) {
			if (!found) cout << "The elements connected to this node : " << endl;
			found = true;
			cout << char(254);
			if (Data_Table[0][column] == 1) cout << " R";
			else if (Data_Table[0][column] == 2) cout << " E";
			else if (Data_Table[0][column] == 3) cout << " J";
			cout << Data_Table[1][column] << " = " << Data_Table[2][column];
			if (Data_Table[0][column] == 1) cout << " Ohm";
			else if (Data_Table[0][column] == 2) cout << " Volt";
			else if (Data_Table[0][column] == 3) cout << " Ampere";
			cout << endl;

		}
	}

	if (!found) cout << " - This node is not connected yet -" << endl;
	cout << "________________________________________________________________________________________________" << endl;
}

void Choose_Element(int choise, int Node_Num) {
	cout << endl << endl << "Choose the type of the element by pressing 'W' or 'S' : " << endl;
	if (choise == 1)			cout << "==>  R (Resistance)" << endl;
	else if (choise == 2)	cout << "==>  E (Voltage source)" << endl;
	else if (choise == 3)	cout << "==>  J (Current source)" << endl;

	cout << "(Press 'ENTER' to choose or 'Esc' when finished node " << Node_Num << " or 'ESC' twice when finished all nodes)" << endl << endl;

}

void Store_Data(float ** Data_Table, int& column_index, int Element_Type, int Element_Num, float Element_Value, int Node_Num) {
	Data_Table[0][column_index] = float(Element_Type);					//Storing the element's data in the table.
	Data_Table[1][column_index] = float(Element_Num);					//
	Data_Table[2][column_index] = Element_Value;						//
	Data_Table[3][column_index] = float(Node_Num);						//
	column_index++;					//To store the next element in the next column.
}

void Print_All_Elements(float** Data_Table) {
	for (int row = 0; row < 4; row++) {
		for (int column = -1; column < 100; column++) {
			if (Data_Table[0][column] != 0) {
				if (column != -1)
					cout << Data_Table[row][column] << "   ";
				else {
					if (row == 0) cout << "Element Type   : ";
					else if (row == 1) cout << "Element Number : ";
					else if (row == 2) cout << "Element Value  : ";
					else if (row == 3) cout << "Node Number    : ";
				}
			}
		}
		cout << endl;
	}
	cout << endl << endl;
}

void Scroll_Up(int& choise) {
	if (choise == 3) choise = 1;
	else choise++;
}

void Scroll_Down(int& choise) {
	if (choise == 1) choise = 3;
	else choise--;
}

void Get_Out(bool found, bool& FinishedAllNodes, int& Node_Num, int& choise) {
	if (found) {							    	//If the node is connected close it , if not exit all nodes.
		Node_Num++;							//Moving to the next node.
		choise = 1;							//Resetting the choise to resistance.
	}
	else
		FinishedAllNodes = true;			//To exit the loop.
}

void Enter_Data(int& Element_Type, int& Element_Num, float& Element_Value, int choise) {
	Element_Type = choise;					//1 = resistance , 2 = voltage source , 3 = current source. 

	cout << "Please enter the number of the element : ";		//Getting the number of the element.
	float Temp_Element_Num;
	cin >> Temp_Element_Num;								//
	Element_Num = floor(Temp_Element_Num);

	cout << "Now enter the value in ";						//Getting the value of the element.
	if (Element_Type == 1)		cout << "Ohms            : ";				//
	else if (Element_Type == 2)	cout << "Volts           : ";				//
	else if (Element_Type == 3)	cout << "Amperes         : ";			//
	cin >> Element_Value;										//

}

void Delete_Table(float** Data_Table) {
	for (int i = 0; i<4; i++)
		delete[] Data_Table[i];
	delete[] Data_Table;
}

bool Check_Input(float **Dataa_Table, int Element_Number, int Element_Type, float Value, int Column_Index, int Node_Number, string& MSG)                      // law el element zahr fi two nodes be nafs el eshara
{
	// We have these cases
	// 1- An element has 2 different values.
	// 2- Current or Voltage source has 2 different signs.
	// 3- If there is an element connected to one node twice.
	// 4- If R = -ve.
	// 5- If Element = 0.
	// 6- If ID = -ve.
	if (Value == 0)													// If Element value = 0.
	{
		MSG = Error(4, NULL, NULL);
		return false;
	}
	else if (Element_Type == 1 && Value < 0)						// If a resistor = -ve.
	{
		MSG = Error(5, NULL, NULL);
		return false;
	}
	else if (Element_Number < 0)									// If a ID = -ve.
	{
		MSG = Error(6, NULL, NULL);
		return false;
	}

	for (int i = (Column_Index - 1); i >= 0; i--)
	{
		if (Dataa_Table[0][i] == Element_Type && Dataa_Table[1][i] == Element_Number)
		{
			if (Dataa_Table[3][i] == Node_Number)							// If there is element connected to one node twice.
			{
				MSG = Error(0, NULL, NULL);
				return false;
			}
			else if (fabs(Dataa_Table[2][i]) != fabs(Value))				// If there is an element with 2 different values.
			{
				MSG = Error(1, NULL, NULL);
				return false;
			}
			else if (Element_Type != 1 && Dataa_Table[2][i] == Value)		// If there is source appears with the same sign twice.
			{
				MSG = Error(2, NULL, NULL);
				return false;
			}

		}
	}

	return true;
}

void Delete_Node(float** Data_Table, int column_index) {
	Data_Table[0][column_index] = 0;
	cout << char(251) << " Deleted succesfully!" << endl << endl;
}

void Connect_Node(float** Data_Table, int column_index) {
	int Node_Num;
	cout << "Enter the number of the second node : " << endl;
	cin >> Node_Num;
	int First_Empty_Column = 0;

	while (true)
		if (Data_Table[0][First_Empty_Column] == 0)
			break;
		else
			First_Empty_Column++;

	Data_Table[0][First_Empty_Column] = Data_Table[0][column_index];
	Data_Table[1][First_Empty_Column] = Data_Table[1][column_index];
	Data_Table[2][First_Empty_Column] = Data_Table[2][column_index];
	Data_Table[3][First_Empty_Column] = Node_Num;

	cout << char(251) << " Connected succesfully!" << endl << endl;

}

int Matrix_Size(int Essential_Nodes_Num) {
	return Essential_Nodes_Num - 1;
}

string Error(int num, float** Data_Table, int Element_index) {
	if (num == 0)
		return "****** You have connected this element to this node before.";
	else if (num == 1)
		return "****** You have connected this element with another value before";
	else if (num == 2)
		return "****** This source has appeared with the same sign twice";
	else if (num == 3)
	{
		cout << "  *******  The element ";
		if (Data_Table[0][Element_index] == 1)
			cout << "R";
		else if (Data_Table[0][Element_index] == 2)
			cout << "E";
		else if (Data_Table[0][Element_index] == 3)
			cout << "J";
		cout << Data_Table[1][Element_index] << " is connected only to node " << Data_Table[3][Element_index] << "  ******** " << endl;
		cout << "To delete press 'D' ,to connect it to another node press 'C'." << endl;

		while (true) {
			char press = _getch();
			if (press == 'd' || press == 'D') {
				Delete_Node(Data_Table, Element_index);
				break;
			}
			else if (press == 'c' || press == 'C') {
				Connect_Node(Data_Table, Element_index);
				break;
			}
		}
		return "";
	}
	else if (num == 4)			// If value = 0.
		return "****** Elements values can't be assigned to 0";
	else if (num == 5)			// R = -ve.
		return "****** Resistors can't be assigned to -ve values";
	else if (num == 6)			// If a ID = -ve.
		return "****** Elements numbers can't be assigned to -ve values";

}
