#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>
#include <cctype>
using namespace std;

const string FILENAME = "passwords.dat";
const string MASTER_FILE = "master.key";
const int SHIFT_KEY = 5;

// Caesar cipher encryption
string encrypt(const string& text) {
    string result = "";
    for (char c : text) {
        if (isalpha(c)) {
            char base = islower(c) ? 'a' : 'A';
            c = ((c - base + SHIFT_KEY) % 26) + base;
        }
        result += c;
    }
    return result;
}

// Caesar cipher decryption
string decrypt(const string& text) {
    string result = "";
    for (char c : text) {
        if (isalpha(c)) {
            char base = islower(c) ? 'a' : 'A';
            c = ((c - base - SHIFT_KEY + 26) % 26) + base;
        }
        result += c;
    }
    return result;
}

class PasswordManager {
private:
    map<string, string> passwords; // Using map for O(log n) access
    string masterKey;

    void loadMasterKey() {
        ifstream file(MASTER_FILE);
        if (file) {
            getline(file, masterKey);
            file.close();
        }
    }

    void createMasterKey() {
        cout << "Create a master password: ";
        cin >> masterKey;
        ofstream file(MASTER_FILE);
        file << masterKey;
        file.close();
        cout << "Master password set!\n";
    }

    void loadPasswords() {
        ifstream file(FILENAME);
        string line, service, encrypted;
        while (getline(file, line)) {
            size_t pos = line.find(':');
            if (pos != string::npos) {
                service = line.substr(0, pos);
                encrypted = line.substr(pos + 1);
                passwords[service] = encrypted;
            }
        }
        file.close();
    }

    void savePasswords() {
        ofstream file(FILENAME);
        for (const auto& entry : passwords) {
            file << entry.first << ":" << entry.second << "\n";
        }
        file.close();
    }

public:
    PasswordManager() {
        ifstream masterFile(MASTER_FILE);
        if (!masterFile) {
            createMasterKey();
        } else {
            loadMasterKey();
        }
        loadPasswords();
    }

    bool authenticate() {
        string attempt;
        cout << "Enter master password: ";
        cin >> attempt;
        return attempt == masterKey;
    }

    void addPassword() {
        string service, password;
        cout << "Enter service name: ";
        cin >> service;
        
        if (passwords.find(service) != passwords.end()) {
            cout << "Service already exists! Use update instead.\n";
            return;
        }

        cout << "Enter password: ";
        cin >> password;
        
        passwords[service] = encrypt(password);
        savePasswords();
        cout << "Password added successfully!\n";
    }

    void getPassword() {
        string service;
        cout << "Enter service name: ";
        cin >> service;
        
        auto it = passwords.find(service);
        if (it != passwords.end()) {
            cout << "Password: " << decrypt(it->second) << "\n";
        } else {
            cout << "Service not found!\n";
        }
    }

    void listServices() {
        if (passwords.empty()) {
            cout << "No services stored!\n";
            return;
        }

        vector<string> services;
        for (const auto& entry : passwords) {
            services.push_back(entry.first);
        }
        
        sort(services.begin(), services.end());
        cout << "\nStored Services:\n";
        for (const string& service : services) {
            cout << "- " << service << "\n";
        }
    }

    void changeMasterKey() {
        string current;
        cout << "Enter current master password: ";
        cin >> current;
        
        if (current != masterKey) {
            cout << "Incorrect master password!\n";
            return;
        }

        cout << "Enter new master password: ";
        cin >> masterKey;
        
        ofstream file(MASTER_FILE);
        file << masterKey;
        file.close();
        cout << "Master password updated!\n";
    }
};

void displayMenu() {
    cout << "\nPassword Manager Menu:\n";
    cout << "1. Add new password\n";
    cout << "2. Retrieve password\n";
    cout << "3. List all services\n";
    cout << "4. Change master password\n";
    cout << "5. Exit\n";
    cout << "Choice: ";
}

int main() {
    PasswordManager manager;
    cout << "\n=== C++ Password Manager ===";
    cout << "\nNIT Silchar - Electrical Engineering\n";
    
    if (!manager.authenticate()) {
        cout << "Authentication failed!\n";
        return 1;
    }

    int choice;
    do {
        displayMenu();
        cin >> choice;
        
        switch(choice) {
            case 1: manager.addPassword(); break;
            case 2: manager.getPassword(); break;
            case 3: manager.listServices(); break;
            case 4: manager.changeMasterKey(); break;
            case 5: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice!\n";
        }
    } while (choice != 5);

    return 0;
}
