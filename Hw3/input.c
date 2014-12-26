// print hello world
#pragma source off
#pragma token off
#pragma statistic off
#pragma symbol on





float func(float a){
    return 1.0;
}
float func2(float a){
    return 2e3; // errpr
}

int main(){

    float a = 1.0;
    double b = 1.0;
    float c = a;
    float d = b; // error
    const float e = 2e3; // error
    const double f = 2e3;
    func(1.0);
    func(2e3); // error

}
