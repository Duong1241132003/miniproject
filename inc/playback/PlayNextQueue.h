#ifndef PLAYNEXT_QUEUE_H
#define PLAYNEXT_QUEUE_H

#include <queue>
#include "Song.h"

/*
 * PlaybackQueue
 * -------------
 * Manages songs marked as "Play Next" using FIFO logic.
 */
class PlayNextQueue
{
private:
    /*
    * FIFO queue of upcoming songs.
    */
    std::queue<Song> queue;

public:
    /*
     * Adds a song to the end of the queue.
     */
    void addSong(const Song& song);

    /*
     * Returns the next song to be played.
     * Removes it from the queue.
     */
    Song playNext();

    /*
     * Checks whether the queue is empty.
     */
    bool isEmpty() const;

    /*
     * Print all songs in the queue
     */
    void getAllSongs() const;

};

#endif
