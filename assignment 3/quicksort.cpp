//  quicksort.cpp
//  23.06.17 Dan
//  sort given array using quicksort and returns the number of comparisons made
//  in the process using:
//  1. always left most pivots
//  2. always right most pivots
//  3. median of middle, left and right most pivots
//  4. random pivots

#include <iostream>
#include <fstream>
#include <vector>
#include <random>

using namespace std;

void quickSort(vector<int>& array, int left, int right, unsigned int& comparisons,
               const int pivotRule);
int partition(vector<int>& array, int left, int right, int pivotIndex,
              unsigned int& comparisons);
int selectPivot(const vector<int>& array, int left, int right, const int pivotRule);
int getRandomNumber(int min, int max);

int main() {
    vector<int> array;
    array.reserve(10000);
    
    // open file and write integers into vector
    ifstream inputFile("QuickSort.txt");
    if (!inputFile)
    {
        cout << "File could not be opened" << endl;
        return 1;
    }
    else
    {
        int value;
        while (inputFile >> value)
        {
            array.push_back(value);
            inputFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    inputFile.close();
    vector<int> input = array;
    unsigned int comparisons = 0;

    // sort array using left pivots (pivotRule = 1)
    quickSort(input, 0, input.size(), comparisons, 1);
    cout << "Number of comparisons using left pivots: " << comparisons << endl;
    
    // sort array using right pivots (pivotRule = 2)
    comparisons = 0;
    input = array;
    quickSort(input, 0, input.size(), comparisons, 2);
    cout << "Number of comparisons using right pivots: " << comparisons << endl;
    
    // sort array using median of three pivots (pivotRule = 3)
    comparisons = 0;
    input = array;
    quickSort(input, 0, input.size(), comparisons, 3);
    cout << "Number of comparisons using median pivots: " << comparisons << endl;
    
    // sort array using random pivots (pivotRule = 4)
    comparisons = 0;
    input = array;
    quickSort(input, 0, input.size(), comparisons, 4);
    cout << "Number of comparisons using random pivots: " << comparisons << endl;

    return 0;
}

// recursive quicksort implementation using pivotRule switch, counting inversions
void quickSort(vector<int>& array, int left, int right, unsigned int& comparisons,
               const int pivotRule)
{
    if (right - left <= 1)
    {
        return;
    }
    int pivotIndex = selectPivot(array, left, right, pivotRule);
    pivotIndex = partition(array, left, right, pivotIndex, comparisons);

    quickSort(array, left, pivotIndex, comparisons, pivotRule);
    quickSort(array, pivotIndex + 1, right, comparisons, pivotRule);
}

// partition elements around pivot while counting comparisons
int partition(vector<int>& array, int left, int right, int pivotIndex,
              unsigned int& comparisons)
{
    swap(array[pivotIndex], array[left]);
    int pivot = array[left];
    int i = left + 1;

    for (int j = i; j < right; j++)
    {
        if (array[j] < pivot)
        {
            swap(array[j], array[i++]);
        }
    }
    swap(array[left], array[i - 1]);
    comparisons += right - left - 1;
    return i - 1;
}

// select pivot according to pivotRule switch: 1. left, 2. right, 3. median of three, 4. random
int selectPivot(const vector<int>& array, int left, int right, const int pivotRule)
{
    if (pivotRule == 1)
    {
        return left;
    }
    else if (pivotRule == 2)
    {
        return right - 1;
    }
    else if (pivotRule == 3)
    {
        int middleIndex = left + (right - left - 1) / 2;
        if (array[left] < array[middleIndex])
        {
            if (array[right - 1] < array[left])
                return left;
            else if (array[right - 1] < array[middleIndex])
                return right - 1;
            else
                return middleIndex;
        }
        else
        {
            if (array[left] < array[right - 1])
                return left;
            else if (array[right - 1] < array[middleIndex])
                return middleIndex;
            else
                return right - 1;
        }
    }
    else
    {
        return getRandomNumber(left, right - 1);
    }
}

// get random integer between min and max included using the mersenne twister
int getRandomNumber(int min, int max)
{
    mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(min, max);
    return dist6(rng);
}

