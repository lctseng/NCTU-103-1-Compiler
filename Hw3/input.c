// print hello world
#pragma source off
#pragma token off
#pragma statistic off
#pragma symbol on



/*

bool func(double a,float b,string c[4],int p3[4][4]);

int main(){
    int intArr[4][4];

    int arrErr1[2] = {10.0+2}; // error
    string strA = "compiler";
    string strB = "test";
    string strArr[5];
    string strArr2[4];
    bool res = func(10.0,0,strArr,intArr); // error
    bool res = func(10,0,strArr2,intArr);

}
*/
int g,a,b,c;
int gr[1];
const int clist1 = 123,clist2=-456.4,clist3=0; //error
const int err1 = -3.14156; //error
const int err2 = 3.14156; //error
const int err3 = true; //error
const int err4 = -true; //error
const bool bool1 = -true; //error
const bool bool2 = true;
const bool bool3 = false;
const bool bool1 = -true; //error
const double double1 = -4.646;
const float float1 = 10;
const string str1 = "Hello world!";
const string str2 = -"Hello world!"; //error
const string str3 = -10; // error
string e04[10][9];
int clist1; //error
bool func(int a,float b,string c[4],int p3[4]);
int func2(int a);
bool func(int a,float b,string c[4][3],int p3[4]); //error
float func3(){} //error
void test(){
    int i;
    for(;;){
        int ii;
    }
}
int func2(int a){
    a = 10;
    return a;
}

int main(int a,float b,bool c[10][0],bool a){ //error
    double double1 = 10;
    bool b = "str"; //error
    bool boolErr = !"str"; //error
    bool boolErr2 = "str" && false; //error
    bool boolErr3 = -(100.0 == 10 && 10.6 < 1.245); //error
    bool bool1 = (100.0 != 10 && 10.6 < 1.245);
    float c = 10 % b; // error
    bool mod = 10 % 20; //error
    int int1 = 10 + 5 * (8 - 4.0); //error
    int a[10][9]; // error
    int a = func();  //error
    int int2;
    int intArr[4];

    int arrErr1[2] = {10.0+2}; // error
    string strA = "compiler";
    string strB = "test";
    string strArr[5];
    string strArr2[4];
    bool res = func(10,0,strArr,intArr); // error
    bool res = func(10,0,strArr2,intArr);
    const int cInt = 10;
    int intArrErr[10][0]; //error
    bool2 = false;
    b = 10; 
    g = true; // error
    ccc = 10; // error
    c = a[c][-1] + 3;  //error
    c = a[-1]["true"]; // error
    func(); // error 
    func3();
    func2();
    func4(); // error
    strB = strA;
    strB = strB + strA; // error
    bool1 = a % 5; // error
    a = 10;
    a[4][10] = b; // error
    bool1 = a; // error
    str1 = "ho"; // error
    cInt = 3.0;  // error  

} // error
int funcMismatch(float param);
int funcMismatch(int param){ //error
    const double db = 10.5;
    const float fb = 10.5;
    void a; // error
    int a[2][2][2] = {};
    int b;
    funcMismatch(10.4);
    funcMismatch(db); // error
    funcMismatch(fb);
    return a[1][2+b][b-3];
    param = 1; //error
} // error

void foo(){
    int a[3] = {1,2,3}; // legal
    int b[4] = {1,2,3,4,5}; // illegal
    int c[2][3] = {1,2,3,4,5,6,7}; // illegal
    int d[5] = {1}; // legal
    int e = "This is a string"; // illegal
}

int foobar(int a[3][3], int b[5][3], int i) {
    int k = a[3][i]; // legal : note that bounds are not checked
    a[0][0] = i; // legal
    i = a[1][1]; // legal
    a[0][1] = b[1][2]; // legal
    i = 3+a[0]; // illegal : array arithmetic
    a = b; // illegal : array assignment
    a[1] = b[2]; // illegal : array assignment
    return a[0][0]; // legal : 'a[0][0]' is a scalar type, but 'a' is an array type.
}

int WTF123456789012345678901234567890(int a,bool b,int array[10][9][8]){
    bool e04;
    int i;
    while(true){
        if(e04){
            continue;
        }
        break;
        WTF123456789012345678901234567890(a,b,array); 
        return WTF123456789012345678901234567890(a,b,array); 
    }
    do{
        continue;
        if(false){
            break;
        }
    }while(e04);
    if(e04){
        break; // error
    }
    read clist1; // error
    read array[1][2][a];
    read array[1][2]; // error
    print a+5;
    print array[1][2]; //error
    print array[1][2][3];
    for(a=6;a+1;i=i+1){ // error
        a = 10;
    }
    
    return array[10][1][a]; 
}


void functest();

void functest(){
    int a = 10;
}
