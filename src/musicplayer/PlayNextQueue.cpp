#include "PlayNextQueue.h"
#include <stdexcept>

void PlayNextQueue::addSong(const Song& song)
{
    queue.push(song);
}

Song PlayNextQueue::playNext()
{
    if (queue.empty())
    {
        throw std::runtime_error("Playback queue is empty");
    }

    Song nextSong = queue.front();
    queue.pop();

    return nextSong;
}

bool PlayNextQueue::isEmpty() const
{
    return queue.empty();
}
