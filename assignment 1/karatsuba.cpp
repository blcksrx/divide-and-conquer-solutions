// karatsuba.cpp
// 21.5.2017 Dan
// calculates the product of two integers using the karatsuba algorithm

#include <iostream>
#include <vector>
#include <string>

using namespace std;

string recursion(vector<string> values);
string stringSum(string x, string y);
string stringSub(string x, string y);
string stringMul(string x, string y);
string stringAppend(string x, int n);

int main() {
    string x, y;
    cout << "Insert positive numbers:" << endl;
    cin >> x >> y;
    vector<string> values {x, y};
    string result = recursion(values);
    cout << result << endl;
    return 0;
}

// recursive karatsuba algorithm
string recursion(vector<string> val)
{
    int xLength = val[0].length();
    int yLength = val[1].length();
    
    // base case
    if (xLength == 1 || yLength == 1)
    {
        return stringMul(val[0], val[1]);
    }
    
    // decomposition of numbers
    int aLength = xLength / 2 + xLength % 2;
    int cLength = yLength / 2 + yLength % 2;
    int m = min(xLength/2, yLength/2);
    if (xLength >= yLength)
    {
        aLength = xLength - m;
    }
    else
    {
        cLength = yLength - m;
    }
    
    // calculation of coefficients ac, bd and (a + b)*(c + d)
    vector<string> acvec {val[0].substr(0, aLength), val[1].substr(0, cLength)};
    string ac = recursion(acvec);
    
    vector<string> bdvec {val[0].substr(aLength, xLength - aLength), val[1].substr(cLength, yLength - cLength)};
    string bd = recursion(bdvec);
    
    string aplusb = stringSum(val[0].substr(0, aLength), val[0].substr(aLength, xLength - aLength));
    string cplusd = stringSum(val[1].substr(0, cLength), val[1].substr(cLength, yLength - cLength));
    vector<string> sumvec {aplusb, cplusd};
    string sum = recursion(sumvec);

    // summing the coefficients together:
    // ac * 10E(2m) + (sum - ac - bd) * 10E(m) + bd
    string result = stringSum(stringAppend(ac, 2 * m), bd);
    result = stringSum(stringAppend(sum, m), result);
    result = stringSub(result, stringAppend(bd, m));
    result = stringSub(result, stringAppend(ac, m));
    return result;
}

// calculates the sum of two positive integers represented by strings
string stringSum(string x, string y)
{
    string result;
    unsigned int carry = 0;
    if (x.length() < y.length())
        y.swap(x);
    
    unsigned long county = y.length();
    unsigned long countx = x.length();

    for (int i = 1; i <= county; i++)
    {
        unsigned int sum = x[countx - i] - '0' + y[county - i] - '0' + carry;
        string number = to_string(sum % 10);
        result.append(number);
        carry = sum / 10;
    }
    
    for (unsigned long i = county + 1; i <= countx; i++)
    {
        unsigned int sum = x[countx - i] - '0' + carry;
        string number = to_string(sum % 10);
        result.append(number);
        carry = sum / 10;
    }
    
    if (carry == 1)
    {
        result.append("1");
    }
    reverse(result.begin(), result.end());
    return result;
}

// calculates the subtraction of smaller from bigger integer in string representation
// returns positive result
string stringSub(string x, string y)
{
    string result;
    int carry = 0;
    if (x.length() < y.length())
        y.swap(x);
    
    unsigned long county = y.length();
    unsigned long countx = x.length();
    int sum;
    
    for (int i = 1; i <= county; i++)
    {
        sum = x[countx - i] - '0' - y[county - i] + '0' + carry;
        if (sum < 0)
        {
            sum = x[countx - i] - '0' - y[county - i] + '0' + carry + 10;
            carry = -1;
        }
        else
        {
            carry = 0;
        }
        
        string number = to_string(sum);
        result.append(number);
    }
    
    for (int i = county + 1; i <= countx; i++)
    {
        sum = x[countx - i] - '0' + carry;
        if (sum < 0)
        {
            sum = x[countx - i] - '0' + carry + 10;
            carry = -1;
        }
        else
        {
            carry = 0;
        }
        string number = to_string(sum);
        result.append(number);
    }
    
    if (result.back() == '0')
    {
        result.pop_back();
    }
        
    reverse(result.begin(), result.end());
    return result;
}

// calculates the product of two positive integers, represented by strings, where one is a one digit number
string stringMul(string x, string y)
{
    string result;
    unsigned int carry = 0;
    if (x.length() < y.length())
        y.swap(x);
    
    unsigned long county = y.length();
    unsigned long countx = x.length();
    
    for (int i = 1; i <= countx; i++)
    {
        unsigned int sum = (x[countx - i] - '0') * (y[0] - '0') + carry;
        string number = to_string(sum % 10);
        result.append(number);
        carry = sum / 10;
    }
    
    if (carry >= 1)
    {
        string number = to_string(carry);
        result.append(number);
    }
    reverse(result.begin(), result.end());
    return result;
}

// returns integer x multiplied by the m-th power of ten in string representation
string stringAppend(string x, int n)
{
    for (int i = 0; i < n; i++)
        x += "0";
    return x;
}
