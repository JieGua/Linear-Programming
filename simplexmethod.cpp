#include <iostream>
#include <limits>
#include <string>
using namespace std;

#define NaN std::numeric_limits<float>::quiet_NaN()
#define isNaN(X) (X != X) // NaN is the only float that is not equal to itself

//class declaration of Row: to manage data and row operation in matrix
class ROW {
public:
	ROW();
	ROW (int size);
	ROW (int size, double *arr);
	~ROW ();
	ROW& operator = (const ROW& x);
	ROW operator + (const ROW& x);
	ROW operator - (const ROW& x);
	ROW operator * (const double scalar);
	ROW operator / (const double scalar);
	void print();

	friend bool FindIndicatorRow (ROW** matrix, int& IndicatorRow, const int numofrow, const int numofcolumn);
	friend int FindIndicator (ROW** matrix, const int IndicatorRow, const int numofrow, const int numofcolumn);
	friend int NonstandardToStandard (ROW** matrix, const int numofrow, const int numofcolumn);
	friend bool PosAndFindPivot (ROW** matrix, int& pivot, const int numofcolumn, const int numofrow);
	friend double* GetRatioInfo (ROW** matrix, const int pivot, const int numofrow, const int numofcolumn);
	friend int findpivotrow (ROW** matrix, const int pivot, const int numofrow, const int numofcolumn);
	friend void rowoperation (ROW** matrix, const int pivot, const int pivotrow, const int numofrow);
	friend double* SimplexMethod (ROW** matrix, const int nonbasic, const int numofrow, const int numofcolumn);
private:
	double* row;
	int rowsize;
};

ROW::ROW() {
	//VOID
}

ROW::ROW(int size) {
	row = new double[size];
	for (int i = 0; i < size; i++)
		row[i] = 0;
	rowsize = size;
}

ROW::ROW(int size, double *arr) {
	row = new double[size];
	rowsize = size;
	for (int i = 0; i < rowsize; i++) {
		row[i] = arr[i];
	}
}

ROW::~ROW() {
	rowsize = -1;
	delete[] row;
}

ROW& ROW::operator = (const ROW& x) {
	for (int i = 0; i < rowsize; i++) {
		this->row[i] = x.row[i];
	}
	return *this;
}

ROW ROW::operator + (const ROW& x) {
	int size = x.rowsize;
	double *copy = new double[size];
	for (int i = 0; i < rowsize; i++ ) {
		copy[i] = this->row[i] + x.row[i];
	}
	ROW result(size, copy);
	delete[] copy;
	return result;
}

ROW ROW::operator - (const ROW& x) {
	int size = x.rowsize;
	double *copy = new double[size];
	for (int i = 0; i < rowsize; i++) {
		copy[i] = this->row[i] - x.row[i];
	}
	ROW result(size, copy);
	delete[] copy;
	return result;
}


ROW ROW::operator * (const double scalar) {
	int size = this->rowsize;
	double *copy = new double[size];
	for (int i = 0; i < rowsize; i++) {
		copy[i] = this->row[i] * scalar ;
	}
	ROW result(size, copy);
	delete[] copy;
	return result;
}

ROW ROW::operator / (const double scalar) {
	int size = this->rowsize;
	double *copy = new double[size];
	for (int i = 0; i < rowsize; i++) {
		copy[i] = this->row[i] / scalar;
	}
	ROW result(size, copy);
	delete[] copy;
	return result;
}

void ROW::print() {
	for (int i = 0; i < this->rowsize; i++)
		cout << this->row[i] << ' ';
	cout << endl;
}

//functions for simplex method

bool FindIndicatorRow (ROW** matrix, int& IndicatorRow, const int numofrow, const int numofcolumn){
  IndicatorRow=0;
  for (int i=0; i<numofrow-1; i++){
    if (matrix[i]->row[numofcolumn-1]<matrix[i]->row[IndicatorRow])
      IndicatorRow=i;
  }
  if (matrix[IndicatorRow]->row[numofcolumn-1]>=0)
    return false;//indicate that the tableau is already standard
  else
    return true;
}

int FindIndicator (ROW** matrix, const int IndicatorRow, const int numofrow, const int numofcolumn){
  double mostnegative=0;
  for (int j=0; j<numofcolumn-1; j++){
    if (matrix[IndicatorRow]->row[j]<mostnegative)
      mostnegative=matrix[IndicatorRow]->row[j];
  }
  if (mostnegative==0)
    return -1;//unsolvable
  for (int j=0; j<numofcolumn-1; j++){
    if (matrix[IndicatorRow]->row[j]==mostnegative)
      return j;
  }
}

int NonstandardToStandard (ROW** matrix, const int numofrow, const int numofcolumn){
  int IndicatorRow;
  while (FindIndicatorRow (matrix, IndicatorRow, numofrow, numofcolumn)){
    int pivot=FindIndicator (matrix, IndicatorRow, numofrow, numofcolumn);
    if (pivot==-1)
      return -1;//unsolvable
    int pivotrow=findpivotrow(matrix, pivot, numofrow, numofcolumn);
    if (pivotrow==-1)
      return -1;//unsolvable
    rowoperation(matrix, pivot, pivotrow, numofrow);
  }
  return 0;
}

bool PosAndFindPivot (ROW** matrix, int& pivot, const int numofcolumn, const int numofrow){
	pivot=0;
  for (int i=0; i<numofcolumn-1; i++){
    if (matrix[numofrow-1]->row[i]<matrix[numofrow-1]->row[pivot]){
      pivot=i;
    }
  }
	if (matrix[numofrow-1]->row[pivot]<0)
	  return false;
  return true;
}//good

double* GetRatioInfo (ROW** matrix, const int pivot, const int numofrow, const int numofcolumn){
  double* info = new double [numofrow-1];
  for (int i=0; i<numofrow-1; i++){
    if (matrix[i]->row[pivot]==0)
      info[i]=NaN;
    else{
      info[i]=matrix[i]->row[numofcolumn-1]/matrix[i]->row[pivot];
      if (info[i]<0)
        info[i]=NaN;
      else if (info[i]==0){
        if (matrix[i]->row[pivot]<0)
          info[i]=NaN;
      }
    }
  }
  return info;
}//good

int findpivotrow (ROW** matrix, const int pivot, const int numofrow, const int numofcolumn){
  double* info=GetRatioInfo(matrix, pivot, numofrow, numofcolumn);
  int index=-1;
	int i=0;
	while (i<numofrow-1&&isNaN(info[i])){
		i++;
	}
	if (i>=numofrow-1)
	  return index;
	double minratio=info[i];
	index=i;
  for (int j=i+1; j<numofrow-1; j++){
    if (isNaN(info[j])){
      //do nothing
    }
    else {
      if (info[j]<minratio){
        index=j;
        minratio=info[j];
      }
    }
  }
  delete [] info;
  return index;
}//good

void rowoperation (ROW** matrix, const int pivot, const int pivotrow, const int numofrow){
	*(matrix[pivotrow])=*(matrix[pivotrow])/matrix[pivotrow]->row[pivot];
  for (int i=0; i<numofrow; i++){
    if (i!=pivotrow){
      double coeff = matrix[i]->row[pivot];
      *(matrix[i])=*(matrix[i])-*(matrix[pivotrow])*coeff;
    }
  }
}//good

double* SimplexMethod (ROW** matrix, const int nonbasic, const int numofrow, const int numofcolumn){
  int pivot=0;
  int pivotrow=0;
  while (!PosAndFindPivot(matrix, pivot, numofcolumn, numofrow)){
    pivotrow=findpivotrow(matrix, pivot, numofrow, numofcolumn);
    if (pivotrow==-1)
      return NULL;
    rowoperation(matrix, pivot, pivotrow, numofrow);
  }
  double* result= new double [1+nonbasic];
  result[nonbasic]=matrix[numofrow-1]->row[numofcolumn-1];// answer for z
  for (int i=0; i<nonbasic; i++){
    bool isbasic=true;
		int counter=0;
		for (int j=0; j<numofrow; j++){
			if (matrix[j]->row[i]!=0)
			  counter++;
		}
		if (counter!=1)
		  result[i]=0;
		else {
			for (int j=0; j<numofrow; j++){
				if (matrix[j]->row[i]!=0){
					result[i]=matrix[j]->row[numofcolumn-1]/matrix[j]->row[i];
				break;
				}
			}
		}
  }
  return result;
}//good

int main (){
	cout<<"Linear Promramming Calculater v1.0 by Shuang Zheng and Xinzhe Wang"<<endl;
	cout<<endl;
	bool done=false;
	while (!done){
		int nonbasic;
		int basic;
		string type;
		cout<<"Maximization or Minimization (Enter max or min): "<<endl;
		cin >> type;
		cout<<endl;
		cout<<"Number of Variables: "<<endl;
		cin>>nonbasic;
		cout<<endl;
		cout<<"Number of Inequalities: "<<endl;
		cin>>basic;
		cout<<endl;
		cout<<"Please Enter All the Coefficients of All Inequalities with Inequality Symbols in the Order from x1 to xn (Leave a space between each coefficient and symbol, one inequality per row):"<<endl;
		cout<<"Below is an example of how to convert the system of inequalities (on the left) to the input format of this calculator (on the right)"<<endl;
		cout<<endl;
		cout<<" ***********************************************"<<endl;
		cout<<" *                                             *"<<endl;
		cout<<" *     x1 +  x2 <= 10            1 1 <= 10     *"<<endl;
		cout<<" *     x1 + 2x2 >= 12            1 2 >= 12     *"<<endl;
		cout<<" *    2x1 +  x2 >= 12            2 1 >= 12     *"<<endl;
		cout<<" *                                             *"<<endl;
		cout<<" ***********************************************"<<endl;
		cout<<endl;
		int numofcolumn=nonbasic+basic+1;
		int numofrow=basic+1;
		string *str=new string[numofrow];
		double** matrixcopy;
		matrixcopy=new double* [numofrow];
		//initialize
		for (int i=0; i<numofrow; i++){
			matrixcopy[i]=new double [numofcolumn];
			for (int j=0; j<numofcolumn; j++){
				matrixcopy[i][j]=0;
			}
		}

		for (int i=0; i<numofrow-1; i++){
			for (int j=0; j<nonbasic; j++){
				cin>>matrixcopy[i][j];
			}
			cin>>str[i];
			cin>>matrixcopy[i][numofcolumn-1];
		}
		cout<<endl;

		cout<<"Please Enter All the Coefficients of the Objective Function in the Order from x1 to xn in One Row (Leave a space between each coefficient)"<<endl;
		cout<<"Below is a sample input for the objective function z = 2x1 + 3x2: "<<endl;
		cout<<endl;
		cout<<" ***************"<<endl;
		cout<<" *             *"<<endl;
		cout<<" *     2 3     *"<<endl;
		cout<<" *             *"<<endl;
		cout<<" ***************"<<endl;
		cout<<endl;
		for (int j=0; j<nonbasic; j++){
		cin>>matrixcopy[numofrow-1][j];
	  }

		for (int i=0; i<numofrow-1; i++){
			if (str[i] == ">="){
				for(int j=0; j<numofcolumn; j++)
				if (matrixcopy[i][j]!=0)
	    	matrixcopy[i][j] = -matrixcopy[i][j];
			}
		}
		for (int i=0; i<numofrow-1; i++){
				matrixcopy[i][nonbasic+i]=1;
		}
		if (type == "max") {
			for(int j=0; j<numofcolumn; j++)
			if (matrixcopy[numofrow-1][j]!=0){
			matrixcopy[numofrow-1][j] = -matrixcopy[numofrow-1][j];
		  }
		}

	  ROW** matrix=new ROW* [numofrow];
	  for (int i=0; i<numofrow; i++){
	    matrix[i] = new ROW (numofcolumn, matrixcopy[i]);
	  }

	  //free memory
	  for (int i=0; i<numofrow; i++){
	    delete [] matrixcopy[i];
	  }
	  delete [] matrixcopy;
	  // memory has been free;
		cout<<endl;

	  double* result;
	  int signaltype=NonstandardToStandard (matrix, numofrow, numofcolumn);
	  if (signaltype==-1){
	    cout<<"Unsolvable"<<endl;
	  }
	  else{
	    result=SimplexMethod(matrix, nonbasic, numofrow, numofcolumn);
			if (type == "min") result[nonbasic] = -result[nonbasic];
	    if (result==NULL)
	      cout<<"Cannot find the answer."<<endl;
	    else {
				cout<<"The results are: "<<endl;
	      for (int i=0; i<nonbasic; i++)
	        cout<<"x"<<i+1<<" = "<<result[i]<<endl;
	      cout<<"z = "<<result[nonbasic]<<endl;
	    }
	  }
    cout<<endl;
	  for (int i=0; i<numofrow; i++)
	    delete matrix[i];
	  delete [] matrix;
	  delete [] result;
	  delete str;

		cout<<"Continue? (enter Y to continue, or anything else to exit the program)"<<endl;
		char c;
		cin>>c;
		if (c=='Y'){
			cout<<endl;//continue
		}
		else {
			return 0;//exit
		}
	}
}
