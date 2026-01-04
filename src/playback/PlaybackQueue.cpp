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
                auto next = std::next(it);
                queue.erase(it);

                current = (next != queue.end()) ? next : queue.begin();
            }
            else
            {
                queue.erase(it);
            }

            break;
        }
    }

    // Reset current if the queue becomes empty
    if (queue.empty())
    {
        current = queue.end();
    }
}

const Song& PlaybackQueue::getCurrentSong()
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

/*
 * Iterates through the entire music library and appends
 * all songs that match the given album name to the queue.
 */
void addAlbumToQueue(const std::string& albumName,
                     const MusicLibrary& library,
                     PlaybackQueue& queue)
{
    const size_t songCount = library.getSongCount();

    for (size_t i = 0; i < songCount; ++i)
    {
        const Song& song = library.getSongByIndex(i);

        if (song.album == albumName)
        {
            queue.addSong(song);
        }
    }
}
