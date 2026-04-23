/* Names
Project
Description:


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
 DONALD STURKEY
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
     DONALD STURKEY
     Load seat prices and availability
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

        // Load seating
        for (int r = 0; r < ROWS; r++) {
            for (int c = 0; c < COLS; c++) {

                if (seatFile)
                    seatFile >> seats[r][c].available;
                else
                    seats[r][c].available = '#';

                seats[r][c].price = rowPrice[r];
            }
        }
    }

    /*
     DONALD STURKEY
     Save updated availability to file
    */
    void saveData() {
        fstream out("SeatAvailability.dat");

        for (int r = 0; r < ROWS; r++) {
            for (int c = 0; c < COLS; c++) {
                out << (seats[r][c].available ? '#' : '*') << " ";
            }
            out << endl;
        }
    }

public:

    bool invalidSeat = false;

    /*
     DONALD STURKEY
     Constructor / Destructor
    */
    TicketManager() {
        totalSales = 0.0;
        loadData();
    }

    ~TicketManager() {
        saveData();
    }

    /*
     Display seating chart
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
     KIERSTEN BRYCE DAVISON
     Ticket request validation and price quote logic
     =====================================================
    */
    string requestTickets(int row, int startSeat, int numSeats) {
        ostringstream out;

        if (row < 0 || row >= ROWS ||
            startSeat < 0 || startSeat + numSeats > COLS ||
            numSeats <= 0) 
        {
            invalidSeat = true;
            return "Invalid seat selection.\n";
        }

        for (int i = 0; i < numSeats; i++)
            if (!seats[row][startSeat + i].available) {
                invalidSeat = true;
                return "One or more selected seats are unavailable.\n";
            }
        double cost = seats[row][0].price * numSeats;

        out << fixed << setprecision(2);
        out << "Seats available!\n"
            << "Seats requested: " << numSeats << "\n"
            << "Price per seat: $" << seats[row][0].price << "\n"
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
        double totalCost = seats[row][0].price * numSeats;

        if (amountPaid < totalCost)
            return "Insufficient payment.\n";

        for (int i = 0; i < numSeats; i++)
            seats[row][startSeat + i].available = false;

        totalSales += totalCost;

        out << fixed << setprecision(2);
        out << "Purchase complete!\n"
            << "Change due: $" << amountPaid - totalCost << "\n";

        return out.str();
    }

    /*
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

void welcomeScreen() {
    cout << "Welcome to Curington Theater!" << endl << endl;
    cout << "Now Playing: Nightmare on CTC Street IV: Student Tears" << endl;
    system("pause");
}

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
         =====================================================
         KIERSTEN DAVISON
         User-driven ticket request and purchase workflow
         =====================================================
        */
        else if (choice == 2) {
            int row, seat, num;
            double payment;

            do {
                do {
                    cout << "Enter row (1�15): ";
                    cin >> row;
                    if (cin.fail() || row > 15 || row < 1) {
                        cout << "Invalid Row #" << endl;
                        cin.clear();
                        cin.ignore(1000, '\n');
                        row = 0;
                    }
                } while (row > 15 || row < 1);

                do {
                    cout << "Enter starting seat (1�30): ";
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