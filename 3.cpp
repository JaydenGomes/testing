#include <iostream>
using namespace std;

// XOR two binary strings
string xorOp(string a, string b) {
    string res = "";
    for (int i = 1; i < b.size(); i++)
        res += (a[i] == b[i]) ? '0' : '1';
    return res;
}

// Perform CRC division
string crcDiv(string data, string divisor) {
    int len = divisor.size();
    string temp = data.substr(0, len);
    while (len < data.size()) {
        temp = (temp[0] == '1') ? xorOp(temp, divisor) + data[len] : temp.substr(1) + data[len];
        len++;
    }
    return (temp[0] == '1') ? xorOp(temp, divisor) : temp.substr(1);
}

// Encode message with CRC
string encode(string msg, string divisor, string &crc) {
    string data = msg + string(divisor.size() - 1, '0');
    crc = crcDiv(data, divisor);
    return msg + crc;
}

// Check if error occurred
bool hasError(string recv, string divisor, string &rem) {
    rem = crcDiv(recv, divisor);
    return rem.find('1') != string::npos;
}

// Check binary input
bool isBinary(const string &s) {
    return s.find_first_not_of("01") == string::npos;
}

int main() {
    string msg, divisor, crc, rem;
    cout << "--Sender--\nEnter message: ";
    cin >> msg;
    cout << "Enter divisor: ";
    cin >> divisor;

    if (!isBinary(msg) || !isBinary(divisor) || divisor.size() < 2) {
        cout << "Invalid input.\n";
        return 1;
    }

    string sent = encode(msg, divisor, crc);
    cout << "CRC: " << crc << "\nSent: " << sent << endl;

    cout << "\n--Receiver--\nMessage changed? (y/n): ";
    char ch;
    cin >> ch;
    if (ch == 'y') {
        int bit;
        cout << "Which bit? (1-based): ";
        cin >> bit;
        if (bit >= 1 && bit <= sent.size())
            sent[bit - 1] ^= 1; // flip bit
    }

    cout << "Received: " << sent << endl;
    if (hasError(sent, divisor, rem))
        cout << "Error detected. Remainder: " << rem << endl;
    else
        cout << "No error. Remainder: " << rem << endl;

    return 0;
}