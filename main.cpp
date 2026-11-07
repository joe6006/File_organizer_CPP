#include <iostream>
#include <string>
#include <vector>

struct File{
    //store title
    
    std::string title;
    std::string description;
    bool completed =false;

};
//use refrence to refer to the vector in the main function instead of creating a copy
void Add( std::vector <File>& Files){
        File newFiles;
        
        std::cout<< "enter new File"<<std::endl;
        std::getline(std::cin,newFiles.title);
        std::cout<< "Add a description";
        std::getline(std::cin, newFiles.description);
        Files.push_back(newFiles);
};  
void Read(std::vector <File>& Files){
    std::cout<< "select the file number you'd like to read"<<std::endl;
    std::string input;
    std::getline(std::cin, input);
    int num;
    try{
            num =std::stoi(input);
            std::cout<<num<<std::endl;
        }
        catch(const std::invalid_argument& e){
            std::cerr << "Invalid input: Not a number. \n";
            return;

        }
        catch(const std::out_of_range& e){
            std::cerr << "overflow: number is too large for an int. \n";
            return;
        };
    num--;
    std::cout<<Files[num].title<<std::endl;
    std::cout<<Files[num].description<<std::endl;

}
//edit the information in Files title
void Edit( std::vector <File>& Files){
   
        std::cout<< "Select which File to edit. use #'s EX: 1, 2, 3..."<<std::endl;
        std::string input;
        std::getline(std::cin, input);

        //using a try and catch to verify the input string is the correct size and type
        //this also catches any errors on input if there are
        //any extra characters
        //we use & to create to refer to only the item that it is checking instead of creating a copy 
        //if you use it without a "&" it creates what you call a memory slice which it creates
        //its own copy instead of refer to  the object at hand
        //this also means that when using "&" we can save space and make it slightly faster. 

        //PSA:: intialize the int or whatever type outside the TRY or else it won't be global
            int num;

        try{
            num =std::stoi(input);
            std::cout<<num<<std::endl;
        }
        catch(const std::invalid_argument& e){
            std::cerr << "Invalid input: Not a number. \n";
            return;

        }
        catch(const std::out_of_range& e){
            std::cerr << "overflow: number is too large for an int. \n";
            return;
        };


        num--;

        
        std::cout<<"current Files title:"<<std::endl;
        std::cout<<Files[num].title<<std::endl;
        std::cout<<"New title: "<<std::endl;
        std::getline(std::cin, Files[num].title);
        std::cout<<"Success"<<std::endl;
         

};  

void Delete(std::vector <File>& Files){
    //same concept as edit but we use .erase to delete the users
    //wanted title
         std::cout<< "Select which File to delete. use #'s EX: 1, 2, 3..."<<std::endl;
        std::string input;
        std::getline(std::cin, input);

     
            int num;

        try{
            num =std::stoi(input);
            std::cout<<num<<std::endl;
        }
        catch(const std::invalid_argument& e){
            std::cerr << "Invalid input: Not a number. \n";
            return;

        }
        catch(const std::out_of_range& e){
            std::cerr << "overflow: number is too large for an int. \n";
            return;
        };      
        num--;

        Files.erase(Files.begin() + num);


};



int main(){
    //create a vector to hold multiple File and its resizable

    std::vector<File> Files;

    //File initiates the struct to create a variable called example
    //use  title to call the string title

    bool running = true;
    while(running){
        std::cout<<"\n";
        for(std::size_t i = 0; i < Files.size(); ++i){
            
                std::cout<< i+1 <<"." << Files[i].title<<std::endl;
                
            }; 
        std::cout<<"\n";
        std::cout<<"To-Do List menu\n";
       
        std::cout<<"To add a File, type 'add'"<<std::endl;
        std::cout<<"To edit a File, type 'edit'"<<std::endl;
        std::cout<<"To delete a File, type 'delete'"<<std::endl;
        std::cout<<"To quit, type 'quit'"<<std::endl;

        std::string choice;
        std::getline(std::cin, choice);

        if(choice == "add"){                
                 Add(Files);
            }else if(choice == "read"){
                Read(Files);
            }else if(choice == "edit"){
                Edit(Files);
            }else if(choice == "quit"){
            running = false;
        }else if( choice == "delete"){
            Delete(Files);
        }

        
                
    }
    
    
    return 0;
}