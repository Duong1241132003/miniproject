#include <iostream>
#include <string>
#include <limits>
#include <iomanip> 

#include "MusicPlayer.h"


/*
 * Helper function: Print full song details in a table format
 */
void printSongDetails(const Song* s)
{
    if (!s) return;
    std::cout << "\n------------------------------------------------------------\n";
    std::cout << " ID       : " << s->id << "\n";
    std::cout << " Title    : " << s->title << "\n";
    std::cout << " Artist   : " << s->artist << "\n";
    std::cout << " Album    : " << s->album << "\n";
    std::cout << " Duration : " << s->duration << "s\n";
    std::cout << " Path     : " << s->path << "\n";
    std::cout << "------------------------------------------------------------\n";
}

/*
 * Display the interactive menu
 */
void printMenu()
{
    std::cout << "\n================ MUSIC PLAYER DEMO ================\n";
    
    std::cout << " [ PLAYBACK CONTROLS ]\n";
    std::cout << "  1. Play Song (by ID)       2. Play Next\n";
    std::cout << "  3. Play Previous           4. Exit\n";
    std::cout << "  5. Pause                   6. Resume\n";

    std::cout << "\n [ QUEUE MANAGEMENT ]\n";
    std::cout << "  7. Add Song to Queue (ID)  8. Add Album to Queue\n";
    std::cout << "  9. Add LIBRARY to Queue   10. Remove Song from Queue\n";
    std::cout << " 11. View Queue             12. View History\n";
    std::cout << " 13. Enable Shuffle\n";

    std::cout << "\n [ LIBRARY & SEARCH ]\n";
    std::cout << " 14. Find by ID             15. Find by Title\n";
    std::cout << " 16. Find by Artist         17. Find by Album\n";

    std::cout << "\n [ ADVANCED ]\n";
    std::cout << " 18. Generate Smart Playlist (BFS)\n";

    std::cout << "===================================================\n";
    std::cout << "Select option: ";
}

int main()
{
    MusicPlayer player;

    /*
     * Note: Library loading is handled in the MusicPlayer constructor.
     */
    std::cout << "System initialized. Library loaded.\n";

    bool running = true;
    while (running)
    {
        printMenu();

        int choice;
        // Check for input failure
        if (!(std::cin >> choice))
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        
        // Clear buffer before getline operations
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice)
        {
            /* ================= PLAYBACK CONTROLS ================= */
            case 1: // Play ID
            {
                int id;
                std::cout << "Enter Song ID to play: ";
                std::cin >> id;
                try {
                    player.selectAndPlaySong(id);
                } catch (const std::exception& e) {
                    std::cout << "Error: " << e.what() << "\n";
                }
                break;
            }
            case 2: // Next
                try {
                    player.playNext();
                } catch (const std::exception& e) {
                    std::cout << "Error: " << e.what() << "\n";
                }
                break;
            case 3: // Previous
                try {
                    player.playPrevious();
                } catch (const std::exception& e) {
                    std::cout << "Error: " << e.what() << "\n";
                }
                break;
            case 4: // Stop
                running = false;               
                std::cout << "Exiting Music Player. Goodbye!\n";
                break;
            case 5: // Pause
                pauseSong();
                break;
            case 6: // Resume
                resumeSong();
                break;

            /* ================= QUEUE MANAGEMENT ================= */
            case 7: // Add Single Song to Queue
            {
                int id;
                std::cout << "Enter Song ID to add: ";
                std::cin >> id;
                Song* s = player.getLibrary().findSongByID(id);
                if (s) {
                    player.getPlaybackQueue().addSong(*s);
                    std::cout << "Added '" << s->title << "' to queue.\n";
                } else {
                    std::cout << "Song ID not found.\n";
                }
                break;
            }
            case 8: // Add Album to Queue
            {
                std::string album;
                std::cout << "Enter Album name: ";
                std::getline(std::cin, album);
                // Assuming addAlbumToQueue is a helper function from previous context
                addAlbumToQueue(album, player.getLibrary(), player.getPlaybackQueue());
                break;
            }
            case 9: // Add Entire Library to Queue
            {
                auto& allSongs = player.getLibrary().getSongs();
                if (allSongs.empty()) 
                {
                    std::cout << "Library is empty.\n";
                } 
                else 
                {
                    for (Song& s : allSongs) 
                    {
                        player.getPlaybackQueue().addSong(s);
                    }
                    std::cout << "Added " << allSongs.size() << " songs to queue.\n";
                }
                break;
            }
            case 10: // Remove Song from Queue
            {
                int id;
                std::cout << "Enter Song ID to remove: ";
                std::cin >> id;
                player.getPlaybackQueue().removeSongById(id);
                std::cout << "Removed song with ID " << id << " from queue (if it existed).\n";
                break;
            }
            case 11: // View Queue
                std::cout << "\n--- CURRENT PLAYBACK QUEUE ---\n";
                player.getPlaybackQueue().getAllSongs();
                break;
            case 12: // View History
                std::cout << "\n--- PLAYBACK HISTORY ---\n";
                player.getPlaybackHistory().getHistory();
                break;
            case 13: // Shuffle
                player.enableShuffle();
                break;

            /* ================= SEARCH & LIBRARY ================= */
            case 14: // Find by ID
            {
                int id;
                std::cout << "Enter Song ID: ";
                std::cin >> id;
                Song* s = player.getLibrary().findSongByID(id);
                if(s) printSongDetails(s);
                else std::cout << "Not found.\n";
                break;
            }
            case 15: // Find by Title
            {
                std::string title;
                std::cout << "Enter Title: ";
                std::getline(std::cin, title);
                Song* s = player.getLibrary().findSongByTitle(title);
                if(s) printSongDetails(s);
                else std::cout << "Not found.\n";
                break;
            }
            case 16: // Find by Artist
            {
                std::string artist;
                std::cout << "Enter Artist: ";
                std::getline(std::cin, artist);
                auto songs = player.getLibrary().findSongsByArtist(artist);
                std::cout << "Found " << songs.size() << " songs by " << artist << ":\n";
                for (auto* s : songs) printSongDetails(s);
                break;
            }
            case 17: // Find by Album
            {
                std::string album;
                std::cout << "Enter Album: ";
                std::getline(std::cin, album);
                auto songs = player.getLibrary().findSongsByAlbum(album);
                std::cout << "Found " << songs.size() << " songs in album " << album << ":\n";
                for (auto* s : songs) printSongDetails(s);
                break;
            }

            /* ================= ADVANCED ================= */
            case 18: // BFS Smart Playlist
            {
                int startID, maxSize;
                std::cout << "Start Song ID: ";
                std::cin >> startID;
                std::cout << "Max Playlist Size: ";
                std::cin >> maxSize;
                try {
                    player.BFS(startID, maxSize);
                } catch (const std::exception& e) {
                    std::cout << "BFS Error: " << e.what() << "\n";
                }
                break;
            }

            default:
                std::cout << "Invalid option. Please try again.\n";
                break;
        }
    }

    return 0;
}