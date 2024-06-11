/*
Abdulsamet Ekinci
150220723
*/
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <cstring>
#include <iterator>
#include "doublelinklist.h"
#include "objects.h"

using namespace std;

template<typename T>
class Stack {
private:
    DoublyList<T> list;

public:
    void push(T data) {
        list.addFront(data);
    }

    void pop() {
        list.removeFront();
    }

    
    T top() {
        return list.get(0);
    }

    bool empty() {
        return list.elemcount == 0;
    }
};

void DFS(GameState& state, Stack<GameState>& stack) {
    // Print current state situation
    state.print_situation();

     // Simulate advancing game state based on action and object
    for (int action = 1; action <= 5; ++action) {
        for (int object = 0; object < state.rooms.get(state.room_id)->room_objects.elemcount; ++object) {
            GameState next_state = state;

           
            int result = next_state.advance(action, object);
            
            // If statements to prevent loops
            if (result == 13 && !check_for_item(state.inventory, "Toilet info") || result == 13 && find_index(state.rooms.get(1)->room_objects, "Guard (Near toilet)") != -1) {           
                continue;
            }
            if(result == 14 && find_index(state.rooms.get(1)->room_objects, "Guard (Near toilet)") == -1){
            continue;
            }
            if (result == 16 && check_for_item(state.inventory, "Coin")){
                continue;
            }
            if(result == 12){
                return;
            }
            
           
            if ( result > 0) {
                
                
                // Push next state onto the stack if result > 0 (no loss)
                
               return DFS(next_state, stack);

                // Break out of the inner loop
                break;
            }
           
            
           else{
                continue;
            }
        }

       
        

}}

int main() {
    GameState first_state;
    first_state.create_init_state();

    Stack<GameState> stack;
    stack.push(first_state);
    
    GameState current_state = stack.top();
        
    // Perform DFS traversal recursively
    DFS(current_state, stack);

         
    return 0;
}