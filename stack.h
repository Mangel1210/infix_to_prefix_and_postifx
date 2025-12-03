#ifndef STACK_H
#define STACK_H

#include <iostream>

template <typename T> 
class Stack{
  public:
    
    Stack(){
      this->max_size = 25;
      this->top = -1;
      this->items = new T[25];
    }
    
    Stack(int max_size){
      this->max_size = max_size;
      this->top = -1;
      this->items = new T[max_size];
    }
    
    bool empty();
    bool full();
    void push(T item);
    T pop();
    T peek();
    void show_stack();
    void free_stack();  

  private:
    int top, max_size;
    T *items;
    void resize();
};


template<typename T>
bool Stack<T>::empty(){
  
  if (this->top == -1) return 1;
  
  return 0;
}

template<typename T>
bool Stack<T>::full(){

  if(this->top == this->max_size - 1) return 1;

  return 0;
}

template <typename T>
void Stack<T>::push(T item){
  
  if(this->full()){
    std::cout << "La pila esta llena" << '\n';
    return;
  }

  this->top++;
  this->items[this->top] = item;
}

template <typename T>
T Stack<T>::pop(){

  T elem;

  if(this->empty()){
    std::cout << "No hay elementos en la pila" << '\n';
    return NULL;
  }

  elem = this->items[this->top];
  this->top--;

  return elem;
}

template<typename T>
T Stack<T>::peek(){
  
  if(this->empty()){
    std::cout << "No hay elementos en la pila" << '\n';
    return NULL;
  }

  return this->items[this->top];
}

template<typename T>
void Stack<T>::show_stack(){
  
  int i;

  if(this->empty()){
    std::cout << "La pila esta vacía" << '\n'; 
    return;
  }

  std::cout << "[";
  for(i = 0; i < this->top+1; ++i){
    std::cout << this->items[i];
    if(i < this->top){
      std::cout << ",";
    }
  }
  std::cout << "]" << '\n';
}

template<typename T>
void Stack<T>::free_stack(){

  delete[] this->items;

}

#endif
