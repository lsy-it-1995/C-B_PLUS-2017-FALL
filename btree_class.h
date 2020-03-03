#ifndef BTREE_CLASS_H
#define BTREE_CLASS_H
#include <iostream>
#include <cassert>
#include <iomanip>
#include "arrayfunctions.h"

using namespace std;

template <class T>
class BTree
{
public:
    BTree(bool dups = true)
    {
        dups_ok = dups;
        child_count = 0;
        data_count = 0;
        size_number = 0;
    }

    BTree(const BTree<T>& other)
    {
        copy_tree(other);
    }
    ~BTree()
    {
        clear_tree();
    }
    BTree<T>& operator =(const BTree<T>& RHS)
    {
        if(this!=&RHS){
            clear_tree();
            copy_tree(RHS);
        }
        return *this;

    }

    void insert(const T& entry);                //insert entry into the tree
    void remove(const T& entry);                //remove entry from the tree

    void clear_tree(); //clear this object (delete all nodes etc.)
    void copy_tree(const BTree<T>& other); //copy other into this object

    bool contains(const T& entry); //true if entry can be found in the array

    T& get(const T& entry)
    {
        bool debug = false;
        int i = first_ge(data, data_count, entry);
        //        bool found = (i<data_count && data[i]==entry);
        if(debug) cout<<"i: "<<i<<endl;
        if(contains(entry))
        {
            if(debug) cout<<"data[i]"<<data[i]<<endl;
            if(debug) cout<<"found"<<endl;
            return data[i];
        }
        if(is_leaf())
        {
            cout<<"Get with non-existing entry: "<<entry<<endl;
            insert(entry);
            return get(entry);
        }
        if(!contains(entry))
        {
            if(debug) cout<<"NOT found"<<endl;
            return subset[i]->get(entry);
        }
    }//return a reference to entry in the tree

    T* find(const T& entry)
    {
        int i = first_ge(data, data_count, entry);
        if(data[i]==entry)
            return &data[i];
        else if(is_leaf())
            return NULL;
        else
            return subset[i]->find(entry);
    }//return a pointer to this key. NULL if not there.

    int size() const
    {
        return size_number;
    }
    //count the number of elements in the tree

    bool empty() const
    {
        return (data_count==0);
    }//true if the tree is empty

    void print_tree(int level = 0, ostream &outs=cout) const; //print a readable version of the tree

    friend ostream& operator<<(ostream& outs, const BTree<T>& print_me)
    {
        print_me.print_tree(0,outs);
        return outs;
    }
private:
    static const int MINIMUM = 1;
    static const int MAXIMUM = 2 * MINIMUM;

    bool dups_ok;                                   //true if duplicate keys may be inserted
    int data_count;                                 //number of data elements
    int size_number;
    T data[MAXIMUM + 1];                            //holds the keys
    int child_count;                                //number of children
    BTree* subset[MAXIMUM + 2];                     //subtrees

    bool is_leaf() const
    {
        return child_count==0;
    }   //true if this is a leaf node

    //insert element functions
    bool loose_insert(const T& entry);              //allows MAXIMUM+1 data elements in the root
    void fix_excess(int i);                         //fix excess of data elements in child i

    //remove element functions:
    bool loose_remove(const T& entry);              //allows MINIMUM-1 data elements in the root
    void fix_shortage(int i);                       //fix shortage of data elements in child i

    void remove_biggest(T& entry);

    void rotate_left(int i); //transfer one element LEFT from child i
    void rotate_right(int i);                       //transfer one element RIGHT from child i
    void merge_with_next_subset(int i);             //merge subset i with subset i+1
};

template<class T>
void BTree<T>:: copy_tree(const BTree<T> &other)
{
    bool debug = false;
    if(debug) cout<<"in copy tree"<<endl;
    copy_array(data,other.data,other.data_count);
    data_count = other.data_count;
    if(!other.is_leaf())
    {
        child_count = other.child_count;
        for(int i = 0; i<other.child_count;i++)
        {
            if(debug) cout<<"in for loop"<<endl;
            BTree<T> *temp = new BTree;
            subset[i] = temp;
            subset[i]->copy_tree(*other.subset[i]);
        }
    }
}
template<class T>
void BTree<T>::merge_with_next_subset(int i)
{
    T item;
    bool debug = false;
    if(debug) cout<<"merge_with_next_subsetmerge_with_next_subsetmerge_with_next_subset"<<endl;
    if(debug) cout<<"\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\"<<endl;
    if(debug) cout<<"i: "<<i<<endl;
    if(debug) print_tree();
    if(debug) cout<<"\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\"<<endl;
    /*
     *  Merge subset[i] with subset [i+1] with data[i] in the middle
     *
     *   1. remove data[i] from this object
     *   2. append it to child[i]->data
     *   3. Move all data items from subset[i+1]->data to subset[i]->data
     *   4. Move all subset pointers from subset[i+1]->subset to subset[i]->subset
     *   5. delete subset[i+1] (store in a temp ptr)
     *   6. delete temp ptr
     */
    //remove item in the top

    delete_item(data,i,data_count,item);
    if(debug) cout<<"delete item: "<<item<<endl;
    //adding in subset[i-1]
    attach_item(subset[i]->data,subset[i]->data_count,item);
    //    ordered_insert();
    if(debug) cout<<"ordered insert item: "<<item<<endl;
    if(debug) print_tree();
    if(debug) cout<<"************************"<<endl;

    merge(subset[i]->data,subset[i]->data_count,subset[i+1]->data,subset[i+1]->data_count);
    merge(subset[i]->subset,subset[i]->child_count,subset[i+1]->subset,subset[i+1]->child_count);
    if(debug) cout<<"after 2 merges"<<endl;
    if(debug) print_tree();
    if(debug) cout<<"************************"<<endl;
    BTree<T> *trash;
    delete_item(subset,i+1,child_count,trash);
    trash->child_count = 0;
    delete trash;

    if(debug) cout<<"merge_with_next_subsetmerge_with_next_subsetmerge_with_next_subset"<<endl;
}

template<class T>
void BTree<T>::remove(const T &entry)
{
    //Loose_remove the entry from this tree.
    //once you return from loose_remove, the root (this object) may have no data and only a single subset
    //now, the tree must shrink:
    //  point a temporary pointer (shrink_ptr) and point it to this root's only subset
    //  copy all the data and subsets of this subset into the root (through shrink_ptr)
    //  now, the root contains all the data and poiners of it's old child.
    //  now, simply delete shrink_ptr (blank out child), and the tree has shrunk by one level.
    //  Note, the root node of the tree will always be the same, it's the child node we delete
    bool debug = false;
    if(loose_remove(entry))
    {
        if(debug) cout<<"REMOVE!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
        if((data_count == 0) && (child_count ==1 ))
        {
            if(debug) cout<<"(data_count == 0) && (child_count ==1) "<<endl;
            BTree<T> *temp = subset[0];
            copy_array(data,temp->data,temp->data_count);
            copy_array(subset,temp->subset,temp->child_count);
            data_count = temp->data_count;
            child_count = temp->child_count;
            temp->child_count = 0;
            delete temp;
        }
        if(debug) cout<<"REMVOE!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;

    }
    else
    {
        cout<<"DIDN'T REMOVE THE ITEM(CAN'T FIND)"<<endl;
    }


}
//Loose_remove the entry from this tree.
//once you return from loose_remove, the root (this object) may have no data and only a single subset
//now, the tree must shrink:
//  point a temporary pointer (shrink_ptr) and point it to this root's only subset
//  copy all the data and subsets of this subset into the root (through shrink_ptr)
//  now, the root contains all the data and poiners of it's old child.
//  now, simply delete shrink_ptr (blank out child), and the tree has shrunk by one level.
//  Note, the root node of the tree will always be the same, it's the child node we delete

template<class T>
void BTree<T>::fix_shortage(int i)
{
    bool debug = false;
    if(debug) cout<<"fix shortagefix shortagefix shortagefix shortagefix shortagefix shortage"<<endl;
    if(debug) cout<<"i: "<<i<<endl;
    if(i<child_count-1&&subset[i+1]->data_count>MINIMUM)
    {
        if(debug) cout<<"i<child_count-1&&subset[i]->data_count>MINIMUM"<<endl;

        if(debug) cout<<"rotate left"<<endl;
        rotate_left(i);
    }
    else if(i>0&&subset[i-1]->data_count>MINIMUM)
    {
        if(debug) cout<<"i>0&&subset[i-1]->data_count>MINIMUM"<<endl;

        if(debug) cout<<"rotate right"<<endl;
        rotate_right(i);
    }
    else if(i<child_count-1)//merge right
    {
        if(debug) cout<<"subset[i-1]->child_count==MINIMUM&&i>0"<<endl;
        if(debug) cout<<"merge right"<<endl;
        merge_with_next_subset(i);
    }
    else if(i>0)//merge left
    {
        if(debug) cout<<"merge left"<<endl;
        if(debug) cout<<"merge!!!"<<endl;
        merge_with_next_subset(i-1);
    }
    if(debug) cout<<"fix shortagefix shortagefix shortagefix shortagefix shortagefix shortage"<<endl;

}

template<class T>
void BTree<T>::rotate_left(int i)
{
    bool debug = false;
    T item;
    BTree<T> *temp;

    if(!subset[i+1]->is_leaf())
    {
        if(debug) cout<<"FFFFFFFFFFFFFFFFFFFFFFFFFFFFF"<<endl;
        if(debug) cout<<"in !subset[i]->is_leaf()"<<endl;
        delete_item(subset[i+1]->subset,0,subset[i+1]->child_count,temp);
        attach_item(subset[i]->subset,subset[i]->child_count,temp);

    }
    if(debug) cout<<"rotate_leftrotate_leftrotate_leftrotate_leftrotate_left"<<endl;
    if(debug) cout<<"i: "<<i<<endl;
    if(debug) cout<<"is leaf or not"<<!subset[i]->is_leaf()<<endl;
    if(debug) cout<<"how many child in subset[i]"<<subset[i]->child_count<<endl;

    delete_item(subset[i+1]->data,0,subset[i+1]->data_count,item);
    ordered_insert(data,data_count,item);
    delete_item(data,i,data_count,item);
    ordered_insert(subset[i]->data,subset[i]->data_count,item);
    if(debug) cout<<"rotate_leftrotate_leftrotate_leftrotate_leftrotate_left"<<endl;
}
template<class T>
void BTree<T>::rotate_right(int i)
{
    bool debug = false;
    T item;
    BTree<T> *temp;

    if(!subset[i-1]->is_leaf())
    {
        if(debug) cout<<"OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"<<endl;
        if(debug) cout<<"in subset[i]->is_leaf()"<<endl;
        delete_item(subset[i-1]->subset,subset[i-1]->child_count-1,subset[i-1]->child_count,temp);
        insert_item(subset[i]->subset,0,subset[i]->child_count,temp);
    }

    if(debug) cout<<"rotate_rightrotate_rightrotate_rightrotate_rightrotate_rightrotate_right"<<endl;
    if(debug) cout<<"i: "<<i<<endl;
    if(debug) cout<<"is leaf or not"<<!subset[i]->is_leaf()<<endl;
    if(debug) cout<<"how many child in subset[i]"<<subset[i]->child_count<<endl;
    if(debug) cout<<"tree"<<endl;print_tree();

    delete_item(subset[i-1]->data,subset[i-1]->data_count-1,subset[i-1]->data_count,item);
    if(debug) cout<<"delete item: "<<item<<endl;
    ordered_insert(data,data_count,item);
    if(debug) cout<<"ordered insert item: "<<item<<endl;

    delete_item(data,i,data_count,item);
    if(debug) cout<<"delete item: "<<item<<endl;
    if(debug) cout<<"tree"<<endl;print_tree();
    ordered_insert(subset[i]->data,subset[i]->data_count,item);
    if(debug) cout<<"ordered inserted item: "<<item<<endl;
    if(debug) cout<<"rotate_rightrotate_rightrotate_rightrotate_rightrotate_rightrotate_right"<<endl;
}

template<class T>
bool BTree<T>::loose_remove(const T &entry)
{
    bool debug = false;
    T item;
    int i = first_ge(data,data_count,entry);
    if(debug) cout<<"loose_removeloose_removeloose_removeloose_removeloose_remove"<<endl;
    bool found = (i<data_count&&entry == data[i]);
    if(debug) cout<<"i<data_count-1: "<<data_count-1<<endl;
    if(debug) cout<<"loose remove starts"<<endl;
    if(debug) cout<<"i: "<<i<<endl;
    if(debug) cout<<"data[i]"<<data[i]<<endl;
    //not leaf and not found target: recursive call to loose_remove
    if(!is_leaf()&&!found)
    {
        if(debug) cout<<"first case"<<endl;

        subset[i]->loose_remove(entry);
        if(subset[i]->data_count<MINIMUM)
        {
            if(debug) cout<<"subset datacount < MINIMUM"<<endl;
            fix_shortage(i);
        }
        return true;
    }
    //leaf && not found target: there is nothing to do
    else if(is_leaf()&&!found)
    {
        if(debug) cout<<"second case"<<endl;
        return false;
    }
    //leaf && found target: just remove the target
    else if(is_leaf()&&found)
    {

        if(debug) cout<<"third case"<<endl;
        if(debug) cout<<"data count: "<<data_count<<endl;
        if(debug) cout<<"||||||||||||||||||||"<<endl;
        if(debug) print_tree();
        if(debug) cout<<"||||||||||||||||||||"<<endl;
        delete_item(data,i,data_count,item);
        if(debug) cout<<"loose remove item: "<<item<<endl;
        if(debug) cout<<"end of third case"<<endl;
        size_number--;
        return true;

    }
    //not leaf and found: replace target with largest child of subset[i]
    else /*if(!is_leaf()&&found)*/
    {
        if(debug) cout<<"fourth case"<<endl;
        if(debug) cout<<"data count: "<<data_count<<endl;
        if(debug) cout<<"Child Count: "<<subset[i]->child_count<<endl;
        subset[i]->remove_biggest(data[0]);
        size_number--;
        if(debug) print_tree();
        if(subset[i]->data_count<MINIMUM)
        {
            if(debug) cout<<"if statement in fourth case"<<endl;
            fix_shortage(i);
        }
        return true;
    }
    if(debug) cout<<"sth missing"<<endl;
    if(debug) cout<<"loose_removeloose_removeloose_removeloose_removeloose_remove"<<endl;
}
/* four cases:
        |   !found    |   found       |
  ------|-------------|---------------|-------
  leaf  |  a: nothing | b: delete     |
        |     to do   |    target     |
  ------|-------------|---------------|-------
  !leaf | c: loose_   | d: replace    |
        |    remove   |    w/ biggest |
  ------|-------------|---------------|-------
*/

template<class T>
void BTree<T>::remove_biggest(T& entry)
{
    bool debug = false;
    if(debug) cout<<"remove biggest starts"<<endl;
    if(is_leaf())
    {
        if(debug) cout<<"isleaf"<<endl;
        detach_item(data,data_count,entry);
    }
    else
    {
        if(debug) cout<<"else"<<endl;
        subset[child_count-1]->remove_biggest(entry);
        if(debug) cout<<"subset[child_count-1]->data_count"<<subset[child_count-1]->data_count<<endl;
        if(subset[child_count-1]->data_count<MINIMUM)
        {
            if(debug) cout<<"remove biggest fix shortage"<<endl;
            fix_shortage(child_count-1);
        }
    }
    if(debug) cout<<"entry"<<entry<<endl;
    if(debug) cout<<"remove biggest end"<<endl;
}

template<class T>
bool BTree<T>:: contains(const T &entry)
{
    bool debug = false;
    if(debug) cout<<"-----------------------"<<endl;
    if(debug) cout<<"entry: "<<entry<<endl;
    int i = first_ge(data,data_count,entry);
    if(debug) cout<<"data[i]: "<<data[i]<<endl;
    if(debug) cout<<"i: "<<i<<endl;
    if(data[i]==entry)
    {
        if(debug) cout<<"FOUND"<<endl;
        if(debug) cout<<"data[i]"<<data[i]<<endl;
        if(debug) cout<<"i: "<<i<<endl;
        return true;
    }
    else if(is_leaf())
    {
        if(debug) cout<<"NOT FOUND"<<endl;
        return false;
    }
    else
    {
        if(debug) cout<<"recurrsion"<<endl;
        return subset[i]->contains(entry);
    }
    if(debug) cout<<"-----------------------"<<endl;

}

template<class T>
void BTree<T>::clear_tree()
{
    for(int i = 0; i< child_count;i++)
    {
        subset[i]->clear_tree();
        subset[i]->child_count = 0;
        data_count = 0;
        delete subset[i];
    }
    child_count = 0;
    data_count = 0;
    size_number = 0;
}

template<class T>
void BTree<T>::insert(const T& entry)
{
    bool debug = false;
    if(loose_insert(entry))
    {
        if(data_count>MAXIMUM)
        {
            if(debug) cout<<"in insert datacount > max"<<endl;
            BTree<T> *temp = new BTree<T>;
            copy_array(temp->data,data,data_count);
            copy_array(temp->subset,subset,child_count);
            temp->child_count = child_count;
            temp->data_count = data_count;
            child_count = 1;
            data_count = 0;
            subset[0] = temp;
            fix_excess(0);
        }
    }
}//insert entry into the tree

template<class T>
bool BTree<T>::loose_insert(const T& entry)
{
    int i = first_ge(data,data_count,entry);
    bool found = (i<data_count&&entry == data[i]);

    bool debug = false;
    if(debug) cout<<"*****************************"<<endl;
    if(debug) cout<<"i"<<i<<endl;
    if(found)
    {
        if(debug) cout<<"entry"<<entry<<endl;
        data[i] = entry;
        if(debug) cout<<"data[i]"<<data[i]<<endl;
        size_number++;
        return false;
    }
    else if(is_leaf())
    {
        if(debug) cout<<"is_leaf"<<endl;
        ordered_insert(data,data_count,entry);
        size_number++;
        return true;
    }
    else
    {
        size_number++;
        if(debug) cout<<"else"<<endl;
        if(subset[i]->loose_insert(entry))
        {

            if(subset[i]->data_count>MAXIMUM)
                fix_excess(i);

            return true;
        }
        return true;
    }

    if(debug) cout<<"*****************************"<<endl;
}//allows MAXIMUM+1 data elements in the root

template<class T>
void BTree<T>::fix_excess(int i)
{
    //    assert(i<child_count);
    BTree<T> *temp = new BTree<T>;
    T item;
    insert_item(subset,i+1,child_count,temp);
    split(subset[i]->data,subset[i]->data_count,subset[i+1]->data,subset[i+1]->data_count);
    split(subset[i]->subset,subset[i]->child_count,subset[i+1]->subset,subset[i+1]->child_count);
    detach_item(subset[i]->data,subset[i]->data_count,item);
    ordered_insert(data,data_count,item);
}//fix excess of data elements in child i

template<class T>
void BTree<T>::print_tree(int level, ostream &outs) const
{
    bool debug = false;
    if(debug) cout<<"in print tree"<<endl;
    if(child_count>MINIMUM)
    {
        if(debug) cout<<"childcount>MIN"<<endl;
        if(debug) cout<<"child count: "<<child_count<<endl;
        subset[child_count-1]->print_tree(level+1,outs);
    }
    if(debug) cout<<"data count: "<<data_count<<endl;
    for(int i = 0; i <data_count;i++)
    {
        outs<<setw(level*4)<<data[data_count-i-1]<<endl;
        if(!is_leaf())
        {
            if(debug) cout<<"!is_leaf"<<endl;
            subset[data_count-i-1]->print_tree(level+1,outs);
        }
    }
    if(debug) cout<<"end of print tree"<<endl;
}
#endif // BTREE_CLASS_H
