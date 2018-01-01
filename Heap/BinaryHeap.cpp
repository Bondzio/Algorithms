#include <iostream>
#include <vector>
using namespace std;
template <typename T>
class MaxHeap{
public:
    MaxHeap(){
    }

    MaxHeap(const vector<T>&v){
        heap_vector=v;
        buildHeap(heap_vector);
    }

    void push(const T& x){
        heap_vector.push_back(x);
        size_t i=heap_vector.size()-1;
        while(i>0&&heap_vector[parent(i)]<x){
            heap_vector[i]=heap_vector[parent(i)];
            i=parent(i);
        }
        heap_vector[i]=x;


    }
    void pop(size_t i){
        heap_vector[i]=heap_vector.back();
        heap_vector.pop_back();
        maxHeapify(heap_vector,i);
    }
    void max(){
        return heap_vector[0];
    }

    void increaseKey(size_t i,const T& x){
        if(heap_vector[i]<x){
            heap_vector[i]=x;
            while(i>0&&heap_vector[parent(i)]<x){
                heap_vector[i]=heap_vector[parent(i)];
                i=parent(i);
            }
            heap_vector[i]=x;

        }

    }
    void buildHeap(vector<T> &v){
        for(int i=v.size()/2-1;i>=0;i--){
            maxHeapify(v,i);
        }

    }

    void print(){
        for(auto iter=heap_vector.cbegin();iter!=heap_vector.cend();iter++){
            cout<<*iter<<" ";
        }
        cout<<endl;
    }

private:
    vector<T> heap_vector;
    void maxHeapify(vector<T> &v,size_t i){
        T tmp=v[i];
        size_t pos=i;
        if(left(i)<v.size() &&v[left(i)]>v[pos]){
            pos=left(i);
        }
        if(right(i)<v.size() &&v[right(i)]>v[pos]){
            pos=right(i);
        }
        if(pos!=i){
            v[i]=v[pos];
            v[pos]=tmp;
            maxHeapify(v,pos);
        }

    }

    inline size_t parent(size_t i){
        return (i-1)>>1;
    }
    inline size_t left(size_t i){
        return (i<<1)+1;
    }
    inline size_t right(size_t i){
        return (i<<1)+2;
    }


};

int main()
{
    vector<double> v{1.0,2.0,-1.0,-1.5,2.5,1.5};
    MaxHeap<double> heap(v);
    for(int i=-10;i<11;i++){
        heap.push(i);
    }

    heap.print();
}
