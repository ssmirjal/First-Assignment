/******************************************************
 * File: main.cpp
 * Author: Seyedshahabaddin Mirjalili!
 *
 * This is the code for HW1 CS106L
 */

#include <iostream>
#include <sstream>
#include <vector>
#include "GraphVisualizer.h"
#include "SimpleGraph.h"
#include <fstream>
#include <string>
#include <cmath>
#include <ctime>

using namespace std;
const double Pi=3.14159265358979323;
void Welcome() {
	cout << "Welcome to CS106L GraphViz!" << endl;
	cout << "This program uses a force-directed graph layout algorithm" << endl;
	cout << "to render sleek, snazzy pictures of various graphs." << endl;
	cout << endl;
}
void Set_initial_graph(SimpleGraph& graph){
    int n=graph.nodes.size();
    for(int k=0;k<n;k++){
        graph.nodes[k].x=cos(2*Pi*k/n);
        graph.nodes[k].y=sin(2*Pi*k/n);
    }
}
void Compute_Repulsions(SimpleGraph& graph,vector<double>& F_rep_x,vector<double>& F_rep_y){
    double k_rep=1e-3;
    double Force_Size,theta;
    for(int i=0;i<graph.nodes.size();i++){
        F_rep_x[i]=0;
        F_rep_y[i]=0;
        for(int j=0;j<graph.nodes.size();j++){
            if(j!=i){
                Force_Size=k_rep/sqrt((graph.nodes[i].x-graph.nodes[j].x)*(graph.nodes[i].x-graph.nodes[j].x)+(graph.nodes[i].y-graph.nodes[j].y)*(graph.nodes[i].y-graph.nodes[j].y));
                theta=atan2(graph.nodes[i].y-graph.nodes[j].y,graph.nodes[i].x-graph.nodes[j].x);
                F_rep_x[i]+=Force_Size*cos(theta);
                F_rep_y[i]+=Force_Size*sin(theta);
                }
            }
        }
}
void Compute_Attractions(SimpleGraph& graph,vector<double>& F_att_x,vector<double>& F_att_y){
    double k_att=1e-3;
    double Force_Size,theta;
    for(int i=0;i<graph.edges.size();i++){
        F_att_x[i]=0;
        F_att_y[i]=0;
    }
    for(int k=0;k<graph.edges.size();k++){
        int i=graph.edges[k].start;
        int j=graph.edges[k].end;
            Force_Size=k_att*((graph.nodes[i].x-graph.nodes[j].x)*(graph.nodes[i].x-graph.nodes[j].x)+(graph.nodes[i].y-graph.nodes[j].y)*(graph.nodes[i].y-graph.nodes[j].y));
            theta=atan2(graph.nodes[i].y-graph.nodes[j].y,graph.nodes[i].x-graph.nodes[j].x);
            F_att_x[i]-=Force_Size*cos(theta);
            F_att_y[i]-=Force_Size*sin(theta);
            F_att_x[j]+=Force_Size*cos(theta);
            F_att_y[j]+=Force_Size*sin(theta);
    }
}

void Displace(SimpleGraph& graph,vector<double>& F_rep_x,vector<double>& F_rep_y,vector<double>& F_att_x,vector<double>& F_att_y){
    for(int i=0;i<graph.nodes.size();i++){
        graph.nodes[i].x+=F_rep_x[i];
        graph.nodes[i].y+=F_rep_y[i];
        graph.nodes[i].x+=F_att_x[i];
        graph.nodes[i].y+=F_att_y[i];
    }
}


int main() {
	Welcome();
    
    InitGraphVisualizer();
    
    //Prompting the user for the input file name and the simulation run time
    cout << "Please enter the name of the file: ";
    string filename;
    getline(cin,filename);
    cout << "Please enter how long you want the simulation to be: ";
    string input;
    getline(cin,input);
    stringstream converter;
    converter << input;
    double runtime;
    converter >> runtime;
    
    //Reading values from file into the SimpleGraph object
    ifstream input_f;
    SimpleGraph graph;
    input_f.open(filename.c_str());
    int numnodes,value1,value2;
    input_f >> numnodes;
    int count = 0;
    while (true){
        input_f >> value1;
        input_f >> value2;
        Edge myedge;
        myedge.start=value1;
        myedge.end=value2;
        if(input_f.fail()) break;
        graph.edges.push_back(myedge);
        count++;
    }
    graph.nodes.resize(numnodes);
    
    //Now it's time to set the initial position of the nodes
	Set_initial_graph(graph);
    
    //Now let's go into the loop of displacing the nodes
    //Need to build a vector with the size of the edges for attracting forces and a vector for repulsive forces with size of nodes
    vector<double> F_att_x(graph.nodes.size());
    vector<double> F_att_y(graph.nodes.size());
    vector<double> F_rep_x(graph.nodes.size());
    vector<double> F_rep_y(graph.nodes.size());
    
    time_t Start_Time = time(NULL);
    while(difftime(time(NULL), Start_Time)<runtime){
        Compute_Repulsions(graph,F_rep_x,F_rep_y);
        Compute_Attractions(graph,F_att_x,F_att_y);
        Displace(graph,F_rep_x,F_rep_y,F_att_x,F_att_y);
        DrawGraph(graph);
        
    }
       
    
    return 0;
}
