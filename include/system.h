#pragma once
#include<vector>
#include<string>
#include <set>
using namespace std;

class Node
{
    private:
        vector<double>position;
    public:
        const int id;
        Node(double x, double y,int _id);
        vector<double> getPosition() const;
};

class Edge
{
    private:
        Node* one; Node* two; double length;
    public:
        Edge(Node* _one, Node* _two);
        double getLength() const;
        vector<int> getNodeID() const;
};

class Robot
{
    private:
        int capacity;
        vector<double>location;
    public:
        Robot(int _capacity);
        void setCapacity(int newCapacity)
        {
            capacity = newCapacity;
        }
};

class Customer:public Node
{
    private:
        int basket;
    public:
        Customer(double x,double y,int _id);
        void Order(int basketNum);
        int getNumBasket() const;
};

class Store:public Node
{
    private:
        Robot& robotOne; //-> can become vector of references to robots
    public:
        //Store has ID 0 
        Store(double x, double y, Robot& robot); 
};

class World
{
    private:
        vector<Customer> CustomerList; //Should contain 1 store (first in the vector) and 10 customers
        vector<Edge> EdgeList;
        Store grocer;
        vector<vector<double>> convertMaptoMatrix();
        double minDistance(vector<double>& distance, vector<bool>& visited) const;
        void processOutput(vector<string>& input);
        vector<set<int>> deliveryPath;
    public:
        //Initialize nodes and connectivity
        World(Store& storeName); 
        void addAllCustomersCustom(); //this method is used just to make it easier to test functionality and debug
        void addAllEdgesCustom(); //this method is used just to make it easier to test functionality and debug
        void addAllCustomersRandom(int numNodes);
        void takeOrderRandom();
        void addAllEdgesRandom(); //Initialize random connections for a connected graph
        vector<Customer>& getCustomerList();
        vector<Edge>& getEdgeList();
        vector<double> Dijkstra_Shortest_Path();
        void computeDeliveryPlan();
};
