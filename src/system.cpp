#include "system.h"
#include <vector>
#include<cmath>
#include <iostream>
#include <random>
#include <string>
#include <sstream>
#include <set>
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
    //Use a spanning tree algorithm to ensure it's a connected graph
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
    vector<vector<double>> adjacency_matrix = convertMaptoMatrix();
    int numNodes = adjacency_matrix.size();
    vector<double> distance(numNodes,INT_MAX); 
    vector<bool> visited(numNodes,false);
    vector<string> output(numNodes);
    distance[0] = 0;
    output[0] = "0";
 
    for (int count = 0; count < numNodes - 1; count++) {
        int u = minDistance(distance, visited);
        visited[u] = true;
        for (int v = 0; v < numNodes; v++)
        {
            if (!visited[v] && adjacency_matrix[u][v] && distance[u] != INT_MAX 
            && distance[u] + adjacency_matrix[u][v] < distance[v])
            {
                distance[v] = distance[u] + adjacency_matrix[u][v];
                output[v] = output[u] + "-" + to_string(u) + "-" + to_string(v);
            }
        }
    }
    //Process the output, which contains the actual path (the list of nodes on the shortest path)
    processOutput(output);
    return distance;
}

void World::processOutput(vector<string>& input)
{
    for(int i = 0; i < input.size(); i++)
    {
        set<int> path;
        stringstream ss(input[i]);
        string token;
        while (getline(ss, token,'-')) {
            int num;
            istringstream (token ) >> num;
            path.insert(num);
        }
        deliveryPath.emplace_back(path);
    }
}

double World::minDistance(vector<double>& distance, vector<bool>& visited) const
{
    int min = INT_MAX; 
    int min_index = INT_MAX;
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
    cout<<"Delivery plan:"<<endl;
    //Simplest: deliver on shortest path 
    for(int node = 0; node < CustomerList.size(); node++)
    {
        if(CustomerList[node].getNumBasket() != 0)
        {
            string deliveryOutput = "";
            for(int i: deliveryPath[node+1])
            {
                deliveryOutput = deliveryOutput + to_string(i) + "-";
            }
            cout<<"Deliver "<<CustomerList[node].getNumBasket()<<" basket(s) to customer "<<CustomerList[node].id<<": "<<deliveryOutput<<endl;
        }
    }
}
