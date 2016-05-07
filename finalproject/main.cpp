#include <vector>
#include <unordered_map>
#include <algorithm>
#include <queue>
#include <map>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


using namespace std;

struct Information {
    Information(double i_weight, double i_price) : weight(i_weight), price(i_price) {}
    double weight; // time
    double price;  // price
};

struct Edge { // structure is defined for edge unification to use in both edge list and adjacency list
    int from; // from node
    int to; // to node
    Information info; // info about distance and price
};

class Graph { // graph class
public:
    Graph(int number_of_nodes = 0) { // initing graph with number of nodes
        edges.resize(number_of_nodes); // adjacency table initialization, allowing for multiple edges from one vertex to another
        for (int i = 0; i < number_of_nodes; ++i) {
            edges[i].resize(number_of_nodes);
        }
    }
    void addNode(int from, int to, double weight, double price) { // adding node to adjacency table
        edges[from][to].push_back(Information(weight, price));
    }
    
    double minDistance(int from, int to) { // minimum distance
        double result = 0; // result will be stored here
        queue<int> q; // queue in what all elements will be stored
        q.push(from); // adding first element to queue
        vector<bool> processed(edges.size(), false);  // initing processed flags
        vector<double> distance(edges.size(), 1000000); // distance
        distance[from] = 0; // distance to itself is 0
        while (!q.empty()) { // while we haven't visit all vertexes
            int current = q.front(); // getting first  unvisited vertex
            int size = adj_list[current].size(); // number of adjacent nodes
            for (int i = 0; i < size; ++i) {
                int current_to = adj_list[current][i].to; // adding new edge if all is ok
                double weight = adj_list[current][i].info.weight; // getting its length
                
                if (!processed[current_to]) {
                    distance[current_to] = min(distance[current_to], distance[current] + weight);
                    q.push(current_to);
                }
            }
            processed[current] = true;
            q.pop();
        }
        result = distance[to];
        return result;
    }
    
    double minPrice(int from, int to) //same comments as min price.
    {
        double result = 0;
        queue<int> q;
        q.push(from);
        vector<bool> processed(edges.size(), false);
        vector<double> price(edges.size(), 1000000);
        price[from] = 0;
        while (!q.empty()) {
            int current = q.front();
            int size = adj_list[current].size();
            for (int i = 0; i < size; ++i) {
                int current_to = adj_list[current][i].to;
                double pprice = adj_list[current][i].info.price;
                if (!processed[current_to]) {
                    price[current_to] = min(price[current_to], price[current] + pprice);
                    q.push(current_to);
                }
            }
            processed[current] = true;
            q.pop();
        }
        result = price[to];
        return result;
    }
    
private:
    vector<vector<vector<Information> > > edges;
    map<int, vector<Edge>> adj_list;
};

class Mapper { // maps string station name to int for usage with graph and Dijkstra algorithm
public:
    void addString(const std::string str) { // adding string
        if (mp.find(str) == mp.end()) {
            v.push_back(str); // adding to vector for fast int -> string transform
            mp[str] = mp.size(); // adding to map for fast transform
        }
    }
    
    int getKey(const std::string& str) { // getting key by value if map is bijective
        if (mp.find(str) == mp.end()) {
            return -1;
        }
        else {
            return mp[str];
        }
    }
    
    string getString(int key) { // just returnning the string
        return v[key];
    }
    
    int getSize() const {
        return v.size();
    }
    
private:
    map<string, int> mp;
    vector<string> v;
};

int main() {
    ifstream in;
    in.open("project1.csv"); //open file
    if (in.fail()) // output error message when file fails to open
    {
        cerr << "File cannot be found" << endl;
        exit(1);
    }
    string line;  //just reading in each column.
    string one;
    string two;
    int td_num; //timedes column
    int ta_num; //timearrival column
    int distance_num; //distance column
    double price; //price column
    vector <string > arrival_v;
    vector <string > depart_v;
    vector <int> time_des_v;
    vector <int> time_arrival_v;
    vector <int> distance_v;
    vector <double> price_v;
    getline(in, line); //strip off the first line
    while (getline(in, line)) { //read whole line into line
        istringstream iss(line); //eliminate first row
        getline(iss, one, ',');  // read each column by skipping commas.
        arrival_v.push_back(one);
        getline(iss, two, ',');
        depart_v.push_back(two);
        iss >> td_num;
        getline(iss, line, ',');
        iss >> ta_num;
        getline(iss, line, ',');
        iss >> distance_num;
        getline(iss, line, ',');
        iss >> price;
        getline(iss, line, '\n');
        time_des_v.push_back(td_num); //push it onto each individual vector
        time_arrival_v.push_back(ta_num);
        distance_v.push_back(distance_num);
        price_v.push_back(price);
    }
    Mapper mapper;
    for (unsigned int i = 0; i < arrival_v.size(); ++i)
    {
        mapper.addString(arrival_v[i]);  //add arrival airports to transform into int for usage with graph and dijkstra algorithm
    }
    for (unsigned int i = 0; i < depart_v.size(); ++i) {
        mapper.addString(depart_v[i]);   //add departure airports to transform into int for usage with graph and dijkstra algorithm
    }
    string departure;
    string arrival;
    cout<<"Dear customer, I will be assisting you today to book a flight with our small commercial airline."<<endl;
    cout <<"Here is the list of arrival and departure airport destinations."<<endl;
    
    cout<<"Carefully pick a departure airport destination as well as arrival airport destination."<<endl;
    cout<<"Our system will kindly let you know the shortest time flight available and the cheapest route as well."<<endl;
    cout <<"Happy Travels!"<<endl;
    
    ifstream input;
    input.open ("airportsfile.txt");//open file with all of the airports displayed
    if (input.fail()) // output error message when file fails to open
    {
        cerr << "File cannot be found." << endl;
        exit(1);
    }
    string outputtxt;
    while (!input.eof () )
    {
        input >> outputtxt;
        cout<< outputtxt<<endl; //display airports
    }
   
    cout << "Enter departure airport: ";  //ask user for departure airports
    cin >> departure;
    cout << "Enter arrival airport: ";  //ask user for arrival airport
    cin >> arrival;
    int departure_key = mapper.getKey(departure); //get key by value
    int arrival_key = mapper.getKey(arrival);  //get key by value
    Graph g(mapper.getSize());
    for (unsigned int i = 0; i < arrival_v.size(); ++i) //loop to go through arrival vector
    {
        string c_dep = depart_v[i], c_arr = arrival_v[i];
        Information info((double)time_arrival_v[i] - time_des_v[i], price_v[i]); //insert into struct information
        int dep_key = mapper.getKey(c_dep),   //get that key by value
        arr_key = mapper.getKey(c_arr);        //get that key by value
        g.addNode(dep_key, arr_key, info.weight, info.price); //pass the structs into addnode function
    }
    cout << "Shortest time flight: ";  //output shortest time flight
    cout << g.minDistance(departure_key, arrival_key) << endl;  //call min distance function
    cout << "Cheapest flight: "; //output cheapest flight
    cout << g.minPrice(departure_key, arrival_key) << endl; //call min price function
    return 0;
}