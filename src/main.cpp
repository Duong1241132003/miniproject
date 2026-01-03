#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <windows.h>
#include <mmsystem.h>

#include "Song.h"
#include "MusicLibrary.h"
#include "PlaybackQueue.h"
#include "SmartPlaylist.h"

/*
 * Loads songs from a CSV file into the music library.
 * Expected CSV format:
 * id,title,artist,album,duration,path
 */
void loadLibraryFromCSV(const std::string& filePath, MusicLibrary& library)
{
    std::ifstream file(filePath);

    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open CSV file");
    }

    std::string line;

    // Skip header line
    std::getline(file, line);

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string token;

        Song song;

        // id
        std::getline(ss, token, ',');
        std::cout << "Parsing id = [" << token << "]\n";
        song.id = std::stoi(token);

        // title
        std::getline(ss, song.title, ',');

        // artist
        std::getline(ss, song.artist, ',');

        // album
        std::getline(ss, song.album, ',');

        // duration
        std::getline(ss, token, ',');
        song.duration = std::stoi(token);

        // path (last column)
        std::getline(ss, song.path);

        library.addSong(song);
    }
}

/*
 * Simulates playing a song.
 * This is a placeholder for real audio playback logic.
 */
void playSong(const Song& song)
{
    std::cout << "Now playing:\n";
    std::cout << "  Title   : " << song.title << "\n";
    std::cout << "  Artist  : " << song.artist << "\n";
    std::cout << "  Album   : " << song.album << "\n";
    std::cout << "  Duration: " << song.duration << " s\n";
    std::cout << "  Path    : " << song.path << "\n";
    std::cout << "-----------------------------------\n";
    PlaySoundA(
                    song.path.c_str(),
                    NULL,
                    SND_FILENAME | SND_SYNC
                );
}

// test function for require 1.1, 1.2
int testMusicPlayer();
int testSmartPlaylist();
int main()
{
    testSmartPlaylist();
    return 0;
}

int testMusicPlayer()
{
    try
        {
            MusicLibrary library;
            PlaybackQueue queue;

            // Load music library from CSV
            loadLibraryFromCSV("data/playlist.csv", library);

            std::cout << "Loaded "
                    << library.getSongCount()
                    << " songs into the library.\n\n";

            // Add some songs to the playback queue
            queue.addSong(library.getSongByIndex(0));
            queue.addSong(library.getSongByIndex(3));
            queue.addSong(library.getSongByIndex(5));

            // Simulate playback
            for (int i = 0; i < 3; ++i)
            {
                Song current = queue.getCurrentSong();
                playSong(current);
                queue.playNext();
            }
        }
    catch (const std::exception& ex)
    {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }
    return 0;
}

int testSmartPlaylist()
{
    try
        {
            MusicLibrary library;
            PlaybackQueue queue;

            // Load music library from CSV
            loadLibraryFromCSV("data/playlist.csv", library);

            std::cout << "Loaded "
                    << library.getSongCount()
                    << " songs into the library.\n\n";

            // Add some songs to the playback queue
            addAlbumToQueue("album1", library, queue);

            // Simulate playback
            for (int i = 0; i < 3; ++i)
            {
                Song current = queue.getCurrentSong();
                playSong(current);
                queue.playNext();
            }
        }
    catch (const std::exception& ex)
    {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }
    return 0;
}