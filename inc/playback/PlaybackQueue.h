#ifndef PLAYBACK_QUEUE_H
#define PLAYBACK_QUEUE_H

#include <list>
#include <string>
#include "Song.h"
#include "MusicLibrary.h"

/*
 * PlaybackQueue manages the order of songs during playback.
 * It supports frequent insertions and removals at arbitrary positions.
 */
class PlaybackQueue
{
public:
    /*
     * Adds a song to the end of the playback queue.
     */
    void addSong(const Song& song);

    /*
     * Removes a song identified by its ID.
     * This operation does not invalidate iterators except the erased one.
     */
    void removeSongById(int songId);

    /*
     * Returns the currently playing song.
     * Throws an exception if the queue is empty.
     */
    const Song& getCurrentSong();
    /*
     * Advances playback to the next song.
     */
    void playNext();

    /*
     * Checks whether the queue is empty.
     */
    bool isEmpty() const;

private:
    /*
     * std::list is chosen because:
     *  - insert/remove operations are O(1)
     *  - iterators remain valid after modifications
     */
    std::list<Song> queue;

    /*
     * Iterator pointing to the currently playing song.
     */
    std::list<Song>::iterator current = queue.end();
};

/*
 * Adds all songs belonging to a specific album
 * from the music library to the playback queue.
 */
void addAlbumToQueue(const std::string& albumName,
                     const MusicLibrary& library,
                     PlaybackQueue& queue);

#endif
