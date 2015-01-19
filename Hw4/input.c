// print hello world
#pragma source off
#pragma token off
#pragma statistic off
#pragma symbol on

int gga = 10;
double ggd = 5.58;

void printDouble(double d,int l,double c,float r){
    print d;
    print "\n";
    print l;
    print "\n";
    print c;
    print "\n";
    print r;
    print "\n";
    print gga;
    print "\n";
}

int main(){
    double a;
    int b;
    a = 1.0001;
    b = 10;
    print ggd;
    print "\n";
    printDouble(b,b,b,b);
    return 0;
}


