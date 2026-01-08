#include "PlaybackQueue.h"
#include <stdexcept>
#include <iostream>

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

PlaybackQueue::PlaybackQueue(const PlaybackQueue& other)
    : queue(other.queue)
{
    if (other.current == other.queue.end())
    {
        current = queue.end();
    }
    else
    {
        current = queue.begin();
    }
}

PlaybackQueue& PlaybackQueue::operator=(const PlaybackQueue& other)
{
    if (this == &other)
    {
        return *this;
    }

    queue = other.queue;

    if (other.current == other.queue.end())
    {
        current = queue.end();
    }
    else
    {
        current = queue.begin();
    }

    return *this;
}

void PlaybackQueue::getAllSongs() const
{
    for (const auto& song : queue)
    {
        std::cout
        << "ID: " << song.id
        << " | Title: " << song.title
        << " | Artist: " << song.artist
        << " | Album: " << song.album
        << " | Duration: " << song.duration << " s"
        << " | Path: " << song.path
        << '\n';

        std::cout << "-----------------------------------\n";
    }
}