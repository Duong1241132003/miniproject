#include "ShuffleManager.h"
#include <algorithm>
#include <random>

void ShuffleManager::initialize(const std::vector<Song*>& playlist)
{
    shuffledSongs = playlist;
    playedSongIDs.clear();
    currentIndex = 0;

    std::random_device rd;
    std::mt19937 gen(rd());

    std::shuffle(shuffledSongs.begin(), shuffledSongs.end(), gen);
}

Song* ShuffleManager::getNextSong()
{
    if (shuffledSongs.empty())
    {
        return nullptr;
    }

    while (currentIndex < shuffledSongs.size())
    {
        Song* song = shuffledSongs[currentIndex];
        ++currentIndex;

        if (playedSongIDs.find(song->id) == playedSongIDs.end())
        {
            playedSongIDs.insert(song->id);
            return song;
        }
    }

    resetCycle();
    return getNextSong();
}

void ShuffleManager::resetCycle()
{
    playedSongIDs.clear();
    currentIndex = 0;
}
