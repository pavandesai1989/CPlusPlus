// Online C++ compiler to run C++ program online
#include <iostream>
using namespace std;

template<typename T>
class myvector
{
   private:
     T* _pdata;
     int _size;
     int _capacity;

    public:
        myvector();
        ~myvector();

        void push_back(const T& data); 
        void pop_back();
        T& at(size_t idx);
        T& operator[](size_t idx);
        size_t size()const;
        size_t capacity()const;
        bool empty()const;
        void print(); 
        void clear();
};


template<typename T>
bool myvector<T>::empty()const
{
    return _size==0;
}

template<typename T>
void myvector<T>::clear()
{
    _size = 0;
}

template<typename T>
size_t myvector<T>::size()const
{
    return _size;
}

template<typename T>
size_t myvector<T>::capacity()const
{
    return _capacity;
}

template<typename T>
T& myvector<T>::at(size_t idx)
{
    if(idx>=_size)
    {
        throw std::out_of_range("Index out of range");
    }
    return _pdata[idx];
}

template<typename T>
void myvector<T>:: pop_back()
{
  if(_size==0)
  {
    throw std::out_of_range("Vector is empty");
  }
  _size--;
}

template<typename T>
T& myvector<T>::operator[](size_t idx)
{
    return _pdata[idx];
}


template<typename T>
void myvector<T>::print()
{
  for(int i=0;i<_size;i++)
  {
    cout<<_pdata[i]<<endl;
  }
}

template<typename T>
myvector<T>::myvector():_pdata(new int),_capacity(1),_size(0)
{
}

template<typename T>
myvector<T>::~myvector()
{
    delete[]_pdata;
}

template<typename T>
void myvector<T>::push_back(const T& data)
{
    if(_size == _capacity)
    {
        cout<<"doubling size"<<endl;
        T* _temp = new T [2*_capacity];
        for(int i=0;i<_size;i++)
        {
            _temp[i] = _pdata[i];
        }
        delete[]_pdata;
        _capacity = 2*_capacity;
        _pdata = _temp;
    }
    _pdata[_size++] = data;
}

int main() {
    // Write C++ code here
    myvector<int> ob;
    ob.push_back(1);
    ob.push_back(2);
    ob.push_back(3);
    ob.push_back(4);
    ob.push_back(5);

    cout<<ob.at(0)<<endl;;
    cout<<"size is "<<ob.size()<<endl;
    cout<<"capacity is "<<ob.capacity()<<endl;
    ob.pop_back();
    ob.print();

    return 0;
}
