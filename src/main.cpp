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
#include "PlayNextQueue.h"
#include "ShuffleManager.h"
#include "PlaybackHistory.h"
#include "MusicPlayer.h"

// /*
//  * Loads songs from a CSV file into the music library.
//  * Expected CSV format
//  * id,title,artist,album,duration,path
//  */
// void loadLibraryFromCSV(const std::string& filePath, MusicLibrary& library)
// {
//     std::ifstream file(filePath);

//     if (!file.is_open())
//     {
//         throw std::runtime_error("Failed to open CSV file");
//     }

//     std::string line;

//     // Skip header line
//     std::getline(file, line);

//     while (std::getline(file, line))
//     {
//         std::stringstream ss(line);
//         std::string token;

//         Song song;

//         // id
//         std::getline(ss, token, ',');
//         std::cout << "Parsing id = [" << token << "]\n";
//         song.id = std::stoi(token);

//         // title
//         std::getline(ss, song.title, ',');

//         // artist
//         std::getline(ss, song.artist, ',');

//         // album
//         std::getline(ss, song.album, ',');

//         // duration
//         std::getline(ss, token, ',');
//         song.duration = std::stoi(token);

//         // path (last column)
//         std::getline(ss, song.path);

//         library.addSong(song);
//     }
    
//     // Create an index entry pointing to the stored song
//     library.initializeSongByID();
//     library.initializeSongByTitle();
//     library.initializeSongByArtist();
// }

/*
 * Simulates playing a song.
 * This is a placeholder for real audio playback logic.
 */
// void playSong(const Song& song)
// {
//     std::cout << "Now playing:\n";
//     std::cout << "  ID      : " << song.id << "\n";
//     std::cout << "  Title   : " << song.title << "\n";
//     std::cout << "  Artist  : " << song.artist << "\n";
//     std::cout << "  Album   : " << song.album << "\n";
//     std::cout << "  Duration: " << song.duration << " s\n";
//     std::cout << "  Path    : " << song.path << "\n";
//     std::cout << "-----------------------------------\n";
//     PlaySoundA(
//                     song.path.c_str(),
//                     NULL,
//                     SND_FILENAME | SND_SYNC
//                 );
// }

// test function for require 1.1, 1.2
int testMusicLibrary();
int testSmartPlaylist();
void testFindSongByID(MusicLibrary& library, int songID);
void testFindSongByTitle(MusicLibrary& library, const std::string& title);
void testFindSongByArtist(MusicLibrary& library, const std::string& artist);
void testPlayNextQueue(MusicLibrary& library);
void testShuffleManager(MusicLibrary& library);
void testPlaybackHistory(MusicLibrary& library);
void testMusicPlayer(MusicPlayer& player);

MusicLibrary library;
PlaybackQueue queue;


int main()
{
    MusicPlayer player;
    // Load music library from CSV
    // loadLibraryFromCSV("data/playlist.csv", library);

    // std::cout << "Loaded "
    //         << library.getSongCount()
    //         << " songs into the library.\n\n";
    // player.library = library;
    testMusicPlayer(player);
    while(1)
    {
        player.playNext();
    }
    return 0;
}

int testMusicLibrary()
{
    try
        {
            // Add some songs to the playback queue
            queue.addSong(library.getSongByIndex(1));
            queue.addSong(library.getSongByIndex(4));
            queue.addSong(library.getSongByIndex(8));

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
            // Add some songs to the playback queue
            addAlbumToQueue("album3", library, queue);

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

void testFindSongByID(MusicLibrary& library, int songID)
{
    Song* song = library.findSongByID(songID);

    if (song != nullptr)
    {
        std::cout << "Found song: " << song->title << std::endl;
    }
    else
    {
        std::cout << "Song not found\n";
    }

}

void testFindSongByTitle(MusicLibrary& library, const std::string& title)
{
    Song* song = library.findSongByTitle(title);

    if (song != nullptr)
    {
        std::cout << "Found song: " << song->id << std::endl;
    }
    else
    {
        std::cout << "Song not found\n";
    }

}

void testFindSongByArtist(MusicLibrary& library, const std::string& artist)
{
    std::vector<Song*> songs = library.findSongsByArtist(artist);

    if (!songs.empty())
    {
        std::cout << "Found songs by artist: " << artist << std::endl;
        for (const Song* song : songs)
        {
            playSong(*song);
        }
    }
    else
    {
        std::cout << "No songs found for artist: " << artist << std::endl;
    }
}

void testPlayNextQueue(MusicLibrary& library)
{
    PlayNextQueue playNextQueue;

    // Add songs to PlayNextQueue
    playNextQueue.addSong(library.getSongByIndex(1));
    playNextQueue.addSong(library.getSongByIndex(4));
    playNextQueue.addSong(library.getSongByIndex(6));

    // Simulate playing songs from PlayNextQueue
    while (!playNextQueue.isEmpty())
    {
        Song nextSong = playNextQueue.playNext();
        playSong(nextSong);
    }
}
void testShuffleManager(MusicLibrary& library)
{
    ShuffleManager shuffleManager;

    // Initialize shuffle with all songs in the library
    std::vector<Song*> allSongs;
    for (size_t i = 0; i < library.getSongCount(); ++i)
    {
        allSongs.push_back(const_cast<Song*>(&library.getSongByIndex(i)));
    }
    shuffleManager.initialize(allSongs);

    // Simulate playing shuffled songs
    for (size_t i = 0; i < library.getSongCount(); ++i)
    {
        Song* nextSong = shuffleManager.getNextSong();
        if (nextSong != nullptr)
        {
            playSong(*nextSong);
        }
    }
}
void testPlaybackHistory(MusicLibrary& library)
{
    PlaybackHistory playbackHistory;

    // Simulate playing some songs and recording history
    for (size_t i = 0; i < 10; i+=2)
    {
        Song currentSong = library.getSongByIndex(i);
        playSong(currentSong);
        playbackHistory.pushSong(currentSong);
    }

    // Simulate going back in playback history
    while (!playbackHistory.isEmpty())
    {
        Song previousSong = playbackHistory.playPreviousSong();
        std::cout << "Replaying previous song from history:\n";
        playSong(previousSong);
    }
}

void testMusicPlayer(MusicPlayer& player)
{
    player.selectAndPlaySong(3);
    player.selectAndPlaySong(5);

    player.addSongToPlayNext(*(player.getLibrary().findSongByID(1)));
    player.addSongToPlayNext(*(player.getLibrary().findSongByID(7)));

}