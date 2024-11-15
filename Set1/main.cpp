#include <iostream>
#include <string>
#include <bitset>

using namespace std;

// Given a hex string, converts and returns the binary representation
string hexToBinary(string input)
{
    string binary;
    for (char c : input)
    {
        // Convert each hex character to an integer
        int value = (c >= '0' && c <= '9')   ? (c - '0')
                    : (c >= 'A' && c <= 'F') ? (c - 'A' + 10)
                                             : (c - 'a' + 10);

        // Use bitset to convert the integer value to a 4-bit binary string.
        binary += bitset<4>(value).to_string();
    }
    return binary;
}

// Given a binary string, converts and returns the hex representation
string binaryToHex(string input)
{
    string hex;
    for (size_t i = 0; i < input.size(); i += 4) {
        // get a 4 bit chunk and convert it to an int value
        string chunk = input.substr(i, 4);  
        int value = stoi(chunk, nullptr, 2);
        // Convert integer to hex char
        hex += (value < 10) ? char('0' + value) : char('a' + value - 10); 
    }
    return hex;
}

// Problem 1, hex to base 64 converter
string hexToBase64(string input)
{
    // Convert hex to binary
    string binary = hexToBinary(input);

    // ensure the binary is in 6-bit sections.
    while (binary.size() % 6 != 0)
    {
        binary += '0';
    }

    // Map each 6-bit group to the corresponding Base64 character using a lookup table.
    string base64Chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    string base64;

    // convert each 6-bit chunk into an ascii value
    for (size_t i = 0; i < binary.size(); i += 6)
    {
        string chunk = binary.substr(i, 6);
        int index = stoi(chunk, nullptr, 2);
        base64 += base64Chars[index];
    }

    return base64;
}


// takes two equal-length buffers and produces their XOR combination.
string fixedXOR(string input, string input2)
{
    if (input.size() != input2.size())
        return "Error: Different length buffer.";

    // convert to binary
    string binary, binary2;
    binary = hexToBinary(input);
    binary2 = hexToBinary(input2);

    // perform xor
    string result;
    for (size_t i = 0; i < binary.size(); i++) {
        // Convert characters to integers
        int bit1 = binary[i] - '0'; 
        int bit2 = binary2[i] - '0';

        // Perform the XOR on the bits and convert the result back to a character
        result += (bit1 ^ bit2) + '0';
    }


    // convert back to hex
    string output = binaryToHex(result);

    return output;
}

// Converts a hex string to a plain text string (assuming ASCII characters)
string hexToAscii(const string& hex) {
    string ascii;
    for (size_t i = 0; i < hex.length(); i += 2) {
        int byte = stoi(hex.substr(i, 2), nullptr, 16);
        ascii += static_cast<char>(byte);
    }
    return ascii;
}

// Scoring function for English text based on frequency of common characters
int scoreText(const string& text) {
    string commonChars = "etaoin shrdlu";
    int score = 0;
    for (char c : text) {
        if (isalpha(c) || c == ' ') {
            score += commonChars.find(tolower(c)) != string::npos ? 1 : 0;
        }
    }
    return score;
}


void singleByteXorDecoder(const string& hexInput) 
{
    string asciiInput = hexToAscii(hexInput);
    
    int bestScore = 0;
    string bestDecryption;
    char bestKey;

    // Try all possible single-byte keys (0-255)
    for (int key = 0; key < 256; ++key) {
        string decryptedText;

        // XOR each character in the ASCII string with the key
        for (char c : asciiInput) {
            decryptedText += c ^ key;
        }

        // Score the result based on letter frequency
        int score = scoreText(decryptedText);

        // Update best score and decryption if this one is better
        if (score > bestScore) {
            bestScore = score;
            bestDecryption = decryptedText;
            bestKey = static_cast<char>(key);
        }
    }

    // Output best result
    cout << "Best Key: " << bestKey << "\nScore: " << bestScore << "\nDecryption: " << bestDecryption << endl;
}

// Void function to test each problems example input and compare it to the output.
void problems()
{
    // 1
    string problemOneInput = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
    string problemOneOutput = hexToBase64(problemOneInput);
    (problemOneOutput == "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t") ? cout << "Passed Problem 1.\n" : cout << "Failed Problem 1.\n";

    // 2
    string problemTwoInput1 = "1c0111001f010100061a024b53535009181c";
    string problemOneInput2 = "686974207468652062756c6c277320657965";
    string problemTwoOutput = fixedXOR(problemTwoInput1, problemOneInput2);
    (problemTwoOutput == "746865206b696420646f6e277420706c6179") ? cout << "Passed Problem 2.\n" : cout << "Failed Problem 2.\n";

    // 3
    singleByteXorDecoder("1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736");
}

int main()
{
    problems();
    return 0;
}