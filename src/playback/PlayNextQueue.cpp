#include "PlayNextQueue.h"
#include <iostream>

void PlayNextQueue::addSong(const Song& song)
{
    queue.push(song);
}

Song PlayNextQueue::playNext()
{
    if (queue.empty())
    {
        std::cout << "Playback queue is empty";
    }

    Song nextSong = queue.front();
    
    queue.pop();

    return nextSong;
}

bool PlayNextQueue::isEmpty() const
{
    return queue.empty();
}

void PlayNextQueue::getAllSongs() const
{
    std::queue<Song> tempQueue = queue; // Create a copy to iterate through

    while (!tempQueue.empty())
    {
        const Song& song = tempQueue.front();
        std::cout
        << "ID: " << song.id
        << " | Title: " << song.title
        << " | Artist: " << song.artist
        << " | Album: " << song.album
        << " | Duration: " << song.duration << " s"
        << " | Path: " << song.path
        << '\n';
        tempQueue.pop();
    }
}