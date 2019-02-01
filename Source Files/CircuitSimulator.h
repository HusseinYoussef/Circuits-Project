#include <list>
#include<iterator>
#include <vector>
#include <map>
#include <sstream>
#include <iostream>
#include <Eigen/Dense>

using namespace std;

enum element_type
{
	RES = 1,
	VS,
	CS
};

struct NODE
{
	int ID;
	bool visited;
	bool deleted;		/////////// Ahmed	initially = false
	bool temp_ess;		///////////	Ahmed	initially = false
	std::list<struct Element*> El_List;
};

struct Element
{
	element_type type;
	int ID;
	bool visited;
	bool deleted;		/////////// Ahmed
	double Value;
	NODE* Connectd_Nodes[2];		// Each element has 2 connected nodes. 
};

/////////////////////////////////////////////////////////////////// Filling into a graph

bool check_vs_and_Res(NODE node);
void Node_Analysis(vector<NODE>&v, vector<double>& voltage, int &ground_index);
void Build_Circuit(float ** Data_Table, int column_index, vector<NODE> &vertices);
int Get_Ground(vector<NODE>v);
void print(vector<NODE>v);
Element* find_element(vector<NODE>&circuit, int Type, int Id);
double get_current(vector<double> voltage , Element *element);
double get_voltage(vector<double> voltage, int node1, int node2);
double get_power(vector<double> voltage , Element *element);
void power_balance(vector<double> voltage, vector<NODE>circuit);
void V_Deleted_Nodes(vector<NODE>& v, vector<double>& voltage, int ground);

//*************************************************************

Element* find_element(vector<NODE>&circuit, int Type, int Id);
void connect_element(vector<NODE>&circuit, Element* element);
Element* make_element(int id, element_type type, float value, NODE *node1, NODE *node2);
void vs_to_shortcircuit(vector<NODE>&circuit, Element * element);
void delete_element(Element *element);
float calculate_RTH(vector<NODE>&circuit, Element* element);
float calculate_VTH(vector<NODE>&circuit, Element *element);
void remove_all_sources(vector<NODE>&circuit, Element * element);

//*******************************************************************
//void SuperPosition(vector<NODE> &v, element_type type, int ID);
void superPosition(vector<NODE>&circuit1, vector<NODE>&circuit2, Element *element1, Element *element2, vector<double>&voltage, int ground_index);
