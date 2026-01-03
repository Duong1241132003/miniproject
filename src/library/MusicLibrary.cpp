#include "MusicLibrary.h"
#include <stdexcept>

void MusicLibrary::addSong(const Song& song)
{
    // Append-only behavior matches the "load once at startup" requirement
    songs.push_back(song);
}

const Song& MusicLibrary::getSongByIndex(size_t index) const
{
    // Defensive check to avoid undefined behavior
    if (index >= songs.size())
    {
        throw std::out_of_range("MusicLibrary: index out of range");
    }

    return songs[index];
}

size_t MusicLibrary::getSongCount() const
{
    return songs.size();
}
