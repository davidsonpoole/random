#include <iostream>
#include <string>
#include <set>

using namespace std;


struct Row {
    int id;
    set<int> seats{};
};

struct Section {
    int id;
    set<Row> rows{};
};

class Event {
private:
    string name;
    set<Section> sections{};

public:
    Event(const string& name, int numSections, int rowsPerSection, int seatsPerRow) : name(name) {
        for (int i=0; i<numSections) {
            for (int j=0; j<rowsPerSection; j++) {
                for (int k=0; k<seatsPerRow; k++) {
                    Row 
                }
            }
        }
    }
};

int main() {

    Event e("Braves Game");

    // how to represent seats?
    // sections, subsections, rows, seats?
    // a graph? link the seats together?
    // how are views represented?
    // I'm guessing it is done by post analysis of prices. 
    // basically a fixed grid divided up into sections that then analytics teams can
    // adjust prices for based on demand etc. but it's all manual bc there aren't too many
    // sections.

    // so in this case let's say that we just have individual seats that have values as sections.
    // rows can be reassigned whenever.
    // section has rows, rows have seats. 
    // rows can move between sections based on post-analytics. sections can be divided or merged

    // start by creating 100x10 rows. Split into 10 sections. Each section has 10 rows. 10 seats per row.
    // seats represented by ints


    
    cout << "Hello, world!\n";
}