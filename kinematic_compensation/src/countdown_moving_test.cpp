#include <iostream>
#include <chrono>
#include <stdio.h>
#include <unistd.h> 0.3 
#include <cmath> 
using namespace std;

/*Using chrono a s timer to calculate moving distance in pre-known speed scenario*/
#define REDUCTION_RATIO 0.1 //gear's ratio
#define BASE_SPEED 40 //RPM
#define DIAMETER 0.35 //diameter of wheel, meter

/*keycode, value = 0x"deci - 4"*/
#define KEYCODE_RIGHT 0x43
#define KEYCODE_LEFT 0x44
#define KEYCODE_UP 0x41 
#define KEYCODE_DOWN 0x42

class command{
public:
    command();
    void publisher(string direction);
};

command::command(){
    cout<<"/This is fake process, start ros process here/"<<endl;
}
void command::publisher(string direction){
    cout<<"Get message: "<<direction<<endl;
}

double time_to_dist(double time){
    return M_PI * DIAMETER * BASE_SPEED * time * REDUCTION_RATIO / 60;
}
double dist_to_time(double distance){
    return distance * 60 / M_PI / DIAMETER / BASE_SPEED / REDUCTION_RATIO;
}

int main(){
    ///initialize///
    command com;
    double time = 3.0;
    string direction = "fwd";
    chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
    sleep(3);
    com.publisher(direction);
    chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
    chrono::duration<double> time_used = chrono::duration_cast<chrono::duration<double>>(t2 - t1);
    cout<<"elapsed time = "<<time_used.count() << "seconds" << endl;
    cout<<"estimated moving distance0: "<<time_to_dist(time)<<" meters"<<endl;

    return 0;
}
