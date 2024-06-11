#include<iostream>
#include<string>
#include<cstdlib>
#include<time.h>
#include "doublelinklist.h"

using namespace std;

struct attack {
private:
    string name;
    int pp;
    int accuracy;
    int damage;
    int first;
public:
    attack(const string&, int, int, int, int);
    int get_first();
    int get_accuracy();
    int get_damage();
    int get_pp();
    string get_name();
};

attack::attack(const string& name_in, int p, int a, int d, int f) {
    name = name_in;
    pp = p;
    accuracy = a;
    damage = d;
    first = f;
}

string attack::get_name() {
    return name;
}

int attack::get_pp() {
    return pp;
}

int attack::get_damage() {
    return damage;
}

int attack::get_first() {
    return first;
}

int attack::get_accuracy() {
    return accuracy;
}

struct pokemon {
    int pp;
    int hp;
    string name;
    DoublyList<attack*> attacks;
    pokemon(const string& name_in, int, int);
    string get_name();
    void set(char, int to_reduce);
};

pokemon::pokemon(const string& name_in, int p, int h) {
    name = name_in;
    pp = p;
    hp = h;
}

void pokemon::set(char which, int to_reduce) {
    if (which == 'p') {
        pp += to_reduce;
    }else{
		hp -= to_reduce;
	}
    
}

struct node {
    string ifef;        // attack is efficient or nonefficent
    string curattack;   // attack name
    int num;            // node num
    pokemon* pikachu;   // current pikachu
    pokemon* blastoise; // currrent blastoise
    char status;        // whose turn?
    int level;          // level of the node
    bool isleaf;        // node is a leaf or not
    double prob;        // prob of the node happening

    DoublyList<node*> child;    // children nodes of the node
    node* parent;           // i added this for after finding leaf node tracking the path
    node(const string& ifef_in, const string& curattack_in, int num_in, pokemon* pikachu_in, pokemon* blastoise_in, char status_in, int level_in, bool isleaf_in, double prob_in, node* parent_in)
        : ifef(ifef_in), curattack(curattack_in), num(num_in), pikachu(pikachu_in), blastoise(blastoise_in), status(status_in), level(level_in), isleaf(isleaf_in), prob(prob_in), parent(parent_in) {}
};

struct graph {
    node* root;
    char z[2] = "l"; //random controller.
    graph() {
        root = nullptr;
    }
int nodenum = 0; //node num and i iterate it by num++
   
void buildGraph(node* n, int level, int maxLevel, char* aaa, node* parent, char* bbb) {
 //these couts are debugging codes 
    //cout << "Building node at level " << level << endl;  
   //cout << "P HP: " << n->pikachu->hp << " P PP: " << n->pikachu->pp << " B HP: " << n->blastoise->hp << " B PP: " << n->blastoise->pp << " PROB: " << n->prob << endl;
     string control = aaa;
     string control1 = bbb;
     if(control == "part2" && control1 == "pikachu"){
        buildGraph(root, 0, 7, z, root->parent, z );
        return ;
     }else if(control == "part2" && control1 == "blastoist"){
            buildGraph(root, 0, 8, z, root->parent, z );
            return ;

     }
    if(control == "part1"){
   if (n->isleaf || level == maxLevel ) {
        cout << "P HP: " << n->pikachu->hp << " P PP: " << n->pikachu->pp << " B HP: " << n->blastoise->hp << " B PP: " << n->blastoise->pp << " PROB: " << n->prob << endl;
       
        return ;
    }
    }
    if( n->isleaf ){
        return;
        }
    
   // if(level == maxLevel){return;} // i changed it to n is leaf
    pokemon* attacker = (level % 2 == 0) ? n->pikachu : n->blastoise;
    pokemon* defender = (level % 2 == 0) ? n->blastoise : n->pikachu;
    //cout << attacker->name<< endl << defender->name<<endl;

    // Iterate over attacker's attacks
    for (int i = 0; i < ((level >= 3 ) ? 4 : 3); i++) {
        attack* a = attacker->attacks.get(i);
        
        double accuracy = a->get_accuracy() / 100.0;

        double attackProb = ((level >= 3) ? 0.25 : 0.333333);
        string attackType = a->get_name();
    
        
        
        // cout << "  Attack: " << attackType << ", Accuracy: " << accuracy << ", Attack Probability: " << attackProb << endl;
        
        // copied pokemons and changed their stats and saved it to new node
        pokemon* attackerCopyEff = new pokemon(attacker->name, attacker->pp, attacker->hp);
        attackerCopyEff->attacks = attacker->attacks; // first i have forgotten this and couldn't debug due some problems finally in online debugger i saw that i didn't copied attacks 
        pokemon* defenderCopy = new pokemon(defender->name, defender->pp, defender->hp);
        defenderCopy->attacks = defender->attacks;
        // Create child node for effective attack
        attackerCopyEff->set('p', a->get_pp());
        defenderCopy->set('h', a->get_damage());
        nodenum++;
        node* EffectiveNode = new node("effective", attackType, nodenum ,(level % 2 == 0) ? attackerCopyEff : defenderCopy , (level % 2 == 0) ? defenderCopy : attackerCopyEff, (level % 2 == 0) ? 'p' : 'b', level + 1, ( (defenderCopy->hp <1 || level == maxLevel ) ? true : false),  n->prob*(accuracy)*attackProb, n );
        n->child.addBack(EffectiveNode);//add to children list
        
        
if(accuracy < 1) //if accuracy is not 100 so we will create not effective nodes
{
    nodenum++;
        pokemon* attackerCopyNonEff = new pokemon(attacker->name, attacker->pp, attacker->hp);
        attackerCopyNonEff->attacks = attacker->attacks;
        // Create child node for non-effective attack
        attackerCopyNonEff->set('p', a->get_pp());
        node* nonEffectiveNode = new node("not effective", attackType, nodenum , (level % 2 == 0) ? attackerCopyEff : defender , (level % 2 == 0) ? defender : attackerCopyEff, (level % 2 == 0) ? 'p' : 'b', level + 1, ((level == maxLevel ) ? true : false), n->prob*(1 - (accuracy))* attackProb,n );
        n->child.addBack(nonEffectiveNode);//add to children list
       
        } 
              
    }
   
   
   //recursively do for next levels
   for (Node<node*>* current = n->child.head; current != NULL; current = current->next) {
    node* child = current->data;  
    buildGraph(child, level + 1, maxLevel, aaa, n, bbb);
} 

    return ;
   
}

void findlast(node* n, node** lastList, node*** parentList, char* ctrl2) {
    string ctrl3 = ctrl2;

    if(ctrl3 == "pikachu"){
    // Base case: find leaf's for pikachu win
    if (n->blastoise->hp <= 0) {
        // Find an empty slot 
        int i = 0;
        while (lastList[i] != nullptr) {
            i++;
        }
        lastList[i] = n;

        // Add parent nodes to the parentList
        node* currentParent = n->parent;
        while (currentParent != nullptr) {
            int j = 0;
            while (parentList[i][j] != nullptr) {
                j++;
            }
            parentList[i][j] = currentParent;
            currentParent = currentParent->parent;
        }
        return;
    }
    } if(ctrl3 == "blastoist"){
    // Base case: find leaf's for blastoist win
    if (n->pikachu->hp <= 0) {
        // Find an empty slot 
        int i = 0;
        while (lastList[i] != nullptr) {
            i++;
        }
        lastList[i] = n;

        // Add parent nodes to the parentList
        node* currentParent = n->parent;
        while (currentParent != nullptr) {
            int j = 0;
            while (parentList[i][j] != nullptr) {
                j++;
            }
            parentList[i][j] = currentParent;
            currentParent = currentParent->parent;
        }
        return;
    }
    }

    // Recursive for finding leafs
    for (Node<node*>* current = n->child.head; current != NULL; current = current->next) {
        findlast(current->data, lastList, parentList, ctrl2);
    }
}

void printlastList(node** lastList, node*** parentList, char* ctrl22) {
    string ctrl3 = ctrl22;
    
    // Find the leaf node with the maximum probability
    int maxProbIndex = -1;
    double maxProb = 0.0;
    for (int i = 0; lastList[i] != nullptr; i++) {
        if (lastList[i]->prob > maxProb) {
            maxProb = lastList[i]->prob;
            maxProbIndex = i;
        }
    }

    // Print the parent nodes of the leaf node with the maximum probability
    if (maxProbIndex != -1) {
        for (int j = ((ctrl3 == "pikachu") ? 5 : 6); j >= 0 && parentList[maxProbIndex][j] != nullptr; j--) {
            node* parent = parentList[maxProbIndex][j];
            cout << ((parent->status == 'p') ? "Pikachu" : "Blastoist") << " used " << parent->curattack << ": " << parent->ifef << endl;
        }
        
        // Print the leaf node with the maximum probability
        node* n = lastList[maxProbIndex];
        cout << ((n->status == 'p') ? "Pikachu" : "Blastoist") << " used " << n->curattack << ": " << n->ifef << endl;
        cout << "Level count: " << n->level << endl;
        cout << "Probability: " << n->prob << endl;
    }
}


};

int main(int argc, char** argv) {
   	string part = argv[1];
	int max;
	string which;
	char* pika_or_blastoise;

	if(part == "part1"){
		max = atoi(argv[2]);
	}else if(part == "part2"){
		pika_or_blastoise = argv[2];
	}
	else
		return -1;

    pokemon* pikachu = new pokemon("pikachu", 100, 200);
    pokemon* blastoise = new pokemon("blastoise", 100, 200);

    attack* thundershock = new attack("thundershock", -10, 100, 40, 0);
    attack* skullbash = new attack("skullbash", -15, 70, 50, 0);
    attack* slam = new attack("slam", -20, 80, 60, 0);
    attack* pskip = new attack("skip", 100, 100, 0, 3);
    pikachu->attacks.addBack(thundershock);
    pikachu->attacks.addBack(skullbash);
    pikachu->attacks.addBack(slam);
    pikachu->attacks.addBack(pskip);

    attack* tackle = new attack("tackle", -10, 100, 30, 0);
    attack* watergun = new attack("watergun", -20, 100, 40, 0);
    attack* bite = new attack("bite", -25, 100, 60, 0);
    attack* bskip = new attack("skip", 100, 100, 0, 3);
    blastoise->attacks.addBack(tackle);
    blastoise->attacks.addBack(watergun);
    blastoise->attacks.addBack(bite);
    blastoise->attacks.addBack(bskip);

    // Create the root node and build the graph
    node* root = new node("", "", 0, pikachu, blastoise, 'p', 0, false, 1, nullptr);
    graph battleGraph;
    battleGraph.root = root;
    battleGraph.buildGraph(battleGraph.root, 0, max, argv[1], root->parent, argv[2]);
  node*** parentList = new node**[50000];  // Assuming a maximum of 50000 leafs
    for (int i = 0; i < 50000; i++) {
        parentList[i] = new node*[8];   // Assuming a maximum of 8 parent nodes
        for (int j = 0; j < 8; j++) {
            parentList[i][j] = nullptr;
        }
    }
   
    node** lastList = new node*[50000]; // Assuming a maximum of 50000 leafs
    for (int i = 0; i < 50000; i++) {
        lastList[i] = nullptr;
    }
    
     battleGraph.findlast(root, lastList, parentList, argv[2]);

    // Print the results
    battleGraph.printlastList(lastList, parentList, argv[2]);

    // Free memory
    for (int i = 0; i < 50000; i++) {
        delete[] parentList[i];
    }
    delete[] parentList;
    delete[] lastList;
   
   // cout << root->child.elemcount << endl;
    // cout << root->child.get(1)->child.elemcount ;
    

    return 0;
}
