#include "PlaybackHistory.h"
#include <stdexcept>
#include <iostream>
void PlaybackHistory::pushSong(const Song& song)
{
    history.push(song);
}

Song PlaybackHistory::playPreviousSong()
{
    if (history.empty())
    {
        throw std::runtime_error("Playback history is empty");
    }

    Song previousSong = history.top();
    history.pop();

    return previousSong;
}

bool PlaybackHistory::isEmpty() const
{
    return history.empty();
}

void PlaybackHistory::getHistory() const
{
    std::stack<Song> tempStack = history; // Create a copy to iterate through

    while (!tempStack.empty())
    {
        const Song& song = tempStack.top();
        std::cout
        << "ID: " << song.id
        << " | Title: " << song.title
        << " | Artist: " << song.artist
        << " | Album: " << song.album
        << " | Duration: " << song.duration << " s"
        << " | Path: " << song.path
        << '\n';
        tempStack.pop();
    }
}