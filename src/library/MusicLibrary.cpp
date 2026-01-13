#include "MusicLibrary.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

void MusicLibrary::loadLibraryFromCSV(const std::string& filePath)
{
    std::ifstream file(filePath);

    /* Fail immediately if the file cannot be accessed */
    if (!file.is_open())
    {
        throw std::runtime_error("[IO Error] Unable to open file: " + filePath);
    }

    std::string line;
    /* Skip the CSV header row */
    std::getline(file, line);

    int lineNumber = 1;

    while (std::getline(file, line))
    {
        lineNumber++;
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string token;
        Song song;

        try 
        {          
            /* Parse ID from first column */
            std::getline(ss, token, ',');
            song.id = std::stoi(token);

            /* Parse title, artist and album strings */
            std::getline(ss, song.title, ',');
            std::getline(ss, song.artist, ',');
            std::getline(ss, song.album, ',');

            /* Parse duration from numeric column */
            std::getline(ss, token, ',');
            song.duration = std::stoi(token);

            /* Parse file path from the last column */
            std::getline(ss, song.path);

            addSong(song);
        }
        catch (const std::exception& e)
        {
            /* Wrap parser error with line context and re-throw */
            throw std::runtime_error("CSV Format Error [Line " + std::to_string(lineNumber) + "]: " + e.what());
        }
    }

    /* Build lookup indexes after loading */
    initializeSongByID();
    initializeSongByTitle();
    initializeSongByArtist();
    initializeSongByAlbum();
}

void MusicLibrary::addSong(const Song& song)
{
    /* Store the song in the main container */
    songs.push_back(song);
}

const Song& MusicLibrary::getSongByIndex(size_t index) const
{
    /* Notify if index is out of range */
    if (index >= songs.size())
    {
        std::cout << "MusicLibrary: index out of range";
    }

    return songs[index];
}

Song* MusicLibrary::findSongByID(int id)
{
    /* Locate a single song by its unique ID */
    auto it = songByID.find(id);

    if (it == songByID.end())
    {
        return nullptr;
    }

    return it->second;
}

Song* MusicLibrary::findSongByTitle(const std::string& title)
{
    /* Locate a single song by its title */
    auto it = songByTitle.find(title);

    if (it == songByTitle.end())
    {
        return nullptr;
    }

    return it->second;
}

std::vector<Song*> MusicLibrary::findSongsByArtist(const std::string& artist)
{
    /* Retrieve list of songs associated with an artist */
    auto it = songByArtist.find(artist);

    if (it == songByArtist.end())
    {
        return {};
    }

    return it->second;
}

std::vector<Song*> MusicLibrary::findSongsByAlbum(const std::string& album)
{
    /* Retrieve list of songs associated with an album */
    auto it = songByAlbum.find(album);

    if (it == songByAlbum.end())
    {
        return {};
    }

    return it->second;
}

size_t MusicLibrary::getSongCount() const
{
    /* Return current size of the song collection */
    return songs.size();
}

void MusicLibrary::initializeSongByID()
{
    /* Map IDs to song references */
    songByID.clear();
    for (Song& song : songs)
    {
        songByID[song.id] = &song;
    }
}

void MusicLibrary::initializeSongByTitle()
{
    /* Map titles to song references */
    songByTitle.clear();

    for (Song& song : songs)
    {
        songByTitle[song.title] = &song;
    }
}

void MusicLibrary::initializeSongByArtist()
{
    /* Group song references by artist name */
    songByArtist.clear();

    for (Song& song : songs)
    {
        songByArtist[song.artist].push_back(&song);
    }
}

void MusicLibrary::initializeSongByAlbum()
{
    /* Group song references by album name */
    songByAlbum.clear();

    for (Song& song : songs)
    {
        songByAlbum[song.album].push_back(&song);
    }
}

std::vector<Song>& MusicLibrary::getSongs()
{
    /* Provide direct access to the songs vector */
    return songs;
}