#include "Reading.h"
#include "CircuitSimulator.h"

int Req_List(int list_el,int sub_list,float** Data_Table,int column_index,int Node_Num,int ground_index);
void Display_List(int list_el,int sub_list);

int main() {
	repeat1:
	float **Data_Table = new float*[4];		//For making an array of pointers to pointers (2D array).
	for (int i = 0; i<4; i++)					//With 4 rows and 100 columns.
		Data_Table[i] = new float[100];		//100 : the number of the elements & 4 : number of data inputs for each.

	for (int i = 0; i<100; i++)
		Data_Table[0][i] = 0;

	int  choise = 1, column_index = 0;
	bool FinishedAllNodes = false, found = false;

	// Ramy :  ba3d ma neda5l kol el input bashof law fe element connected to node wa7da law el kalam dah true ba3eed  ed5al kol el inputs .

	int Element_Type, Element_Num, Node_Num = 1;				//The data we need to store for every element.
	float Element_Value;
	string Error_Message;
	bool error = false;

	while (!FinishedAllNodes) {
		system("cls");				//For clearing the screen at each key press.
		if (error) {
			cout << Error_Message << endl;
			error = false;
		}
		Print_Node_Elements(Data_Table, found, Node_Num);
		Choose_Element(choise, Node_Num);
		char press = _getch();						//Getting a char from the user and storing it in 'press'.
		if (press == 'w' || press == 'W')
			Scroll_Up(choise);
		else if (press == 's' || press == 'S')
			Scroll_Down(choise);
		else if (press == 27)						//If you pressed 'Esc' (ASCII Code).
			Get_Out(found, FinishedAllNodes, Node_Num, choise);
		else if (press == 13)      //If you pressed 'Enter'
		{
			Enter_Data(Element_Type, Element_Num, Element_Value, choise);
			if (Check_Input(Data_Table, Element_Num, Element_Type, Element_Value, column_index, Node_Num, Error_Message))   //Checking on input.     
				Store_Data(Data_Table, column_index, Element_Type, Element_Num, Element_Value, Node_Num);				 //Storing the element's data in the table.
			else
				error = true;
		}
	}

	One_Node_Check(Data_Table, column_index);		// Ramy : If true dah ma3nah 5alsna kol el input flazm nem3l check delwa2ty 3la law fi element connected to one node.

	int ground_index;
	bool incorrect = true;

	while(incorrect)
	{
	cout << endl << "Please enter the number of the ground node :\t" ;
	cin >> ground_index;
	cout << endl;
	if(ground_index > 0 && ground_index < Node_Num)
		incorrect = false;
	else
		cout << "This node doesn't exist!" << endl << endl;
	}
	if(Req_List(1,0,Data_Table,column_index,Node_Num,ground_index) == 0)		// This shows the list of requirements. If it returns 0 then the user wants to re-enter the circuit.
		goto repeat1;

	Delete_Table(Data_Table);

	return 0;
}

void Display_List(int list_el,int sub_list)
{
	system("cls");
	cout << "Choose what you want to calculate : " << endl << endl ;

	if(list_el == 1) cout << char(254) << " ";
	else cout << "  ";
	cout << "The current passing through an element." << endl;
	//////////////////////////////// Current sub-list.
	if(sub_list == 11 || sub_list == 12)		// 11 for the 1st in the list & 12 for the 2nd.
	{
		if(sub_list == 11) cout << "\t" << char(254) << " ";
		else cout << "\t  ";
		cout << "The current due to one source." << endl;

		if(sub_list == 12) cout << "\t" << char(254) << " ";
		else cout << "\t  ";
		cout << "The current due to the whole circuit." << endl;
	}
	////////////////////////////////

	if(list_el == 2) cout << char(254) << " ";
	else cout << "  ";
	cout << "The voltage difference between two nodes." << endl;	
	//////////////////////////////// Voltage sub-list.
	if(sub_list == 21 || sub_list == 22)		// 21 for the 1st in the list & 22 for the 2nd.
	{
		if(sub_list == 21) cout << "\t" << char(254) << " ";
		else cout << "\t  ";
		cout << "The voltage due to one source." << endl;

		if(sub_list == 22) cout << "\t" << char(254) << " ";
		else cout << "\t  ";
		cout << "The voltage due to the whole circuit." << endl;
	}
	////////////////////////////////

	if(list_el == 3) cout << char(254) << " ";
	else cout << "  ";
	cout << "The power supplied or dissipated in an element." << endl;

	if(list_el == 4) cout << char(254) << " ";
	else cout << "  ";
	cout << "The maximum resistance & maximum power." << endl;
		
	if(list_el == 5) cout << char(254) << " ";
	else cout << "  ";
	cout << "Power balance check." << endl << endl << endl;

	cout << "(Press 'W','S' to go up & down - 'ENTER' to choose - 'ESC' to exit a sub-list or to re-enter the circuit)" << endl << endl;

}


int Req_List(int list_el,int sub_list,float** Data_Table,int column_index,int Node_Num,int ground_index)
{  
repeat2: 
	Display_List(list_el,sub_list);		// Shows the list of requirements to the user.
	vector<NODE>original_vertix_List(Node_Num);
	Build_Circuit(Data_Table, column_index, original_vertix_List);

	while(true)
	{
		char press = _getch();						//Getting a char from the user and storing it in 'press'.
		if (press == 'w' || press == 'W')
		{
			if(list_el != 0 && list_el != 1)
			{	list_el--;	goto repeat2;}
			else if(sub_list == 12)
			{	sub_list = 11;	goto repeat2;}
			else if(sub_list == 22)
			{	sub_list = 21;	goto repeat2;}

		}
		else if (press == 's' || press == 'S')
		{
			if(list_el != 0 && list_el != 5)
			{	list_el++;	goto repeat2;}
			else if(sub_list == 11)
			{	sub_list=12;	goto repeat2;}
			else if(sub_list == 21)
			{	sub_list=22;	goto repeat2;}

		}
		else if (press == 27)						//If you pressed 'Esc' (ASCII Code).
		{ 
			if(sub_list == 11 || sub_list == 12)
			{	sub_list=0;		list_el=1;	goto repeat2;}
			else if(sub_list == 21 || sub_list == 22)
			{	sub_list=0;		list_el=2;	goto repeat2;}
			else
			{
				cout << "Do you want to re-enter the circuit? (press 'Y' to confirm or any other character to decline)\t\t" ;
				char choise = _getch();
				if(choise == 'Y' ||choise ==  'y')
					return 0;
				else 
				{	sub_list=0;		list_el=1;	goto repeat2;}
			}
		}
		else if (press == 13)      //If you pressed 'Enter'000000000000000000000000000000000000000000000000000000000000000000000
		{
			if(list_el == 1)		// Shows the sub-list of the current option.
			{	sub_list=11;	list_el=0;	goto repeat2;}
			else if(list_el == 2)		// Shows the sub-list of the voltage option.
			{	sub_list=21;	list_el=0;	goto repeat2;}
			else if(sub_list == 11)	 ///////*******************Current through element due to one source************************
			{
				system("cls");
				//vector<NODE>vertix_List(Node_Num);
				//vector<double>voltage(Node_Num);
				//Build_Circuit(Data_Table, column_index, vertix_List);		// Now the vertix_list is the original graph
				//Node_Analysis(vertix_List, voltage, ground_index);			// Now the voltage list contains all voltages accept
				//												
				//int type,ID;
				//cout << "Enter the element type (1 Resistor , 2 Voltage Source , 3 Current Source) : ";
				//cin >> type;
				//cout << "Enter the element number : ";
				//cin >> ID;
				//Element *element = find_element(vertix_List, type, ID);
				//int src_type,src_id;
				//cout << "Enter the source type (2 Voltage Source , 3 Current Source) : ";
				//cin >> src_type;
				//cout << "Enter the source number : ";
				//cin >> src_id;

				///// ramy**************************************
				vector<NODE> vertix_list1(Node_Num);
				Build_Circuit(Data_Table, column_index, vertix_list1);
				vector<NODE> vertix_list2(Node_Num);
				Build_Circuit(Data_Table, column_index, vertix_list2);
				vector<double>voltage;
				cout << " Enter the type of required source    (2-VS   3-CS)" << endl;
				int type=0;
				int id=0;
				cin >> type;
				cout << " Enter the id of required source   " << endl;
				cin >> id;
				Element* element1 =find_element(vertix_list1, type, id);
				Element* element2 = find_element(vertix_list2, type, id);

				if (element1 != NULL)
				{
					superPosition(vertix_list1, vertix_list2, element1,element2 ,voltage, ground_index);
					cout << " Enter the id of the resistor you want to get the current" << endl;
					cin >> id;

					element1 = find_element(vertix_list1, 1, id);
					cout << " Current =  " << get_current(voltage, element1);

				}
				else
				{
					cout << " The element not found" << endl;
				}
				cout << "Press 'ESC' to return to the list." << endl;
				bool back = false;
				while(!back)
				{
					char press = _getch();
					if(press == 27)
					{
						back = true;
						goto repeat2;
					}
				}


			}

			else if(sub_list == 12)	///////*******************Current due to the whole circuit goes here************************
			{
				system("cls");
				vector<NODE>vertix_List(Node_Num);
				vector<double>voltage(Node_Num);
				Build_Circuit(Data_Table, column_index, vertix_List);		// Now the vertix_list is the original graph
				Node_Analysis(vertix_List, voltage, ground_index);			// Now the voltage list contains all voltages accept
				V_Deleted_Nodes(original_vertix_List, voltage, ground_index);
				int type,ID;
				cout << "Enter the element type (1 Resistor , 2 Voltage Source , 3 Current Source) : ";
				cin >> type;
				cout << "Enter the element number : ";
				cin >> ID;
				Element *element = find_element(original_vertix_List, type, ID);
				cout << "The current in the element due to the whole circuit = " << get_current(voltage,element) << endl << endl;
								
				cout << "Press 'ESC' to return to the list." << endl;
				bool back = false;
				while(!back)
				{
					char press = _getch();
					if(press == 27)
					{
						back = true;
						goto repeat2;
					}
				}

			}

			else if(sub_list == 21)	///////*******************Voltage due to one source goes here************************
			{
				system("cls");
				vector<NODE> vertix_list1(Node_Num);
				Build_Circuit(Data_Table, column_index, vertix_list1);
				vector<NODE> vertix_list2(Node_Num);
				Build_Circuit(Data_Table, column_index, vertix_list2);
				vector<double>voltage;
				cout << " enter the type of required source    (2-VS   3-CS)  " << endl;
				int type = 0;
				int id = 0;
				cin >> type;
				cout << " enter the id of required source   " << endl;
				cin >> id;
				Element* element1 = find_element(vertix_list1, type, id);
				Element* element2 = find_element(vertix_list2, type, id);

				if (element1 != NULL)
				{
					superPosition(vertix_list1, vertix_list2, element1, element2, voltage, ground_index);
					int nodee1 = 0; int nodee2 = 0;
					cout << " enter two nodes you want to get voltage difference between them" << endl;
					cin >> nodee1 >> nodee2;
					cout << " voltage  =  " << get_voltage(voltage, nodee1,nodee2);

				}
				else
				{
					cout << " the element not found" << endl;
				}
				cout << "Press 'ESC' to return to the list." << endl;
				bool back = false;
				while(!back)
				{
					char press = _getch();
					if(press == 27)
					{
						back = true;
						goto repeat2;
					}
				}

			}

			else if(sub_list == 22)///////*******************Voltage due to the whole circuit goes here************************
			{
				system("cls");
				vector<NODE>vertix_List(Node_Num);
				vector<double>voltage(Node_Num);
				Build_Circuit(Data_Table, column_index, vertix_List);		// Now the vertix_list is the original graph
				Node_Analysis(vertix_List, voltage, ground_index);			// Now the voltage list contains all voltages accept
				V_Deleted_Nodes(original_vertix_List, voltage, ground_index);
				int node1,node2;
				cout << "Enter the first node : ";
				cin >> node1;
				cout << "Enter the second node : ";
				cin >> node2;
				cout << "The voltage between the two nodes = " << get_voltage(voltage,node1,node2) << endl << endl;
								
				cout << "Press 'ESC' to return to the list." << endl;
				bool back = false;
				while(!back)
				{
					char press = _getch();
					if(press == 27)
					{
						back = true;
						goto repeat2;
					}
				}

			}
			else if(list_el == 3)	///////*******************Power on an element goes here************************
			{
				system("cls");
				vector<NODE>vertix_List(Node_Num);
				vector<double>voltage(Node_Num);
				Build_Circuit(Data_Table, column_index, vertix_List);		// Now the vertix_list is the original graph
				Node_Analysis(vertix_List, voltage, ground_index);			// Now the voltage list contains all voltages accept
				V_Deleted_Nodes(original_vertix_List, voltage, ground_index);
				int type,ID;
				 type = 0;  ID  = 0;
				cout << "Enter the element type ( 1 Resistor , 2 Voltage Source , 3 Current Source) : ";
				cin >> type;
				cout << "Enter the element number : ";
				cin >> ID;
				Element *element = find_element(original_vertix_List, type, ID);
				cout << "The power in the element = " << get_power(voltage,element) << endl << endl;
							
				cout << "Press 'ESC' to return to the list." << endl;
				bool back = false;
				while(!back)
				{
					char press = _getch();
					if(press == 27)
					{
						back = true;
						goto repeat2;
					}
				}

			}
			else if(list_el == 4)	///////*******************Calculate Rmax & Pmax************************
			{
				system("cls");
				vector<NODE> vertix_list1(Node_Num);
				Build_Circuit(Data_Table, column_index, vertix_list1);
				vector<NODE> vertix_list2(Node_Num);
				Build_Circuit(Data_Table, column_index, vertix_list2);
				int type, id;
				type = 1;			// Always resistor.
				id = 0;
				cout << "Enter the number of the resistor you want to get maximum resistance & power for :";
				cin >> id;
				Element *element = find_element(vertix_list1, type, id);
				float Rth = calculate_RTH(vertix_list1, element);
				element = find_element(vertix_list2, type, id);
				float Vth = calculate_VTH(vertix_list2, element);
				cout << "The resistance to recieve maximum power = " << Rth << endl;
				cout << "The voltage    to recieve maximum power = " << Vth << endl;
				cout << "The maximum power for the element = " << pow(Vth,2)/(4*Rth) <<endl << endl << endl;
								
				cout << "Press 'ESC' to return to the list." << endl;
				bool back = false;
				while(!back)
				{
					char press = _getch();
					if(press == 27)
					{
						back = true;
						goto repeat2;
					}
				}

			}
			else if(list_el == 5)	///////*******************Check power balance************************
			{
				system("cls");
				vector<NODE>vertix_List(Node_Num);
				vector<double>voltage(Node_Num);
				Build_Circuit(Data_Table, column_index, vertix_List);		// Now the vertix_list is the original graph
				Node_Analysis(vertix_List, voltage, ground_index);			// Now the voltage list contains all voltages accept
																				//for vanished nodes during transformation.

				V_Deleted_Nodes(original_vertix_List, voltage, ground_index);
				power_balance(voltage, original_vertix_List);

				cout << "Press 'ESC' to return to the list." << endl;
				bool back = false;
				while(!back)
				{
					char press = _getch();
					if(press == 27)
					{
						back = true;
						goto repeat2;
					}
				}

			}

		}
	}
}

