#include <fstream>
#include <iostream>
#include <array>
#include <iomanip>
#include <cctype>

int main(int argc, char* argv[])
{   
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << "<filename>";
        return 1;
    } 

    // reads the given files
    std::ifstream inf{argv[1], std::ios::binary};

    // check if the file is not read we exit with an error display
    if (!inf){
        std::cerr << "The file could not be read\n";
        return 1;
    }

    // we store what we read from the file in a character array of 16 size 
    // here we use a char array instead of string since for hex dump we need each separate character
    std::array<char,16> buffer{};

    // set the hex formatting, uppercase, hexadecimal format and fill unused bytes with 0
    std::cout << std::uppercase << std::hex << std::setfill('0');

    // the offset before printing the hexadecimal values to count number of bytes read
    unsigned int offset{0}; 
    
    // keep reading the file until all contents are read
    // here we use .read() instead of >> since we want to read as raw bytes instead of text which stores the bytes in the array
    while(inf.read(buffer.data(), 16)){

        // gcount gives us count of bytes that we got on last read (it can't exceed 16 since we set it to that) 
        size_t bytesRead{static_cast<size_t>(inf.gcount())};
        
        std::cout << std::setw(8) << offset << ": "; 

        // loop through the buffer and print each byte in hex format
        for (size_t i = 0; i < bytesRead; i++){
            std::cout << std::setw(2) << static_cast<int>(static_cast<unsigned char>(buffer[i])) << " "; // casting to unsigned char 1st to avoid negatives and then int to print int hex format
        }

        for (size_t i = 0; i <= 16 - bytesRead; i++){
            std::cout << "   ";
        }
        

        // printing the ascii values
        for (size_t i = 0; i < bytesRead; i++){
            // check if the character cannot be printed we print '.'
            if (!std::isprint(buffer[i])){
                std::cout << '.';
            }
            else {
                std::cout << buffer[i];
            }
            
        }

        std::cout << "\n";

        // add the gcount (count of bytes) to offset each time then print it in 8 digit format
        offset += static_cast<unsigned int>(bytesRead);
    }
    
    // check if there are any remaining bytes in the file which can be less than 16
    if (inf.gcount() > 0){
        
        std::cout << std::setw(8) << offset << ": ";

        size_t bytesLeft{static_cast<size_t>(inf.gcount())};

        for (size_t i = 0; i < bytesLeft; i++){
            std::cout << std::setw(2) << std::setfill('0') << std::hex << std::uppercase
            << static_cast<int>(static_cast<unsigned char>(buffer[i])) << " ";
        }

        for (size_t i = 0; i <= 16 - bytesLeft; i++){
            std::cout << "   ";
        }

        for (size_t i = 0; i < bytesLeft; i++){
            if (!std::isprint(buffer[i])){
                std::cout << '.';
            }
            else {
                std::cout << buffer[i];
        }
    }
        
        std::cout << "\n";

        // add the remaining bytes to offset
        offset += static_cast<unsigned int>(bytesLeft);
    }

    
    return 0;
}
