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

    /*
     * Construcor default
     */
    PlaybackQueue() = default;

    /*
     * Copy constructor
     */
    PlaybackQueue(const PlaybackQueue& other);

    /*
     * Copy assignment operator
     */
    PlaybackQueue& operator=(const PlaybackQueue& other);

    /*
     * Get all playbackQueue
     */
    std::list<Song>& getQueue();

    /*
     * Print all songs in the queue
     */
    void printAllSongs() const;
};

/*
 * Adds all songs belonging to a specific album
 * from the music library to the playback queue.
 */
void addAlbumToQueue(std::string& albumName,
                     MusicLibrary& library,
                     PlaybackQueue& queue);

#endif
