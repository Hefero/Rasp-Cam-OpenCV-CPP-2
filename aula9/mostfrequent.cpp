// CPP program to find the most frequent element 
// in an array. 
#include <bits/stdc++.h> 
using namespace std; 
  
int mostFrequent(int* arr, int n) 
{ 
    // code here 
    int maxcount = 0; 
    int element_having_max_freq; 
    for (int i = 0; i < n; i++) { 
        int count = 0; 
        for (int j = 0; j < n; j++) { 
            if (arr[i] == arr[j]) 
                count++; 
        } 
  
        if (count > maxcount) { 
            maxcount = count; 
            element_having_max_freq = arr[i]; 
        } 
    } 
  
    return element_having_max_freq; 
} 
int calcMode(int array[], int array_size)
{
int topCount=0, count, topElement = 10;

for ( int i=0 ; i<array_size ;i++)
{
    count=0;
    for (int j=0 ; j<array_size ; j++)
    {
        if (array[i]==array[j]) count++;
    }
    if (count>=topCount)
    {
        topCount=count;
        if (topElement > array[i])
            topElement=array[i];
    }
}

return topElement;
}
// Driver program 
int main() 
{ 
    int arr[] = { 40, 50, 30, 40, 50, 30, 30};
    int n = 6;//sizeof(arr) / sizeof(arr[0]); 
    cout << calcMode(arr, n); 
    return 0; 
} 