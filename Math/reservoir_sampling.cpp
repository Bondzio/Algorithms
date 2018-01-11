#include <iostream>
#include <list>
#include <vector>
#include <random>
#include <ctime>
using namespace std;

template <typename T>
class reservoir{
private:
    size_t n_sample;//sample n elements
    unsigned long long current;//record num of elements have been seen
    vector<T> samples;
    default_random_engine generator;

public:
    reservoir(size_t n_):n_sample(n_),current(1ULL){
        generator.seed(time(0));
    }
    virtual ~reservoir()=default;
    vector<T> & getSamples(){
        return samples;
    }
    void sampling(list<T> stream){
        for(auto iter=stream.cbegin();iter!=stream.cend();iter++){
            if(current<=n_sample){
                //100 % hold
                samples.push_back(*iter);
            }else{
                //decide whether to hold at a chance of n_sample/current
                uniform_int_distribution<unsigned long long> reserve(1,current);
                unsigned long long number = reserve(generator);
                if(number<=n_sample){
                    uniform_int_distribution<size_t> replace(0,n_sample-1);
                    samples[replace(generator)]=*iter;
                }
            }
            ++current;
        }
    }

};

int main()
{
    //一个list
    list<int> list{1,2,3,4,5,6,7,8,9,10};
    //创建一个只采集一个元素的reservoir
    reservoir<int> sample_list(1);
    
    sample_list.sampling(list);
    auto result=sample_list.getSamples();
    for(auto iter=result.cbegin();iter!=result.cend();iter++){
        cout<<*iter<<" ";
    }
    cout<<endl;
}
