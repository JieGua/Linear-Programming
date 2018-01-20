# Linear-Programming
## Introduction
This calculator is designed to solve [linear-programming](https://en.wikipedia.org/wiki/Linear_programming) problems which maximize
or minimize the objective functions given a set of inequality constraints.

## Setup
1. If you are using Windows, you can either use the exe file provided or download the source code to compile it yourself.
2. If you are using Mac, please download the source code and compile it yourself.

## Usage
Please strictly follow the instructions displayed in the program.  
Here are some basic steps:
1. Enter the type of problem (maximization or minimization)
2. Enter the number of variables
3. Enter the number of inequalities
4. Enter all the coefficients of all inequalities with inequality symbols in the order from x1 to xn  
> Example:  
Here is the set of inequalities with two variables you want to enter to the program:  
> ```
> x1+3x2<=10  
> x2<=2  
> x1-2x2>=3  
> ```
> You should input:  
> ```
> 1 3 <= 10  
> 0 1 <= 2  
> 1 -2 >= 3  
> ```
> Note: For the second inequality, x1 is implicitly  written as 0x1, so you should enter the coefficient of 0 for x1 in the second inequality.  
5. Enter all the coefficients of the objective function in the order from x1 to xn
> Example:  
Here is an objective function with two variables you want to enter to the program:  
> ```
> z=2x1+3x2  
> ```
> You should input:  
> ```
> 2 3  
> ```
6. The program will output the value for the objective (maximum or minimum) as well as the values for the variables to achieve the result.


