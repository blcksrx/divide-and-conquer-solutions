//  inversions.cpp
//  08.06.17 Dan
//  calculates the number of inversions in the textfile "IntegerArray.txt"

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

struct Array
{
    unsigned long inversions;
    vector<unsigned long> data;
};

Array sortAndCount(Array array);
Array countSplitInv(vector<unsigned long> leftArray, vector<unsigned long> rightArray, unsigned long n);

int main() {

    vector<unsigned long> array;
    array.reserve(100000);
    
    // open file and write integers into vector
    ifstream inputFile("IntegerArray.txt");
    if (!inputFile)
    {
        cout << "File could not be opened" << endl;
        return 1;
    }
    else
    {
        unsigned long value;
        while (inputFile >> value)
        {
            array.push_back(value);
            inputFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    inputFile.close();
    Array input = {0, array};

    Array result = sortAndCount(input);
    cout << "Number of inversions: " << result.inversions << endl;
    return 0;
}

//  sorts the array using merge sort while counting inversions
Array sortAndCount(Array array)
{
    unsigned long n = array.data.size();
    if (n <= 1)
        return array;
    else
    {
        vector<unsigned long> leftArray(array.data.begin(), array.data.begin() + n/2);
        vector<unsigned long> rightArray(array.data.begin() + n/2, array.data.begin() + n);

        Array leftResult = sortAndCount({0, leftArray});
        Array rightResult = sortAndCount({0, rightArray});
        Array splitResult = countSplitInv(leftResult.data, rightResult.data, n);
        return {leftResult.inversions + rightResult.inversions + splitResult.inversions, splitResult.data};
    }
}

//count split inversions by comparing the left and right halves of the array and merging them
Array countSplitInv(vector<unsigned long> leftArray, vector<unsigned long> rightArray, unsigned long n)
{
    Array result = {0, {}};
    unsigned long leftSize = leftArray.size();
    unsigned long rightSize = rightArray.size();
    
    for (int i = 0, j = 0, k = 0; k < n; k++)
    {
        if (i < leftSize && j < rightSize)
        {
            if (leftArray[i] < rightArray[j])
            {
                result.data.push_back(leftArray[i]);
                i++;
            }
            else
            {
                result.data.push_back(rightArray[j]);
                result.inversions += leftArray.size() - i;
                j++;
            }
        }
        else if (i < leftSize && j >= rightSize)
        {
            result.data.push_back(leftArray[i]);
            i++;
        }
        else
        {
            result.data.push_back(rightArray[j]);
            j++;
        }
    }
    return result;
}
