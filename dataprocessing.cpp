#include <iostream>
#include <unordered_map>
#include <stdexcept>

class InMemoryDB {
public:
    void put(const std::string& key, int val);
    int get(const std::string& key);
    void begin_transaction();
    void commit();
    void rollback();

private:
    std::unordered_map<std::string, int> mainState;
    std::unordered_map<std::string, int> transactionState;
    bool inTransaction = false;
};

void InMemoryDB::put(const std::string& key, int val) {
    if (!inTransaction) {
        throw std::runtime_error("Transaction not in progress");
    }
    transactionState[key] = val;
}

int InMemoryDB::get(const std::string& key) {
    if (inTransaction && transactionState.find(key) != transactionState.end()) {
        return transactionState[key];
    } else {
        return mainState[key];
    }
}

void InMemoryDB::begin_transaction() {
    if (inTransaction) {
        throw std::runtime_error("Transaction already in progress");
    }
    inTransaction = true;
}

void InMemoryDB::commit() {
    if (!inTransaction) {
        throw std::runtime_error("No open transaction");
    }
    mainState = transactionState;  // Corrected: Assign transactionState directly to mainState
    transactionState.clear();
    inTransaction = false;
}

void InMemoryDB::rollback() {
    if (!inTransaction) {
        throw std::runtime_error("No ongoing transaction");
    }
    transactionState.clear();
}

int main() {
    InMemoryDB inMemoryDB;

    std::string userInput;
    char action;

    do {
        std::cout << "Enter action (P for put, G for get, B for begin_transaction, C for commit, R for rollback, Q to quit): ";
        std::cin >> action;

        try {
            switch (action) {
                case 'P': {
                    std::string key;
                    int value;
                    std::cout << "Enter key and value: ";
                    std::cin >> key >> value;
                    inMemoryDB.put(key, value);
                    break;
                }
                case 'G': {
                    std::string key;
                    std::cout << "Enter key: ";
                    std::cin >> key;
                    std::cout << "Value: " << inMemoryDB.get(key) << std::endl;
                    break;
                }
                case 'B':
                    inMemoryDB.begin_transaction();
                    std::cout << "Transaction begun." << std::endl;
                    break;
                case 'C':
                    inMemoryDB.commit();
                    std::cout << "Transaction committed." << std::endl;
                    break;
                case 'R':
                    inMemoryDB.rollback();
                    std::cout << "Transaction rolled back." << std::endl;
                    break;
                case 'Q':
                    std::cout << "Exiting." << std::endl;
                    break;
                default:
                    std::cerr << "Invalid action. Try again." << std::endl;
            }
        } catch (const std::runtime_error& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }

    } while (action != 'Q');

    return 0;
}
