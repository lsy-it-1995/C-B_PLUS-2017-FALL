#include <iostream>
//#include "arrayfunctions.h"
#include "btree_class.h"
#include "locale"

using namespace std;
void mymain();
void ordered_insert_test();
void btree_test();
int Random(int low,int hi);
void test_bplustree_auto(int tree_size, int how_many, bool report);
bool test_bplustree_auto(int how_many, bool report);


int main()
{
    btree_test();
    //mymain();
    return 0;
}

void mymain()
{
    BTree<int> bt;
    BTree<int> bt2;
    char userinput;
    int usernumber = 0;
    int MAX = 1000;
    int MIN = 1;
    cout<<"[R]andon [I]nsert [C]lear c[O]py [D]elete [F]ind [S]ize [E]xit "<<endl;
    while(cin>>userinput)
    {
        userinput = toupper(userinput);
        switch (userinput)
        {
        case 'R':
        {
            usernumber = ((rand()%MAX)+ MIN+ 1);
            bt.insert(usernumber);
            cout<<"---------BTREE is now like this ---------"<<endl;
            bt.print_tree();
            break;
        }
        case 'C':
        {
            bt.clear_tree();
            cout<<"---------BTREE is now like this ---------"<<endl;
            bt.print_tree();
            break;
        }
        case 'O':
        {
            bt2.copy_tree(bt);
            cout<<"---------BTREE is now like this ---------"<<endl;
            bt.print_tree();
            cout<<"---------BTREE COPY is now like this ---------"<<endl;
            bt2.print_tree();
            break;
        }
        case 'I':
        {
            cin>>usernumber;
            cout<<"USER NUMBER IS "<<usernumber<<endl;
            bt.insert(usernumber);
            cout<<"---------BTREE is now like this ---------"<<endl;
            bt.print_tree();
            break;
        }
        case 'D':
        {
            cin>>usernumber;
            if(bt.find(usernumber))
                cout<<"USER NUMBER IS "<<usernumber<<endl;
            else
                cout<<"NOT IN THIS B-TREE"<<endl;
            bt.remove(usernumber);
            cout<<"---------BTREE is now like this ---------"<<endl;
            bt.print_tree();
            break;
        }
        case 'F':
        {
            cin>>usernumber;
            if(bt.find(usernumber))
                cout<<"FOUND: "<<usernumber<<endl;
            else
                cout<<"NOT FOUND"<<endl;
            break;
        }
        case 'S':
        {
            cout<<"Size is: "<<bt.size()<<endl;
            break;
        }
        case 'E':
        {
            exit(1);
            break;
        }
        default:
            break;
        }
        cout<<"[R]andon [I]nsert [C]lear c[O]py [D]elete [F]ind [S]ize [E]xit "<<endl;

    }
    cout << "Hello World!" << endl;
}

void btree_test(){
    cout <<endl<<endl<<endl<< "===============================" << endl<<endl<<endl<<endl;

    //------------------------------------------
    srand(time(0));
    //------------------------------------------



    //    test_bplustree_insert_random();
    //    test_bplustree_remove();
    //    test_bplustree_interactive();
    //    test_bplustree_big_three();
    test_bplustree_auto(100000000,100000000,false);

    //    test_map();
    //    test_map_interactive();

    //    test_multimap();
    //    test_multimap_interactive();
    //    test_multimap_auto();

    //    test_iterator();
    cout <<endl<<endl<<endl<< "===============================" << endl;
}
void BPlusTreeTest(){
    cout <<endl<<endl<<endl<< "===============================" << endl<<endl<<endl<<endl;

    //------------------------------------------
    srand(time(0));
    //------------------------------------------



    //    test_bplustree_insert_random();
    //    test_bplustree_remove();
    //    test_bplustree_interactive();
    //    test_bplustree_big_three();
    test_bplustree_auto(100,100,false);

    //    test_map();
    //    test_map_interactive();

    //    test_multimap();
    //    test_multimap_interactive();
    //    test_multimap_auto();

    //    test_iterator();
    cout <<endl<<endl<<endl<< "===============================" << endl;
}
void test_bplustree_auto(int tree_size, int how_many, bool report){
    bool verified = true;
    for (int i = 0; i< how_many; i++){
        if (report){
            cout<<"*********************************************************"<<endl;
            cout<<" T E S T:    "<<i<<endl;
            cout<<"*********************************************************"<<endl;
        }
        if(report)
            if (!test_bplustree_auto(tree_size, report)){
                cout<<"T E S T :   ["<<i<<"]    F A I L E D ! ! !"<<endl;
                verified = true;
                return;
            }

    }
    cout<<"**************************************************************************"<<endl;
    cout<<"**************************************************************************"<<endl;
    cout<<"             E N D     T E S T: "<<how_many<<" tests of "<<tree_size<<" items: ";
    cout<<(verified?"VERIFIED": "VERIFICATION FAILED")<<endl;
    cout<<"**************************************************************************"<<endl;
    cout<<"**************************************************************************"<<endl;

}
bool test_bplustree_auto(int how_many, bool report){
    const int MAX = 100000000;
    assert(how_many < MAX);
    BTree<int> bpt;
    int a[MAX];
    int original[MAX];
    int deleted_list[MAX];

    int original_size;
    int size;
    int deleted_size = 0;

    //fill a[ ]
    for (int i= 0; i< how_many; i++){
        a[i] = i;
    }
    //shuffle a[ ]: Put this in a function!
    for (int i = 0; i< how_many+1; i++){
        int from = Random(0, how_many-1);
        int to = Random(0, how_many -1);
        int temp = a[to];
        a[to] = a[from];
        a [from] = temp;
    }
    //copy  a[ ] -> original[ ]:
    copy_array(original, a, how_many);
    size = how_many;
    original_size = how_many;
    for (int i=0; i<size; i++){
        bpt.insert(a[i]);
    }
    if (report){
        cout<<"========================================================"<<endl;
        cout<<"  "<<endl;
        cout<<"========================================================"<<endl;
        cout<<bpt<<endl<<endl;
    }
    for (int i= 0; i<how_many; i++){
        int r = Random(0, how_many - i );
        if (report){
            cout<<"========================================================"<<endl;
            cout<<bpt<<endl;
            cout<<". . . . . . . . . . . . . . . . . . . . . . . . . . . . "<<endl;
            cout<<"deleted: "; print_array(deleted_list, deleted_size);
            cout<<"   from: "; print_array(original, original_size);
            cout<<endl;
            cout<<"  REMOVING ["<<a[r]<<"]"<<endl;
            cout<<"========================================================"<<endl;
        }
        bpt.remove(a[r]);


        delete_item(a, r, size, deleted_list[deleted_size++]);
        //        if (!bpt.is_valid()){
        //            cout<<setw(6)<<i<<" I N V A L I D   T R E E"<<endl;
        //            cout<<"Original Array: "; print_array(original, original_size);
        //            cout<<"Deleted Items : "; print_array(deleted_list, deleted_size);
        //            cout<<endl<<endl<<bpt<<endl<<endl;
        //            return false;
        //        }

    }
    if (report) cout<<" V A L I D    T R E E"<<endl;

    return true;
}
int Random(int low,int hi){
    return rand()%hi+low;
}
