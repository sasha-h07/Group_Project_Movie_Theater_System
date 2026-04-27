/* Team Members: 
   Sasha Herberger, Donald Sturkey, Kiersten Davison, 
   Isaac McKeon, Michael Turner & Thomas Schroder
================================================================
Project Description:
 This program simulates a ticket sales and seat management
 system for the Curington Theater, featuring the film
 "Nightmare on CTC Street IV: Student Tears." The system allows
 users to view a seating chart, request and purchase tickets,
 and track total sales while enforcing seat availability and
 input validation. Seat pricing is managed by row, availability
 is loaded from and saved to files, and sales data is reported
 accurately throughout program execution.
================================================================

*/
/*
 ==============================
 SASHA HERBERGER
 HEADER INCLUDES AND NAMESPACE
 ==============================
*/
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>


using namespace std;

const int ROWS = 15;
const int COLS = 30;

/*
 ==============================
 SASHA HERBERGER
 Seat data structure
 ==============================
*/
struct Seat {
    double price;
    bool available;
};

/*
 ==============================
 DONALD STURKEY
 TicketManager class
 ==============================
*/
class TicketManager {
private:
    Seat seats[ROWS][COLS];
    double totalSales;
    double rowPrice[ROWS];

    /*
    ==============================
    DONALD STURKEY
    Load seat prices and availability
    ==============================
    */
    void loadData() {
        ifstream priceFile("SeatPrices.dat");
        ifstream seatFile("SeatAvailability.dat");

        // Load prices
        for (int r = 0; r < ROWS; r++) {

            if (priceFile)
                priceFile >> rowPrice[r];
            else
                rowPrice[r] = 1 + r; // fallback pricing
        }

       // Load seating edited by Kiersten Davison to prevent truncation
        for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {

        char seatChar;

        if (seatFile) {
            seatFile >> seatChar;
        } else {
            seatChar = '#';  // default to available
        }

        seats[r][c].available = (seatChar == '#');
        seats[r][c].price = rowPrice[r];   
    }
}
}


    /*
     ===================================
     DONALD STURKEY
     Save updated availability to file
     ===================================
    */
    void saveData() {
       ofstream out("SeatAvailability.dat");
        if (!out) return; //to prevent crashing if file can't be opened - Kiersten 


        for (int r = 0; r < ROWS; r++) {
            for (int c = 0; c < COLS; c++) {
                out << (seats[r][c].available ? '#' : '*') << " ";
            }
            out << endl;
        }
    }

public:
    
    //THOMAS SCHROEDER - Flag to track invalid seat selections for input validation loop
    bool invalidSeat = false;

    /*
    ==========================================================
     DONALD STURKEY
     Constructor and destructor to manage data loading/saving
     =========================================================
     */
   
    TicketManager() {
        totalSales = 0.0;
        loadData();
    }

    ~TicketManager() {
        saveData();
    }

     /*
    ==============================
     MICHAEL TURNER
     Diplay seating chart
     ==============================
    */
    string getSeatingChart() const {
        ostringstream out;
        out << "\nSeating Chart (# = Available, * = Sold)\n";

        for (int r = 0; r < ROWS; r++) {
            out << "Row " << setw(2) << r + 1 << ": ";
            for (int c = 0; c < COLS; c++)
                out << (seats[r][c].available ? '#' : '*') << " ";
            out << "\n";
        }
        return out.str();
    }

    /*
     =====================================================
     KIERSTEN DAVISON
     Ticket request validation and price quote logic
     =====================================================
    */
    string requestTickets(int row, int startSeat, int numSeats) {
        invalidSeat = false;
        // Create a string stream to build formatted output messages
        ostringstream out;

        // Validate request boundaries:
        if (row < 0 || row >= ROWS ||
            startSeat < 0 || startSeat + numSeats > COLS ||
            numSeats <= 0) 

        // Mark request as invalid and return error message 
        {
            invalidSeat = true;
            return "Invalid seat selection.\n";
        }
        // Check availability for every seat in the requested group
        // Ensures group seating is handled atomically
        for (int i = 0; i < numSeats; i++)
            if (!seats[row][startSeat + i].available) {
                invalidSeat = true;
                return "One or more selected seats are unavailable.\n";
            }
            // Calculate total cost using row-based pricing
        double cost = seats[row][startSeat].price * numSeats;
        
        // Format output to display monetary values correctly
        out << fixed << setprecision(2);
        //Quote Summary
        out << "Seats available!\n"
            << "Seats requested: " << numSeats << "\n"
            << "Price per seat: $" << seats[row][startSeat].price << "\n"
            << "Total cost: $" << cost << "\n";

        return out.str();
    }

    /*
     =====================================================
     KIERSTEN DAVISON
     Ticket purchase logic and payment processing
     =====================================================
    */
    string purchaseTickets(int row, int startSeat, int numSeats, double amountPaid) {
    ostringstream out;
    double totalCost = seats[row][startSeat].price * numSeats;

    // Step 1: Verify payment
    if (amountPaid < totalCost)
        return "Insufficient payment.\n";

    
// Step 2: AVAILABILITY CHECK
for (int i = 0; i < numSeats; i++) {
    if (!seats[row][startSeat + i].available) {
        return "Purchase failed: one or more seats are no longer available.\n";
    }
}

// Step 3: Commit the transaction
for (int i = 0; i < numSeats; i++)
    seats[row][startSeat + i].available = false;

    totalSales += totalCost;

    out << fixed << setprecision(2);
    out << "Purchase complete!\n"
        << "Change due: $" << amountPaid - totalCost << "\n";

    return out.str();
}

    /*
     =====================================================
     ISAAC MCKEON
     Sales report logic, including totals and breakdowns.
     =====================================================
    */

    string getSalesReport() const {
        int sold = 0, available = 0;

        for (int r = 0; r < ROWS; r++)
            for (int c = 0; c < COLS; c++)
                seats[r][c].available ? available++ : sold++;

        ostringstream out;
        out << fixed << setprecision(2);
        out << "\n--- Sales Report ---\n"
            << "Seats Sold: " << sold << "\n"
            << "Seats Available: " << available << "\n"
            << "Total Sales: $" << totalSales << "\n";

        return out.str();
    }
};

/*
 ==============================
 DONALD STURKEY
 Menu display
 ==============================
*/
void displayMenu() {
    cout << "\n===== MENU =====\n"
        << "1. Display Seating Chart\n"
        << "2. Request Tickets\n"
        << "3. Print Sales Report\n"
        << "4. Exit\n"
        << "Enter choice: ";
}
/*
 ==============================
 THOMAS SCHROEDER
 Movie selection and welcome screen
 ==============================
*/
void welcomeScreen() {
    cout << "Welcome to Curington Theater!" << endl << endl;
    cout << "Now Playing: Nightmare on CTC Street IV: Student Tears" << endl;
    system("pause");
}
/*
===================================================
 Donald Sturkey, Thomas Schroeder, Sasha Herberger
 main()
 Controls program execution by displaying the menu, handling
 user input, managing ticket requests/purchases, and
 exiting the program when selected.
 ==================================================
*/

int main() {
    TicketManager manager;
    int choice;
    

    welcomeScreen();

    do {
        displayMenu();
        cin >> choice;

        if (choice == 1) {
            cout << manager.getSeatingChart();
        }
        /*
         ===========================================================
         Thomas Schroeder, Kiersten Davison
         Validation Input/Testing for ticket requests and purchases
         ===========================================================
        */
        else if (choice == 2) {
            int row, seat, num;
            double payment;

            do {
                do {
                    cout << "Enter row (1-15): ";
                    cin >> row;
                    if (cin.fail() || row > 15 || row < 1) {
                        cout << "Invalid Row #" << endl;
                        cin.clear();
                        cin.ignore(1000, '\n');
                        row = 0;
                    }
                } while (row > 15 || row < 1);

                do {
                    cout << "Enter starting seat (1-30): ";
                    cin >> seat;
                    if (cin.fail() || seat > 30 || seat < 1) {
                        cout << "Invalid Seat #" << endl;
                        cin.clear();
                        cin.ignore(1000, '\n');
                        seat = 0;
                    }
                } while (seat > 30 || seat < 1);

                do {
                    cout << "Enter number of seats: ";
                    cin >> num;
                    if (cin.fail() || num < 1 || (seat + num > 30)) {
                        cout << "Invalid Amount of Seats" << endl;
                        cin.clear();
                        cin.ignore(1000, '\n');
                        num = 0;
                    }
                } while (num < 1 || (seat + num - 1 > 30));

                row--;
                seat--;
                cout << manager.requestTickets(row, seat, num);
            } while (manager.invalidSeat == true);

            cout << "Enter payment amount: $";
            cin >> payment;

            cout << manager.purchaseTickets(row, seat, num, payment);
        }
        else if (choice == 3) {
            cout << manager.getSalesReport();
        }
        else if (choice == 4) {
            cout << "Exiting program...\n";
        }
        else {
            cout << "Invalid option.\n";
            system("pause");
            cin.clear();
            system("cls");
        }

    } while (choice != 4);

    return 0;
}
