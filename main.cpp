#include <conio.h>
#include <iostream>
#include <string>
#include <vector>

struct File
{
    std::string title;
    std::string description;
    bool completed = false;
};

struct Folder
{
    std::string name;
    std::vector<File> Files;
};

// Reads a numbered choice and checks that it refers to an existing item.
bool ReadIndex(const std::string &prompt, std::size_t itemCount, std::size_t &index)
{
    std::cout << prompt << std::endl;
    std::string input;
    std::getline(std::cin, input);

    try
    {
        std::size_t charactersRead = 0;
        // stoull converts typed text to an unsigned number and reports how much
        // input it consumed so trailing characters can be rejected.
        const unsigned long long number = std::stoull(input, &charactersRead);
        // Reject partial conversions, zero (the menu starts at 1), and choices
        // larger than the list before converting to a zero-based vector index.
        if (charactersRead != input.size() || number == 0 || number > itemCount)
        {
            std::cerr << "Please enter a number from 1 to " << itemCount << ".\n";
            return false;
        }
        index = static_cast<std::size_t>(number - 1);
        return true;
    }
    // stoull throws this when the input contains no valid numeric prefix.
    catch (const std::invalid_argument &)
    {
        std::cerr << "Please enter a valid number.\n";
    }
    // stoull throws this when the number cannot fit in an unsigned long long.
    catch (const std::out_of_range &)
    {
        std::cerr << "That number is too large.\n";
    }
    // Both conversion errors leave the output index untouched and report failure.
    return false;
}

// Prints the files in a list with numbers for typed selection.
void ListFiles(const std::vector<File> &files)
{
    for (std::size_t i = 0; i < files.size(); ++i)
    {
        std::cout << i + 1 << ". " << files[i].title << '\n';
    }
}

// Prints folder names, numbers, and the number of files in each folder.
void ListFolders(const std::vector<Folder> &folders)
{
    for (std::size_t i = 0; i < folders.size(); ++i)
    {
        std::cout << i + 1 << ". " << folders[i].name
                  << " (" << folders[i].Files.size() << " files)\n";
    }
}

// Prompts the user to create a folder or a file and chooses where the file goes.
void Add(std::vector<File> &files, std::vector<Folder> &folders)
{
    std::cout << "Type 'folder' or 'file': ";
    std::string choice;
    std::getline(std::cin, choice);

    if (choice == "folder")
    {
        Folder newFolder;
        std::cout << "Enter the new folder name: ";
        std::getline(std::cin, newFolder.name);
        // Do not add a folder that cannot be identified by a name.
        if (newFolder.name.empty())
        {
            std::cout << "Folder name cannot be empty.\n";
            return;
        }
        folders.push_back(newFolder);
        std::cout << "Folder added.\n";
    }
    else if (choice == "file")
    {
        File newFile;
        std::cout << "Enter the new file name: ";
        std::getline(std::cin, newFile.title);
        // A file needs a title; descriptions are allowed to be empty.
        if (newFile.title.empty())
        {
            std::cout << "File name cannot be empty.\n";
            return;
        }
        std::cout << "Add a description: ";
        std::getline(std::cin, newFile.description);

        // Without folders, the only available destination is Home.
        if (folders.empty())
        {
            files.push_back(newFile);
            std::cout << "No folders are available; file added to Home.\n";
            return;
        }

        std::cout << "Put this file in a folder? Type 'yes' or 'no': ";
        std::getline(std::cin, choice);
        // On "yes", ask which existing folder should contain the new file.
        if (choice == "yes")
        {
            ListFolders(folders);
            std::size_t folderIndex;
            // Add only after the folder selection has passed numeric validation.
            if (ReadIndex("Select a folder number:", folders.size(), folderIndex))
            {
                folders[folderIndex].Files.push_back(newFile);
                std::cout << "File added to " << folders[folderIndex].name << ".\n";
            }
        }
        // On "no", keep the file at the top level on Home.
        else if (choice == "no")
        {
            files.push_back(newFile);
            std::cout << "File added to Home.\n";
        }
        else
        {
            std::cout << "Please type 'yes' or 'no'; file was not added.\n";
        }
    }
    else
    {
        std::cout << "Please type 'folder' or 'file'.\n";
    }
}

// Displays a file's name and description.
void ShowFile(const File &file)
{
    std::cout << "\nFile: " << file.title << "\nDescription: "
              << file.description << '\n';
}

// Lets the user select and display a file on Home or inside a folder.
void Read(std::vector<File> &files, std::vector<Folder> &folders)
{
    std::cout << "Type 'file' for a Home file or 'folder' to choose a folder: ";
    std::string choice;
    std::getline(std::cin, choice);

    if (choice == "file")
    {
        // Avoid prompting for a selection when there are no Home files.
        if (files.empty())
        {
            std::cout << "There are no files on Home.\n";
            return;
        }
        ListFiles(files);
        std::size_t fileIndex;
        // Display the file only when the typed number identifies an existing one.
        if (ReadIndex("Select a file number:", files.size(), fileIndex))
        {
            ShowFile(files[fileIndex]);
        }
    }
    else if (choice == "folder")
    {
        // A folder cannot be selected if none have been created.
        if (folders.empty())
        {
            std::cout << "There are no folders.\n";
            return;
        }
        ListFolders(folders);
        std::size_t folderIndex;
        // Stop here if the folder number is invalid; later indexing relies on it.
        if (!ReadIndex("Select a folder number:", folders.size(), folderIndex))
        {
            return;
        }
        // An empty folder has no file that can be displayed.
        if (folders[folderIndex].Files.empty())
        {
            std::cout << "This folder is empty.\n";
            return;
        }
        ListFiles(folders[folderIndex].Files);
        std::size_t fileIndex;
        // Display only the file selected by a valid one-based menu number.
        if (ReadIndex("Select a file number:", folders[folderIndex].Files.size(), fileIndex))
        {
            ShowFile(folders[folderIndex].Files[fileIndex]);
        }
    }
    else
    {
        std::cout << "Please type 'file' or 'folder'.\n";
    }
}

// Lets the user rename a Home file, a folder file, or a folder.
void Edit(std::vector<File> &files, std::vector<Folder> &folders)
{
    std::cout << "Type 'file', 'folder-file', or 'folder' to choose what to edit: ";
    std::string choice;
    std::getline(std::cin, choice);

    if (choice == "file")
    {
        // Each edit path checks its list before showing numbered choices.
        if (files.empty())
        {
            std::cout << "There are no files on Home.\n";
            return;
        }
        ListFiles(files);
        std::size_t fileIndex;
        // Return before indexing the vector if the user's selection is invalid.
        if (!ReadIndex("Select a file number:", files.size(), fileIndex))
        {
            return;
        }
        std::cout << "New file name: ";
        std::getline(std::cin, files[fileIndex].title);
        std::cout << "File updated.\n";
    }
    else if (choice == "folder-file")
    {
        if (folders.empty())
        {
            std::cout << "There are no folders.\n";
            return;
        }
        ListFolders(folders);
        std::size_t folderIndex;
        // Validate the folder first so accessing its files is safe.
        if (!ReadIndex("Select a folder number:", folders.size(), folderIndex))
        {
            return;
        }
        std::vector<File> &folderFiles = folders[folderIndex].Files;
        // Do not offer a file-edit choice for an empty folder.
        if (folderFiles.empty())
        {
            std::cout << "This folder has no files to edit.\n";
            return;
        }
        ListFiles(folderFiles);
        std::size_t fileIndex;
        // Validate the file number before changing the selected file's title.
        if (!ReadIndex("Select a file number:", folderFiles.size(), fileIndex))
        {
            return;
        }
        std::cout << "New file name: ";
        std::getline(std::cin, folderFiles[fileIndex].title);
        std::cout << "File updated.\n";
    }
    else if (choice == "folder")
    {
        if (folders.empty())
        {
            std::cout << "There are no folders.\n";
            return;
        }
        ListFolders(folders);
        std::size_t folderIndex;
        // Validate the folder number before changing that folder's name.
        if (!ReadIndex("Select a folder number:", folders.size(), folderIndex))
        {
            return;
        }
        std::cout << "New folder name: ";
        std::getline(std::cin, folders[folderIndex].name);
        std::cout << "Folder updated.\n";
    }
    else
    {
        std::cout << "Please type 'file', 'folder-file', or 'folder'.\n";
    }
}

// Lets the user select and remove a file from Home.
void Delete(std::vector<File> &files)
{
    // There is nothing to delete when Home has no files.
    if (files.empty())
    {
        std::cout << "There are no files on Home.\n";
        return;
    }
    ListFiles(files);
    std::size_t fileIndex;
    // Erase only after a valid index is returned; invalid input leaves the list unchanged.
    if (ReadIndex("Select a Home file number to delete:", files.size(), fileIndex))
    {
        files.erase(files.begin() + static_cast<std::ptrdiff_t>(fileIndex));
        std::cout << "File deleted.\n";
    }
}

enum class Key
{
    Up,
    Down,
    Enter,
    Escape,
    Other
};

// Reads one keyboard key, including arrow keys, without requiring Enter.
Key ReadKey()
{
    const int key = _getch();
    // Arrow keys arrive as a two-part sequence in the Windows console.
    if (key == 0 || key == 224)
    {
        const int arrow = _getch();
        // Windows reports 72 for Up and 80 for Down.
        if (arrow == 72)
        {
            return Key::Up;
        }
        if (arrow == 80)
        {
            return Key::Down;
        }
    }
    // Map Enter and Esc to named keys used by the browser.
    else if (key == 13)
    {
        return Key::Enter;
    }
    else if (key == 27)
    {
        return Key::Escape;
    }
    return Key::Other;
}

// Clears the console before the keyboard browser redraws its contents.
void ClearScreen()
{
    std::cout << "\x1B[2J\x1B[H";
}

// Shows a keyboard-driven file browser; Enter opens items and Esc goes back.
void Browse(const std::vector<File> &files, const std::vector<Folder> &folders)
{
    bool inFolder = false;
    std::size_t folderIndex = 0;
    std::size_t selected = 0;

    while (true)
    {
        // Inside a folder, the visible list is its files; at Home, folders and
        // Home files share one selection list.
        const std::vector<File> *visibleFiles = inFolder ? &folders[folderIndex].Files : &files;
        const std::size_t itemCount = inFolder ? visibleFiles->size() : folders.size() + files.size();

        ClearScreen();
        // The conditional operator chooses the matching location label.
        if (inFolder)
        {
            std::cout << "Folder: " << folders[folderIndex].name << "\n\n";
        }
        else
        {
            std::cout << "Home\n\n";
        }

        // Avoid drawing a selection when there are no items in the current view.
        if (itemCount == 0)
        {
            std::cout << (inFolder ? "This folder is empty.\n" : "No files or folders yet.\n");
        }
        else if (inFolder)
        {
            for (std::size_t i = 0; i < visibleFiles->size(); ++i)
            {
                // The conditional operator marks the selected row with an arrow.
                std::cout << (i == selected ? "> " : "  ")
                          << visibleFiles->at(i).title << '\n';
            }
        }
        else
        {
            for (std::size_t i = 0; i < folders.size(); ++i)
            {
                std::cout << (i == selected ? "> " : "  ")
                          << "[Folder] " << folders[i].name << " ("
                          << folders[i].Files.size() << " files)\n";
            }
            for (std::size_t i = 0; i < files.size(); ++i)
            {
                const std::size_t itemIndex = folders.size() + i;
                std::cout << (itemIndex == selected ? "> " : "  ")
                          << "[File]   " << files[i].title << '\n';
            }
        }
        std::cout << "\nUse Up/Down to move, Enter to open, Esc to go back or exit.\n";

        const Key key = ReadKey();
        // Esc returns from a folder to Home, or exits the browser from Home.
        if (key == Key::Escape)
        {
            if (!inFolder)
            {
                return;
            }
            inFolder = false;
            selected = folderIndex;
        }
        // Only move the selection when there are items; wrap at either end.
        else if (itemCount > 0 && key == Key::Up)
        {
            selected = selected == 0 ? itemCount - 1 : selected - 1;
        }
        else if (itemCount > 0 && key == Key::Down)
        {
            selected = (selected + 1) % itemCount;
        }
        // Enter opens a folder or displays the selected file; it is ignored
        // for an empty list to prevent invalid access.
        else if (itemCount > 0 && key == Key::Enter)
        {
            if (inFolder)
            {
                ClearScreen();
                ShowFile(visibleFiles->at(selected));
                std::cout << "\nPress any key to return to the folder.";
                ReadKey();
            }
            // Home entries list folders first, so these indices open folders.
            else if (selected < folders.size())
            {
                folderIndex = selected;
                inFolder = true;
                selected = 0;
            }
            else
            {
                ClearScreen();
                ShowFile(files[selected - folders.size()]);
                std::cout << "\nPress any key to return to Home.";
                ReadKey();
            }
        }
    }
}

// Displays the organized Home listing and the available typed commands.
void ShowHome(const std::vector<File> &files, const std::vector<Folder> &folders)
{
    std::cout << "========== File Organizer ==========\n";
    std::cout << "Home\n";
    // Show an empty-state message only when neither kind of item exists.
    if (folders.empty() && files.empty())
    {
        std::cout << "  (empty)\n";
    }
    if (!folders.empty())
    {
        std::cout << "\nFolders\n";
        ListFolders(folders);
    }
    if (!files.empty())
    {
        std::cout << "\nFiles\n";
        ListFiles(files);
    }
    std::cout << "\nCommands: add | browse | read | edit | delete | quit\n";
    std::cout << "Choose an action: ";
}

// Creates the in-memory file system and runs the command menu until quit.
int main()
{
    std::vector<File> files;
    std::vector<Folder> folders;

    bool running = true;
    while (running)
    {
        ShowHome(files, folders);

        std::string choice;
        std::getline(std::cin, choice);
        // Dispatch the typed command to its matching operation.
        if (choice == "add")
        {
            Add(files, folders);
        }
        else if (choice == "browse")
        {
            Browse(files, folders);
        }
        else if (choice == "read")
        {
            Read(files, folders);
        }
        else if (choice == "edit")
        {
            Edit(files, folders);
        }
        else if (choice == "delete")
        {
            Delete(files);
        }
        else if (choice == "quit")
        {
            running = false;
        }
        else
        {
            std::cout << "Unknown command. Type one of the listed commands.\n";
        }
    }

    return 0;
}
