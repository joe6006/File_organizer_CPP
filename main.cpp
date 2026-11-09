#include <iostream>
#include <string>
#include <vector>

struct File
{
    // store title
    std::string title;
    std::string description;
    bool completed = false;
};
struct Folder
{
    std::string name;
    std::vector<File> Files;
};
// use refrence to refer to the vector in the main function instead of creating a copy

void Add(std::vector<File> &Files, std::vector<Folder> &Folders)
{
    Folder newFolders;
    std::string input;
    std::cout << "Type 'folder' or 'file'" << std::endl;
    std::getline(std::cin, input);
    if (input == "folder")
    {

        std::cout << "enter new folder name" << std::endl;
        std::getline(std::cin, newFolders.name);
        Folders.push_back(newFolders);
    }
    else if (input == "file")
    {
        File newFiles;
        std::cout << "enter new File" << std::endl;
        std::getline(std::cin, newFiles.title);
        std::cout << "Add a description" << std::endl;
        std::getline(std::cin, newFiles.description);
        std::cout << "Would you like to put this in an existing folder? Type 'yes' or 'no'." << std::endl;
        std::string choice;
        std::getline(std::cin, choice);

        if (Folders.empty())
        {
            std::cout << "no Folders available: the file will be place in the homescreen." << std::endl;
            Files.push_back(newFiles);
        }
        else if (choice == "yes")
        {
            std::cout << "select which folder by number" << std::endl;
            for (std::size_t i = 0; i < Folders.size(); ++i)
            {
                std::cout << i + 1 << ". " << Folders[i].name << std::endl;
            };
            int num;
            std::cin >> num;
            num--;
            Folders[num].Files.push_back(newFiles);
        }
        else if (choice == "no")
        {

            Files.push_back(newFiles);
        }
    }
};

void Read(std::vector<File> &Files, std::vector<Folder> &Folders)
{
    std::cout << "type 'folder' or 'file' to choose what to display" << std::endl;
    std::string choice;
    std::getline(std::cin, choice);

    if (choice == "file")
    {
        std::cout << "select the file number you'd like to read" << std::endl;
        for (std::size_t i = 0; i < Files.size(); ++i)
        {

            std::cout << i + 1 << "." << Files[i].title << std::endl;
        };
        std::string input;
        std::getline(std::cin, input);
        int num;
        try
        {
            num = std::stoi(input);
            std::cout << num << std::endl;
        }
        catch (const std::invalid_argument &e)
        {
            std::cerr << "Invalid input: Not a number. \n";
            return;
        }
        catch (const std::out_of_range &e)
        {
            std::cerr << "overflow: number is too large for an int. \n";
            return;
        };
        num--;
        std::cout << "File Name: " << Files[num].title << std::endl;
        std::cout << "Description: " << std::endl;
        std::cout << Files[num].description << std::endl;
    }
    else if (choice == "folder")
    {
        std::cout << "select the folder number you'd like to open" << std::endl;
        for (std::size_t i = 0; i < Folders.size(); ++i)
        {
            std::cout << i + 1 << ". " << Folders[i].name << std::endl;
        };
        std::string input;
        std::getline(std::cin, input);
        int num;
        try
        {
            num = std::stoi(input);
        }
        catch (const std::invalid_argument &e)
        {
            std::cerr << "Invalid input: Not a number. \n";
            return;
        }
        catch (const std::out_of_range &e)
        {
            std::cerr << "overflow: number is too large for an int. \n";
            return;
        };
        num--;
        if (num >= 0 && num < Folders.size())
        {
            for (int i = 0; i < Folders[num].Files.size(); ++i)
            {
                std::cout << i + 1 << ". "
                          << Folders[num].Files[i].title
                          << std::endl;
            }
            std::cout << "select a file to read" << std::endl;
            std::string input;
            std::getline(std::cin, input);
            int fileNum = std::stoi(input);
            fileNum--;
            std::cout << Folders[num].Files[fileNum].title << std::endl;
            std::cout << Folders[num].Files[fileNum].description << std::endl;
        }
    }
}
// edit the information in Files title
void Edit(std::vector<File> &Files, std::vector<Folder> &Folders)
{
    // using a try and catch to verify the input string is the correct size and type
    // this also catches any errors on input if there are
    // any extra characters
    // we use & to create to refer to only the item that it is checking instead of creating a copy
    // if you use it without a "&" it creates what you call a memory slice which it creates
    // its own copy instead of refer to  the object at hand
    // this also means that when using "&" we can save space and make it slightly faster.

    // PSA:: intialize the int or whatever type outside the TRY or else it won't be global

    std::cout << "type 'folder' or 'file' to choose what to display" << std::endl;
    std::string choice;
    std::getline(std::cin, choice);

   if(choice != "folder" || choice != "file"){
    bool valid = false;
    while(valid){
        std::cout<<"please type file or folder, input will repeat if not typed correctly"<<std::endl;
        std::getline(std::cin, choice);
        if(choice == "file" || choice == "folder"){
            valid = true;
        };
    }

   }else if (choice == "file")
    {
         for (std::size_t i = 0; i < Files.size(); ++i)
        {

            std::cout << i + 1 << "." << Files[i].title << std::endl;
        };
        std::cout << "Select which File to edit. use #'s EX: 1, 2, 3..." << std::endl;
        std::string input;
        std::getline(std::cin, input);

        int num;

        try
        {
            num = std::stoi(input);
            std::cout << num << std::endl;
        }
        catch (const std::invalid_argument &e)
        {
            std::cerr << "Invalid input: Not a number. \n";
            return;
        }
        catch (const std::out_of_range &e)
        {
            std::cerr << "overflow: number is too large for an int. \n";
            return;
        };

        num--;

        std::cout << "current Files title:" << std::endl;
        std::cout << Files[num].title << std::endl;
        std::cout << "New title: " << std::endl;
        std::getline(std::cin, Files[num].title);
        std::cout << "Success" << std::endl;
    } else if(choice=="folder"){
       for (std::size_t i = 0; i < Folders.size(); ++i)
        {
            std::cout << i + 1 << ". " << Folders[i].name << std::endl;
        };
        std::string input;
        std::getline(std::cin, input);
        int num;
        try
        {
            num = std::stoi(input);
        }
        catch (const std::invalid_argument &e)
        {
            std::cerr << "Invalid input: Not a number. \n";
            return;
        }
        catch (const std::out_of_range &e)
        {
            std::cerr << "overflow: number is too large for an int. \n";
            return;
        };
        num--;
        std::cout << "current Folder title:" << std::endl;
        std::cout << Folders[num].name<< std::endl;
        std::cout << "New title: " << std::endl;
        std::getline(std::cin, Folders[num].name);
        std::cout << "Success!" << std::endl;

    }
};

void Delete(std::vector<File> &Files)
{
    // same concept as edit but we use .erase to delete the users
    // wanted title
    std::cout << "Select which File to delete. use #'s EX: 1, 2, 3..." << std::endl;
    std::string input;
    std::getline(std::cin, input);

    int num;

    try
    {
        num = std::stoi(input);
        std::cout << num << std::endl;
    }
    catch (const std::invalid_argument &e)
    {
        std::cerr << "Invalid input: Not a number. \n";
        return;
    }
    catch (const std::out_of_range &e)
    {
        std::cerr << "overflow: number is too large for an int. \n";
        return;
    };
    num--;

    Files.erase(Files.begin() + num);
};

int main()
{
    // create a vector to hold multiple File and its resizable

    std::vector<File> Files;
    std::vector<Folder> Folders;
    // File initiates the struct to create a variable called example
    // use  title to call the string title

    bool running = true;
    while (running)
    {
        std::cout << "\n";

        if (!Folders.empty())
        {
            for (std::size_t i = 0; i < Folders.size(); ++i)
            {
                std::cout << "- " << Folders[i].name << std::endl;
            }
        }
        else if (!Files.empty())
        {

            for (std::size_t i = 0; i < Files.size(); ++i)
            {

                std::cout << i + 1 << "." << Files[i].title << std::endl;
            };
        }
        std::cout << "\n";
        std::cout << "File system" << std::endl;

        std::cout << "To add a Folder or File, type 'add'" << std::endl;
        std::cout << "To edit a File, type 'edit'" << std::endl;
        std::cout << "To read a file, type 'read" << std::endl;
        std::cout << "To delete a File, type 'delete'" << std::endl;
        std::cout << "To quit, type 'quit'" << std::endl;

        std::string choice;
        std::getline(std::cin, choice);

        if (choice == "add")
        {
            Add(Files, Folders);
        }
        else if (choice == "read")
        {
            Read(Files, Folders);
        }
        else if (choice == "edit")
        {
            Edit(Files, Folders);
        }
        else if (choice == "quit")
        {
            running = false;
        }
        else if (choice == "delete")
        {
            Delete(Files);
        }
    }

    return 0;
}