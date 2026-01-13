#include "PlayNextQueue.h"
#include <iostream>

void PlayNextQueue::addSong(const Song& song)
{
    /* Append a new song to the end of the FIFO queue */
    queue.push(song);
}

Song PlayNextQueue::playNext()
{
    /* Notify user if attempting to play from an empty queue */
    if (queue.empty())
    {
        std::cout << "PlayNext queue is empty";
    }

    /* Retrieve the song at the front of the queue */
    Song nextSong = queue.front();
    
    /* Remove the retrieved song from the queue */
    queue.pop();

    return nextSong;
}

bool PlayNextQueue::isEmpty() const
{
    /* Check if there are any songs remaining in the queue */
    return queue.empty();
}

void PlayNextQueue::printAllSongs() const
{
    /* Copy the queue to a temporary container for non-destructive iteration */
    std::queue<Song> tempQueue = queue;

    while (!tempQueue.empty())
    {
        /* Access the current front song for display */
        const Song& song = tempQueue.front();
        
        /* Output formatted song details to the console */
        std::cout << "ID: " << song.id
                  << " | Title: " << song.title
                  << " | Artist: " << song.artist
                  << " | Album: " << song.album
                  << " | Duration: " << song.duration << " s"
                  << '\n';
                  
        /* Remove the processed song from the temporary copy */
        tempQueue.pop();
    }
}