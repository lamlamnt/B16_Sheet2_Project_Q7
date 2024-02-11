#include "system.h"
#include <vector>
#include<cmath>
#include <iostream>
#include <random>
#include <string>
using namespace std;

Robot::Robot(int _capacity):capacity(_capacity){};

Node::Node(double x, double y, int _id):id(_id)
{
    position={x,y};
}

vector<double> Node::getPosition() const
{
    return position;
}

Edge::Edge(Node* _one, Node* _two):one(_one),two(_two)
{
    //calculate length using nodes positions
    length = sqrt(pow((one->getPosition()[0] - two->getPosition()[0]),2) + pow((one->getPosition()[1] - two->getPosition()[1]),2));
};

double Edge::getLength() const
{
    return length;
}

Store::Store(double x, double y, Robot& robot):Node(x,y,0),robotOne(robot){};

Customer::Customer(double x,double y, int _id):Node(x,y,_id){};

void Customer::Order(int basketNum)
{
    basket = basketNum;
}

int Customer::getNumBasket() const
{
    return basket;
}

World::World(Store& storeName):grocer(storeName){};

void World::addAllCustomersCustom()
{
    Customer cus1(1.,1.,1);
    Customer cus2(2.,4.,2);
    Customer cus3(-1.,1.,3);
    Customer cus4(0.,4.,4);
    CustomerList.emplace_back(cus1);
    CustomerList.emplace_back(cus2);
    CustomerList.emplace_back(cus3);
    CustomerList.emplace_back(cus4);
}

vector<Customer>& World::getCustomerList() 
{
    return CustomerList;
}

vector<Edge>& World::getEdgeList()
{
    return EdgeList;
}

vector<int> Edge::getNodeID() const
{
    return {one->id,two->id};
}

void World::addAllEdgesCustom()
{
    Edge edge1(&grocer,&CustomerList[0]);
    Edge edge2(&grocer,&CustomerList[2]);
    Edge edge3(&CustomerList[0],&CustomerList[1]);
    Edge edge4(&CustomerList[2],&CustomerList[3]);
    Edge edge5(&CustomerList[0],&CustomerList[3]);
    EdgeList.emplace_back(edge1);
    EdgeList.emplace_back(edge2);
    EdgeList.emplace_back(edge3);
    EdgeList.emplace_back(edge4);
    EdgeList.emplace_back(edge5);
}

void World::addAllCustomersRandom(int numNodes)
{
    for(int i = 0; i < numNodes; i++)
    {
        //Do int (between -5 and 5) for now but can do double later.
        int x = rand()%11-5; int y = rand()%11-5;
        Customer newCustomer(x,y,i+1);
        //Make sure no duplicates -> use set?
        //Try emplace back
        CustomerList.push_back(newCustomer);
    }
}

//Iterate through all customer nodes and assign 0,1,2,or 3 baskets
void World::takeOrderRandom()
{
    for(int i = 0; i < CustomerList.size();i++)
    {
        int numBasket = rand()%4; //uniform distribution from 0 to 3
        CustomerList[i].Order(numBasket);
    }
}

void World::addAllEdgesRandom()
{

}

vector<vector<double>> World::convertMaptoMatrix()
{
    //The store will have have index 0. 
    //Customer 0 in the CustomerList will have index 1 in the adjacency matrix and so on.
    vector<vector<double>> adjacency_matrix;
    int numNodes = CustomerList.size() + 1;
    adjacency_matrix.resize(numNodes);
    for(int i = 0; i < numNodes; i++)
    {
        adjacency_matrix[i].resize(numNodes);
    }
    for(int i = 0; i < EdgeList.size(); i++)
    {
        adjacency_matrix[EdgeList[i].getNodeID()[0]][EdgeList[i].getNodeID()[1]] = EdgeList[i].getLength();
        adjacency_matrix[EdgeList[i].getNodeID()[1]][EdgeList[i].getNodeID()[0]] = EdgeList[i].getLength();
    }
    return adjacency_matrix;
}

vector<double> World::Dijkstra_Shortest_Path()
{
    vector<vector<double>> adjacency_matrix = //convertMaptoMatrix();
    { { 0, 4, 0, 0, 0, 0, 0, 8, 0 },
                        { 4, 0, 8, 0, 0, 0, 0, 11, 0 },
                        { 0, 8, 0, 7, 0, 4, 0, 0, 2 },
                        { 0, 0, 7, 0, 9, 14, 0, 0, 0 },
                        { 0, 0, 0, 9, 0, 10, 0, 0, 0 },
                        { 0, 0, 4, 14, 10, 0, 2, 0, 0 },
                        { 0, 0, 0, 0, 0, 2, 0, 1, 6 },
                        { 8, 11, 0, 0, 0, 0, 1, 0, 7 },
                        { 0, 0, 2, 0, 0, 0, 6, 7, 0 } };
    int numNodes = adjacency_matrix.size();
    vector<double> distance(numNodes,INT_MAX); //Holds shortest distance from source to each node
    vector<bool> visited(numNodes,false); //Whether we have found the shortest distance to this node 
    vector<string> output(numNodes); //Stores the path as a string
    distance[0] = 0;
    //Visit each node
    for(int i = 0; i < numNodes; i++)
    {
        //Find minimum distance from source to set of vertices not yet processed. 
        int node_min_distance = minDistance(distance,visited);
        visited[node_min_distance] = true;
        //Update the distance value of adjacent vertices of the picked vertex
        for(int v = 0; v < numNodes; v++)
        {
            if(visited[v] == false && adjacency_matrix[i][v] != 0 && distance[i] != INT_MAX && distance[i] + adjacency_matrix[i][v] < distance[v])
            {
                distance[v] = distance[i] + adjacency_matrix[i][v];
                output[v] = output[i] + "-" + to_string(i);
            }
        }
    }
    for(int i = 0; i < numNodes; i++)
    {
        cout<<output[i]<<endl;
    }
    return distance;
}

double World::minDistance(vector<double>distance, vector<bool>visited)
{
    int min = INT_MAX; 
    int min_index = 0;
    for(int i = 0; i < distance.size(); i++)
    {
        if(visited[i] == false && distance[i] <= min)
        {
            min = distance[i];
            min_index = i;
        }
    }
    return min_index;
}

void World::computeDeliveryPlan()
{
    //Simplest: deliver on shortest path 
    //Deliver to customers with 3 baskets first
    //Deliver closest node and then node next to it until no more basket and so on
    //Print to console
}
