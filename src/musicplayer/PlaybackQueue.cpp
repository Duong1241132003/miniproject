#include "PlaybackQueue.h"
#include <stdexcept>

void PlaybackQueue::addSong(const Song& song)
{
    queue.push_back(song);

    // If this is the first song, it becomes the current one
    if (queue.size() == 1)
    {
        current = queue.begin();
    }
}

void PlaybackQueue::removeSongById(int songId)
{
    for (auto it = queue.begin(); it != queue.end(); ++it)
    {
        if (it->id == songId)
        {
            // Move current iterator safely if the current song is removed
            if (it == current)
            {
                current = std::next(it);
            }

            queue.erase(it);
            break;
        }
    }

    // Reset current if the queue becomes empty
    if (queue.empty())
    {
        current = queue.end();
    }
}

Song PlaybackQueue::getCurrentSong() const
{
    if (queue.empty() || current == queue.end())
    {
        throw std::runtime_error("PlaybackQueue: no current song");
    }

    return *current;
}

void PlaybackQueue::playNext()
{
    if (queue.empty() || current == queue.end())
    {
        return;
    }

    ++current;

    // Optional behavior: loop back to the beginning
    if (current == queue.end())
    {
        current = queue.begin();
    }
}

bool PlaybackQueue::isEmpty() const
{
    return queue.empty();
}
