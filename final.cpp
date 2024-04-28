#include <iostream>
#include <fstream>
#include <string>

using namespace std;

string letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

void create(string file_name);
void insert(string file_name);
void display(string file_name);
bool check_key(string key);
void process(string input_file, string key, string output_file, bool mode);
void encrypt(string file_name, string key);
void decrypt(string file_name, string key);

int main (int argc, char* argv[]) {     // argv[0] - ./program, argv[1] - KEY

    // checking if command has 2 arguments
    if (argc != 2) {
        cout << "Usage: " << "./program KEY" << endl;
        return 1;
    }
    // checking if the key is valid
    if (!check_key(argv[1])) {
        return 1;
    }

    // menu
    while (true) {
        cout << "\n";
        cout << "\tChoose the operation to perform" << endl;
        cout << "1 - Create new file" << endl;
        cout << "2 - Insert data into the file (creates new file if not found)" << endl;
        cout << "3 - Display data in the file" << endl;
        cout << "4 - Encrypt file using the provided key (saves in encrypted.txt)" << endl;
        cout << "5 - Decrypt file using the provided key (saves in decrypted.txt)" << endl;
        cout << "0 - Exit" << endl;

        int choice;
        cout << "\nEnter the number: ";
        cin >> choice;
        
        string file;

        // prompting for file name if operation requires that
        if (1 <= choice && choice <= 5) {
            cout << "Enter file name (ex. file.txt): ";
            cin >> file;
            cin.ignore();
        } else if (choice == 0) {
            cout << "Bye :)" << endl;
            return 0;
        } else {
            cout << "Unknown operation. Please try again" << endl;
            return 1;
        }

        // executing the chosen operation
        if (choice == 1) {
            create(file);
        } else if (choice == 2) {
            insert(file);
        } else if (choice == 3) {
            display(file);
        } else if (choice == 4) {
            encrypt(file, argv[1]);
        } else if (choice == 5) {
            decrypt(file, argv[1]);
        }

        cout << "\nEnter 1 to perform another operation, 0 to exit: ";
        cin >> choice;
        if (!choice) {
            cout << "Bye :)";
            break;
        }
    }

    return 0;
}

// checking if the key is valid
bool check_key(string key) {
    if (key.length() != 26) {
        cout << "Key must contain 26 characters" << endl;
        return false;
    }
    for (int i = 0; i < key.length(); i++) {
        if (!isalpha(key[i])) {
            cout << "Key must only contain alphabetic characters" << endl;
            return false;
        }
        // key[i] = toupper(key[i]);   // this is not working because it is not global variable
    }
    for (int i = 0; i < key.length(); i++) {
        for (int j = i+1; j < key.length(); j++) {
            if (key[i] == key[j]) {
                cout << "Key must not contain repeated characters" << endl;
                return false;
            }
        }
    }
    return true;
}

// creating a new file
void create(string file_name) {
    ofstream f1(file_name);     // fstream f1(file_name, ios:: out);

    string text;
    cout << "Enter text to write into the file: ";
    getline(cin, text);
    f1 << text << endl;

    f1.close();
    cout << "\nThe text was written into the file!" << endl;
}

// inserting data into the file
void insert(string file_name) {
    ofstream f1(file_name, ios:: app);      // fstream f1(file_name, ios:: app, ios:: out);

    string text;
    cout << "Enter text to insert: ";
    getline(cin, text);
    f1 << text << endl;

    f1.close();
    cout << "\nThe text was inserted into the file!" << endl;
}

// displaying data from the file
void display(string file_name) {
    ifstream f1(file_name);         // fstream f1(file_name, ios:: in);
    if (!f1) {
        cout << "No such file found..." << endl;
        return;
    }

    string line;
    cout << "\n\tData in " << file_name << endl;

    while (getline(f1, line)) {     // or while(f1) { getline(f1, line); cout << line; }
        cout << line << endl;
    }
    f1.close();
}

// encrypt mode
void encrypt(string file_name, string key) {
    process(file_name, key, "encrypted.txt", true);
    cout << "\nThe text was encrypted into encrypted.txt!" << endl;
}

// decrypt mode
void decrypt(string file_name, string key) {
    process(file_name, key, "decrypted.txt", false);
    cout << "\nThe text was decrypted into decrypted.txt!" << endl;
}

void process(string input_file, string key, string output_file, bool mode) {

    // opening files to read from and write to
    ifstream f1(input_file);
    if (!f1) {
        cout << "No such file found..." << endl;
        return;
    }
    ofstream f2(output_file);

    string input, output;

    // reading the text from the file (line by line)
    while (getline(f1, input)) {
        output.resize(input.length());

        // plaintext <-> ciphertext     // ex. "ABCDEFGHIJKLMNOPQRSTUVWXYZ" <-> "QWERTYUIOPASDFGHJKLZXCVBNM"
        for (int i = 0, len = input.length(); i < len; i++) {
            if (isalpha(input[i])) {
                for (int j = 0; j < 26; j++) {

                    // encrypting data from the input_file into "encrypted.txt"
                    if (mode) {
                        if (toupper(input[i]) == letters[j]) {
                            if (isupper(input[i])) {
                                output[i] = toupper(key[j]);
                            } else if (islower(input[i])) {
                                output[i] = tolower(key[j]);
                            }
                            break;
                        }

                    // decrypting data from the input_file into "decrypted.txt"
                    } else {
                        if (input[i] == toupper(key[j]) || input[i] == tolower(key[j])) {
                            if (isupper(input[i])) {
                                output[i] = toupper(letters[j]);
                            } else if (islower(input[i])) {
                                output[i] = tolower(letters[j]);
                            }
                            break;
                        }
                    }

                }
            } else {
                output[i] = input[i];
            }
        }

        // inserting ciphertext into the file
        f2 << output << endl;
        cout << input << " -> " << output << endl;
    }

    f1.close();
    f2.close();
}
