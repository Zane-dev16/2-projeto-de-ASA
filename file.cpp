#include <iostream>
#include <string>
#include <vector>
#include <stack>

using namespace std;

enum class Color {
    WHITE,
    GREY,
    BLACK
};

struct Person {
    Color color = Color::WHITE;
    int scc_id = -1;
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

    void push_friends(int personId, vector<vector<int>>& edges, stack<int>& queue) {
        for (int friendId : edges[personId]) {
            if (get_person_color(friendId) == Color::WHITE) {
                queue.push(friendId);
            }
        }
    }

    void dfs_visit(int sourceId, vector<vector<int>>& edges, function<void(int)> actionOnComplete) {
        stack<int> queue;
        queue.push(sourceId);
        while (!queue.empty()) {
            int currPersonId = queue.top();
            switch (get_person_color(currPersonId)) {
                case Color::WHITE: {
                    set_person_color(currPersonId, Color::GREY);
                    push_friends(currPersonId, edges, queue);
                    break;
                }
                case Color::GREY: {
                    set_person_color(currPersonId, Color::BLACK);
                    actionOnComplete(currPersonId);
                }
                default: {
                    queue.pop();
                    break;
                }
            }
        }
    }

    void dfs() {
        for (size_t i = 0; i < people.size(); i++) {
            if (people[i].color == Color::WHITE) {
                dfs_visit(i, outgoingEdges, [&](int id) {
                    visitedPeople.push(id);
                });
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

        dfs_visit(sourceId, incomingEdges, [&](int id) {
            people[id].scc_id = sccs.size();
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

    void compress() {
        dfs();
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
            vector<int> queue;
            push_SCC_neigbours(0, queue);
            return 0;
        }    
};

TugaNet buildTugaNet() {
    int numPeople, numoutgoingEdges;
    scanf("%d %d", &numPeople, &numoutgoingEdges);

    TugaNet tugaNet(numPeople);

    for (int i = 0; i < numoutgoingEdges; i++) {
        int person, personFriend;
        scanf("%d %d", &person, &personFriend);
        tugaNet.addEdge(person, personFriend);
    }

    return tugaNet;
}

int main() {
    TugaNet tugaNet = buildTugaNet();
    printf("%d\n", tugaNet.calc_max_propagation());

    return 0;
}
