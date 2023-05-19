#include <iostream>
#include <string.h>
#include <vector>
using namespace std;
/*
    @brief: Test str.find & str.substr function to treat arm's msg before public to ros.
*/


void parse(string& msg){
    std::string j1 = "<joint1>", j2 = "<joint2>", j3 = "<joint3>", j4 = "<joint4>", 
        j5 = "<joint5>", j6 = "<joint6>"; //start mark

    std::string j11 = "</joint1>", j22 = "</joint2>", j33 = "</joint3>", 
        j44 = "</joint4>", j55 = "</joint5>", j66 = "</joint6>"; // end mark
    std::string a1, a2, a3, a4, a5, a6;
    size_t p1, p2, p3, p4, p5, p6;
    size_t p11, p22, p33, p44, p55, p66;
    size_t l1 = j1.length();
    p1 = msg.find("<joint1>", 0); p11 = msg.find("</joint1>", 0);
    p2 = msg.find("<joint2>", 0); p22 = msg.find("</joint2>", 0);
    p3 = msg.find("<joint3>", 0); p33 = msg.find("</joint3>", 0);
    p4 = msg.find("<joint4>", 0); p44 = msg.find("</joint4>", 0);
    p5 = msg.find("<joint5>", 0); p55 = msg.find("</joint5>", 0);
    p6 = msg.find("<joint6>", 0); p66 = msg.find("</joint6>", 0);
    a1 = msg.substr(p1 + l1, p11 - p1 - l1);
    a2 = msg.substr(p2 + l1, p22 - p2 - l1);
    a3 = msg.substr(p3 + l1, p33 - p3 - l1);
    a4 = msg.substr(p4 + l1, p44 - p4 - l1);
    a5 = msg.substr(p5 + l1, p55 - p5 - l1);
    a6 = msg.substr(p6 + l1, p66 - p6 - l1);
    cout<<"a1: "<<a1<<"a2: "<<a2<<"a3: "<<a3<<"a4: "<<a4<<"a5: "<<a5<<"a6: "<<a6<<endl;
    
    size_t px, py, pz, pa, pb, pc;
    size_t pxx, pyy, pzz, paa, pbb, pcc;
    size_t lpx, lpxx;
    std::string x, y, z, a, b, c;
    std::string PX = "<PX>"; lpx = PX.length();

    px = msg.find("<PX>", 0); pxx = msg.find("</PX>", 0);
    py = msg.find("<PY>", 0); pyy = msg.find("</PY>", 0);
    pz = msg.find("<PZ>", 0); pzz = msg.find("</PZ>", 0);
    pa = msg.find("<PA>", 0); paa = msg.find("</PA>", 0);
    pb = msg.find("<PB>", 0); pbb = msg.find("</PB>", 0);
    pc = msg.find("<PC>", 0); pcc = msg.find("</PC>", 0);

    x = msg.substr(px + lpx, pxx - px - lpx);
    y = msg.substr(py + lpx, pyy - py - lpx);
    z = msg.substr(pz + lpx, pzz - pz - lpx);
    a = msg.substr(pa + lpx, paa - pa - lpx);
    b = msg.substr(pb + lpx, pbb - pb - lpx);
    c = msg.substr(pc + lpx, pcc - pc - lpx);
    cout<<"x: "<<x<<"y: "<<y<<"z: "<<z<<"a: "<<a<<"b: "<<b<<"c: "<<c<<endl;
}

int main(int argc, char** argv){
    
    std::string j1 ="<joint1>11.234</joint1>", j2 = "<joint2>22.234</joint2>", j3 = "<joint3>33.234</joint3>",
        j4 = "<joint4>44.234</joint4>", j5 = "<joint5>55.234</joint5>", j6 = "<joint6>66.234</joint6>",
        px = "<PX>77.234</PX>", py = "<PY>88.234</PY>", pz = "<PZ>99.234</PZ>", 
        pa = "<PA>1.23</PA>", pb = "<PB>2.34</PB>", pc = "<PC>3.234</PC>";
    /**fake msg: <joint1>11.234</joint1><joint2>22.234</joint2><joint3>33.234</joint3><joint4>44.234</joint4>
    <joint5>55.234</joint5><joint6>66.234</joint6><PX>77.234</PX><PY>88.234</PY>
    <PZ>99.234</PZ><PA>1.23</PA><PB>2.34</PB><PC>3.234</PC>**/
    string msg = j1+j2+j3+j4+j5+j6+px+py+pz+pa+pb+pc;
    cout<<"msg: "<<msg<<endl;
    parse(msg);
    cout<<"new msg: "<<msg<<endl;
   

    return 0;
}