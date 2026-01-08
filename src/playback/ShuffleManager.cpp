#include "ShuffleManager.h"
#include <iostream> 
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

    if (currentIndex >= shuffledSongs.size())
    {
        return nullptr;   // kết thúc 1 vòng shuffle
    }

    Song* song = shuffledSongs[currentIndex];
    ++currentIndex;
    return song;
}


void ShuffleManager::resetCycle()
{
    playedSongIDs.clear();
    currentIndex = 0;
}

void ShuffleManager::getAllSongs() const
{
    for (const Song* song : shuffledSongs)
    {
       std::cout
        << "ID: " << song -> id
        << " | Title: " << song -> title
        << " | Artist: " << song -> artist
        << " | Album: " << song -> album
        << " | Duration: " << song -> duration << " s"
        << " | Path: " << song -> path
        << '\n';
    }
}