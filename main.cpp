#include "ATCSystem.h"
#include "Exceptions.h"
#include "Utilities.h"
#include "Colors.h"
#include <iostream>

using namespace std;

int main() {
    try {
        setColor(15);
        ATCSystem atc;
        atc.run();
    }
    catch (const ATCException& e) {
        setColor(12); 
        cout << "\n[FATAL ERROR] " << e.what() << endl;
        cout << "The application will now exit." << endl;
        setColor(15);
        return 1;
    }
    catch (const exception& e) {
        setColor(12);
        cout << "\n[FATAL ERROR] " << e.what() << endl;
        cout << "The application will now exit." << endl;
        setColor(15);
        return 1;
    }
    catch (...) {
        setColor(12);
        cout << "\n[FATAL ERROR] An unknown error occurred." << endl;
        cout << "The application will now exit." << endl;
        setColor(15);
        return 1;
    }

    return system("pause");
}
