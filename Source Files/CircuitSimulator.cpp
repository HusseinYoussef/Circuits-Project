#include "CircuitSimulator.h"
using namespace Eigen;



void Build_Circuit(float ** Data_Table, int column_index, vector<NODE> &vertices)   // ** By Hassan & Hussein **
{
	map<int, bool>mp;                        // El map bs 3shan n3rf e7na 3dena 3la el Elem da 2bl da wla l2   ***matnso4 el #include <map>
	for (int col = 0; col < column_index - 1; ++col)
	{
		int type = Data_Table[0][col], Num = Data_Table[1][col];           // Temp variables 3shan mktbsh kter :D

		if (mp[type * 100 + Num] == true) continue;			//3lshan mn5odsh element mowgod 2bl keda.

		Element* Ele = new Element;
		mp[type * 100 + Num] = true;							   //aho .. l element dah msh hytkrr tany. ex: element 11 "R1",element 22 "V2"
		Ele->Connectd_Nodes[0] = Ele->Connectd_Nodes[1] = NULL;       // Initilizing the Connected nodes with NULL At first

		if (type == 1) Ele->type = RES;			        	 //---------------------------------------------
		else if (type == 2) Ele->type = VS;		            // El if else dol 3shan el Type  (enum w bta3)
		else Ele->type = CS;                               //-----------------------------------------------

		Ele->visited = false;
		Ele->ID = Data_Table[1][col];
		Ele->Value = fabs(Data_Table[2][col]);

		if (Data_Table[2][col] < 0) Ele->Connectd_Nodes[0] = &vertices[Data_Table[3][col]];
		else Ele->Connectd_Nodes[1] = &vertices[Data_Table[3][col]];

		vertices[Data_Table[3][col]].El_List.push_front(Ele);			// B7ot el Element fe el Corresponding Node in vertix list
		vertices[Data_Table[3][col]].ID = Data_Table[3][col];

		for (int i = col + 1; i < column_index; ++i)
		{
			if (Data_Table[0][i] == type && Data_Table[1][i] == Num)
			{
				if (Ele->Connectd_Nodes[0] == NULL) Ele->Connectd_Nodes[0] = &vertices[Data_Table[3][i]];
				else Ele->Connectd_Nodes[1] = &vertices[Data_Table[3][i]];

				vertices[Data_Table[3][i]].El_List.push_front(Ele);      // B7ot el Element fe el next Node in vertix list
				vertices[Data_Table[3][i]].ID = Data_Table[3][i];
				break;
			}
		}
	}
}

void print(vector<NODE>v)
{
	list<Element*>::iterator j;
	int size = 0;
	for (int i = 1; i < v.size(); ++i)
	{
		if (!(v[i].El_List.size())) continue;       // Empty Node --> Deleted Node

		++size;               // To get the size of the Vertix list without useless Nodes  (First index & Deleted Nodes)
		cout << " Node " << v[i].ID << ": element list size = " << v[i].El_List.size();
		cout << " -> ";
		for (j = v[i].El_List.begin(); j != v[i].El_List.end(); j++)
		{
			cout << "( " << "Elem.ID " << (*j)->ID << " Type " << (*j)->type << " )";
		}
		cout << endl;
	}
	cout << "The Size of Vertix list " << size << endl;
}

int Get_Ground(vector<NODE>v)           // ** By Hassan & Hussein **
{
	int max = 0;
	int indx;
	for (int i = 1; i < v.size(); ++i)
	{
		if (v[i].El_List.size() > max)
		{
			max = v[i].El_List.size();
			indx = i;                          // Node index with max Elements
		}
	}
	return indx;
}

void Node_Analysis(vector<NODE>&v, vector<double>& voltage, int &ground_index)    // ***** By H & H
{
	list<Element*>::iterator j;
	list<Element*>::iterator k;
	//--------------------------------------------------------------
	NODE* RLeg = NULL; NODE* VSLeg = NULL;
	float Rval, VSval;
	for (int i = 1; i < v.size(); ++i)		// Traverse through nodes until finds one between VS & R
	{

		if ((v[i].El_List.size() == 2))     // (v[i] is the middle node) This Node is connected with 1 Res & 1 VS Need to Transform to CS
		{
			if (check_vs_and_Res(v[i]))
			{
				///////************* Storing the outer nodes addresses into (Rleg,VSLeg) & the values of R and VS *************
				for (j = v[i].El_List.begin(); j != v[i].El_List.end(); j++)
				{
					if ((*j)->type == RES)		// This is the R.
					{
						Rval = (*j)->Value;
						if ((*j)->Connectd_Nodes[0] != &v[i])
							RLeg = (*j)->Connectd_Nodes[0];
						else
							RLeg = (*j)->Connectd_Nodes[1];
					}
					else						// This is the VS
					{
						VSval = (*j)->Value;
						if ((*j)->Connectd_Nodes[0] != &v[i])
							VSLeg = (*j)->Connectd_Nodes[0];
						else
							VSLeg = (*j)->Connectd_Nodes[1];
					}
				}


				////////***************** Changing the data inside structs ****************
				////////Changing the node connected to the R from the middle into the VLeg and the same for VS
				////////Adding the elements to the legs nodes
				////////Changin the type and the value of VS

				for (j = v[i].El_List.begin(); j != v[i].El_List.end(); j++)
				{
					if ((*j)->type == RES)		// *j is the R.
					{
						VSLeg->El_List.push_front(*j);			// Put the R in the EL_List of the VSLeg node.
						if ((*j)->Connectd_Nodes[0] == &v[i])		// Connecting the R with the VSLeg.
							(*j)->Connectd_Nodes[0] = VSLeg;		//
						else										//
							(*j)->Connectd_Nodes[1] = VSLeg;		//
					}
					else						// *j is the VS
					{
						RLeg->El_List.push_front(*j);			// Put the VS in the EL_List of the RLeg node.
						(*j)->ID = 555;            // To be able to See it in TEST :D   
						(*j)->type = CS;
						(*j)->Value = VSval / Rval;
						if ((*j)->Connectd_Nodes[0] == &v[i])		// Connecting the VS with the RLeg.
							(*j)->Connectd_Nodes[0] = RLeg;			//
						else										//
							(*j)->Connectd_Nodes[1] = RLeg;			//
					}
				}

				v[i].El_List.clear();  // To delete the El_List connected to the middle node.                  

									   //ground_index = Get_Ground(v);
			}
		}
	}

	cout << "ground index __________________" << ground_index << endl;
	vector<NODE*>Matrix_Nodes;
	for (int i = 1; i < v.size(); ++i)
		if (v[i].El_List.size() > 0 && i != ground_index)
			Matrix_Nodes.push_back(&v[i]);

	int matrix_size = Matrix_Nodes.size();
	//---------------------------------------------------- Eigen
	MatrixXd G(matrix_size, matrix_size);              // Matrix [G]
	VectorXd I(matrix_size);                           // Vector [I]         
	VectorXd V(matrix_size);                           // Vector [V]

	for (int i = 0; i < Matrix_Nodes.size(); ++i)
	{
		I(i, 0) = 0;         // initialize          Vector for the Current [I]
		for (int k = i; k < Matrix_Nodes.size(); ++k)
		{
			G(i, k) = G(k, i) = 0;
			for (j = Matrix_Nodes[i]->El_List.begin(); j != Matrix_Nodes[i]->El_List.end(); j++)
			{
				if ((*j)->type == RES)
				{
					if (i == k) { G(i, k) += (1 / (*j)->Value); }

					else
					{
						if ((*j)->Connectd_Nodes[0] == Matrix_Nodes[i])
						{
							if ((*j)->Connectd_Nodes[1] == Matrix_Nodes[k])
							{
								G(i, k) -= (1 / (*j)->Value);
								G(k, i) -= (1 / (*j)->Value);
							}
						}
						else
						{
							if ((*j)->Connectd_Nodes[0] == Matrix_Nodes[k])
							{
								G(i, k) -= (1 / (*j)->Value);
								G(k, i) -= (1 / (*j)->Value);
							}
						}
					}
				}
				else
				{
					if (i == k)
					{
						if ((*j)->Connectd_Nodes[0] == Matrix_Nodes[i])
							I(i, 0) -= (*j)->Value;                           // Current Out
						else
							I(i, 0) += (*j)->Value;                           // current In
					}
				}
			}
		}
	}
	V = G.colPivHouseholderQr().solve(I);                // Solve the Matrices to get the voltages

	for (int i = 0; i < V.size(); ++i)
	{
		voltage[Matrix_Nodes[i]->ID] = V(i, 0);
	}
	//cout << G << "\n\n" << V << "\n\n" << I;
	//print(v);
}

bool check_vs_and_Res(NODE node)
{
	///**************** check that this node are connected to vs and resistor******************
	// parameters : node that its element list size equal 2;
	list<Element*>::iterator i;
	int count = 0;
	Element * element1 = NULL;
	Element * element2 = NULL;
	for (i = node.El_List.begin(); i != node.El_List.end(); i++)
	{
		if (count == 0)
		{
			element1 = (*i);
		}
		else
		{
			element2 = (*i);
		}
		count++;
	}
	if ((element1->type == VS) && (element2->type == RES)) return true;
	if ((element2->type == VS) && (element1->type == RES)) return true;
	return false;

}

Element* find_element(vector<NODE>&circuit, int Type, int Id)
{
	///************* find an element in the circuit*****************
	// parameters :
	// parameters :
	//1- circuit
	//2- Id of the recuired element
	//3- type of the recuired element
	// return :
	// 1- NULL if the element not found 
	// 2- the adress of the element 

	vector<NODE>      ::iterator i;
	list<Element *>  ::iterator j;

	for (i = circuit.begin(); i != circuit.end(); i++)
	{
		for (j = i->El_List.begin(); j != i->El_List.end(); j++)
		{
			if (((*j)->ID == Id) && ((*j)->type == Type))
			{
				return (*j);
			}
		}
	}
	return NULL;
}  
//**************************
double get_current(vector<double> voltage, Element *element)
{
	///************calculate  current in specified Element **********************8
	// parameters:
	//1-vector contain nodal voltage
	//- the element 
	//
	// return : the needed value;
	if (element->type == RES)
	{
		int node1 = element->Connectd_Nodes[0]->ID;
		int node2 = element->Connectd_Nodes[1]->ID;
		return (fabs(voltage[node1] - voltage[node2]) / element->Value);
	}
	else if (element->type == VS)     // based on always there will be resistance series with the battery  //  getting the current in this resistance
	{
		list<Element*>::iterator i;
		if (element->Connectd_Nodes[0]->El_List.size() == 2)
		{
			if (check_vs_and_Res((*element->Connectd_Nodes[0])))
			{
				int node1 = element->Connectd_Nodes[0]->ID;
				for (i = element->Connectd_Nodes[0]->El_List.begin(); i != element->Connectd_Nodes[0]->El_List.end(); i++)
				{
					if ((*i) != element)
					{
						int node2;
						if ((*i)->Connectd_Nodes[0] != element->Connectd_Nodes[0])
						{
							node2 = (*i)->Connectd_Nodes[0]->ID;
						}
						else
						{
							node2 = (*i)->Connectd_Nodes[1]->ID;
						}

						return (fabs(voltage[node1] - voltage[node2])) / (*i)->Value;
					}
				}
			}
		}
		if (element->Connectd_Nodes[1]->El_List.size() == 2)
		{
			if (check_vs_and_Res((*element->Connectd_Nodes[1])))
			{
				int node1 = element->Connectd_Nodes[1]->ID;
				for (i = element->Connectd_Nodes[1]->El_List.begin(); i != element->Connectd_Nodes[1]->El_List.end(); i++)
				{
					if ((*i) != element)
					{
						int node2;
						if ((*i)->Connectd_Nodes[0] != element->Connectd_Nodes[1])
							node2 = (*i)->Connectd_Nodes[0]->ID;
						else
							node2 = (*i)->Connectd_Nodes[1]->ID;

						return (fabs((voltage[node1] - voltage[node2])) / (*i)->Value);
					}
				}
			}
		}
	}
	else                      //always the current in CS his value;
	{
		return element->Value;
	}

}

double get_voltage(vector<double> voltage, int node1, int node2)
{
	///***************get potiential difference******************
	// parameters:
	//1-vector contain nodal voltage
	//2-node1
	//3-node2

	//return the value of PD

	return (voltage[node1] - voltage[node2]);
}

double get_power(vector<double> voltage, Element *element)
{

	///************calculate powr  in specified Element **********************8
	// parameters:
	//1-vector contain nodal voltage
	//3- the element 
	//
	// return : the needed value;

	// assume that power supplied has positive value and power dissapated has negative value

	if (element->type == RES)
	{
		int node1 = element->Connectd_Nodes[0]->ID;
		int node2 = element->Connectd_Nodes[1]->ID;
		return (-(((voltage[node1] - voltage[node2])*(voltage[node1] - voltage[node2])) / element->Value));
	}
	else if (element->type == VS)
	{
		list<Element*>::iterator i;
		if (element->Connectd_Nodes[0]->El_List.size() == 2)
		{
			if (check_vs_and_Res((*element->Connectd_Nodes[0])))
			{
				int node1 = element->Connectd_Nodes[0]->ID;
				for (i = element->Connectd_Nodes[0]->El_List.begin(); i != element->Connectd_Nodes[0]->El_List.end(); i++)
				{
					if ((*i) != element)
					{
						int node2;
						if ((*i)->Connectd_Nodes[0] != element->Connectd_Nodes[0])
							node2 = (*i)->Connectd_Nodes[0]->ID;
						else
							node2 = (*i)->Connectd_Nodes[1]->ID;

						double current = (voltage[node1] - voltage[node2]) / (*i)->Value;
						return  (current*(-element->Value));
					}
				}
			}
		}
		if (element->Connectd_Nodes[1]->El_List.size() == 2)
		{
			if (check_vs_and_Res((*element->Connectd_Nodes[1])))
			{
				int node1 = element->Connectd_Nodes[1]->ID;
				for (i = element->Connectd_Nodes[1]->El_List.begin(); i != element->Connectd_Nodes[1]->El_List.end(); i++)
				{
					if ((*i) != element)
					{
						int node2;
						if ((*i)->Connectd_Nodes[0] != element->Connectd_Nodes[1])
							node2 = (*i)->Connectd_Nodes[0]->ID;
						else
							node2 = (*i)->Connectd_Nodes[1]->ID;

						double current = (voltage[node1] - voltage[node2]) / (*i)->Value;
						return  (current*(element->Value));
					}
				}
			}
		}
	}
	else
	{
		int node1 = element->Connectd_Nodes[1]->ID;
		int node2 = element->Connectd_Nodes[0]->ID;

		return ((voltage[node1] - voltage[node2])*element->Value);
	}


}
//***************************
void power_balance(vector<double> voltage, vector<NODE>circuit)
{
	//************** power balance************************
	// assume the dissapated power has negative sign and supplied power has positive sign 
	vector<NODE>::iterator i;
	list<Element*>::iterator j;
	double power_dissapated = 0;
	double power_supplied = 0;

	// // traverse throw all element and calculate the power on each element 
	cout << "\n\n ***************   power balance check  *************************\n\n";
	for (i = circuit.begin(); i != circuit.end(); i++)
	{
		for (j = i->El_List.begin(); j != i->El_List.end(); j++)
		{
			if ((*j)->visited == false)
			{
				string type = "";

				switch ((*j)->type)
				{
				case RES: type = "R"; break;
				case VS: type = "E"; break;
				default: type = "I";
				}
				double power = get_power(voltage, (*j));
				cout << type << (*j)->ID << "  =  " << power;
				if (power < 0) { cout << "      ( dissapated )\n\n"; power_dissapated = power_dissapated + power; }
				else { cout << "       ( supplied ) \n\n"; power_supplied = power_supplied + power; }
				(*j)->visited = true;
			}

		}
	}
	for (i = circuit.begin(); i != circuit.end(); i++)
	{
		for (j = i->El_List.begin(); j != i->El_List.end(); j++)
		{
			if ((*j)->visited == true)
			{
				(*j)->visited = false;
			}
		}
	}
	power_dissapated = ceil(power_dissapated*1000.0) / 1000.0;
	power_supplied = floor(power_supplied*1000.0) / 1000.0;

	cout << "Total power dissapated =  " << power_dissapated << endl;
	cout << "Total power supplied =  " << power_supplied << endl;
}

void V_Deleted_Nodes(vector<NODE>& v, vector<double>& voltage, int ground) // *** By H & H ***
{
	list<Element*>::iterator j;
	NODE* VSleg = NULL;
	int VSvalue = 0;
	for (int i = 1; i < voltage.size(); ++i)
	{
		if ((voltage[i] == 0) && (i != ground) && (v[i].El_List.size() == 2) && (check_vs_and_Res(v[i])))
		{
			for (j = v[i].El_List.begin(); j != v[i].El_List.end(); ++j)
			{
				if ((*j)->type == VS)                     // hna5od msar mn el Middle Node ll Node ely motsla b el VS mn el N7ya el Tanya
				{
					if ((*j)->Connectd_Nodes[0] == &v[i])              // The middle Node is connected with Negative side of VS
					{
						VSleg = (*j)->Connectd_Nodes[1];
						VSvalue = (*j)->Value *-1;                   // el msar h y3dy 3la el negative side so the value is Negative
					}
					else                                               // The middle Node is connected with positive side of VS
					{
						VSleg = (*j)->Connectd_Nodes[0];           // el msar h y3dy 3la el positive side so the value is positive
						VSvalue = (*j)->Value;
					}
					break;             //break .. since the Node is connected with only 2 elements..1 of them is VS and we have found it
				}
			}
			voltage[i] = voltage[VSleg->ID] + VSvalue;      // Calculate the voltage of the Middle Node  
		}
	}
}
//***************************
void connect_element(vector<NODE>&circuit, Element* element)
{
	//*********************connect element to the circuit ***********************

	NODE* node1;
	NODE* node2;
	node1 = element->Connectd_Nodes[0];
	node2 = element->Connectd_Nodes[1];
	node1->El_List.push_back(element);
	node2->El_List.push_back(element);

}
Element* make_element(int id, element_type type, float value, NODE *node1, NODE *node2)
{
	/// ******************** making new element ********************
	// parameters 
	//1-3 the values of the new element
	// 4- the node we connect the CS or Vs with negative sign
	// 5 the node we connect the CS or Vs with positive sign
	Element* element = new Element;
	element->ID = id;
	element->type = type;
	element->Value = value;
	element->deleted = false;
	element->Connectd_Nodes[0] = node1;
	element->Connectd_Nodes[1] = node2;
	return element;

}
void vs_to_shortcircuit(vector<NODE>&circuit, Element * element)
{
	///********** element to short circuit **********************
	////// based on there is only real volt source(working if the element is real vs )  function used in RTH  help in in transform vs to short circuit 

	int l = 0;
	while (l <= 1)
	{
		if (element->Connectd_Nodes[l]->El_List.size() == 2)
		{
			int id_of_far_node = element->Connectd_Nodes[(l + 1) % 2]->ID;
			int id_of_deleted_node = element->Connectd_Nodes[l]->ID;
			list<Element*>::iterator j, k;

			for (j = element->Connectd_Nodes[l]->El_List.begin(); j != element->Connectd_Nodes[l]->El_List.end(); j++)
			{
				if ((*j) != element)
				{
					element->Connectd_Nodes[(l + 1) % 2]->El_List.push_back((*j));

					if ((*j)->Connectd_Nodes[0] == element->Connectd_Nodes[l])
					{
						(*j)->Connectd_Nodes[0] = element->Connectd_Nodes[(l + 1) % 2];
					}
					else
					{
						(*j)->Connectd_Nodes[1] = element->Connectd_Nodes[(l + 1) % 2];
					}
					for (k = circuit[id_of_deleted_node].El_List.begin(); k != circuit[id_of_deleted_node].El_List.end(); k++)
					{
						if ((*j) == (*k)) {
							k = circuit[id_of_deleted_node].El_List.erase(k); return;
						}
					}
					return;
				}
				
			}

		}
		l++;
	}

}
float calculate_RTH(vector<NODE>&circuit, Element* element)
{
	/// ************* calulate RTH***************************
	// parameters :
	//1- circuit
	//2- the element we want to get Rth between his terminals
	//return : the value of the Rth 

	// in general: to calculate RTH ,i will add a current source it's value 1 ampere and calculate the voltage between his terminal after removing all sources
	// RTH =(difference_in_voltage)/1;

	// first dissapated all the sources  (make the current source open circuit &make the vs short circuit)
	remove_all_sources(circuit, element);

	int new_circuit_id = 101;    //  using this variable te make difference betweem original component and the additional compenent i will add;
	int size = circuit.size();
	
	// second : making assumption of the direction of the current always exist from element->connected_nodes[0] 
	// connect current source between the terminals
	Element* Cs = make_element(new_circuit_id, CS, 1, element->Connectd_Nodes[0], element->Connectd_Nodes[1]);
	new_circuit_id++;
	connect_element(circuit, Cs);

	NODE*node0 = element->Connectd_Nodes[0];
	NODE*node1 = element->Connectd_Nodes[1];

	delete_element(element);

	// third :solve the circuit using nodal analysis 
	vector<double> voltage(size);
	int ground_index = Get_Ground(circuit);
	cout << "ground is   " << ground_index << endl;
	Node_Analysis(circuit, voltage, ground_index);
	cout << endl;
	double PD = get_voltage(voltage, node0->ID, node1->ID);

	return fabs(PD);
}
void delete_element(Element *element)
{
	NODE* node1 = (element)->Connectd_Nodes[0];
	NODE* node2 = (element)->Connectd_Nodes[1];
	list<Element *> ::iterator i;
	for(i=node1->El_List.begin();i!=node1->El_List.end();i++)
	{
		if ((*i) == element) {
			node1->El_List.erase(i); break;
		}
	}
	for (i = node2->El_List.begin(); i != node2->El_List.end(); i++)
	{
		if ((*i) == element) {
			node2->El_List.erase(i); break; 
		}
		
	}
}
//*********************************************
float calculate_VTH(vector<NODE>&circuit, Element *element)
{
   ///*****************************calculate VTH******************************
	//paramaters : 1- circuit 2- the required element


	// put open circuit on the required element

	NODE*node0 = element->Connectd_Nodes[0];
	NODE*node1 = element->Connectd_Nodes[1];
	delete_element(element);
	 //second solve the circuit using nodal analysis 
	int size = circuit.size();
	vector<double> voltage(size);
	int ground_index = Get_Ground(circuit);
	cout << "ground is   " << ground_index << endl;
	Node_Analysis(circuit, voltage, ground_index);
	cout << endl;
	double PD = get_voltage(voltage, node0->ID, node1->ID);

	return fabs(PD);


}

//void SuperPosition(vector<NODE> &v, element_type type, int ID) // *** By H & H ***   Objective Delete All Other sources from the Graph
//{
//	list<Element*>::iterator j, k;
//	NODE* VSleg = NULL;
//	vector<Element*>del;
//	for (int i = 1; i < v.size(); ++i)           // Traversing the Grapth
//	{
//		if (v[i].El_List.size() > 0)              // 3shan mn3desh 3la ely et3mlohom Clear
//			for (j = v[i].El_List.begin(); j != v[i].El_List.end(); ++j)
//			{
//				if ((*j)->type == CS)                            // Deleting the Source if it's a CS && not the Required Source
//				{
//					if (!((*j)->type == type && (*j)->ID == ID))
//						del.push_back(*j);                  // b7ot ely hytshal f vector 3shan el remove dlwa2t bt3ml error m3 el End pointer f el loop
//				}
//				else if ((*j)->type == VS)
//				{
//					if (!((*j)->type == type && (*j)->ID == ID))
//					{                                             // kda l2et VS el Mfrod ytshal
//						if ((*j)->Connectd_Nodes[0] == &v[i])     // so we need to keep 1 of them only and clear the list of the other
//							VSleg = (*j)->Connectd_Nodes[1];       // but after connecting the Elements of Both 2 Nodes
//						else
//							VSleg = (*j)->Connectd_Nodes[0];      // VSleg de el NODE el b3d ma nshel el VS htb2a hya hya el v[i]										  
//																  // f kda el 2 Nodes that surround el VS will be the same
//						for (k = VSleg->El_List.begin(); k != VSleg->El_List.end(); ++k)
//						{
//							if ((*j) != (*k))					// hna han7ot el Elements in VSleg in v[i] ma 3da el VS
//							{
//								v[i].El_List.push_front(*k);    // adding the New elements to v[i]
//							}
//						}
//						VSleg->El_List.clear();                 // Clearing the list so we wont find it while traversing the Graph
//						del.push_back(*j);
//					}
//				}
//			}
//		for (int z = 0; z < del.size(); ++z)
//			v[i].El_List.remove(del[z]);           // deleting the elements w dlwa2t h5rog mn el loop f msh hy7sl error
//
//		del.clear();                               // clearing the vector w n5sh 3la el node el tanya
//	}
//	// *** NOTE  The Graph kda Et8yer  If U wanna use it again U need To Rebuild IT ...
//}
void remove_all_sources (vector<NODE>&circuit, Element * element)
{
	///*************** remove all sources except(if the element are source)**************************
	vector<NODE>::iterator i;
	list<Element*> ::iterator j, k;

	for (i = circuit.begin(); i != circuit.end(); i++)// travesre throw all nodes  
	{
		for (j = i->El_List.begin(); j != i->El_List.end();)
		{

			if (((*j)->type == CS) && ((*j) != element))
			{
				j = i->El_List.erase(j);
				if (j == i->El_List.end())break;
			}
			else if (((*j)->type == VS) && ((*j) != element))
			{
				Element * element = (*j);
				if (element->visited == true)
				{

					j = i->El_List.erase(j);
					if (j == i->El_List.end())break;

				}
				else
				{
					vs_to_shortcircuit(circuit, element); // fun help in transform volt source to open circuit
					j = i->El_List.erase(j);
					element->visited = true;
					if (j == i->El_List.end())break;
				}

			}
			else
			{
				++j;
			}
		}
	}

}
void superPosition(vector<NODE>&circuit1,vector<NODE>&circuit2, Element *element1,Element *element2,vector<double>&voltage,int ground_index)
{
	// first remove all sources
	remove_all_sources(circuit1, element1);  // on this circuit we want to get the response
	remove_all_sources(circuit2, element2);

	// third :solve the circuit using nodal analysis 
    voltage.resize(circuit2.size());
	cout << "ground is   " << ground_index << endl;
	Node_Analysis(circuit2, voltage, ground_index);
	V_Deleted_Nodes(circuit1, voltage,ground_index);
     


}
