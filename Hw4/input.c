// print hello world
#pragma source off
#pragma token off
#pragma statistic off
#pragma symbol on


int a, b;
bool d;

bool bar(){
    print "BAR!!\n";
    return true && false || 5 > 10;
}

int foo( int a , int ab ) {
    int i, result;
    result = 0;
    i = 1;
    while( i <= a ) {
        result = result + i;
        i = i + 1;
    }
    return result + ab ;
}

int main() {
    int c = 10 ;
    read a;
    
    c = foo( a , 999);
    print c;
    print foo(100,1);
    print "\n";
    if( c >= 100 ) {
        print "c >= 100 \n";
    }
    else {
        print "c < 100 \n";
    }
    return 0;

}



