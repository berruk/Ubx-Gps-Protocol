/*
* 
* Binary File Operations
*
* @author Berru
*
*/

#ifndef BIN_FILE_H
#define BIN_FILE_H


/*
* open binary with fstream
* @param std::ifstream file
* @param file_name
*/
void open_binary(std::ifstream& binfile, std::string& file_name){

    binfile.open(file_name, std::ios::in | std::ios :: binary);

    if (binfile) std::cout << "File opened " << std::endl;        
    else std::cout << "File can not be opened" << std::endl;
}

/*
* read one byte into char
* @param std::ifstream file
*/
char read_byte(std::ifstream& binfile){
    

    if (binfile.good() && !binfile.eof())
    {
        char buffer;        
        binfile.read(&buffer, 1);
        return buffer;
    }

    else
    {
        std::cout << "Can not read a byte" << std::endl;
        
        return -1;    
    }    

}

/*
* read 2 bytes into uint16_t
* @param std::ifstream file
*/
uint16_t read_2bytes(std::ifstream& binfile){

    if (binfile.good() && !binfile.eof())
    {
        uint16_t read = 0;
        char buffer[2];       
        binfile.read(buffer, 2); //read 2bytes seperately
        return concatbin(buffer[0], buffer[1], sizeof(char)*8); //concat
        
         read;
    }

     else
    {
        std::cout << "Can not read 2 bytes" << std::endl;
        
        return -1;    
    }    

}

/*
* read 4 bytes into uint32_t
* @param std::ifstream file
* @return uint32_t
*/
uint32_t read_4bytes(std::ifstream& binfile){

    if (binfile.good() && !binfile.eof())
    {
        uint16_t read1 = read_2bytes(binfile);
        return concatbin(read1, 
                         read_2bytes(binfile), 
                         sizeof(char)*16);
      
    }

     else
    {
        std::cout << "Can not read 4 bytes" << std::endl;
        
        return -1;    
    }    

}


/*
* LITTLE ENDIAN 
* read 2 bytes into uint16_t 
* @param std::ifstream file
*/
uint16_t read_2bytes_LE(std::ifstream& binfile){

    if (binfile.good() && !binfile.eof())
    {
        uint16_t read = 0;
        char buffer[2];       
        binfile.read(buffer, 2); //read 2bytes seperately
        return concatbin(buffer[1], buffer[0], sizeof(char)*8); //concat
        
         read;
    }

     else
    {
        std::cout << "Can not read 2 bytes" << std::endl;
        
        return -1;    
    }    

}


#endif