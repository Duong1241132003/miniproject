#include "PlaybackHistory.h"
#include <iostream>
#include <stdexcept>

/* Limit history size to avoid performance issues with stack operations */
static const size_t MAX_HISTORY_SIZE = 200;

void PlaybackHistory::pushSong(const Song& song)
{
    std::stack<Song> tempStack;

    /* Filter out duplicates by moving non-matching songs to temp stack */
    while (!history.empty())
    {
        Song current = history.top();
        history.pop();

        if (current.id != song.id)
        {
            tempStack.push(current);
        }
    }

    /* Enforce size limit by trimming oldest entries from temp stack */
    while (tempStack.size() >= MAX_HISTORY_SIZE)
    {
        tempStack.pop(); 
    }

    /* Restore filtered history back to main stack */
    while (!tempStack.empty())
    {
        history.push(tempStack.top());
        tempStack.pop();
    }

    /* Add new song to the top */
    history.push(song);
}

Song PlaybackHistory::playPreviousSong()
{
    /* Prevent undefined behavior when accessing empty stack */
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

void PlaybackHistory::printHistory() const
{
    /* Use a copy to iterate without modifying the actual stack */
    std::stack<Song> tempStack = history;

    if (tempStack.empty())
    {
        std::cout << "Playback history is empty.\n";
        return;
    }

    std::cout << "--- Recent Playback History (Max " << MAX_HISTORY_SIZE << ") ---\n";

    /* Display from Most Recent (top) to Oldest (bottom) */
    while (!tempStack.empty())
    {
        const Song& song = tempStack.top();
        
        /* Format output for each song entry */
        std::cout << "ID: " << song.id
                  << " | Title: " << song.title
                  << " | Artist: " << song.artist
                  << " | Album: " << song.album
                  << " | Duration: " << song.duration << " s"
                  << '\n';
        
        tempStack.pop();
    }
}