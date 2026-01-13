#include <iostream>
#include <string>
#include <limits>
#include <iomanip>

#include "MusicPlayer.h"

/*
 * Print full song details in a readable format
 */
void printSongDetails(const Song* s)
{
    if (!s)
    {
        return;
    }

    std::cout << "\n------------------------------------------------------------\n";
    std::cout << " ID       : " << s->id << "\n";
    std::cout << " Title    : " << s->title << "\n";
    std::cout << " Artist   : " << s->artist << "\n";
    std::cout << " Album    : " << s->album << "\n";
    std::cout << " Duration : " << s->duration << " s\n";
    std::cout << "------------------------------------------------------------\n";
}

/*
 * Display the interactive menu
 */
void printMenu()
{
    std::cout << "\n================ MUSIC PLAYER ================\n";

    std::cout << " [ PLAYBACK CONTROLS ]\n";
    std::cout << "  1. Play Song (by ID)       2. Play Next\n";
    std::cout << "  3. Play Previous           4. Stop\n";
    std::cout << "  5. Pause                   6. Resume\n";

    std::cout << "\n [ QUEUE MANAGEMENT ]\n";
    std::cout << "  7. Add Song to Queue (ID)  8. Add Album to Queue\n";
    std::cout << "  9. Add Library to Queue   10. Remove Song from Queue\n";
    std::cout << " 11. View Queue             12. View History\n";
    std::cout << " 13. Add Song to Play Next  14. View Play Next Queue\n";
    std::cout << " 15. Enable Shuffle         16. Disable Shuffle\n";

    std::cout << "\n [ LIBRARY & SEARCH ]\n";
    std::cout << " 17. Find by ID             18. Find by Title\n";
    std::cout << " 19. Find by Artist         20. Find by Album\n";

    std::cout << "\n [ ADVANCED ]\n";
    std::cout << " 21. Enable Smart Playlist (BFS)\n";
    std::cout << " 22. Disable Smart Playlist (BFS)\n";
    std::cout << " 23. Enable Repeat          24. Disable Repeat\n";
    std::cout << "===================================================\n";
    std::cout << "Select option: ";
}

int main()
{
    /*
     * Create music player instance
     * Library is loaded inside constructor
     */
    MusicPlayer player;

    std::cout << "System initialized. Library loaded.\n";

    bool running = true;

    while (running)
    {
        printMenu();

        int choice;

        /* Validate numeric input */
        if (!(std::cin >> choice))
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        /* Clear input buffer */
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice)
        {
            /*================= PLAYBACK CONTROLS =================*/

            case 1:
            {
                int id;

                std::cout << "Enter Song ID to play: ";
                std::cin >> id;

                player.selectAndPlaySong(id);
                break;
            }

            case 2:
            {
                player.playNext();
                break;
            }

            case 3:
            {
                player.playPrevious();
                break;
            }

            case 4:
            {
                std::cout << "Exiting Music Player. Goodbye!\n";
                running = false;
                break;
            }

            case 5:
            {
                pauseSong();
                break;
            }

            case 6:
            {
                resumeSong();
                break;
            }

            /*================= QUEUE MANAGEMENT =================*/

            case 7:
            {
                int id;

                std::cout << "Enter Song ID to add: ";
                std::cin >> id;

                Song* s = player.getLibrary().findSongByID(id);

                if (s)
                {
                    player.getPlaybackQueue().addSong(*s);
                    std::cout << "Added '" << s->title << "' to queue.\n";
                }
                else
                {
                    std::cout << "Song ID not found.\n";
                }

                break;
            }

            case 8:
            {
                std::string album;

                std::cout << "Enter Album name: ";
                std::getline(std::cin, album);

                addAlbumToQueue(album, player.getLibrary(),player.getPlaybackQueue());
                break;
            }

            case 9:
            {
                std::vector<Song>& allSongs = player.getLibrary().getSongs();

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

            case 10:
            {
                int id;

                std::cout << "Enter Song ID to remove: ";
                std::cin >> id;

                player.getPlaybackQueue().removeSongById(id);
                std::cout << "Remove request processed.\n";
                break;
            }

            case 11:
            {
                std::cout << "\n--- CURRENT PLAYBACK QUEUE ---\n";
                player.getPlaybackQueue().printAllSongs();
                break;
            }

            case 12:
            {
                std::cout << "\n--- PLAYBACK HISTORY ---\n";
                player.getPlaybackHistory().printHistory();
                break;
            }

           case 13:
            {
                int id;

                std::cout << "Enter Song ID to add to Play Next: ";
                std::cin >> id;

                player.addSongToPlayNext(id);
                break;
            }


            case 14:
            {
                player.printPlayNextQueue();
                break;
            }

            case 15:
            {
                player.enableShuffle();
                break;
            }

            case 16:
            {
                player.disableShuffle();
                break;
            }

            /*================= SEARCH & LIBRARY =================*/

            case 17:
            {
                int id;

                std::cout << "Enter Song ID: ";
                std::cin >> id;

                Song* s = player.getLibrary().findSongByID(id);

                if (s)
                {
                    printSongDetails(s);
                }
                else
                {
                    std::cout << "Song not found.\n";
                }

                break;
            }

            case 18:
            {
                std::string title;

                std::cout << "Enter Title: ";
                std::getline(std::cin, title);

                Song* s = player.getLibrary().findSongByTitle(title);

                if (s)
                {
                    printSongDetails(s);
                }
                else
                {
                    std::cout << "Song not found.\n";
                }

                break;
            }

            case 19:
            {
                std::string artist;

                std::cout << "Enter Artist: ";
                std::getline(std::cin, artist);

                std::vector<Song*> songs = player.getLibrary().findSongsByArtist(artist);

                std::cout << "Found " << songs.size() << " songs by " << artist << "\n";

                for (Song* s : songs)
                {
                    printSongDetails(s);
                }

                break;
            }

            case 20:
            {
                std::string album;

                std::cout << "Enter Album: ";
                std::getline(std::cin, album);

                std::vector<Song*> songs = player.getLibrary().findSongsByAlbum(album);

                std::cout << "Found " << songs.size() << " songs in album " << album << "\n";

                for (Song* s : songs)
                {
                    printSongDetails(s);
                }

                break;
            }


            /*================= ADVANCED =================*/

            case 21:
            {
                int startID;
                int maxSize;

                std::cout << "Start Song ID: ";
                std::cin >> startID;

                std::cout << "Max Playlist Size: ";
                std::cin >> maxSize;

                player.enableSmartPlaylist(startID, maxSize);
                break;
            }

            case 22:
            {
                player.disableSmartPlaylist();
                break;
            }

            case 23:
            {
                player.enableRepeat();
                break;
            }

            case 24:
            {
                player.disableRepeat();
                break;
            }

            default:
            {
                std::cout << "Invalid option. Please try again.\n";
                break;
            }
        }
    }

    return 0;
}
