#include <iostream>
#include <vector>
using namespace std;
template <typename T>
class FenwickTree{
private:
    vector<T> value;
    vector<T> bit;
    T query(int right){
        T result=0;
        while(right){
            result+=bit[right];
            right-=lowbit(right);
        }
        return result;
    }
    inline int lowbit(int x){
        return x&-x;
    }

public:
    FenwickTree(const vector<T> &init):value(init.size(),0),bit(init.size(),0){
        for(size_t i=1;i<init.size();i++){
            update(i,init[i]);
        }
    }
    void update(size_t position ,const T&new_value){
        T d_value=new_value-value[position];
        value[position]=new_value;
        while(position<bit.size()){
            bit[position]+=d_value;
            position+=lowbit(position);
        }
    }
    T query(int left,int right){
        return query(right)-query(left-1);
    }
};

int main(){
    vector<float> v={0,1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0};
    FenwickTree<float> ft(v);
    ft.update(1,2);
    ft.update(10,0);

    for(size_t left=1;left<v.size();left++){
        for(size_t right=left;right<v.size();right++){
            cout<<"sum["<<left<<","<<right<<"]:"<<ft.query(left,right)<<endl;
        }
    }
}
