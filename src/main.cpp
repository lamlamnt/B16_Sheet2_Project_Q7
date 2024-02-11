#include<vector>
#include "system.h"
#include <iostream>
#include<cstdlib>
using namespace std;

int main()
{
    //Initialize the nodes
    srand((unsigned) 10); //random seed 
    Robot newRobot(3);
    Store grocer(0,0,newRobot);
    World Oxford(grocer);
    Oxford.addAllCustomersCustom();
    //Oxford.addAllCustomersRandom(10);
    Oxford.addAllEdgesCustom();
    //Generate a list of random customer orders
    Oxford.takeOrderRandom(); 
    vector<Customer> oxfordCustomerList = Oxford.getCustomerList();
    vector<Edge> oxfordEdgeList = Oxford.getEdgeList();
    for(int i = 0; i< oxfordEdgeList.size(); i++)
    {
        cout<<oxfordEdgeList[i].getLength()<<endl;
    }
    for(int i = 0; i < oxfordCustomerList.size(); i++)
    {
        cout<<oxfordCustomerList[i].id<<": "<<oxfordCustomerList[i].getPosition()[0]<<", "<<oxfordCustomerList[i].getPosition()[1]<<": "<<oxfordCustomerList[i].getNumBasket()<<endl;
    }
    vector<double> shortest_distance = Oxford.Dijkstra_Shortest_Path();
    for(int i = 0; i<shortest_distance.size(); i++)
    {
        cout<<shortest_distance[i]<<endl;
    }
}