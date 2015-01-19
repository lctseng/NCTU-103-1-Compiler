// print hello world
#pragma source off
#pragma token off
#pragma statistic off
#pragma symbol on

void printDouble(double d,int l,double c,float r){
    print d;
    print "\n";
    print l;
    print "\n";
    print c;
    print "\n";
    print r;
    print "\n";
}

int main(){
    double a;
    int b;
    a = 1.0001;
    b = 10;
    printDouble(b,b,b,b);
    return 0;
}


