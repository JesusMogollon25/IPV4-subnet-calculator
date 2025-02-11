#include <iostream>
#include <cmath>
#include <conio.h>

using std::cout;
using std::cin;

enum IPClass{A, B, C, D, E};

void printIpAddress(int address[], bool fullIP);
void printSubNets(int address[], int jump, IPClass ipClass);

struct IPaddress {
    int ipMask[4] = {0, 0, 0, 0};
    int address[4] = {192, 168, 1, 0};
    IPClass ipClass = C;

    void setIpMask(){
        int octets;
        switch(ipClass){
            case A: octets = 1;
                break;
            case B: octets = 2;
                break;
            case C: octets = 3;
                break;
            case D: octets = 4;
                cout << "This IP calculator cannot, at the moment, calculate Class D IP Addresses";
                cout << "Press any key to quit";
                exit(1);
                break;
            case E: octets = 5;
                cout << "This is an experimental IP Address, and as such, is not valid at the moment";
                cout << "Press any key to quit";
                exit(1);
            default:
                cout << "Error, cannot set IP mask for current IP Address (Invalid IPV4 Address)";
                cout << "Press any key to quit";
                exit(1);
        }
    
        for(int i = 0;i < octets; i++){
            ipMask[i] = 255;
        }
    }
};

// IPV4 Address calculator.

int main(){
    const int DEFMASK = 255;

    //Defaults;
    IPaddress ipAddress;
    int numSubnets = 64;

    // logo and stuff
    cout << "IPV4 IP Address Calculator\n" << "---------------\n";

    cout << ">Type IPV4 Address (using space instead of points e.g = 192 168 1 0): ";

    for(int i = 0; i <= 3; i ++){
        cin >> ipAddress.address[i];
    }

    cout << ">Type number of subnets to create: ";
    cin >> numSubnets;
    
    cout << "---------------\n";

    
    cout << "Info about ";
    printIpAddress(ipAddress.address, true);
    cout << ":\n";

    

    cout << "IP Class: ";

    if(ipAddress.address[0] <= 0){
        cout << "Error. Non valid IP Address detected";
        return 1;
    }else if(ipAddress.address[0] <= 126){
        cout << "Class A";
        ipAddress.ipClass = A;
        ipAddress.setIpMask();
    }else if(ipAddress.address[0] <= 191){
        cout<< "Class B";
        ipAddress.ipClass = B;
        ipAddress.setIpMask();
    }else if(ipAddress.address[0] <= 223){
        cout<< "Class C";
        ipAddress.ipClass = C;
        ipAddress.setIpMask();
    }else if(ipAddress.address[0] <= 239){
        cout<< "Class D";
        ipAddress.ipClass = D;
        ipAddress.setIpMask();
    }else if(ipAddress.address[0] <= 255){
        cout<< "Class E";
        ipAddress.ipClass = E;
        ipAddress.setIpMask();
    }else{
        cout<< "Error. Non valid IP Address detected";
        return 1;
    }

    cout << " IP address\n";

    cout<< "Default IP mask: ";
    printIpAddress(ipAddress.ipMask, true);

    auto calculateJump = [] (int numSubnets){
        int n = 0;
        while(pow(2, n) < numSubnets){
            n++;
        }

        int total = 0;
        const int CONVTABLE[] = {128, 64, 32, 16, 8, 4, 2, 1};
        for(int i = 0;i < n;i++){
            total += CONVTABLE[i];
        }

        cout << "\n2^N: " << (pow(2, n));
        cout << "\nN: " << n;
        cout << "\nNumber: " << total;

        return 256 - total;
    };

    int jump = abs(calculateJump(numSubnets));
    
    if(jump < 4||jump > 256){
        cout << "\nFatal error. Number of subnets impossible.\n";
        cout << "Press any key to quit.\n";
        getch();
        return 1;
    }

    cout << "\nJump: " << jump;

    cout<< "\nIP mask: ";
    switch (ipAddress.ipClass)
    {
    case A: ipAddress.ipMask[1] = 256 - jump;
    break;
    case B: ipAddress.ipMask[2] = 256 - jump;
    break;
    case C: ipAddress.ipMask[3] = 256 - jump;
    break;
    }
    
    printIpAddress(ipAddress.ipMask, true);

    cout << "\n---------------";

    cout << "\nnumber of subnets to create: " << numSubnets;

    cout << "\nPress any key to print subnets\n";
    getch();
    printSubNets(ipAddress.address, jump, ipAddress.ipClass);

    cout << "\nPress any key to quit";
    getch();
    
    return 0;
}

void printIpAddress(int address[], bool fullIP){
    for(int i = 0; i < 3; i++){
        cout << address[i];

        if(i <= 2){
            cout << ".";
        }
    }

    if(fullIP){
        cout << address[3];
    }
}

void printSubNets(int address[], int jump, IPClass ipClass){
    cout <<
          "\n    SUBNET" << "                   NETS                    " << "BROADCAST\n";
    cout << "   --------" << "                 ------                  " << "-----------" << "\n";

    auto addSpace = [] (int current){
        cout << "    ";
        if(current < 10){
            cout << "  ";
        }else if(current < 100){
            cout << " ";
        }
    };

    //these represent the numbers after the first octate, from left to right
    int calculatedIP[] = {0, 0, 0};
    int repeatPerClass = 0;

    switch (ipClass)
    {
    case A:
        repeatPerClass = 3;
        break;
    case B:
        repeatPerClass = 2;
        break;
    case C:
        repeatPerClass = 1;
        break;
    }

    do{
        if(ipClass < 3){
            address[2] = calculatedIP[1];

            if(ipClass < 2){
                address[1] = calculatedIP[2];
            }
        }
        while(calculatedIP[0] < 256){
            // Subnet
            address[3] = calculatedIP[0];               
            printIpAddress(address, true);
            addSpace(calculatedIP[0]);

            // First usable
            address[3] = calculatedIP[0] + 1;
            printIpAddress(address, true);
            addSpace(calculatedIP[0]);

            // Last usable
            address[3] = calculatedIP[0] - 2 + jump;
            printIpAddress(address, true);
            addSpace(calculatedIP[0]);

            // Broadcast
            address[3] = calculatedIP[0] - 1 + jump;
            printIpAddress(address, true);
            addSpace(calculatedIP[0]);
            cout << '\n';

            calculatedIP[0] += jump;
            if(calculatedIP[0] == 0){
                calculatedIP[0]++;
            }
        }

        calculatedIP[0] = 0;

        if(calculatedIP[1] < 256){
            calculatedIP[1]++;
        }else{
            repeatPerClass--;
        }

        if(ipClass == A){
            if(calculatedIP[2] < 256 && calculatedIP[1] >= 256){
                calculatedIP[2]++;
                calculatedIP[1] = 0;

                if(calculatedIP[2] >= 256){
                    return;
                }
            }else if(calculatedIP[2] > 255){
                repeatPerClass--;
            }
        }

        if(ipClass == C){
            repeatPerClass = 0;
        }
    }while(repeatPerClass > 0);
}