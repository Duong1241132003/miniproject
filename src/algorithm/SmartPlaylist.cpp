#include "SmartPlaylist.h"

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
