#include "PlaybackQueue.h"
#include <iostream>

void PlaybackQueue::addSong(const Song& song)
{
    /* Avoid duplicates by checking if the song ID already exists in queue */
    for (const auto& s : queue)
    {
        if (s.id == song.id)
        {
            /* Exit if song is already present */
            return;
        }
    }

    queue.push_back(song);

    /* Set the first added song as the current playback entry */
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
            /* Safeguard current iterator if it points to the song being removed */
            if (it == current)
            {
                auto next = std::next(it);
                queue.erase(it);

                /* Point to next available song or wrap around to the beginning */
                current = (next != queue.end()) ? next : queue.begin();
            }
            else
            {
                /* Remove non-active song without affecting current pointer */
                queue.erase(it);
            }

            break;
        }
    }

    /* Invalidate current iterator if the queue is now empty */
    if (queue.empty())
    {
        current = queue.end();
    }
}

const Song& PlaybackQueue::getCurrentSong()
{
    /* Warning message if current song is accessed on empty queue */
    if (queue.empty() || current == queue.end())
    {
        std::cout << "PlaybackQueue: no current song";
    }

    return *current;
}

void PlaybackQueue::playNext()
{
    /* Safety check for empty queue or invalid iterator */
    if (queue.empty() || current == queue.end())
    {
        return;
    }

    ++current;

    /* Loop back to start if current reaches the end of queue */
    if (current == queue.end())
    {
        current = queue.begin();
    }
}

bool PlaybackQueue::isEmpty() const
{
    /* Return queue empty state */
    return queue.empty();
}

/* Batch add all songs from a specific album to the queue */
void addAlbumToQueue(std::string& albumName,
                     MusicLibrary& library,
                     PlaybackQueue& queue)
{
    for (Song& i : library.getSongs())
    {
        if (i.album == albumName)
        {
            queue.addSong(i);
        }
    }
}

PlaybackQueue::PlaybackQueue(const PlaybackQueue& other) : queue(other.queue)
{
    /* Preserve playback position by copying iterator offset */
    if (other.current == other.queue.end())
    {
        current = queue.end();
    }
    else
    {
        size_t offset = 0;

        for (auto it = other.queue.begin(); it != other.current; ++it)
        {
            ++offset;
        }

        current = queue.begin();
        for (size_t i = 0; i < offset; ++i)
        {
            ++current;
        }
    }
}


PlaybackQueue& PlaybackQueue::operator=(const PlaybackQueue& other)
{
    /* Protect against self-assignment */
    if (this == &other)
    {
        return *this;
    }

    queue = other.queue;

    /* Preserve playback position by copying iterator offset */
    if (other.current == other.queue.end())
    {
        current = queue.end();
    }
    else
    {
        size_t offset = 0;

        for (auto it = other.queue.begin(); it != other.current; ++it)
        {
            ++offset;
        }

        current = queue.begin();
        for (size_t i = 0; i < offset; ++i)
        {
            ++current;
        }
    }

    return *this;
}


std::list<Song>& PlaybackQueue::getQueue()
{
    return queue;
}

void PlaybackQueue::printAllSongs() const
{
    /* Iterate and display each song detail in the current queue */
    for (const auto& song : queue)
    {
        std::cout << "ID: " << song.id
                  << " | Title: " << song.title
                  << " | Artist: " << song.artist
                  << " | Album: " << song.album
                  << " | Duration: " << song.duration << " s"
                  << '\n';

        std::cout << "-----------------------------------\n";
    }
}