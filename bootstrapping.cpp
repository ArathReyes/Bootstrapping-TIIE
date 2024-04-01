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
        Curve(vector<float> ts, vector<float> dfs){
            tenors = ts;
            discount_factors = dfs;
        }

        float get_discount_factor(float t){
            vector<float> rates;
            float r, df;
            int n = tenors.size();
            for(int i = 0; i<=n-1; i++){rates.push_back(-log(discount_factors[i]));}
            if (t >= tenors[n-1]){
                r =  rates[n-1] / tenors[n-1];
            }else if (t <= tenors[0]){
                r = rates[0] / tenors[0];
            }else{
                r = linear_interpolation(tenors, rates, t) / t;
            }
            df = exp( - r * t);
            return df;
        };
        
        float get_forward_rate(float t1, float t2){
            float tau = (t2 - t1);
            float df1 = get_discount_factor(t1);
            float df2 = get_discount_factor(t2);
            return (df1 / df2 - 1.) / tau;
        };
        
        void insert_tenor(float t, float df){
            tenors.push_back(t);
            discount_factors.push_back(df);
        };
        
        void replace_last_tenor(float df){
            discount_factors[discount_factors.size()-1] = df;
        };
};

class TIIE_Swap{
    public:
        Curve curve;
        int nCoupons;
        float notional, strike;
        
        TIIE_Swap(auto crv, int ncps, float N, float K){
            curve = crv;
            nCoupons = ncps;
            notional = N;
            strike = K;
        }
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
    
    vector<float> dfCetes{0.9937394415184339,0.9798157946306095,0.9570751782552519,0.9113278046113188};
    vector<float> tCetes{1. / 12., 3. / 12., 6./12., 1.};
    
    Curve crv(tCetes, dfCetes);
    cout << crv.get_discount_factor(0.75) << endl;
    
    
    float root = newton_ramphson_method(f,1.85, 0.001, 25);
    cout << root << endl;
    cout << f(root);

    return 0;
}
