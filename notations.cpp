#include <iostream>
#include <vector>
#include "utils.h"
#include "stack.h"

class Expression{
public:
    
    Expression(){};
    
    Expression(const char *str) : expr(str){
      Utils::expr_to_vector(str, this->infix);
    }
    
    void set_infix(char *str);
    void set_sufix(std::vector<char *> &v);
    void set_postfix(std::vector<char *> &v);
   
    std::vector<char *> get_infix();
    std::vector<char *> get_sufix();
    std::vector<char *> get_postfix();
    
    void show_infix();
    void show_sufix();
    void show_postfix();

    void free();

  private:
    const char *expr;
    std::vector<char *> infix;
    std::vector<char *> sufix;
    std::vector<char *> postfix;

};


class Conversions : public Utils{
  
  public:
    
    using Utils::is_operator;
    using Utils::compare_operator;
    using Utils::is_number;

    std::vector<char *> infix_to_sufix(std::vector<char *> &v);
    std::vector<char *> infix_to_postfix(std::vector<char *> &v);
  
  private:
    

    short int precedence(char *op);
    bool is_right_associative(char *op);
    void invert_expr(std::vector<char *> &expr, std::vector<char *> &inv_expr);
    
    std::vector<char *> shunting_yard(std::vector<char *> &v);
    
    std::vector<char *> invexpr;
  

};


class Evaluate : public Utils{
  public:
    
    using Utils::compare_operator;
    //using Utils::is_number;
    
    int evaluate_sufix(std::vector<char *> &sufix);
    int evaluate_postfix(std::vector<char *> &postfix);

  private:

    int str_len(char *num);
    int str_to_num(char *num);
    int operation(char *operation, int a, int b);
};


//Expression functions

void Expression::set_infix(char *str){
  Utils::expr_to_vector(str, this->infix);
}

void Expression::set_sufix(std::vector<char *> &v){
  this->sufix = v;
}

void Expression::set_postfix(std::vector<char *> &v){
  this->postfix = v;
}

std::vector<char *> Expression::get_infix(){
  
  return this->infix;
}

std::vector<char *> Expression::get_postfix(){

  return this->postfix;
}

std::vector<char *> Expression::get_sufix(){

  return this->sufix;
}

void Expression::show_infix(){
  Utils::show_expression(this->infix);
}

void Expression::show_sufix(){
  Utils::show_expression(this->sufix);
  
}

void Expression::show_postfix(){
  Utils::show_expression(this->postfix);
}

void Expression::free(){
  
  int i;
  
  for (i = 0; i < this->infix.size(); ++i) {
    delete[] this->infix[i];
  }
}

// Conversions functions
short int Conversions::precedence(char *op){
  
  char c;

  c = op[0];

  if(c == '+' || c == '-'){
    return 1;
  }else if (c == '*' || c == '/') {
    return 2;
  }else if (c == '^') {
    return 3;
  }else {
    return 0;
  }
}

bool Conversions::is_right_associative(char *op){

  char c;

  c = op[0];
  
  if(c == '^') return 1;
  
  return 0;
}


void Conversions::invert_expr(std::vector<char *> &expr, std::vector<char *> &inv_expr){

  int vlen,i;

  vlen = expr.size();

  for (i = 0; i < vlen; i++){
    if(compare_operator(expr.at(vlen - 1 -i), "(")){
      inv_expr.push_back(")");
    }else if (compare_operator(expr.at(vlen - 1 -i), ")")){
      inv_expr.push_back("(");
    }else {
      inv_expr.push_back(expr.at(vlen - 1 - i));
    }
  }
}

std::vector<char *> Conversions::shunting_yard(std::vector<char *> &v){

  char *op;
  short int prc_in, prc_top;
  int cond, i;
  Stack<char *> stk(10);
  std::vector<char *> conv;

  for (i = 0; i < v.size(); ++i) {
    
    op = v.at(i);

    if(is_number(op)){
      conv.push_back(v.at(i));
      continue;
    } 

    if(compare_operator(op, "(")){
      stk.push(op);
      continue;
    }
    
    if(compare_operator(op, ")")){
      while(!stk.empty() && !compare_operator(stk.peek(), "(")){
        conv.push_back(stk.pop());
      }
      stk.pop();
      continue;
    }
    
    if(is_operator(op)){
      if(compare_operator(op, "^")){
        while(!stk.empty() && !compare_operator(stk.peek(), "(") && precedence(op) < precedence(stk.peek())){
          conv.push_back(stk.pop());
          ///Nunca se entra aqui
        }
      }else{
        while(!stk.empty() && !compare_operator(stk.peek(), "(") && precedence(op) <= precedence(stk.peek())){
          conv.push_back(stk.pop());
        }
      }
      
      stk.push(op);
      continue;
    }
  }

  while(!stk.empty()){
    conv.push_back(stk.pop());
  }

  stk.free_stack();

  return conv;
}


std::vector<char *> Conversions::infix_to_postfix(std::vector<char *> &v){
  
  std::vector<char *> postfix;

  postfix = shunting_yard(v);

  return postfix;
}

std::vector<char *> Conversions::infix_to_sufix(std::vector<char *> &v){
  
  std::vector<char *> postfix, invexpr, sufix;

  invert_expr(v, invexpr);

  postfix = shunting_yard(invexpr);

  invert_expr(postfix, sufix);
  
  return sufix;
}


//Evaluate functions

int Evaluate::str_len(char *str){
  
  int len;
  
  len = 0;
  while(str[len] != '\0') len++;
  
  return len;
}

int Evaluate::str_to_num(char *str){
  
  int num, base, temp_num, num_len, i;

  num_len = str_len(str);
  base = 1;
  num = 0;
  for (i = num_len - 1; i >= 0; i--){
    temp_num = (str[i] - 48);
    num += (base * temp_num);
    base *= 10;
  }

  return num;
}

int Evaluate::operation(char *op, int a, int b){
  
  if(compare_operator(op, "+")){
    return a + b;
  }else if (compare_operator(op, "-")){
    return a - b;
  }else if (compare_operator(op, "*")){
    return a * b;
  }else if (compare_operator(op, "/")){
    return a / b;
  }else if (compare_operator(op, "^")){
    return Utils::exp(a , b);
  }
  
  std::cout << "Operacion desconocida" << '\n';
  return 0;
}

int Evaluate::evaluate_postfix(std::vector<char *> &postfix){
  
  int a, b, res, num, n_items, i;
  char *out;
  Stack<int> stk(10);

  n_items = postfix.size();

  for (i = 0; i < n_items; ++i){
    stk.show_stack();

    out = postfix.at(i);

    if(is_number(out)){
      num = str_to_num(out);
      stk.push(num);
      continue;
    }

    if(is_operator(out)){
      b = stk.pop();
      a = stk.pop();
      res = operation(out, a, b);
      stk.push(res);
    }
  }

  res = stk.pop();
  
  stk.free_stack();

  return res;
}

int Evaluate::evaluate_sufix(std::vector<char *> &sufix){
  int a, b, res, num, n_items, i;
  char *out;
  Stack<int> stk(10);

  n_items = sufix.size();

  for (i = n_items - 1; i >= 0; i--){

    stk.show_stack();

    out = sufix.at(i);

    if(is_number(out)){
      num = str_to_num(out);
      stk.push(num);
      continue;
    }

    if(is_operator(out)){
      a = stk.pop();
      b = stk.pop();
      res = operation(out, a, b);
      stk.push(res);
    }    
  }

  res = stk.pop();

  stk.free_stack();
  
  return res;
}

void main_program(int argc, const char *str){

  int res, opt, in, sol, i;
  char expr[100];

  Conversions conversions;
  Evaluate evaluations;

  res = 1;
  
  in = 1;
  while(in){
    
    if(res){

      if(argc == 1){
        std::cout << "Ingrese una expresión " << '\n';
        std::cin >> expr;
      }else if(argc == 2){
        i = 0;
        while(str[i] != '\0'){
          expr[i] = str[i];
          i++;
        }
        expr[i] = '\0'; 
      }else if(argc > 2){
        std::cout << "Demasiados argumentos" << '\n';
        return;
      }
      
      if(!Utils::validate_parenth(expr)){
        std::cout << "Los paréntesis no coinciden"<< '\n';
        continue;
      }
      res = 0;
    }
   
    Expression e(expr);
    std::vector<char *> infix;
    std::vector<char *> postfix;
    std::vector<char *> prefix;
    
    infix = e.get_infix();
    postfix = conversions.infix_to_postfix(infix);
    prefix = conversions.infix_to_sufix(infix);
    
    e.set_sufix(prefix);
    e.set_postfix(postfix); 
    
    if(!res){
      while(1){
        std::cout << "1) Mostrar expresion infija" << '\n';
        std::cout << "2) Mostrar expresion prefija" << '\n';
        std::cout << "3) Mostrar expresion postfija" << '\n';
        std::cout << "4) Calcular expresion prefija" << '\n';
        std::cout << "5) Calcular expresion postfija" << '\n';
        std::cout << "6) Ingresar una nueva expresión" << '\n';
        std::cout << "7) Salir" << '\n';
        std::cin >> opt;
       
        std::cout << '\n';
        if(opt == 1){
          e.show_infix();
        }else if (opt == 2){
          e.show_sufix();
        }else if (opt == 3){
          e.show_postfix();
        }else if (opt == 4){
          sol = evaluations.evaluate_sufix(prefix);
          std::cout << '\n' << "Resultado:" << sol <<'\n';
        }else if (opt == 5) {
          sol = evaluations.evaluate_postfix(postfix);
          std::cout << '\n' << "Resultado:" << sol<<'\n';
        }else if (opt == 6){
          e.free();
          res = 1;
          break;
        }else if (opt == 7){
          e.free();
          in = 0;
          break;
        }
        std::cout << '\n';
      }  
    }
  }
}

int main(int argc, char *argv[]){

  main_program(argc, argv[1]);

  return 0;
}
