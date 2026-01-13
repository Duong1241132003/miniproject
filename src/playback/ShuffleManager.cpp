#include "ShuffleManager.h"
#include <iostream> 
#include <algorithm>
#include <random>

void ShuffleManager::initialize(const std::vector<Song*>& playlist)
{
    shuffledSongs = playlist;
    playedSongIDs.clear();

    /* Initialize Mersenne Twister engine with a hardware random seed */
    std::random_device rd;
    gen = std::mt19937(rd());
}

Song* ShuffleManager::getNextSong()
{
    /* Return nullptr if the source playlist is empty */
    if (shuffledSongs.empty())
    {
        return nullptr;
    }

    /* Return nullptr if all songs in the current cycle have been played */
    if (playedSongIDs.size() == shuffledSongs.size())
    {
        return nullptr;
    }

    /* Define a uniform distribution for valid index range */
    std::uniform_int_distribution<> dist(0, shuffledSongs.size() - 1);

    while (true)
    {
        /* Generate a random index to pick a candidate song */
        int idx = dist(gen);

        /* Extract ID for uniqueness check using the played set */
        int id = shuffledSongs[idx]->id;

        /* Check if the song has already been played in this cycle */
        if (playedSongIDs.find(id) == playedSongIDs.end())
        {
            /* Mark as played and return the song pointer */
            playedSongIDs.insert(id);  
            return shuffledSongs[idx];
        }
    }
}

void ShuffleManager::printAllSongs() const
{
    /* Iterate and print details for all songs in the shuffle list */
    for (const Song* song : shuffledSongs)
    {
       std::cout << "ID: " << song->id
                 << " | Title: " << song->title
                 << " | Artist: " << song->artist
                 << " | Album: " << song->album
                 << " | Duration: " << song->duration << " s"
                 << '\n';
    }
}