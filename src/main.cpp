#include <iostream>
#include <string>
#include <limits>


#include "MusicPlayer.h"
#include "SmartPlaylist.h"

/*
 * Show all features required by the assignment
 */
void printMenu()
{
    std::cout << "\n========== MUSIC PLAYER – FULL DEMO ==========\n";

    // std::cout << "[CORE PLAYBACK]\n";
    std::cout << " 1. Play song by ID\n";
    std::cout << " 2. Play next song\n";
    std::cout << " 3. Play previous song (Back)\n";

    // std::cout << "\n[PLAY NEXT QUEUE - std::queue]\n";
    std::cout << " 4. Add song to Play Next queue\n";

    // std::cout << "\n[PLAYBACK QUEUE - std::list]\n";
    std::cout << " 5. Add all songs from an album to playback queue\n";
    std::cout << " 6. Remove song from playback queue\n";

    // std::cout << "\n[SEARCH & INDEXING]\n";
    std::cout << " 7. Find song by ID\n";
    std::cout << " 8. Find song by title\n";
    std::cout << " 9. List songs by artist\n";
    std::cout << "10. List songs by album\n";

    // std::cout << "\n[SHUFFLE]\n";
    std::cout << "11. Enable shuffle (no repeat)\n";

    // std::cout << "\n[ADVANCED ALGORITHM]\n";
    std::cout << "12. Generate Smart Playlist (BFS)\n";

    // std::cout << "\n[DEBUGGING TOOLS]\n";
    std::cout << "13 List all songs in playback queue\n";
    std::cout << "14 List all songs in playNext queue\n";
    std::cout << "15 List all songs in playback history\n";

    // std::cout << "\n[SYSTEM]\n";
    std::cout << " 0. Pause\n";

    std::cout << "=============================================\n";
    std::cout << "Choose: ";
}

int main()
{
    MusicPlayer player;

    // /*
    //  * Load music library (Part 1 - vector)
    //  */
    // loadLibraryFromCSV("data/playlist.csv", player.getLibrary());

    // /*
    //  * Initialize all indexes (Part 2)
    //  */
    // MusicLibrary& library = player.getLibrary();
    // library.initializeSongByID();
    // library.initializeSongByTitle();
    // library.initializeSongByArtist();
    // library.initializeSongByAlbum();

    // std::cout << "Library loaded and indexed successfully.\n";

    bool running = true;
    while (running)
    {
        printMenu();

        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice)
        {
            /* ================= CORE PLAYBACK ================= */
            case 1:
            {
                int id;
                std::cout << "Song ID: ";
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

            /* ================= PLAY NEXT QUEUE ================= */
            case 4:
            {
                int id;
                std::cout << "Song ID to Play Next: ";
                std::cin >> id;
                player.addSongToPlayNext(id);
                break;
            }

            /* ================= PLAYBACK QUEUE ================= */
            case 5:
            {
               std::string album;
                std::cout << "Album name: ";
                std::getline(std::cin, album);

                addAlbumToQueue(album, player.getLibrary(), player.getPlaybackQueue());
                break;
            }

            case 6:
            {
                int id;
                std::cout << "Song ID to remove from queue: ";
                std::cin >> id;
                player.getPlaybackQueue().removeSongById(id);
                break;
            }

            /* ================= SEARCH & INDEX ================= */
            case 7:
            {
                int id;
                std::cout << "Song ID: ";
                std::cin >> id;

                Song* song = player.getLibrary().findSongByID(id);
                if (song)
                {
                    std::cout << "Found: " << song->id << " | " << song->title << "\n";
                }
                else
                {
                    std::cout << "Song not found.\n";
                }
                break;
            }

            case 8:
            {
                std::string title;
                std::cout << "Song title: ";
                std::getline(std::cin, title);

                Song* song = player.getLibrary().findSongByTitle(title);
                if (song)
                {
                    std::cout << "Found: " << song->id << " | " << song->title << "\n";
                }
                else
                {
                    std::cout << "Song not found.\n";
                }
                break;
            }

            case 9:
            {
                std::string artist;
                std::cout << "Artist name: ";
                std::getline(std::cin, artist);

                auto songs = player.getLibrary().findSongsByArtist(artist);
                std::cout << "Songs by " << artist << ":\n";
                for (auto* s : songs)
                {
                    std::cout << " - " << s->id << " | " << s->title << "\n";
                }
                break;
            }

            case 10:
            {
                std::string album;
                std::cout << "Album name: ";             
                std::getline(std::cin, album);

                std::vector<Song*> s = player.getLibrary().findSongsByAlbum(album);
                std::cout << "Songs in album " << album << ":\n";
                for (auto* s : s)
                {
                    std::cout << " - " << s->title << "\n";
                }
                break;
            }

            /* ================= SHUFFLE ================= */
            case 11:
            {
                player.enableShuffle();
                break;
            }

            /* ================= ADVANCED ================= */
            case 12:
            {
                int startID;
                int maxSize;

                std::cout << "Start song ID: ";
                std::cin >> startID;
                std::cout << "Max playlist size: ";
                std::cin >> maxSize;

                player.BFS(startID, maxSize);
                break;
            }

            /* ================= DEBUGGING TOOLS ================= */
            case 13:
            {
                player.getPlaybackQueue().getAllSongs();
                break;
            }
            case 14:
            {
                player.getPlayNextQueue().getAllSongs();
                break;
            }
            case 15:
            {
                player.getPlaybackHistory().getHistory();
                break;
            }

            /* ================= SYSTEM ================= */
            case 0:
            {
                running = false;
                break;
            }

            default:
            {
                std::cout << "Invalid option.\n";
                break;
            }
        }
    }

    return 0;
}


