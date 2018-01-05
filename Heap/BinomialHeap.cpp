#include <iostream>
#include <vector>
#include <memory>
#include <cmath>
#include <queue>
#include <list>
using namespace std;
template <typename T>
class packet{
public:
    packet()=default;

    packet(const T& key){
        this->key=key;
        this->size=1;
    }

    friend shared_ptr<packet<T>> meldTwoPackets(shared_ptr<packet<T>> p1,shared_ptr<packet<T>> p2){
        if(p1->size==0){
            return p2;
        }
        if(p2->size==0){
            return p1;
        }
        if(p1->key<p2->key){
            p2->parent=p1;
            p1->son.push_front(p2);
            p1->size*=2;
            return p1;
        }else{
            p1->parent=p2;
            p2->son.push_front(p1);
            p2->size*=2;
            return p2;
        }
    }



    T top(){
        return key;
    }

    void print(){
        print_BFS(make_shared<packet<T>>(this));

    }
    void print_BFS(shared_ptr<packet<T>> root){
        queue<shared_ptr<packet<T>>> bfs;
        bfs.push(make_shared<packet<T>>(root));
        while(!bfs.empty()){
            shared_ptr<packet<T>> tmp=bfs.top();
            bfs.pop();
            cout<<tmp->key<<" ";
            shared_ptr<packet<T>>  son_iter=tmp->son;
            while(son_iter){
                bfs.push(make_shared<packet<T>>(son_iter));
                son_iter=son_iter->next;
            }
        }

    }

    size_t size=0;

    virtual ~packet()=default ;


    inline bool isValidSize(const int n){
        return n&(n-1)==0;
    }
//    shared_ptr<packet<T>> son;
    list<shared_ptr<packet<T>>>son;
//    shared_ptr<packet<T>> next;
    shared_ptr<packet<T>> parent;
    T key;
};


//min binominalHeap
template <typename T>
class binominalHeap{
public:
    binominalHeap()=default;
    binominalHeap(const T&x){
        binominalTrees.push_back(make_shared<packet<T>>(x));

    }
    binominalHeap(vector<T>& v){
        for(auto iter=v.cbegin();iter!=v.cend();iter++){
            this->push(*iter);
        }
    }

    void push(const T& x){
        auto one=make_shared<binominalHeap<T>>(x);
        this->meldWith(one);
    }

    T top(){
        auto iter=binominalTrees.cbegin();
        while((*iter)->size==0){
            iter++;
        }
        T result= (*iter)->top();
        for(;iter!=binominalTrees.cend();iter++){
            if((*iter)->size>0){
                if((*iter)->top()<result){
                    result=(*iter)->top();
                }
            }
        }
        return result;
    }

    void pop(){
        auto iter=binominalTrees.cbegin();
        while((*iter)->size==0){
            iter++;
        }
        T result= (*iter)->top();
        size_t min_position=iter-binominalTrees.cbegin();
        for(;iter!=binominalTrees.cend();iter++){
            if((*iter)->size>0){
                if((*iter)->top()<result){
                    result=(*iter)->top();
                    min_position=iter-binominalTrees.cbegin();
                }
            }
        }
        auto remain=binominalTrees[min_position]->son;
        if(remain.size()>0){
            remain.reverse();

            binominalHeap<T> another;

            for(auto iter=remain.begin();iter!=remain.end();iter++){
                (*iter)->parent=nullptr;
                another.binominalTrees.push_back(*iter);

            }
            binominalTrees[min_position]=make_shared<packet<T>>();
            meldWith(another);
        }
    }

    void meldWith(shared_ptr<binominalHeap<T>> another){
       size_t i;
       shared_ptr<packet<T>> bonus=make_shared<packet<T>>();
       size_t i_position_size=1;
       if(another->getBinominalTress().size()>this->binominalTrees.size()){
           for(size_t extra=0;extra<another->getBinominalTress().size()-this->binominalTrees.size();extra++){
               this->binominalTrees.push_back(make_shared<packet<T>>());
           }
       }

       for(i=0;i<another->getBinominalTress().size();i++){

           shared_ptr<packet<T>> tmp=meldTwoPackets(this->binominalTrees[i],another->getBinominalTress()[i]);
           if(tmp->size==0){
               this->binominalTrees[i]=bonus;
               bonus=make_shared<packet<T>>();
           }else if(tmp->size==i_position_size){
               tmp=meldTwoPackets(tmp,bonus);
               if(tmp->size==i_position_size){
                   this->binominalTrees[i]=tmp;
                   bonus=make_shared<packet<T>>();
               }else{
                   this->binominalTrees[i]=make_shared<packet<T>>();
                   bonus=tmp;
               }

           }else{
               this->binominalTrees[i]=bonus;
               bonus=tmp;
           }
           i_position_size<<=1;
       }
       if(bonus->size>0){
           for(size_t j=i;j<this->binominalTrees.size();j++){
                shared_ptr<packet<T>> tmp=meldTwoPackets(this->binominalTrees[j],bonus);
                if(tmp->size==i_position_size){
                    this->binominalTrees[j]=tmp;
                    bonus=make_shared<packet<T>>();
                    break;
                }else{
                    this->binominalTrees[j]=make_shared<packet<T>>();
                    bonus=tmp;
                }
                i_position_size<<=1;
           }
           if(bonus->size>0){
               this->binominalTrees.push_back(bonus);
           }
       }
    }
    void meldWith(binominalHeap<T> another){
        meldWith(make_shared<binominalHeap<T>>(another));
    }

    vector<shared_ptr<packet<T>>> getBinominalTress(){
        return this->binominalTrees;
    }
//    void print(){
//        for(auto iter=binominalTrees.cbegin();iter!=binominalTrees.cend();iter++){
//            (*iter)->print();
//            cout<<endl;
//        }
//    }


    vector<shared_ptr<packet<T>>> binominalTrees;
};


int main()
{
    vector<int> v1={1,4,3,-1,-2};
    vector<int> v2={2,3,5};
    binominalHeap<int> bh1(v1);
    cout<<bh1.top()<<endl;
    binominalHeap<int> bh2(v2);
    cout<<bh2.top()<<endl;
    bh1.meldWith(bh2);
    cout<<bh1.top()<<endl;
    cout<<"=============="<<endl;




    return 0;
}
