#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

float linear_interpolation(vector<float> X, vector<float> Y, float x){
    float y = 0.0;
    if(x <= X[0])
        y = Y[0];
    if(x>=X[X.size() - 1])
        y = Y[Y.size() - 1];
    
    int n = X.size();
    
    for(int i=0; i<= n-2; i++){
        if(x>=X[i] && x<= X[i+1]){
            float x0 = X[i];
            float x1 = X[i+1];
            float y0 = Y[i];
            float y1 = Y[i+1];
            y = y0 + (x-x0)*((y1 - y0)/(x1 - x0));
            break;
        }
    }
    return y;
}

float newton_ramphson_method(float (*f)(float), float x0, float tol, int nmax){
    
    auto der = [&f](float x) -> float
    {
        return (f(x+0.001)-f(x))/0.001;
    };
    
    float xn_1 = x0;
    float xn = 0.0;
    for(int i = 0; i <= nmax - 1; i++){
        xn = xn_1 -(f(xn_1)/der(xn_1));
        if(abs(xn-xn_1)<tol)
            break;
        else
            xn_1 = xn;
    }
    return xn;
}

class Curve{
    public:
        vector<float> tenors;
        vector<float> discount_factors;
};


int main() {
    auto f = [](float x) -> float
    {
        return  pow(x,3) + 2*x - 3;
    };
    //vector <float> X{1.0,2.0,3.0,4.0,5.0,7.0};
    //vector <float> Y{1.0,2.0,3.0,4.0,5.0,7.0};
    //float x = 6.0;
    //float y = f(5);
    vector<float> tenors{1, 3, 6, 9, 13, 26, 39, 52, 65, 130, 156, 195, 260, 390};
    vector<float> par_swaps{8.0375, 8.31, 8.96, 9.37, 9.56, 9.61, 9.65, 9.70, 9.75, 9.8, 9.85, 9.90, 9.94, 9.98};
    
    float root = newton_ramphson_method(f,1.85, 0.001, 25);
    cout << root << endl;
    cout << f(root);

    return 0;
}
