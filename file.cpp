#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <functional>


using namespace std;

enum class Color {
    WHITE,
    GREY,
    BLACK
};

struct Person {
    Color color = Color::WHITE;
    int scc_id = -1;
    int value = 0;
};

struct SCC {
    vector<int> people;
};

class TugaNet {
    vector<Person> people;
    vector<vector<int>> outgoingEdges;
    vector<vector<int>> incomingEdges;
    stack<int> visitedPeople;

    vector<SCC> sccs;

    Color get_person_color(int personId) {
        return people[personId].color;
    }

    void set_person_color(int personId, Color color) {
        people[personId].color = color;
    }

    void push_friends(int personId, vector<vector<int>>& edges, stack<int>& stack) {
        for (int friendId : edges[personId]) {
            if (get_person_color(friendId) == Color::WHITE) {
                stack.push(friendId);
            }
            else if (get_person_color(friendId) == Color::BLACK){
                if (people[personId].value <= people[friendId].value ){
                    people[personId].value = people[friendId].value+1;
                }
            }
            else{
                if (people[personId].value <= people[friendId].value){
                    people[personId].value = people[friendId].value;
                }
            }
        }
    }

    void update_value(int personId, vector<vector<int>>& edges, stack<int>& stack){
        for (int friendId : edges[personId]) {
            if (get_person_color(friendId) == Color::BLACK){
                if (people[personId].value <= people[friendId].value){
                    people[personId].value = people[friendId].value;
                }
            }
            else{
                if (people[personId].value <= people[friendId].value){
                    people[personId].value = people[friendId].value;
                }
            }
        }
    }


    void dfs_visit(int sourceId, vector<vector<int>>& edges, function<void(int)> actionOnComplete) {
        stack<int> stack;
        stack.push(sourceId);
        while (!stack.empty()) {
            int currPersonId = stack.top();
            switch (get_person_color(currPersonId)) {
                case Color::WHITE: {
                    set_person_color(currPersonId, Color::GREY);
                    push_friends(currPersonId, edges, stack);
                    break;
                }
                case Color::GREY: {
                    set_person_color(currPersonId, Color::BLACK);
                    update_value(currPersonId, edges, stack);
                    actionOnComplete(currPersonId);
                    break;
                }
                default: {
                    stack.pop();
                    break;
                }
            }
        }
    }

    void dfs() {
        for (size_t i = 0; i < people.size(); i++) {
            if (people[i].color == Color::WHITE) {
                dfs_visit(i, outgoingEdges, [&](int id) {visitedPeople.push(id);});
            }
        }
    }

    void reset_colors() {
        for (size_t i = 0; i < people.size(); i++) {
            set_person_color(i, Color::WHITE);
        }
    }

    bool has_SCC(int personId) {
        return people[personId].scc_id == -1;
    }

    void build_SCC(int sourceId) {
        SCC scc;

        dfs_visit(sourceId, incomingEdges, [&](int id) {people[id].scc_id = sccs.size(); 
                                                        scc.people.push_back(id);
                                                        });
        sccs.push_back(scc);
    }

    void build_SCCs() {
        reset_colors();

        while (!visitedPeople.empty()) {
            int personId = visitedPeople.top();
            visitedPeople.pop();
            if (has_SCC(personId)) {
                build_SCC(personId);
            }
        }
    }

    void clear_values(){
        for (int personId=0; personId<(int)people.size();personId++){
            people[personId].value=0;
        }
    }

    void compress() {
        dfs();
        clear_values();
        build_SCCs();
    }

    void push_SCC_neigbours(int sccId, vector<int>& queue) { 
        for (int personId : sccs[sccId].people) {
            for (int friendId : outgoingEdges[personId]) {

                if (people[friendId].scc_id != sccId) {
                    queue.push_back(people[friendId].scc_id);
                }
            }
        }
    }

    public:
        TugaNet(int numPeople) {
            people = vector<Person>(numPeople);
            outgoingEdges = vector<vector<int>>(numPeople, vector<int>());
            incomingEdges = vector<vector<int>>(numPeople, vector<int>());
        }

        void addEdge(int person, int personFriend) {
            outgoingEdges[person - 1].push_back(personFriend - 1);
            incomingEdges[personFriend - 1].push_back(person - 1);
        }

        int calc_max_propagation() {
            compress();
            int max_propagation = 0;
            for (Person person : people){
                if (person.value > max_propagation){
                    max_propagation = person.value;
                }
            }
            //vector<int> queue;
            //push_SCC_neigbours(0, queue);
            return max_propagation;
        }    
};

TugaNet buildTugaNet() {
    int numPeople, numoutgoingEdges;
    if (scanf("%d %d", &numPeople, &numoutgoingEdges) != 2) {
        fprintf(stderr, "Error reading input for numPeople and numOutgoingEdges\n");
        exit(EXIT_FAILURE);
    }

    TugaNet tugaNet(numPeople);
    for (int i = 0; i < numoutgoingEdges; i++) {
        int person, personFriend;
        if (scanf("%d %d", &person, &personFriend) != 2) {
            fprintf(stderr, "Error reading input for person and personFriend\n");
            exit(EXIT_FAILURE);
        }
        tugaNet.addEdge(person, personFriend);
    }

    return tugaNet;
}

int main() {
    TugaNet tugaNet = buildTugaNet();
    printf("%d\n", tugaNet.calc_max_propagation());

    return 0;
}
