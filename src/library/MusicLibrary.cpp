#include "MusicLibrary.h"
#include <stdexcept>

void MusicLibrary::addSong(const Song& song)
{
    // Store the song in the main container
    songs.push_back(song);
}

const Song& MusicLibrary::getSongByIndex(size_t index) const
{
    if (index >= songs.size())
    {
        throw std::out_of_range("MusicLibrary: index out of range");
    }

    return songs[index];
}

Song* MusicLibrary::findSongByID(int id)
{
    auto it = songByID.find(id);

    if (it == songByID.end())
    {
        return nullptr;
    }

    return it->second;
}

Song* MusicLibrary::findSongByTitle(const std::string& title)
{
    auto it = songByTitle.find(title);

    if (it == songByTitle.end())
    {
        return nullptr;
    }

    return it->second;
}

std::vector<Song*> MusicLibrary::findSongsByArtist(const std::string& artist)
{
    auto it = songByArtist.find(artist);

    if (it == songByArtist.end())
    {
        return {};
    }

    return it->second;
}

size_t MusicLibrary::getSongCount() const
{
    return songs.size();
}

void MusicLibrary::initializeSongByID()
{
    songByID.clear();
    for (Song& song : songs)
    {
        songByID[song.id] = &song;
    }
}

void MusicLibrary::initializeSongByTitle()
{
    songByTitle.clear();

    for (Song& song : songs)
    {
        songByTitle[song.title] = &song;
    }
}

void MusicLibrary::initializeSongByArtist()
{
    songByArtist.clear();

    for (Song& song : songs)
    {
        songByArtist[song.artist].push_back(&song);
    }
}