#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <iterator>
#include "doublelinklist.h"
#include "objects.h"

using namespace std;



int main()
{
    GameState first_state;
    first_state.create_init_state();

    while (!first_state.lose && !first_state.win)
    {
        first_state.print_situation();
        int action, object;
        cout << "Select action: 1-Open 2-Look At 3-Pick Up 4- Misbehave 5- Talk To: ";
        cin >> action;
        cout << "Select object: ";
        cin >> object;
        int effectID = first_state.advance(action, object);//You may use effectID to check whether your last action changed something...
    }


    return 0;

}
