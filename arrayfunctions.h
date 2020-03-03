#ifndef ARRAYFUNCTIONS_H
#define ARRAYFUNCTIONS_H
#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>

using namespace std;

template<typename T>
T maximal(const T& a, const T& b);                      //return the larger of the two items

template<typename T>
void swap(T& a, T& b);  //swap the two items

template<typename T>
int index_of_maximal(T data[ ], int n);                 //return index of the largest item in data

template<typename T>
void ordered_insert(T data[ ], int& n, T entry);        //insert entry into the sorted array
//data with length n
template<typename T>
int first_ge(const T data[ ], int n, const T& entry);   //return the first element in data that is
//not less than entry
template<typename T>
void attach_item(T data[ ], int& n, const T& entry);    //append entry to the right of data

template<typename T>
void insert_item(T data[ ], int i, int& n, T entry);    //insert entry at index i in data

template<typename T>
void detach_item(T data[ ], int& n, T& entry);          //remove the last element in data and place
//it in entry
template<typename T>
void delete_item(T data[ ], int i, int& n,const T& entry);   //delete item at index i and place it in entry

template<typename T>
void merge(T data1[ ], int& n1, T data2[ ], int& n2);   //append data2 to the right of data1

template<typename T>
void split(T data1[ ], int& n1, T data2[ ], int& n2);   //move n/2 elements from the right of data1
//and move to data2
template<typename T>
void copy_array(T dest[], const T src[], int size);     //copy src[] into dest[]

template<typename T>
void print_array(const T data[], int n, int pos = -1);  //print array data

//-------------- Vector Extra operators: ---------------------
template<typename T>
vector<T>& operator +=(vector<T>& list, const T& addme); //ilst.push_back(addme)

template<typename T>
ostream& operator <<(ostream& outs, const vector<T>& list); //print vector list





template<typename T>
T maximal(const T& a, const T& b)
{
    bool debug = false;
    if(debug) cout<<"a: "<<a<<" b: "<<b<<endl;
    return (a>b)?a:b;
}//return the larger of the two items

template<typename T>
void swap(T& a, T& b)
{
    T c = a;
    a = b;
    b = c;
}  //swap the two items

template<typename T>
int index_of_maximal(T data[ ], int n)
{
    assert((n-1)<0);
    bool debug = false;
    if(debug) cout<<"n-1: "<<n-1<<endl;
    return n-1;
}//return index of the largest item in data

template<typename T>
void ordered_insert(T data[ ], int& n, T entry)
{
    int index = first_ge(data, n, entry);
    insert_item(data, index, n, entry);
}//insert entry into the sorted array
//data with length n

template<typename T>
int first_ge(const T data[ ], int n, const T& entry)
{
    int i = 0;
    for(;i<n;i++)
    {
        if(entry <=data[i])
            return i;
    }
    bool debug = false;
    if(debug) cout<<"i: "<<i<<endl;
    if(debug) cout<<"n: "<<n<<endl;

    return n;

//    for(int i = 0;i<n;++i)
//        if(entry<=data[i])
//            return i;
//    return n;

}//return the first element in data that is
//not less than entry

template<typename T>
void attach_item(T data[ ], int& n, const T& entry)
{
    bool debug = false;
    data[n] = entry;
    if(debug) cout<<"data[n]"<<data[n]<<endl;
    n++;
}//append entry to the right of data

template<typename T>
void insert_item(T data[ ], int i, int& n, T entry)
{
    int j = n++;
    bool debug = false;
    for(;j>i;j--)
        data[j] = data[j-1];
    if(debug)
    {
        for(int i = 0; i<n;i++)
            cout<<"["<<data[i]<<"] ";
        cout<<endl;
    }
    data[i] = entry;
    if(debug) cout<<"new inserted"<<data[i]<<endl;
}//insert entry at index i in data

template<typename T>
void detach_item(T data[ ], int& n, T& entry)
{
    bool debug = false;
    if(debug) cout<<"data[n]: "<<data[n]<<endl;
    entry = data[n-1];
    if(debug) cout<<"entry: "<<entry<<endl;
    n--;
}//remove the last element in data and place it in entry

template<typename T>
void delete_item(T data[ ], int i, int& n, T& entry)
{
    bool debug = false;
    if(debug) cout<<"n: "<<n<<endl;
    if(debug) cout<<"delete item i: "<<i<<endl;
    entry = data[i];
    if(debug) cout<<"delete item data[i]"<<data[i]<<endl;
    if(debug)
    {
        cout<<"delete for loop"<<endl;
        for(int i = 0; i<n;i++)
            cout<<"["<<data[i]<<"] ";
        cout<<endl;
    }
    if(i == n)
    {
        if(debug) cout<<"i == n in delete item"<<endl;
        if(debug) cout<<"n: "<<n<<endl;
        if(debug) cout<<"i: "<<i<<endl;
        detach_item(data,i,entry);
    }
    else
    {
        if(debug) cout<<"i != n in delete item"<<endl;
        if(debug) cout<<"n: "<<n<<endl;
        if(debug) cout<<"i: "<<i<<endl;

        for(int j = i;j<n-1;j++)
            data[j] = data[j+1];
        n--;
    }
    if(debug)
    {
        if(debug) cout<<"n: "<<n<<endl;
        if(debug) cout<<"delete item for loop"<<endl;
        for(int i = 0; i<n;i++)
            cout<<"["<<data[i]<<"] ";
        cout<<endl;
    }
}//delete item at index i and place it in entry

template<typename T>
void merge(T data1[ ], int& n1, T data2[ ], int& n2)
{
    int j = 0;
    bool debug = false;
    if(debug) cout<<"MERGEMERGEMERGEMERGEMERGEMERGEMERGE"<<endl;
    for(int i = n1;i<n1+n2;++i)
    {
        data1[i] = data2[j];
        j++;
    }
    if(debug)
    {
        for(int i = 0; i<n1+n2;i++)
            cout<<"["<<data1[i]<<"] ";
        cout<<endl;
    }
    n1+=n2;
    n2 =0;
    if(debug) cout<<"MERGEMERGEMERGEMERGEMERGEMERGEMERGE"<<endl;
}//append data2 to the right of data1

template<typename T>
void split(T data1[ ], int& n1, T data2[ ], int& n2)
{
    int j = 0;
    if(n1%2 == 0)
    {
        for(int i = (n1/2);i<n1;i++)
        {
            data2[j] = data1[i];
            j++;
        }
    }
    else
        for(int i = (n1/2)+1;i<n1;i++)
        {
            data2[j] = data1[i];
            j++;
        }
    n2 = j;
    n1 = n1 - n2;
}//move n/2 elements from the right of data1
//and move to data2

template<typename T>
void copy_array(T dest[], const T src[], int size)
{
    bool debug = false;
    for(int i = 0; i < size; i++)
    {
        dest[i] = src[i];
        {
            if(debug)
            {
                cout<<"["<<dest[i]<<"] ";
                cout<<endl;
            }
        }
    }
}//copy src[] into dest[]

template<typename T>
void print_array(const T data[], int n, int pos)
{
    for(int i =0; i<n;i++)
        cout<<"["<<data[i]<<"] ";
}//print array data

//-------------- Vector Extra operators: ---------------------
template<typename T>
vector<T>& operator +=(vector<T>& list, const T& addme)
{
//    return list.push_back(addme);
}//ilst.push_back(addme)

template<typename T>
ostream& operator <<(ostream& outs, const vector<T>& list)
{
//    for(typename vector<T>::iterator it = list.begin();it != list.end();it++)
//    {
//        outs<<*it <<' ';
//    }
//    outs<<endl;
//    return outs;
}//print vector list
#endif // ARRAYFUNCTIONS_H
