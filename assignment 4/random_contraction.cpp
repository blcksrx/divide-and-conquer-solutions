//
//  random_contraction.cpp
//
//  Implementation of random contraction algorithm that finds the minimum cut
//  of provided graph.
//
//  Created by Dan on 12.09.17.
//

#include <iostream>
#include <vector>
#include <random>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

struct Node
{
    int number;
    vector<Node *> ptr;
};

void randomContraction(Node nodes[200], vector<int> numbers, int& result, int n);
int getRandomNumber(int min, int max);

int main()
{
    int iterations = 40;
    int result = 199;
    Node nodes[200], nodesOriginal[200];
    
    /* read from file to memory, the graph is represented by an array of nodes
     where each node contains its number and a vector of pointers to all nodes with
     which it shares an edge */
    string stringToRead;
    int numberToRead;
    ifstream fileStream{"kargerMinCut.txt"};
    if (!fileStream)
    {
        cout << "File could not be opened" << endl;
        return 1;
    }
    else
    {
        for (int i = 0; i < 200; i++)
        {
            getline(fileStream, stringToRead, '\n');
            istringstream istr(stringToRead);
            istr >> numberToRead;
            nodes[i].number = numberToRead;
            nodesOriginal[i].number = numberToRead;
            
            while (istr >> numberToRead)
            {
                (nodes[i].ptr).push_back(&nodes[numberToRead - 1]);
                (nodesOriginal[i].ptr).push_back(&nodes[numberToRead - 1]);
            }
        }
    }
    
    // vector of numbers of nodes that haven't been contracted yet
    vector<int> numbers, numbersOriginal;
    for (int i = 0; i < 200; i++)
    {
        numbers.push_back(i);
        numbersOriginal.push_back(i);
    }
    
    // iterations of algorithm
    for (int n = 0; n < iterations; n++)
    {
        randomContraction(nodes, numbers, result, n + 1);
        
        // reset input data
        numbers = numbersOriginal;
        for (int k = 0; k < 200; k++)
        {
            nodes[k] = nodesOriginal[k];
        }
    }
    cout << "Result after " << iterations << " iterations: " << result << endl;
}

// random contraction algorithm
void randomContraction(Node nodes[200], vector<int> numbers, int& result, int n)
{
    for (int i = 200; i > 2; i--)
    {
        // choose random node from the ones that haven't been contracted yet
        int selectFromVector = getRandomNumber(0, i - 1);
        int select = numbers[selectFromVector];
        numbers.erase(numbers.begin() + selectFromVector);
        
        // select random edge of random node to contract
        int selectEdge = getRandomNumber(0, nodes[select].ptr.size() - 1);
        
        // the node that will "dissapear" in the fusion
        Node *contracting = &nodes[select];
        // the node that will remain after the fusion
        Node *merging = contracting->ptr.at(selectEdge);
        
        /* copy all pointers from contracted to merged node, except the ones pointing
         to the merged node */
        int contractingSize = contracting->ptr.size();
        for (int j = 0; j < contractingSize; j++)
        {
            // if it doesn't point to merged
            if (contracting->ptr.at(j) != merging)
            {
                // add to merged
                merging->ptr.push_back(contracting->ptr.at(j));
                
                /* change all pointers that point to the contracted node to point
                 to the merged node */
                Node* otherNode = contracting->ptr.at(j);
                int otherSize = otherNode->ptr.size();
                for (int k = 0; k < otherSize; k++)
                {
                    if (otherNode->ptr.at(k) == contracting)
                    {
                        otherNode->ptr.at(k) = merging;
                    }
                }
            }
        }

        int mergingSize = merging->ptr.size();
        /* remove all pointers from merged to contracted node (would be self-loops) */
        for (int j = 0; j < mergingSize; j++)
        {
            if (merging->ptr.at(j) == contracting)
            {
                merging->ptr.erase(merging->ptr.begin() + j);
                mergingSize--;
                j--;
            }
        }
    }
    
    // algorithm sanity check
    if (nodes[numbers.at(0)].ptr.size() != nodes[numbers.at(1)].ptr.size())
    {
        cout << "Pointer numbers of last nodes not equal!";
        exit(-1);
    }
    // new minimal cut found
    else if (nodes[numbers.at(0)].ptr.size() < result)
    {
        result = nodes[numbers.at(0)].ptr.size();
        cout << n << ". iteration: " << result << endl;
    }
}

// get random integer between min and max included using the mersenne twister
int getRandomNumber(int min, int max)
{
    mt19937 rng;
    rng.seed(random_device()());
    uniform_int_distribution<mt19937::result_type> dist6(min, max);
    return dist6(rng);
}
