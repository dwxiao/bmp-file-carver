#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>

using namespace std;

string extractFileName(string input) {
    string filename;
    int len = (int)input.length();
    int start = 0, end = len;
    
    while (--len >= 0) {
        if (input[len] == '/') {
            start = len + 1;
            break;
        }
        if (input[len] == '.') {
            end = len;
        }
    }
    
    filename = input.substr(start, end - start);
    
    return filename;
}

int getFileSize(ifstream& ifs) {
    streampos fsize = ifs.tellg();
    ifs.seekg(0, std::ios::end);
    fsize = ifs.tellg() - fsize;
    ifs.seekg(0);
    
    return (int)fsize;
}

void writeFile(ifstream &ifs, string path, int size, int offset) {
    ifs.seekg(offset); // reset position back to the beginning of the block to prepare for writing out
    ofstream ofs(path.c_str());
    char *out_buffer = new char[size];
    ifs.read(out_buffer, size);
    ofs.write(out_buffer, size);
    delete[] out_buffer;
    ofs.close();
    ifs.seekg(offset + 1); // reset position back to place where it was after reading in the first byte of the buffer
}


void printFileInfo(int offset, int size, string path) {
    cout << "A file was detected at offset " << offset;
    cout << " with a size of " << size;
    cout << ". The path of the file is " << path << endl;
}

int main() {
    ifstream ifs;
    string input;
    
    cout << "Please enter a .bmp file path: ";
    cin >> input;
    
    ifs.open(input, ifstream::in);
    if (ifs.is_open()) {
        // extract the file name from the input path
        string filename = extractFileName(input);
        string dirname = filename + "_Output";
        
        // make the directory to hold all the output files
        int dir = mkdir(dirname.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        if (dir == -1)
        {
            cout << "Error creating directory." << endl;
            exit(1);
        }
        
        // get the file size
        int filesize = getFileSize(ifs);
        
        // start scanning the file byte by byte
        char c;
        int bmp_counter = 0;
        int non_bmp_counter = 0;
        while (ifs.get(c)) {
            if ((c == 'B' && ifs.peek() == 'M') || (int)ifs.tellg() == filesize) {
                if (non_bmp_counter > 0) { // reached the end of a non-bmp file, save and output information related to the non-bmp file
                    // get and store the offset
                    int offset = (int)ifs.tellg() - non_bmp_counter;
                    
                    // get and store the file size
                    int size = non_bmp_counter;
                    
                    // save path to a string variable
                    string path = dirname + "/" + to_string(offset) + ".unknown";
                    
                    // save file to <inputFileName>_Output as <offset>.unknown
                    writeFile(ifs, path, size, offset);
                    ifs.seekg(offset + size); // since the non-bmp block is detected at the end of reading the block, we must reset it back to that position after writing it out
                    
                    // print information to standard out
                    printFileInfo(offset, size, path);
                    
                    // reset the counter
                    non_bmp_counter = 0;
                }
                else if (c == 'B' && ifs.peek() == 'M') { // found a BMP file, save and output information related to bmp file
                    // get and store the offset
                    int offset = (int)ifs.tellg() - 1;
                    
                    // get and store the file size
                    char size_buffer[5]; // use 5 to compensate for the face that get() already read one byte
                    ifs.read(size_buffer, 5);
                    int size = *(int*)&size_buffer[1];
                    //ifs.seekg(offset); // reset position back to start of the block to prepare for writing out to file
                    bmp_counter = std::max(bmp_counter, size); // don't override bmp_counter if you detect a bmp inside a bmp
                    
                    // save path to a string variable;
                    string path = dirname + "/" + to_string(offset) + ".bmp";
                    
                    // save file to <inputFileName>_Output as <offset>.bmp
                    writeFile(ifs, path, size, offset);
                    
                    // print information to standard out
                    printFileInfo(offset, size, path);
                }
            }
            
            if (bmp_counter > 0) {
                bmp_counter--;
            }
            
            if (bmp_counter == 0 && !(c == 'B' && ifs.peek() == 'M')) {
                non_bmp_counter++;
            }
        }
    }
    else {
        cout << "Can't open file." << endl;
    }
    
    ifs.close();
    
    return 0;
}

