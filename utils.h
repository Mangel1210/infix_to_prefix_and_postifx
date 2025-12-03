#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <vector>
#include "stack.h"

class Utils{
  public:
    static bool valid_char(char c);
    static bool is_digit(char c);
    static bool is_bracket(char c);
    static char *chr_to_str(char d);
    static bool is_operator(char *op);
    static bool is_number(char *op);
    static bool compare_operator(char *op1, const char *op2);
    static bool is_operator_char(char op);
    static bool validate_parenth(char *expr);
    static void expr_to_vector(const char *expr, std::vector<char *> &vec);
    static void show_expression(std::vector<char *> &vec);
    
    static int exp(int base, int expo);
};

bool Utils::valid_char(char c){
  
  if('0' <= c && c <= '9') return 1;
  if(c == '+' || c == '-' || c == '*' || c== '/' || c == '^') return 1;
  if(c == '(' || c == ')') return 1;

  return 0;
}

bool Utils::is_digit(char c){
  
  if('0' <= c && c <= '9') return 1;
  
  return 0;
}


bool Utils::is_bracket(char c){
  
if(c == '(' || c == ')') return 1;

  return 0;
}

char* Utils::chr_to_str(char d){
  char *str;

  str = new char[2];

  str[0] = d;
  str[1] = '\0';

  return str;
}

bool Utils::is_operator(char *op){
  char c;

  c = op[0];
  
  if(c == '+' || c == '-' || c == '*' || c== '/' || c == '^') return 1;

  return 0;
}

bool Utils::compare_operator(char *op1, const char *op2){
  
  return (op1[0] == op2[0]);
}

bool Utils::is_operator_char(char c){
  
  if(c == '+' || c == '-' || c == '*' || c== '/' || c == '^') return 1;
  
  return 0;
}

bool Utils::validate_parenth(char *expr){
  
  bool res;
  char c;
  int i;
  Stack<char> stk(10);
  
  i = 0;
  while((c = expr[i]) != '\0'){
    
    if(expr[i] == '('){
      stk.push(c);
    }else if (expr[i] == ')'){
      
      if(stk.empty()) return 0;
      
      if(stk.peek() == '('){
        
        stk.pop();
        
      }else {
        
        return 0;
      }
    }
    i++;
  }

  res = stk.empty();

  if(res){
    return 1;
  }

  return 0;
}

void Utils::expr_to_vector(const char *expr, std::vector<char *> &vec){
  
  int i,j,k;
  char c;
  char *ob;

  i = 0;
  while((c = expr[i]) != '\0'){
    if(valid_char(c)){
      if(is_operator_char(c) || is_bracket(c)){
        ob = chr_to_str(c);
        vec.push_back(ob);
      }else if(is_digit(c)){
        int n_digs;
        char *num;
        
        j = i;
        n_digs = 0;
        while(is_digit(expr[j])){
          n_digs++;
          j++;
        }
        
        num = new char[n_digs+1];
        
        for(k = 0; k < n_digs; k++){
          num[k] = expr[i+k]; 
        }
        num[k] = '\0';

        vec.push_back(num);
        
        i = j;
        continue;
      }
    }
    i++;
  }
}

bool Utils::is_number(char *num){
  char c;

  c = num[0];
  if('0' <= c && c <= '9'){
    return 1;
  }

  return 0;
}

void Utils::show_expression(std::vector<char *> &vec){
  
  int i;

  for (i = 0; i < vec.size(); ++i) {
    std::cout << vec.at(i);
  }
  std::cout << '\n';
}

int Utils::exp(int base, int expo){
  int res;

  res = 1;
  while(expo > 0){
    if(expo & 1) res *= base;
    base *= base;
    expo = expo >> 1;
  }

  return res;
}

#endif
