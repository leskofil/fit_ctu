#include <iostream>

#include "CApplication.h"

using namespace std;

int main() {
    try {
        CApplication app;
        app.Run();
    } catch (const exception &ex) {
        cerr << "Error: " << ex.what() << endl;
        return 1;
    } catch (const string &ex) {
        cerr << "Error: " << ex << endl;
        return 2;
    }
    return 0;
}
