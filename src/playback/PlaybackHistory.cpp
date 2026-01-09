#include "PlaybackHistory.h"
#include <iostream>

// Limit the history to the last 50 songs to maintain performance 
// since stack operations are O(N)
static const size_t MAX_HISTORY_SIZE = 50;

void PlaybackHistory::pushSong(const Song& song)
{
    std::stack<Song> tempStack;

    // Move songs to a temporary stack to find and remove any existing 
    // instance of the song being added. This ensures no duplicates.
    while (!history.empty())
    {
        Song current = history.top();
        history.pop();

        if (current.id != song.id)
        {
            tempStack.push(current);
        }
    }

    // Check if we exceeded the history limit. Since the temporary stack 
    // is reversed (top is the oldest), popping here removes the oldest entries.
    while (tempStack.size() >= MAX_HISTORY_SIZE)
    {
        tempStack.pop(); 
    }

    // Restore the filtered and trimmed list back to the main history stack
    while (!tempStack.empty())
    {
        history.push(tempStack.top());
        tempStack.pop();
    }

    // Push the new song to the top as the most recent entry
    history.push(song);
}

Song PlaybackHistory::playPreviousSong()
{
    if (history.empty())
    {
        std::cout << "Playback history is empty";
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
    // Create a copy to iterate without modifying the actual history stack
    std::stack<Song> tempStack = history;

    if (tempStack.empty())
    {
        std::cout << "Playback history is empty.\n";
        return;
    }

    std::cout << "--- Recent Playback History (Max " << MAX_HISTORY_SIZE << ") ---\n";

    // Display from top (Most Recent) to bottom (Oldest)
    while (!tempStack.empty())
    {
        const Song& song = tempStack.top();
        std::cout
        << "ID: " << song.id
        << " | Title: " << song.title
        << " | Artist: " << song.artist
        << " | Album: " << song.album
        << " | Duration: " << song.duration << " s"
        << '\n';
        
        tempStack.pop();
    }
}