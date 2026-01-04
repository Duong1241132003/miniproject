#include "PlayNextQueue.h"
#include <stdexcept>
#include <iostream>

void PlayNextQueue::addSong(const Song& song)
{
    queue.push(song);
}

Song PlayNextQueue::playNext()
{
    std::cout << "Before pop, size = " << queue.size() << "\n";
    if (queue.empty())
    {
        throw std::runtime_error("Playback queue is empty");
    }

    Song nextSong = queue.front();
    
    queue.pop();
    std::cout << "After pop, size = " << queue.size() << "\n";

    return nextSong;
}

bool PlayNextQueue::isEmpty() const
{
    return queue.empty();
}
